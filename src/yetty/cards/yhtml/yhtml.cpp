#include "yhtml.h"
#include "html-container.h"
#include "http-fetcher.h"
#include <litehtml.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace yetty::card {

//=============================================================================
// YHtmlImpl
//=============================================================================

class YHtmlImpl : public YHtml {
public:
    YHtmlImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YHtml(ctx, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {}

    ~YHtmlImpl() override = default;

    HttpFetcher& fetcher() override { return *_fetcher; }
    const HttpFetcher& fetcher() const override { return *_fetcher; }

    Result<void> init() {
        if (auto res = initBase(); !res) {
            return res;
        }

        // Create HTTP fetcher
        auto fetcherResult = HttpFetcher::create();
        if (!fetcherResult) {
            return Err<void>("YHtml: failed to create HttpFetcher");
        }
        _fetcher = std::move(*fetcherResult);

        parseArgs(_argsStr);

        // Default to white background (standard for web pages)
        setBgColor(0xFFFFFFFF);

        // Load HTML content now, but defer rendering until setCellSize()
        // provides pixel dimensions for computing the viewport width.
        if (auto res = loadContent(); !res) {
            ywarn("YHtml::init: failed to load content: {}", error_msg(res));
        }

        markDirty();
        return Ok();
    }

    //=========================================================================
    // setCellSize — called by GPUScreen at registration and on zoom
    //=========================================================================
    void setCellSize(uint32_t cellWidth, uint32_t cellHeight) override {
        if (_cellWidthPx == cellWidth && _cellHeightPx == cellHeight) return;
        _cellWidthPx = cellWidth;
        _cellHeightPx = cellHeight;

        // Compute HTML viewport width from card pixel dimensions
        float pixelWidth = static_cast<float>(widthCells() * cellWidth);
        if (pixelWidth > 0 && !_htmlContent.empty()) {
            _viewWidth = pixelWidth;
            _needsHtmlRender = true;
            yinfo("YHtml::setCellSize: {}x{} → viewport {}px",
                  cellWidth, cellHeight, _viewWidth);
        }
    }

    //=========================================================================
    // allocateBuffers — render HTML here (after setCellSize provides px dims)
    //=========================================================================
    Result<void> allocateBuffers() override {
        if (_needsHtmlRender) {
            clear();
            setInitParsing(true);
            if (auto res = renderHtml(); !res) {
                ywarn("YHtml::allocateBuffers: render failed: {}", error_msg(res));
            }
            setInitParsing(false);
            _needsHtmlRender = false;
            markDirty();
        }
        return YDrawBase::allocateBuffers();
    }

private:
    void parseArgs(const std::string& args) {
        if (args.empty()) return;

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--input") {
                std::string val;
                if (iss >> val) _inputSource = val;
            } else if (token == "--font-size") {
                float val;
                if (iss >> val) _fontSize = val;
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.size() > 2 &&
                        (colorStr.substr(0, 2) == "0x" ||
                         colorStr.substr(0, 2) == "0X")) {
                        colorStr = colorStr.substr(2);
                    }
                    setBgColor(static_cast<uint32_t>(
                        std::stoul(colorStr, nullptr, 16)));
                }
            }
        }
    }

    Result<void> loadContent() {
        if (!_inputSource.empty()) {
            // Source from plugin args: URL or file path
            if (HttpFetcher::isUrl(_inputSource)) {
                _fetcher->setBaseUrl(_inputSource);
                yinfo("YHtml::loadContent: fetching URL: {}", _inputSource);
                auto body = _fetcher->fetch(_inputSource);
                if (!body) {
                    return Err<void>("YHtml::loadContent: failed to fetch URL");
                }
                _htmlContent = std::move(*body);
            } else {
                yinfo("YHtml::loadContent: reading file: {}", _inputSource);
                std::ifstream file(_inputSource);
                if (!file) {
                    return Err<void>("YHtml::loadContent: failed to open file");
                }
                std::stringstream buffer;
                buffer << file.rdbuf();
                _htmlContent = buffer.str();
            }
        } else if (!_payloadStr.empty()) {
            // Content from payload (stdin or inline)
            _htmlContent = _payloadStr;
        }

        if (_htmlContent.empty()) {
            return Err<void>("YHtml::loadContent: empty content");
        }

        yinfo("YHtml::loadContent: {} bytes", _htmlContent.size());
        return Ok();
    }

    Result<void> renderHtml() {
        if (_htmlContent.empty()) {
            return Err<void>("YHtml::renderHtml: no content loaded");
        }

        // Create container via ObjectFactory
        auto containerResult = HtmlContainer::create(
            this, font(), _fontSize, _fetcher.get());
        if (!containerResult) {
            return Err<void>("YHtml::renderHtml: failed to create HtmlContainer");
        }
        _container = std::move(*containerResult);

        int viewW = static_cast<int>(_viewWidth);
        int viewH = static_cast<int>(_viewWidth * 1.5f);
        _container->setViewportSize(viewW, viewH);

        _document = litehtml::document::createFromString(
            _htmlContent.c_str(), _container.get());

        if (!_document) {
            return Err<void>("YHtml::renderHtml: failed to parse HTML");
        }

        _document->render(viewW);

        int docHeight = _document->height();
        _container->setViewportSize(viewW, docHeight);

        // Scene bounds cover full document — scroll/pan handles navigation
        setSceneBounds(0.0f, 0.0f,
                       static_cast<float>(viewW),
                       static_cast<float>(docHeight));

        litehtml::position clip(0, 0, viewW, docHeight);
        _document->draw(0, 0, 0, &clip);

        yinfo("YHtml::renderHtml: layout {}x{}, prims={} glyphs={}",
              viewW, docHeight, primitiveCount(), glyphCount());

        return Ok();
    }

    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputSource;  // URL or file path from --input arg

    // HTTP client (owned)
    HttpFetcher::Ptr _fetcher;

    // Loaded HTML content (set in loadContent, used in renderHtml)
    std::string _htmlContent;

    // litehtml state
    HtmlContainer::Ptr _container;
    std::shared_ptr<litehtml::document> _document;

    // Cell pixel dimensions (set by setCellSize callback)
    uint32_t _cellWidthPx = 0;
    uint32_t _cellHeightPx = 0;
    bool _needsHtmlRender = false;

    // Rendering parameters
    float _fontSize = 16.0f;
    float _viewWidth = 600.0f;  // default until setCellSize provides real value
    uint32_t _textColor = 0xFFE6E6E6;
};

//=============================================================================
// Factory
//=============================================================================

Result<YHtml::Ptr> YHtml::createImpl(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YHtml::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto impl = Ptr(new YHtmlImpl(ctx, x, y, widthCells, heightCells,
                                   args, payload));

    if (auto res = static_cast<YHtmlImpl*>(impl.get())->init(); !res) {
        yerror("YHtml::create: init FAILED: {}", error_msg(res));
        return Err<Ptr>("YHtml::create: init failed");
    }

    yinfo("YHtml::create: SUCCESS");
    return Ok(std::move(impl));
}

} // namespace yetty::card
