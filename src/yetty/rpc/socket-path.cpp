#include <yetty/rpc/socket-path.h>

#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

namespace yetty {
namespace rpc {

Result<std::string> createSocketPath() {
    // Determine base directory: $XDG_RUNTIME_DIR or fallback
    std::string baseDir;
    if (auto* xdg = std::getenv("XDG_RUNTIME_DIR")) {
        baseDir = xdg;
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
}

Result<void> exportSocketPath(const std::string& path) {
    if (setenv("YETTY_SOCKET", path.c_str(), 1) != 0) {
        return Err("Failed to set YETTY_SOCKET");
    }
    return Ok();
}

} // namespace rpc
} // namespace yetty
