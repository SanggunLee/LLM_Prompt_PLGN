// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLMDialogueMessageWidget.generated.h"

class UTextBlock;
class URichTextBlock;

/**
 * Widget for displaying a single message in the dialogue conversation
 */
UCLASS()
class LLMDIALOGUEUIPLUGIN_API ULLMDialogueMessageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    ULLMDialogueMessageWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativePreConstruct() override;

    /** Set the message content */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void SetMessageContent(const FString& SpeakerName, const FString& MessageText);

    /** Set whether this message is from an NPC (affects styling) */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI")
    void SetIsNPCMessage(bool bInIsNPC);

protected:
    /** Name of the speaker */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SpeakerNameText;

    /** Text of the message */
    UPROPERTY(meta = (BindWidget))
    URichTextBlock* MessageText;

    /** Background for the message */
    UPROPERTY(meta = (BindWidget))
    UWidget* MessageBackground;

    /** Whether this message is from an NPC */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Dialogue UI")
    bool bIsNPC;

    /** Style color for player messages */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Dialogue UI")
    FLinearColor PlayerMessageColor = FLinearColor(0.5f, 0.7f, 1.0f, 1.0f);

    /** Style color for NPC messages */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Dialogue UI")
    FLinearColor NPCMessageColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
};