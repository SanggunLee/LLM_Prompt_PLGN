# LLM Dialogue Plugin 설계 및 명세 문서

이 문서는 Unreal Engine 5 기반 게임에서 NPC 대사 생성을 위해 LLM API(OpenAI GPT 등)를 연동하는 **LLM Dialogue Plugin**의 아키텍처 및 함수 설계 명세입니다. 이 문서를 기반으로 LLM이 자동으로 C++ 코드 및 관련 블루프린트 연동을 생성할 수 있도록 구성되어 있습니다.

---

## 개요

- **플러그인 이름:** `LLMDialoguePlugin`
- **목적:** 게임 내 NPC가 플레이어와 상호작용할 때, LLM API를 통해 대사를 실시간 생성하도록 지원
- **기반 엔진:** Unreal Engine 5
- **주요 언어:** C++

---

## 아키텍처 구성도

```plaintext
[Player] 클릭
   ↓
[NPC Actor] + [LLMDialogueComponent]
   ↓
[Prompt 구성 함수] + [HTTP 요청 처리 함수]
   ↓
[LLM API (ex: GPT-4)]
   ↓
[응답 수신] → [Blueprint Event로 전달]
   ↓
[UI 출력 및 대화 로그 저장]
```

---

## 주요 컴포넌트

### 1. `ULLMDialogueComponent`

> NPC Actor에 부착 가능한 ActorComponent. LLM 요청과 응답 처리를 담당.

#### 주요 함수 및 변수

##### `void SendPromptToLLM(const FString& Prompt)`
- **역할:** 전달된 프롬프트를 LLM API에 POST 방식으로 전송
- **입력:** `Prompt` (대사 생성을 위한 전체 텍스트)
- **출력:** 없음 (결과는 콜백을 통해 전달)

##### `void OnResponseReceived(...)`
- **역할:** HTTP 응답이 완료되었을 때 호출되며, JSON에서 메시지를 추출하여 이벤트로 전달
- **출력:** `FString Message` (LLM 응답 텍스트)

##### `FOnLLMResponse OnLLMResponse`
- **역할:** LLM 응답을 블루프린트에 전달하는 delegate
- **BlueprintAssignable:** UI 연동에 사용

---

## Dialogue State Machine 설계

> NPC 대화 흐름을 상태 기반(State Machine)으로 제어하여 문맥적 일관성과 퀘스트, 감정 반응 등을 반영할 수 있음

### 상태 예시 및 전이 흐름
```plaintext
[인사 상태] → [정보 제공 상태] → [퀘스트 제안 상태] → [작별 상태]
```

### 상태별 프롬프트 예시
- 인사 상태: "플레이어가 처음 말을 걸었을 때, 정중하게 인사한다"
- 정보 제공 상태: "플레이어가 마을에 대해 질문함, 알고 있는 정보만 전달"
- 퀘스트 제안 상태: "신뢰도가 충분하면, 퀘스트를 제안하는 말투로 설명"
- 작별 상태: "더 이상 대화가 없을 때 작별 인사를 건넨다"

### 상태 전이 방식
- 플레이어의 입력 분석 결과(예: 키워드 포함 여부, 의도 파악 등)를 통해 상태 전이
- 상태는 NPC별로 유지되며 SaveGame 등으로 지속 가능

### 기술적 적용 방식
- `CurrentDialogueState` enum 및 상태 전이 함수 작성
- 상태값에 따라 프롬프트 템플릿 선택 후 LLM 호출
- LLM 응답 후 다음 상태 결정 및 반영

### 시각화 및 블루프린트 노드화
- DialogueStateMachine을 블루프린트 노드로 시각화하여 각 상태 간 전이를 직관적으로 설계 가능
- 상태 노드마다 다음과 같은 메타 정보 포함:
  - 상태명 (예: Greeting, Info, QuestOffer, Goodbye)
  - LLM Prompt Template
  - 전이 조건 리스트 (키워드 기반, 선택지, LLM 응답 태그 등)
- 에디터에서 시각적으로 NPC 대화 흐름을 설계하고, 실시간으로 LLM 프롬프트 전환 적용 가능

---

## LLM 응답 기반 상태 판단 로직

