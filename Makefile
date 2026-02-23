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

# WebGPU backends
BACKEND_WGPU := wgpu
BACKEND_DAWN := dawn

# Build directories (platform-backend-buildtype)
BUILD_DIR_DESKTOP_WGPU_DEBUG := build-desktop-wgpu-debug
BUILD_DIR_DESKTOP_WGPU_RELEASE := build-desktop-wgpu-release
BUILD_DIR_DESKTOP_DAWN_DEBUG := build-desktop-dawn-debug
BUILD_DIR_DESKTOP_DAWN_RELEASE := build-desktop-dawn-release
BUILD_DIR_DESKTOP_DAWN_ASAN := build-desktop-dawn-asan

BUILD_DIR_ANDROID_WGPU_DEBUG := build-android-wgpu-debug
BUILD_DIR_ANDROID_WGPU_RELEASE := build-android-wgpu-release
BUILD_DIR_ANDROID_DAWN_DEBUG := build-android-dawn-debug
BUILD_DIR_ANDROID_DAWN_RELEASE := build-android-dawn-release

# WebAssembly uses browser native WebGPU (Dawn in Chrome)
BUILD_DIR_WEBASM_DAWN_DEBUG := build-webasm-dawn-debug
BUILD_DIR_WEBASM_DAWN_RELEASE := build-webasm-dawn-release

# Parallel jobs (override with: make build-... PARALLEL_JOBS=30)
PARALLEL_JOBS ?=
CMAKE_PARALLEL := $(if $(PARALLEL_JOBS),--parallel $(PARALLEL_JOBS),--parallel)

# Distributed build with ccache + distcc
DISTCC_HOSTS ?= localhost 192.168.1.10
export DISTCC_HOSTS
export CCACHE_PREFIX := distcc
CMAKE_COMPILER_LAUNCHER := -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

# CMake options
CMAKE := cmake
CMAKE_GENERATOR := -G Ninja
CMAKE_RELEASE := -DCMAKE_BUILD_TYPE=Release
CMAKE_DEBUG := -DCMAKE_BUILD_TYPE=Debug
CMAKE_BACKEND_WGPU := -DWEBGPU_BACKEND=wgpu
CMAKE_BACKEND_DAWN := -DWEBGPU_BACKEND=dawn
CMAKE_ASAN := -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"

# Gradle options (path relative to build-tools/android/)
GRADLE_OPTS_WGPU_DEBUG := --project-cache-dir=../../$(BUILD_DIR_ANDROID_WGPU_DEBUG)/.gradle
GRADLE_OPTS_WGPU_RELEASE := --project-cache-dir=../../$(BUILD_DIR_ANDROID_WGPU_RELEASE)/.gradle
GRADLE_OPTS_DAWN_DEBUG := --project-cache-dir=../../$(BUILD_DIR_ANDROID_DAWN_DEBUG)/.gradle
GRADLE_OPTS_DAWN_RELEASE := --project-cache-dir=../../$(BUILD_DIR_ANDROID_DAWN_RELEASE)/.gradle

# Default target - show help
.PHONY: all
all: help

#=============================================================================
# Desktop - wgpu backend
#=============================================================================

.PHONY: config-desktop-wgpu-debug
config-desktop-wgpu-debug: ## Configure desktop wgpu debug build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP_WGPU_DEBUG) $(CMAKE_GENERATOR) $(CMAKE_DEBUG) $(CMAKE_BACKEND_WGPU) $(CMAKE_COMPILER_LAUNCHER)

.PHONY: config-desktop-wgpu-release
config-desktop-wgpu-release: ## Configure desktop wgpu release build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP_WGPU_RELEASE) $(CMAKE_GENERATOR) $(CMAKE_RELEASE) $(CMAKE_BACKEND_WGPU) $(CMAKE_COMPILER_LAUNCHER)

.PHONY: build-desktop-wgpu-debug
build-desktop-wgpu-debug: ## Build desktop wgpu debug
	@if [ ! -f "$(BUILD_DIR_DESKTOP_WGPU_DEBUG)/build.ninja" ]; then $(MAKE) config-desktop-wgpu-debug; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_WGPU_DEBUG) $(CMAKE_PARALLEL)

.PHONY: build-desktop-wgpu-release
build-desktop-wgpu-release: ## Build desktop wgpu release
	@if [ ! -f "$(BUILD_DIR_DESKTOP_WGPU_RELEASE)/build.ninja" ]; then $(MAKE) config-desktop-wgpu-release; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_WGPU_RELEASE) $(CMAKE_PARALLEL)

.PHONY: run-desktop-wgpu-debug
run-desktop-wgpu-debug: build-desktop-wgpu-debug ## Run desktop wgpu debug build
	./$(BUILD_DIR_DESKTOP_WGPU_DEBUG)/yetty

