// Copyright LLM Dialogue Plugin Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LLMDialogueComponent.generated.h"

/** Enum defining the possible dialogue states */
UENUM(BlueprintType)
enum class EDialogueState : uint8
{
    Greeting UMETA(DisplayName = "Greeting"),
    Information UMETA(DisplayName = "Information"),
    QuestOffer UMETA(DisplayName = "Quest Offer"),
    Goodbye UMETA(DisplayName = "Goodbye")
};

/** Delegate for when LLM response is received */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLLMResponse, const FString&, Message);

/** Delegate for when dialogue state changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStateChanged, EDialogueState, OldState, EDialogueState, NewState);

/**
 * LLMDialogueComponent handles sending prompts to LLM APIs and processing responses.
 * It can be attached to NPC Actors to enable AI-driven dialogue.
 */
UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class LLMDIALOGUEPLUGIN_API ULLMDialogueComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    ULLMDialogueComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    /** Send a prompt to the LLM API */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue")
    void SendPromptToLLM(const FString& Prompt);
    
    /** Generate a prompt based on the current state and context */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue")
    FString GeneratePrompt(const FString& PlayerInput);
    
    /** Change the current dialogue state */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue")
    void SetDialogueState(EDialogueState NewState);
    
    /** Get the current dialogue state */
    UFUNCTION(BlueprintPure, Category = "LLM Dialogue")
    EDialogueState GetDialogueState() const { return CurrentDialogueState; }
    
    /** Add dialogue history entry */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue")
    void AddToDialogueHistory(const FString& Speaker, const FString& Message);
    
    /** Clear dialogue history */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue")
    void ClearDialogueHistory();
    
    /** Get the complete dialogue history as a string */
    UFUNCTION(BlueprintPure, Category = "LLM Dialogue")
    FString GetDialogueHistoryAsString() const;
    
public:
    /** Event triggered when LLM response is received */
    UPROPERTY(BlueprintAssignable, Category = "LLM Dialogue")
    FOnLLMResponse OnLLMResponse;
    
    /** Event triggered when dialogue state changes */
    UPROPERTY(BlueprintAssignable, Category = "LLM Dialogue")
    FOnDialogueStateChanged OnDialogueStateChanged;
    
    /** The OpenAI API Key */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue", meta = (DisplayName = "API Key"))
    FString APIKey;
    
    /** The model to use for LLM requests (e.g., "gpt-4") */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue", meta = (DisplayName = "Model Name"))
    FString ModelName = TEXT("gpt-4");
    
    /** Temperature setting for the LLM (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Temperature = 0.7f;
    
    /** Maximum number of dialogue history entries to keep */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue", meta = (ClampMin = "1", ClampMax = "50"))
    int32 MaxHistoryEntries = 10;
    
    /** System prompt describing the NPC's personality and behavior */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue", meta = (MultiLine = true))
    FString SystemPrompt = TEXT("You are an NPC in a fantasy RPG game. Speak in character and keep responses concise.");
    
    /** Prompt templates for each dialogue state */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue|Templates", meta = (MultiLine = true))
    FString GreetingTemplate = TEXT("Greet the player in a friendly manner. Include <NEXT_STATE:Information> tag at the end.");
    
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue|Templates", meta = (MultiLine = true))
    FString InformationTemplate = TEXT("Provide information based on the player's question. Use <NEXT_STATE:QuestOffer> if appropriate.");
    
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue|Templates", meta = (MultiLine = true))
    FString QuestOfferTemplate = TEXT("Offer a quest or task to the player. Use <NEXT_STATE:Goodbye> when concluding.");
    
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue|Templates", meta = (MultiLine = true))
    FString GoodbyeTemplate = TEXT("Say farewell to the player in a way fitting your character.");
    
private:
    /** Handle HTTP response from LLM API */
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);
    
    /** Extract text from LLM API response */
    FString ExtractMessageFromResponse(const FString& ResponseString);
    
    /** Determine next dialogue state from response */
    void ParseStateTransition(const FString& ResponseText);
    
    /** Current dialogue state */
    UPROPERTY()
    EDialogueState CurrentDialogueState = EDialogueState::Greeting;
    
    /** Structure to hold dialogue history entries */
    struct FDialogueHistoryEntry
    {
        FString Speaker;
        FString Message;
        FDateTime Timestamp;
    };
    
    /** Dialogue history */
    TArray<FDialogueHistoryEntry> DialogueHistory;
    
    /** Flag to track if a request is in progress */
    bool bRequestInProgress = false;
};