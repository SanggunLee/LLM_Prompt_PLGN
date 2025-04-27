# LLMDialogueUIPlugin 사용 가이드

이 문서는 LLMDialogueUIPlugin을 사용하여 NPC와 대화하는 UI 시스템을 구현하는 방법을 설명합니다.

## 플러그인 설정

1. **플러그인 활성화**:
   - 언리얼 에디터에서 Edit > Plugins 메뉴로 이동합니다.
   - LLMDialoguePlugin과 LLMDialogueUIPlugin을 모두 활성화합니다.
   - 에디터를 재시작합니다.

2. **종속성 참고사항**:
   - LLMDialogueUIPlugin은 LLMDialoguePlugin을 필요로 합니다.
   - 두 플러그인이 모두 활성화되어 있어야 합니다.

## 위젯 블루프린트 생성

1. **메시지 위젯 생성**:
   - Content Browser에서 우클릭 > User Interface > Widget Blueprint
   - 이름: "BP_LLMDialogueMessageWidget"
   - Parent Class: "LLMDialogueMessageWidget"
   - 디자인:
     ```
     - Border (이름: MessageBackground)
       - VerticalBox
         - TextBlock (이름: SpeakerNameText)
         - RichTextBlock (이름: MessageText)
     ```

2. **입력 위젯 생성**:
   - Content Browser에서 우클릭 > User Interface > Widget Blueprint
   - 이름: "BP_LLMDialogueInputWidget"
   - Parent Class: "LLMDialogueInputWidget"
   - 디자인:
     ```
     - HorizontalBox
       - EditableTextBox (이름: InputTextBox)
       - Button (이름: SendButton, 텍스트: "전송")
     ```

3. **대화 위젯 생성**:
   - Content Browser에서 우클릭 > User Interface > Widget Blueprint
   - 이름: "BP_LLMDialogueWidget"
   - Parent Class: "LLMDialogueWidget"
   - 디자인:
     ```
     - Border (이름: DialogueBorder)
       - VerticalBox
         - TextBlock (이름: NPCNameText)
         - ScrollBox (이름: MessageScrollBox)
         - BP_LLMDialogueInputWidget (이름: InputWidget)
     ```
   - **중요**: 디폴트 탭에서 "LLM Dialogue UI" 카테고리의 "Message Widget Class" 값을 BP_LLMDialogueMessageWidget으로 설정합니다.

## NPC에 대화 시스템 추가하기

1. **NPC 블루프린트 생성**:
   - Content Browser에서 우클릭 > Blueprint Class > Character
   - 이름: "BP_DialogueNPC"
   - 컴포넌트 추가:
     * LLMDialogueComponent
     * Details 패널에서:
       - API Key 설정
       - System Prompt 설정
       - 기타 필요한 설정

2. **클릭으로 대화 시작하기 위한 설정**:
   ```
   // BP_DialogueNPC 블루프린트에서:

   // 변수 선언
   - DialogueWidget (LLMDialogueWidget 타입)
   - bIsInDialogue (Boolean 타입, 기본값 false)

   // Event Begin Play에서
   - Enable Click Events

   // Event OnClicked 생성
   - Branch (Condition: bIsInDialogue)
   - False인 경우 → StartDialogue 함수 호출
   - True인 경우 → EndDialogue 함수 호출

   // StartDialogue 함수 생성
   - LLMDialogueUIPluginLibrary.CreateDialogueUI 호출
     (파라미터: Self, Self->LLMDialogueComponent, BP_LLMDialogueWidget Class)
   - 반환된 위젯을 DialogueWidget 변수에 저장
   - DialogueWidget->AddToViewport()
   - DialogueWidget->SetDialogueVisible(true)
   - Set Input Mode UI Only
   - Show Mouse Cursor = true
   - bIsInDialogue = true

   // EndDialogue 함수 생성
   - DialogueWidget->RemoveFromParent()
   - Set Input Mode Game Only
   - Show Mouse Cursor = false
   - bIsInDialogue = false
   ```

## 플레이어 컨트롤러 설정

1. **플레이어 블루프린트 수정**:
   ```
   // PlayerCharacter 블루프린트에서:

   // Event Begin Play에서
   - Enable Mouse Over Events

   // ESC 키 입력 처리
   - Input Action Mapping: "ExitDialogue" = Escape
   - Event ExitDialogue에서:
     * Get All Actors Of Class (BP_DialogueNPC)
     * ForEach 루프:
       - Cast To BP_DialogueNPC
       - 변수 bIsInDialogue가 true인지 확인
       - true면 EndDialogue 함수 호출
   ```

## C++ 코드에서 사용하기

```cpp
#include "LLMDialogueUIPluginIncludes.h"

// NPC에 대화 시스템 추가
ULLMDialogueWidget* DialogueUI = ULLMDialogueUIPluginLibrary::CreateDialogueUI(
    this, 
    NPCActor->FindComponentByClass<ULLMDialogueComponent>(), 
    BP_LLMDialogueWidgetClass
);

// 화면에 표시
if (DialogueUI)
{
    DialogueUI->AddToViewport();
    DialogueUI->SetDialogueVisible(true);
}
```

## 테스트하기

1. **테스트 맵 생성**:
   - 새 레벨 생성
   - BP_DialogueNPC 인스턴스 배치
   - 플레이어 시작 위치 설정
   - 게임 플레이 테스트

2. **대화 시스템 작동 방식**:
   - NPC에 마우스를 올리면 커서가 변경됩니다 (클릭 가능)
   - 클릭하면 대화 UI가 나타납니다
   - 메시지를 입력하고 전송하면 LLMDialogueComponent가 API로 요청을 보냅니다
   - 응답이 오면 대화 UI에 표시됩니다
   - ESC 키나 NPC 재클릭으로 대화를 종료할 수 있습니다

## 추가 커스터마이징

1. **위젯 스타일 변경**:
   - BP_LLMDialogueMessageWidget에서 PlayerMessageColor, NPCMessageColor를 수정하여 대화 색상 변경
   - BP_LLMDialogueWidget에서 배경, 폰트, 여백 등 조정

2. **대화 상태에 따른 UI 변경**:
   - BP_LLMDialogueWidget의 OnDialogueStateChanged 함수를 오버라이드하여 상태별 스타일 적용

3. **NPC 상호작용 확장**:
   - 거리 기반 상호작용 추가
   - 시선 방향으로 NPC 회전 기능 추가
   - 대화 중 NPC 애니메이션 또는 시각 효과 추가