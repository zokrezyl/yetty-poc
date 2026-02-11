#!/bin/bash
# Demo of different Mermaid shapes
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(cat <<'EOF' | base64 -w0
graph TD
    A[Rectangle] --> B(Rounded)
    B --> C((Circle))
    C --> D{Diamond}
    D --> E{{Hexagon}}
    E --> F[(Database)]
    F --> G([Stadium])
EOF
)

printf '\033]666666;run -c mermaid -x 0 -y 0 -w 30 -h 30 -r;;%s\033\\' "$PAYLOAD"
echo
