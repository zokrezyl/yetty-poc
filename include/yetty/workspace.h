#pragma once

#include <memory>
#include <yetty/base/base.h>
#include <yetty/result.hpp>
#include <yetty/tile.h>
#include <yetty/ymery/types.h>
#include <yetty/yetty-context.h>

namespace yetty {

using namespace yetty::base;

class Workspace : public EventListener, public ymery::TreeLike, public ObjectFactory<Workspace> {
public:
  using Ptr = std::shared_ptr<Workspace>;

  static Result<Ptr> createImpl(ContextType &ctx, const YettyContext &yettyCtx);

  ~Workspace() override = default;

  // EventListener - default: don't consume
  Result<bool> onEvent(const Event &event) override { return Ok(false); }

  virtual Result<void> render(WGPURenderPassEncoder pass) = 0;
  virtual Result<void> resize(float width, float height) = 0;
  virtual Result<void> setRoot(Tile::Ptr tile) = 0;
  virtual Result<Pane::Ptr> createPane() = 0;

  virtual Result<void> splitPane(ObjectId paneId, Orientation orientation) = 0;
  virtual Result<void> closeTarget(ObjectId targetId) = 0;

  virtual Tile::Ptr rootTile() = 0;
  virtual float width() const = 0;
  virtual float height() const = 0;

  // TreeLike
  Result<std::vector<std::string>> getChildrenNames(const ymery::DataPath& path) override;
  Result<ymery::Dict> getMetadata(const ymery::DataPath& path) override;
  Result<std::vector<std::string>> getMetadataKeys(const ymery::DataPath& path) override;
  Result<ymery::Value> get(const ymery::DataPath& path) override;
  Result<void> set(const ymery::DataPath& path, const ymery::Value& value) override;
  Result<void> addChild(const ymery::DataPath& path, const std::string& name, const ymery::Dict& data) override;
  Result<std::string> asTree(const ymery::DataPath& path, int depth = -1) override;

protected:
  Workspace() = default;
};

} // namespace yetty
