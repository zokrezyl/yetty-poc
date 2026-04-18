//=============================================================================
// OSC Integration Tests
//
// Tests the ACTUAL data flow: ymux output -> PTY -> OscScanner -> processing
// This simulates what happens when ymux runs inside yetty terminal.
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/osc-scanner.h>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

using namespace boost::ut;
using namespace yetty;

// Simulates terminal's onPtyData + processPtyData flow
class TerminalSimulator {
public:
    using OscHandler = std::function<bool(const std::string& body)>;

    void setOscHandler(OscHandler handler) {
        _oscHandler = std::move(handler);
    }

    // Get all non-OSC output that was written to "screen"
    const std::string& screenOutput() const { return _screenOutput; }

    // Simulates terminal's onPtyData - called when PTY has data
    void onPtyData(const char* data, size_t len) {
        if (data == nullptr || len == 0) return;

        // Accumulate in buffer
        size_t oldSize = _ptyBuffer.size();
        _ptyBuffer.resize(oldSize + len);
        std::memcpy(_ptyBuffer.data() + oldSize, data, len);

        // Scan for incomplete OSC
        _scanner.scan(data, len);

        // If potentially in OSC (including just saw ESC), wait for more data
        if (_scanner.needsMoreData()) {
            return;
        }

        // Process complete buffer
        if (!_ptyBuffer.empty()) {
            _scanner.reset();
            processPtyData(_ptyBuffer.data(), _ptyBuffer.size());
            _ptyBuffer.clear();
        }
    }

    uint32_t oscHandledCount() const { return _oscHandledCount; }
    uint32_t oscForwardedCount() const { return _oscForwardedCount; }

private:
    enum class OscState { Normal, Esc, InOsc, OscEscEnd };

    void processPtyData(const char* data, size_t len) {
        size_t i = 0;
        size_t normalStart = 0;

        auto flushNormal = [&](size_t end) {
            if (end > normalStart) {
                _screenOutput.append(data + normalStart, end - normalStart);
            }
        };

        auto handleOsc = [&](size_t terminatorEnd, bool isST) {
            bool handled = false;
            if (_oscHandler) {
                handled = _oscHandler(_oscBuffer);
            }
            if (handled) {
                _oscHandledCount++;
            } else {
                // Forward to screen (as vterm would receive it)
                _oscForwardedCount++;
                _screenOutput += "\033]";
                _screenOutput += _oscBuffer;
                _screenOutput += isST ? "\033\\" : "\007";
            }
            _oscBuffer.clear();
            _oscState = OscState::Normal;
            normalStart = terminatorEnd;
        };

        while (i < len) {
            char c = data[i];

            switch (_oscState) {
            case OscState::Normal:
                if (c == '\033') {
                    _oscState = OscState::Esc;
                    _oscEscPos = i;
                }
                i++;
                break;

            case OscState::Esc:
                if (c == ']') {
                    flushNormal(_oscEscPos);
                    _oscBuffer.clear();
                    _oscState = OscState::InOsc;
                } else {
                    _oscState = OscState::Normal;
                }
                i++;
                break;

            case OscState::InOsc:
                if (c == '\007') {
                    i++;
                    handleOsc(i, false);
                } else if (c == '\033') {
                    _oscState = OscState::OscEscEnd;
                    i++;
                } else {
                    _oscBuffer += c;
                    i++;
                }
                break;

            case OscState::OscEscEnd:
                if (c == '\\') {
                    i++;
                    handleOsc(i, true);
                } else if (c == '\033') {
                    // Another ESC - first one was body data, this one might start ST
                    _oscBuffer += '\033';
                    // Stay in OscEscEnd
                    i++;
                } else {
                    _oscBuffer += '\033';
                    _oscBuffer += c;
                    _oscState = OscState::InOsc;
                    i++;
                }
                break;
            }
        }

        // Flush remaining normal data
        if (_oscState == OscState::Normal) {
            flushNormal(len);
        } else if (_oscState == OscState::Esc) {
            flushNormal(len);
            _oscState = OscState::Normal;
        }
    }

    OscScanner _scanner;
    std::vector<char> _ptyBuffer;
    OscState _oscState = OscState::Normal;
    std::string _oscBuffer;
    size_t _oscEscPos = 0;
    std::string _screenOutput;
    OscHandler _oscHandler;
    uint32_t _oscHandledCount = 0;
    uint32_t _oscForwardedCount = 0;
};

// ---------------------------------------------------------------------------
// Integration: Complete OSC in single write
// ---------------------------------------------------------------------------

