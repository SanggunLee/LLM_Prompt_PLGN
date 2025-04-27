
# 이제 플러그인 개발이 완료되었습니다. 다음은 사용 방법에 대한 안내입니다:  

  1. 플러그인 컴파일 및 빌드 방법:  
    - 언리얼 에디터를 재시작하면 새 플러그인이 로드됩니다.  
    - 또는 UnrealBuildTool을 사용하여 컴파일할 수 있습니다.  
  2. 블루프린트 위젯 생성 단계:  
    - 각 C++ 위젯 클래스를 상속받는 블루프린트 위젯을 생성합니다.  
    - ULLMDialogueMessageWidget을 상속받는 BP_LLMDialogueMessageWidget 생성  
    - ULLMDialogueInputWidget을 상속받는 BP_LLMDialogueInputWidget 생성 
    - ULLMDialogueWidget을 상속받는 BP_LLMDialogueWidget 생성  
    - 각 위젯의 디자인을 블루프린트 에디터에서 구성합니다. 
  3. 게임에서 사용하는 방법:
```
  // C++ 코드에서  
  #include "LLMDialogueUIPluginIncludes.h"  

  // 위젯 생성
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

  3. 블루프린트에서는:  
    a. Create Dialogue UI 노드 사용  
    b. Add to Viewport 호출  
    c. Set Dialogue Visible(true) 호출  

  이 플러그인은 기존 LLMDialoguePlugin을 확장하면서 UI 기능을 추가합니다. 종속성은 Build.cs 파일과 uplugin 파일에 올바르게 설정되었습니다.  

 ##  > 프로젝트에서 이 플러그인을 사용하려면 LLMDialoguePlugin 플러그인도 프로젝트에 포함 시켜야 하나?  

● 네, 그렇습니다. LLMDialogueUIPlugin은 LLMDialoguePlugin에 의존하기 때문에 프로젝트에서 이 UI 플러그인을 사용하려면 반드시 LLMDialoguePlugin도 함께 포함시켜야 합니다.  

  이유는 다음과 같습니다:  

  1. 의존성 설정: LLMDialogueUIPlugin.uplugin 파일에 명시적으로 LLMDialoguePlugin을 의존성으로 선언했습니다.
```
  "Plugins": [  
      {
          "Name": "LLMDialoguePlugin",  
          "Enabled": true  
      }  
  ]
