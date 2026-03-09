# LibWeb-to-YDraw Build System
# Uses system compilers for distcc compatibility while running in nix shell for libraries

# Build directory
BUILD_DIR := build

# Distributed build with ccache + distcc
DISTCC_HOSTS ?= localhost 192.168.1.10
export DISTCC_HOSTS
ifneq ($(shell which distcc 2>/dev/null),)
export CCACHE_PREFIX := distcc
endif

# Force system compiler (for distcc compatibility)
# distcc doesn't understand /nix/store/... paths
SYSTEM_CXX := /usr/bin/clang++-19
SYSTEM_CC := /usr/bin/clang-19
CMAKE_COMPILER := -DCMAKE_C_COMPILER=$(SYSTEM_CC) -DCMAKE_CXX_COMPILER=$(SYSTEM_CXX)
CMAKE_COMPILER_LAUNCHER := -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

# Parallel jobs
PARALLEL_JOBS ?=
CMAKE_PARALLEL := $(if $(PARALLEL_JOBS),--parallel $(PARALLEL_JOBS),--parallel)

# Ladybird source location
LADYBIRD_SOURCE := $(CURDIR)/../../tmp/ladybird

.PHONY: all
all: build

.PHONY: config
config: ## Configure the build (runs in nix shell for library paths)
	@mkdir -p $(BUILD_DIR)
	nix develop ../../#default --command cmake -B $(BUILD_DIR) -G Ninja \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_LIBWEB=ON \
		-DLADYBIRD_SOURCE_DIR=$(LADYBIRD_SOURCE) \
		$(CMAKE_COMPILER) \
		$(CMAKE_COMPILER_LAUNCHER)

.PHONY: build
build: ## Build everything (runs in nix shell for library paths)
	@if [ ! -f "$(BUILD_DIR)/build.ninja" ]; then $(MAKE) config; fi
	nix develop ../../#default --command cmake --build $(BUILD_DIR) $(CMAKE_PARALLEL)

.PHONY: clean
clean: ## Clean build directory
	rm -rf $(BUILD_DIR)

.PHONY: distcc-test
distcc-test: ## Test distcc connectivity
	@echo "Testing distcc to $(DISTCC_HOSTS)..."
	@echo 'int main() { return 0; }' | distcc gcc -x c -c - -o /dev/null && echo "distcc OK" || echo "distcc FAILED"

.PHONY: help
help: ## Show this help
	@echo "LibWeb-to-YDraw Build System"
	@echo ""
	@echo "Usage: make <target>"
	@echo ""
	@echo "Targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'
	@echo ""
	@echo "Environment:"
	@echo "  DISTCC_HOSTS=$(DISTCC_HOSTS)"
	@echo "  CCACHE_PREFIX=$(CCACHE_PREFIX)"
