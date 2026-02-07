#!/bin/bash
# JDraw Giant Demo (binary): ~17,500 SDF primitives with GPU compute shader tile culling
# Uses pre-converted binary (tools/yaml2bin) to skip slow YAML parsing

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/cards/x-draw/giant.bin)
printf '\033]666666;run -c jdraw -x 0 -y 0 -w 80 -h 25 -r;bin;%s\033\\' "$PAYLOAD"
echo
