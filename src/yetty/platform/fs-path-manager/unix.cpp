#include <yetty/platform/fs-path-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !defined(_WIN32) && !YETTY_IOS

#include <yetty/result.hpp>
#include <cstdlib>
#include <unistd.h>

namespace yetty {

class UnixFsPathManager : public FsPathManager {
public:
    Result<void> init() {
#if defined(__APPLE__)
        // macOS: ~/Library/Caches/yetty
        const char* home = std::getenv("HOME");
        if (home) {
            _cacheBase = std::string(home) + "/Library/Caches/yetty";
        } else {
            _cacheBase = "/tmp/yetty-cache";
        }

        // macOS: TMPDIR for runtime
        const char* tmpdir = std::getenv("TMPDIR");
        if (tmpdir) {
            _runtimeBase = tmpdir;
            if (!_runtimeBase.empty() && _runtimeBase.back() == '/') {
                _runtimeBase.pop_back();
            }
        } else {
            _runtimeBase = "/tmp/yetty-" + std::to_string(getuid());
        }
#else
        // Linux: XDG_CACHE_HOME or ~/.cache/yetty
        const char* xdgCache = std::getenv("XDG_CACHE_HOME");
        if (xdgCache) {
            _cacheBase = std::string(xdgCache) + "/yetty";
        } else {
            const char* home = std::getenv("HOME");
            if (home) {
                _cacheBase = std::string(home) + "/.cache/yetty";
            } else {
                _cacheBase = "/tmp/yetty-cache";
            }
        }

        // Linux: XDG_RUNTIME_DIR for sockets/pipes
        const char* xdgRuntime = std::getenv("XDG_RUNTIME_DIR");
        if (xdgRuntime) {
            _runtimeBase = xdgRuntime;
        } else {
            _runtimeBase = "/tmp/yetty-" + std::to_string(getuid());
        }
#endif
        return Ok();
    }

    std::string getShadersDir() const override {
        return _cacheBase + "/shaders";
    }

    std::string getMsdfFontsDir() const override {
        return _cacheBase + "/msdf-fonts";
    }

    std::string getFontsDir() const override {
        return _cacheBase + "/fonts";
    }

    std::string getRuntimeDir() const override {
        return _runtimeBase + "/yetty";
    }

    std::string getBinDir() const override {
        return "/usr/bin";  // Not used on Unix
    }

private:
    std::string _cacheBase;
    std::string _runtimeBase;
};

// Factory
Result<FsPathManager::Ptr> FsPathManager::createImpl() {
    auto mgr = new UnixFsPathManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("UnixFsPathManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__ && !_WIN32