.PHONY: run-desktop-wgpu-release
run-desktop-wgpu-release: build-desktop-wgpu-release ## Run desktop wgpu release build
	./$(BUILD_DIR_DESKTOP_WGPU_RELEASE)/yetty

.PHONY: test-desktop-wgpu-debug
test-desktop-wgpu-debug: ## Run desktop wgpu debug tests
	@if [ ! -f "$(BUILD_DIR_DESKTOP_WGPU_DEBUG)/build.ninja" ]; then $(MAKE) config-desktop-wgpu-debug; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_WGPU_DEBUG) --target yetty_tests $(CMAKE_PARALLEL)
	./$(BUILD_DIR_DESKTOP_WGPU_DEBUG)/test/ut/yetty_tests

#=============================================================================
# Desktop - dawn backend
#=============================================================================

.PHONY: config-desktop-dawn-debug
config-desktop-dawn-debug: ## Configure desktop dawn debug build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP_DAWN_DEBUG) $(CMAKE_GENERATOR) $(CMAKE_DEBUG) $(CMAKE_BACKEND_DAWN) $(CMAKE_COMPILER_LAUNCHER)
	@ln -sfn $(BUILD_DIR_DESKTOP_DAWN_DEBUG)/compile_commands.json compile_commands.json

.PHONY: config-desktop-dawn-release
config-desktop-dawn-release: ## Configure desktop dawn release build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP_DAWN_RELEASE) $(CMAKE_GENERATOR) $(CMAKE_RELEASE) $(CMAKE_BACKEND_DAWN) $(CMAKE_COMPILER_LAUNCHER)
	@ln -sfn $(BUILD_DIR_DESKTOP_DAWN_RELEASE)/compile_commands.json compile_commands.json

.PHONY: build-desktop-dawn-debug
build-desktop-dawn-debug: ## Build desktop dawn debug
	@if [ ! -f "$(BUILD_DIR_DESKTOP_DAWN_DEBUG)/build.ninja" ]; then $(MAKE) config-desktop-dawn-debug; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_DAWN_DEBUG) $(CMAKE_PARALLEL)

.PHONY: build-desktop-dawn-release
build-desktop-dawn-release: ## Build desktop dawn release
	@if [ ! -f "$(BUILD_DIR_DESKTOP_DAWN_RELEASE)/build.ninja" ]; then $(MAKE) config-desktop-dawn-release; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_DAWN_RELEASE) $(CMAKE_PARALLEL)

.PHONY: run-desktop-dawn-debug
run-desktop-dawn-debug: build-desktop-dawn-debug ## Run desktop dawn debug build
	./$(BUILD_DIR_DESKTOP_DAWN_DEBUG)/yetty

.PHONY: run-desktop-dawn-release
run-desktop-dawn-release: build-desktop-dawn-release ## Run desktop dawn release build
	./$(BUILD_DIR_DESKTOP_DAWN_RELEASE)/yetty

.PHONY: test-desktop-dawn-debug
test-desktop-dawn-debug: ## Run desktop dawn debug tests
	@if [ ! -f "$(BUILD_DIR_DESKTOP_DAWN_DEBUG)/build.ninja" ]; then $(MAKE) config-desktop-dawn-debug; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_DAWN_DEBUG) --target yetty_tests $(CMAKE_PARALLEL)
	./$(BUILD_DIR_DESKTOP_DAWN_DEBUG)/test/ut/yetty_tests

#=============================================================================
# Desktop - dawn backend with ASAN
#=============================================================================

.PHONY: config-desktop-dawn-asan
config-desktop-dawn-asan: ## Configure desktop dawn ASAN build
	PATH="$(SYSTEM_PATH)" $(CMAKE) -B $(BUILD_DIR_DESKTOP_DAWN_ASAN) $(CMAKE_GENERATOR) $(CMAKE_DEBUG) $(CMAKE_BACKEND_DAWN) $(CMAKE_ASAN) $(CMAKE_COMPILER_LAUNCHER)
	@ln -sfn $(BUILD_DIR_DESKTOP_DAWN_ASAN)/compile_commands.json compile_commands.json

.PHONY: build-desktop-dawn-asan
build-desktop-dawn-asan: ## Build desktop dawn ASAN
	@if [ ! -f "$(BUILD_DIR_DESKTOP_DAWN_ASAN)/build.ninja" ]; then $(MAKE) config-desktop-dawn-asan; fi
	PATH="$(SYSTEM_PATH)" $(CMAKE) --build $(BUILD_DIR_DESKTOP_DAWN_ASAN) $(CMAKE_PARALLEL)

.PHONY: run-desktop-dawn-asan
run-desktop-dawn-asan: build-desktop-dawn-asan ## Run desktop dawn ASAN build
	./$(BUILD_DIR_DESKTOP_DAWN_ASAN)/yetty

