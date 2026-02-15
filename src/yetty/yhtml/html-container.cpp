#include "html-container.h"
#include "http-fetcher.h"
#include <yetty/ydraw-writer.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <unordered_map>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace yetty::card {

//=============================================================================
// Image dimension parsers — extract width/height from raw image data
//=============================================================================

static bool parsePngSize(const std::string& data, int& w, int& h) {
    if (data.size() < 24) return false;
    auto* d = reinterpret_cast<const uint8_t*>(data.data());
    if (d[0] != 0x89 || d[1] != 'P' || d[2] != 'N' || d[3] != 'G') return false;
    w = static_cast<int>((d[16] << 24) | (d[17] << 16) | (d[18] << 8) | d[19]);
    h = static_cast<int>((d[20] << 24) | (d[21] << 16) | (d[22] << 8) | d[23]);
    return w > 0 && h > 0;
}

static bool parseGifSize(const std::string& data, int& w, int& h) {
    if (data.size() < 10) return false;
    if (data[0] != 'G' || data[1] != 'I' || data[2] != 'F') return false;
    auto* d = reinterpret_cast<const uint8_t*>(data.data());
    w = d[6] | (d[7] << 8);
    h = d[8] | (d[9] << 8);
    return w > 0 && h > 0;
}

static bool parseJpegSize(const std::string& data, int& w, int& h) {
    if (data.size() < 4) return false;
    auto* d = reinterpret_cast<const uint8_t*>(data.data());
    if (d[0] != 0xFF || d[1] != 0xD8) return false;
    size_t pos = 2;
    while (pos + 4 < data.size()) {
        if (d[pos] != 0xFF) { pos++; continue; }
        uint8_t marker = d[pos + 1];
        if (marker == 0 || marker == 0xFF) { pos++; continue; }
        // SOF markers
        if (marker >= 0xC0 && marker <= 0xCF && marker != 0xC4 && marker != 0xC8) {
            if (pos + 9 < data.size()) {
                h = (d[pos + 5] << 8) | d[pos + 6];
                w = (d[pos + 7] << 8) | d[pos + 8];
                return w > 0 && h > 0;
            }
            return false;
        }
        if (pos + 3 < data.size()) {
            uint16_t len = static_cast<uint16_t>((d[pos + 2] << 8) | d[pos + 3]);
            pos += 2 + len;
        } else break;
    }
    return false;
}

static bool parseWebpSize(const std::string& data, int& w, int& h) {
    if (data.size() < 30) return false;
    if (data.substr(0, 4) != "RIFF" || data.substr(8, 4) != "WEBP") return false;
    auto* d = reinterpret_cast<const uint8_t*>(data.data());
    if (data.substr(12, 4) == "VP8 " && data.size() >= 30) {
        // Lossy VP8: width at 26, height at 28 (little-endian, 14-bit)
        w = (d[26] | (d[27] << 8)) & 0x3FFF;
        h = (d[28] | (d[29] << 8)) & 0x3FFF;
        return w > 0 && h > 0;
    }
    if (data.substr(12, 4) == "VP8L" && data.size() >= 25) {
        // Lossless VP8L: packed in 4 bytes starting at offset 21
        uint32_t bits = d[21] | (d[22] << 8) | (d[23] << 16) | (d[24] << 24);
        w = static_cast<int>((bits & 0x3FFF) + 1);
        h = static_cast<int>(((bits >> 14) & 0x3FFF) + 1);
        return w > 0 && h > 0;
    }
    return false;
}

static bool parseImageSize(const std::string& data, int& w, int& h) {
    return parsePngSize(data, w, h) ||
           parseJpegSize(data, w, h) ||
           parseGifSize(data, w, h) ||
           parseWebpSize(data, w, h);
}

//=============================================================================
// HtmlContainerImpl
//=============================================================================

class HtmlContainerImpl : public HtmlContainer {
public:
    HtmlContainerImpl(YDrawWriter* writer, MsMsdfFont::Ptr /*font*/,
                      float defaultFontSize, HttpFetcher* fetcher)
        : _writer(writer)
        , _fetcher(fetcher)
        , _defaultFontSize(defaultFontSize)
    {}

    ~HtmlContainerImpl() override {
        for (auto* fi : _fonts) delete fi;
    }

    void setViewportSize(int width, int height) override {
        _viewWidth = width;
        _viewHeight = height;
    }

    void setNavigateCallback(NavigateCallback cb) override {
        _navigateCb = std::move(cb);
    }

    void setCursorCallback(CursorCallback cb) override {
        _cursorCb = std::move(cb);
    }

    //=========================================================================
    // Font handling
    //=========================================================================

    litehtml::uint_ptr create_font(
        const char* faceName, int size, int weight,
        litehtml::font_style italic, unsigned int decoration,
        litehtml::font_metrics* fm) override
    {
        auto* fi = new FontInfo();
        fi->size = static_cast<float>(size);
        fi->bold = (weight >= 700);
        fi->italic = (italic == litehtml::font_style_italic);
        fi->decoration = decoration;
        fi->fontId = 0;  // default font

        // Resolve font via fontconfig and register with builder
        if (faceName && *faceName && _writer) {
            std::string ttfPath = resolveFontPath(
                faceName, weight, fi->italic);
            if (!ttfPath.empty()) {
                auto fontResult = _writer->addFont(ttfPath);
                if (fontResult) {
                    fi->fontId = *fontResult;
                    yinfo("HtmlContainer: font '{}' w={} i={} -> fontId={}",
                          faceName, weight, fi->italic, *fontResult);
                }
            }
        }

        // Get font metrics from builder (centralized atlas logic)
        fi->ascent = _writer->fontAscent(fi->size, fi->fontId);
        fi->descent = _writer->fontDescent(fi->size, fi->fontId);
        fi->height = fi->ascent + fi->descent;
        fi->xHeight = fi->ascent * 0.65f;

        if (fm) {
            fm->ascent = static_cast<int>(fi->ascent);
            fm->descent = static_cast<int>(fi->descent);
            fm->height = static_cast<int>(fi->height);
            fm->x_height = static_cast<int>(fi->xHeight);
            fm->draw_spaces = (decoration != 0);
        }

        _fonts.push_back(fi);
        return reinterpret_cast<litehtml::uint_ptr>(fi);
    }

    void delete_font(litehtml::uint_ptr hFont) override {
        auto* fi = reinterpret_cast<FontInfo*>(hFont);
        auto it = std::find(_fonts.begin(), _fonts.end(), fi);
        if (it != _fonts.end()) {
            _fonts.erase(it);
        }
        delete fi;
    }

    int text_width(const char* text, litehtml::uint_ptr hFont) override {
        auto* fi = reinterpret_cast<FontInfo*>(hFont);
        if (!fi) return 0;
        return static_cast<int>(measureTextWidth(text, *fi));
    }

    //=========================================================================
    // Drawing
    //=========================================================================

    void draw_text(litehtml::uint_ptr /*hdc*/, const char* text,
        litehtml::uint_ptr hFont, litehtml::web_color color,
        const litehtml::position& pos) override {
        auto* fi = reinterpret_cast<FontInfo*>(hFont);
        if (!fi || !text || !*text || !_writer) return;

        uint32_t packed = packColor(color);
        // Baseline = bottom of position box - descent (matches litehtml reference containers)
        float baseline = static_cast<float>(pos.y + pos.height) - fi->descent;

        ydebug("draw_text: '{}' pos=({},{} {}x{}) size={:.1f} fontId={} ascent={:.1f} descent={:.1f} height={:.1f} baseline={:.1f} color=({},{},{},{})",
              text, pos.x, pos.y, pos.width, pos.height,
              fi->size, fi->fontId, fi->ascent, fi->descent, fi->height, baseline,
              color.red, color.green, color.blue, color.alpha);

        _writer->addText(static_cast<float>(pos.x), baseline,
                           text, fi->size, packed, _layer, fi->fontId);

        if (fi->decoration & litehtml::font_decoration_underline) {
            float y = baseline + 2.0f;
            float w = measureTextWidth(text, *fi);
            _writer->addSegment(
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                packed, 1.0f, _layer);
        }

        if (fi->decoration & litehtml::font_decoration_linethrough) {
            float y = static_cast<float>(pos.y) + fi->xHeight;
            float w = measureTextWidth(text, *fi);
            _writer->addSegment(
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                packed, 1.0f, _layer);
        }
    }

