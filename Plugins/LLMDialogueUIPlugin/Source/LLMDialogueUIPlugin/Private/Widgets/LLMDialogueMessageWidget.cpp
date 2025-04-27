// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "Widgets/LLMDialogueMessageWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"

ULLMDialogueMessageWidget::ULLMDialogueMessageWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsNPC(false)
{
}

void ULLMDialogueMessageWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    // Apply styling based on message type
    if (MessageBackground)
    {
        FLinearColor BackgroundColor = bIsNPC ? NPCMessageColor : PlayerMessageColor;
        MessageBackground->SetColorAndOpacity(BackgroundColor);
    }
}

void ULLMDialogueMessageWidget::SetMessageContent(const FString& SpeakerName, const FString& MessageText)
{
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(FText::FromString(SpeakerName));
    }

    if (this->MessageText)
    {
        this->MessageText->SetText(FText::FromString(MessageText));
    }
}

void ULLMDialogueMessageWidget::SetIsNPCMessage(bool bInIsNPC)
{
    bIsNPC = bInIsNPC;

    if (MessageBackground)
    {
        FLinearColor BackgroundColor = bIsNPC ? NPCMessageColor : PlayerMessageColor;
        MessageBackground->SetColorAndOpacity(BackgroundColor);
    }
}