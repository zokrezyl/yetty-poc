#include <yetty/ms-msdf-font.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

// Style name suffixes for CDB files
static const char* STYLE_SUFFIXES[] = {
    "-Regular.cdb",
    "-Bold.cdb",
    "-Oblique.cdb",
    "-BoldOblique.cdb"
};

//=============================================================================
// MsMsdfFontImpl - composes MsdfAtlas, implements Font style dispatch
//=============================================================================

class MsMsdfFontImpl : public MsMsdfFont {
public:
    MsMsdfFontImpl(const std::string& cdbBasePath, GpuAllocator::Ptr allocator)
        : _cdbBasePath(cdbBasePath), _allocator(std::move(allocator)) {}

    //=========================================================================
    // Initialization
    //=========================================================================

    Result<void> init() override {
        yinfo("MsMsdfFontImpl::init: entered");
        // Create atlas
        yinfo("MsMsdfFontImpl::init: creating MsdfAtlas");
        auto atlasResult = MsdfAtlas::create(_allocator);
        yinfo("MsMsdfFontImpl::init: MsdfAtlas::create returned");
        if (!atlasResult) {
            return Err<void>("Failed to create MsdfAtlas", atlasResult);
        }
        _atlas = std::move(*atlasResult);
        yinfo("MsMsdfFontImpl::init: atlas created");

        // Open all style CDBs on the atlas
        yinfo("MsMsdfFontImpl::init: opening CDBs, cdbBasePath={}", _cdbBasePath);
        for (int i = 0; i < 4; ++i) {
            std::string path = _cdbBasePath + STYLE_SUFFIXES[i];
            yinfo("MsMsdfFontImpl::init: opening CDB[{}] path={}", i, path);
            int fontId = _atlas->openCdb(path);
            yinfo("MsMsdfFontImpl::init: openCdb returned fontId={}", fontId);
            if (fontId < 0) {
                return Err<void>("Failed to open CDB: " + path);
            }
            _styleFontId[i] = fontId;
        }

        yinfo("MsMsdfFontImpl::init: all CDBs opened, pre-loading basic Latin");
        // Pre-load basic Latin
        initBasicLatin();
        yinfo("MsMsdfFontImpl::init: done");

        return Ok();
    }

    //=========================================================================
    // Atlas access
    //=========================================================================

    MsdfAtlas::Ptr atlas() const override { return _atlas; }

    //=========================================================================
    // CJK fallback
    //=========================================================================

    void setFallbackCdb(const std::string& cdbPath) override {
        _fallbackCdbPath = cdbPath;
    }

    //=========================================================================
    // Font interface — style dispatch
    //=========================================================================

    uint32_t getGlyphIndex(uint32_t codepoint) override {
        return getGlyphIndex(codepoint, Style::Regular);
    }

    uint32_t getGlyphIndex(uint32_t codepoint, Style style) override {
        int idx = styleIndex(style);
        int fontId = _styleFontId[idx];

        if (fontId < 0) {
            // Style CDB not available, fall back to Regular
            if (style != Style::Regular) {
                return getGlyphIndex(codepoint, Style::Regular);
            }
            return 0;
        }

        // Try loading from this style's CDB (atlas handles caching)
        uint32_t glyphIdx = _atlas->loadGlyph(fontId, codepoint);
        if (glyphIdx != 0) return glyphIdx;

        // Fallback: try Regular if this was a variant style
        if (style != Style::Regular) {
            int regularId = _styleFontId[styleIndex(Style::Regular)];
            if (regularId >= 0) {
                glyphIdx = _atlas->loadGlyph(regularId, codepoint);
                if (glyphIdx != 0) return glyphIdx;
            }
        }

        // Fallback: try CJK
        if (!_fallbackCdbPath.empty() && isCJKCodepoint(codepoint)) {
            if (_fallbackFontId < 0) {
                _fallbackFontId = _atlas->openCdb(_fallbackCdbPath);
                if (_fallbackFontId < 0) {
                    ywarn("Failed to open CJK fallback CDB: {}", _fallbackCdbPath);
                }
            }
            if (_fallbackFontId >= 0) {
                glyphIdx = _atlas->loadGlyph(_fallbackFontId, codepoint);
                if (glyphIdx != 0) return glyphIdx;
            }
        }

        return 0;
    }

    uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override {
        Style style = static_cast<Style>(styleIndex(bold, italic));
        return getGlyphIndex(codepoint, style);
    }

    //=========================================================================
    // Font interface — delegated to atlas
    //=========================================================================

    void uploadToGpu() override { _atlas->clearDirty(); }
    bool isDirty() const override { return _atlas->isDirty(); }
    void clearDirty() override { _atlas->clearDirty(); }
    uint32_t getAtlasWidth() const override { return _atlas->getAtlasWidth(); }
    uint32_t getAtlasHeight() const override { return _atlas->getAtlasHeight(); }
    const std::vector<uint8_t>& getAtlasData() const override { return _atlas->getAtlasData(); }

private:
    //=========================================================================
    // Basic Latin pre-loading
    //=========================================================================

    void initBasicLatin() {
        int regularId = _styleFontId[styleIndex(Style::Regular)];
        for (uint32_t cp = 0x20; cp <= 0x7E; ++cp) {
            _atlas->loadGlyph(regularId, cp);
        }
        yinfo("Pre-loaded basic Latin glyphs");
    }

    //=========================================================================
    // CJK codepoint detection
    //=========================================================================

    static bool isCJKCodepoint(uint32_t cp) {
        if (cp >= 0x4E00 && cp <= 0x9FFF) return true;   // CJK Unified Ideographs
        if (cp >= 0x3400 && cp <= 0x4DBF) return true;   // CJK Extension A
        if (cp >= 0x20000 && cp <= 0x2CEAF) return true;  // CJK Extension B-F (SIP)
        if (cp >= 0x3040 && cp <= 0x309F) return true;   // Hiragana
        if (cp >= 0x30A0 && cp <= 0x30FF) return true;   // Katakana
        if (cp >= 0xAC00 && cp <= 0xD7AF) return true;   // Hangul Syllables
        if (cp >= 0x1100 && cp <= 0x11FF) return true;   // Hangul Jamo
        if (cp >= 0x3000 && cp <= 0x303F) return true;   // CJK Symbols and Punctuation
        if (cp >= 0xFF00 && cp <= 0xFFEF) return true;   // Halfwidth and Fullwidth Forms
        if (cp >= 0x3100 && cp <= 0x312F) return true;   // Bopomofo
        return false;
    }

    //=========================================================================
    // Style helpers
    //=========================================================================

    static int styleIndex(Style style) { return static_cast<int>(style); }
    static int styleIndex(bool bold, bool italic) {
        return (bold ? 1 : 0) | (italic ? 2 : 0);
    }

    //=========================================================================
    // Private data
    //=========================================================================

    MsdfAtlas::Ptr _atlas;
    GpuAllocator::Ptr _allocator;
    int _styleFontId[4] = {-1, -1, -1, -1};  // Regular, Bold, Italic, BoldItalic
    int _fallbackFontId = -1;
    std::string _cdbBasePath;
    std::string _fallbackCdbPath;
};

//=============================================================================
// MsMsdfFont::createImpl - ObjectFactory entry point
//=============================================================================

Result<MsMsdfFont::Ptr> MsMsdfFont::createImpl(ContextType&,
                                                 const std::string& cdbBasePath,
                                                 GpuAllocator::Ptr allocator) {
    yinfo("MsMsdfFont::createImpl: entered cdbBasePath={}", cdbBasePath);
    auto font = Ptr(new MsMsdfFontImpl(cdbBasePath, std::move(allocator)));
    yinfo("MsMsdfFont::createImpl: font created");
    if (auto res = static_cast<MsMsdfFontImpl*>(font.get())->init(); !res) {
        return Err<Ptr>("Failed to initialize MsMsdfFont", res);
    }
    return Ok(std::move(font));
}

} // namespace yetty
