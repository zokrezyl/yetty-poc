//=============================================================================
// YMux Tests
//
// Tests for:
//   - Pane: vterm integration, YGRD payload building, damage tracking
//   - Multiplexer: base64 encoding, OSC sequence generation
//   - Integration: spawn process, capture output, validate OSC
//=============================================================================

#include <cstddef>
#include <cstring>
#include <version>
#include <thread>
#include <chrono>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

#include <boost/ut.hpp>
#include "ymux.h"
#include "pane.h"

using namespace boost::ut;
using namespace ymux;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Decode YGRD header from raw payload bytes
struct YgrdHeader {
    uint32_t magic;
    uint16_t flags;
    uint8_t cols;
    uint8_t rows;
};

static YgrdHeader readHeader(const std::vector<uint8_t>& payload) {
    YgrdHeader h{};
    if (payload.size() >= 8) {
        std::memcpy(&h.magic, payload.data(), 4);
        std::memcpy(&h.flags, payload.data() + 4, 2);
        h.cols = payload[6];
        h.rows = payload[7];
    }
    return h;
}

// Read a GridCell at index from a full payload (after 8-byte header)
static GridCell readCellFromFull(const std::vector<uint8_t>& payload, size_t idx) {
    GridCell gc{};
    size_t off = 8 + idx * sizeof(GridCell);
    if (off + sizeof(GridCell) <= payload.size()) {
        std::memcpy(&gc, payload.data() + off, sizeof(GridCell));
    }
    return gc;
}

// Read partial update entry count
static uint32_t readPartialCount(const std::vector<uint8_t>& payload) {
    uint32_t count = 0;
    if (payload.size() >= 12) {
        std::memcpy(&count, payload.data() + 8, 4);
    }
    return count;
}

