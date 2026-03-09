#!/usr/bin/env bash
# Build libweb-to-ydraw using nix remote builder
# See docs/nix-remote-build.md for configuration

set -e
cd "$(dirname "$0")"

# Force all builds to remote (--max-jobs 0), show logs (-L)
nix build --max-jobs 0 -L "$@"
