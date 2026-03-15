# Testing

## Log-Based Testing

Log-based testing validates runtime behavior by analyzing log output. This approach is particularly useful for verifying initialization sequences, component creation order, and ensuring critical code paths are executed.

### Strategy

The strategy uses the `ytest` macro from the [ytrace](https://github.com/zokrezyl/ytrace) library. Unlike regular logging macros (`yinfo`, `ydebug`), `ytest` is specifically designed for testing:

1. **Unique context** - Each test point has a unique identifier (e.g., `config-created`, `shader-manager-created`)
2. **Counter** - Each call site maintains a static counter, useful for verifying repeated operations
3. **Info level** - Test points log at info level, making them easy to filter from debug noise

Output format:
```
[test:<context>:<counter>] <message>
```

Example:
```
[2026-03-15 19:26:18.052] [info] [config.cpp:585] [test:config-created:1] Config created successfully
[2026-03-15 19:26:18.193] [info] [shader-manager.cpp:329] [test:shader-manager-created:1] ShaderManager created successfully
```

### Adding Test Points

Use `ytest` at critical points in your code:

```cpp
#include <ytrace/ytrace.hpp>

Result<Config::Ptr> Config::createImpl(...) {
    // ... initialization code ...
    ytest("config-created", "Config created successfully");
    return Ok(std::move(impl));
}
```

Guidelines for context names:
- Use lowercase with hyphens: `component-action`
- Be specific: `font-manager-created` not `init`
- Include the action: `assets-extraction-started`, `assets-extraction-complete`

### Running Tests

1. **Capture log output** - Run yetty with tracing enabled:
   ```bash
   YTRACE_DEFAULT_ON=yes ./build-desktop-ytrace-release/yetty 2>&1 > /tmp/yetty.log
   ```

2. **Validate test points** - Run the validation script:
   ```bash
   ./test/scripts/runtime-test-points/test.sh /tmp/yetty.log
   ```

### Expected Sequence File

The test validates against `test/scripts/runtime-test-points/expected-startup-sequence.txt`:

```
config-created:1
assets-shaders-registered:1
assets-fonts-registered:1
...
yetty-created:1
```

Each line is `<context>:<expected-counter>`.

### Test Failures

The test fails if:
1. **Missing test points** - An expected test point doesn't appear in the log
2. **Wrong order** - Test points appear but not in the expected sequence
3. **Unexpected test points** - New `ytest` calls appear that aren't in the expected sequence

When adding new `ytest` calls, update `expected-startup-sequence.txt` accordingly.

### Build Configuration

The `ytest` macro requires `YTRACE_ENABLE_YTEST=1` (enabled by default in ytrace v0.0.13+).

For builds with tracing:
```bash
make build-desktop-ytrace-release
```
