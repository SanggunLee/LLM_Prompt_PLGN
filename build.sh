#!/bin/bash

# ====== 설정 ======

# Unreal Engine 설치 경로 (Windows 드라이브 기준 -> /mnt/c/로 변환)
UE_ENGINE_PATH="/mnt/f/UE_5.4"   # 꼭 자신의 버전에 맞게 수정하세요!

# UnrealBuildTool.exe 경로
UBT_EXECUTABLE="${UE_ENGINE_PATH}/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.exe"

# 현재 프로젝트 루트 기준
PROJECT_DIR=$(pwd)
UPROJECT_FILE=$(find "$PROJECT_DIR" -maxdepth 1 -name "*.uproject" | head -n 1)

if [ ! -f "$UPROJECT_FILE" ]; then
    echo "❌ .uproject 파일을 찾을 수 없습니다. 이 스크립트는 프로젝트 루트에서 실행해야 합니다."
    exit 1
fi

# 프로젝트 이름 추출
PROJECT_NAME=$(basename "$UPROJECT_FILE" .uproject)

# 빌드 타겟 설정
BUILD_TARGET="${PROJECT_NAME}Editor"   # 에디터 빌드
PLATFORM="Win64"
CONFIGURATION="Development"

# ====== 실행 ======
echo "🔨 빌드 시작: ${PROJECT_NAME} (${PLATFORM} / ${CONFIGURATION})..."

# WSL 경로를 Windows 경로로 변환
WIN_UPROJECT_PATH=$(wslpath -w "$UPROJECT_FILE")

# UBT 호출
"$UBT_EXECUTABLE" "$BUILD_TARGET" "$PLATFORM" "$CONFIGURATION" -project="$WIN_UPROJECT_PATH" -waitmutex -progress

BUILD_RESULT=$?

if [ $BUILD_RESULT -ne 0 ]; then
    echo "❌ 빌드 실패!"
    exit 1
else
    echo "✅ 빌드 성공!"
    exit 0
fi
