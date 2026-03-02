#include <yetty/pty-reader.h>

#if defined(__ANDROID__)

#include "telnet/telnet-pty-reader.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

/**
 * Android PTY Factory
 *
 * On Android, we use telnet to connect to a local server (Termux).
 * The shell path in PtyConfig is interpreted as "host:port" or just "port".
 *
 * Default: localhost:8023 (Termux telnetd default port)
 *
 * Setup in Termux:
 *   pkg install busybox
 *   busybox telnetd -l /bin/bash -p 8023 -F
 *
 * Or with socat for better PTY support:
 *   pkg install socat
 *   socat TCP-LISTEN:8023,reuseaddr,fork EXEC:bash,pty,stderr,setsid
 */
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config) {
    PtyConfig telnetConfig = config;

    // If shell looks like a path (starts with /), use default telnet address
    // Otherwise interpret as host:port
    if (telnetConfig.shell.empty() || telnetConfig.shell[0] == '/') {
        telnetConfig.shell = "127.0.0.1:8023";
        yinfo("Android PtyReader: using default telnet address {}", telnetConfig.shell);
    }

    auto reader = std::make_shared<telnet::TelnetPtyReader>();
    if (auto res = reader->init(telnetConfig); !res) {
        return Err<Ptr>("Failed to initialize TelnetPtyReader", res);
    }
    return Ok<Ptr>(reader);
}

} // namespace yetty

#endif // __ANDROID__
