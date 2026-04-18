#!/bin/bash
# Complex flowchart with edge labels
DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/../../../.."

PAYLOAD=$(cat <<'EOF' | base64 -w0
graph TD
    Start[Start] --> Init[Initialize]
    Init --> Load[Load Config]
    Load --> Check{Config Valid?}
    Check -->|Yes| Process[Process Data]
    Check -->|No| Error[Show Error]
    Error --> Load
    Process --> Save[(Save to DB)]
    Save --> Notify((Notify))
    Notify --> End[End]
EOF
)

printf '\033]666666;run -c mermaid -x 0 -y 0 -w 50 -h 35 -r;;%s\033\\' "$PAYLOAD"
echo
