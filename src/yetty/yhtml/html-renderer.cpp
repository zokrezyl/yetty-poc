#include "html-renderer.h"
#include "html-container.h"
#include <yetty/ydraw-writer.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

HtmlRenderResult renderHtmlToWriter(
    const std::string& html, YDrawWriter* writer,
    float fontSize, float viewWidth,
    HttpFetcher* fetcher)
{
    HtmlRenderResult result;

    if (html.empty() || !writer) return result;

    auto containerResult = HtmlContainer::create(
        writer, nullptr, fontSize, fetcher);
    if (!containerResult) {
        ywarn("renderHtmlToWriter: failed to create HtmlContainer");
        return result;
    }
    result.container = std::move(*containerResult);

    int viewW = static_cast<int>(viewWidth);
    int viewH = static_cast<int>(viewWidth * 1.5f);
    result.container->setViewportSize(viewW, viewH);

    result.document = litehtml::document::createFromString(
        html.c_str(), result.container.get());

    if (!result.document) {
        ywarn("renderHtmlToWriter: failed to parse HTML");
        return result;
    }

    result.document->render(viewW);

    result.documentWidth = viewW;
    result.documentHeight = result.document->height();
    result.container->setViewportSize(viewW, result.documentHeight);

    litehtml::position clip(0, 0, viewW, result.documentHeight);
    result.document->draw(0, 0, 0, &clip);

    return result;
}

} // namespace yetty::card
