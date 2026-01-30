#pragma once

#include <memory>
#include <yetty/base/base.h>
#include <yetty/result.hpp>

namespace yetty {

using namespace yetty::base;

class Yetty : public virtual Object, public ObjectFactory<Yetty> {
public:
  using Ptr = std::shared_ptr<Yetty>;

  ~Yetty() override = default;

  static Result<Ptr> createImpl(ContextType &ctx, int argc, char *argv[]) noexcept;

  virtual int run() noexcept = 0;

protected:
  Yetty() = default;
};

} // namespace yetty