```

  2. 코드 참조: LLMDialogueUIPlugin의 코드가 LLMDialoguePlugin의 클래스들(예: ULLMDialogueComponent)을 직접 참조하고 있습니다.  
  3. 빌드 의존성: LLMDialogueUIPlugin.Build.cs 파일에서 "LLMDialoguePlugin"을 명시적으로 의존성 모듈로 추가했습니다.  

  언리얼 엔진에서는 두 플러그인을 모두 활성화해야 하며, 이 플러그인을 다른 프로젝트에서 사용하려면 두 플러그인을 모두 복사하거나 패키징해야 합니다. UI 플러그인이 대화 기능을 제공하는 메인  플러그인의 컴포넌트와 통신하는 방식으로 설계되었기 때문입니다.  
 
##  > 너가 만들어준 플러그인을 사용해서 UI 위젯을 만들어서 npc 케릭터와 대화하는 방법을 설명해줘  

### LLMDialogueUIPlugin을 사용하여 NPC 캐릭터와 대화하는 방법을 단계별로 설명해 드리겠습니다:   

###  1. 플러그인 활성화    
 
  1. 언리얼 에디터를 실행하고 프로젝트를 엽니다.  
  2. Edit > Plugins 메뉴로 이동합니다.  
  3. LLMDialoguePlugin과 LLMDialogueUIPlugin을 모두 활성화합니다.  
  4. 에디터를 재시작하라는 메시지가 나타나면 재시작합니다.  

###  2. 블루프린트 위젯 생성  

  1. 메시지 위젯 생성:  
    - Content Browser에서 우클릭 > User Interface > Widget Blueprint 선택  
    - 이름을 "BP_LLMDialogueMessageWidget"으로 지정  
    - Parent Class로 "LLMDialogueMessageWidget" 선택  
    - 디자이너에서 위젯 계층구조 생성:  
    - Border (MessageBackground로 이름 지정)  
    - VerticalBox  
      - TextBlock (SpeakerNameText로 이름 지정)  
      - RichTextBlock (MessageText로 이름 지정)  
  2. 입력 위젯 생성:  
    - 새 Widget Blueprint 생성, 이름을 "BP_LLMDialogueInputWidget"로 지정  
    - Parent Class로 "LLMDialogueInputWidget" 선택  
    - 디자이너에서 위젯 계층구조 생성:  
    - HorizontalBox  
    - EditableTextBox (InputTextBox로 이름 지정, Hint Text는 "메시지 입력...")  
    - Button (SendButton로 이름 지정, 텍스트는 "전송")  
  3. 메인 대화 위젯 생성:  
    - 새 Widget Blueprint 생성, 이름을 "BP_LLMDialogueWidget"으로 지정  
    - Parent Class로 "LLMDialogueWidget" 선택  
    - 디자이너에서 위젯 계층구조 생성:  
    - Border (DialogueBorder로 이름 지정)  
    - VerticalBox  
      - TextBlock (NPCNameText로 이름 지정)  
      - ScrollBox (MessageScrollBox로 이름 지정)  
      - BP_LLMDialogueInputWidget (InputWidget으로 이름 지정, 방금 만든 위젯 추가)  
    - 이벤트 그래프에서 아래와 같이 설정:  
        - Event Construct에서 MessageWidgetClass 변수를 BP_LLMDialogueMessageWidget 클래스로 설정  

###  3. NPC 설정하기  

  1. NPC 블루프린트 생성:  
    - Content Browser에서 우클릭 > Blueprint Class > Character 선택  
    - 이름을 "BP_DialogueNPC"로 지정  
    - 컴포넌트 추가:  
        - LLMDialogueComponent 추가 (기존 LLMDialoguePlugin의 컴포넌트)  
      - Component 탭에서 LLMDialogueComponent 선택 후 Details 패널에서:  
            - API Key 설정  
        - System Prompt 설정 (NPC 성격 설정)  
        - 필요한 기타 설정 적용  
  2. NPC 상호작용 설정:  
    - BP_DialogueNPC 블루프린트의 이벤트 그래프에서:  
    * Event BeginPlay  
  * 변수 선언: DialogueWidget (LLMDialogueWidget 타입)  

  * 상호작용 함수 생성 (예: StartDialogue)  
  * 함수 내부:  
    - LLMDialogueUIPluginLibrary.CreateDialogueUI 호출  
      (파라미터: Self, Self->LLMDialogueComponent, BP_LLMDialogueWidget Class)  
    - 반환된 위젯을 DialogueWidget 변수에 저장  
    - DialogueWidget->AddToViewport() 호출  
    - DialogueWidget->SetDialogueVisible(true) 호출  
    - Set Input Mode UI Only 호출  
    - Show Mouse Cursor = true 설정  

  * 대화 종료 함수 생성 (예: EndDialogue)  
  * 함수 내부:  
    - DialogueWidget->RemoveFromParent() 호출  
    - Set Input Mode Game Only 호출  
    - Show Mouse Cursor = false 설정  

###  4. 플레이어 설정하기  

  1. 플레이어 블루프린트 수정:  
    - 프로젝트의 플레이어 캐릭터 블루프린트를 열거나 새로 생성합니다.  
    - NPC와 상호작용할 수 있는 로직 추가:  
    * NPC 감지 기능 (예: 오버랩 이벤트, 트레이스, 또는 액션 키)  
  * 상호작용 키 입력 설정 (예: E 키):  
    - 입력 이벤트에서:  
      • NPC가 범위 내에 있는지 확인  
      • 있다면 NPC의 StartDialogue 함수 호출  
  * 대화 종료 키 입력 설정 (예: Escape 키):  
    - 입력 이벤트에서:  
      • 현재 대화 중인지 확인  
      • 대화 중이라면 NPC의 EndDialogue 함수 호출  

###  5. 게임 모드 설정  

  1. 게임 모드 수정:  
    - 필요하다면 게임 모드를 수정하여 플레이어 캐릭터와 NPC 시스템이 잘 작동하도록 설정합니다.  

###  6. 테스트 맵 생성  

  1. 테스트 맵 설정:  
    - 새 레벨 생성  
    - NPC 배치  
    - 플레이어 시작 위치 설정  
    - 게임 플레이 테스트  

 ### 실제 대화 흐름  

  1. 플레이어가 NPC에 접근합니다.  
  2. 상호작용 키(E)를 누르면 대화 UI가 나타납니다.  
  3. UI에 NPC의 이름이 표시되고, 초기 환영 메시지가 표시됩니다.  
  4. 플레이어는 입력 필드에 메시지를 입력하고 전송합니다.  
  5. 메시지가 대화 영역에 추가되고, LLMDialogueComponent가 LLM API로 요청을 보냅니다.  
  6. 응답이 오면 NPC의 대답이 대화 영역에 추가됩니다.  
  7. 플레이어는 계속 대화하거나 ESC를 눌러 대화를 종료할 수 있습니다.  

  이렇게 설정하면 LLM API를 활용하여 동적인 NPC 대화 시스템을 구현할 수 있습니다. 각 NPC마다 다른 시스템 프롬프트를 설정하여 다양한 성격과 지식을 가진 NPC를 만들 수   있습니다.  
