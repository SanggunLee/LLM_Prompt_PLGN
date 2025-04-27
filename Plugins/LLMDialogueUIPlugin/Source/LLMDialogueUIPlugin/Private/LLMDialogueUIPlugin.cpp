// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#include "LLMDialogueUIPlugin.h"

#define LOCTEXT_NAMESPACE "FLLMDialogueUIPluginModule"

void FLLMDialogueUIPluginModule::StartupModule()
{
    // 모듈 초기화 코드
    UE_LOG(LogTemp, Log, TEXT("LLM Dialogue UI Plugin module has been loaded"));
}

void FLLMDialogueUIPluginModule::ShutdownModule()
{
    // 모듈 종료 코드
    UE_LOG(LogTemp, Log, TEXT("LLM Dialogue UI Plugin module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLLMDialogueUIPluginModule, LLMDialogueUIPlugin)