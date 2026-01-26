//=============================================================================
// GPUScreen Cell Buffer Tests
//
// Tests for GPUScreen's Cell buffer after processing ANSI sequences
// This tests the actual rendering path, not just libvterm parsing
//=============================================================================

// Include C++ standard headers before boost/ut.hpp (cdb version file conflict)
#include <cstddef>
#include <version>
#include <algorithm>
#include <cstring>

#include <boost/ut.hpp>
#include "yetty/gpu-screen.h"

using namespace boost::ut;
using namespace yetty;

// Helper macro to write string with automatic length calculation
#define WRITE(screen, str) screen->write(str, strlen(str))

// Helper to check style bits
constexpr uint8_t STYLE_BOLD = 0x01;
constexpr uint8_t STYLE_ITALIC = 0x02;
constexpr uint8_t STYLE_UNDERLINE_MASK = 0x0C;
constexpr uint8_t STYLE_STRIKE = 0x10;
constexpr uint8_t STYLE_FONT_TYPE_MASK = 0xE0;

suite gpu_screen_cell_tests = [] {
    "plain text has default colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "A");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u) << "glyph should be set";
        // Default fg is white (240,240,240) and bg is black (0,0,0)
        expect(cell.fgR == 240_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 240_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 240_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "foreground color red (31)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[31mR");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Red: 224,0,0
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "foreground color green (32)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[32mG");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Green: 0,224,0
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "foreground color blue (34)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[34mB");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Blue: 0,0,224
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 224_u) << "fgB=" << cell.fgB;
    };

    "background color red (41)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[41mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.bgR == 224_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "background color green (42)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[42mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 224_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "reverse video swaps fg and bg"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Set red fg, then reverse - fg and bg swap
        // Original: fg=red(224,0,0), bg=default(0,0,0)
        // After reverse: fg=default_bg(0,0,0), bg=red(224,0,0)
        WRITE(screen, "\033[31;7mR");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // After reverse: red moves to bg, default bg (black) moves to fg
        expect(cell.bgR == 224_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "bold attribute sets style bit"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[1mB");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect((cell.style & STYLE_BOLD) != 0_u) << "style=" << cell.style << " should have bold bit";
    };

    "italic attribute sets style bit"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[3mI");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect((cell.style & STYLE_ITALIC) != 0_u) << "style=" << cell.style << " should have italic bit";
    };

    "underline attribute sets style bits"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[4mU");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        uint8_t underline = (cell.style & STYLE_UNDERLINE_MASK) >> 2;
        expect(underline > 0_u) << "underline=" << underline << " should be non-zero";
    };

    "strikethrough attribute sets style bit"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[9mS");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect((cell.style & STYLE_STRIKE) != 0_u) << "style=" << cell.style << " should have strike bit";
    };

    "reset clears all attributes"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[1;3;4;31m");  // bold, italic, underline, red
        WRITE(screen, "\033[mN");  // reset + N
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // After reset, should have default colors and no style bits (except font type)
        expect((cell.style & 0x1F) == 0_u) << "style=" << cell.style << " should have no attr bits after reset";
    };

    "truecolor foreground (38;2;r;g;b)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[38;2;100;150;200mT");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 100_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 150_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 200_u) << "fgB=" << cell.fgB;
    };

    "truecolor background (48;2;r;g;b)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[48;2;50;100;150mB");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.bgR == 50_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 100_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 150_u) << "bgB=" << cell.bgB;
    };

    "256-color foreground (38;5;n)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // 38;5;1 = red (index 1) = 224,0,0
        WRITE(screen, "\033[38;5;1mR");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "combined fg and bg colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Red text on green background
        WRITE(screen, "\033[31;42mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Foreground red (224,0,0)
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
        // Background green (0,224,0)
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 224_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "multiple chars with different colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[31mR\033[32mG\033[34mB\033[mN");

        Cell cellR = screen->getCell(0, 0);
        Cell cellG = screen->getCell(0, 1);
        Cell cellB = screen->getCell(0, 2);
        Cell cellN = screen->getCell(0, 3);

        // R should be red (224,0,0)
        expect(cellR.fgR == 224_u) << "R: fgR=" << cellR.fgR;
        expect(cellR.fgG == 0_u) << "R: fgG=" << cellR.fgG;
        expect(cellR.fgB == 0_u) << "R: fgB=" << cellR.fgB;
        // G should be green (0,224,0)
        expect(cellG.fgR == 0_u) << "G: fgR=" << cellG.fgR;
        expect(cellG.fgG == 224_u) << "G: fgG=" << cellG.fgG;
        expect(cellG.fgB == 0_u) << "G: fgB=" << cellG.fgB;
        // B should be blue (0,0,224)
        expect(cellB.fgR == 0_u) << "B: fgR=" << cellB.fgR;
        expect(cellB.fgG == 0_u) << "B: fgG=" << cellB.fgG;
        expect(cellB.fgB == 224_u) << "B: fgB=" << cellB.fgB;
        // N should be default fg (240,240,240)
        expect(cellN.fgR == 240_u) << "N: fgR=" << cellN.fgR;
        expect(cellN.fgG == 240_u) << "N: fgG=" << cellN.fgG;
        expect(cellN.fgB == 240_u) << "N: fgB=" << cellN.fgB;
    };

    //=========================================================================
    // Reverse video tests
    //=========================================================================

    "reverse video with default colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Just reverse, no explicit colors
        // Default: fg=240,240,240 bg=0,0,0
        // After reverse: fg=0,0,0 bg=240,240,240
        WRITE(screen, "\033[7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 240_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 240_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 240_u) << "bgB=" << cell.bgB;
    };

    "reverse video with green fg"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Green fg + reverse
        // Before reverse: fg=0,224,0 bg=0,0,0
        // After reverse: fg=0,0,0 bg=0,224,0
        WRITE(screen, "\033[32;7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 224_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "reverse video with blue bg"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Blue bg + reverse
        // Before reverse: fg=240,240,240 bg=0,0,224
        // After reverse: fg=0,0,224 bg=240,240,240
        WRITE(screen, "\033[44;7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 224_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 240_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 240_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 240_u) << "bgB=" << cell.bgB;
    };

    "reverse video with both fg and bg colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Red fg + Green bg + reverse
        // Before reverse: fg=224,0,0 bg=0,224,0
        // After reverse: fg=0,224,0 bg=224,0,0
        WRITE(screen, "\033[31;42;7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // fg becomes green (was bg)
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
        // bg becomes red (was fg)
        expect(cell.bgR == 224_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "reverse video with truecolor"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Truecolor fg=100,150,200 + reverse
        // Before reverse: fg=100,150,200 bg=0,0,0
        // After reverse: fg=0,0,0 bg=100,150,200
        WRITE(screen, "\033[38;2;100;150;200;7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 100_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 150_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 200_u) << "bgB=" << cell.bgB;
    };

    "reverse video with truecolor bg"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Truecolor bg=50,100,150 + reverse
        // Before reverse: fg=240,240,240 bg=50,100,150
        // After reverse: fg=50,100,150 bg=240,240,240
        WRITE(screen, "\033[48;2;50;100;150;7mX");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 50_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 100_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 150_u) << "fgB=" << cell.fgB;
        expect(cell.bgR == 240_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 240_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 240_u) << "bgB=" << cell.bgB;
    };

    "reverse off restores normal colors"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Set red, reverse, then turn off reverse with SGR 27
        WRITE(screen, "\033[31;7mR\033[27mN");
        Cell cellR = screen->getCell(0, 0);
        Cell cellN = screen->getCell(0, 1);

        // R should have reversed colors (fg=black, bg=red)
        expect(cellR.fgR == 0_u) << "R fgR=" << cellR.fgR;
        expect(cellR.bgR == 224_u) << "R bgR=" << cellR.bgR;

        // N should have red fg again (reverse off)
        expect(cellN.fgR == 224_u) << "N fgR=" << cellN.fgR;
        expect(cellN.fgG == 0_u) << "N fgG=" << cellN.fgG;
        expect(cellN.fgB == 0_u) << "N fgB=" << cellN.fgB;
        expect(cellN.bgR == 0_u) << "N bgR=" << cellN.bgR;
        expect(cellN.bgG == 0_u) << "N bgG=" << cellN.bgG;
        expect(cellN.bgB == 0_u) << "N bgB=" << cellN.bgB;
    };

    //=========================================================================
    // 256-color palette tests
    //=========================================================================

    "256-color foreground index 2 (green)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[38;5;2mG");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Index 2 = green = 0,224,0
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "256-color background index 4 (blue)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[48;5;4mB");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Index 4 = blue = 0,0,224
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 224_u) << "bgB=" << cell.bgB;
    };

    "256-color bright red (index 9)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[38;5;9mR");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Index 9 = bright red (vterm palette: 255,64,64)
        expect(cell.fgR == 255_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 64_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 64_u) << "fgB=" << cell.fgB;
    };

    "256-color grayscale (index 244)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Index 244 is in grayscale range (232-255)
        // vterm grayscale formula produces 133 for index 244
        WRITE(screen, "\033[38;5;244mG");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        expect(cell.fgR == 133_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 133_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 133_u) << "fgB=" << cell.fgB;
    };

    //=========================================================================
    // Bright colors (90-97 fg, 100-107 bg)
    //=========================================================================

    "bright red foreground (91)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[91mR");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Bright red (vterm palette: 255,64,64)
        expect(cell.fgR == 255_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 64_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 64_u) << "fgB=" << cell.fgB;
    };

    "bright green foreground (92)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[92mG");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Bright green (vterm palette: 64,255,64)
        expect(cell.fgR == 64_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 255_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 64_u) << "fgB=" << cell.fgB;
    };

    "bright blue background (104)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        WRITE(screen, "\033[104mB");
        Cell cell = screen->getCell(0, 0);

        expect(cell.glyph != 0_u);
        // Bright blue (vterm palette: 64,64,255)
        expect(cell.bgR == 64_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 64_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 255_u) << "bgB=" << cell.bgB;
    };

    //=========================================================================
    // Default color reset tests
    //=========================================================================

    "reset fg to default (39)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Set red, then reset fg to default
        WRITE(screen, "\033[31mR\033[39mD");
        Cell cellR = screen->getCell(0, 0);
        Cell cellD = screen->getCell(0, 1);

        // R should be red
        expect(cellR.fgR == 224_u) << "R fgR=" << cellR.fgR;
        // D should be default (240,240,240)
        expect(cellD.fgR == 240_u) << "D fgR=" << cellD.fgR;
        expect(cellD.fgG == 240_u) << "D fgG=" << cellD.fgG;
        expect(cellD.fgB == 240_u) << "D fgB=" << cellD.fgB;
    };

    "reset bg to default (49)"_test = [] {
        auto result = GPUScreen::createForTest(80, 25);
        expect(result.has_value() >> fatal);
        auto screen = *result;

        // Set green bg, then reset bg to default
        WRITE(screen, "\033[42mG\033[49mD");
        Cell cellG = screen->getCell(0, 0);
        Cell cellD = screen->getCell(0, 1);

        // G should have green bg
        expect(cellG.bgG == 224_u) << "G bgG=" << cellG.bgG;
        // D should be default bg (0,0,0)
        expect(cellD.bgR == 0_u) << "D bgR=" << cellD.bgR;
        expect(cellD.bgG == 0_u) << "D bgG=" << cellD.bgG;
        expect(cellD.bgB == 0_u) << "D bgB=" << cellD.bgB;
    };
};
