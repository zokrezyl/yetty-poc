#include "highlight.h"
#include "grammars.h"

#include <tree_sitter/api.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ycat::ts {

// ──────────────────────────────────────────────────────────────────────────────
// ANSI theme — capture name → SGR escape code
// ──────────────────────────────────────────────────────────────────────────────

static const char* RESET = "\033[0m";

static const std::unordered_map<std::string_view, const char*>& theme() {
    static const std::unordered_map<std::string_view, const char*> t = {
        {"keyword",              "\033[38;5;176m"},
        {"keyword.return",       "\033[1;38;5;176m"},
        {"keyword.function",     "\033[38;5;176m"},
        {"keyword.operator",     "\033[38;5;176m"},
        {"keyword.import",       "\033[38;5;176m"},
        {"keyword.type",         "\033[38;5;176m"},
        {"keyword.modifier",     "\033[38;5;176m"},
        {"keyword.repeat",       "\033[38;5;176m"},
        {"keyword.conditional",  "\033[38;5;176m"},
        {"keyword.exception",    "\033[38;5;176m"},
        {"keyword.directive",    "\033[38;5;176m"},

        {"string",               "\033[38;5;114m"},
        {"string.special",       "\033[38;5;114m"},
        {"string.escape",        "\033[38;5;209m"},

        {"number",               "\033[38;5;209m"},
        {"number.float",         "\033[38;5;209m"},
        {"float",                "\033[38;5;209m"},

        {"comment",              "\033[38;5;245m"},
        {"comment.documentation","\033[38;5;245m"},

        {"function",             "\033[38;5;75m"},
        {"function.call",        "\033[38;5;75m"},
        {"function.builtin",     "\033[38;5;75m"},
        {"function.method",      "\033[38;5;75m"},
        {"function.method.call", "\033[38;5;75m"},
        {"function.macro",       "\033[1;38;5;75m"},
        {"method",               "\033[38;5;75m"},
        {"method.call",          "\033[38;5;75m"},

        {"type",                 "\033[38;5;186m"},
        {"type.builtin",         "\033[38;5;186m"},
        {"type.definition",      "\033[38;5;186m"},

        {"variable",             "\033[38;5;252m"},
        {"variable.builtin",     "\033[38;5;209m"},
        {"variable.parameter",   "\033[38;5;252m"},
        {"variable.member",      "\033[38;5;117m"},

        {"property",             "\033[38;5;117m"},
        {"field",                "\033[38;5;117m"},
        {"attribute",            "\033[38;5;186m"},

        {"constant",             "\033[38;5;209m"},
        {"constant.builtin",     "\033[38;5;209m"},
        {"boolean",              "\033[38;5;209m"},

        {"operator",             "\033[38;5;252m"},

        {"punctuation",          "\033[38;5;248m"},
        {"punctuation.bracket",  "\033[38;5;248m"},
        {"punctuation.delimiter","\033[38;5;248m"},
        {"punctuation.special",  "\033[38;5;248m"},

        {"include",              "\033[38;5;176m"},
        {"label",                "\033[38;5;176m"},
        {"namespace",            "\033[38;5;186m"},
        {"module",               "\033[38;5;186m"},

        {"constructor",          "\033[38;5;186m"},
        {"tag",                  "\033[38;5;75m"},
        {"tag.attribute",        "\033[38;5;186m"},
        {"tag.delimiter",        "\033[38;5;248m"},
    };
    return t;
}

// Hierarchical lookup: "keyword.return" → "keyword" → nullptr
static const char* lookupColor(std::string_view captureName) {
    auto& t = theme();

    // Strip leading @ if present
    if (!captureName.empty() && captureName[0] == '@') {
        captureName = captureName.substr(1);
    }

    // Try exact match first
    auto it = t.find(captureName);
    if (it != t.end()) return it->second;

    // Try parent: "keyword.return" → "keyword"
    auto dot = captureName.rfind('.');
    if (dot != std::string_view::npos) {
        it = t.find(captureName.substr(0, dot));
        if (it != t.end()) return it->second;
    }

    return nullptr;
}