suite integration_single_write = [] {
    "complete OSC with BEL in single write"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> received;
        term.setOscHandler([&](const std::string& body) {
            received.push_back(body);
            return body.find("666666;") == 0;  // Handle vendor OSCs
        });

        std::string data = "\033]666666;run -c ygrid\007";
        term.onPtyData(data.data(), data.size());

        expect(received.size() == 1_u);
        expect(received[0] == "666666;run -c ygrid"_b);
        expect(term.oscHandledCount() == 1_u);
    };

    "complete OSC with ST in single write"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> received;
        term.setOscHandler([&](const std::string& body) {
            received.push_back(body);
            return body.find("666666;") == 0;
        });

        std::string data = "\033]666666;update --name test\033\\";
        term.onPtyData(data.data(), data.size());

        expect(received.size() == 1_u);
        expect(received[0] == "666666;update --name test"_b);
        expect(term.oscHandledCount() == 1_u);
    };

    "unhandled OSC forwards to screen"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return false; });

        std::string data = "\033]0;window title\007";
        term.onPtyData(data.data(), data.size());

        expect(term.oscForwardedCount() == 1_u);
        expect(term.screenOutput() == "\033]0;window title\007"_b);
    };
};

// ---------------------------------------------------------------------------
// Integration: OSC split across multiple PTY reads
// ---------------------------------------------------------------------------

suite integration_chunked = [] {
    "OSC header and body in separate reads"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> received;
        term.setOscHandler([&](const std::string& body) {
            received.push_back(body);
            return true;
        });

        // First read: just header
        term.onPtyData("\033]666666;", 9);
        expect(received.empty());  // Should be buffering

        // Second read: body + terminator
        term.onPtyData("payload\033\\", 9);
        expect(received.size() == 1_u);
        expect(received[0] == "666666;payload"_b);
    };

    "OSC split at ESC of ST terminator"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> received;
        term.setOscHandler([&](const std::string& body) {
            received.push_back(body);
            return true;
        });

        // Split right at ESC
        term.onPtyData("\033]666666;data\033", 14);
        expect(received.empty());  // Waiting for \ or other

        term.onPtyData("\\", 1);
        expect(received.size() == 1_u);
        expect(received[0] == "666666;data"_b);
    };

    "OSC split byte by byte"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> received;
        term.setOscHandler([&](const std::string& body) {
            received.push_back(body);
            return true;
        });

        std::string osc = "\033]666666;test\033\\";
        for (size_t i = 0; i < osc.size(); i++) {
            term.onPtyData(&osc[i], 1);
        }

        expect(received.size() == 1_u);
        expect(received[0] == "666666;test"_b);
    };

    "large payload in multiple chunks"_test = [] {
        TerminalSimulator term;
        std::string receivedBody;
        term.setOscHandler([&](const std::string& body) {
            receivedBody = body;
            return true;
        });

        // Build large OSC like ymux would
        std::string header = "\033]666666;run -c ygrid;;";
        std::string payload(30000, 'A');  // ~30KB base64-like
        std::string terminator = "\033\\";

        // Send in 4KB chunks (typical read size)
        std::string full = header + payload + terminator;
        size_t pos = 0;
        while (pos < full.size()) {
            size_t chunk = std::min(size_t(4096), full.size() - pos);
            term.onPtyData(full.data() + pos, chunk);
            pos += chunk;
        }

        expect(term.oscHandledCount() == 1_u);
        expect(receivedBody.size() == header.size() - 2 + payload.size());  // minus \033]
    };
};

// ---------------------------------------------------------------------------
// Integration: Mixed content (normal text + OSC)
// ---------------------------------------------------------------------------

suite integration_mixed = [] {
    "normal text before OSC"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return true; });

        std::string data = "Hello World\033]666666;cmd\007";
        term.onPtyData(data.data(), data.size());

        expect(term.screenOutput() == "Hello World"_b);
        expect(term.oscHandledCount() == 1_u);
    };

    "normal text after OSC"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return true; });

        std::string data = "\033]666666;cmd\007Goodbye";
        term.onPtyData(data.data(), data.size());

        expect(term.screenOutput() == "Goodbye"_b);
        expect(term.oscHandledCount() == 1_u);
    };

    "text-OSC-text-OSC-text"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return true; });

        std::string data = "A\033]1;x\007B\033]2;y\033\\C";
        term.onPtyData(data.data(), data.size());

        expect(term.screenOutput() == "ABC"_b);
        expect(term.oscHandledCount() == 2_u);
    };

    "CSI sequences pass through"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return true; });

        std::string data = "\033[31mRed\033[0m Normal \033]666666;osc\007";
        term.onPtyData(data.data(), data.size());

        expect(term.screenOutput() == "\033[31mRed\033[0m Normal "_b);
        expect(term.oscHandledCount() == 1_u);
    };
};

// ---------------------------------------------------------------------------
// Integration: Ymux-specific scenarios
// ---------------------------------------------------------------------------

