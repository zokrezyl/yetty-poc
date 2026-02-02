#pragma once

#include <string>
#include <string_view>

namespace ycat::ts {

// Syntax-highlight source code and write ANSI-colored output to stdout.
// Returns true on success, false if the grammar is unavailable or parsing fails.
bool highlightToStdout(std::string_view source, std::string_view grammarName);

// Resolve a MIME type and/or file extension to a grammar name.
// Returns empty string if no grammar matches.
std::string resolveGrammar(std::string_view mime, std::string_view extension);

} // namespace ycat::ts
