#!/usr/bin/env bash
# Build libweb-to-ydraw using nix remote builder
# See docs/nix-remote-build.md for configuration

set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

cd "$SCRIPT_DIR"

# Force all builds to remote (--max-jobs 0), show logs (-L)
# Output to build-ladybird in project root
nix build --max-jobs 0 -L --out-link "$PROJECT_ROOT/build-ladybird" "$@"
