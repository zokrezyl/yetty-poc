//=============================================================================
// YEcho Parser Tests
//
// Tests for the yecho text parser with glyphs and blocks
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>
#include "yecho-parser.h"

using namespace boost::ut;
using namespace yetty;

suite yecho_parser_basic_tests = [] {
    "parser parses plain text"_test = [] {
        YEchoParser parser;
        auto spans = parser.parse("hello world");
        expect(spans.size() == 1_u);
        expect(spans[0].type == YEchoSpan::Type::Text);
        expect(spans[0].text == "hello world"_b);
    };

    "parser parses glyph"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("spinner", 0x101000);
        YEchoParser parser(&registry);
        
        auto spans = parser.parse("@spinner");
        
        expect(spans.size() == 1_u);
        expect(spans[0].type == YEchoSpan::Type::Glyph);
        expect(spans[0].glyphName == "spinner"_b);
    };

    "parser parses text with glyph"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("spinner", 0x101000);
        YEchoParser parser(&registry);
        
        auto spans = parser.parse("hello @spinner world");
        
        expect(spans.size() == 3_u);
        expect(spans[0].type == YEchoSpan::Type::Text);
        expect(spans[0].text == "hello "_b);
        expect(spans[1].type == YEchoSpan::Type::Glyph);
        expect(spans[1].glyphName == "spinner"_b);
        expect(spans[2].type == YEchoSpan::Type::Text);
        expect(spans[2].text == " world"_b);
    };

    "parser handles escape sequences"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("user\\@example.com");
        
        expect(spans.size() == 1_u);
        expect(spans[0].type == YEchoSpan::Type::Text);
        expect(spans[0].text == "user@example.com"_b);
    };

    "parser handles escaped brace"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("\\{not a block\\}");
        
        expect(spans.size() == 1_u);
        expect(spans[0].text == "{not a block}"_b);
    };
};

suite yecho_block_tests = [] {
    "parser parses simple block"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{color=#ff0000: red text}");
        
        expect(spans.size() == 1_u);
        expect(spans[0].type == YEchoSpan::Type::Block);
        expect(spans[0].content == "red text"_b);
        expect(spans[0].attrs.size() == 1_u);
        expect(spans[0].attrs[0].key == "color"_b);
        expect(spans[0].attrs[0].value == "#ff0000"_b);
    };

    "parser parses block with semicolon-separated attrs"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{card=plot; w=80; h=20: sin(x)}");
        
        expect(spans.size() == 1_u);
        expect(spans[0].type == YEchoSpan::Type::Block);
        expect(spans[0].content == "sin(x)"_b);
        expect(spans[0].attrs.size() == 3_u);
        
        // Check attrs
        bool hasCard = false, hasW = false, hasH = false;
        for (const auto& attr : spans[0].attrs) {
            if (attr.key == "card" && attr.value == "plot") hasCard = true;
            if (attr.key == "w" && attr.value == "80") hasW = true;
            if (attr.key == "h" && attr.value == "20") hasH = true;
        }
        expect(hasCard && hasW && hasH);
    };

    "parser parses block with @ prefix attrs"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{card=plot; @view=-1..1,-1..1: f=sin(x)}");
        
        expect(spans.size() == 1_u);
        expect(spans[0].attrs.size() == 2_u);
        
        bool hasView = false;
        for (const auto& attr : spans[0].attrs) {
            if (attr.key == "@view" && attr.value == "-1..1,-1..1") hasView = true;
        }
        expect(hasView);
    };

    "parser handles content with semicolons"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{card=plot; w=80: f1=sin(x); @f1.color=#FF0000}");
        
        expect(spans.size() == 1_u);
        expect(spans[0].content == "f1=sin(x); @f1.color=#FF0000"_b);
    };

    "parser handles quotes in attrs"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{card=ytext; @title='Hello; World': content}");
        
        expect(spans.size() == 1_u);
        // The semicolon in quotes should NOT split the attr
        bool hasTitleWithSemicolon = false;
        for (const auto& attr : spans[0].attrs) {
            if (attr.key == "@title" && attr.value == "'Hello; World'") {
                hasTitleWithSemicolon = true;
            }
        }
        expect(hasTitleWithSemicolon);
    };

    "parser handles mixed text and blocks"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("before {color=#00ff00: green} after");
        
        expect(spans.size() == 3_u);
        expect(spans[0].type == YEchoSpan::Type::Text);
        expect(spans[0].text == "before "_b);
        expect(spans[1].type == YEchoSpan::Type::Block);
        expect(spans[1].content == "green"_b);
        expect(spans[2].type == YEchoSpan::Type::Text);
        expect(spans[2].text == " after"_b);
    };
};

