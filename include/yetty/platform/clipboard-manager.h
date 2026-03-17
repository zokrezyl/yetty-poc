#pragma once

#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <string>

namespace yetty {

// ClipboardManager - clipboard read/write
//
// Listens for clipboard-related events and implements platform-specific
// clipboard access.
//
// Platform implementations:
// - glfw.cpp: GLFW clipboard (Linux, macOS, Windows)
// - android.cpp: JNI to ClipboardManager
// - webasm.cpp: navigator.clipboard API
//
class ClipboardManager : public base::ThreadSingleton<ClipboardManager> {
public:
    using Ptr = std::shared_ptr<ClipboardManager>;
    static Result<Ptr> createImpl();

    virtual ~ClipboardManager() = default;

    virtual std::string getText() const = 0;
    virtual void setText(const std::string& text) = 0;

protected:
    ClipboardManager() = default;
};

} // namespace yetty
