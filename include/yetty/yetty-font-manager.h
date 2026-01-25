#pragma once

#include <yetty/ms-msdf-font.h>
#include <yetty/bm-font.h>
#include <yetty/shader-font.h>
#include <yetty/gpu-context.h>
#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <string>
#include <memory>

namespace yetty {

class YettyFontManager : public base::ThreadSingleton<YettyFontManager> {
    friend class base::ThreadSingleton<YettyFontManager>;
public:
    using Ptr = std::shared_ptr<YettyFontManager>;

    virtual ~YettyFontManager() = default;

    // Factory for ThreadSingleton
    static Ptr createImpl() noexcept;

    virtual Result<void> init(const GPUContext& gpu) noexcept = 0;

    virtual Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string& fontName) noexcept = 0;
    virtual MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept = 0;
    virtual BmFont::Ptr getDefaultBmFont() noexcept = 0;
    virtual ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept = 0;
    virtual ShaderFont::Ptr getDefaultCardFont() noexcept = 0;

    virtual void setDefaultFont(const std::string& fontName) noexcept = 0;
    virtual bool hasFont(const std::string& fontName) const noexcept = 0;
    virtual const std::string& getCacheDir() const noexcept = 0;

protected:
    YettyFontManager() = default;
};

} // namespace yetty
