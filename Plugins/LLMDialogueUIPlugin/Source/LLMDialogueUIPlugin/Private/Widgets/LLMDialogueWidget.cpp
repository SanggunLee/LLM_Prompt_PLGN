// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "Widgets/LLMDialogueWidget.h"
#include "Widgets/LLMDialogueMessageWidget.h"
#include "Widgets/LLMDialogueInputWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "LLMDialogueComponent.h"

ULLMDialogueWidget::ULLMDialogueWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentNPCName(TEXT("NPC"))
{
}

void ULLMDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InputWidget)
    {
        InputWidget->OnMessageSent.AddDynamic(this, &ULLMDialogueWidget::OnMessageSent);
    }

    // 기본적으로 MessageWidgetClass가 설정되지 않았다면 블루프린트에서 설정해야 함을 로그로 알림
    if (!MessageWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("MessageWidgetClass not set in LLMDialogueWidget. Set this in your Blueprint subclass."));
    }
}

void ULLMDialogueWidget::NativeDestruct()
{
    // 델리게이트 바인딩 해제
    if (DialogueComponent)
    {
        DialogueComponent->OnLLMResponse.RemoveAll(this);
        DialogueComponent->OnDialogueStateChanged.RemoveAll(this);
    }

    if (InputWidget)
    {
        InputWidget->OnMessageSent.RemoveAll(this);
    }

    Super::NativeDestruct();
}

void ULLMDialogueWidget::Initialize(ULLMDialogueComponent* InDialogueComponent)
{
    // 이전 연결 해제
    if (DialogueComponent)
    {
        DialogueComponent->OnLLMResponse.RemoveAll(this);
        DialogueComponent->OnDialogueStateChanged.RemoveAll(this);
    }

    // 새 대화 컴포넌트에 연결
    DialogueComponent = InDialogueComponent;
    
    if (DialogueComponent)
    {
        DialogueComponent->OnLLMResponse.AddDynamic(this, &ULLMDialogueWidget::OnLLMResponseReceived);
        DialogueComponent->OnDialogueStateChanged.AddDynamic(this, &ULLMDialogueWidget::OnDialogueStateChanged);
        
        // 대화 내용 초기화
        ClearMessages();
    }
}

void ULLMDialogueWidget::SetNPCName(const FString& Name)
{
    CurrentNPCName = Name;
    
    if (NPCNameText)
    {
        NPCNameText->SetText(FText::FromString(Name));
    }
}

void ULLMDialogueWidget::AddMessage(const FString& SpeakerName, const FString& MessageText, bool bIsNPC)
{
    if (!MessageWidgetClass || !MessageScrollBox)
    {
        return;
    }

    // 메시지 위젯 생성
    ULLMDialogueMessageWidget* MessageWidget = CreateWidget<ULLMDialogueMessageWidget>(this, MessageWidgetClass);
    if (MessageWidget)
    {
        MessageWidget->SetMessageContent(SpeakerName, MessageText);
        MessageWidget->SetIsNPCMessage(bIsNPC);
        
        // 스크롤 박스에 추가
        MessageScrollBox->AddChild(MessageWidget);
        
        // 아래로 스크롤
        ScrollToBottom();
    }
}

void ULLMDialogueWidget::ClearMessages()
{
    if (MessageScrollBox)
    {
        MessageScrollBox->ClearChildren();
    }
}

void ULLMDialogueWidget::SetDialogueVisible(bool bVisible)
{
    if (DialogueBorder)
    {
        DialogueBorder->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (bVisible && InputWidget)
    {
        // 다이얼로그가 표시될 때 입력 필드에 포커스
        InputWidget->SetInputEnabled(true);
    }
}

void ULLMDialogueWidget::OnMessageSent(const FString& Message)
{
    // 플레이어 메시지 추가
    AddMessage(PlayerName, Message, false);
    
    // 입력 비활성화 (응답 대기 중)
    if (InputWidget)
    {
        InputWidget->SetInputEnabled(false);
    }
    
    // LLM에 메시지 전송
    if (DialogueComponent)
    {
        DialogueComponent->SendPromptToLLM(DialogueComponent->GeneratePrompt(Message));
    }
}

void ULLMDialogueWidget::OnLLMResponseReceived(const FString& Message)
{
    // NPC 응답 메시지 추가
    AddMessage(CurrentNPCName, Message, true);
    
    // 입력 활성화
    if (InputWidget)
    {
        InputWidget->SetInputEnabled(true);
    }
}

void ULLMDialogueWidget::OnDialogueStateChanged(EDialogueState OldState, EDialogueState NewState)
{
    // 대화 상태 변경 처리 (예: 다른 스타일이나 배경으로 표시)
    UE_LOG(LogTemp, Log, TEXT("Dialogue state changed from %s to %s"), 
        *UEnum::GetValueAsString(OldState), 
        *UEnum::GetValueAsString(NewState));
}

void ULLMDialogueWidget::ScrollToBottom()
{
    if (MessageScrollBox)
    {
        // 약간 지연을 두고 스크롤 - 콘텐츠 크기가 업데이트될 시간을 줌
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            if (MessageScrollBox)
            {
                MessageScrollBox->ScrollToEnd();
            }
        });
    }
}