#include <yetty/rpc/socket-path.h>

#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace yetty {
namespace rpc {

Result<std::string> createSocketPath(const std::string& runtimeDir) {
#ifdef _WIN32
    // Windows: use named pipes (\\.\pipe\name format required by libuv)
    // The runtimeDir parameter is ignored on Windows
    (void)runtimeDir;
    std::string path = "\\\\.\\pipe\\yetty-" + std::to_string(_getpid());
    return Ok(path);
#else
    // Use cache directory for sockets (more reliable than XDG_RUNTIME_DIR)
    // The runtimeDir parameter points to cache/yetty or similar
    (void)runtimeDir;

    // Build path: $HOME/.cache/yetty/rpc-socket/
    const char* xdgCache = std::getenv("XDG_CACHE_HOME");
    const char* home = std::getenv("HOME");
    std::string cacheBase;
    if (xdgCache) {
        cacheBase = std::string(xdgCache) + "/yetty";
    } else if (home) {
        cacheBase = std::string(home) + "/.cache/yetty";
    } else {
        cacheBase = "/tmp/yetty-" + std::to_string(getuid());
    }

    // Create directories
    mkdir(cacheBase.c_str(), 0700);
    std::string dir = cacheBase + "/rpc-socket";
    mkdir(dir.c_str(), 0700);

    // Socket file: yetty-<pid>.sock
    std::string path = dir + "/yetty-" + std::to_string(getpid()) + ".sock";
    return Ok(path);
#endif
}

Result<void> exportSocketPath(const std::string& path) {
#ifdef _WIN32
    if (_putenv_s("YETTY_SOCKET", path.c_str()) != 0) {
        return Err("Failed to set YETTY_SOCKET");
    }
#else
    if (setenv("YETTY_SOCKET", path.c_str(), 1) != 0) {
        return Err("Failed to set YETTY_SOCKET");
    }
#endif
    return Ok();
}

} // namespace rpc
} // namespace yetty
