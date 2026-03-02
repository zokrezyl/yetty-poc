# Yetty on Android

Yetty on Android connects to a local telnet server to provide terminal functionality. This is typically Termux running a telnet daemon.

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│ Yetty App                                                       │
│   └── Terminal                                                  │
│         └── TelnetPtyReader                                     │
│               └── TelnetClient (async TCP + telnet protocol)    │
│                     ↓                                           │
│               localhost:8023                                    │
│                     ↓                                           │
│ Termux App                                                      │
│   └── telnetd → /bin/bash                                       │
└─────────────────────────────────────────────────────────────────┘
```

## Why Telnet?

Android's app sandbox prevents direct access to other apps' files and processes. Yetty cannot directly execute Termux's shell. Instead:

1. Termux runs a telnet server inside its sandbox
2. Yetty connects to `localhost:8023` (TCP connection is allowed between apps)
3. Full terminal via telnet protocol with NAWS (window size) support

## Setup Instructions

### 1. Install Termux

Download Termux from F-Droid (recommended) or GitHub releases. The Play Store version is outdated.

### 2. Install and Start telnetd

```bash
# In Termux
pkg update
pkg install busybox

# Start telnet daemon (foreground, for testing)
busybox telnetd -l /bin/bash -p 8023 -F

# Or run in background
busybox telnetd -l /bin/bash -p 8023 &
```

### 3. Auto-start on Boot (Optional)

Create a Termux:Boot script to start telnetd automatically:

```bash
# Install Termux:Boot from F-Droid
pkg install termux-services

# Create boot script
mkdir -p ~/.termux/boot
cat > ~/.termux/boot/start-telnetd.sh << 'EOF'
#!/data/data/com.termux/files/usr/bin/bash
busybox telnetd -l /bin/bash -p 8023
EOF
chmod +x ~/.termux/boot/start-telnetd.sh
```

### 4. Launch Yetty

Yetty automatically connects to `127.0.0.1:8023` on Android.

## Alternative: socat for Better PTY

For improved PTY support (job control, etc.), use socat instead of telnetd:

```bash
pkg install socat

# Start PTY server
socat TCP-LISTEN:8023,reuseaddr,fork EXEC:bash,pty,stderr,setsid,sigint,sane
```

## Telnet Protocol Features

The implementation supports:

| Feature | RFC | Description |
|---------|-----|-------------|
| Binary mode | 856 | 8-bit clean for UTF-8 |
| Echo | 857 | Server-side echo |
| Suppress Go-Ahead | 858 | Full-duplex mode |
| **NAWS** | 1073 | Window size negotiation |
| **TTYPE** | 1091 | Terminal type (xterm-256color) |

## Default Configuration

- **Host**: `127.0.0.1` (localhost)
- **Port**: `8023` (Termux default, avoids root requirement for port 23)
- **Terminal type**: `xterm-256color`

## Troubleshooting

### Connection refused

Ensure telnetd is running in Termux:
```bash
pgrep telnetd || busybox telnetd -l /bin/bash -p 8023
```

### Wrong terminal size

NAWS (window size) should be negotiated automatically. If not, try:
```bash
# In Termux shell
stty rows 40 cols 120
```

### No colors

Verify TERM is set:
```bash
echo $TERM  # Should be xterm-256color
export TERM=xterm-256color
```

## Security Note

Telnet is unencrypted, but since it's localhost-only, data never leaves the device. For remote connections, use SSH instead (see future roadmap).