suite integration_ymux = [] {
    "ymux create card sequence"_test = [] {
        TerminalSimulator term;
        std::string receivedBody;
        term.setOscHandler([&](const std::string& body) {
            receivedBody = body;
            return body.find("666666;") == 0;
        });

        // Exact format ymux uses
        std::string osc = "\033]666666;run -c ygrid -x 0 -y 0 -w 80 -h 24 -r --name ymux-pane-0;;";
        osc += "WUdSRAEAUBgAAAA=";  // Base64 payload
        osc += "\033\\";

        term.onPtyData(osc.data(), osc.size());

        expect(term.oscHandledCount() == 1_u);
        expect(receivedBody.find("run -c ygrid") != std::string::npos);
        expect(receivedBody.find("--name ymux-pane-0") != std::string::npos);
    };

    "ymux update sequence"_test = [] {
        TerminalSimulator term;
        std::string receivedBody;
        term.setOscHandler([&](const std::string& body) {
            receivedBody = body;
            return body.find("666666;") == 0;
        });

        std::string osc = "\033]666666;update --name ymux-pane-0;;WUdSRAAAAFAY\033\\";
        term.onPtyData(osc.data(), osc.size());

        expect(term.oscHandledCount() == 1_u);
        expect(receivedBody.find("update --name ymux-pane-0") != std::string::npos);
    };

    "ymux sequence split at realistic boundaries"_test = [] {
        TerminalSimulator term;
        int oscCount = 0;
        term.setOscHandler([&](const std::string&) {
            oscCount++;
            return true;
        });

        // Simulate kernel PTY buffer behavior - data may arrive in chunks
        std::string osc = "\033]666666;run -c ygrid -x 0 -y 0 -w 80 -h 24;;";
        osc += std::string(25000, 'X');  // Large base64-ish payload
        osc += "\033\\";

        // Simulate reads of varying sizes (as PTY would deliver)
        std::vector<size_t> chunkSizes = {17, 4096, 8192, 4096, 8192, 1024, 512, 100};
        size_t pos = 0;
        for (size_t sz : chunkSizes) {
            if (pos >= osc.size()) break;
            size_t actual = std::min(sz, osc.size() - pos);
            term.onPtyData(osc.data() + pos, actual);
            pos += actual;
        }
        // Send remainder
        if (pos < osc.size()) {
            term.onPtyData(osc.data() + pos, osc.size() - pos);
        }

        expect(oscCount == 1_i);
    };

    "multiple ymux panes"_test = [] {
        TerminalSimulator term;
        std::vector<std::string> bodies;
        term.setOscHandler([&](const std::string& body) {
            bodies.push_back(body);
            return true;
        });

        // Three panes created
        std::string data;
        data += "\033]666666;run -c ygrid --name pane-0;;AAAA\033\\";
        data += "\033]666666;run -c ygrid --name pane-1;;BBBB\033\\";
        data += "\033]666666;run -c ygrid --name pane-2;;CCCC\033\\";

        term.onPtyData(data.data(), data.size());

        expect(bodies.size() == 3_u);
        expect(bodies[0].find("pane-0") != std::string::npos);
        expect(bodies[1].find("pane-1") != std::string::npos);
        expect(bodies[2].find("pane-2") != std::string::npos);
    };
};

// ---------------------------------------------------------------------------
// Integration: Edge cases that could break things
// ---------------------------------------------------------------------------

suite integration_edge_cases = [] {
    "ESC in OSC body that's not terminator"_test = [] {
        TerminalSimulator term;
        std::string receivedBody;
        term.setOscHandler([&](const std::string& body) {
            receivedBody = body;
            return true;
        });

        // ESC followed by something other than backslash
        std::string osc = "\033]666666;data\033Xmore\033\\";
        term.onPtyData(osc.data(), osc.size());

        expect(term.oscHandledCount() == 1_u);
        // The body should contain the ESC X sequence
        expect(receivedBody.find("\033X") != std::string::npos);
    };

    "multiple ESC before backslash"_test = [] {
        TerminalSimulator term;
        std::string receivedBody;
        term.setOscHandler([&](const std::string& body) {
            receivedBody = body;
            return true;
        });

        // ESC ESC backslash - first ESC is data, second ESC + \ is terminator
        std::string osc = "\033]666666;test\033\033\\";
        term.onPtyData(osc.data(), osc.size());

        expect(term.oscHandledCount() == 1_u);
    };

    "empty PTY reads interspersed"_test = [] {
        TerminalSimulator term;
        term.setOscHandler([](const std::string&) { return true; });

        term.onPtyData("\033]666", 5);
        term.onPtyData("", 0);  // Empty read
        term.onPtyData("666;", 4);
        term.onPtyData("", 0);  // Empty read
        term.onPtyData("cmd\033\\", 6);

        expect(term.oscHandledCount() == 1_u);
    };

    "rapid small writes"_test = [] {
        TerminalSimulator term;
        int count = 0;
        term.setOscHandler([&](const std::string&) {
            count++;
            return true;
        });

        // 100 tiny OSCs
        for (int i = 0; i < 100; i++) {
            std::string osc = "\033]666666;" + std::to_string(i) + "\007";
            // Write in 2-byte chunks
            for (size_t j = 0; j < osc.size(); j += 2) {
                size_t len = std::min(size_t(2), osc.size() - j);
                term.onPtyData(osc.data() + j, len);
            }
        }

        expect(count == 100_i);
    };
};
