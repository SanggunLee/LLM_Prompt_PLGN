// 이 파일은 언리얼 에디터에서 블루프린트 위젯으로 생성해야 합니다.
// 아래는 위젯 블루프린트를 만들기 위한 가이드입니다.

/*
위젯 블루프린트 생성 가이드:

1. 콘텐츠 브라우저에서 LLMDialoguePlugin/UI 폴더에 우클릭합니다.
2. User Interface > Widget Blueprint를 선택합니다.
3. 이름을 "WBP_DialogueUI"로 지정합니다.

위젯 디자인:
1. 캔버스 패널 추가:
   - Size Box (크기: 800x200)
     - Background (Image)
     - Vertical Box
       - Text Block (이름: "NPCNameText", 텍스트: "NPC 이름", 스타일: 큰 글씨)
       - Text Block (이름: "DialogueText", 텍스트: "여기에 대화 내용이 표시됩니다.", 스타일: 일반 글씨)
       - Horizontal Box (이름: "InputContainer")
         - Edit Text Box (이름: "InputTextBox", 힌트 텍스트: "메시지를 입력하세요...")
         - Button (이름: "SendButton", 텍스트: "전송")

그래프(이벤트):
1. "SendButton" OnClicked 이벤트 생성:
   - InputTextBox에서 텍스트 가져오기
   - OnMessageSent 사용자 이벤트 호출 (String 파라미터로 텍스트 전달)
   - InputTextBox 초기화

2. 사용자 함수 "UpdateDialogueText" 생성 (String 파라미터 "NewText"):
   - DialogueText.SetText(NewText) 설정

3. 사용자 함수 "SetNPCName" 생성 (String 파라미터 "Name"):
   - NPCNameText.SetText(Name) 설정

4. 사용자 이벤트 "OnMessageSent" 생성 (String 파라미터 "Message"):
   - 이 이벤트는 외부에서 바인딩 될 예정
*/