suite yecho_osc_generation_tests = [] {
    "generateOSC creates plot card OSC"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{card=plot; w=80; h=20: f1=sin(x)}"));
        
        // Should contain OSC 666666
        expect(output.find("\033]666666;") != std::string::npos);
        // Should contain create -p plot
        expect(output.find("create -p plot") != std::string::npos);
        // Should contain dimensions
        expect(output.find("-w 80") != std::string::npos);
        expect(output.find("-h 20") != std::string::npos);
        // Should contain expression
        expect(output.find("--expr") != std::string::npos);
    };

    "generateOSC includes view attribute"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{card=plot; w=60; @view=-3.14..3.14,-1..1: sin(x)}"));
        
        expect(output.find("--view") != std::string::npos);
    };
};

suite yecho_ansi_generation_tests = [] {
    "toTerminal generates ANSI for color"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{color=#ff0000: red}"));
        
        // Should contain ANSI color escape
        expect(output.find("\033[38;2;255;0;0m") != std::string::npos);
        // Should reset at end
        expect(output.find("\033[0m") != std::string::npos);
    };

    "toTerminal generates ANSI for bg"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{bg=#00ff00: green bg}"));
        
        // Should contain ANSI background escape (48;2;r;g;b)
        expect(output.find("\033[48;2;0;255;0m") != std::string::npos);
    };

    "toTerminal generates ANSI for style bold"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{style=bold: bold text}"));
        
        // Bold is ANSI code 1
        expect(output.find("\033[1m") != std::string::npos);
    };

    "toTerminal generates ANSI for style italic"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{style=italic: italic text}"));
        
        // Italic is ANSI code 3
        expect(output.find("\033[3m") != std::string::npos);
    };

    "toTerminal generates ANSI for combined styles"_test = [] {
        YEchoParser parser;
        
        std::string output = parser.toTerminal(parser.parse("{style=bold|italic: text}"));
        
        // Should have both 1 and 3
        expect(output.find("1") != std::string::npos);
        expect(output.find("3") != std::string::npos);
    };
};

suite yecho_glyph_registry_tests = [] {
    "registry returns codepoint for registered glyph"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("test", 1052738);  // 0x101042
        
        expect(registry.getCodepoint("test") == 1052738_u);  // 0x101042
    };

    "registry returns 0 for unknown glyph"_test = [] {
        GlyphRegistry registry;
        expect(registry.getCodepoint("unknown") == 0_u);
    };

    "registry hasGlyph works"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("spinner", 1052672);  // 0x101000
        
        expect(registry.hasGlyph("spinner"));
        expect(!registry.hasGlyph("notexist"));
    };

    "registry getNames returns all names"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("alpha", 1052672);   // 0x101000
        registry.registerGlyph("beta", 1052673);    // 0x101001
        
        auto names = registry.getNames();
        expect(names.size() == 2_u);
    };
};

suite yecho_edge_cases = [] {
    "parser handles empty input"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("");
        
        expect(spans.empty());
    };

    "parser handles unclosed block gracefully"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{color=#ff0000: no close");
        
        // Should have error
        expect(!parser.errors().empty());
    };

    "parser handles block without colon"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("{no colon here}");
        
        // Should have error
        expect(!parser.errors().empty());
    };

    "parser handles multiple glyphs"_test = [] {
        GlyphRegistry registry;
        registry.registerGlyph("a", 0x101000);
        registry.registerGlyph("b", 0x101001);
        YEchoParser parser(&registry);
        
        auto spans = parser.parse("@a @b @a");
        
        expect(spans.size() == 5_u);  // glyph space glyph space glyph
        expect(spans[0].type == YEchoSpan::Type::Glyph);
        expect(spans[2].type == YEchoSpan::Type::Glyph);
        expect(spans[4].type == YEchoSpan::Type::Glyph);
    };

    "parser handles @ not followed by identifier"_test = [] {
        YEchoParser parser;
        
        auto spans = parser.parse("test@ ");
        
        // @ should be treated as literal
        expect(spans.size() == 1_u);
        expect(spans[0].text == "test@ "_b);
    };
};
