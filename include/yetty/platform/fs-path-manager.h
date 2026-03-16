#pragma once

#include <yetty/base/factory.h>
#include <string>

namespace yetty {

// FsPathManager - platform-specific filesystem paths
//
// Platform implementations:
// - linux.cpp, macos.cpp, windows.cpp, android.cpp, webasm.cpp
//
class FsPathManager : public base::ThreadSingleton<FsPathManager> {
public:
    virtual ~FsPathManager() = default;

    // Asset directories (shaders, fonts extracted via incbin)
    virtual std::string getShadersDir() const = 0;
    virtual std::string getMsdfFontsDir() const = 0;
    virtual std::string getFontsDir() const = 0;

    // Runtime directory (sockets, temp files)
    virtual std::string getRuntimeDir() const = 0;

    // Binary directory (for Android: symlinks to toybox commands)
    virtual std::string getBinDir() const = 0;

protected:
    FsPathManager() = default;
};

} // namespace yetty
