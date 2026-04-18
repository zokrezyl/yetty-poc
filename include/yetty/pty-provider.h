#pragma once

#include <yetty/result.hpp>
#include <yetty/base/event-listener.h>
#include <functional>
#include <memory>
#include <string>

namespace yetty {

/**
 * PTYProvider - Abstract interface for terminal backends
 *
 * Implementations:
 * - ForkPTY: Desktop (Linux/macOS) using forkpty()
 * - WebPTY: WebAssembly using postMessage to iframe with JSLinux
 * - AndroidPTY: Android using toybox or other shell
 */
class PTYProvider {
public:
    using Ptr = std::shared_ptr<PTYProvider>;

    // Callback when data arrives from the "shell" (to be displayed)
    using DataCallback = std::function<void(const char* data, size_t len)>;

    // Callback when the shell/process exits
    using ExitCallback = std::function<void(int exitCode)>;

    virtual ~PTYProvider() = default;

    /**
     * Start the shell/emulator process
     * @param shell Shell path (desktop) or VM config URL (web)
     * @param cols Initial terminal columns
     * @param rows Initial terminal rows
     */
    virtual Result<void> start(const std::string& shell, uint32_t cols, uint32_t rows) = 0;

    /**
     * Stop the shell/emulator
     */
    virtual void stop() = 0;

    /**
     * Write data to the shell (user input)
     */
    virtual void write(const char* data, size_t len) = 0;

    /**
     * Resize the terminal
     */
    virtual void resize(uint32_t cols, uint32_t rows) = 0;

    /**
     * Check if the shell is running
     */
    virtual bool isRunning() const = 0;

    /**
     * Set callback for incoming data (from shell)
     */
    virtual void setDataCallback(DataCallback cb) = 0;

    /**
     * Set callback for shell exit
     */
    virtual void setExitCallback(ExitCallback cb) = 0;

    /**
     * Get a unique ID for this PTY instance
     */
    virtual uint32_t getId() const = 0;
};

} // namespace yetty
