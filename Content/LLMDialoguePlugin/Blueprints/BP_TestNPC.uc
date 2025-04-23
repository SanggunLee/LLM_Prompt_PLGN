// 이 파일은 언리얼 에디터에서 블루프린트로 생성해야 합니다.
// 아래는 NPC 블루프린트를 만들기 위한 가이드입니다.

/*
액터 블루프린트 생성 가이드:

1. 콘텐츠 브라우저에서 LLMDialoguePlugin/Blueprints 폴더에 우클릭합니다.
2. Blueprint Class를 선택합니다.
3. Character 클래스를 선택합니다.
4. 이름을 "BP_TestNPC"로 지정합니다.

컴포넌트 구성:
1. 기본 컴포넌트 구성 유지 (Character 기본 컴포넌트)
2. Components 패널에서 컴포넌트 추가 버튼 클릭
3. "LLMDialogueComponent" 검색 및 추가
4. 새 컴포넌트 이름을 "DialogueComponent"로 변경

디테일 패널에서 DialogueComponent 설정:
1. API Key: (OpenAI API 키 입력)
2. Model Name: "gpt-4"
3. System Prompt: "당신은 판타지 세계의 NPC입니다. 플레이어와 대화할 때는 역할에 맞게 간결하게 대답하세요."
4. 상태별 템플릿 설정 (Templates 카테고리):
   - Greeting Template: "플레이어를 친근하게 맞이하세요. 마지막에 <NEXT_STATE:Information> 태그를 포함하세요."
   - Information Template: "플레이어 질문에 도움이 되는 정보를 제공하세요. 필요한 경우 <NEXT_STATE:QuestOffer> 태그를 사용하세요."
   - Quest Offer Template: "플레이어에게 간단한 퀘스트나 임무를 제안하세요. 마무리할 때는 <NEXT_STATE:Goodbye> 태그를 사용하세요."
   - Goodbye Template: "캐릭터에 맞게 작별 인사를 하세요."

이벤트 그래프:
1. Begin Play 이벤트 생성:
   - DialogueUI 위젯 생성 (WBP_DialogueUI 클래스)
   - 위젯을 화면에 추가
   - 위젯의 SetNPCName 함수 호출 (파라미터: "마을 주민")
   - DialogueComponent.OnLLMResponse 이벤트에 커스텀 함수 바인딩

2. 커스텀 함수 "HandleLLMResponse" 생성 (String 파라미터 "Message"):
   - DialogueUI.UpdateDialogueText(Message) 호출

3. 커스텀 함수 "SendPlayerMessage" 생성 (String 파라미터 "Message"):
   - DialogueComponent.SendPromptToLLM(Message) 호출

4. DialogueUI의 OnMessageSent 이벤트를 SendPlayerMessage 함수에 바인딩

5. 플레이어와 상호작용 설정:
   - Input > Interaction 추가 (E 키 등)
   - 상호작용 시 DialogueUI 표시/숨김 토글
*/