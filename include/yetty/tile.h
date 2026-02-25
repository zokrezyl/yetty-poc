#pragma once

#include <memory>
#include <vector>
#include <webgpu/webgpu.h>
#include <yetty/base/base.h>
#include <yetty/result.hpp>
#include <yetty/ymery/types.h>

namespace yetty {

using namespace yetty::base;

class View;
class FrameRenderer;

struct Rect {
  float x = 0, y = 0, width = 0, height = 0;
};

enum class Orientation { Horizontal, Vertical };

// Render context shared across the tile tree for frame rendering
struct TileRenderContext {
  WGPUQueue queue = nullptr;
  uint32_t screenWidth = 0;
  uint32_t screenHeight = 0;
  std::shared_ptr<FrameRenderer> frameRenderer;
};

// Base class for tiles (Split and Pane)
class Tile : public EventListener, public ymery::TreeLike {
public:
  using Ptr = std::shared_ptr<Tile>;

  ~Tile() override = default;

  virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

  // Default: tiles don't consume events
  Result<bool> onEvent(const Event &event) override { return Ok(false); }

  Rect bounds() const { return _bounds; }
  virtual Result<void> setBounds(Rect r) {
    _bounds = r;
    return Ok();
  }

  bool containsPoint(float x, float y) const {
    return x >= _bounds.x && x < _bounds.x + _bounds.width && y >= _bounds.y &&
           y < _bounds.y + _bounds.height;
  }

  // Set render context for frame rendering (propagates to children)
  virtual void setRenderContext(const TileRenderContext &ctx) {
    _renderCtx = ctx;
  }

  // TreeLike — default implementations (leaf node with no children)
  Result<std::vector<std::string>> getChildrenNames(const ymery::DataPath& path) override;
  Result<ymery::Dict> getMetadata(const ymery::DataPath& path) override;
  Result<std::vector<std::string>> getMetadataKeys(const ymery::DataPath& path) override;
  Result<ymery::Value> get(const ymery::DataPath& path) override;
  Result<void> set(const ymery::DataPath& path, const ymery::Value& value) override;
  Result<void> addChild(const ymery::DataPath& path, const std::string& name, const ymery::Dict& data) override;
  Result<std::string> asTree(const ymery::DataPath& path, int depth = -1) override;

  // Override in subclasses to provide type-specific metadata
  virtual ymery::Dict tileMetadata() const;

protected:
  Rect _bounds;
  TileRenderContext _renderCtx;
  Tile() = default;

  // Render frame border around this tile using the render context
  void renderFrame(WGPURenderPassEncoder pass,
                   float r, float g, float b, float a,
                   float thickness);
};

// Split - contains two child tiles
class Split : public Tile, public ObjectFactory<Split> {
public:
  using Ptr = std::shared_ptr<Split>;

  static Result<Ptr> createImpl(Orientation orientation);

  Result<void> render(WGPURenderPassEncoder pass) override = 0;
  Result<void> setBounds(Rect r) override = 0;
  void setRenderContext(const TileRenderContext &ctx) override = 0;

  virtual Result<void> setFirst(Tile::Ptr tile) = 0;
  virtual Result<void> setSecond(Tile::Ptr tile) = 0;
  virtual Tile::Ptr first() const = 0;
  virtual Tile::Ptr second() const = 0;

  virtual Orientation orientation() const = 0;
  virtual float ratio() const = 0;
  virtual Result<void> setRatio(float r) = 0;

  // Tree manipulation helpers
  virtual Result<void> replaceTile(ObjectId targetId, Tile::Ptr replacement) = 0;

protected:
  Split() = default;
};

// Pane - leaf tile that holds views, handles events
class Pane : public Tile, public ObjectFactory<Pane> {
public:
  using Ptr = std::shared_ptr<Pane>;

  static Result<Ptr> createImpl();

  Result<void> render(WGPURenderPassEncoder pass) override = 0;
  Result<void> setBounds(Rect r) override = 0;

  // EventListener
  Result<bool> onEvent(const Event &event) override = 0;

  virtual bool focused() const = 0;
  virtual Rect innerBounds() const = 0;
  virtual Result<void> pushView(std::shared_ptr<View> view) = 0;
  virtual Result<void> popView() = 0;
  virtual std::shared_ptr<View> activeView() = 0;
  virtual Result<void> closeView(ObjectId viewId) = 0;
  virtual size_t viewCount() const = 0;
  virtual bool hasView(ObjectId viewId) const = 0;

protected:
  Pane() = default;
};

} // namespace yetty
