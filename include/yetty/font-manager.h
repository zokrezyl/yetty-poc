#pragma once

#include <yetty/ms-msdf-font.h>
#include <yetty/bm-font.h>
#include <yetty/shader-font.h>
#include <yetty/vector-sdf-font.h>
#include <yetty/vector-coverage-font.h>
#include <yetty/raster-font.h>
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

    // Vector font (SDF curve-based rendering)
    virtual Result<VectorSdfFont::Ptr> getVectorSdfFont(const std::string& ttfPath) noexcept = 0;
    virtual VectorSdfFont::Ptr getDefaultVectorSdfFont() noexcept = 0;

    // Vector font (coverage-based rendering)
    virtual Result<VectorCoverageFont::Ptr> getVectorCoverageFont(const std::string& ttfPath) noexcept = 0;
    virtual VectorCoverageFont::Ptr getDefaultVectorCoverageFont() noexcept = 0;

    // Raster font (texture atlas rendering)
    // Cell size defaults are placeholder - caller should use setCellSize() once actual size is known
    virtual Result<RasterFont::Ptr> getRasterFont(const std::string& ttfPath,
                                                  uint32_t cellWidth = 16,
                                                  uint32_t cellHeight = 32) noexcept = 0;
    virtual RasterFont::Ptr getDefaultRasterFont() noexcept = 0;

    virtual void setDefaultFont(const std::string& fontName) noexcept = 0;
    virtual bool hasFont(const std::string& fontName) const noexcept = 0;
    virtual const std::string& getCacheDir() const noexcept = 0;
    virtual MsdfCdbProvider::Ptr getCdbProvider() const noexcept = 0;

protected:
    FontManager() = default;
};

} // namespace yetty
