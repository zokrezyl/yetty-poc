#pragma once

#include <yetty/font.h>
#include <yetty/gpu-allocator.h>
#include <yetty/gpu-context.h>
#include <yetty/base/base.h>

#include <string>
#include <memory>

namespace yetty {

/**
 * VectorCoverageFont - Coverage-based curve font rendering
 *
 * Instead of SDF evaluation, uses analytic coverage calculation:
 * - Cast rays from each pixel
 * - Count curve intersections for winding number
 * - Integrate coverage over pixel area
 *
 * Benefits:
 *   - More accurate anti-aliasing at all sizes
 *   - Correct handling of overlapping curves
 *   - No distance field artifacts
 *
 * Tradeoffs vs VectorSdfFont:
 *   - Slightly more GPU computation per pixel
 *   - Better quality at small sizes
 *
 * Codepoint range: Plane 15 PUA-A (0xF1000 - 0xF1FFF)
 *
 * GPU buffer layout identical to VectorSdfFont:
 *   [u32: curveCount | flags]
 *   [u32: p0] [u32: p1] [u32: p2]  <- curve 0 (12 bytes)
 *   ... (curveCount curves)
 *
 * Each point packed as: x[16] | y[16] normalized to cell space [0,1]
 */
class VectorCoverageFont : public Font,
                           public base::Object,
                           public base::ObjectFactory<VectorCoverageFont> {
public:
    using Ptr = std::shared_ptr<VectorCoverageFont>;
    using base::ObjectFactory<VectorCoverageFont>::create;

    // Factory: loads font from TTF path
    static Result<Ptr> createImpl(ContextType& ctx,
                                  const GPUContext& gpu,
                                  GpuAllocator::Ptr allocator,
                                  const std::string& ttfPath);

    ~VectorCoverageFont() override = default;

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

    // PUA-A range for coverage vector font (0xF1000 - 0xF1FFF)
    static constexpr uint32_t CODEPOINT_BASE = 0xF1000;
    static constexpr uint32_t CODEPOINT_END  = 0xF1FFF;

    // Map ASCII to coverage range: 'A' (65) -> 0xF1041
    static constexpr uint32_t toCodepoint(uint32_t ascii) {
        return CODEPOINT_BASE + ascii;
    }
};

} // namespace yetty
