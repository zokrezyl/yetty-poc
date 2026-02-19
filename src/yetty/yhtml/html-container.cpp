#include "html-container.h"
#include "http-fetcher.h"
#include <yetty/font/raw-font.h>
#include <yetty/font/raw-font-manager.h>
#include "../ydraw/ydraw-buffer.h"
#include <fstream>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <unordered_map>
#include <stb_image.h>
#include <litehtml/html_tag.h>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace yetty::card {

//=============================================================================
// Forward declare so el_form_submit can reference it
//=============================================================================
class HtmlContainerImpl;

//=============================================================================
// URL-encode a string for form data
//=============================================================================
static std::string urlEncode(const std::string& s) {
    std::string encoded;
    for (unsigned char c : s) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += static_cast<char>(c);
        } else if (c == ' ') {
            encoded += '+';
        } else {
            char hex[4];
            std::snprintf(hex, sizeof(hex), "%%%02X", c);
            encoded += hex;
        }
    }
    return encoded;
}

//=============================================================================
// Collect form data from all <input> children of a <form> element
//=============================================================================
static void collectFormFields(const std::shared_ptr<litehtml::element>& el,
                              const litehtml::element* clickedSubmit,
                              std::string& out) {
    if (el->tag() == litehtml::_input_) {
        const char* name = el->get_attr("name");
        if (!name || !*name) return;  // skip unnamed inputs

        const char* type = el->get_attr("type");
        std::string typeStr = type ? type : "text";
        for (auto& c : typeStr) c = static_cast<char>(std::tolower(c));

        if (typeStr == "submit") {
            // Only include the clicked submit button's name=value
            if (el.get() == clickedSubmit) {
                const char* value = el->get_attr("value");
                if (!out.empty()) out += '&';
                out += urlEncode(name) + "=" + urlEncode(value ? value : "");
            }
        } else if (typeStr == "hidden" || typeStr == "text" ||
                   typeStr == "email" || typeStr == "password") {
            const char* value = el->get_attr("value");
            if (!out.empty()) out += '&';
            out += urlEncode(name) + "=" + urlEncode(value ? value : "");
        }
        return;  // <input> elements don't have children to traverse
    }

    for (auto& child : el->children()) {
        collectFormFields(child, clickedSubmit, out);
    }
}

//=============================================================================
// Custom element for form submit: <button> and <input type="submit">
// On click, collects parent <form> data and triggers form submit callback.
//=============================================================================
class el_form_submit : public litehtml::html_tag {
public:
    explicit el_form_submit(const std::shared_ptr<litehtml::document>& doc)
        : litehtml::html_tag(doc) {}

