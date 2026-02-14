#pragma once

#include <litehtml.h>
#include <memory>
#include <string>

namespace yetty {
class YDrawWriter;
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

// Render HTML content through a YDrawWriter.
// Returns the container and document (kept alive for interactive use).
HtmlRenderResult renderHtmlToWriter(
    const std::string& html, YDrawWriter* writer,
    float fontSize, float viewWidth,
    HttpFetcher* fetcher = nullptr);

} // namespace yetty::card
