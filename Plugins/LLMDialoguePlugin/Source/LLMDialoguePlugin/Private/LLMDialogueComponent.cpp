// Copyright LLM Dialogue Plugin Team. All Rights Reserved.

#include "LLMDialogueComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"

// Define a log category for this module
DEFINE_LOG_CATEGORY_STATIC(LogLLMDialogue, Log, All);

ULLMDialogueComponent::ULLMDialogueComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULLMDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Ensure HTTP module is loaded
    FHttpModule::Get();
    
    UE_LOG(LogLLMDialogue, Log, TEXT("LLM Dialogue Component initialized on %s"), *GetOwner()->GetName());
}

void ULLMDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void ULLMDialogueComponent::SendPromptToLLM(const FString& Prompt)
{
    if (Prompt.IsEmpty())
    {
        UE_LOG(LogLLMDialogue, Warning, TEXT("Cannot send empty prompt to LLM"));
        return;
    }
    
    if (APIKey.IsEmpty())
    {
        UE_LOG(LogLLMDialogue, Error, TEXT("No API Key provided for LLM API"));
        return;
    }
    
    if (bRequestInProgress)
    {
        UE_LOG(LogLLMDialogue, Warning, TEXT("Request already in progress, ignoring new prompt"));
        return;
    }
    
    bRequestInProgress = true;
    
    // Add player input to dialogue history
    AddToDialogueHistory(TEXT("Player"), Prompt);
    
    // Create HTTP request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey));
    
    // Create JSON payload
    TSharedPtr<FJsonObject> JsonPayload = MakeShareable(new FJsonObject);
    JsonPayload->SetStringField(TEXT("model"), ModelName);
    JsonPayload->SetNumberField(TEXT("temperature"), Temperature);
    
    // Create messages array
    TArray<TSharedPtr<FJsonValue>> MessagesArray;
    
    // Add system message
    TSharedPtr<FJsonObject> SystemMessageObj = MakeShareable(new FJsonObject);
    SystemMessageObj->SetStringField(TEXT("role"), TEXT("system"));
    SystemMessageObj->SetStringField(TEXT("content"), SystemPrompt);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(SystemMessageObj)));
    
    // Add dialogue history (limited)
    int32 HistoryToInclude = FMath::Min(DialogueHistory.Num() - 1, MaxHistoryEntries);
    for (int32 i = DialogueHistory.Num() - HistoryToInclude - 1; i < DialogueHistory.Num() - 1; i++)
    {
        if (i >= 0)
        {
            TSharedPtr<FJsonObject> HistoryMessageObj = MakeShareable(new FJsonObject);
            
            // Set role based on speaker
            FString Role = DialogueHistory[i].Speaker.Equals(TEXT("Player")) ? TEXT("user") : TEXT("assistant");
            HistoryMessageObj->SetStringField(TEXT("role"), Role);
            HistoryMessageObj->SetStringField(TEXT("content"), DialogueHistory[i].Message);
            MessagesArray.Add(MakeShareable(new FJsonValueObject(HistoryMessageObj)));
        }
    }
    
    // Add current prompt with appropriate template based on state
    TSharedPtr<FJsonObject> UserMessageObj = MakeShareable(new FJsonObject);
    UserMessageObj->SetStringField(TEXT("role"), TEXT("user"));
    
    FString TemplatedPrompt;
    switch (CurrentDialogueState)
    {
        case EDialogueState::Greeting:
            TemplatedPrompt = GreetingTemplate;
            break;
        case EDialogueState::Information:
            TemplatedPrompt = InformationTemplate;
            break;
        case EDialogueState::QuestOffer:
            TemplatedPrompt = QuestOfferTemplate;
            break;
        case EDialogueState::Goodbye:
            TemplatedPrompt = GoodbyeTemplate;
            break;
    }
    
    // Replace placeholder with actual player input if needed
    TemplatedPrompt = TemplatedPrompt.Replace(TEXT("{PlayerInput}"), *Prompt);
    UserMessageObj->SetStringField(TEXT("content"), TemplatedPrompt);
    MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessageObj)));
    
    // Add messages array to payload
    JsonPayload->SetArrayField(TEXT("messages"), MessagesArray);
    
    // Serialize payload to string
    FString RequestString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&RequestString);
    FJsonSerializer::Serialize(JsonPayload.ToSharedRef(), JsonWriter);
    
    // Set request payload
    HttpRequest->SetContentAsString(RequestString);
    
    // Set completion callback
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULLMDialogueComponent::OnResponseReceived);
    
    // Send request
    HttpRequest->ProcessRequest();
    
    UE_LOG(LogLLMDialogue, Log, TEXT("Sent LLM request for state: %s"), *UEnum::GetValueAsString(CurrentDialogueState));
}

void ULLMDialogueComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
{
    bRequestInProgress = false;
    
    if (!bSucceeded || !Response.IsValid())
    {
        UE_LOG(LogLLMDialogue, Error, TEXT("LLM API request failed"));
        return;
    }
    
    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        UE_LOG(LogLLMDialogue, Error, TEXT("LLM API returned error code: %d, Response: %s"), 
            ResponseCode, *Response->GetContentAsString());
        return;
    }
    
    FString ResponseString = Response->GetContentAsString();
    FString Message = ExtractMessageFromResponse(ResponseString);
    
    if (!Message.IsEmpty())
    {
        // Add NPC response to dialogue history
        AddToDialogueHistory(TEXT("NPC"), Message);
        
        // Check for state transition tags
        ParseStateTransition(Message);
        
        // Clean up potential state tags from message before displaying
        static const FRegexPattern StateTagPattern(TEXT("<NEXT_STATE:\\w+>"));
        FRegexMatcher Matcher(StateTagPattern, Message);
        while (Matcher.FindNext())
        {
            Message.RemoveAt(Matcher.GetMatchBeginning(), Matcher.GetMatchEnding() - Matcher.GetMatchBeginning());
            Matcher.SetLimits(0, Message.Len());
        }
        
        // Broadcast response
        OnLLMResponse.Broadcast(Message);
        
        UE_LOG(LogLLMDialogue, Log, TEXT("LLM Response received, current state: %s"), 
            *UEnum::GetValueAsString(CurrentDialogueState));
    }
    else
    {
        UE_LOG(LogLLMDialogue, Error, TEXT("Failed to extract message from LLM response"));
    }
}

