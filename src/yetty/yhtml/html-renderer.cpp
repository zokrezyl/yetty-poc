#include "html-renderer.h"
#include "html-container.h"
#include "../ydraw/ydraw-buffer.h"
#include <ytrace/ytrace.hpp>

namespace yetty::card {

HtmlRenderResult renderHtmlToBuffer(
    const std::string& html, std::shared_ptr<YDrawBuffer> buffer,
    float fontSize, float viewWidth,
    HttpFetcher* fetcher,
    const char* userStyles)
{
    HtmlRenderResult result;

    if (html.empty() || !buffer) return result;

    auto containerResult = HtmlContainer::create(
        buffer, fontSize, fetcher);
    if (!containerResult) {
        ywarn("renderHtmlToBuffer: failed to create HtmlContainer");
        return result;
    }
    result.container = std::move(*containerResult);

    int viewW = static_cast<int>(viewWidth);
    int viewH = static_cast<int>(viewWidth * 1.5f);
    result.container->setViewportSize(viewW, viewH);

    result.document = litehtml::document::createFromString(
        html.c_str(), result.container.get(), litehtml::master_css,
        userStyles ? userStyles : "");

    if (!result.document) {
        ywarn("renderHtmlToBuffer: failed to parse HTML");
        return result;
    }

    result.document->render(viewW);

    result.documentWidth = viewW;
    result.documentHeight = result.document->height();
    result.container->setViewportSize(viewW, result.documentHeight);

    yinfo("renderHtmlToBuffer: viewW={} docHeight={} buffer primCount before draw={}",
          viewW, result.documentHeight, buffer->primCount());

    litehtml::position clip(0, 0, viewW, result.documentHeight);
    result.document->draw(0, 0, 0, &clip);

    yinfo("renderHtmlToBuffer: buffer primCount after draw={}", buffer->primCount());

    return result;
}

} // namespace yetty::card
