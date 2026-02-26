//=============================================================================
// OscScanner Tests
//
// Tests for the OSC sequence detection state machine used to buffer large
// OSC payloads before sending to vterm.
//=============================================================================

#include <boost/ut.hpp>
#include <yetty/osc-scanner.h>
#include <string>
#include <vector>
#include <chrono>

using namespace boost::ut;
using namespace yetty;

// ---------------------------------------------------------------------------
// Basic state transitions
// ---------------------------------------------------------------------------

suite scanner_basic_tests = [] {
    "initial state is Normal"_test = [] {
        OscScanner scanner;
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
    };

    "reset returns to Normal"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]", 2);
        expect(scanner.isInOsc());
        scanner.reset();
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
    };

    "ESC alone goes to Esc state"_test = [] {
        OscScanner scanner;
        scanner.scan("\033", 1);
        expect(scanner.state() == OscScanner::State::Esc);
        expect(!scanner.isInOsc());
    };

    "ESC followed by non-] returns to Normal"_test = [] {
        OscScanner scanner;
        scanner.scan("\033[", 2);  // CSI sequence start
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
    };

    "ESC ] enters InOsc"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]", 2);
        expect(scanner.state() == OscScanner::State::InOsc);
        expect(scanner.isInOsc());
    };

    "BEL terminates OSC"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]0;title\007", 10);
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "ST (ESC \\) terminates OSC"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]0;title\033\\", 11);
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "ESC in OSC body not followed by \\ stays in InOsc"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]data\033x", 9);
        expect(scanner.state() == OscScanner::State::InOsc);
        expect(scanner.isInOsc());
    };

    "OscEscEnd state when ESC seen in body"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]data\033", 7);
        expect(scanner.state() == OscScanner::State::OscEscEnd);
        expect(scanner.isInOsc());
    };
};

// ---------------------------------------------------------------------------
// Multi-chunk scanning (simulates data arriving in pieces)
// ---------------------------------------------------------------------------

