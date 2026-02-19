#include "ydraw-maze.h"
#include "maze-renderer.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/ydraw-types.gen.h"
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YDrawMaze::YDrawMaze(const YettyContext& ctx,
                     int32_t x, int32_t y,
                     uint32_t widthCells, uint32_t heightCells,
                     const std::string& args)
    : _screenId(ctx.screenId)
    , _cardMgr(ctx.cardManager)
    , _gpu(ctx.gpu)
    , _fontManager(ctx.fontManager)
    , _gpuAllocator(ctx.gpuAllocator)
    , _x(x), _y(y)
    , _widthCells(widthCells), _heightCells(heightCells)
{
    _shaderGlyph = SHADER_GLYPH;
    _buffer = *yetty::YDrawBuffer::create();

    auto cfg = maze::parseArgs(args);
    _bgColor = cfg.bgColor;
    _renderer = std::make_unique<maze::MazeRenderer>(cfg);
}

YDrawMaze::~YDrawMaze() { dispose(); }

//=============================================================================
// Initialization
//=============================================================================

Result<void> YDrawMaze::init() {
    auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
    if (!metaResult) {
        return Err<void>("YDrawMaze::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;

    auto builderRes = YDrawBuilder::create(
        _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
    if (!builderRes) {
        return Err<void>("YDrawMaze::init: failed to create builder", builderRes);
    }
    _builder = *builderRes;

    _builder->setSceneBounds(0, 0, _renderer->config().sceneWidth, _renderer->config().sceneHeight);
    _builder->setBgColor(_bgColor);
    _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
    _builder->setViewport(_widthCells, _heightCells);

    return Ok();
}

//=============================================================================
// Card lifecycle
//=============================================================================

bool YDrawMaze::needsBufferRealloc() {
    if (!_builder) return false;
    return _builder->needsBufferRealloc();
}

void YDrawMaze::renderToStaging(float time) {
    if (!_builder) return;
    _renderer->render(_buffer, time);
    _builder->calculate();
}

void YDrawMaze::declareBufferNeeds() {
    if (!_builder) return;
    if (auto res = _builder->declareBufferNeeds(); !res) {
        yerror("YDrawMaze::declareBufferNeeds: {}", error_msg(res));
    }
}

Result<void> YDrawMaze::allocateBuffers() {
    if (!_builder) return Ok();
    return _builder->allocateBuffers();
}

Result<void> YDrawMaze::finalize() {
    if (!_builder) return Ok();
    return _builder->writeBuffers();
}

Result<void> YDrawMaze::dispose() {
    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YDrawMaze::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
}

void YDrawMaze::suspend() {
}

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YDrawMaze::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    (void)payload;

    yinfo("YDrawMaze::create: pos=({},{}) size={}x{} args='{}'", x, y, widthCells, heightCells, args);

    if (!ctx.cardManager) {
        return Err<CardPtr>("YDrawMaze::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDrawMaze>(ctx, x, y, widthCells, heightCells, args);

    if (auto res = card->init(); !res) {
        yerror("YDrawMaze::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YDrawMaze::create: init failed");
    }

    yinfo("YDrawMaze::create: SUCCESS");
    return Ok<CardPtr>(card);
}

Result<YDrawMaze::Ptr> YDrawMaze::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create YDrawMaze", result);
    }
    auto maze = std::dynamic_pointer_cast<YDrawMaze>(*result);
    if (!maze) {
        return Err<Ptr>("Created card is not a YDrawMaze");
    }
    return Ok(maze);
}

} // namespace yetty::card
