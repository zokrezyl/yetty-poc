#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <memory>
#include <string>
#include <cstdint>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::thorvg {

//=============================================================================
// ThorVgRenderer - Converts ThorVG scene to YDraw primitives
//
// Instead of rasterizing to pixels via SwCanvas, this walks the ThorVG scene
// graph and emits corresponding YDraw SDF primitives (circles, boxes, beziers).
//
// Usage:
//   auto renderer = *ThorVgRenderer::create(buffer);
//   renderer->load(svgContent, "svg");
//   renderer->render();  // primitives now in buffer
//   builder->calculate(); // rebuild grid
//=============================================================================
class ThorVgRenderer : public base::Object,
                       public base::ObjectFactory<ThorVgRenderer> {
public:
    using Ptr = std::shared_ptr<ThorVgRenderer>;

    // Factory - takes the YDrawBuffer to render into
    static Result<Ptr> createImpl(std::shared_ptr<YDrawBuffer> buffer);

    ~ThorVgRenderer() override = default;
    const char* typeName() const override { return "ThorVgRenderer"; }

    // Load content from string (SVG, Lottie JSON, or raw data)
    // mimeType: "svg", "lottie", or nullptr for auto-detect
    virtual Result<void> load(const std::string& data,
                              const char* mimeType = nullptr) = 0;

    // Load content from file path
    virtual Result<void> loadFile(const std::string& path) = 0;

    // Get content dimensions (from SVG viewBox or Lottie size)
    virtual float contentWidth() const = 0;
    virtual float contentHeight() const = 0;

    // Animation support
    virtual bool isAnimated() const = 0;
    virtual float totalFrames() const = 0;
    virtual float duration() const = 0;

    // Set current animation frame (0 to totalFrames-1)
    virtual void setFrame(float frame) = 0;

    // Render current frame to YDrawBuffer
    // Clears buffer and adds all primitives from the ThorVG scene
    virtual Result<void> render() = 0;

    // Clear loaded content
    virtual void clear() = 0;

protected:
    ThorVgRenderer() = default;
};

//=============================================================================
// ThorVG engine lifecycle (reference counted, thread-safe)
//=============================================================================
Result<void> thorvgInit();
void thorvgTerm();

} // namespace yetty::thorvg
