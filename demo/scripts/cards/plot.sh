#!/bin/bash
# Plot Card: Interactive chart using the card system (shader-glyph based)
#
# Expects simple comma-separated values in payload.

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../.."

# Generate sine wave data and emit OSC sequence with base94-encoded payload
python3 -c "
import sys
import math
sys.path.insert(0, 'tools/yetty-client')
from core import base94

# Generate sine wave data
points = 100
values = [math.sin(x * math.pi * 2 / points) for x in range(points)]
payload = ','.join(f'{v:.3f}' for v in values)

# Base94 encode the payload
encoded = base94.encode_string(payload)

# Plugin args
plugin_args = '--type line --grid --axes'

# Emit OSC sequence
# Format: ESC ] 666666;create -p plot -x 0 -y 0 -w W -h H -r;plugin-args;payload ESC \\
sys.stdout.write(f'\x1b]666666;create -p plot -x 0 -y 0 -w 60 -h 20 -r;{plugin_args};{encoded}\x1b\\\\')
sys.stdout.flush()
"
echo  # newline after the card
