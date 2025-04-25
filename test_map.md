# LLM 대화 플러그인 테스트 맵 구성 및 사용 가이드

> 이 플러그인을 테스트 해 볼수 있는 테스트 용 맵을 만드는게 가능한가?  

● 네, 테스트용 맵을 만들어 플러그인을 테스트할 수 있습니다. 간단한 테스트 맵과 필요한 요소들을 구현해 보겠습니다.  

● Call(Create test map directories)…  
  ⎿  Bash(mkdir -p /mnt/f/UE_Project/LLM_Prompt_PLGN/Content/LLMDialoguePlugin/Maps)…  
  ⎿  Bash(mkdir -p /mnt/f/UE_Project/LLM_Prompt_PLGN/Content/LLMDialoguePlugin/Blueprints)…  
  ⎿  Bash(mkdir -p /mnt/f/UE_Project/LLM_Prompt_PLGN/Content/LLMDialoguePlugin/UI)…  
  ⎿  Done (3 tool uses · 6.9s)  

● 이제 테스트에 필요한 블루프린트 파일들을 생성해보겠습니다. 먼저 대화 UI 위젯을 만들겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/UI/WBP_DialogueUI.uc)…    
  ⎿ Wrote 36 lines to Content/LLMDialoguePlugin/UI/WBP_DialogueUI.uc  
     // 이 파일은 언리얼 에디터에서 블루프린트 위젯으로 생성해야 합니다.  
     // 아래는 위젯 블루프린트를 만들기 위한 가이드입니다.  
     /*  
     위젯 블루프린트 생성 가이드:  
     1. 콘텐츠 브라우저에서 LLMDialoguePlugin/UI 폴더에 우클릭합니다.  
     2. User Interface > Widget Blueprint를 선택합니다.  
     3. 이름을 "WBP_DialogueUI"로 지정합니다.  
     … +26 lines (ctrl+r to expand)  

● 이제 NPC 블루프린트 스크립트를 만들겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/Blueprints/BP_TestNPC.uc)…  
  ⎿ Wrote 46 lines to Content/LLMDialoguePlugin/Blueprints/BP_TestNPC.uc  
     // 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.  
     // 아래는 NPC 블루프린트를 만들기 위한 가이드입니다.  
     /*  
     액터 블루프린트 생성 가이드:  
     1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.  
     2. Blueprint Class를 선택합니다.  
     3. Character 클래스를 선택합니다.  
     4. 이름을 "BP_TestNPC"로 지정합니다.  
     … +36 lines (ctrl+r to expand)  

● 이제 테스트 맵 구성 가이드를 생성하겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/Maps/TestMap_Guide.txt)…  
  ⎿ Wrote 74 lines to Content/LLMDialoguePlugin/Maps/TestMap_Guide.txt  
     # LLM 대화 플러그인 테스트 맵 구성 가이드  
     이 가이드는 LLM 대화 플러그인을 테스트하기 위한 맵 구성 방법을 설명합니다.  
     ## 1. 테스트 맵 생성하기  
     1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Maps 폴더에 우클릭합니다.  
     2. Level > 빈 레벨을 선택합니다.  
     3. 맵 이름을 "TestMap"으로 저장합니다.  
     … +64 lines (ctrl+r to expand)  

● 이제 플레이어 블루프린트를 위한 상호작용 시스템 가이드를 만들겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/Blueprints/BP_InteractivePlayer.uc)…  
  ⎿ Wrote 77 lines to Content/LLMDialoguePlugin/Blueprints/BP_InteractivePlayer.uc  
     // 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.  
     // 아래는 상호작용 가능한 플레이어 블루프린트를 만들기 위한 가이드입니다.  
     /*  
     플레이어 블루프린트 생성 가이드:  
     1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.  
     2. Blueprint Class를 선택합니다.  
     3. Character 클래스를 선택합니다.  
     4. 이름을 "BP_InteractivePlayer"로 지정합니다.  
     … +67 lines (ctrl+r to expand)  

● 이제 테스트 맵에 대한 README 파일을 만들어 전체 프로세스를 설명하겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/README.md)…  
  ⎿ Wrote 74 lines to Content/LLMDialoguePlugin/README.md  
     # LLM 대화 플러그인 테스트 가이드  
     이 폴더에는 LLM 대화 플러그인을 테스트하기 위한 에셋과 가이드가 포함되어 있습니다.  
     ## 1. 플러그인 활성화하기  
     1. 언리얼 에디터를 엽니다.  
     2. Edit > Plugins 메뉴를 선택합니다.  
     3. "LLM Dialogue Plugin"을 검색합니다.  
     4. 해당 플러그인을 활성화하고 에디터를 재시작합니다.  
     … +64 lines (ctrl+r to expand)  

● 이제 테스트 맵을 위한 DefaultGameMode 블루프린트 가이드를 만들겠습니다:  

● Write(file_path: Content/LLMDialoguePlugin/Blueprints/BP_TestGameMode.uc)…  
  ⎿ Wrote 27 lines to Content/LLMDialoguePlugin/Blueprints/BP_TestGameMode.uc  
     // 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.  
     // 아래는 테스트 게임 모드 블루프린트를 만들기 위한 가이드입니다.  
     /*  
     게임 모드 블루프린트 생성 가이드:  
     1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.  
     2. Blueprint Class를 선택합니다.  
     3. GameModeBase 클래스를 선택합니다.  
     4. 이름을 "BP_TestGameMode"로 지정합니다.  
     … +17 lines (ctrl+r to expand)  

● 테스트 맵을 위한 가이드 파일들을 모두 생성했습니다. 이제 플러그인과 테스트 맵을 사용하는 방법을 요약하겠습니다:  

  1. 플러그인 설치 및 활성화:  
    - 생성된 LLMDialoguePlugin 폴더를 프로젝트의 Plugins 디렉토리에 배치  
    - 언리얼 에디터에서 플러그인 활성화  
  2. 테스트 에셋 생성:  
    - 대화 UI 위젯 (WBP_DialogueUI)  
    - NPC 블루프린트 (BP_TestNPC)  
    - 상호작용 플레이어 블루프린트 (BP_InteractivePlayer)   
    - 게임 모드 블루프린트 (BP_TestGameMode)  
  3. 테스트 맵 구성:  
    - 맵 생성 및 환경 설정  
    - NPC 배치  
    - 플레이어 시작 위치 설정  
    - 게임 모드 설정  
  4. 테스트 실행:  
    - 플레이어가 NPC에 접근하여 상호작용   
    - 대화 UI에 메시지 입력 
    - LLM API를 통한 응답 수신 및 표시  

  각 블루프린트와 에셋 생성에 대한 자세한 가이드는 해당 .uc 파일과 README.md에 포함되어 있습니다. 이 가이드를 따라 테스트 맵을   
  설정하고 LLM 대화 플러그인의 기능을 테스트할 수 있습니다. 

  주의할 점은, OpenAI API 키가 필요하며 API 사용에 비용이 발생할 수 있다는 점입니다.
