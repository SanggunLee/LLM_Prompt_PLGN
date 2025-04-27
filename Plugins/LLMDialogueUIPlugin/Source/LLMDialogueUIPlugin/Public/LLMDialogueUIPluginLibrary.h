// Copyright LLM Dialogue UI Plugin Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LLMDialogueUIPluginLibrary.generated.h"

class ULLMDialogueComponent;
class ULLMDialogueWidget;

/**
 * Blueprint function library for LLM Dialogue UI Plugin
 */
UCLASS()
class LLMDIALOGUEUIPLUGIN_API ULLMDialogueUIPluginLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** 
     * Create a dialogue UI widget and initialize it with the given dialogue component 
     * @param WorldContextObject World context
     * @param DialogueComponent The dialogue component to connect to the UI
     * @param WidgetClass Widget class to create (must be a subclass of ULLMDialogueWidget)
     * @return The created dialogue UI widget
     */
    UFUNCTION(BlueprintCallable, Category = "LLM Dialogue UI", meta = (WorldContext = "WorldContextObject", DisplayName = "Create Dialogue UI"))
    static ULLMDialogueWidget* CreateDialogueUI(const UObject* WorldContextObject, ULLMDialogueComponent* DialogueComponent, TSubclassOf<ULLMDialogueWidget> WidgetClass);
};