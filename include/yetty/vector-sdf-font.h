#pragma once

#include <yetty/font.h>
#include <yetty/gpu-allocator.h>
#include <yetty/gpu-context.h>
#include <yetty/base/base.h>

#include <string>
#include <memory>

namespace yetty {

/**
 * VectorSdfFont - SDF curve-based font rendering
 *
 * Instead of pre-rendered MSDF textures, stores actual Bezier curves
 * and evaluates signed distance analytically in the shader.
 *
 * Benefits:
 *   - ~25x smaller than MSDF atlas (curves vs pixels)
 *   - Resolution independent
 *   - No atlas texture management
 *
 * Codepoint range for testing: Plane 15 PUA-A (0xF0000 - 0xF0FFF)
 *
 * GPU buffer layout (per glyph):
 *   [u32: curveCount | flags]
 *   [u32: p0] [u32: p1] [u32: p2]  <- curve 0 (12 bytes)
 *   [u32: p0] [u32: p1] [u32: p2]  <- curve 1
 *   ... (curveCount curves)
 *
 * Each point packed as: x[16] | y[16] normalized to cell space [0,1]
 */
class VectorSdfFont : public Font,
                      public base::Object,
                      public base::ObjectFactory<VectorSdfFont> {
public:
    using Ptr = std::shared_ptr<VectorSdfFont>;
    using base::ObjectFactory<VectorSdfFont>::create;

    // Factory: loads font from TTF path
    static Result<Ptr> createImpl(ContextType& ctx,
                                  const GPUContext& gpu,
                                  GpuAllocator::Ptr allocator,
                                  const std::string& ttfPath);

    ~VectorSdfFont() override = default;

    // Load glyphs for codepoints (extracts curves from TTF)
    virtual Result<void> loadGlyphs(const std::vector<uint32_t>& codepoints) = 0;

    // Load ASCII printable range (0x20 - 0x7E)
    virtual Result<void> loadBasicLatin() = 0;

    // GPU buffer access
    virtual WGPUBuffer getGlyphBuffer() const = 0;
    virtual WGPUBuffer getOffsetBuffer() const = 0;

    // Statistics
    virtual size_t glyphCount() const = 0;
    virtual size_t totalCurves() const = 0;
    virtual size_t bufferSize() const = 0;
    virtual size_t offsetTableSize() const = 0;

    // PUA-A range for SDF vector font (0xF0000 - 0xF0FFF)
    static constexpr uint32_t CODEPOINT_BASE = 0xF0000;
    static constexpr uint32_t CODEPOINT_END  = 0xF0FFF;

    // Map ASCII to SDF range: 'A' (65) -> 0xF0041
    static constexpr uint32_t toCodepoint(uint32_t ascii) {
        return CODEPOINT_BASE + ascii;
    }
};

} // namespace yetty