// Simple base64 decode for test validation
static int b64val(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static std::vector<uint8_t> base64Decode(const std::string& encoded) {
    std::vector<uint8_t> out;
    out.reserve(encoded.size() * 3 / 4);
    int val = 0, valb = -8;
    for (unsigned char c : encoded) {
        int v = b64val(c);
        if (v < 0) break;
        val = (val << 6) + v;
        valb += 6;
        if (valb >= 0) {
            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

// Drain PTY fd into pane's vterm with timeout (ms)
static void drainPty(Pane& pane, int timeoutMs) {
    char buf[4096];
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    while (std::chrono::steady_clock::now() < deadline) {
        struct pollfd pfd = {pane.fd(), POLLIN, 0};
        int remaining = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
            deadline - std::chrono::steady_clock::now()).count());
        if (remaining <= 0) break;
        int ret = poll(&pfd, 1, std::min(remaining, 50));
        if (ret > 0 && (pfd.revents & POLLIN)) {
            ssize_t n = read(pane.fd(), buf, sizeof(buf));
            if (n > 0) {
                pane.feedInput(buf, n);
            } else {
                break;
            }
        } else if (ret == 0) {
            // No data for 50ms, check if we've been running long enough
            continue;
        } else {
            break;
        }
    }
}

// Extract the base64-encoded payload from an OSC sequence
// Format: \033]666666;...;;PAYLOAD\033\.
static std::string extractOscPayload(const std::string& osc) {
    // Find ";;" separator
    auto sep = osc.find(";;");
    if (sep == std::string::npos) return {};
    size_t start = sep + 2;
    // Find ST (\033\)
    auto end = osc.find("\033\\", start);
    if (end == std::string::npos) return {};
    return osc.substr(start, end - start);
}

// Extract the command part between first ; and ;;
static std::string extractOscCommand(const std::string& osc) {
    // Skip \033]666666;
    auto first = osc.find(';');
    if (first == std::string::npos) return {};
    auto sep = osc.find(";;", first);
    if (sep == std::string::npos) return {};
    return osc.substr(first + 1, sep - first - 1);
}

// ---------------------------------------------------------------------------
// Base64 tests
// ---------------------------------------------------------------------------

suite base64_tests = [] {
    "base64 encodes empty"_test = [] {
        std::vector<uint8_t> data;
        auto result = Multiplexer::base64Encode(data);
        expect(result.empty());
    };

    "base64 encodes single byte"_test = [] {
        std::vector<uint8_t> data = {0x41}; // 'A'
        auto result = Multiplexer::base64Encode(data);
        expect(result == "QQ=="_b);
    };

    "base64 encodes two bytes"_test = [] {
        std::vector<uint8_t> data = {0x41, 0x42}; // "AB"
        auto result = Multiplexer::base64Encode(data);
        expect(result == "QUI="_b);
    };

    "base64 encodes three bytes"_test = [] {
        std::vector<uint8_t> data = {0x41, 0x42, 0x43}; // "ABC"
        auto result = Multiplexer::base64Encode(data);
        expect(result == "QUJD"_b);
    };

    "base64 roundtrip"_test = [] {
        // Encode some bytes, decode, compare
        std::vector<uint8_t> data = {0x59, 0x47, 0x52, 0x44, 0x01, 0x00, 0x0a, 0x03};
        auto encoded = Multiplexer::base64Encode(data);
        auto decoded = base64Decode(encoded);
        expect(decoded.size() == data.size());
        expect(std::memcmp(decoded.data(), data.data(), data.size()) == 0_i);
    };

    "base64 roundtrip large"_test = [] {
        // Simulate a full payload for 4x2 grid
        std::vector<uint8_t> data(8 + 4 * 2 * 12, 0);
        data[0] = 0x44; data[1] = 0x52; data[2] = 0x47; data[3] = 0x59; // YGRD
        data[4] = 0x01; data[5] = 0x00; // flags=FULL
        data[6] = 4; data[7] = 2; // 4x2
        for (size_t i = 8; i < data.size(); i++) {
            data[i] = static_cast<uint8_t>(i & 0xFF);
        }
        auto encoded = Multiplexer::base64Encode(data);
        auto decoded = base64Decode(encoded);
        expect(decoded.size() == data.size());
        expect(std::memcmp(decoded.data(), data.data(), data.size()) == 0_i);
    };
};

// ---------------------------------------------------------------------------
// Pane payload tests (vterm-based, no PTY needed for feeding)
// ---------------------------------------------------------------------------

suite pane_payload_tests = [] {
    "pane creates with correct dimensions"_test = [] {
        auto pane = Pane::create(10, 5, "/bin/true");
        expect(pane != nullptr);
        expect(pane->cols() == 10_u);
        expect(pane->rows() == 5_u);
    };

    "full payload has YGRD magic and correct header"_test = [] {
        auto pane = Pane::create(10, 5, "/bin/true");
        expect(pane != nullptr);

        auto payload = pane->buildFullPayload();
        auto hdr = readHeader(payload);

        expect(hdr.magic == YGRD_MAGIC);
        expect(hdr.flags == YGRD_FLAG_FULL);
        expect(hdr.cols == 10_u);
        expect(hdr.rows == 5_u);
    };

    "full payload has correct size"_test = [] {
        auto pane = Pane::create(10, 5, "/bin/true");
        expect(pane != nullptr);

        auto payload = pane->buildFullPayload();
        size_t expected = 8 + 10 * 5 * sizeof(GridCell);
        expect(payload.size() == expected);
    };

    "initial cells are spaces with default colors"_test = [] {
        auto pane = Pane::create(4, 2, "/bin/true");
        expect(pane != nullptr);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>(' '));
        expect(cell.alpha == 255_u);
    };

    "feedInput writes text to vterm and produces correct payload"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Feed "Hello" at row 0, col 0
        pane->feedInput("Hello", 5);

        auto payload = pane->buildFullPayload();
        auto hdr = readHeader(payload);
        expect(hdr.cols == 10_u);
        expect(hdr.rows == 3_u);

        // Check first 5 cells spell "Hello"
        const char* text = "Hello";
        for (int i = 0; i < 5; i++) {
            auto cell = readCellFromFull(payload, i);
            expect(cell.codepoint == static_cast<uint32_t>(text[i]));
        }

        // Cell 5 should be space
        auto cell5 = readCellFromFull(payload, 5);
        expect(cell5.codepoint == static_cast<uint32_t>(' '));
    };

    "feedInput with newline moves to next row"_test = [] {
        auto pane = Pane::create(10, 5, "/bin/true");
        expect(pane != nullptr);

        pane->feedInput("AB\r\nCD", 6);

        auto payload = pane->buildFullPayload();

        // Row 0: A, B, space...
        auto cellA = readCellFromFull(payload, 0);
        auto cellB = readCellFromFull(payload, 1);
        expect(cellA.codepoint == static_cast<uint32_t>('A'));
        expect(cellB.codepoint == static_cast<uint32_t>('B'));

        // Row 1: C, D, space...
        auto cellC = readCellFromFull(payload, 10); // row 1, col 0
        auto cellD = readCellFromFull(payload, 11); // row 1, col 1
        expect(cellC.codepoint == static_cast<uint32_t>('C'));
        expect(cellD.codepoint == static_cast<uint32_t>('D'));
    };

    "feedInput with ANSI color sets fg color"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Red foreground: ESC[31m then 'R'
        pane->feedInput("\033[31mR", 6);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>('R'));
        // Red in 256-color: should have high red, low green/blue
        expect(cell.fgR > 100_u);
        expect(cell.fgG < cell.fgR);
    };

    "feedInput with bold sets style bit"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Bold: ESC[1m then 'B'
        pane->feedInput("\033[1mB", 5);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>('B'));
        expect((cell.style & ATTR_BOLD) != 0_u);
    };

    "feedInput with italic sets style bit"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Italic: ESC[3m then 'I'
        pane->feedInput("\033[3mI", 5);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>('I'));
        expect((cell.style & ATTR_ITALIC) != 0_u);
    };

    "feedInput with underline sets style bits"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Underline: ESC[4m then 'U'
        pane->feedInput("\033[4mU", 5);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>('U'));
        uint8_t ulType = (cell.style >> ATTR_UNDERLINE_SHIFT) & 0x03;
        expect(ulType > 0_u);
    };

    "feedInput with strikethrough sets style bit"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Strikethrough: ESC[9m then 'S'
        pane->feedInput("\033[9mS", 5);

        auto payload = pane->buildFullPayload();
        auto cell = readCellFromFull(payload, 0);

        expect(cell.codepoint == static_cast<uint32_t>('S'));
        expect((cell.style & ATTR_STRIKETHROUGH) != 0_u);
    };
};