suite scanner_chunked_tests = [] {
    "OSC split: ESC in one chunk, ] in next"_test = [] {
        OscScanner scanner;
        scanner.scan("\033", 1);
        expect(scanner.state() == OscScanner::State::Esc);

        scanner.scan("]", 1);
        expect(scanner.state() == OscScanner::State::InOsc);
        expect(scanner.isInOsc());
    };

    "OSC split: body and BEL in separate chunks"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]666666;data", 13);
        expect(scanner.isInOsc());

        scanner.scan("\007", 1);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "OSC split: body, ESC, and \\ in three chunks"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]666666;payload", 16);
        expect(scanner.isInOsc());

        scanner.scan("\033", 1);
        expect(scanner.state() == OscScanner::State::OscEscEnd);
        expect(scanner.isInOsc());

        scanner.scan("\\", 1);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "large payload in multiple chunks"_test = [] {
        OscScanner scanner;

        // Start OSC
        scanner.scan("\033]666666;", 9);
        expect(scanner.isInOsc());

        // Send 10 chunks of 1000 bytes each
        std::string chunk(1000, 'A');
        for (int i = 0; i < 10; i++) {
            scanner.scan(chunk.data(), chunk.size());
            expect(scanner.isInOsc());
        }

        // Terminate
        scanner.scan("\033\\", 2);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "multiple complete OSCs in one chunk"_test = [] {
        OscScanner scanner;
        // Two OSCs: one with BEL, one with ST
        std::string data = "\033]0;title1\007\033]0;title2\033\\";
        scanner.scan(data.data(), data.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 2_u);
    };

    "incomplete OSC at end of chunk"_test = [] {
        OscScanner scanner;
        // Complete OSC followed by incomplete one
        std::string data = "\033]0;done\007\033]1;incompl";
        scanner.scan(data.data(), data.size());
        expect(scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };
};

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

suite scanner_edge_cases = [] {
    "empty input"_test = [] {
        OscScanner scanner;
        scanner.scan("", 0);
        expect(scanner.state() == OscScanner::State::Normal);
    };

    "nullptr with zero length"_test = [] {
        OscScanner scanner;
        scanner.scan(nullptr, 0);
        expect(scanner.state() == OscScanner::State::Normal);
    };

    "normal text without ESC"_test = [] {
        OscScanner scanner;
        scanner.scan("Hello, World!", 13);
        expect(scanner.state() == OscScanner::State::Normal);
        expect(!scanner.isInOsc());
    };

    "ESC at very end of chunk"_test = [] {
        OscScanner scanner;
        scanner.scan("text\033", 5);
        expect(scanner.state() == OscScanner::State::Esc);
    };

    "multiple ESCs before ]"_test = [] {
        OscScanner scanner;
        scanner.scan("\033\033\033]", 4);
        expect(scanner.state() == OscScanner::State::InOsc);
    };

    "OSC with empty body and BEL"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]\007", 3);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "OSC with empty body and ST"_test = [] {
        OscScanner scanner;
        scanner.scan("\033]\033\\", 4);
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "binary data in OSC body"_test = [] {
        OscScanner scanner;
        // Simulates base64 encoded binary data
        std::string osc = "\033]666666;";
        osc += std::string(100, '\x00');  // Null bytes
        osc += std::string(100, '\xFF');  // High bytes
        osc += "\033\\";
        scanner.scan(osc.data(), osc.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "ESC ] ESC ] nesting"_test = [] {
        OscScanner scanner;
        // Start OSC, then see another ESC ] - should NOT nest
        scanner.scan("\033]data\033]more\007", 15);
        // The inner \033] is just data, BEL terminates
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "CSI sequence does not trigger OSC"_test = [] {
        OscScanner scanner;
        scanner.scan("\033[31m", 5);  // Red color CSI
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 0_u);
    };

    "interleaved normal and OSC"_test = [] {
        OscScanner scanner;
        std::string data = "Hello\033]0;title\007World\033]1;test\033\\End";
        scanner.scan(data.data(), data.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 2_u);
    };
};

// ---------------------------------------------------------------------------
// Ymux-specific scenarios
// ---------------------------------------------------------------------------

suite scanner_ymux_tests = [] {
    "ymux create OSC format"_test = [] {
        OscScanner scanner;
        // Simulates ymux create command
        std::string osc = "\033]666666;run -c ygrid -x 0 -y 0 -w 80 -h 24 -r --name ymux-pane-0;;";
        // Add base64 payload (simulated)
        osc += "WUdSRAEACgMAAA==";  // Small base64 payload
        osc += "\033\\";

        scanner.scan(osc.data(), osc.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "ymux update OSC format"_test = [] {
        OscScanner scanner;
        std::string osc = "\033]666666;update --name ymux-pane-0;;";
        osc += "WUdSRAAAAAoD";  // Base64 payload
        osc += "\033\\";

        scanner.scan(osc.data(), osc.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "large ymux payload (80x24 grid)"_test = [] {
        OscScanner scanner;
        std::string osc = "\033]666666;run -c ygrid -x 0 -y 0 -w 80 -h 24;;";
        // 80x24x12 bytes = 23040 bytes -> ~30720 base64 chars
        osc += std::string(30720, 'A');  // Simulated base64
        osc += "\033\\";

        scanner.scan(osc.data(), osc.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };

    "ymux payload split across reads"_test = [] {
        OscScanner scanner;

        // First read: header
        std::string part1 = "\033]666666;run -c ygrid;;";
        scanner.scan(part1.data(), part1.size());
        expect(scanner.isInOsc());

        // Second read: partial base64
        std::string part2(10000, 'B');
        scanner.scan(part2.data(), part2.size());
        expect(scanner.isInOsc());

        // Third read: more base64
        std::string part3(10000, 'C');
        scanner.scan(part3.data(), part3.size());
        expect(scanner.isInOsc());

        // Fourth read: rest + terminator
        std::string part4(10000, 'D');
        part4 += "\033\\";
        scanner.scan(part4.data(), part4.size());
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
    };
};

// ---------------------------------------------------------------------------
// Performance tests
// ---------------------------------------------------------------------------

suite scanner_performance_tests = [] {
    "scan 1MB of normal text"_test = [] {
        OscScanner scanner;
        std::string data(1024 * 1024, 'X');

        auto start = std::chrono::high_resolution_clock::now();
        scanner.scan(data.data(), data.size());
        auto end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        expect(!scanner.isInOsc());
        expect(ms < 100_i);  // Should be well under 100ms
    };

    "scan 1MB with many small OSCs"_test = [] {
        OscScanner scanner;
        std::string data;
        // Create 10000 small OSC sequences
        for (int i = 0; i < 10000; i++) {
            data += "\033]0;title" + std::to_string(i) + "\007";
        }

        auto start = std::chrono::high_resolution_clock::now();
        scanner.scan(data.data(), data.size());
        auto end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 10000_u);
        expect(ms < 200_i);  // Should complete quickly
    };

    "scan in 1-byte chunks (worst case)"_test = [] {
        OscScanner scanner;
        std::string osc = "\033]666666;some payload data here\033\\";

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < osc.size(); i++) {
            scanner.scan(&osc[i], 1);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        expect(!scanner.isInOsc());
        expect(scanner.completedCount() == 1_u);
        expect(us < 1000_i);  // Should be well under 1ms
    };
};

// ---------------------------------------------------------------------------
// State persistence tests
// ---------------------------------------------------------------------------

suite scanner_state_persistence = [] {
    "state persists across scans"_test = [] {
        OscScanner scanner;

        // Scan partial OSC
        scanner.scan("\033]666", 5);
        auto state1 = scanner.state();
        expect(scanner.isInOsc());

        // Scan more
        scanner.scan("666;data", 8);
        auto state2 = scanner.state();
        expect(scanner.isInOsc());
        expect(state1 == state2);

        // Complete it
        scanner.scan("\007", 1);
        expect(!scanner.isInOsc());
    };

    "completed count accumulates"_test = [] {
        OscScanner scanner;

        scanner.scan("\033]a\007", 4);
        expect(scanner.completedCount() == 1_u);

        scanner.scan("\033]b\007", 4);
        expect(scanner.completedCount() == 2_u);

        scanner.scan("\033]c\033\\", 5);
        expect(scanner.completedCount() == 3_u);

        scanner.resetCompletedCount();
        expect(scanner.completedCount() == 0_u);
    };
};
