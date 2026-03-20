#include <yetty/platform/fs-path-manager.h>

#if defined(_WIN32)

#include <yetty/result.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shlobj.h>

namespace yetty {

class WindowsFsPathManager : public FsPathManager {
public:
    Result<void> init() {
        // Get %LOCALAPPDATA%\yetty for cache
        char localAppData[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, localAppData))) {
            _cacheBase = std::string(localAppData) + "\\yetty";
        } else {
            // Fallback to temp
            char tempPath[MAX_PATH];
            GetTempPathA(MAX_PATH, tempPath);
            _cacheBase = std::string(tempPath) + "yetty";
        }

        // Runtime: same as cache on Windows
        _runtimeBase = _cacheBase;

        return Ok();
    }

    std::string getShadersDir() const override {
        return _cacheBase + "\\shaders";
    }

    std::string getMsdfFontsDir() const override {
        return _cacheBase + "\\msdf-fonts";
    }

    std::string getFontsDir() const override {
        return _cacheBase + "\\fonts";
    }

    std::string getRuntimeDir() const override {
        return _runtimeBase;
    }

    std::string getBinDir() const override {
        return "";  // Not used on Windows
    }

private:
    std::string _cacheBase;
    std::string _runtimeBase;
};

// Factory
Result<FsPathManager::Ptr> FsPathManager::createImpl() {
    auto mgr = new WindowsFsPathManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WindowsFsPathManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // _WIN32
