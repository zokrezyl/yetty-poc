#!/bin/bash
# Ymery Card: YAML-driven ImGui widget demo

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/ymery/simple.yaml)
printf '\033]666666;run -c ymery -x 0 -y 0 -w 60 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
