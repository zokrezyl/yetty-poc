# Yetty on Windows

Yetty runs natively on Windows 10+ using ConPTY (Console Pseudo Terminal), the same technology that powers Windows Terminal.

## Requirements

- Windows 10 version 1809 or later (ConPTY support)
- Visual Studio 2022 or compatible C++ toolchain
- CMake 3.20+

## Building

```powershell
# Clone and build
git clone https://github.com/user/yetty.git
cd yetty

# Configure with Visual Studio generator
cmake -B build-windows -G "Visual Studio 17 2022"

# Build Release
cmake --build build-windows --config Release

# Or Debug
cmake --build build-windows --config Debug
```

## Running

```powershell
# Run with default shell (cmd.exe or COMSPEC)
.\build-windows\Release\yetty.exe

# Run with PowerShell
$env:SHELL = "powershell.exe"
.\build-windows\Release\yetty.exe

# Run with PowerShell 7
$env:SHELL = "pwsh.exe"
.\build-windows\Release\yetty.exe

# Run with WSL
$env:SHELL = "wsl.exe"
.\build-windows\Release\yetty.exe

# Run specific WSL distro
$env:SHELL = "wsl.exe -d Ubuntu"
.\build-windows\Release\yetty.exe
```

## Shell Selection

Yetty determines the shell in this order:

1. `SHELL` environment variable (if set)
2. `COMSPEC` environment variable (Windows default, usually `C:\Windows\System32\cmd.exe`)
3. Fallback to `cmd.exe`

### Examples

```powershell
# Use Git Bash
$env:SHELL = "C:\Program Files\Git\bin\bash.exe"
.\yetty.exe

# Use Cygwin
$env:SHELL = "C:\cygwin64\bin\bash.exe"
.\yetty.exe

# Use MSYS2
$env:SHELL = "C:\msys64\usr\bin\bash.exe"
.\yetty.exe
```

## Command Execution

Run a specific command instead of an interactive shell:

```powershell
# Run a command
.\yetty.exe -c "dir /w"

# Run PowerShell command
$env:SHELL = "powershell.exe"
.\yetty.exe -c "Get-Process | Select-Object -First 10"
```

## ConPTY Features

The Windows implementation supports:

- Full VT100/xterm escape sequences
- 256 colors and true color (24-bit)
- Unicode/UTF-8 text
- Window resize (SIGWINCH equivalent)
- Mouse input
- Clipboard integration

## Telnet Mode

Connect to a remote telnet server (or local server like WSL):

```powershell
# Connect to local telnet server
.\yetty.exe --telnet 127.0.0.1:23

# Connect to remote server
.\yetty.exe --telnet example.com:23
```

## Troubleshooting

### "Failed to create pseudo console"

This error occurs on older Windows versions without ConPTY support. Upgrade to Windows 10 version 1809 or later.

### Unicode characters not displaying

Ensure your console font supports the characters. Try:
- Cascadia Code
- Consolas
- DejaVu Sans Mono

### Colors not working

Some older shells don't enable VT processing by default. In cmd.exe:
```cmd
reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1
```

### WSL performance

For best performance with WSL, consider using `wsl.exe` directly rather than running bash through it:
```powershell
$env:SHELL = "wsl.exe"  # Faster than wsl.exe bash
```

## Known Limitations

- No native SSH client yet (use `--telnet` or run ssh inside the shell)
- Clipboard uses Windows APIs (Ctrl+C/Ctrl+V work in most shells)
