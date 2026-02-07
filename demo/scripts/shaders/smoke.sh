#!/bin/bash
# Shader Card: Fluid smoke - mouse-interactive smoke simulation
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../../.."

PAYLOAD=$(base64 -w0 < demo/assets/shader/smoke.wgsl)
printf '\033]666666;run -c shadertoy -x 0 -y 0 -w 60 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
