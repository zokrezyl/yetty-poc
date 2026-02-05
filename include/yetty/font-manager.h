#pragma once

#include <yetty/ms-msdf-font.h>
#include <yetty/bm-font.h>
#include <yetty/shader-font.h>
#include <yetty/gpu-context.h>
#include <yetty/shader-manager.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <string>
#include <memory>

namespace yetty {

/**
 * FontManager manages all font types for the terminal.
 *
 * Created via ObjectFactory with GPUContext and ShaderManager.
 * Stored in YettyContext for access by renderers.
 */
class FontManager : public base::ObjectFactory<FontManager> {
public:
    using Ptr = std::shared_ptr<FontManager>;

    virtual ~FontManager() = default;

    // Factory - creates FontManagerImpl
    static Result<Ptr> createImpl(ContextType& ctx,
                                  const GPUContext& gpu,
                                  ShaderManager::Ptr shaderMgr,
                                  MsdfCdbProvider::Ptr cdbProvider = nullptr) noexcept;

    virtual Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string& fontName) noexcept = 0;
    virtual MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept = 0;
    virtual BmFont::Ptr getDefaultBmFont() noexcept = 0;
    virtual ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept = 0;
    virtual ShaderFont::Ptr getDefaultCardFont() noexcept = 0;

    virtual void setDefaultFont(const std::string& fontName) noexcept = 0;
    virtual bool hasFont(const std::string& fontName) const noexcept = 0;
    virtual const std::string& getCacheDir() const noexcept = 0;
    virtual MsdfCdbProvider::Ptr getCdbProvider() const noexcept = 0;

protected:
    FontManager() = default;
};

} // namespace yetty
