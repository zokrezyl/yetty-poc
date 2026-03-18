#include <yetty/platform/pty-manager.h>
#include "../../telnet/telnet-pty-reader.h"
#include "../shared/android-app-singleton.h"

#include <ytrace/ytrace.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

namespace yetty {

class AndroidPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<PtyReader::Ptr> createPtyReader(const PtyConfig& config) override {
        ydebug("AndroidPtyManager::createPtyReader called");

        auto& paths = AndroidAppSingleton::instance().paths();
        if (paths.dataDir.empty() || paths.nativeLibDir.empty()) {
            return Err<PtyReader::Ptr>("Android paths not initialized");
        }

        PtyConfig telnetConfig = config;
        std::string telnetAddr;

        if (!telnetConfig.shell.empty() && telnetConfig.shell[0] != '/') {
            telnetAddr = telnetConfig.shell;
            ydebug("Using user-specified address: {}", telnetAddr);
        } else {
            telnetAddr = "127.0.0.1:8023";
            ydebug("Using default Termux address: {}", telnetAddr);
        }

        telnetConfig.shell = telnetAddr;

        auto reader = std::make_shared<telnet::TelnetPtyReader>();
        ydebug("TelnetPtyReader created, connecting to {}...", telnetAddr);

        if (auto res = reader->init(telnetConfig); res) {
            ydebug("Connected to Termux!");
            return Ok<PtyReader::Ptr>(reader);
        }

        // Termux not available, try toybox fallback
        ydebug("Termux not available, trying toybox fallback...");

        std::string toyboxExe = paths.nativeLibDir + "/libtoybox.so";
        if (access(toyboxExe.c_str(), X_OK) != 0) {
            return Err<PtyReader::Ptr>("toybox not available or not executable");
        }

        ydebug("Starting toybox telnetd...");

        std::string shPath = paths.nativeLibDir + "/liblogin.so";

        setenv("HOME", paths.dataDir.c_str(), 1);
        setenv("PATH", (paths.binDir + ":" + paths.nativeLibDir + ":" + paths.dataDir + ":/system/bin").c_str(), 1);
        setenv("SHELL", (paths.binDir + "/sh").c_str(), 1);
        setenv("TERM", "xterm-256color", 1);
        setenv("USER", "shell", 1);

        pid_t pid = fork();
        if (pid == 0) {
            chdir(paths.dataDir.c_str());
            execl(toyboxExe.c_str(), "telnetd",
                  "-l", shPath.c_str(),
                  "-p", "8024",
                  "-F",
                  nullptr);
            yerror("execl failed: {}", strerror(errno));
            _exit(1);
        } else if (pid > 0) {
            ydebug("toybox telnetd started with pid {}", pid);
            usleep(200000);

            telnetConfig.shell = "127.0.0.1:8024";
            auto reader2 = std::make_shared<telnet::TelnetPtyReader>();
            if (auto res = reader2->init(telnetConfig); res) {
                ydebug("Connected to toybox telnetd!");
                return Ok<PtyReader::Ptr>(reader2);
            } else {
                yerror("Failed to connect to toybox: {}", res.error().message());
            }
        } else {
            yerror("fork() failed: {}", strerror(errno));
        }

        return Err<PtyReader::Ptr>("Failed to connect to any telnet server");
    }
};

Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new AndroidPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("AndroidPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty
