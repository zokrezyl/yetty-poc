#!/bin/bash
# Demo: Simple sine wave
cd "$(dirname "$0")/../../.."
YTRACE_DEFAULT_ON=yes timeout 3 ./build-desktop-dawn-release/tools/card-runner/card-runner yplot "{f=sin(x) @range=-pi..pi,-1..1 @grid=on}" 2>&1
