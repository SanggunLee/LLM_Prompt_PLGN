// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "LLMDialogueUIPluginLibrary.h"
#include "Widgets/LLMDialogueWidget.h"
#include "LLMDialogueComponent.h"
#include "Kismet/GameplayStatics.h"

ULLMDialogueWidget* ULLMDialogueUIPluginLibrary::CreateDialogueUI(const UObject* WorldContextObject, ULLMDialogueComponent* DialogueComponent, TSubclassOf<ULLMDialogueWidget> WidgetClass)
{
    if (!WorldContextObject || !DialogueComponent || !WidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateDialogueUI: Invalid parameters"));
        return nullptr;
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), 0);
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateDialogueUI: Failed to get player controller"));
        return nullptr;
    }

    ULLMDialogueWidget* DialogueWidget = CreateWidget<ULLMDialogueWidget>(PlayerController, WidgetClass);
    if (DialogueWidget)
    {
        DialogueWidget->Initialize(DialogueComponent);
        
        // NPC 이름 설정 (대화 컴포넌트의 소유자 이름이나 태그 사용)
        AActor* OwnerActor = DialogueComponent->GetOwner();
        if (OwnerActor)
        {
            DialogueWidget->SetNPCName(OwnerActor->GetActorNameOrLabel());
        }
    }

    return DialogueWidget;
}