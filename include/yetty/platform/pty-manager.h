#pragma once

#include <memory>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>

namespace yetty {

class PTYProvider;

// PtyManager - creates platform-appropriate PTY/shell
//
// Platform implementations:
// - unix.cpp: forkpty (Linux, macOS)
// - windows.cpp: ConPTY
// - android.cpp: toybox or Termux
// - webasm.cpp: JSLinux iframe
//
class PtyManager : public base::ThreadSingleton<PtyManager> {
public:
  using Ptr = std::shared_ptr<PtyManager>;

  static Result<Ptr> createImpl();

  virtual ~PtyManager() = default;

  virtual Result<std::shared_ptr<PTYProvider>> createPTY() = 0;

protected:
  PtyManager() = default;
};

} // namespace yetty
