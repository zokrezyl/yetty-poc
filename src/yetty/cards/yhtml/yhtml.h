#pragma once

#include "../../ydraw/ydraw-base.h"
#include <yetty/base/factory.h>
#include <string>
#include <cstdint>
#include <memory>

// Forward declare litehtml types
namespace litehtml {
class document;
class context;
}

namespace yetty::card {

class HtmlContainer;
class HttpFetcher;

//=============================================================================
// YHtml - HTML/CSS rendering card using litehtml + YDrawBase
//
// Parses HTML+CSS via litehtml, renders boxes and text as SDF primitives
// and MSDF glyphs via YDrawBase. GPU-native HTML rendering.
//
// Payload can be:
//   - A URL (http:// or https://) → fetched via cpr
//   - A file path                  → read from disk
//   - "inline:<html>"              → used directly
//   - Raw HTML with tags           → used directly
//
// Subclasses YDrawBase to reuse:
//   - Spatial hash grid with O(1) lookup
//   - MSDF text rendering via addText()
//   - SDF primitives via addBox(), addSegment(), addCircle()
//   - Zoom/pan support
//=============================================================================
class YHtml : public yetty::YDrawBase,
              public base::ObjectFactory<YHtml> {
public:
    using Ptr = std::shared_ptr<YHtml>;

    static Result<Ptr> createImpl(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~YHtml() override = default;

    const char* typeName() const override { return "yhtml"; }

    virtual HttpFetcher& fetcher() = 0;
    virtual const HttpFetcher& fetcher() const = 0;

protected:
    YHtml(const YettyContext& ctx,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : YDrawBase(ctx, x, y, widthCells, heightCells) {}
};

} // namespace yetty::card