// ---------------------------------------------------------------------------
// Damage tracking tests
// ---------------------------------------------------------------------------

suite pane_damage_tests = [] {
    "no damage initially after payload build"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // vterm_screen_reset triggers initial damage, clear it
        pane->buildDamagePayload();

        expect(!pane->hasDamage());
    };

    "feedInput triggers damage"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Clear initial damage
        pane->buildDamagePayload();

        pane->feedInput("X", 1);
        expect(pane->hasDamage());
    };

    "damage payload is partial when few cells changed"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Clear initial damage
        pane->buildDamagePayload();

        // Feed a single char — damages 1 cell
        pane->feedInput("X", 1);

        auto payload = pane->buildDamagePayload();
        auto hdr = readHeader(payload);

        expect(hdr.magic == YGRD_MAGIC);
        // flags should NOT have FULL bit
        expect((hdr.flags & YGRD_FLAG_FULL) == 0_u);

        // Count should be small
        uint32_t count = readPartialCount(payload);
        expect(count >= 1_u);
        expect(count < 15_u); // cursor move may add a few extra

        // Total size: 8 (header) + 4 (count) + count * 14 (row+col+GridCell)
        size_t expected = 8 + 4 + count * (2 + sizeof(GridCell));
        expect(payload.size() == expected);
    };

    "damage payload has correct partial entry format"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Clear initial damage
        pane->buildDamagePayload();

        // Write 'Z' at current cursor (row 0, col 0 after reset)
        pane->feedInput("Z", 1);

        auto payload = pane->buildDamagePayload();
        uint32_t count = readPartialCount(payload);
        expect(count >= 1_u);

        // First entry starts at offset 12
        uint8_t row = payload[12];
        uint8_t col = payload[13];
        expect(row == 0_u);
        expect(col == 0_u);

        // Read the GridCell for this entry
        GridCell gc{};
        std::memcpy(&gc, payload.data() + 14, sizeof(GridCell));
        expect(gc.codepoint == static_cast<uint32_t>('Z'));
    };

    "large damage falls back to full payload"_test = [] {
        auto pane = Pane::create(4, 2, "/bin/true");
        expect(pane != nullptr);

        // Clear initial damage
        pane->buildDamagePayload();

        // Fill most of the 4x2=8 cells (>50% = >4)
        pane->feedInput("ABCD\r\nEFGH", 10);

        auto payload = pane->buildDamagePayload();
        auto hdr = readHeader(payload);

        // Should be FULL since >50% damaged
        expect(hdr.magic == YGRD_MAGIC);
        expect((hdr.flags & YGRD_FLAG_FULL) != 0_u);
    };

    "buildDamagePayload clears damage"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // Clear initial damage
        pane->buildDamagePayload();

        pane->feedInput("X", 1);
        expect(pane->hasDamage());

        pane->buildDamagePayload();
        expect(!pane->hasDamage());
    };
};

