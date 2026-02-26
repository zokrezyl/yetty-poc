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

Result<std::string> createSocketPath() {
#ifdef _WIN32
    // Windows: use named pipes or temp directory
    std::string baseDir;
    if (auto* temp = std::getenv("TEMP")) {
        baseDir = temp;
    } else if (auto* tmp = std::getenv("TMP")) {
        baseDir = tmp;
    } else {
        baseDir = "C:\\Temp";
    }

    // Create yetty subdirectory
    std::string dir = baseDir + "\\yetty";
    _mkdir(dir.c_str()); // ok if already exists

    // Socket file (Windows will use named pipes, but path format similar)
    std::string path = dir + "\\yetty-" + std::to_string(_getpid()) + ".sock";
    return Ok(path);
#else
    // Determine base directory: $XDG_RUNTIME_DIR or fallback
    std::string baseDir;
    if (auto* xdg = std::getenv("XDG_RUNTIME_DIR")) {
        baseDir = xdg;
    } else if (auto* tmpdir = std::getenv("TMPDIR")) {
        // macOS: TMPDIR is per-user, set by launchd (e.g. /var/folders/.../T/)
        baseDir = tmpdir;
        if (!baseDir.empty() && baseDir.back() == '/') {
            baseDir.pop_back();
        }
    } else {
        // Fallback: /tmp/yetty-<uid>
        baseDir = "/tmp/yetty-" + std::to_string(getuid());
    }

    // Create yetty subdirectory: <baseDir>/yetty/
    std::string dir = baseDir + "/yetty";
    mkdir(dir.c_str(), 0700); // ok if already exists

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