    void draw_background(
        litehtml::uint_ptr /*hdc*/,
        const std::vector<litehtml::background_paint>& bg) override
    {
        if (!_writer) return;

        for (const auto& paint : bg) {
            float x = static_cast<float>(paint.clip_box.x);
            float y = static_cast<float>(paint.clip_box.y);
            float w = static_cast<float>(paint.clip_box.width);
            float h = static_cast<float>(paint.clip_box.height);

            // Draw background image placeholder
            if (!paint.image.empty() && w > 0 && h > 0) {
                // Light gray placeholder box for images
                uint32_t placeholderColor = 0xFFE0E0E0;  // ABGR light gray
                float cx = x + w * 0.5f;
                float cy = y + h * 0.5f;
                float round = static_cast<float>(paint.border_radius.top_left_x);
                _writer->addBox(cx, cy, w * 0.5f, h * 0.5f,
                                  placeholderColor, 0, 0, round, _layer);
            }

            if (paint.color.alpha == 0) continue;

            uint32_t color = packColor(paint.color);

            // Root/body background: covers full viewport width -> set card bg
            if (w >= static_cast<float>(_viewWidth) && x <= 0.0f) {
                _writer->setBgColor(color);
                continue;
            }

            if (w > 0 && h > 0) {
                float cx = x + w * 0.5f;
                float cy = y + h * 0.5f;
                float round = static_cast<float>(paint.border_radius.top_left_x);
                _writer->addBox(cx, cy, w * 0.5f, h * 0.5f,
                                  color, 0, 0, round, _layer);
            }
        }
    }

    void draw_borders(
        litehtml::uint_ptr /*hdc*/,
        const litehtml::borders& borders,
        const litehtml::position& draw_pos,
        bool /*root*/) override
    {
        if (!_writer) return;

        float x = static_cast<float>(draw_pos.x);
        float y = static_cast<float>(draw_pos.y);
        float w = static_cast<float>(draw_pos.width);
        float h = static_cast<float>(draw_pos.height);

        ydebug("draw_borders: pos=({},{} {}x{}) top=({},w={}) right=({},w={}) bottom=({},w={}) left=({},w={})",
               draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height,
               borders.top.color.alpha, borders.top.width,
               borders.right.color.alpha, borders.right.width,
               borders.bottom.color.alpha, borders.bottom.width,
               borders.left.color.alpha, borders.left.width);

        if (borders.top.width > 0 && borders.top.color.alpha > 0) {
            uint32_t c = packColor(borders.top.color);
            float bw = static_cast<float>(borders.top.width);
            _writer->addSegment(x, y + bw * 0.5f,
                                  x + w, y + bw * 0.5f, c, bw, _layer);
        }
        if (borders.bottom.width > 0 && borders.bottom.color.alpha > 0) {
            uint32_t c = packColor(borders.bottom.color);
            float bw = static_cast<float>(borders.bottom.width);
            _writer->addSegment(x, y + h - bw * 0.5f,
                                  x + w, y + h - bw * 0.5f, c, bw, _layer);
        }
        if (borders.left.width > 0 && borders.left.color.alpha > 0) {
            uint32_t c = packColor(borders.left.color);
            float bw = static_cast<float>(borders.left.width);
            _writer->addSegment(x + bw * 0.5f, y,
                                  x + bw * 0.5f, y + h, c, bw, _layer);
        }
        if (borders.right.width > 0 && borders.right.color.alpha > 0) {
            uint32_t c = packColor(borders.right.color);
            float bw = static_cast<float>(borders.right.width);
            _writer->addSegment(x + w - bw * 0.5f, y,
                                  x + w - bw * 0.5f, y + h, c, bw, _layer);
        }
    }

    void draw_list_marker(
        litehtml::uint_ptr /*hdc*/,
        const litehtml::list_marker& marker) override
    {
        if (!_writer) return;

        uint32_t color = packColor(marker.color);
        float x = static_cast<float>(marker.pos.x);
        float y = static_cast<float>(marker.pos.y);
        float w = static_cast<float>(marker.pos.width);
        float h = static_cast<float>(marker.pos.height);

        ydebug("draw_list_marker: type={} pos=({},{} {}x{}) color=({},{},{},{})",
               static_cast<int>(marker.marker_type),
               marker.pos.x, marker.pos.y, marker.pos.width, marker.pos.height,
               marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha);

        if (marker.marker_type == litehtml::list_style_type_disc) {
            float r = std::min(w, h) * 0.3f;
            _writer->addCircle(x + w * 0.5f, y + h * 0.5f, r,
                                 color, 0, 0, _layer);
        } else if (marker.marker_type == litehtml::list_style_type_square) {
            float s = std::min(w, h) * 0.3f;
            _writer->addBox(x + w * 0.5f, y + h * 0.5f, s, s,
                              color, 0, 0, 0, _layer);
        } else if (marker.marker_type == litehtml::list_style_type_circle) {
            float r = std::min(w, h) * 0.3f;
            _writer->addCircle(x + w * 0.5f, y + h * 0.5f, r,
                                 0, color, 1.0f, _layer);
        }
    }

    //=========================================================================
    // Stubs and simple implementations
    //=========================================================================

    int pt_to_px(int pt) const override {
        return static_cast<int>(static_cast<float>(pt) * 96.0f / 72.0f);
    }

