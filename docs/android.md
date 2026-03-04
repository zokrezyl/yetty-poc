# Yetty on Android

## Core Architecture

**Yetty uses an internal telnet client to connect to toybox telnetd for shell access.**

```
┌─────────────────────────────────────────────────────────────────┐
│ Yetty Android App                                               │
│                                                                 │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ Terminal (GPU-rendered, WebGPU)                         │   │
│   │     │                                                   │   │
│   │     ▼                                                   │   │
│   │ TelnetPtyReader                                         │   │
│   │     │                                                   │   │
│   │     ▼                                                   │   │
│   │ TelnetClient (internal, async TCP + telnet protocol)    │   │
│   └─────────────────────────────────────────────────────────┘   │
│             │                                                   │
│             │ TCP localhost connection                          │
│             ▼                                                   │
│   ┌─────────────────────────────────────────────────────────┐   │
│   │ toybox telnetd (bundled, runs on port 8024)             │   │
│   │     │                                                   │   │
│   │     ▼                                                   │   │
│   │ toybox sh (shell, invoked via symlink)                  │   │
│   └─────────────────────────────────────────────────────────┘   │
│                                                                 │
│   Alternative: Connect to Termux telnetd on port 8023           │
└─────────────────────────────────────────────────────────────────┘
```

## How It Works

1. **Yetty starts** and creates a `TelnetPtyReader`
2. **Try Termux first**: Attempt connection to `localhost:8023` (Termux telnetd)
3. **Fallback to toybox**: If Termux unavailable:
   - Fork and exec `toybox telnetd -l /path/to/sh -p 8024 -F`
   - Wait for telnetd to start
   - Connect internal telnet client to `localhost:8024`
4. **Telnet protocol negotiation**: NAWS (window size), ECHO, SGA
5. **Shell session**: PTY data flows via telnet connection

## Why Telnet?

Android's app sandbox prevents direct PTY access:
- Apps cannot fork/exec shells directly with proper PTY
- Apps cannot access other apps' processes or files
- TCP localhost connections ARE allowed between apps

Telnet provides:
- Full PTY semantics via protocol
- Window resize via NAWS (RFC 1073)
- Terminal type via TTYPE (RFC 1091)
- Works within Android sandbox restrictions

## Toybox Integration

Toybox is bundled in the APK as `libtoybox.so` (in jniLibs):
- **Location**: `/data/app/.../lib/<arch>/libtoybox.so`
- **Executable from lib dir**: Android SELinux allows execution from native lib directory
- **Multicall binary**: argv[0] determines which applet runs

### Login Wrapper

telnetd passes `-h hostname` to the login program, but toybox sh doesn't support `-h`.
Solution: `liblogin.so` wrapper that ignores arguments and execs toybox sh.

```
telnetd -l liblogin.so
    │
    └── liblogin.so (ignores -h hostname)
            │
            └── execv("libtoybox.so", {"sh", NULL})
                    │
                    └── toybox sh (argv[0]="sh" → shell mode)
```

### Files

| File | Location | Purpose |
|------|----------|---------|
| `libtoybox.so` | APK lib/ | Toybox multicall binary |
| `liblogin.so` | APK lib/ | Wrapper: ignores -h, execs sh |
| `libtelnetd.so` | APK lib/ | Symlink to libtoybox.so |
| `toybox` | App assets | Legacy, extracted on first run |

## Connection Strategy

```
PtyReader::create()
    │
    ├── Try connect to localhost:8023 (Termux)
    │       │
    │       ├── Success → Use Termux shell
    │       │
    │       └── Fail → Continue to fallback
    │
    └── Start bundled toybox telnetd
            │
            ├── fork()
            │     │
            │     └── Child: execl(libtoybox.so, "telnetd", "-l", sh_symlink, "-p", "8024", "-F")
            │
            └── Parent: wait 200ms, then connect to localhost:8024
```

## Termux Integration (Optional)

For a full Linux environment, install Termux and start telnetd:

```bash
# In Termux
pkg update && pkg install busybox
busybox telnetd -l /bin/bash -p 8023 -F
```

Yetty will automatically connect to Termux instead of using bundled toybox.

### Auto-start telnetd on Boot

```bash
# Install Termux:Boot from F-Droid
mkdir -p ~/.termux/boot
cat > ~/.termux/boot/start-telnetd.sh << 'EOF'
#!/data/data/com.termux/files/usr/bin/bash
busybox telnetd -l /bin/bash -p 8023
EOF
chmod +x ~/.termux/boot/start-telnetd.sh
```

## Toybox Commands

Toybox provides ~200 Unix commands:

| Category | Commands |
|----------|----------|
| Shell | `sh` |
| Files | `ls`, `cp`, `mv`, `rm`, `mkdir`, `cat`, `grep`, `find`, `chmod` |
| Text | `sed`, `sort`, `uniq`, `wc`, `head`, `tail`, `vi` |
| Process | `ps`, `kill`, `sleep`, `top` |
| Network | `netcat`, `ping`, `ifconfig`, `wget`, `telnetd` |
| System | `uname`, `id`, `whoami`, `env`, `date`, `df`, `mount` |

## Telnet Protocol

Implemented features:

| Feature | RFC | Description |
|---------|-----|-------------|
| Binary | 856 | 8-bit clean for UTF-8 |
| Echo | 857 | Server-side echo |
| SGA | 858 | Suppress Go-Ahead (full-duplex) |
| NAWS | 1073 | Window size negotiation |
| TTYPE | 1091 | Terminal type (xterm-256color) |

## Configuration

| Setting | Value | Notes |
|---------|-------|-------|
| Termux port | 8023 | Checked first |
| Toybox port | 8024 | Fallback |
| Terminal type | xterm-256color | Sent via TTYPE |
| Host | 127.0.0.1 | Localhost only |

## Troubleshooting

### Shell exits immediately

Check if toybox sh works:
```bash
adb shell run-as <package> /data/user/0/<package>/files/sh -c "echo test"
```

Check SELinux denials:
```bash
adb logcat | grep -E "(avc|denied)"
```

### Connection refused

Verify telnetd is running:
```bash
adb shell "ps -A | grep telnet"
```

### No shell prompt

Check symlink:
```bash
adb shell run-as <package> ls -la /data/user/0/<package>/files/sh
```

### Termux not detected

Start telnetd in Termux:
```bash
pgrep telnetd || busybox telnetd -l /bin/bash -p 8023
```

## Security

- Telnet is unencrypted but localhost-only (data never leaves device)
- No authentication (single-user device assumption)
- For remote access, use SSH (future roadmap)

## Source Files

| File | Purpose |
|------|---------|
| `src/yetty/pty-reader-android.cpp` | Android PTY factory, toybox startup |
| `src/yetty/telnet/telnet-client.cpp` | Telnet protocol client |
| `src/yetty/telnet/telnet-pty-reader.cpp` | PTY interface over telnet |
| `build-tools/android/build-toybox.sh` | Builds toybox for Android |
