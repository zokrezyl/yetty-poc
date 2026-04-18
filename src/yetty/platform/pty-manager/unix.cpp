#include <yetty/platform/pty-manager.h>

// Unix PTY: Linux, macOS desktop (not iOS - iOS uses telnet/SSH)
#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !defined(_WIN32) && !YETTY_IOS

#include "../pty-reader/unix.h"
#include "../../ssh/ssh-pty-reader.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

class UnixPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<PtyReader::Ptr> createPtyReader(const PtyConfig& config) override {
        // Check if SSH mode is configured via PtyConfig
        if (!config.sshHost.empty()) {
            ssh::SshConfig sshCfg;
            sshCfg.host = config.sshHost;
            sshCfg.port = config.sshPort;
            sshCfg.username = config.sshUser;
            sshCfg.privateKeyPath = config.sshIdentityFile;
            sshCfg.cols = config.cols;
            sshCfg.rows = config.rows;

            yinfo("SSH mode: connecting to {}@{}:{}",
                  sshCfg.username.empty() ? "(prompt)" : sshCfg.username,
                  sshCfg.host, sshCfg.port);

            auto result = ssh::createSshPtyReader(config, sshCfg);
            if (result) {
                yinfo("SSH connection established");
                return result;
            }
            ywarn("SSH connection failed: {}", result.error().message());
            return result;
        }

        // Default: local PTY
        auto reader = std::make_shared<PtyReaderUnix>();
        if (auto res = reader->init(config); !res) {
            return Err<PtyReader::Ptr>("Failed to create PtyReaderUnix", res);
        }
        return Ok<PtyReader::Ptr>(reader);
    }
};

Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new UnixPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("UnixPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // Unix PTY