    int get_default_font_size() const override {
        return static_cast<int>(_defaultFontSize);
    }

    const char* get_default_font_name() const override {
        return "sans-serif";
    }

    std::shared_ptr<litehtml::element> create_element(
        const char* /*tag_name*/,
        const litehtml::string_map& /*attributes*/,
        const std::shared_ptr<litehtml::document>& /*doc*/) override
    {
        return nullptr;
    }

    void get_image_size(const char* src, const char* /*baseurl*/,
                        litehtml::size& sz) override {
        sz.width = 0;
        sz.height = 0;
        if (!src || !*src) return;
        auto it = _imageCache.find(src);
        if (it != _imageCache.end()) {
            sz.width = it->second.width;
            sz.height = it->second.height;
        }
    }

    void load_image(const char* src, const char* /*baseurl*/,
                    bool /*redraw_on_ready*/) override {
        if (!src || !*src || !_fetcher) return;
        if (_imageCache.count(src)) return;  // already loaded
        if (_imageCache.size() >= 200) return;  // limit per page

        std::string url(src);
        // Skip data: URIs for now
        if (url.substr(0, 5) == "data:") return;

        auto body = _fetcher->fetch(url);
        if (!body || body->empty()) {
            _imageCache[url] = {0, 0};
            return;
        }

        int w = 0, h = 0;
        if (parseImageSize(*body, w, h)) {
            // Cap to reasonable dimensions
            w = std::min(w, 4096);
            h = std::min(h, 4096);
            yinfo("HtmlContainer::load_image: {} -> {}x{}", url, w, h);
        }
        _imageCache[url] = {w, h};
    }

    void set_caption(const char* /*caption*/) override {}

    void set_base_url(const char* base_url) override {
        if (base_url && _fetcher) {
            _fetcher->setBaseUrl(base_url);
        }
    }

    void link(const std::shared_ptr<litehtml::document>& /*doc*/,
              const litehtml::element::ptr& /*el*/) override {}

    void on_anchor_click(const char* url,
                         const litehtml::element::ptr& /*el*/) override {
        if (url && _navigateCb) {
            yinfo("HtmlContainer::on_anchor_click: {}", url);
            _navigateCb(url);
        }
    }

    void set_cursor(const char* cursor) override {
        if (cursor && _cursorCb) {
            _cursorCb(cursor);
        }
    }

    void transform_text(litehtml::string& text,
                        litehtml::text_transform tt) override {
        if (text.empty()) return;
        switch (tt) {
            case litehtml::text_transform_capitalize:
                if (!text.empty()) text[0] = static_cast<char>(toupper(text[0]));
                break;
            case litehtml::text_transform_uppercase:
                std::transform(text.begin(), text.end(), text.begin(), ::toupper);
                break;
            case litehtml::text_transform_lowercase:
                std::transform(text.begin(), text.end(), text.begin(), ::tolower);
                break;
            default:
                break;
        }
    }

    void import_css(litehtml::string& text, const litehtml::string& url,
                    litehtml::string& /*baseurl*/) override {
        if (!_fetcher || url.empty()) return;

        yinfo("HtmlContainer::import_css: fetching {}", url);
        auto css = _fetcher->fetch(url);
        if (css) {
            text = std::move(*css);
        } else {
            ywarn("HtmlContainer::import_css: failed to fetch {}", url);
        }
    }

    void set_clip(const litehtml::position& pos,
                  const litehtml::border_radiuses& /*bdr_radius*/) override {
        ydebug("set_clip: pos=({},{} {}x{})", pos.x, pos.y, pos.width, pos.height);
    }

    void del_clip() override {
    }

    void get_client_rect(litehtml::position& client) const override {
        client.x = 0;
        client.y = 0;
        client.width = _viewWidth;
        client.height = _viewHeight;
    }

    void get_media_features(litehtml::media_features& media) const override {
        media.type = litehtml::media_type_screen;
        media.width = _viewWidth;
        media.height = _viewHeight;
        media.device_width = _viewWidth;
        media.device_height = _viewHeight;
        media.color = 8;
        media.monochrome = 0;
        media.color_index = 256;
        media.resolution = 96;
    }

    void get_language(litehtml::string& language,
                      litehtml::string& culture) const override {
        language = "en";
        culture = "US";
    }

private:
    struct FontInfo {
        float size;
        bool bold;
        bool italic;
        unsigned int decoration;
        float ascent;
        float descent;
        float height;
        float xHeight;
        int fontId;  // builder font ID (0 = default)
    };

    //=========================================================================
    // Fontconfig resolution
    //=========================================================================