> LLM이 생성한 응답 메시지를 분석하여 다음 상태를 자동 판단하는 로직입니다.

### 적용 시나리오
- LLM 응답이 퀘스트 제안이라면 → `퀘스트 제안 상태`로 전이
- 작별 인사가 포함되어 있다면 → `작별 상태`로 전이

### 방법 1: 명시적 태그 삽입 방식
- 프롬프트에서 LLM에게 다음 상태를 판단해 JSON 태그로 결과를 함께 포함시키도록 요청

```prompt
NPC의 대사를 생성하되, 마지막에 다음 상태를 다음 형식으로 표시:
<NEXT_STATE:QuestOffer>
```

응답 예시:
```plaintext
...정말 도와주겠나? <NEXT_STATE:QuestOffer>
```

→ 정규표현식으로 `<NEXT_STATE:(\w+)>` 추출 → enum으로 상태 전이

### 방법 2: 후처리 키워드 매칭
- "도와줘", "부탁이야", "이제 가보게" 등의 키워드를 기준으로 상태 분류
- 단점: 오탐 가능성 있음. fallback으로 사용

### 방법 3: 별도 판단용 LLM 호출
- 응답을 분석하는 별도의 LLM 요청을 통해 상태 결정
- 예: "다음 문장에서 현재 상태와 다음 상태를 추론해줘"

```json
{
  "response": "더 이상 할 말이 없군. 이만 가보게.",
  "current_state": "Information",
  "output": { "next_state": "Goodbye" }
}
```

---

## API 요청 사양 (OpenAI GPT 예시)

### URL
```
https://api.openai.com/v1/chat/completions
```

### 요청 Body 예시
```json
{
  "model": "gpt-4",
  "messages": [
    {"role": "system", "content": "세계관 설명 및 말투 규칙 등"},
    {"role": "user", "content": "NPC 프롬프트 텍스트"}
  ],
  "temperature": 0.7
}
```

### 요청 헤더
- `Content-Type: application/json`
- `Authorization: Bearer <YOUR_API_KEY>`

---

## 사용 예 (블루프린트)

1. NPC에 `LLMDialogueComponent` 부착
2. `SendPromptToLLM(PromptString)` 호출
3. `OnLLMResponse` 바인딩 → 텍스트 출력 위젯에 연결

---

## 확장 가능 항목

| 기능 | 설명 |
|------|------|
| SaveGame 연동 | NPC별 대화 로그를 저장하여 기억 유지 |
| 다양한 LLM 연동 | Claude, Gemini 등으로 확장 가능 |
| Prompt Template 시스템 | JSON 기반 템플릿 구성 자동화 |
| 프롬프트 큐 | 다수의 NPC 요청을 순차적으로 처리하는 대기열 |
| 감정 분석 | 대사에 포함된 감정 태그 추출 가능 |
| Dialogue State Machine | 상태 기반 대화 흐름 지원 및 LLM과 연동된 전이 처리 |
| LLM 응답 기반 상태 추론 | 태그 분석, 키워드 매칭, 별도 LLM 분석 방식 적용 가능 |
| Dialogue StateMachine 시각화 | 블루프린트 기반 시각 노드로 상태 관리 및 편집 가능 |

---

## 파일 구조 예시

```plaintext
LLMDialoguePlugin/
├─ Source/LLMDialoguePlugin/
│  ├─ Public/
│  │  └─ LLMDialogueComponent.h
│  └─ Private/
│     └─ LLMDialogueComponent.cpp
├─ Resources/
│  └─ Icon128.png
├─ LLMDialoguePlugin.uplugin
```

---

## 향후 발전 방향
- DialogueStateMachine과의 통합 및 시각화 노드화
- NPC별 프로필 파일 자동 로딩 (DataTable 또는 JSON)
- 멀티 캐릭터 동시 대화 지원
- 프롬프트 내 세계관 정보 병합 최적화
- LLM 응답 기반 상태 추론 고도화 및 커스텀 모델 학습

---

이 문서를 기반으로 LLM은 C++ 코드 생성, 블루프린트 노드 생성, HTTP API 자동화 등을 수행할 수 있습니다.