// ---------------------------------------------------------------------------
// OSC sequence format tests
// ---------------------------------------------------------------------------

suite osc_format_tests = [] {
    "create OSC starts with ESC]666666"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test-pane-0");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        expect(osc.find("\033]666666;") == 0u);
    };

    "create OSC contains run -c ygrid"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test-pane-0");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        expect(osc.find("run -c ygrid") != std::string::npos);
    };

    "create OSC contains correct position and size"_test = [] {
        auto pane = Pane::create(20, 10, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test-pane-1");

        auto osc = Multiplexer::buildCreateOsc(*pane, 5, 3, 20, 10);

        expect(osc.find("-x 5") != std::string::npos);
        expect(osc.find("-y 3") != std::string::npos);
        expect(osc.find("-w 20") != std::string::npos);
        expect(osc.find("-h 10") != std::string::npos);
    };

    "create OSC contains --name with space separator"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("mypane");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        // Must be "--name mypane" NOT "--name=mypane"
        expect(osc.find("--name mypane") != std::string::npos);
        expect(osc.find("--name=") == std::string::npos);
    };

    "create OSC contains -r flag"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        expect(osc.find(" -r") != std::string::npos);
    };

    "create OSC ends with ST"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        // Should end with \033\;
        expect(osc.size() >= 2u);
        expect(osc[osc.size() - 2] == '\033');
        expect(osc[osc.size() - 1] == '\\');
    };

    "create OSC has ;; payload separator"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);

        expect(osc.find(";;") != std::string::npos);
    };

    "create OSC payload decodes to valid YGRD"_test = [] {
        auto pane = Pane::create(8, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        auto osc = Multiplexer::buildCreateOsc(*pane, 0, 0, 8, 3);
        auto b64 = extractOscPayload(osc);
        expect(!b64.empty());

        auto decoded = base64Decode(b64);
        auto hdr = readHeader(decoded);
        expect(hdr.magic == YGRD_MAGIC);
        expect((hdr.flags & YGRD_FLAG_FULL) != 0_u);
        expect(hdr.cols == 8_u);
        expect(hdr.rows == 3_u);
    };

    "update OSC uses update --name"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("mypane");

        // Build a dummy payload
        std::vector<uint8_t> payload = {0x59, 0x47, 0x52, 0x44, 0x00, 0x00, 0x0a, 0x03};
        auto osc = Multiplexer::buildUpdateOsc(*pane, payload);

        expect(osc.find("update --name mypane") != std::string::npos);
        // Must NOT use --id
        expect(osc.find("--id") == std::string::npos);
    };

    "update OSC with empty payload returns empty"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        std::vector<uint8_t> empty;
        auto osc = Multiplexer::buildUpdateOsc(*pane, empty);
        expect(osc.empty());
    };

    "update OSC payload roundtrips correctly"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("test");

        // Clear initial damage, feed data, get damage payload
        pane->buildDamagePayload();
        pane->feedInput("Hello", 5);
        auto damagePayload = pane->buildDamagePayload();

        auto osc = Multiplexer::buildUpdateOsc(*pane, damagePayload);
        auto b64 = extractOscPayload(osc);
        auto decoded = base64Decode(b64);

        // Decoded should match original payload
        expect(decoded.size() == damagePayload.size());
        expect(std::memcmp(decoded.data(), damagePayload.data(), damagePayload.size()) == 0_i);
    };
};

