#include <yetty/platform/clipboard-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/result.hpp>
#include <emscripten/emscripten.h>

namespace yetty {

class WebClipboardManager : public ClipboardManager {
public:
    Result<void> init() {
        return Ok();
    }

    std::string getText() const override {
        // Async clipboard API - would need Promise handling
        // For now return empty (paste handled via keyboard events)
        return "";
    }

    void setText(const std::string& text) override {
        EM_ASM({ navigator.clipboard.writeText(UTF8ToString($0)); }, text.c_str());
    }
};

// Factory
Result<ClipboardManager::Ptr> ClipboardManager::createImpl() {
    auto mgr = new WebClipboardManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebClipboardManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
