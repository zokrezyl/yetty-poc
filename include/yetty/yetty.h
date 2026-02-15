#pragma once

#include <memory>
#include <yetty/base/base.h>
#include <yetty/result.hpp>

#if defined(__ANDROID__)
struct android_app;
#endif

namespace yetty {

using namespace yetty::base;

class Yetty : public virtual Object, public ObjectFactory<Yetty> {
public:
  using Ptr = std::shared_ptr<Yetty>;

  ~Yetty() override = default;

  static Result<Ptr> createImpl(ContextType &ctx, int argc, char *argv[]) noexcept;

#if defined(__ANDROID__)
  static Result<Ptr> createImpl(ContextType &ctx, struct android_app* app) noexcept;
#endif

  virtual Result<void> run() noexcept = 0;

protected:
  Yetty() = default;
};

} // namespace yetty
