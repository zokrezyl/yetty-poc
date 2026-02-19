#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <string>
#include <memory>

namespace yetty {
class YDrawBuilder;
class YDrawBuffer;
}

namespace yetty::maze { class MazeRenderer; }

namespace yetty::card {

//=============================================================================
// YDrawMaze - Procedural maze with animated solver
//
// Thin card wrapper around maze::MazeRenderer library.
//=============================================================================
class YDrawMaze : public Card,
                  public base::ObjectFactory<YDrawMaze> {
public:
    using Ptr = std::shared_ptr<YDrawMaze>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    YDrawMaze(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args);

    ~YDrawMaze() override;

    const char* typeName() const override { return "ydraw-maze"; }

    // Card lifecycle
    bool needsBuffer() const override { return true; }
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return _shaderGlyph; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }
    bool needsBufferRealloc() override;
    void renderToStaging(float time) override;
    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> finalize() override;
    Result<void> dispose() override;
    void suspend() override;

    Result<void> init();

private:
    // Renderer (library)
    std::unique_ptr<maze::MazeRenderer> _renderer;
    uint32_t _bgColor = 0xFF1A1A2E;

    // Builder (owns GPU buffers)
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    std::shared_ptr<yetty::YDrawBuffer> _buffer;
    std::shared_ptr<YDrawBuilder> _builder;

    // Screen ID
    base::ObjectId _screenId = 0;

    // Common card state
    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x = 0, _y = 0;
    uint32_t _widthCells = 0, _heightCells = 0;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
