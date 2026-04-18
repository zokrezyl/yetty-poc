#!/bin/bash
# Left-to-right flowchart
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(cat <<'EOF' | base64 -w0
graph LR
    A[Input] --> B[Process]
    B --> C[Validate]
    C --> D{OK?}
    D -->|Yes| E[Output]
    D -->|No| B
EOF
)

printf '\033]666666;run -c mermaid -x 0 -y 0 -w 50 -h 15 -r;;%s\033\\' "$PAYLOAD"
echo
