//=============================================================================
// Terminal Pen/Color Tests
//
// Tests for SGR (Select Graphic Rendition) escape sequences
// Based on libvterm's 64screen_pen.test
//=============================================================================

// Include C++ standard headers before boost/ut.hpp (cdb version file conflict)
#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>
#include "harness/terminal_harness.h"

using namespace boost::ut;
using namespace yetty::test;

suite terminal_pen_tests = [] {
    "plain text has default colors"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("A");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('A'));
        expect(cell.fgDefault == true) << "fg should be default";
        expect(cell.bgDefault == true) << "bg should be default";
        // Default colors: fg=240,240,240 bg=0,0,0
        expect(cell.fgR == 240_u);
        expect(cell.fgG == 240_u);
        expect(cell.fgB == 240_u);
        expect(cell.bgR == 0_u);
        expect(cell.bgG == 0_u);
        expect(cell.bgB == 0_u);
    };

    "bold attribute"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[1mB");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('B'));
        expect(cell.bold == true);
    };

    "italic attribute"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[3mI");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('I'));
        expect(cell.italic == true);
    };

    "underline attribute"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[4mU");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('U'));
        expect(cell.underline == true);
    };

    "combined bold+italic"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[1m\033[3mC");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('C'));
        expect(cell.bold == true);
        expect(cell.italic == true);
    };

    "reset clears attributes"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[1m\033[3m\033[4m");  // bold, italic, underline
        term.feed("\033[mE");  // reset + E

        auto cell = term.getCell(0, 0);
        expect(cell.glyph == uint32_t('E'));
        expect(cell.bold == false);
        expect(cell.italic == false);
        expect(cell.underline == false);
    };

    "foreground color red (31)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[31mR\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('R'));
        expect(cell.fgDefault == false);
        // Red should be index 1 -> rgb(224,0,0) per libvterm
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "foreground color green (32)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[32mG\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('G'));
        expect(cell.fgDefault == false);
        // Green should be index 2 -> rgb(0,224,0)
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "foreground color blue (34)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[34mB\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('B'));
        expect(cell.fgDefault == false);
        // Blue should be index 4 -> rgb(0,0,224)
        expect(cell.fgR == 0_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 0_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 224_u) << "fgB=" << cell.fgB;
    };

    "foreground color yellow (33)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[33mY\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('Y'));
        expect(cell.fgDefault == false);
        // Yellow should be index 3 -> rgb(224,224,0)
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 0_u) << "fgB=" << cell.fgB;
    };

    "background color red (41)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[41mX\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('X'));
        expect(cell.bgDefault == false);
        expect(cell.bgR == 224_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 0_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "background color green (42)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[42mH\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('H'));
        expect(cell.bgDefault == false);
        expect(cell.bgR == 0_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 224_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 0_u) << "bgB=" << cell.bgB;
    };

    "bright foreground colors (90-97)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[91mR");  // Bright red
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('R'));
        expect(cell.fgDefault == false);
        // Bright red is index 9 -> rgb(255,64,64)
        expect(cell.fgR > 200_u) << "bright red fgR=" << cell.fgR;
    };

    "256-color mode foreground (38;5;n)"_test = [] {
        TerminalHarness term(80, 25);

        // 38;5;1 = red (index 1)
        term.feed("\033[38;5;1mR\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('R'));
        expect(cell.fgDefault == false);
        expect(cell.fgR == 224_u);
        expect(cell.fgG == 0_u);
        expect(cell.fgB == 0_u);
    };

    "256-color grayscale (38;5;232-255)"_test = [] {
        TerminalHarness term(80, 25);

        // 38;5;250 is a light gray (grayscale range 232-255)
        term.feed("\033[38;5;250mG\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('G'));
        expect(cell.fgDefault == false);
        // Grayscale 250 should be near-white
        expect(cell.fgR > 180_u) << "grayscale fgR=" << cell.fgR;
    };

    "truecolor foreground (38;2;r;g;b)"_test = [] {
        TerminalHarness term(80, 25);

        // Set exact RGB values
        term.feed("\033[38;2;100;150;200mT\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('T'));
        expect(cell.fgDefault == false);
        expect(cell.fgR == 100_u) << "fgR=" << cell.fgR;
        expect(cell.fgG == 150_u) << "fgG=" << cell.fgG;
        expect(cell.fgB == 200_u) << "fgB=" << cell.fgB;
    };

    "truecolor background (48;2;r;g;b)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[48;2;50;100;150mB\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('B'));
        expect(cell.bgDefault == false);
        expect(cell.bgR == 50_u) << "bgR=" << cell.bgR;
        expect(cell.bgG == 100_u) << "bgG=" << cell.bgG;
        expect(cell.bgB == 150_u) << "bgB=" << cell.bgB;
    };

    "reset foreground to default (39)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[31mR");  // Red
        term.feed("\033[39mD");  // Reset fg to default

        auto cell = term.getCell(0, 1);
        expect(cell.glyph == uint32_t('D'));
        expect(cell.fgDefault == true);
    };

    "reset background to default (49)"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[41mR");  // Red bg
        term.feed("\033[49mD");  // Reset bg to default

        auto cell = term.getCell(0, 1);
        expect(cell.glyph == uint32_t('D'));
        expect(cell.bgDefault == true);
    };

    "combined fg and bg colors"_test = [] {
        TerminalHarness term(80, 25);

        // Red text on green background
        term.feed("\033[31;42mX\033[m");
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('X'));
        expect(cell.fgR == 224_u);  // Red fg
        expect(cell.fgG == 0_u);
        expect(cell.bgR == 0_u);    // Green bg
        expect(cell.bgG == 224_u);
    };

    "erase line sets colors"_test = [] {
        TerminalHarness term(80, 25);

        // Set colors and erase line
        term.feed("\033[H\033[33;44m\033[K");  // Yellow on blue, erase line

        auto cell = term.getCell(0, 0);
        // Erased cells should have the current colors
        expect(cell.fgR == 224_u) << "fgR=" << cell.fgR;  // Yellow
        expect(cell.fgG == 224_u) << "fgG=" << cell.fgG;
        expect(cell.bgB == 224_u) << "bgB=" << cell.bgB;  // Blue
    };

    "reverse video swaps colors"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[7mR");  // Reverse video
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('R'));
        expect(cell.reverse == true);
    };

    "blink attribute"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[5mB");  // Blink
        auto cell = term.getCell(0, 0);

        expect(cell.glyph == uint32_t('B'));
        expect(cell.blink == true);
    };

    "multiple sequential color changes"_test = [] {
        TerminalHarness term(80, 25);

        term.feed("\033[31mA");  // Red
        term.feed("\033[32mB");  // Green
        term.feed("\033[34mC");  // Blue
        term.feed("\033[mD");    // Default

        auto cellA = term.getCell(0, 0);
        auto cellB = term.getCell(0, 1);
        auto cellC = term.getCell(0, 2);
        auto cellD = term.getCell(0, 3);

        expect(cellA.fgR == 224_u);  // Red
        expect(cellB.fgG == 224_u);  // Green
        expect(cellC.fgB == 224_u);  // Blue
        expect(cellD.fgDefault == true);
    };
};