    // Defined after HtmlContainerImpl so we can call submitForm()
    void on_click() override;
};

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
    HtmlContainerImpl(YDrawBuffer::Ptr buffer,
                      float defaultFontSize, HttpFetcher* fetcher)
        : _buffer(std::move(buffer))
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

    void setFormSubmitCallback(FormSubmitCallback cb) override {
        _formSubmitCb = std::move(cb);
    }

    // Called by el_form_submit::on_click()
    void submitForm(const std::string& action, const std::string& method,
                    const std::string& formData) {
        yinfo("HtmlContainer::submitForm: action={} method={} data={}bytes",
              action, method, formData.size());
        if (_formSubmitCb) {
            _formSubmitCb(action, method, formData);
        }
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

        // Resolve font via fontconfig and load into buffer + RawFont
        if (faceName && *faceName) {
            std::string ttfPath = resolveFontPath(
                faceName, weight, fi->italic);
            if (!ttfPath.empty()) {
                fi->rawFont = loadFont(ttfPath, fi->fontId);
                if (fi->fontId >= 0) {
                    yinfo("HtmlContainer: font '{}' w={} i={} -> fontId={}",
                          faceName, weight, fi->italic, fi->fontId);
                }
            }
        }

        // Get font metrics from RawFont
        if (fi->rawFont) {
            fi->ascent = fi->rawFont->fontAscent(fi->size);
            fi->descent = fi->rawFont->fontDescent(fi->size);
        } else {
            fi->ascent = fi->size * 0.8f;
            fi->descent = fi->size * 0.2f;
        }
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
        if (!fi || !text || !*text || !_buffer) return;

        uint32_t packed = packColor(color);
        // Baseline = bottom of position box - descent (matches litehtml reference containers)
        float baseline = static_cast<float>(pos.y + pos.height) - fi->descent;

        ydebug("draw_text: '{}' pos=({},{} {}x{}) size={:.1f} fontId={} ascent={:.1f} descent={:.1f} height={:.1f} baseline={:.1f} color=({},{},{},{})",
              text, pos.x, pos.y, pos.width, pos.height,
              fi->size, fi->fontId, fi->ascent, fi->descent, fi->height, baseline,
              color.red, color.green, color.blue, color.alpha);

        _buffer->addText(static_cast<float>(pos.x), baseline,
                         text, fi->size, packed, _layer, fi->fontId);

        if (fi->decoration & litehtml::font_decoration_underline) {
            float y = baseline + 2.0f;
            float w = measureTextWidth(text, *fi);
            _buffer->addSegment(_layer,
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                0, packed, 1.0f, 0);
        }

        if (fi->decoration & litehtml::font_decoration_linethrough) {
            float y = static_cast<float>(pos.y) + fi->xHeight;
            float w = measureTextWidth(text, *fi);
            _buffer->addSegment(_layer,
                static_cast<float>(pos.x), y,
                static_cast<float>(pos.x) + w, y,
                0, packed, 1.0f, 0);
        }
    }

    void draw_background(
        litehtml::uint_ptr /*hdc*/,
        const std::vector<litehtml::background_paint>& bg) override
    {
        if (!_buffer) {
            yerror("draw_background: _buffer is null!");
            return;
        }

        for (const auto& paint : bg) {
            float x = static_cast<float>(paint.clip_box.x);
            float y = static_cast<float>(paint.clip_box.y);
            float w = static_cast<float>(paint.clip_box.width);
            float h = static_cast<float>(paint.clip_box.height);

            // Draw background image if available
            if (!paint.image.empty() && w > 0 && h > 0) {
                auto it = _imageCache.find(paint.image);
                if (it != _imageCache.end() && !it->second.pixels.empty()) {
                    // Image position: origin_box + position offset
                    float imgX = static_cast<float>(paint.origin_box.x + paint.position_x);
                    float imgY = static_cast<float>(paint.origin_box.y + paint.position_y);
                    // Image size: use computed image_size, fallback to actual pixels
                    float imgW = paint.image_size.width > 0
                        ? static_cast<float>(paint.image_size.width)
                        : static_cast<float>(it->second.width);
                    float imgH = paint.image_size.height > 0
                        ? static_cast<float>(paint.image_size.height)
                        : static_cast<float>(it->second.height);

                    _buffer->addImage(imgX, imgY, imgW, imgH,
                                      it->second.pixels.data(),
                                      static_cast<uint32_t>(it->second.width),
                                      static_cast<uint32_t>(it->second.height),
                                      _layer);
                } else {
                    // Fallback: light gray placeholder box
                    uint32_t placeholderColor = 0xFFE0E0E0;
                    float cx = x + w * 0.5f;
                    float cy = y + h * 0.5f;
                    float round = static_cast<float>(paint.border_radius.top_left_x);
                    _buffer->addBox(_layer, cx, cy, w * 0.5f, h * 0.5f,
                                    placeholderColor, 0, 0, round);
                }
            }

            if (paint.color.alpha == 0) continue;

            uint32_t color = packColor(paint.color);

            // Root/body background: use is_root flag (not width comparison)
            if (paint.is_root) {
                _buffer->setBgColor(color);
                continue;
            }

            if (w > 0 && h > 0) {
                float cx = x + w * 0.5f;
                float cy = y + h * 0.5f;
                float round = static_cast<float>(paint.border_radius.top_left_x);
                _buffer->addBox(_layer, cx, cy, w * 0.5f, h * 0.5f,
                                color, 0, 0, round);
            }
        }
    }

    void draw_borders(
        litehtml::uint_ptr /*hdc*/,
        const litehtml::borders& borders,
        const litehtml::position& draw_pos,
        bool /*root*/) override
    {
        if (!_buffer) return;

        float x = static_cast<float>(draw_pos.x);
        float y = static_cast<float>(draw_pos.y);
        float w = static_cast<float>(draw_pos.width);
        float h = static_cast<float>(draw_pos.height);

        yinfo("draw_borders: pos=({},{} {}x{}) top=({},w={}) right=({},w={}) bottom=({},w={}) left=({},w={})",
               draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height,
               borders.top.color.alpha, borders.top.width,
               borders.right.color.alpha, borders.right.width,
               borders.bottom.color.alpha, borders.bottom.width,
               borders.left.color.alpha, borders.left.width);

        if (borders.top.width > 0 && borders.top.color.alpha > 0) {
            uint32_t c = packColor(borders.top.color);
            float bw = static_cast<float>(borders.top.width);
            auto res = _buffer->addSegment(_layer, x, y + bw * 0.5f,
                                x + w, y + bw * 0.5f, 0, c, bw, 0);
            yinfo("draw_borders: top segment res={}", res ? "ok" : "fail");
        }
        if (borders.bottom.width > 0 && borders.bottom.color.alpha > 0) {
            uint32_t c = packColor(borders.bottom.color);
            float bw = static_cast<float>(borders.bottom.width);
            auto res = _buffer->addSegment(_layer, x, y + h - bw * 0.5f,
                                x + w, y + h - bw * 0.5f, 0, c, bw, 0);
            yinfo("draw_borders: bottom segment res={}", res ? "ok" : "fail");
        }
        if (borders.left.width > 0 && borders.left.color.alpha > 0) {
            uint32_t c = packColor(borders.left.color);
            float bw = static_cast<float>(borders.left.width);
            auto res = _buffer->addSegment(_layer, x + bw * 0.5f, y,
                                x + bw * 0.5f, y + h, 0, c, bw, 0);
            yinfo("draw_borders: left segment res={}", res ? "ok" : "fail");
        }
        if (borders.right.width > 0 && borders.right.color.alpha > 0) {
            uint32_t c = packColor(borders.right.color);
            float bw = static_cast<float>(borders.right.width);
            auto res = _buffer->addSegment(_layer, x + w - bw * 0.5f, y,
                                x + w - bw * 0.5f, y + h, 0, c, bw, 0);
            yinfo("draw_borders: right segment res={}", res ? "ok" : "fail");
        }
    }

    void draw_list_marker(
        litehtml::uint_ptr /*hdc*/,
        const litehtml::list_marker& marker) override
    {
        if (!_buffer) return;

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
            _buffer->addCircle(_layer, x + w * 0.5f, y + h * 0.5f, r,
                               color, 0, 0, 0);
        } else if (marker.marker_type == litehtml::list_style_type_square) {
            float s = std::min(w, h) * 0.3f;
            _buffer->addBox(_layer, x + w * 0.5f, y + h * 0.5f, s, s,
                            color, 0, 0, 0);
        } else if (marker.marker_type == litehtml::list_style_type_circle) {
            float r = std::min(w, h) * 0.3f;
            _buffer->addCircle(_layer, x + w * 0.5f, y + h * 0.5f, r,
                               0, color, 1.0f, 0);
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
        const char* tag_name,
        const litehtml::string_map& attributes,
        const std::shared_ptr<litehtml::document>& doc) override
    {
        if (!tag_name) return nullptr;

        if (strcmp(tag_name, "button") == 0) {
            return std::make_shared<el_form_submit>(doc);
        }
        if (strcmp(tag_name, "input") == 0) {
            auto it = attributes.find("type");
            if (it != attributes.end()) {
                std::string type = it->second;
                for (auto& c : type) c = static_cast<char>(std::tolower(c));
                if (type == "submit") {
                    return std::make_shared<el_form_submit>(doc);
                }
            }
        }
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
            _imageCache[url] = {0, 0, {}};
            return;
        }

        // Decode image to RGBA using stb_image
        int w = 0, h = 0, channels = 0;
        uint8_t* decoded = stbi_load_from_memory(
            reinterpret_cast<const uint8_t*>(body->data()),
            static_cast<int>(body->size()),
            &w, &h, &channels, 4);  // force RGBA

        if (!decoded || w <= 0 || h <= 0) {
            if (decoded) stbi_image_free(decoded);
            _imageCache[url] = {0, 0, {}};
            ywarn("HtmlContainer::load_image: failed to decode {}", url);
            return;
        }

        // Cap to reasonable dimensions
        if (w > 4096 || h > 4096) {
            stbi_image_free(decoded);
            _imageCache[url] = {0, 0, {}};
            ywarn("HtmlContainer::load_image: image too large {}x{}", w, h);
            return;
        }

        // Store decoded pixels
        ImageInfo info;
        info.width = w;
        info.height = h;
        info.pixels.assign(decoded, decoded + w * h * 4);
        stbi_image_free(decoded);

        yinfo("HtmlContainer::load_image: {} -> {}x{} ({} KB)",
              url, w, h, info.pixels.size() / 1024);
        _imageCache[url] = std::move(info);
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
        int fontId;  // buffer font ID (0 = default)
        font::RawFont::Ptr rawFont;  // for text measurement
    };

    //=========================================================================
    // Font loading — reads TTF, registers with both buffer and fontUtil
    //=========================================================================

    font::RawFont::Ptr loadFont(const std::string& ttfPath, int& fontId) {
        // Check cache — avoid loading the same TTF twice
        auto it = _fontCache.find(ttfPath);
        if (it != _fontCache.end()) {
            fontId = it->second.first;
            return it->second.second;
        }

        // Read TTF file
        std::ifstream file(ttfPath, std::ios::binary);
        if (!file) {
            ywarn("HtmlContainer::loadFont: failed to open {}", ttfPath);
            fontId = 0;
            return nullptr;
        }
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
        if (data.empty()) {
            fontId = 0;
            return nullptr;
        }

        // Extract font name from path
        std::string name = ttfPath;
        auto slash = name.rfind('/');
        if (slash != std::string::npos) name = name.substr(slash + 1);

        // Add to buffer (for serialization and builder's MSDF atlas)
        int bufferFontId = _buffer->addFontBlob(data.data(), data.size(), name);

        // Create RawFont for text measurement
        font::RawFont::Ptr rawFont;
        auto fontMgr = font::RawFontManager::instance();
        if (fontMgr) {
            auto rawFontRes = fontMgr.value()->createFromData(data.data(), data.size(), name);
            if (rawFontRes) {
                rawFont = rawFontRes.value();
            } else {
                ywarn("HtmlContainer::loadFont: RawFont failed for {}", ttfPath);
            }
        }

        _fontCache[ttfPath] = {bufferFontId, rawFont};
        fontId = bufferFontId;
        return rawFont;
    }

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

    float measureTextWidth(const char* text, const FontInfo& fi) {
        if (!text || !*text) return 0.0f;
        if (fi.rawFont) {
            return fi.rawFont->measureTextWidth(text, fi.size);
        }
        // Fallback: estimate based on font size
        return fi.size * 0.5f * static_cast<float>(strlen(text));
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
        std::vector<uint8_t> pixels;  // RGBA8 data
    };

    YDrawBuffer::Ptr _buffer;
    HttpFetcher* _fetcher;
    float _defaultFontSize;
    int _viewWidth = 600;
    int _viewHeight = 800;
    uint32_t _layer = 0;

    NavigateCallback _navigateCb;
    CursorCallback _cursorCb;
    FormSubmitCallback _formSubmitCb;

    std::vector<FontInfo*> _fonts;
    std::unordered_map<std::string, std::string> _fontPathCache;  // key -> ttf path
    std::unordered_map<std::string, std::pair<int, font::RawFont::Ptr>> _fontCache;  // ttf path -> (fontId, rawFont)
    std::unordered_map<std::string, ImageInfo> _imageCache;
};

