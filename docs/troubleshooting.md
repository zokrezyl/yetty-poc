# Troubleshooting

## Enabling Logging

**All logging is OFF by default.** To enable logging output, set the environment variable:

```bash
YTRACE_DEFAULT_ON=yes ./build-desktop-dawn-debug/yetty
```

Accepted values: `yes`, `1`, `true`

Without this, `ydebug`, `yinfo`, `ywarn`, `yerror`, `ytrace` produce no output.

## Compile-Time Flags

Set these via CMake (`-DFLAG=0`) to disable features at compile time:

| Flag | Default | Description |
|------|---------|-------------|
| `YTRACE_ENABLED` | 1 | Master switch - set to 0 to disable all tracing |
| `YTRACE_NO_CONTROL_SOCKET` | 0 | Disable runtime control socket (auto-enabled for Emscripten) |
| `YTRACE_USE_SPDLOG` | 0 | Use spdlog backend instead of snprintf |
| `YTRACE_ENABLE_YDEBUG` | 1 | Enable `ydebug()` macro |
| `YTRACE_ENABLE_YINFO` | 1 | Enable `yinfo()` macro |
| `YTRACE_ENABLE_YWARN` | 1 | Enable `ywarn()` macro |
| `YTRACE_ENABLE_YERROR` | 0 | Enable `yerror()` macro |
| `YTRACE_ENABLE_YTRACE` | 1 | Enable `ytrace()` macro |
| `YTRACE_ENABLE_YLOG` | 1 | Enable `ylog()` macro |
| `YTRACE_ENABLE_YFUNC` | 1 | Enable `yfunc()` macro |
| `YTRACE_ENABLE_YTIMEIT` | 1 | Enable `ytimeit()` macro |

## Runtime Log Control

While the application is running, you can control logging via Unix socket:

```bash
# Find the socket (created on first log statement)
ls /tmp/ytrace.yetty.*.sock

# Connect and send commands
echo "help" | nc -U /tmp/ytrace.yetty.<pid>.<hash>.sock
```

Commands:
- `list` / `l` - List all trace points with their ON/OFF status
- `enable all` / `ea` - Enable all trace points
- `disable all` / `da` - Disable all trace points
- `enable <spec>` - Enable specific trace points
- `disable <spec>` - Disable specific trace points
- `timers` / `t` - Show timer statistics (from `ytimeit()`)
- `help` / `h` - Show help

## Config Persistence

Trace point states are saved to `~/.cache/ytrace/<exec>-<hash>.config` and restored on next run.

## Programmatic Control

```cpp
yenable_all();           // Enable all trace points
ydisable_all();          // Disable all trace points
yenable_level("debug");  // Enable all debug-level logs
ydisable_file("foo.cpp"); // Disable all logs in foo.cpp
yenable_func("render");  // Enable all logs in function "render"
```

## Performance Timing

Use `ytimeit()` to measure scope duration:

```cpp
void expensiveOperation() {
    ytimeit("expensive-op");  // Logs entry/exit with elapsed time
    // ... work ...
}
```

Timer summary is printed to stderr on exit.
