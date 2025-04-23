// 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.
// 아래는 상호작용 가능한 플레이어 블루프린트를 만들기 위한 가이드입니다.

/*
플레이어 블루프린트 생성 가이드:

1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.
2. Blueprint Class를 선택합니다.
3. Character 클래스를 선택합니다.
4. 이름을 "BP_InteractivePlayer"로 지정합니다.

컴포넌트 추가:
1. 기본 컴포넌트 구성 유지 (Character 기본 컴포넌트)
2. Components 패널에서 추가 버튼 클릭
3. "Sphere" 컴포넌트 추가 (상호작용 탐지용)
   - 이름: "InteractionSphere"
   - 반경: 200.0
   - 콜리전: OverlapAllDynamic
   - 부모: CapsuleComponent
   - 위치: (0, 0, 0)
4. Spring Arm 컴포넌트 추가
   - 길이: 300.0
   - 카메라와 연결하기 위함
5. Camera 컴포넌트 추가
   - Spring Arm에 부착

변수 추가:
1. 변수명: "OverlappingNPC"
   - 유형: BP_TestNPC Object Reference
   - 인스턴스 편집 가능: 체크
   - 카테고리: Interaction
2. 변수명: "IsInteracting"
   - 유형: Boolean
   - 기본값: false
   - 카테고리: Interaction
3. 변수명: "DialogueWidget"
   - 유형: WBP_DialogueUI Object Reference
   - 카테고리: UI

이벤트 그래프:
1. InteractionSphere의 ComponentBeginOverlap 이벤트 생성:
   - 충돌한 액터가 BP_TestNPC 클래스인지 확인 (Cast)
   - 맞다면 OverlappingNPC 변수에 저장
   - 상호작용 가능 메시지 표시 (텍스트 위젯 등)

2. InteractionSphere의 ComponentEndOverlap 이벤트 생성:
   - 충돌이 끝난 액터가 OverlappingNPC와 동일한지 확인
   - 맞다면 OverlappingNPC 변수를 null로 설정
   - 상호작용 가능 메시지 숨김

3. Interact 입력 이벤트 생성:
   - OverlappingNPC가 null이 아닌지 확인
   - IsInteracting 토글 (true면 false로, false면 true로)
   - IsInteracting이 true면:
     - OverlappingNPC의 대화 UI 표시 (또는 생성)
     - 플레이어 이동 제한
   - IsInteracting이 false면:
     - 대화 UI 숨김
     - 플레이어 이동 제한 해제

4. Movement Input 이벤트 (MoveForward, MoveRight 등):
   - IsInteracting이 true면 움직임 입력 무시
   - false면 기존 방식대로 움직임 처리

주요 함수:
1. "StartInteraction" 함수 (BP_TestNPC 파라미터):
   - OverlappingNPC 설정
   - DialogueWidget 생성 또는 표시
   - NPC의 DialogueComponent 참조 가져오기
   - DialogueWidget의 OnMessageSent 이벤트를 NPC의 SendPlayerMessage 함수에 바인딩
   - NPC의 OnLLMResponse를 DialogueWidget의 UpdateDialogueText 함수에 바인딩

2. "EndInteraction" 함수:
   - DialogueWidget 숨김
   - 플레이어 이동 제한 해제
   - 바인딩된 이벤트 해제
*/