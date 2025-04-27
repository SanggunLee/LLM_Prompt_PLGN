// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "Widgets/LLMDialogueInputWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

ULLMDialogueInputWidget::ULLMDialogueInputWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULLMDialogueInputWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SendButton)
    {
        SendButton->OnClicked.AddDynamic(this, &ULLMDialogueInputWidget::OnSendButtonClicked);
    }

    if (InputTextBox)
    {
        InputTextBox->OnTextCommitted.AddDynamic(this, &ULLMDialogueInputWidget::OnInputCommitted);
    }
}

void ULLMDialogueInputWidget::OnSendButtonClicked()
{
    if (InputTextBox)
    {
        FString Message = InputTextBox->GetText().ToString();
        if (!Message.IsEmpty())
        {
            OnMessageSent.Broadcast(Message);
            ClearInput();
        }
    }
}

void ULLMDialogueInputWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter)
    {
        FString Message = Text.ToString();
        if (!Message.IsEmpty())
        {
            OnMessageSent.Broadcast(Message);
            ClearInput();
        }
    }
}

void ULLMDialogueInputWidget::SetInputEnabled(bool bEnabled)
{
    if (InputTextBox)
    {
        InputTextBox->SetIsEnabled(bEnabled);
    }

    if (SendButton)
    {
        SendButton->SetIsEnabled(bEnabled);
    }
}

void ULLMDialogueInputWidget::ClearInput()
{
    if (InputTextBox)
    {
        InputTextBox->SetText(FText::GetEmpty());
    }
}