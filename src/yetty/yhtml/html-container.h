#pragma once

#include <litehtml.h>
#include <yetty/base/factory.h>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::card {

class HttpFetcher;

//=============================================================================
// HtmlContainer - litehtml v0.9 document_container that emits primitives
//
// Bridges litehtml's CSS layout callbacks to YDrawBuffer:
//   draw_background() → buffer->addBox()
//   draw_borders()    → buffer->addSegment()
//   draw_text()       → buffer->addText()
//   draw_list_marker() → buffer->addCircle() / buffer->addText()
//
// Uses HttpFetcher (cpr) to download external CSS via import_css().
// Font metrics provided by RawFont (via RawFontManager thread singleton).
//=============================================================================
class HtmlContainer : public litehtml::document_container,
                      public base::ObjectFactory<HtmlContainer> {
public:
    using Ptr = std::shared_ptr<HtmlContainer>;

    static Result<Ptr> createImpl(std::shared_ptr<YDrawBuffer> buffer,
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

    // Callback when a form is submitted (e.g. <input type="submit"> clicked)
    using FormSubmitCallback = std::function<void(const std::string& action,
                                                   const std::string& method,
                                                   const std::string& formData)>;
    virtual void setFormSubmitCallback(FormSubmitCallback cb) = 0;

protected:
    HtmlContainer() = default;
};

} // namespace yetty::card