#=============================================================================
# Android - wgpu backend
#=============================================================================

.PHONY: config-android-wgpu-debug
config-android-wgpu-debug: ## Configure Android wgpu debug build
	@$(MAKE) _android-wgpu-deps-debug

.PHONY: config-android-wgpu-release
config-android-wgpu-release: ## Configure Android wgpu release build
	@$(MAKE) _android-wgpu-deps-release

.PHONY: build-android-wgpu-debug
build-android-wgpu-debug: ## Build Android wgpu debug APK
	@$(MAKE) _android-wgpu-deps-debug
	WEBGPU_BACKEND=wgpu ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_DEBUG) nix develop .#android --command bash -c "cd build-tools/android && ./gradlew $(GRADLE_OPTS_WGPU_DEBUG) assembleDebug"

.PHONY: build-android-wgpu-release
build-android-wgpu-release: ## Build Android wgpu release APK
	@$(MAKE) _android-wgpu-deps-release
	WEBGPU_BACKEND=wgpu ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_RELEASE) nix develop .#android --command bash -c "cd build-tools/android && ./gradlew $(GRADLE_OPTS_WGPU_RELEASE) assembleRelease"

.PHONY: test-android-wgpu-debug
test-android-wgpu-debug: build-android-wgpu-debug ## Install and run Android wgpu debug build
	adb install -r $(BUILD_DIR_ANDROID_WGPU_DEBUG)/app/outputs/apk/debug/app-debug.apk
	adb shell am start -n com.yetty.terminal/android.app.NativeActivity

.PHONY: test-android-wgpu-release
test-android-wgpu-release: build-android-wgpu-release ## Install and run Android wgpu release build
	adb install -r $(BUILD_DIR_ANDROID_WGPU_RELEASE)/app/outputs/apk/release/app-release-unsigned.apk
	adb shell am start -n com.yetty.terminal/android.app.NativeActivity

#=============================================================================
# Android - dawn backend
#=============================================================================

.PHONY: config-android-dawn-debug
config-android-dawn-debug: ## Configure Android dawn debug build
	@$(MAKE) _android-dawn-deps-debug

.PHONY: config-android-dawn-release
config-android-dawn-release: ## Configure Android dawn release build
	@$(MAKE) _android-dawn-deps-release

.PHONY: build-android-dawn-debug
build-android-dawn-debug: ## Build Android dawn debug APK
	@$(MAKE) _android-dawn-deps-debug
	WEBGPU_BACKEND=dawn ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_DAWN_DEBUG) nix develop .#android --command bash -c "cd build-tools/android && ./gradlew $(GRADLE_OPTS_DAWN_DEBUG) assembleDebug"

.PHONY: build-android-dawn-release
build-android-dawn-release: ## Build Android dawn release APK
	@$(MAKE) _android-dawn-deps-release
	WEBGPU_BACKEND=dawn ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_DAWN_RELEASE) nix develop .#android --command bash -c "cd build-tools/android && ./gradlew $(GRADLE_OPTS_DAWN_RELEASE) assembleRelease"

.PHONY: test-android-dawn-debug
test-android-dawn-debug: build-android-dawn-debug ## Install and run Android dawn debug build
	adb install -r $(BUILD_DIR_ANDROID_DAWN_DEBUG)/app/outputs/apk/debug/app-debug.apk
	adb shell am start -n com.yetty.terminal/android.app.NativeActivity

.PHONY: test-android-dawn-release
test-android-dawn-release: build-android-dawn-release ## Install and run Android dawn release build
	adb install -r $(BUILD_DIR_ANDROID_DAWN_RELEASE)/app/outputs/apk/release/app-release-unsigned.apk
	adb shell am start -n com.yetty.terminal/android.app.NativeActivity

#=============================================================================
# WebAssembly (browser native WebGPU - Dawn in Chrome)
#=============================================================================

.PHONY: config-webasm-dawn-debug
config-webasm-dawn-debug: ## Configure WebAssembly debug build
	nix develop .#web --command bash -c '\
		export EMCC_SKIP_SANITY_CHECK=1 && \
		emcmake cmake -B $(BUILD_DIR_WEBASM_DAWN_DEBUG) $(CMAKE_GENERATOR) \
			-DCMAKE_BUILD_TYPE=Debug'

.PHONY: config-webasm-dawn-release
config-webasm-dawn-release: ## Configure WebAssembly release build
	nix develop .#web --command bash -c '\
		export EMCC_SKIP_SANITY_CHECK=1 && \
		emcmake cmake -B $(BUILD_DIR_WEBASM_DAWN_RELEASE) $(CMAKE_GENERATOR) \
			-DCMAKE_BUILD_TYPE=MinSizeRel'