// ──────────────────────────────────────────────────────────────────────────────
// Highlight spans
// ──────────────────────────────────────────────────────────────────────────────

struct Span {
    uint32_t start;
    uint32_t end;
    uint32_t patternIdx; // query pattern index — higher = higher priority
    const char* color;   // ANSI SGR sequence
};

// Build a flat per-byte color map. Later (higher pattern index) and more
// specific (narrower) captures override earlier/broader ones.
static std::vector<const char*> computeColorMap(
    const TSLanguage* lang,
    const char* queryStr, uint32_t queryLen,
    TSTree* tree,
    const char* source, uint32_t sourceLen)
{
    uint32_t errorOffset = 0;
    TSQueryError errorType = TSQueryErrorNone;
    TSQuery* query = ts_query_new(lang, queryStr, queryLen, &errorOffset, &errorType);
    if (!query) return {};

    TSQueryCursor* cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, ts_tree_root_node(tree));

    // Collect all spans with their pattern index for priority
    std::vector<Span> spans;
    TSQueryMatch match;
    while (ts_query_cursor_next_match(cursor, &match)) {
        for (uint16_t i = 0; i < match.capture_count; ++i) {
            const TSQueryCapture& cap = match.captures[i];
            uint32_t nameLen = 0;
            const char* name = ts_query_capture_name_for_id(query, cap.index, &nameLen);
            std::string_view capName(name, nameLen);

            const char* color = lookupColor(capName);
            if (!color) continue;

            uint32_t start = ts_node_start_byte(cap.node);
            uint32_t end = ts_node_end_byte(cap.node);
            if (start < sourceLen && end <= sourceLen) {
                spans.push_back({start, end, match.pattern_index, color});
            }
        }
    }

    ts_query_cursor_delete(cursor);
    ts_query_delete(query);

    // Sort: broader spans first, then narrower/higher-priority overwrite them
    // This ensures more specific captures win over general ones
    std::sort(spans.begin(), spans.end(), [](const Span& a, const Span& b) {
        uint32_t widthA = a.end - a.start;
        uint32_t widthB = b.end - b.start;
        if (widthA != widthB) return widthA > widthB; // wider first
        return a.patternIdx < b.patternIdx;            // lower priority first
    });

    // Paint: each span overwrites the color map, so last (narrowest/highest-priority) wins
    std::vector<const char*> colorMap(sourceLen, nullptr);
    for (const auto& s : spans) {
        for (uint32_t i = s.start; i < s.end; ++i) {
            colorMap[i] = s.color;
        }
    }

    return colorMap;
}

// ──────────────────────────────────────────────────────────────────────────────
// Emit highlighted source
// ──────────────────────────────────────────────────────────────────────────────

static const char* LINE_NUM_COLOR = "\033[38;5;239m";

static void emitHighlighted(const char* source, uint32_t sourceLen,
                            const std::vector<const char*>& colorMap)
{
    // Count lines to determine padding width
    uint32_t lineCount = 1;
    for (uint32_t i = 0; i < sourceLen; ++i) {
        if (source[i] == '\n') ++lineCount;
    }
    int width = 1;
    for (uint32_t n = lineCount; n >= 10; n /= 10) ++width;

    const char* activeColor = nullptr;
    uint32_t pos = 0;
    uint32_t lineNum = 1;
    bool atLineStart = true;

    while (pos < sourceLen) {
        // Print line number at start of each line
        if (atLineStart) {
            if (activeColor) { std::fputs(RESET, stdout); activeColor = nullptr; }
            std::fprintf(stdout, "%s%0*u%s ", LINE_NUM_COLOR, width, lineNum, RESET);
            atLineStart = false;
        }

        const char* newColor = colorMap[pos];

        if (newColor != activeColor) {
            if (activeColor) std::fputs(RESET, stdout);
            if (newColor) std::fputs(newColor, stdout);
            activeColor = newColor;
        }

        // Find run of same color, stopping at newlines
        uint32_t runEnd = pos + 1;
        while (runEnd < sourceLen && colorMap[runEnd] == activeColor && source[runEnd - 1] != '\n') {
            ++runEnd;
        }

        std::fwrite(source + pos, 1, runEnd - pos, stdout);

        // Check if we crossed a newline
        for (uint32_t i = pos; i < runEnd; ++i) {
            if (source[i] == '\n') {
                ++lineNum;
                atLineStart = true;
            }
        }

        pos = runEnd;
    }

    if (activeColor) std::fputs(RESET, stdout);
}