FString ULLMDialogueComponent::ExtractMessageFromResponse(const FString& ResponseString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
    
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
    {
        // Extract the response text from the OpenAI JSON format
        const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
        if (JsonObject->TryGetArrayField(TEXT("choices"), ChoicesArray))
        {
            if (ChoicesArray->Num() > 0)
            {
                const TSharedPtr<FJsonObject>* ChoiceObject;
                if ((*ChoicesArray)[0]->TryGetObject(ChoiceObject))
                {
                    const TSharedPtr<FJsonObject>* MessageObject;
                    if ((*ChoiceObject)->TryGetObjectField(TEXT("message"), MessageObject))
                    {
                        FString Content;
                        if ((*MessageObject)->TryGetStringField(TEXT("content"), Content))
                        {
                            return Content;
                        }
                    }
                }
            }
        }
    }
    
    return FString();
}

void ULLMDialogueComponent::ParseStateTransition(const FString& ResponseText)
{
    // Method 1: Look for explicit tags <NEXT_STATE:StateName>
    static const FRegexPattern StateTagPattern(TEXT("<NEXT_STATE:(\\w+)>"));
    FRegexMatcher Matcher(StateTagPattern, ResponseText);
    
    if (Matcher.FindNext())
    {
        FString NextState = ResponseText.Mid(Matcher.GetCaptureGroupBeginning(1), 
                                           Matcher.GetCaptureGroupEnding(1) - Matcher.GetCaptureGroupBeginning(1));
        
        // Convert string to enum
        if (NextState.Equals(TEXT("Greeting"), ESearchCase::IgnoreCase))
        {
            SetDialogueState(EDialogueState::Greeting);
        }
        else if (NextState.Equals(TEXT("Information"), ESearchCase::IgnoreCase))
        {
            SetDialogueState(EDialogueState::Information);
        }
        else if (NextState.Equals(TEXT("QuestOffer"), ESearchCase::IgnoreCase))
        {
            SetDialogueState(EDialogueState::QuestOffer);
        }
        else if (NextState.Equals(TEXT("Goodbye"), ESearchCase::IgnoreCase))
        {
            SetDialogueState(EDialogueState::Goodbye);
        }
        
        return;
    }
    
    // Method 2: Fallback to keyword matching if no explicit tag found
    if (ResponseText.Contains(TEXT("quest"), ESearchCase::IgnoreCase) || 
        ResponseText.Contains(TEXT("task"), ESearchCase::IgnoreCase) || 
        ResponseText.Contains(TEXT("mission"), ESearchCase::IgnoreCase))
    {
        SetDialogueState(EDialogueState::QuestOffer);
    }
    else if (ResponseText.Contains(TEXT("goodbye"), ESearchCase::IgnoreCase) || 
             ResponseText.Contains(TEXT("farewell"), ESearchCase::IgnoreCase) || 
             ResponseText.Contains(TEXT("bye"), ESearchCase::IgnoreCase))
    {
        SetDialogueState(EDialogueState::Goodbye);
    }
    else if (CurrentDialogueState == EDialogueState::Greeting)
    {
        // Transition from greeting to information by default
        SetDialogueState(EDialogueState::Information);
    }
}

void ULLMDialogueComponent::SetDialogueState(EDialogueState NewState)
{
    if (NewState != CurrentDialogueState)
    {
        EDialogueState OldState = CurrentDialogueState;
        CurrentDialogueState = NewState;
        
        OnDialogueStateChanged.Broadcast(OldState, NewState);
        
        UE_LOG(LogLLMDialogue, Log, TEXT("Dialogue state changed from %s to %s"), 
            *UEnum::GetValueAsString(OldState), *UEnum::GetValueAsString(NewState));
    }
}

FString ULLMDialogueComponent::GeneratePrompt(const FString& PlayerInput)
{
    // This method allows for custom prompt generation based on the current state
    // For now, we're just returning the player input, but this could be expanded
    return PlayerInput;
}

void ULLMDialogueComponent::AddToDialogueHistory(const FString& Speaker, const FString& Message)
{
    FDialogueHistoryEntry NewEntry;
    NewEntry.Speaker = Speaker;
    NewEntry.Message = Message;
    NewEntry.Timestamp = FDateTime::Now();
    
    DialogueHistory.Add(NewEntry);
    
    // Trim history if it exceeds the maximum
    while (DialogueHistory.Num() > MaxHistoryEntries)
    {
        DialogueHistory.RemoveAt(0);
    }
}

void ULLMDialogueComponent::ClearDialogueHistory()
{
    DialogueHistory.Empty();
}

FString ULLMDialogueComponent::GetDialogueHistoryAsString() const
{
    FString HistoryString;
    
    for (const FDialogueHistoryEntry& Entry : DialogueHistory)
    {
        HistoryString += FString::Printf(TEXT("%s: %s\n"), 
            *Entry.Speaker, *Entry.Message);
    }
    
    return HistoryString;
}