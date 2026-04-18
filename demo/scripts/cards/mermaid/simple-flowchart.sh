#!/bin/bash
# Simple Mermaid flowchart demo
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(cat <<'EOF' | base64 -w0
graph TD
    A[Start] --> B{Is it working?}
    B -->|Yes| C[Great!]
    B -->|No| D[Debug]
    D --> B
    C --> E((End))
EOF
)

printf '\033]666666;run -c mermaid -x 0 -y 0 -w 40 -h 25 -r;;%s\033\\' "$PAYLOAD"
echo
