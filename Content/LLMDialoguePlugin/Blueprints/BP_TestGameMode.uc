// 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.
// 아래는 테스트 게임 모드 블루프린트를 만들기 위한 가이드입니다.

/*
게임 모드 블루프린트 생성 가이드:

1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.
2. Blueprint Class를 선택합니다.
3. GameModeBase 클래스를 선택합니다.
4. 이름을 "BP_TestGameMode"로 지정합니다.

설정:
1. 디테일 패널에서 다음 속성을 설정합니다:
   - Default Pawn Class: BP_InteractivePlayer
   - HUD Class: None
   - Player Controller Class: PlayerController
   - Game State Class: GameStateBase
   - Player State Class: PlayerState

2. 월드 세팅에서 이 게임 모드를 테스트 맵의 기본 게임 모드로 설정:
   - 테스트 맵을 엽니다.
   - 월드 세팅 패널을 엽니다 (Window > World Settings)
   - GameMode Override를 BP_TestGameMode로 설정

이 게임 모드는 상호작용 기능이 있는 플레이어 캐릭터를 사용하고,
테스트 맵에서 LLM 대화 기능을 시연하는 데 필요한 기본 설정을 제공합니다.
*/