.PHONY: build-webasm-dawn-debug
build-webasm-dawn-debug: ## Build WebAssembly debug
	@if [ ! -f "$(BUILD_DIR_WEBASM_DAWN_DEBUG)/build.ninja" ]; then $(MAKE) config-webasm-dawn-debug; fi
	nix develop .#web --command bash -c 'cmake --build $(BUILD_DIR_WEBASM_DAWN_DEBUG) --target yetty $(CMAKE_PARALLEL)'
	@cp build-tools/web/index.html build-tools/web/serve.py $(BUILD_DIR_WEBASM_DAWN_DEBUG)/
	@bash build-tools/web/build-fs/build-vfsync.sh $(BUILD_DIR_WEBASM_DAWN_DEBUG)

.PHONY: build-webasm-dawn-release
build-webasm-dawn-release: ## Build WebAssembly release (CDB generation handled by CMake)
	@if [ ! -f "$(BUILD_DIR_WEBASM_DAWN_RELEASE)/build.ninja" ]; then $(MAKE) config-webasm-dawn-release; fi
	nix develop .#web --command bash -c 'cmake --build $(BUILD_DIR_WEBASM_DAWN_RELEASE) --target yetty $(CMAKE_PARALLEL)'
	@cp build-tools/web/index.html build-tools/web/serve.py $(BUILD_DIR_WEBASM_DAWN_RELEASE)/
	@bash build-tools/web/build-fs/build-vfsync.sh $(BUILD_DIR_WEBASM_DAWN_RELEASE)

.PHONY: run-webasm-dawn-debug
run-webasm-dawn-debug: build-webasm-dawn-debug ## Serve WebAssembly debug build
	python3 $(BUILD_DIR_WEBASM_DAWN_DEBUG)/serve.py 8000 $(BUILD_DIR_WEBASM_DAWN_DEBUG)

.PHONY: run-webasm-dawn-release
run-webasm-dawn-release: build-webasm-dawn-release ## Serve WebAssembly release build
	python3 $(BUILD_DIR_WEBASM_DAWN_RELEASE)/serve.py 8000 $(BUILD_DIR_WEBASM_DAWN_RELEASE)


#=============================================================================
# Clean
#=============================================================================

.PHONY: clean
clean: ## Clean all build directories
	rm -rf $(BUILD_DIR_DESKTOP_WGPU_DEBUG) $(BUILD_DIR_DESKTOP_WGPU_RELEASE) \
	       $(BUILD_DIR_DESKTOP_DAWN_DEBUG) $(BUILD_DIR_DESKTOP_DAWN_RELEASE) \
	       $(BUILD_DIR_DESKTOP_DAWN_ASAN) \
	       $(BUILD_DIR_ANDROID_WGPU_DEBUG) $(BUILD_DIR_ANDROID_WGPU_RELEASE) \
	       $(BUILD_DIR_ANDROID_DAWN_DEBUG) $(BUILD_DIR_ANDROID_DAWN_RELEASE) \
	       $(BUILD_DIR_WEBASM_DAWN_DEBUG) $(BUILD_DIR_WEBASM_DAWN_RELEASE) \
	       build-desktop build-android build-webasm \
	       build-desktop-debug build-desktop-release \
	       build-android-debug build-android-release

#=============================================================================
# Internal targets (not shown in help)
#=============================================================================

.PHONY: _android-wgpu-deps-debug
_android-wgpu-deps-debug:
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_DEBUG) bash build-tools/android/build-wgpu.sh
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_DEBUG) bash build-tools/android/build-toybox.sh

.PHONY: _android-wgpu-deps-release
_android-wgpu-deps-release:
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_RELEASE) bash build-tools/android/build-wgpu.sh
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_WGPU_RELEASE) bash build-tools/android/build-toybox.sh

.PHONY: _android-dawn-deps-debug
_android-dawn-deps-debug:
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_DAWN_DEBUG) bash build-tools/android/build-dawn.sh

.PHONY: _android-dawn-deps-release
_android-dawn-deps-release:
	@cd $(CURDIR) && ANDROID_BUILD_DIR=$(CURDIR)/$(BUILD_DIR_ANDROID_DAWN_RELEASE) bash build-tools/android/build-dawn.sh

#=============================================================================
# Help
#=============================================================================

.PHONY: help
help:
	@echo "Yetty Build System"
	@echo ""
	@echo "Usage: make <target>"
	@echo ""
	@echo "WebGPU Backends:"
	@echo "  wgpu  - wgpu-native (Rust implementation, required for pygfx)"
	@echo "  dawn  - Dawn (Google's C++ implementation)"
	@echo ""
	@echo "Targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo ""
	@echo "Build outputs:"
	@echo "  build-desktop-{wgpu,dawn}-{debug,release}/yetty"
	@echo "  build-tools/android/app/build/outputs/apk/{debug,release}/"
	@echo "  build-webasm-{debug,release}/yetty.html"
