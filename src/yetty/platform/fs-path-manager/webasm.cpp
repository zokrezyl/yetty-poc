#include <yetty/platform/fs-path-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/result.hpp>

namespace yetty {

class WebFsPathManager : public FsPathManager {
public:
    Result<void> init() {
        return Ok();
    }

    std::string getShadersDir() const override {
        return "/assets/shaders";
    }

    std::string getMsdfFontsDir() const override {
        return "/assets/msdf-fonts";
    }

    std::string getFontsDir() const override {
        return "/assets";
    }

    std::string getRuntimeDir() const override {
        return "";  // No runtime dir on web
    }

    std::string getBinDir() const override {
        return "";  // No bin dir on web
    }
};

// Factory
Result<FsPathManager::Ptr> FsPathManager::createImpl() {
    auto mgr = new WebFsPathManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebFsPathManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
