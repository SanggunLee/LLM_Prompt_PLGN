# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands
- Build plugin: `UnrealBuildTool LLM_Prompt_PLGN Development Win64`
- Package project: `UnrealEditor-Cmd.exe <project_path>/LLM_Prompt_PLGN.uproject -run=Cook -targetplatform=Win64 -iterate`
- Run: Launch UE5 editor and open LLM_Prompt_PLGN.uproject
- Run single test: `AutomationTool.exe BuildCookRun -project="<project_path>/LLM_Prompt_PLGN.uproject" -noP4 -buildscw -cook -allmaps -stage -platform=Win64 -serverconfig=Development -utf8output -UE5 -CrashReporter -run -automation="Test.<test_name>"`

## Code Style Guidelines
- Follow Unreal Engine C++ coding standards
- Class names prefixed with proper Unreal prefix (U for UObject, A for Actor, F for structs)
- Use `PascalCase` for functions and class names
- Use `camelCase` for variables with member variables prefixed with appropriate type (b for bool, f for float, etc.)
- Methods that return values should have descriptive names indicating what they return
- Blueprint-exposed properties should use UPROPERTY() with appropriate specifiers
- Blueprint-callable functions should use UFUNCTION(BlueprintCallable)
- Add XML-style comments (/** */) for public API methods
- Include proper error handling with UE_LOG macros and categorized log channels
- Use UE container types (TArray, TMap) instead of STL
- Group header includes: CoreMinimal.h first, then Engine headers, then project headers

## Project Structure
- Plugin is designed as an Unreal Engine 5 module for NPC dialogue generation with LLM APIs
- Implementation follows the design specifications in Code_Gen_Doc.md
- Components should follow single responsibility principle
- Use interfaces for abstraction where appropriate (e.g., ILLMService interface)
- Implement HTTP requests using FHttpModule for API integration
- Follow the Dialogue State Machine pattern described in design docs