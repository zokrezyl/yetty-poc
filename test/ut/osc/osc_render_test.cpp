//=============================================================================
// OSC Render Tests
//
// Tests that multiple OSC sequences each trigger screen update correctly.
// This simulates the yplayer scenario where rapid image updates are sent.
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/osc-scanner.h>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

using namespace boost::ut;
using namespace yetty;

// Simulates terminal's processPtyData with screenUpdateEvent dispatch
class RenderTerminalSimulator {
public:
    using OscHandler = std::function<bool(const std::string& body)>;
    using RenderCallback = std::function<void()>;

    void setOscHandler(OscHandler handler) {
        _oscHandler = std::move(handler);
    }

    void setRenderCallback(RenderCallback cb) {
        _renderCallback = std::move(cb);
    }

    uint32_t oscHandledCount() const { return _oscHandledCount; }
    uint32_t renderCallCount() const { return _renderCallCount; }

    // Simulates processPtyData - parses OSC and triggers render after each
    void processPtyData(const char* data, size_t len) {
        size_t i = 0;
        size_t normalStart = 0;

        auto handleOsc = [&](size_t terminatorEnd, bool isST) {
            bool handled = false;
            if (_oscHandler) {
                handled = _oscHandler(_oscBuffer);
            }
            if (handled) {
                _oscHandledCount++;
                // Simulate screenUpdateEvent dispatch - THIS IS THE KEY
                if (_renderCallback) {
                    _renderCallback();
                    _renderCallCount++;
                }
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
                    _oscBuffer += '\033';
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
    }

private:
    enum class OscState { Normal, Esc, InOsc, OscEscEnd };

    OscHandler _oscHandler;
    RenderCallback _renderCallback;
    OscState _oscState = OscState::Normal;
    std::string _oscBuffer;
    size_t _oscEscPos = 0;
    uint32_t _oscHandledCount = 0;
    uint32_t _renderCallCount = 0;
};

// ---------------------------------------------------------------------------
// Tests for multiple OSC sequences triggering renders
// ---------------------------------------------------------------------------

suite render_trigger_tests = [] {
    "each OSC triggers one render"_test = [] {
        RenderTerminalSimulator term;
        term.setOscHandler([](const std::string& body) {
            return body.find("666666;") == 0;  // Handle vendor OSCs
        });
        term.setRenderCallback([]() {
            // Simulates render happening
        });

        // Send 5 OSC sequences in one buffer (like rapid yplayer frames)
        std::string data;
        data += "\033]666666;update --name test;payload1\033\\";
        data += "\033]666666;update --name test;payload2\033\\";
        data += "\033]666666;update --name test;payload3\033\\";
        data += "\033]666666;update --name test;payload4\033\\";
        data += "\033]666666;update --name test;payload5\033\\";

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 5_u) << "Expected 5 OSCs handled, got " << term.oscHandledCount();
        expect(term.renderCallCount() == 5_u) << "Expected 5 renders, got " << term.renderCallCount();
    };

    "render callback doesn't break parsing"_test = [] {
        RenderTerminalSimulator term;
        int parseCount = 0;

        term.setOscHandler([&](const std::string& body) {
            parseCount++;
            return body.find("666666;") == 0;
        });

        int renderCount = 0;
        term.setRenderCallback([&]() {
            renderCount++;
            // Simulate slow render (doesn't actually block in test)
        });

        // 10 OSCs
        std::string data;
        for (int i = 0; i < 10; i++) {
            data += "\033]666666;frame" + std::to_string(i) + "\033\\";
        }

        term.processPtyData(data.data(), data.size());

        expect(parseCount == 10_i) << "Should parse all 10 OSCs";
        expect(renderCount == 10_i) << "Should render 10 times";
    };

    "large payloads with renders"_test = [] {
        RenderTerminalSimulator term;
        std::vector<size_t> payloadSizes;

        term.setOscHandler([&](const std::string& body) {
            // Extract payload size
            auto pos = body.find(";;");
            if (pos != std::string::npos) {
                payloadSizes.push_back(body.size() - pos - 2);
            }
            return body.find("666666;") == 0;
        });

        term.setRenderCallback([]() {});

        // Simulate 3 large image frames (like yplayer)
        std::string data;
        std::string payload1(100000, 'A');  // 100KB
        std::string payload2(100000, 'B');
        std::string payload3(100000, 'C');

        data += "\033]666666;update --name img;;" + payload1 + "\033\\";
        data += "\033]666666;update --name img;;" + payload2 + "\033\\";
        data += "\033]666666;update --name img;;" + payload3 + "\033\\";

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 3_u);
        expect(term.renderCallCount() == 3_u);
        expect(payloadSizes.size() == 3_u);
        expect(payloadSizes[0] == 100000_u);
    };

    "interleaved normal text and OSCs"_test = [] {
        RenderTerminalSimulator term;
        int oscCount = 0;

        term.setOscHandler([&](const std::string&) {
            oscCount++;
            return true;
        });
        term.setRenderCallback([]() {});

        std::string data = "Hello\033]666666;cmd1\007World\033]666666;cmd2\033\\End";
        term.processPtyData(data.data(), data.size());

        expect(oscCount == 2_i) << "Should handle 2 OSCs with text between";
        expect(term.renderCallCount() == 2_u);
    };

    "100 rapid OSCs all trigger renders"_test = [] {
        RenderTerminalSimulator term;

        term.setOscHandler([](const std::string& body) {
            return body.find("666666;") == 0;
        });
        term.setRenderCallback([]() {});

        std::string data;
        for (int i = 0; i < 100; i++) {
            data += "\033]666666;frame" + std::to_string(i) + "\033\\";
        }

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 100_u) << "All 100 OSCs should be handled";
        expect(term.renderCallCount() == 100_u) << "All 100 should trigger render";
    };
};
