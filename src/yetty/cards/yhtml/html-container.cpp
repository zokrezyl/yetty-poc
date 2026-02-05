#include "html-container.h"
#include "http-fetcher.h"
#include "yhtml.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cctype>
#include <unordered_map>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace yetty::card {

//=============================================================================
// HtmlContainerImpl
//=============================================================================

class HtmlContainerImpl : public HtmlContainer {
public:
    HtmlContainerImpl(YHtml* htmlCard, MsMsdfFont::Ptr font,
                      float defaultFontSize, HttpFetcher* fetcher)
        : _htmlCard(htmlCard)
        , _font(std::move(font))
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

        // Resolve font via fontconfig and register with YHtml
        if (faceName && *faceName && _htmlCard) {
            std::string ttfPath = resolveFontPath(
                faceName, weight, fi->italic);
            if (!ttfPath.empty()) {
                int id = _htmlCard->addFont(ttfPath);
                if (id >= 0) {
                    fi->fontId = id;
                    yinfo("HtmlContainer: font '{}' w={} i={} -> fontId={}",
                          faceName, weight, fi->italic, id);
                }
            }
        }

        // Compute font metrics from atlas glyph data
        float fontSize = fi->size;
        auto atlas = _htmlCard ? _htmlCard->atlas() : nullptr;
        if (atlas) {
            float fontBaseSize = atlas->getFontSize();
            float scale = fontSize / std::max(fontBaseSize, 1.0f);
            const auto& metadata = atlas->getGlyphMetadata();

            // Use 'H' bearingY as ascent (capital letter top)
            uint32_t capIdx = atlas->loadGlyph(fi->fontId, 'H');
            uint32_t descIdx = atlas->loadGlyph(fi->fontId, 'p');

            if (capIdx < metadata.size()) {
                fi->ascent = metadata[capIdx]._bearingY * scale;
            } else {
                fi->ascent = fontSize * 0.8f;
            }
            if (descIdx < metadata.size()) {
                fi->descent = (metadata[descIdx]._sizeY - metadata[descIdx]._bearingY) * scale;
            } else {
                fi->descent = fontSize * 0.2f;
            }
            fi->height = fi->ascent + fi->descent;
            fi->xHeight = fi->ascent * 0.65f;
        } else {
            fi->ascent = fontSize * 0.8f;
            fi->descent = fontSize * 0.2f;
            fi->height = fontSize * 1.2f;
            fi->xHeight = fontSize * 0.5f;
        }

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
        if (!fi || !text || !*text || !_htmlCard) return;

        uint32_t packed = packColor(color);
        float baseline = static_cast<float>(pos.bottom()) - fi->descent;
        _htmlCard->addText(static_cast<float>(pos.x), baseline,
                           text, fi->size, packed, _layer, fi->fontId);

        if (fi->decoration & litehtml::font_decoration_underline) {
            float y = baseline + 2.0f;
            float w = measureTextWidth(text, *fi);
            _htmlCard->addSegment(
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                packed, 1.0f, _layer);
        }

