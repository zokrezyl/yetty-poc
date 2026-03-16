#include <yetty/platform/fs-path-manager.h>

#if defined(__ANDROID__)
#include "../shared/android-app-singleton.h"
#include <sys/stat.h>
#include <unistd.h>

namespace yetty {

class AndroidFsPathManager : public FsPathManager {
public:
    Result<void> init() {
        auto appResult = AndroidAppSingleton::instance();
        if (!appResult) {
            return Err<void>("AndroidAppSingleton not available", appResult);
        }
        _dataDir = (*appResult)->getDataDir();
        _nativeLibDir = (*appResult)->getNativeLibDir();
        _binDir = _dataDir + "/bin";

        // Setup bin directory with toybox symlinks
        setupBinDirectory();

        return Ok();
    }

    std::string getShadersDir() const override {
        return _dataDir + "/shaders";
    }

    std::string getMsdfFontsDir() const override {
        return _dataDir + "/msdf-fonts";
    }

    std::string getFontsDir() const override {
        return _dataDir + "/fonts";
    }

    std::string getRuntimeDir() const override {
        return _dataDir;
    }

    std::string getBinDir() const override {
        return _binDir;
    }

private:
    void setupBinDirectory() {
        mkdir(_binDir.c_str(), 0755);

        std::string toyboxLib = _nativeLibDir + "/libtoybox.so";

        static constexpr const char* TOYBOX_COMMANDS[] = {
            "sh", "cat", "ls", "pwd", "echo", "env", "mkdir", "rm",
            "cp", "mv", "ln", "chmod", "touch", "head", "tail", "grep",
            "sort", "uniq", "wc", "tr", "cut", "find", "xargs", "which",
            "ps", "kill", "sleep", "date", "uname", "id", "whoami",
            "clear", "reset", "telnet", "telnetd", nullptr
        };

        if (access(toyboxLib.c_str(), X_OK) == 0) {
            for (int i = 0; TOYBOX_COMMANDS[i]; i++) {
                std::string link = _binDir + "/" + TOYBOX_COMMANDS[i];
                unlink(link.c_str());
                symlink(toyboxLib.c_str(), link.c_str());
            }
        }
    }

    std::string _dataDir;
    std::string _nativeLibDir;
    std::string _binDir;
};

// Factory
Result<FsPathManager::Ptr> FsPathManager::createImpl() {
    auto impl = Ptr(new AndroidFsPathManager());
    auto initResult = static_cast<AndroidFsPathManager*>(impl.get())->init();
    if (!initResult) {
        return Err<Ptr>("AndroidFsPathManager init failed", initResult);
    }
    return Ok(std::move(impl));
}

} // namespace yetty

#endif // __ANDROID__
