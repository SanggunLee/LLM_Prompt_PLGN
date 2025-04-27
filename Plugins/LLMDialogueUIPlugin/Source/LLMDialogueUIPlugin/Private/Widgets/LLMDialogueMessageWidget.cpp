// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "Widgets/LLMDialogueMessageWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/Border.h"

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
        MessageBackground->SetBrushColor(BackgroundColor);
    }
}

void ULLMDialogueMessageWidget::SetMessageContent(const FString& SpeakerName, const FString& Message)
{
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(FText::FromString(SpeakerName));
    }

    if (MessageText)
    {
        MessageText->SetText(FText::FromString(Message));
    }
}

void ULLMDialogueMessageWidget::SetIsNPCMessage(bool bInIsNPC)
{
    bIsNPC = bInIsNPC;

    if (MessageBackground)
    {
        FLinearColor BackgroundColor = bIsNPC ? NPCMessageColor : PlayerMessageColor;
        MessageBackground->SetBrushColor(BackgroundColor);
    }
}