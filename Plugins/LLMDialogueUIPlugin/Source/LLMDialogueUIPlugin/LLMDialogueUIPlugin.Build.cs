using UnrealBuildTool;

public class LLMDialogueUIPlugin : ModuleRules
{
    public LLMDialogueUIPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UMG",
                "Slate",
                "SlateCore",
                "LLMDialoguePlugin" // 의존성: LLMDialoguePlugin 추가
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "HTTP",
                "Json",
                "JsonUtilities"
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
        );
    }
}