// ──────────────────────────────────────────────────────────────────────────────
// Public API
// ──────────────────────────────────────────────────────────────────────────────

bool highlightToStdout(std::string_view source, std::string_view grammarName) {
    const GrammarInfo* info = getGrammar(grammarName);
    if (!info || !info->languageFn) return false;

    const TSLanguage* lang = info->languageFn();
    if (!lang) return false;

    TSParser* parser = ts_parser_new();
    if (!ts_parser_set_language(parser, lang)) {
        ts_parser_delete(parser);
        return false;
    }

    TSTree* tree = ts_parser_parse_string(parser, nullptr,
                                          source.data(),
                                          static_cast<uint32_t>(source.size()));
    if (!tree) {
        ts_parser_delete(parser);
        return false;
    }

    auto colorMap = computeColorMap(lang, info->highlightQuery, info->highlightQueryLen,
                                    tree, source.data(),
                                    static_cast<uint32_t>(source.size()));

    emitHighlighted(source.data(), static_cast<uint32_t>(source.size()), colorMap);

    ts_tree_delete(tree);
    ts_parser_delete(parser);
    return true;
}

// ──────────────────────────────────────────────────────────────────────────────
// MIME / extension → grammar name
// ──────────────────────────────────────────────────────────────────────────────

std::string resolveGrammar(std::string_view mime, std::string_view ext) {
    // MIME-based lookup
    if (mime == "text/x-c" || mime == "text/x-chdr")
        return "c";
    if (mime == "text/x-c++" || mime == "text/x-c++hdr" || mime == "text/x-c++src")
        return "cpp";
    if (mime == "text/x-python" || mime == "text/x-script.python")
        return "python";
    if (mime == "text/x-shellscript")
        return "bash";
    if (mime == "application/javascript" || mime == "text/javascript")
        return "javascript";
    if (mime == "text/x-java" || mime == "text/x-java-source")
        return "java";
    if (mime == "text/x-rustsrc")
        return "rust";
    if (mime == "text/x-go")
        return "go";
    if (mime == "application/json")
        return "json";
    if (mime == "text/x-yaml" || mime == "text/yaml")
        return "yaml";
    if (mime == "text/x-toml")
        return "toml";
    if (mime == "application/typescript" || mime == "text/x-typescript")
        return "typescript";

    // Extension-based fallback
    if (ext == ".c" || ext == ".h")
        return "c";
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx" ||
        ext == ".hpp" || ext == ".hxx" || ext == ".hh" || ext == ".h++")
        return "cpp";
    if (ext == ".py" || ext == ".pyi")
        return "python";
    if (ext == ".js" || ext == ".mjs" || ext == ".cjs" || ext == ".jsx")
        return "javascript";
    if (ext == ".ts" || ext == ".tsx" || ext == ".mts" || ext == ".cts")
        return "typescript";
    if (ext == ".rs")
        return "rust";
    if (ext == ".go")
        return "go";
    if (ext == ".java")
        return "java";
    if (ext == ".sh" || ext == ".bash" || ext == ".zsh")
        return "bash";
    if (ext == ".json" || ext == ".jsonc")
        return "json";
    if (ext == ".yaml" || ext == ".yml")
        return "yaml";
    if (ext == ".toml")
        return "toml";

    return "";
}

} // namespace ycat::ts
