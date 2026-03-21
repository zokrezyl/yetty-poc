#pragma once

#include <memory>
#include <yetty/base/base.h>
#include <yetty/config.h>
#include <yetty/result.hpp>

namespace yetty {

using namespace yetty::base;

class Yetty : public virtual Object, public ObjectFactory<Yetty> {
public:
  using Ptr = std::shared_ptr<Yetty>;

  ~Yetty() override = default;

  // Create Yetty with a fully initialized Config
  static Result<Ptr> createImpl(ContextType &ctx, Config::Ptr config) noexcept;

  virtual Result<void> run() noexcept = 0;

  // Single frame iteration (for Android/external event loop)
  virtual Result<void> iterate() noexcept = 0;

protected:
  Yetty() = default;
};

} // namespace yetty