//=============================================================================
// el_form_submit::on_click — defined here because it needs HtmlContainerImpl
//=============================================================================

void el_form_submit::on_click() {
    // Find parent <form>
    std::shared_ptr<litehtml::element> formEl;
    auto el = parent();
    while (el) {
        if (el->tag() == litehtml::_form_) {
            formEl = el;
            break;
        }
        el = el->parent();
    }

    if (!formEl) {
        yinfo("el_form_submit: no parent <form> found");
        // Check for formaction attribute on the element itself
        const char* formaction = get_attr("formaction");
        if (formaction && *formaction) {
            get_document()->container()->on_anchor_click(formaction, shared_from_this());
            return;
        }
        html_tag::on_click();
        return;
    }

    const char* action = formEl->get_attr("action");
    if (!action || !*action) {
        yinfo("el_form_submit: form has no action");
        html_tag::on_click();
        return;
    }

    const char* method = formEl->get_attr("method");
    std::string methodStr = method ? method : "GET";
    for (auto& c : methodStr) c = static_cast<char>(std::toupper(c));

    // Collect all form field data
    std::string formData;
    collectFormFields(formEl, this, formData);

    yinfo("el_form_submit: action={} method={} data={}", action, methodStr, formData);

    auto* ctr = static_cast<HtmlContainerImpl*>(get_document()->container());
    ctr->submitForm(action, methodStr, formData);
}

//=============================================================================
// Factory
//=============================================================================

Result<HtmlContainer::Ptr> HtmlContainer::createImpl(
    std::shared_ptr<YDrawBuffer> buffer,
    float defaultFontSize, HttpFetcher* fetcher)
{
    return Ok(Ptr(new HtmlContainerImpl(std::move(buffer),
                                         defaultFontSize, fetcher)));
}

} // namespace yetty::card
