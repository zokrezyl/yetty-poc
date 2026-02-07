#!/bin/bash
# Rich-Text Card: Styled text with fonts, colors, sizes from YAML configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../.."

PAYLOAD=$(base64 -w0 < demo/assets/rich-text/simple.yaml)
printf '\033]666666;run -c rich-text -x 2 -y 2 -w 76 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
