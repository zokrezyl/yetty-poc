#include <yetty/pty-reader.h>

#if defined(__ANDROID__)

#include <yetty/platform.h>
#include "telnet/telnet-pty-reader.h"
#include <ytrace/ytrace.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

namespace yetty {

/**
 * Android PTY Factory
 *
 * Strategy:
 * 1. Try connecting to Termux telnetd at localhost:8023
 * 2. If that fails, start bundled toybox telnetd and connect to it
 *
 * Telnet approach works because:
 * - Android apps can connect to localhost TCP (no sandbox restriction)
 * - Toybox includes telnetd command
 * - Full PTY via telnet protocol (NAWS for resize, etc.)
 */
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config, std::shared_ptr<Platform> platform) {
    yinfo("PtyReader::create called");

    // Get paths from Platform
    if (!platform) {
        return Err<Ptr>("Platform not provided");
    }
    const AndroidPaths& paths = platform->getAndroidPaths();
    if (paths.dataDir.empty() || paths.nativeLibDir.empty()) {
        return Err<Ptr>("Android paths not initialized - call platform->setAndroidPaths() first");
    }

    PtyConfig telnetConfig = config;

    // Determine telnet address
    std::string telnetAddr;

    if (!telnetConfig.shell.empty() && telnetConfig.shell[0] != '/') {
        // User specified host:port
        telnetAddr = telnetConfig.shell;
        yinfo("Using user-specified address: {}", telnetAddr);
    } else {
        // Default: try Termux first (port 8023), fallback to local toybox (port 8024)
        telnetAddr = "127.0.0.1:8023";
        yinfo("Using default address: {}", telnetAddr);
    }

    yinfo("Android PtyReader: connecting to {}", telnetAddr);
    telnetConfig.shell = telnetAddr;

    auto reader = std::make_shared<telnet::TelnetPtyReader>();
    yinfo("TelnetPtyReader created, calling init...");
    auto res = reader->init(telnetConfig);
    yinfo("TelnetPtyReader::init returned, success={}", res ? 1 : 0);

    if (!res) {
        yinfo("Initial connection failed, trying toybox fallback");

        // Use libtoybox.so from native lib dir (SELinux allows execution there)
        std::string toyboxExe = paths.nativeLibDir + "/libtoybox.so";
        if (access(toyboxExe.c_str(), X_OK) != 0) {
            return Err<Ptr>("toybox not available or not executable");
        }

        yinfo("toybox available at: {}", toyboxExe);
        yinfo("Termux not available, starting bundled toybox telnetd...");

        // Use liblogin.so as login shell
        // (wrapper that ignores -h flag from telnetd and execs toybox sh)
        std::string shPath = paths.nativeLibDir + "/liblogin.so";
        yinfo("Using login wrapper: {}", shPath);

        // Set up environment for the shell
        // bin directory first (clean names), then native libs, then system
        setenv("HOME", paths.dataDir.c_str(), 1);
        setenv("PATH", (paths.binDir + ":" + paths.nativeLibDir + ":" + paths.dataDir + ":/system/bin").c_str(), 1);
        setenv("SHELL", (paths.binDir + "/sh").c_str(), 1);
        setenv("TERM", "xterm-256color", 1);
        setenv("USER", "shell", 1);

        pid_t pid = fork();
        if (pid == 0) {
            // Child: exec toybox telnetd
            yinfo("Child: execing telnetd with login shell: {}", shPath);
            chdir(paths.dataDir.c_str());  // Set working directory

            execl(toyboxExe.c_str(), "telnetd",
                  "-l", shPath.c_str(),
                  "-p", "8024",
                  "-F",  // foreground
                  nullptr);
            yerror("Child: execl failed: {}", strerror(errno));
            _exit(1);
        } else if (pid > 0) {
            // Parent: wait a moment for telnetd to start, then connect
            yinfo("Parent: telnetd started with pid {}, waiting...", pid);
            usleep(200000);  // 200ms - give telnetd time to bind

            telnetConfig.shell = "127.0.0.1:8024";
            yinfo("Parent: connecting to 127.0.0.1:8024");
            auto reader2 = std::make_shared<telnet::TelnetPtyReader>();
            if (auto res2 = reader2->init(telnetConfig); res2) {
                yinfo("Parent: connected to toybox telnetd!");
                return Ok<Ptr>(reader2);
            } else {
                yerror("Parent: failed to connect to toybox: {}", res2.error().message());
            }
        } else {
            yerror("fork() failed: {}", strerror(errno));
        }

        return Err<Ptr>("Failed to connect to telnet server and no toybox fallback available", res);
    }

    yinfo("Initial connection succeeded!");
    return Ok<Ptr>(reader);
}

} // namespace yetty

#endif // __ANDROID__