    std::string resolveFontPath(const char* faceName, int weight, bool italic) {
        // Check cache first
        std::string key = std::string(faceName) + ":" +
                          std::to_string(weight) + ":" +
                          (italic ? "i" : "r");
        auto it = _fontPathCache.find(key);
        if (it != _fontPathCache.end()) return it->second;

        std::string result;

#ifdef YETTY_USE_FONTCONFIG
        FcConfig* config = FcInitLoadConfigAndFonts();
        if (!config) {
            ywarn("HtmlContainer: fontconfig init failed");
            return result;
        }

        FcPattern* pattern = FcPatternCreate();
        if (!pattern) {
            FcConfigDestroy(config);
            return result;
        }

        // Parse comma-separated font families, use first one
        std::string family(faceName);
        auto comma = family.find(',');
        if (comma != std::string::npos) {
            family = family.substr(0, comma);
        }
        // Trim whitespace and quotes
        while (!family.empty() && (family.front() == ' ' || family.front() == '\'' || family.front() == '"'))
            family.erase(family.begin());
        while (!family.empty() && (family.back() == ' ' || family.back() == '\'' || family.back() == '"'))
            family.pop_back();

        FcPatternAddString(pattern, FC_FAMILY,
                           reinterpret_cast<const FcChar8*>(family.c_str()));

        // Map CSS weight to fontconfig weight
        int fcWeight = FC_WEIGHT_REGULAR;
        if (weight <= 100) fcWeight = FC_WEIGHT_THIN;
        else if (weight <= 200) fcWeight = FC_WEIGHT_EXTRALIGHT;
        else if (weight <= 300) fcWeight = FC_WEIGHT_LIGHT;
        else if (weight <= 400) fcWeight = FC_WEIGHT_REGULAR;
        else if (weight <= 500) fcWeight = FC_WEIGHT_MEDIUM;
        else if (weight <= 600) fcWeight = FC_WEIGHT_SEMIBOLD;
        else if (weight <= 700) fcWeight = FC_WEIGHT_BOLD;
        else if (weight <= 800) fcWeight = FC_WEIGHT_EXTRABOLD;
        else fcWeight = FC_WEIGHT_BLACK;
        FcPatternAddInteger(pattern, FC_WEIGHT, fcWeight);

        FcPatternAddInteger(pattern, FC_SLANT,
                            italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN);

        FcConfigSubstitute(config, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);

        FcResult fcResult;
        FcPattern* match = FcFontMatch(config, pattern, &fcResult);
        FcPatternDestroy(pattern);

        if (match && fcResult == FcResultMatch) {
            FcChar8* fontPath = nullptr;
            if (FcPatternGetString(match, FC_FILE, 0, &fontPath) == FcResultMatch) {
                result = reinterpret_cast<const char*>(fontPath);
            }
            FcPatternDestroy(match);
        }

        FcConfigDestroy(config);
#else
        (void)faceName;
        (void)weight;
        (void)italic;
#endif

        _fontPathCache[key] = result;
        return result;
    }

    //=========================================================================
    // Text measurement
    //=========================================================================

    float measureTextWidth(const char* text, const FontInfo& fi) const {
        if (!text || !*text || !_writer) return 0.0f;
        return _writer->measureText(text, fi.size, fi.fontId);
    }

    static uint32_t packColor(litehtml::web_color c) {
        return static_cast<uint32_t>(c.red)
             | (static_cast<uint32_t>(c.green) << 8)
             | (static_cast<uint32_t>(c.blue) << 16)
             | (static_cast<uint32_t>(c.alpha) << 24);
    }

    struct ImageInfo {
        int width = 0;
        int height = 0;
    };

    YDrawWriter* _writer;
    HttpFetcher* _fetcher;
    float _defaultFontSize;
    int _viewWidth = 600;
    int _viewHeight = 800;
    uint32_t _layer = 0;

    NavigateCallback _navigateCb;
    CursorCallback _cursorCb;

    std::vector<FontInfo*> _fonts;
    std::unordered_map<std::string, std::string> _fontPathCache;  // key -> ttf path
    std::unordered_map<std::string, ImageInfo> _imageCache;
};

//=============================================================================
// Factory
//=============================================================================

Result<HtmlContainer::Ptr> HtmlContainer::createImpl(
    YDrawWriter* writer, MsMsdfFont::Ptr font,
    float defaultFontSize, HttpFetcher* fetcher)
{
    return Ok(Ptr(new HtmlContainerImpl(writer, std::move(font),
                                         defaultFontSize, fetcher)));
}

} // namespace yetty::card
