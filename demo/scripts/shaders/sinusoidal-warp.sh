#!/bin/bash
# Shader Card: Sinusoidal warp - texture deformation effect by Shane
# Note: This shader requires a texture (channel0) which needs special handling
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/shader/sinusoidal-warp.wgsl)
# TODO: channel0 texture support via card args
printf '\033]666666;run -c shadertoy -x 0 -y 0 -w 60 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
