#pragma once

#include <litehtml.h>
#include <memory>
#include <string>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::card {

class HtmlContainer;
class HttpFetcher;

struct HtmlRenderResult {
    int documentWidth = 0;
    int documentHeight = 0;
    std::shared_ptr<HtmlContainer> container;
    std::shared_ptr<litehtml::document> document;
};

// Render HTML content into a YDrawBuffer.
// Returns the container and document (kept alive for interactive use).
// userStyles: optional CSS applied with highest priority (user stylesheet).
HtmlRenderResult renderHtmlToBuffer(
    const std::string& html, std::shared_ptr<YDrawBuffer> buffer,
    float fontSize, float viewWidth,
    HttpFetcher* fetcher = nullptr,
    const char* userStyles = nullptr);

} // namespace yetty::card
