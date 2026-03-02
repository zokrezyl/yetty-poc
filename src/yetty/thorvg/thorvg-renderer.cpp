#include "thorvg-renderer.h"
#include "../ydraw/ydraw-buffer.h"
#include "../ydraw/ydraw-types.gen.h"
#include <thorvg.h>
#include <ytrace/ytrace.hpp>
#include <cstring>
#include <mutex>

namespace yetty::thorvg {

//=============================================================================
// ThorVG engine reference counting
//=============================================================================

static int s_thorvgRefCount = 0;
static std::mutex s_thorvgMutex;

Result<void> thorvgInit() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (s_thorvgRefCount == 0) {
        auto result = tvg::Initializer::init(0);
        if (result != tvg::Result::Success) {
            return Err<void>("ThorVG: tvg::Initializer::init failed");
        }
        uint32_t major, minor, micro;
        const char* version = tvg::Initializer::version(&major, &minor, &micro);
        yinfo("ThorVG engine initialized: {}", version ? version : "unknown");
    }
    ++s_thorvgRefCount;
    return Ok();
}

void thorvgTerm() {
    std::lock_guard<std::mutex> lock(s_thorvgMutex);
    if (--s_thorvgRefCount == 0) {
        tvg::Initializer::term();
        yinfo("ThorVG engine terminated");
    }
}

//=============================================================================
// Color conversion helpers
//=============================================================================

static uint32_t rgbaToPackedABGR(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // YDraw uses ABGR format: 0xAABBGGRR
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(b) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           static_cast<uint32_t>(r);
}

//=============================================================================
// ThorVgRendererImpl
//=============================================================================

class ThorVgRendererImpl : public ThorVgRenderer {
public:
    explicit ThorVgRendererImpl(std::shared_ptr<YDrawBuffer> buffer)
        : _buffer(std::move(buffer)) {}

    ~ThorVgRendererImpl() override {
        clear();
        thorvgTerm();
    }

    Result<void> load(const std::string& data, const char* mimeType) override {
        clear();

        // Auto-detect content type if not specified
        std::string detectedType;
        if (!mimeType) {
            size_t start = data.find_first_not_of(" \t\n\r");
            if (start != std::string::npos && data[start] == '{') {
                detectedType = "lottie";
            } else if (data.find("<svg") != std::string::npos) {
                detectedType = "svg";
            }
            mimeType = detectedType.empty() ? nullptr : detectedType.c_str();
        }

        // Create Animation wrapper (manages Picture lifetime)
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("ThorVgRenderer: failed to create Animation");
        }

        tvg::Picture* picture = _animation->picture();
        if (!picture) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to get picture from Animation");
        }

        // Load content
        tvg::Result result;
        if (mimeType && std::string(mimeType) == "lottie") {
            result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()),
                                   "lottie", nullptr, true);
            if (result == tvg::Result::Success) {
                _isAnimated = true;
                _totalFrames = _animation->totalFrame();
                _duration = _animation->duration();
            }
        } else {
            result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()),
                                   mimeType, nullptr, true);
            _isAnimated = false;
        }

        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to load content");
        }

        // Get content dimensions
        float w = 0, h = 0;
        picture->size(&w, &h);
        _contentWidth = w > 0 ? w : 256;
        _contentHeight = h > 0 ? h : 256;

        _picture = picture;
        ydebug("ThorVgRenderer: loaded {}x{} animated={}", _contentWidth, _contentHeight, _isAnimated);
        return Ok();
    }

    Result<void> loadFile(const std::string& path) override {
        clear();

        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("ThorVgRenderer: failed to create Animation");
        }

        tvg::Picture* picture = _animation->picture();
        if (!picture) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to get picture from Animation");
        }

        auto result = picture->load(path.c_str());
        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("ThorVgRenderer: failed to load file: " + path);
        }

        // Check if it's animated (Lottie)
        _totalFrames = _animation->totalFrame();
        _duration = _animation->duration();
        _isAnimated = (_totalFrames > 1);

        float w = 0, h = 0;
        picture->size(&w, &h);
        _contentWidth = w > 0 ? w : 256;
        _contentHeight = h > 0 ? h : 256;

        _picture = picture;
        return Ok();
    }

    float contentWidth() const override { return _contentWidth; }
    float contentHeight() const override { return _contentHeight; }

    bool isAnimated() const override { return _isAnimated; }
    float totalFrames() const override { return _totalFrames; }
    float duration() const override { return _duration; }

    void setFrame(float frame) override {
        if (_animation && _isAnimated) {
            _currentFrame = frame;
            _animation->frame(frame);
        }
    }

    Result<void> render() override {
        if (!_picture) {
            return Err<void>("ThorVgRenderer: no content loaded");
        }

        _buffer->clear();
        _nextPrimId = 0;

        // Use Accessor to traverse scene tree
        auto accessor = std::unique_ptr<tvg::Accessor>(tvg::Accessor::gen());
        if (accessor) {
            accessor->set(_picture, [](const tvg::Paint* paint, void* data) -> bool {
                auto* self = static_cast<ThorVgRendererImpl*>(data);
                self->renderPaint(paint);
                return true;  // continue traversal
            }, this);
        }

        ydebug("ThorVgRenderer::render: emitted {} primitives", _nextPrimId);
        return Ok();
    }

    void clear() override {
        _animation.reset();
        _picture = nullptr;
        _contentWidth = 0;
        _contentHeight = 0;
        _isAnimated = false;
        _totalFrames = 0;
        _duration = 0;
        _currentFrame = 0;
    }