// ---------------------------------------------------------------------------
// Integration: spawn process, read output, validate
// ---------------------------------------------------------------------------

suite integration_tests = [] {
    "pane with echo captures output in cells"_test = [] {
        // Create a pane running a command that produces known output
        auto pane = Pane::create(20, 5, "/bin/sh");
        expect(pane != nullptr);
        pane->setCardName("int-test-0");

        // Send a command to the shell and drain PTY output
        const char* cmd = "echo TESTOUT\n";
        pane->sendKey(cmd, std::strlen(cmd));
        drainPty(*pane, 500);

        // Build full payload
        auto payload = pane->buildFullPayload();
        auto hdr = readHeader(payload);
        expect(hdr.magic == YGRD_MAGIC);
        expect(hdr.cols == 20_u);
        expect(hdr.rows == 5_u);

        // Search all cells for "TESTOUT" substring
        bool found = false;
        size_t totalCells = 20 * 5;
        for (size_t start = 0; start + 7 <= totalCells && !found; start++) {
            bool match = true;
            const char* target = "TESTOUT";
            for (int i = 0; i < 7; i++) {
                auto cell = readCellFromFull(payload, start + i);
                if (cell.codepoint != static_cast<uint32_t>(target[i])) {
                    match = false;
                    break;
                }
            }
            if (match) found = true;
        }
        expect(found);
    };

    "pane isAlive returns true for running process"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/sh");
        expect(pane != nullptr);
        expect(pane->isAlive());
    };

    "pane isAlive returns false after process exits"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        // /bin/true exits immediately, wait a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        expect(!pane->isAlive());
    };

    "pane cardName set and get"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        pane->setCardName("ymux-pane-42");
        expect(pane->cardName() == "ymux-pane-42"_b);
    };

    "pane resize changes dimensions"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);

        pane->resize(20, 8);
        expect(pane->cols() == 20_u);
        expect(pane->rows() == 8_u);

        auto payload = pane->buildFullPayload();
        auto hdr = readHeader(payload);
        expect(hdr.cols == 20_u);
        expect(hdr.rows == 8_u);

        size_t expected = 8 + 20 * 8 * sizeof(GridCell);
        expect(payload.size() == expected);
    };

    "full create-update cycle produces valid OSC"_test = [] {
        auto pane = Pane::create(10, 3, "/bin/true");
        expect(pane != nullptr);
        pane->setCardName("ymux-pane-0");

        // 1. Create OSC
        auto createOsc = Multiplexer::buildCreateOsc(*pane, 0, 0, 10, 3);
        auto cmd = extractOscCommand(createOsc);
        expect(cmd.find("run -c ygrid") != std::string::npos);
        expect(cmd.find("--name ymux-pane-0") != std::string::npos);

        // 2. Feed data
        pane->buildDamagePayload(); // clear initial
        pane->feedInput("Hi!", 3);

        // 3. Damage payload
        auto damagePayload = pane->buildDamagePayload();
        expect(!damagePayload.empty());
        auto hdr = readHeader(damagePayload);
        expect(hdr.magic == YGRD_MAGIC);

        // 4. Update OSC
        auto updateOsc = Multiplexer::buildUpdateOsc(*pane, damagePayload);
        expect(updateOsc.find("update --name ymux-pane-0") != std::string::npos);

        // 5. Decode and verify
        auto b64 = extractOscPayload(updateOsc);
        auto decoded = base64Decode(b64);
        expect(decoded.size() == damagePayload.size());
    };
};
