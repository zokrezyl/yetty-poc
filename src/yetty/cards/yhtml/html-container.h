#pragma once

#include <litehtml.h>
#include <yetty/base/factory.h>
#include <yetty/ms-msdf-font.h>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>

namespace yetty {
class YDrawBuilder;
}

namespace yetty::card {

class HttpFetcher;

//=============================================================================
// HtmlContainer - litehtml v0.9 document_container that emits SDF primitives
//
// Bridges litehtml's CSS layout callbacks to YDrawBuilder:
//   draw_background() → addBox()
//   draw_borders()    → addSegment()
//   draw_text()       → addText()
//   draw_list_marker() → addCircle() / addText()
//
// Uses HttpFetcher (cpr) to download external CSS via import_css().
// Font metrics are derived from atlas glyph data.
//=============================================================================
class HtmlContainer : public litehtml::document_container,
                      public base::ObjectFactory<HtmlContainer> {
public:
    using Ptr = std::shared_ptr<HtmlContainer>;

    static Result<Ptr> createImpl(YDrawBuilder* builder, MsMsdfFont::Ptr font,
                                  float defaultFontSize,
                                  HttpFetcher* fetcher);

    virtual ~HtmlContainer() = default;

    virtual void setViewportSize(int width, int height) = 0;

    // Callback when user clicks a link
    using NavigateCallback = std::function<void(const std::string& url)>;
    virtual void setNavigateCallback(NavigateCallback cb) = 0;

    // Callback when cursor should change (e.g. pointer over links)
    using CursorCallback = std::function<void(const std::string& cursor)>;
    virtual void setCursorCallback(CursorCallback cb) = 0;

protected:
    HtmlContainer() = default;
};

} // namespace yetty::card
