// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "LLMDialogueInputWidget.generated.h"

/** Delegate for when a message is sent */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageSent, const FString&, Message);

/**
 * Widget for entering dialogue messages
 */
UCLASS()
class LLMDIALOGUEUIPLUGIN_API ULLMDialogueInputWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    ULLMDialogueInputWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

    /** Event triggered when a message is sent */
    UPROPERTY(BlueprintAssignable, Category = "LLM Dialogue UI")
    FOnMessageSent OnMessageSent;

    /** Enable or disable input */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void SetInputEnabled(bool bEnabled);

    /** Clear the input field */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void ClearInput();

protected:
    /** Called when the send button is clicked */
    UFUNCTION()
    void OnSendButtonClicked();

    /** Called when Enter is pressed in the input field */
    UFUNCTION()
    void OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    /** Text input field */
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* InputTextBox;

    /** Send button */
    UPROPERTY(meta = (BindWidget))
    UButton* SendButton;
};