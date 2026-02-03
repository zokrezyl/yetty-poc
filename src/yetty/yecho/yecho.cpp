#include "yecho-parser.h"
#include <args.hxx>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace yetty;

// Generate bash completion script
static void printBashCompletion() {
    std::cout << R"COMPLETION(# yecho bash completion
# Add to ~/.bashrc: eval "$(yecho --bash-completion)"

_yecho_completions() {
    local cur="${COMP_WORDS[COMP_CWORD]}"
    local completions
    
    completions=$(yecho --complete "$cur" 2>/dev/null)
    
    COMPREPLY=($(compgen -W "$completions" -- "$cur"))
}

complete -F _yecho_completions yecho
)COMPLETION";
}

// Generate zsh completion script  
static void printZshCompletion() {
    std::cout << R"COMPLETION(#compdef yecho
# yecho zsh completion
# Add to ~/.zshrc: eval "$(yecho --zsh-completion)"

_yecho() {
    local -a completions
    local cur="${words[CURRENT]}"
    
    completions=("${(@f)$(yecho --complete "$cur" 2>/dev/null)}")
    
    if [[ "$cur" == -* ]]; then
        _describe 'options' completions
    elif [[ "$cur" == @* ]]; then
        _describe 'glyphs' completions
    else
        compadd -a completions
    fi
}

compdef _yecho yecho
)COMPLETION";
}

// Generate completions for current word
static void printCompletions(const std::string& word, GlyphRegistry& registry) {
    // Option completions
    if (word.empty() || word[0] == '-') {
        const char* options[] = {
            "--help", "--list", "--shader-dir=", "-h", "-l", "-n", nullptr
        };
        for (const char** opt = options; *opt; ++opt) {
            if (word.empty() || std::string(*opt).find(word) == 0) {
                std::cout << *opt << "\n";
            }
        }
        return;
    }
    
    // Glyph completions (@name)
    if (word[0] == '@') {
        std::string prefix = word.substr(1);  // Remove @
        for (const auto& name : registry.getNames()) {
            if (prefix.empty() || name.find(prefix) == 0) {
                std::cout << "@" << name << "\n";
            }
        }
        return;
    }
    
    // Attribute completions (inside {})
    if (word.find('=') == std::string::npos) {
        const char* attrs[] = {
            "color=", "bg=", "style=", "font-size=", nullptr
        };
        for (const char** attr = attrs; *attr; ++attr) {
            if (std::string(*attr).find(word) == 0) {
                std::cout << *attr << "\n";
            }
        }
    }
    
    // Style value completions
    if (word.find("style=") == 0) {
        std::string prefix = word.substr(6);
        const char* styles[] = {"bold", "italic", "underline", "bold|italic", "bold|underline", nullptr};
        for (const char** s = styles; *s; ++s) {
            if (prefix.empty() || std::string(*s).find(prefix) == 0) {
                std::cout << "style=" << *s << "\n";
            }
        }
    }
}

int main(int argc, const char** argv) {
    args::ArgumentParser parser(
        "yecho - echo text with embedded glyphs",
        "Grammar:\n"
        "  @name           Shader glyph (e.g., @spinner, @heart)\n"
        "  {attrs: text}   Styled block (e.g., {style=bold: important})\n"
        "  \\@ \\{ \\}       Escaped literals\n"
        "\n"
        "Attributes:\n"
        "  color=#ff0000   Text color (hex)\n"
        "  bg=#000000      Background color (hex)\n"
        "  style=bold      Style (bold, italic, underline, bold|italic)\n"
        "  font-size=32    Font size (requires ytext card)\n"
        "\n"
        "Examples:\n"
        "  yecho \"Loading @spinner please wait\"\n"
        "  yecho \"{color=#ff0000: Error:} something failed\"\n"
        "  yecho \"@heart {style=bold: Welcome} @heart\"\n"
    );
    
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
    args::Flag listGlyphs(parser, "list", "List available glyphs", {'l', "list"});
    args::Flag noNewline(parser, "no-newline", "Don't print trailing newline", {'n'});
    args::Flag bashCompletion(parser, "bash-completion", 
        "Output bash completion script", {"bash-completion"});
    args::Flag zshCompletion(parser, "zsh-completion",
        "Output zsh completion script", {"zsh-completion"});
    args::ValueFlag<std::string> completeWord(parser, "word",
        "Generate completions for word (internal)", {"complete"});
    args::ValueFlag<std::string> shaderDir(parser, "path",
        "Load additional glyphs from shader directory", {"shader-dir"});
    args::PositionalList<std::string> textArgs(parser, "text",
        "Text to echo (multiple args joined with spaces)");
    
    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << "\n";
        std::cerr << parser;
        return 1;
    }
    
    // Create glyph registry (loads built-in glyphs automatically)
    GlyphRegistry registry;
    
    // Optionally load additional glyphs from directory
    if (shaderDir) {
        registry.loadFromDirectory(args::get(shaderDir));
    }
    
    // Shell completion modes
    if (bashCompletion) {
        printBashCompletion();
        return 0;
    }
    
    if (zshCompletion) {
        printZshCompletion();
        return 0;
    }
    
    if (completeWord) {
        printCompletions(args::get(completeWord), registry);
        return 0;
    }
    
    // List glyphs mode
    if (listGlyphs) {
        auto names = registry.getNames();
        if (names.empty()) {
            std::cerr << "No glyphs available\n";
            return 1;
        }
        
        std::cout << "Available glyphs (" << names.size() << "):\n";
        for (const auto& name : names) {
            uint32_t cp = registry.getCodepoint(name);
            std::cout << "  @" << name << " (U+" << std::hex << cp << std::dec << ")\n";
        }
        return 0;
    }
    
    // Get input text
    std::string input;
    auto texts = args::get(textArgs);
    if (texts.empty()) {
        // Read from stdin
        std::stringstream ss;
        ss << std::cin.rdbuf();
        input = ss.str();
    } else {
        // Join positional args with spaces
        for (size_t i = 0; i < texts.size(); ++i) {
            if (i > 0) input += ' ';
            input += texts[i];
        }
    }
    
    if (input.empty()) {
        return 0;
    }
    
    // Parse and convert
    YEchoParser echoParser(&registry);
    auto root = echoParser.parse(input);
    
    // Report warnings for unknown glyphs
    for (const auto& err : echoParser.errors()) {
        std::cerr << "warning: " << err << "\n";
    }
    
    // Check if ytext card is needed (font-size specified)
    if (echoParser.needsYtext(root)) {
        std::cerr << "warning: font-size requires ytext card (not yet implemented)\n";
        // TODO: emit ytext OSC sequence
    }
    
    // Output with ANSI escapes for colors/styles
    std::string output = echoParser.toTerminal(root);
    std::cout << output;
    
    if (!noNewline) {
        std::cout << "\n";
    }
    
    return 0;
}