        if (fi->decoration & litehtml::font_decoration_linethrough) {
            float y = static_cast<float>(pos.y) + fi->xHeight;
            float w = measureTextWidth(text, *fi);
            _htmlCard->addSegment(
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                packed, 1.0f, _layer);
        }
    }

    void draw_background(
        litehtml::uint_ptr /*hdc*/,
        const std::vector<litehtml::background_paint>& bg) override
    {
        if (!_htmlCard) return;

        for (const auto& paint : bg) {
            if (paint.color.alpha == 0) continue;

            uint32_t color = packColor(paint.color);
            float x = static_cast<float>(paint.clip_box.x);
            float y = static_cast<float>(paint.clip_box.y);
            float w = static_cast<float>(paint.clip_box.width);
            float h = static_cast<float>(paint.clip_box.height);

            // Root/body background: covers full viewport width -> set card bg
            if (w >= static_cast<float>(_viewWidth) && x <= 0.0f) {
                _htmlCard->setBgColor(color);
                continue;
            }

            if (w > 0 && h > 0) {
                float cx = x + w * 0.5f;
                float cy = y + h * 0.5f;
                float round = static_cast<float>(paint.border_radius.top_left_x);
                _htmlCard->addBox(cx, cy, w * 0.5f, h * 0.5f,
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
        if (!_htmlCard) return;

        float x = static_cast<float>(draw_pos.x);
        float y = static_cast<float>(draw_pos.y);
        float w = static_cast<float>(draw_pos.width);
        float h = static_cast<float>(draw_pos.height);

        if (borders.top.width > 0 && borders.top.color.alpha > 0) {
            uint32_t c = packColor(borders.top.color);
            float bw = static_cast<float>(borders.top.width);
            _htmlCard->addSegment(x, y + bw * 0.5f,
                                  x + w, y + bw * 0.5f, c, bw, _layer);
        }
        if (borders.bottom.width > 0 && borders.bottom.color.alpha > 0) {
            uint32_t c = packColor(borders.bottom.color);
            float bw = static_cast<float>(borders.bottom.width);
            _htmlCard->addSegment(x, y + h - bw * 0.5f,
                                  x + w, y + h - bw * 0.5f, c, bw, _layer);
        }
        if (borders.left.width > 0 && borders.left.color.alpha > 0) {
            uint32_t c = packColor(borders.left.color);
            float bw = static_cast<float>(borders.left.width);
            _htmlCard->addSegment(x + bw * 0.5f, y,
                                  x + bw * 0.5f, y + h, c, bw, _layer);
        }
        if (borders.right.width > 0 && borders.right.color.alpha > 0) {
            uint32_t c = packColor(borders.right.color);
            float bw = static_cast<float>(borders.right.width);
            _htmlCard->addSegment(x + w - bw * 0.5f, y,
                                  x + w - bw * 0.5f, y + h, c, bw, _layer);
        }
    }

    void draw_list_marker(
        litehtml::uint_ptr /*hdc*/,
        const litehtml::list_marker& marker) override
    {
        if (!_htmlCard) return;

        uint32_t color = packColor(marker.color);
        float x = static_cast<float>(marker.pos.x);
        float y = static_cast<float>(marker.pos.y);
        float w = static_cast<float>(marker.pos.width);
        float h = static_cast<float>(marker.pos.height);

        if (marker.marker_type == litehtml::list_style_type_disc) {
            float r = std::min(w, h) * 0.3f;
            _htmlCard->addCircle(x + w * 0.5f, y + h * 0.5f, r,
                                 color, 0, 0, _layer);
        } else if (marker.marker_type == litehtml::list_style_type_square) {
            float s = std::min(w, h) * 0.3f;
            _htmlCard->addBox(x + w * 0.5f, y + h * 0.5f, s, s,
                              color, 0, 0, 0, _layer);
        } else if (marker.marker_type == litehtml::list_style_type_circle) {
            float r = std::min(w, h) * 0.3f;
            _htmlCard->addCircle(x + w * 0.5f, y + h * 0.5f, r,
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

    void get_image_size(const char* /*src*/, const char* /*baseurl*/,
                        litehtml::size& sz) override {
        sz.width = 0;
        sz.height = 0;
    }

    void load_image(const char* /*src*/, const char* /*baseurl*/,
                    bool /*redraw_on_ready*/) override {
    }

    void set_caption(const char* /*caption*/) override {}

    void set_base_url(const char* base_url) override {
        if (base_url && _fetcher) {
            _fetcher->setBaseUrl(base_url);
        }
    }

    void link(const std::shared_ptr<litehtml::document>& /*doc*/,
              const litehtml::element::ptr& /*el*/) override {}

    void on_anchor_click(const char* /*url*/,
                         const litehtml::element::ptr& /*el*/) override {}

    void set_cursor(const char* /*cursor*/) override {}

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

    void set_clip(const litehtml::position& /*pos*/,
                  const litehtml::border_radiuses& /*bdr_radius*/) override {
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
        int fontId;  // YHtml font ID (0 = default)
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
        if (!text || !*text || !_htmlCard) return 0.0f;

        auto atlas = _htmlCard->atlas();
        if (!atlas) return 0.0f;

        float fontBaseSize = atlas->getFontSize();
        float scale = fi.size / fontBaseSize;
        const auto& metadata = atlas->getGlyphMetadata();

        float width = 0.0f;
        const auto* ptr = reinterpret_cast<const uint8_t*>(text);
        const auto* end = ptr + strlen(text);

        while (ptr < end) {
            uint32_t cp = 0;
            if ((*ptr & 0x80) == 0) {
                cp = *ptr++;
            } else if ((*ptr & 0xE0) == 0xC0) {
                cp = (*ptr++ & 0x1F) << 6;
                if (ptr < end) cp |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF0) == 0xE0) {
                cp = (*ptr++ & 0x0F) << 12;
                if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) cp |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF8) == 0xF0) {
                cp = (*ptr++ & 0x07) << 18;
                if (ptr < end) cp |= (*ptr++ & 0x3F) << 12;
                if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) cp |= (*ptr++ & 0x3F);
            } else {
                ptr++;
                continue;
            }

            uint32_t glyphIdx;
            if (fi.fontId > 0) {
                glyphIdx = atlas->loadGlyph(fi.fontId, cp);
            } else if (_font) {
                Font::Style style = Font::Style::Regular;
                if (fi.bold && fi.italic) style = Font::Style::BoldItalic;
                else if (fi.bold) style = Font::Style::Bold;
                else if (fi.italic) style = Font::Style::Italic;
                glyphIdx = _font->getGlyphIndex(cp, style);
            } else {
                continue;
            }

            if (glyphIdx < metadata.size()) {
                width += metadata[glyphIdx]._advance * scale;
            }
        }

        return width;
    }

    static uint32_t packColor(litehtml::web_color c) {
        return static_cast<uint32_t>(c.red)
             | (static_cast<uint32_t>(c.green) << 8)
             | (static_cast<uint32_t>(c.blue) << 16)
             | (static_cast<uint32_t>(c.alpha) << 24);
    }

    YHtml* _htmlCard;
    MsMsdfFont::Ptr _font;
    HttpFetcher* _fetcher;
    float _defaultFontSize;
    int _viewWidth = 600;
    int _viewHeight = 800;
    uint32_t _layer = 0;

    std::vector<FontInfo*> _fonts;
    std::unordered_map<std::string, std::string> _fontPathCache;  // key -> ttf path
};

//=============================================================================
// Factory
//=============================================================================

Result<HtmlContainer::Ptr> HtmlContainer::createImpl(
    YHtml* htmlCard, MsMsdfFont::Ptr font,
    float defaultFontSize, HttpFetcher* fetcher)
{
    return Ok(Ptr(new HtmlContainerImpl(htmlCard, std::move(font),
                                         defaultFontSize, fetcher)));
}

} // namespace yetty::card
