// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLMDialogueComponent.h" // LLMDialoguePlugin의 컴포넌트 의존성
#include "LLMDialogueWidget.generated.h"

class ULLMDialogueMessageWidget;
class ULLMDialogueInputWidget;
class UScrollBox;
class UTextBlock;
class UBorder;

/**
 * Main dialogue widget for LLM-powered NPC conversations
 */
UCLASS()
class LLMDIALOGUEUIPLUGIN_API ULLMDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    ULLMDialogueWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    /** Initialize the dialogue widget with a dialogue component */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void Initialize(ULLMDialogueComponent* InDialogueComponent);

    /** Set the NPC name displayed in the dialogue */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void SetNPCName(const FString& Name);

    /** Add a new message to the dialogue */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void AddMessage(const FString& SpeakerName, const FString& MessageText, bool bIsNPC);

    /** Clear all messages from the dialogue */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void ClearMessages();

    /** Show or hide the dialogue UI */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void SetDialogueVisible(bool bVisible);

protected:
    /** Called when a new message is sent by the player */
    UFUNCTION()
    void OnMessageSent(const FString& Message);

    /** Called when an LLM response is received */
    UFUNCTION()
    void OnLLMResponseReceived(const FString& Message);

    /** Called when dialogue state changes */
    UFUNCTION()
    void OnDialogueStateChanged(EDialogueState OldState, EDialogueState NewState);

    /** Scroll the message view to the bottom */
    UFUNCTION()
    void ScrollToBottom();

protected:
    /** Container for messages */
    UPROPERTY(meta = (BindWidget))
    UScrollBox* MessageScrollBox;

    /** Text block for displaying NPC name */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NPCNameText;

    /** Input widget for player messages */
    UPROPERTY(meta = (BindWidget))
    ULLMDialogueInputWidget* InputWidget;

    /** Main border for the whole dialogue UI */
    UPROPERTY(meta = (BindWidget))
    UBorder* DialogueBorder;

    /** Message widget class to spawn for new messages */
    UPROPERTY(EditDefaultsOnly, Category = "LLM Dialogue UI")
    TSubclassOf<ULLMDialogueMessageWidget> MessageWidgetClass;

    /** The dialogue component this widget is linked to */
    UPROPERTY()
    ULLMDialogueComponent* DialogueComponent;

    /** Current NPC name */
    UPROPERTY()
    FString CurrentNPCName;

    /** Whether to show loading indicator */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue UI")
    bool bShowLoadingIndicator = true;

    /** Player name to use in the dialogue */
    UPROPERTY(EditAnywhere, Category = "LLM Dialogue UI")
    FString PlayerName = TEXT("Player");
};