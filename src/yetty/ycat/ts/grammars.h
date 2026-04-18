#pragma once

#include <tree_sitter/api.h>
#include <string>
#include <string_view>

namespace ycat::ts {

struct GrammarInfo {
    const char* name;
    const TSLanguage* (*languageFn)();
    const char* highlightQuery;     // contents of highlights.scm
    uint32_t highlightQueryLen;
};

// Get grammar info by name (e.g., "cpp", "python"). Returns nullptr if not found.
const GrammarInfo* getGrammar(std::string_view name);

} // namespace ycat::ts
