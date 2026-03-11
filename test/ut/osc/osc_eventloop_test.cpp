//=============================================================================
// OSC EventLoop Integration Tests
//
// Tests that multiple OSC sequences correctly dispatch screenUpdateEvent
// through the REAL event loop, not mocks.
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/base/event-loop.h>
#include <yetty/base/event.h>
#include <cstring>
#include <string>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::base;

// Real event listener that counts ScreenUpdate events
class ScreenUpdateCounter : public EventListener,
                            public std::enable_shared_from_this<ScreenUpdateCounter> {
public:
    int count = 0;
    std::vector<int64_t> timestamps;

    Result<bool> onEvent(const Event& event) override {
        if (event.type == Event::Type::ScreenUpdate) {
            count++;
            auto now = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count();
            timestamps.push_back(ms);
        }
        return Ok(true);  // consumed
    }
};

// Simulates terminal's processPtyData dispatching to REAL event loop
class RealEventLoopTerminal {
public:
    RealEventLoopTerminal() {
        auto loopResult = EventLoop::instance();
        if (loopResult) {
            _loop = *loopResult;
        }
    }

    void processPtyData(const char* data, size_t len) {
        size_t i = 0;

        auto handleOsc = [&](size_t terminatorEnd, bool isST) {
            (void)terminatorEnd;
            (void)isST;

            // Check if this is a vendor OSC we handle
            if (_oscBuffer.find("666666;") == 0) {
                _oscHandledCount++;

                // THIS IS THE KEY - dispatch through REAL event loop
                if (_loop) {
                    _loop->dispatch(Event::screenUpdateEvent());
                }
            }
            _oscBuffer.clear();
            _oscState = OscState::Normal;
        };

        while (i < len) {
            char c = data[i];

            switch (_oscState) {
            case OscState::Normal:
                if (c == '\033') {
                    _oscState = OscState::Esc;
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

    uint32_t oscHandledCount() const { return _oscHandledCount; }

private:
    enum class OscState { Normal, Esc, InOsc, OscEscEnd };

    EventLoop::Ptr _loop;
    OscState _oscState = OscState::Normal;
    std::string _oscBuffer;
    uint32_t _oscHandledCount = 0;
};

// ---------------------------------------------------------------------------
// Tests using REAL event loop
// ---------------------------------------------------------------------------

suite eventloop_tests = [] {
    "dispatch fires for each OSC through real event loop"_test = [] {
        // Get the singleton event loop
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value()) << "Failed to get event loop";
        auto loop = *loopResult;

        // Create listener
        auto counter = std::make_shared<ScreenUpdateCounter>();
        loop->registerListener(Event::Type::ScreenUpdate, counter);

        // Create terminal that dispatches through real loop
        RealEventLoopTerminal term;

        // Send 5 OSC sequences
        std::string data;
        data += "\033]666666;update --name test;payload1\033\\";
        data += "\033]666666;update --name test;payload2\033\\";
        data += "\033]666666;update --name test;payload3\033\\";
        data += "\033]666666;update --name test;payload4\033\\";
        data += "\033]666666;update --name test;payload5\033\\";

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 5_u) << "Should handle 5 OSCs";
        expect(counter->count == 5_i) << "Event loop should receive 5 ScreenUpdate events, got " << counter->count;

        // Cleanup
        loop->deregisterListener(Event::Type::ScreenUpdate, counter);
    };

    "100 rapid OSCs all dispatch through event loop"_test = [] {
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        auto counter = std::make_shared<ScreenUpdateCounter>();
        loop->registerListener(Event::Type::ScreenUpdate, counter);

        RealEventLoopTerminal term;

        std::string data;
        for (int i = 0; i < 100; i++) {
            data += "\033]666666;frame" + std::to_string(i) + "\033\\";
        }

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 100_u);
        expect(counter->count == 100_i) << "All 100 OSCs should trigger ScreenUpdate, got " << counter->count;

        loop->deregisterListener(Event::Type::ScreenUpdate, counter);
    };

    "large payloads dispatch correctly"_test = [] {
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        auto counter = std::make_shared<ScreenUpdateCounter>();
        loop->registerListener(Event::Type::ScreenUpdate, counter);

        RealEventLoopTerminal term;

        // 3 large payloads like video frames
        std::string payload(100000, 'X');  // 100KB each
        std::string data;
        data += "\033]666666;update;;" + payload + "\033\\";
        data += "\033]666666;update;;" + payload + "\033\\";
        data += "\033]666666;update;;" + payload + "\033\\";

        term.processPtyData(data.data(), data.size());

        expect(term.oscHandledCount() == 3_u);
        expect(counter->count == 3_i) << "3 large OSCs should dispatch, got " << counter->count;

        loop->deregisterListener(Event::Type::ScreenUpdate, counter);
    };

    "dispatch is synchronous - listener called immediately"_test = [] {
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        auto counter = std::make_shared<ScreenUpdateCounter>();
        loop->registerListener(Event::Type::ScreenUpdate, counter);

        RealEventLoopTerminal term;

        // First OSC
        std::string osc1 = "\033]666666;frame1\033\\";
        term.processPtyData(osc1.data(), osc1.size());

        // Check immediately - should already be 1
        expect(counter->count == 1_i) << "Dispatch should be synchronous";

        // Second OSC
        std::string osc2 = "\033]666666;frame2\033\\";
        term.processPtyData(osc2.data(), osc2.size());

        expect(counter->count == 2_i) << "Should be 2 now";

        loop->deregisterListener(Event::Type::ScreenUpdate, counter);
    };

    "dispatch continues even if listener does work"_test = [] {
        auto loopResult = EventLoop::instance();
        expect(loopResult.has_value());
        auto loop = *loopResult;

        // Listener that does "work" (simulated)
        class WorkingListener : public EventListener,
                                public std::enable_shared_from_this<WorkingListener> {
        public:
            int count = 0;

            Result<bool> onEvent(const Event& event) override {
                if (event.type == Event::Type::ScreenUpdate) {
                    count++;
                    // Simulate some work
                    volatile int sum = 0;
                    for (int i = 0; i < 1000; i++) {
                        sum += i;
                    }
                    (void)sum;
                }
                return Ok(true);
            }
        };

        auto worker = std::make_shared<WorkingListener>();
        loop->registerListener(Event::Type::ScreenUpdate, worker);

        RealEventLoopTerminal term;

        std::string data;
        for (int i = 0; i < 50; i++) {
            data += "\033]666666;frame" + std::to_string(i) + "\033\\";
        }

        term.processPtyData(data.data(), data.size());

        expect(worker->count == 50_i) << "All 50 should complete despite work, got " << worker->count;

        loop->deregisterListener(Event::Type::ScreenUpdate, worker);
    };
};
