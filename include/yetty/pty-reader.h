#pragma once

#include <yetty/result.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace yetty {

struct PtyConfig {
    std::string shell;           // Shell path (desktop) or VM config (webasm)
    uint32_t cols = 80;
    uint32_t rows = 24;
};

/**
 * PtyReader - Abstraction for reading from PTY/terminal source
 *
 * Desktop: Direct read from PTY fd (fast, non-blocking)
 * Webasm: Read from internal buffer filled by JS/VM
 *
 * Terminal controls reading pace:
 * - Normal data: read chunk, process, allow rendering
 * - OSC data: keep reading until terminator (could be 2000MB)
 */
class PtyReader {
public:
    using Ptr = std::shared_ptr<PtyReader>;
    using DataAvailableCallback = std::function<void()>;
    using ExitCallback = std::function<void(int exitCode)>;

    virtual ~PtyReader() = default;

    /**
     * Read up to maxLen bytes into buf.
     * Returns actual bytes read, 0 if no data available.
     * NON-BLOCKING - returns immediately if no data.
     */
    virtual size_t read(char* buf, size_t maxLen) = 0;

    /**
     * Write data to PTY input.
     */
    virtual void write(const char* data, size_t len) = 0;

    /**
     * Resize PTY dimensions.
     */
    virtual void resize(uint32_t cols, uint32_t rows) = 0;

    /**
     * Check if PTY/process still running.
     */
    virtual bool isRunning() const = 0;

    /**
     * Stop the PTY/process.
     */
    virtual void stop() = 0;

    /**
     * Set callback for when data becomes available.
     * Desktop: triggered by poll event on PTY fd
     * Webasm: triggered when JS pushes data
     */
    virtual void setDataAvailableCallback(DataAvailableCallback cb) = 0;

    /**
     * Set callback for when process exits.
     */
    virtual void setExitCallback(ExitCallback cb) = 0;

    /**
     * Factory method - creates platform-appropriate implementation.
     */
    static Result<Ptr> create(const PtyConfig& config);
};

} // namespace yetty
