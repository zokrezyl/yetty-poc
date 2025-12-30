# Yetty Build System
# Usage: make <target>

# Use system tools for most builds (avoid nix)
SYSTEM_PATH := /usr/bin:/bin:/usr/local/bin:$(PATH)

# Android SDK
export JAVA_HOME ?= /usr/lib/jvm/java-17-openjdk-amd64
export ANDROID_HOME ?= $(HOME)/android-sdk
export ANDROID_SDK_ROOT ?= $(ANDROID_HOME)
export ANDROID_NDK_HOME ?= $(ANDROID_HOME)/ndk/26.1.10909125
export ANDROID_ABI ?= arm64-v8a
export ANDROID_PLATFORM ?= android-26

# Build directories
BUILD_DIR_DESKTOP := build-desktop
BUILD_DIR_ANDROID := build-android
BUILD_DIR_WEBASM := build-webasm

# External dependencies
BUSYBOX_VERSION := 1.34.1
BUSYBOX_DIR := external/busybox

# CMake options
CMAKE := cmake
CMAKE_GENERATOR := -G Ninja
CMAKE_RELEASE := -DCMAKE_BUILD_TYPE=Release
CMAKE_DEBUG := -DCMAKE_BUILD_TYPE=Debug

# Gradle options
GRADLE_OPTS := --project-cache-dir=../$(BUILD_DIR_ANDROID)/.gradle

# Default target - show help
.PHONY: all
all: help

#=============================================================================
# Desktop
#=============================================================================

.PHONY: config-desktop
config-desktop: ## Configure desktop build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP) $(CMAKE_GENERATOR) $(CMAKE_RELEASE)

.PHONY: build-desktop
build-desktop: ## Build for desktop
	@if [ ! -f "$(BUILD_DIR_DESKTOP)/build.ninja" ]; then $(MAKE) config-desktop; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP) --parallel

#=============================================================================
# Android
#=============================================================================

.PHONY: config-android
config-android: ## Configure Android build
	@$(MAKE) _android-wgpu

.PHONY: build-android
build-android: ## Build for Android
	@$(MAKE) _android-deps
	nix develop .#android --command bash -c "cd android && ./gradlew $(GRADLE_OPTS) assembleDebug"

#=============================================================================
# WebAssembly
#=============================================================================

.PHONY: config-webasm
config-webasm: ## Configure WebAssembly build
	nix develop .#web --command bash -c '\
		export EMCC_SKIP_SANITY_CHECK=1 && \
		emcmake cmake -B $(BUILD_DIR_WEBASM) $(CMAKE_GENERATOR) \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_EXE_LINKER_FLAGS="-O2"'

.PHONY: build-webasm
build-webasm: ## Build for WebAssembly
	@if [ ! -f "$(BUILD_DIR_WEBASM)/build.ninja" ]; then $(MAKE) config-webasm; fi
	nix develop .#web --command bash -c 'cmake --build $(BUILD_DIR_WEBASM) --parallel'
	@cp web/index.html $(BUILD_DIR_WEBASM)/

#=============================================================================
# Run
#=============================================================================

.PHONY: run-desktop
run-desktop: build-desktop ## Run desktop build
	./$(BUILD_DIR_DESKTOP)/yetty

.PHONY: run-webasm
run-webasm: build-webasm ## Serve and open WebAssembly build
	python3 web/serve.py 8000 $(BUILD_DIR_WEBASM)

#=============================================================================
# Test
#=============================================================================

.PHONY: test-desktop
test-desktop: ## Run desktop tests
	@if [ ! -f "$(BUILD_DIR_DESKTOP)/build.ninja" ]; then \
		PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP) $(CMAKE_GENERATOR) $(CMAKE_DEBUG); \
	fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP) --target yetty_tests --parallel
	./$(BUILD_DIR_DESKTOP)/test/ut/yetty_tests

.PHONY: test-android
test-android: build-android ## Run Android tests (installs on connected device)
	adb install -r $(BUILD_DIR_ANDROID)/app/outputs/apk/debug/app-debug.apk
	adb shell am start -n com.yetty.terminal/android.app.NativeActivity

.PHONY: test-webasm
test-webasm: build-webasm ## Run WebAssembly tests (opens in browser)
	python3 web/serve.py 8000 $(BUILD_DIR_WEBASM)

#=============================================================================
# Clean
#=============================================================================

.PHONY: clean
clean: ## Clean all build directories
	rm -rf $(BUILD_DIR_DESKTOP) $(BUILD_DIR_ANDROID) $(BUILD_DIR_WEBASM)

#=============================================================================
# Internal targets (not shown in help)
#=============================================================================

.PHONY: _android-wgpu
_android-wgpu:
	@cd $(CURDIR) && bash build-tools/android/build-wgpu.sh

.PHONY: _android-busybox
_android-busybox:
	@nix develop .#android --command bash build-tools/android/build-busybox.sh

.PHONY: _android-assets
_android-assets:
	@mkdir -p $(BUILD_DIR_ANDROID)/assets
	@cp -f assets/*.ttf assets/*.png assets/*.json $(BUILD_DIR_ANDROID)/assets/ 2>/dev/null || true

.PHONY: _android-deps
_android-deps: _android-wgpu _android-busybox _android-assets

#=============================================================================
# Help
#=============================================================================

.PHONY: help
help:
	@echo "Yetty Build System"
	@echo ""
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-16s\033[0m %s\n", $$1, $$2}'
	@echo ""
	@echo "Build outputs:"
	@echo "  build-desktop/yetty"
	@echo "  build-android/app/outputs/apk/"
	@echo "  build-webasm/yetty.html"
