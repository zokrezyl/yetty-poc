# Plan: PtyReader Abstraction for OSC Handling

## Problem Summary

1. Current terminal.cpp uses callback-based PTYProvider that doesn't allow terminal to control reading pace
2. OSC sequences can be HUGE (up to 2000MB) - must buffer entire sequence before processing
3. Normal data (like `find /` output) must stream through to vterm IMMEDIATELY
4. Need abstraction that works for:
   - Desktop: direct PTY fd read (FAST)
   - Webasm: JS buffer filled by VM

## Current State (Broken)

- Callback-based `onPtyData()` buffers everything when OSC starts
- Breaks streaming for normal commands
- `ycat` broken, `ymux` broken

## Main Branch Behavior (Correct)

```cpp
void readPty() {
    while (true) {
        n = ::read(_ptyMaster, chunk, 64KB);
        if (n <= 0) {
            if (isInOsc()) return;  // wait for more data
            break;
        }
        accumulate in _ptyBuffer;
        scanForOsc(chunk);

        // NOT in OSC: stop after first chunk, allow rendering
        if (!isInOsc() && _ptyBuffer.size() >= 64KB) break;

        // IN OSC: keep reading until terminator found (could be 2000MB)
    }

    processPtyData(_ptyBuffer);  // normal->vterm, OSC->handle
    _ptyBuffer.clear();
}
```

## Solution: PtyReader Interface

### Step 1: Create Interface

**File: `include/yetty/pty-reader.h`**

```cpp
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <functional>

namespace yetty {

class PtyReader {
public:
    using Ptr = std::shared_ptr<PtyReader>;
    using DataAvailableCallback = std::function<void()>;

    virtual ~PtyReader() = default;

    // Read up to maxLen bytes into buf
    // Returns actual bytes read, 0 if no data available
    // NON-BLOCKING for desktop (uses O_NONBLOCK)
    virtual size_t read(char* buf, size_t maxLen) = 0;

    // Write data to PTY
    virtual void write(const char* data, size_t len) = 0;

    // Resize PTY
    virtual void resize(uint32_t cols, uint32_t rows) = 0;

    // Check if PTY/process still running
    virtual bool isRunning() const = 0;

    // Set callback for when data becomes available
    // Desktop: triggered by poll event
    // Webasm: triggered when JS pushes data
    virtual void setDataAvailableCallback(DataAvailableCallback cb) = 0;
};

} // namespace yetty
```

### Step 2: Desktop Implementation

**File: `src/yetty/pty-reader-desktop.cpp`**

- Uses forkpty() to create PTY
- `read()` calls `::read()` on PTY fd directly - LIGHTNING FAST
- fd is O_NONBLOCK
- Poll event on fd triggers DataAvailableCallback
- Terminal then calls read loop

```cpp
size_t PtyReaderDesktop::read(char* buf, size_t maxLen) {
    if (_ptyMaster < 0) return 0;
    ssize_t n = ::read(_ptyMaster, buf, maxLen);
    return (n > 0) ? static_cast<size_t>(n) : 0;
}
```

### Step 3: Webasm Implementation

**File: `src/yetty/pty-reader-web.cpp`**

- Internal ring buffer or queue
- JS/VM calls `pushData()` to fill buffer
- `read()` pulls from internal buffer
- When data pushed, triggers DataAvailableCallback

```cpp
// Called from JS when VM produces output
void PtyReaderWeb::pushData(const char* data, size_t len) {
    _buffer.insert(_buffer.end(), data, data + len);
    if (_dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

size_t PtyReaderWeb::read(char* buf, size_t maxLen) {
    size_t toRead = std::min(maxLen, _buffer.size());
    if (toRead == 0) return 0;
    std::memcpy(buf, _buffer.data(), toRead);
    _buffer.erase(_buffer.begin(), _buffer.begin() + toRead);
    return toRead;
}
```

### Step 4: Terminal Uses PtyReader

**File: `src/yetty/terminal.cpp`**

```cpp
class TerminalImpl : public Terminal {
    PtyReader::Ptr _ptyReader;
    OscScanner _oscScanner;
    std::vector<char> _ptyBuffer;

    void onDataAvailable() {
        readFromPty();
    }

    void readFromPty() {
        static constexpr size_t CHUNK_SIZE = 65536;  // 64KB
        char chunk[CHUNK_SIZE];

        while (true) {
            size_t n = _ptyReader->read(chunk, CHUNK_SIZE);
            if (n == 0) {
                // No more data available right now
                if (_oscScanner.isInOsc()) {
                    // Inside OSC, wait for more data (don't process yet)
                    return;
                }
                break;  // Not in OSC, process what we have
            }

            // Accumulate
            _ptyBuffer.insert(_ptyBuffer.end(), chunk, chunk + n);

            // Scan for OSC state
            _oscScanner.scan(chunk, n);

            // If NOT in OSC and we have enough data, stop to allow rendering
            if (!_oscScanner.isInOsc() && _ptyBuffer.size() >= CHUNK_SIZE) {
                break;
            }
            // If IN OSC, keep reading until terminator (could be 2000MB)
        }

        if (_ptyBuffer.empty()) return;

        // Reset scanner before processing
        _oscScanner.reset();

        // Process: normal data -> vterm, OSC -> handle
        processPtyData(_ptyBuffer.data(), _ptyBuffer.size());
        _ptyBuffer.clear();
    }
};
```

### Step 5: Platform Factory

**File: `src/yetty/pty-reader-factory.cpp`**

```cpp
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config) {
#if YETTY_WEB
    return PtyReaderWeb::create(config);
#elif defined(__ANDROID__)
    return PtyReaderAndroid::create(config);
#else
    return PtyReaderDesktop::create(config);
#endif
}
```

## Files to Create/Modify

### New Files
1. `include/yetty/pty-reader.h` - Interface
2. `src/yetty/pty-reader-desktop.cpp` - Desktop implementation
3. `src/yetty/pty-reader-web.cpp` - Webasm implementation

### Modify
4. `src/yetty/terminal.cpp` - Use PtyReader with proper read loop
5. `src/yetty/CMakeLists.txt` - Add new sources

### Keep
6. `include/yetty/osc-scanner.h` - Already extracted, use in terminal
7. `src/yetty/osc-scanner.cpp` - Already extracted
8. `test/ut/osc/osc_scanner_test.cpp` - Unit tests for scanner

### Remove/Fix
9. `include/yetty/pty-provider.h` - Remove or keep for Android only
10. `src/yetty/platform/glfw-platform.cpp` - Remove ForkPTY class (moved to pty-reader-desktop)
11. `test/ut/osc/osc_integration_test.cpp` - Fix to match real behavior

## Verification

1. `make build-desktop-dawn-release` - builds
2. `./yetty` - terminal works
3. `ycat file.pdf` - works
4. `ymux` - works (creates ygrid cards)
5. `find /` - streams output smoothly
6. `make build-webasm-release` - builds
7. Webasm terminal works in browser

## Key Invariants

1. **Normal data**: Passed to vterm IMMEDIATELY, chunk by chunk
2. **OSC data**: Buffered until COMPLETE (could be 2000MB), then processed
3. **Desktop read**: Direct `::read()` on fd - NO CALLBACKS for data transfer
4. **Webasm read**: Pull from internal buffer - terminal controls pace
5. **Rendering**: Allowed after each normal data chunk (responsive UI)
