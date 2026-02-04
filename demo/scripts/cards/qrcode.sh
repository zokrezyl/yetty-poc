#!/bin/bash
# QRCode Card: GPU-rendered QR code
#
# Usage: ./qrcode.sh [text]
# Default: encodes the yetty GitHub URL

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

TEXT="${1:-https://github.com/zokrezyl/yetty}"

# Use the yetty-client to create a QR code card
python3 -c "
import sys
sys.path.insert(0, 'tools/yetty-client')
from core import base94

text = '''$TEXT'''
encoded = base94.encode_string(text)

# Emit OSC sequence
# Format: ESC ] 666666 ; create -p plugin -x X -y Y -w W -h H -r ; plugin-args ; payload ST
sys.stdout.write(f'\x1b]666666;create -p qrcode -x 0 -y 0 -w 15 -h 15 -r;ecl=M;{encoded}\x1b\\\\')
sys.stdout.flush()
"
echo ""