private:
    //=========================================================================
    // Scene graph traversal
    //=========================================================================

    void renderPaint(const tvg::Paint* paint) {
        if (!paint) return;

        // Get transform - cast away const since Accessor provides non-const access
        auto* mutablePaint = const_cast<tvg::Paint*>(paint);
        tvg::Matrix& m = mutablePaint->transform();

        // Apply translation from matrix
        float tx = m.e13;
        float ty = m.e23;

        // Check paint type
        auto type = paint->type();

        if (type == tvg::Type::Shape) {
            renderShape(static_cast<const tvg::Shape*>(paint), tx, ty);
        }
        // Scene and Picture children are handled by Accessor traversal
    }

    void renderShape(const tvg::Shape* shape, float ox, float oy) {
        if (!shape) return;

        // Get fill color (using getter overload)
        uint8_t fr = 0, fg = 0, fb = 0, fa = 0;
        shape->fill(&fr, &fg, &fb, &fa);
        uint32_t fillColor = (fa > 0) ? rgbaToPackedABGR(fr, fg, fb, fa) : 0;

        // Get stroke
        float strokeWidth = shape->strokeWidth();
        uint8_t sr = 0, sg = 0, sb = 0, sa = 0;
        shape->strokeFill(&sr, &sg, &sb, &sa);
        uint32_t strokeColor = (sa > 0 && strokeWidth > 0)
                               ? rgbaToPackedABGR(sr, sg, sb, sa) : 0;

        // Skip invisible shapes
        if (fillColor == 0 && strokeColor == 0) return;

        // Get path data using the combined path() method
        const tvg::PathCommand* cmds = nullptr;
        uint32_t cmdCount = 0;
        const tvg::Point* pts = nullptr;
        uint32_t ptCount = 0;

        if (shape->path(&cmds, &cmdCount, &pts, &ptCount) != tvg::Result::Success) {
            return;
        }

        if (cmdCount == 0 || ptCount == 0) return;

        // Convert path to YDraw primitives
        renderPath(cmds, cmdCount, pts, ptCount, ox, oy, fillColor, strokeColor, strokeWidth);
    }

    void renderPath(const tvg::PathCommand* cmds, uint32_t cmdCount,
                    const tvg::Point* pts, uint32_t ptCount,
                    float ox, float oy,
                    uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
        
        tvg::Point current = {0, 0};
        tvg::Point start = {0, 0};
        uint32_t ptIdx = 0;

        for (uint32_t i = 0; i < cmdCount && ptIdx < ptCount; ++i) {
            switch (cmds[i]) {
                case tvg::PathCommand::MoveTo:
                    if (ptIdx < ptCount) {
                        current = pts[ptIdx++];
                        start = current;
                    }
                    break;

                case tvg::PathCommand::LineTo:
                    if (ptIdx < ptCount) {
                        tvg::Point end = pts[ptIdx++];
                        // Emit segment primitive (layer=0, id=AUTO)
                        auto res = _buffer->addSegment(0,
                            current.x + ox, current.y + oy,
                            end.x + ox, end.y + oy,
                            fillColor, strokeColor, strokeWidth, 0.0f);
                        if (res) _nextPrimId++;
                        current = end;
                    }
                    break;

                case tvg::PathCommand::CubicTo:
                    if (ptIdx + 2 < ptCount) {
                        tvg::Point cp1 = pts[ptIdx++];
                        tvg::Point cp2 = pts[ptIdx++];
                        tvg::Point end = pts[ptIdx++];
                        // Emit cubic bezier primitive (layer=0, id=AUTO)
                        auto res = _buffer->addBezier3(0,
                            current.x + ox, current.y + oy,
                            cp1.x + ox, cp1.y + oy,
                            cp2.x + ox, cp2.y + oy,
                            end.x + ox, end.y + oy,
                            fillColor, strokeColor, strokeWidth, 0.0f);
                        if (res) _nextPrimId++;
                        current = end;
                    }
                    break;

                case tvg::PathCommand::Close:
                    // Close path - draw line back to start if needed
                    if (current.x != start.x || current.y != start.y) {
                        auto res = _buffer->addSegment(0,
                            current.x + ox, current.y + oy,
                            start.x + ox, start.y + oy,
                            fillColor, strokeColor, strokeWidth, 0.0f);
                        if (res) _nextPrimId++;
                    }
                    current = start;
                    break;
            }
        }
    }

    //=========================================================================
    // Members
    //=========================================================================

    std::shared_ptr<YDrawBuffer> _buffer;
    std::unique_ptr<tvg::Animation> _animation;
    tvg::Picture* _picture = nullptr;

    float _contentWidth = 0;
    float _contentHeight = 0;
    bool _isAnimated = false;
    float _totalFrames = 0;
    float _duration = 0;
    float _currentFrame = 0;

    uint32_t _nextPrimId = 0;
};

//=============================================================================
// Factory
//=============================================================================

Result<ThorVgRenderer::Ptr> ThorVgRenderer::createImpl(
    std::shared_ptr<YDrawBuffer> buffer)
{
    if (!buffer) {
        return Err<Ptr>("ThorVgRenderer: null buffer");
    }

    // Initialize ThorVG engine (ref-counted)
    if (auto res = thorvgInit(); !res) {
        return Err<Ptr>("ThorVgRenderer: engine init failed", res);
    }

    return Ok<Ptr>(std::make_shared<ThorVgRendererImpl>(std::move(buffer)));
}

} // namespace yetty::thorvg
