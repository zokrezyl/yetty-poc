#include "yecho-parser.h"
#include <filesystem>
#include <regex>
#include <algorithm>
#include <cctype>
#include <sstream>

// Include generated glyph registry if available (must be before namespace)
#if __has_include("glyph-registry-generated.h")
#include "glyph-registry-generated.h"
#define HAS_GENERATED_REGISTRY 1
#else
#define HAS_GENERATED_REGISTRY 0
#endif

namespace yetty {

//=============================================================================
// YEchoAttribute
//=============================================================================

bool YEchoAttribute::isInteger() const {
    if (value.empty()) return false;
    return std::all_of(value.begin(), value.end(), ::isdigit);
}

std::optional<uint32_t> YEchoAttribute::asColor() const {
    if (!isColor()) return std::nullopt;
    
    std::string hex = value.substr(1);  // Skip '#'
    
    // Expand #RGB to #RRGGBB
    if (hex.size() == 3) {
        hex = std::string{hex[0], hex[0], hex[1], hex[1], hex[2], hex[2]};
    }
    
    if (hex.size() != 6) return std::nullopt;
    
    try {
        return static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<int> YEchoAttribute::asInteger() const {
    if (!isInteger()) return std::nullopt;
    try {
        return std::stoi(value);
    } catch (...) {
        return std::nullopt;
    }
}

YEchoStyle YEchoAttribute::asStyle() const {
    if (key != "style" || value.empty()) return YEchoStyle::None;
    
    YEchoStyle result = YEchoStyle::None;
    
    // Parse "bold|italic|underline"
    size_t start = 0;
    while (start < value.size()) {
        size_t end = value.find('|', start);
        if (end == std::string::npos) end = value.size();
        
        std::string part = value.substr(start, end - start);
        
        if (part == "bold") {
            result = result | YEchoStyle::Bold;
        } else if (part == "italic") {
            result = result | YEchoStyle::Italic;
        } else if (part == "underline") {
            result = result | YEchoStyle::Underline;
        }
        
        start = end + 1;
    }
    
    return result;
}

//=============================================================================
// GlyphRegistry
//=============================================================================

GlyphRegistry::GlyphRegistry() {
#if HAS_GENERATED_REGISTRY
    // Load built-in glyphs from generated header
    for (const auto& entry : yetty::generated::GLYPHS) {
        glyphs_[std::string(entry.name)] = entry.codepoint;
    }
#endif
}

bool GlyphRegistry::loadFromDirectory(const std::string& shaderDir) {
    std::string glyphsDir = shaderDir;
    if (!glyphsDir.empty() && glyphsDir.back() != '/') {
        glyphsDir += '/';
    }
    glyphsDir += "glyphs/";
    
    if (!std::filesystem::exists(glyphsDir)) {
        return false;
    }
    
    // Pattern: 0xNNNN-name.wgsl
    std::regex pattern(R"(^0x([0-9a-fA-F]+)-(.+)\.wgsl$)");
    
    int loaded = 0;
    for (const auto& entry : std::filesystem::directory_iterator(glyphsDir)) {
        if (!entry.is_regular_file()) continue;
        
        std::string filename = entry.path().filename().string();
        std::smatch match;
        
        if (std::regex_match(filename, match, pattern)) {
            try {
                uint32_t offset = static_cast<uint32_t>(std::stoul(match[1].str(), nullptr, 16));
                std::string name = match[2].str();
                uint32_t codepoint = SHADER_GLYPH_BASE + offset;
                glyphs_[name] = codepoint;
                loaded++;
            } catch (...) {
                // Skip malformed filenames
            }
        }
    }
    
    return loaded > 0;
}

uint32_t GlyphRegistry::getCodepoint(const std::string& name) const {
    auto it = glyphs_.find(name);
    return (it != glyphs_.end()) ? it->second : 0;
}

bool GlyphRegistry::hasGlyph(const std::string& name) const {
    return glyphs_.find(name) != glyphs_.end();
}

std::vector<std::string> GlyphRegistry::getNames() const {
    std::vector<std::string> names;
    names.reserve(glyphs_.size());
    for (const auto& [name, _] : glyphs_) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

void GlyphRegistry::registerGlyph(const std::string& name, uint32_t codepoint) {
    glyphs_[name] = codepoint;
}

//=============================================================================
// YEchoParser
//=============================================================================

YEchoParser::YEchoParser(const GlyphRegistry* registry)
    : registry_(registry) {}

std::vector<YEchoSpan> YEchoParser::parse(const std::string& input) {
    errors_.clear();
    std::vector<YEchoSpan> spans;
    size_t pos = 0;
    parseInto(input, pos, spans, '\0');
    return spans;
}

std::string YEchoParser::toTerminal(const std::vector<YEchoSpan>& spans) const {
    std::string result;
    
    for (const auto& span : spans) {
        switch (span.type) {
            case YEchoSpan::Type::Text:
                result += span.text;
                break;
                
            case YEchoSpan::Type::Glyph: {
                uint32_t cp = 0;
                if (registry_) {
                    cp = registry_->getCodepoint(span.glyphName);
                }
                if (cp == 0) {
                    result += "[?" + span.glyphName + "]";
                } else {
                    appendUtf8(result, cp);
                }
                break;
            }
                
            case YEchoSpan::Type::Block: {
                // Determine card type:
                // 1. Explicit: card=TYPE attribute
                // 2. Implicit ytext: @font-size, @font-family, or other @font.* attributes
                std::string cardType;
                bool hasYtextAttrs = false;
                
                for (const auto& attr : span.attrs) {
                    if (attr.key == "card") {
                        cardType = attr.value;
                    } else if (attr.key.starts_with("@font-") || 
                               attr.key.starts_with("@font.")) {
                        hasYtextAttrs = true;
                    }
                }
                
                // Implicit ytext when font attributes present
                if (cardType.empty() && hasYtextAttrs) {
                    cardType = "ytext";
                }
                
                if (!cardType.empty()) {
                    result += generateOSC(span, cardType);
                } else {
                    // ANSI styled block
                    std::string codes;
                    for (const auto& attr : span.attrs) {
                        if (attr.key == "color") {
                            if (auto color = attr.asColor()) {
                                uint8_t r = (*color >> 16) & 0xFF;
                                uint8_t g = (*color >> 8) & 0xFF;
                                uint8_t b = *color & 0xFF;
                                if (!codes.empty()) codes += ";";
                                codes += "38;2;" + std::to_string(r) + ";" + 
                                         std::to_string(g) + ";" + std::to_string(b);
                            }
                        } else if (attr.key == "bg") {
                            if (auto color = attr.asColor()) {
                                uint8_t r = (*color >> 16) & 0xFF;
                                uint8_t g = (*color >> 8) & 0xFF;
                                uint8_t b = *color & 0xFF;
                                if (!codes.empty()) codes += ";";
                                codes += "48;2;" + std::to_string(r) + ";" + 
                                         std::to_string(g) + ";" + std::to_string(b);
                            }
                        } else if (attr.key == "style") {
                            auto style = attr.asStyle();
                            if (hasStyle(style, YEchoStyle::Bold)) {
                                if (!codes.empty()) codes += ";";
                                codes += "1";
                            }
                            if (hasStyle(style, YEchoStyle::Italic)) {
                                if (!codes.empty()) codes += ";";
                                codes += "3";
                            }
                            if (hasStyle(style, YEchoStyle::Underline)) {
                                if (!codes.empty()) codes += ";";
                                codes += "4";
                            }
                        }
                    }
                    
                    if (!codes.empty()) {
                        result += "\033[" + codes + "m";
                    }
                    result += span.content;
                    result += "\033[0m";
                }
                break;
            }
        }
    }
    
    return result;
}

void YEchoParser::parseInto(const std::string& input, size_t& pos, 
                            std::vector<YEchoSpan>& spans, char endChar) {
    std::string textAccum;
    
    auto flushText = [&]() {
        if (!textAccum.empty()) {
            YEchoSpan span;
            span.type = YEchoSpan::Type::Text;
            span.text = std::move(textAccum);
            spans.push_back(std::move(span));
            textAccum.clear();
        }
    };
    
    while (pos < input.size()) {
        char c = input[pos];
        
        // Check for end character (for nested parsing)
        if (endChar != '\0' && c == endChar) {
            break;
        }
        
        // Escape sequences
        if (c == '\\' && pos + 1 < input.size()) {
            char next = input[pos + 1];
            if (next == '{' || next == '}' || next == '@' || next == '\\') {
                textAccum += next;
                pos += 2;
                continue;
            }
        }
        
        // Glyph: @name
        if (c == '@') {
            YEchoSpan span;
            if (parseGlyph(input, pos, span)) {
                flushText();  // Only flush if glyph is valid
                spans.push_back(std::move(span));
            } else {
                // Failed to parse glyph, treat @ as literal
                textAccum += '@';
                pos++;
            }
            continue;
        }
        
        // Block: {statements: content}
        if (c == '{') {
            YEchoSpan span;
            if (parseBlock(input, pos, span)) {
                flushText();  // Only flush if block is valid
                spans.push_back(std::move(span));
            } else {
                // Failed to parse block, treat { as literal
                textAccum += '{';
                pos++;
            }
            continue;
        }
        
        // Regular character
        textAccum += c;
        pos++;
    }
    
    flushText();
}

bool YEchoParser::parseGlyph(const std::string& input, size_t& pos, YEchoSpan& span) {
    // @name - name is alphanumeric plus hyphen/underscore
    if (pos >= input.size() || input[pos] != '@') {
        return false;
    }
    
    size_t start = pos + 1;
    size_t end = start;
    
    while (end < input.size()) {
        char c = input[end];
        if (std::isalnum(c) || c == '-' || c == '_') {
            end++;
        } else {
            break;
        }
    }
    
    if (end == start) {
        return false;
    }
    
    std::string name = input.substr(start, end - start);
    
    if (registry_ && !registry_->hasGlyph(name)) {
        errors_.push_back("Unknown glyph: @" + name);
    }
    
    span.type = YEchoSpan::Type::Glyph;
    span.glyphName = std::move(name);
    pos = end;
    
    return true;
}

bool YEchoParser::parseBlock(const std::string& input, size_t& pos, YEchoSpan& span) {
    // {statements: content}
    // Statements on left side separated by semicolons
    // Content on right side (raw string)
    
    if (pos >= input.size() || input[pos] != '{') {
        return false;
    }
    
    size_t start = pos + 1;
    
    // Find the first ':' at depth 0, respecting quotes
    int braceDepth = 0;
    bool inQuotes = false;
    char quoteChar = 0;
    size_t colonPos = std::string::npos;
    
    for (size_t i = start; i < input.size(); ++i) {
        char c = input[i];
        
        if (!inQuotes) {
            if (c == '"' || c == '\'') {
                inQuotes = true;
                quoteChar = c;
            } else if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                if (braceDepth == 0) {
                    errors_.push_back("Missing ':' in block");
                    return false;
                }
                braceDepth--;
            } else if (c == ':' && braceDepth == 0) {
                colonPos = i;
                break;
            }
        } else {
            if (c == quoteChar) {
                inQuotes = false;
            }
        }
    }
    
    if (colonPos == std::string::npos) {
        errors_.push_back("Missing ':' in block");
        return false;
    }
    
    // Parse left side (statements separated by semicolons)
    std::string leftSide = input.substr(start, colonPos - start);
    parseStatements(leftSide, span.attrs);
    
    // Find closing '}' for content
    size_t contentStart = colonPos + 1;
    if (contentStart < input.size() && input[contentStart] == ' ') {
        contentStart++;
    }
    
    braceDepth = 0;
    inQuotes = false;
    size_t endPos = std::string::npos;
    
    for (size_t i = contentStart; i < input.size(); ++i) {
        char c = input[i];
        
        if (!inQuotes) {
            if (c == '"' || c == '\'') {
                inQuotes = true;
                quoteChar = c;
            } else if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                if (braceDepth == 0) {
                    endPos = i;
                    break;
                }
                braceDepth--;
            }
        } else {
            if (c == quoteChar) {
                inQuotes = false;
            }
        }
    }
    
    if (endPos == std::string::npos) {
        errors_.push_back("Unclosed block");
        return false;
    }
    
    span.type = YEchoSpan::Type::Block;
    span.content = input.substr(contentStart, endPos - contentStart);
    pos = endPos + 1;
    
    return true;
}

void YEchoParser::parseStatements(const std::string& str, std::vector<YEchoAttribute>& attrs) {
    // Semicolon-separated statements: "card=plot; w=80; h=20; @view=-3.14..3.14,-1..1"
    std::string current;
    bool inQuotes = false;
    char quoteChar = 0;
    
    auto processStatement = [&](const std::string& stmt) {
        // Trim
        size_t start = stmt.find_first_not_of(" \t");
        size_t end = stmt.find_last_not_of(" \t");
        if (start == std::string::npos) return;
        
        std::string trimmed = stmt.substr(start, end - start + 1);
        if (trimmed.empty()) return;
        
        // Split on first '='
        size_t eqPos = trimmed.find('=');
        if (eqPos == std::string::npos) {
            // No '=', treat as key with empty value
            YEchoAttribute attr;
            attr.key = trimmed;
            attrs.push_back(std::move(attr));
        } else {
            YEchoAttribute attr;
            attr.key = trimmed.substr(0, eqPos);
            attr.value = trimmed.substr(eqPos + 1);
            attrs.push_back(std::move(attr));
        }
    };
    
    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        
        if (!inQuotes) {
            if (c == '"' || c == '\'') {
                inQuotes = true;
                quoteChar = c;
                current += c;
            } else if (c == ';') {
                processStatement(current);
                current.clear();
            } else {
                current += c;
            }
        } else {
            current += c;
            if (c == quoteChar) {
                inQuotes = false;
            }
        }
    }
    
    // Process last statement
    processStatement(current);
}

std::string YEchoParser::generateOSC(const YEchoSpan& span, const std::string& cardType) const {
    // Generate OSC 666666 escape sequence for card
    // Format: ESC ] 666666 ; generic-args ; plugin-args ; payload ESC \
    
    int width = 0;
    int height = 0;
    std::string pluginArgs;
    
    for (const auto& attr : span.attrs) {
        if (attr.key == "card") {
            // Skip, we already have cardType
        } else if (attr.key == "w") {
            try { width = std::stoi(attr.value); } catch (...) {}
        } else if (attr.key == "h") {
            try { height = std::stoi(attr.value); } catch (...) {}
        } else {
            // Build plugin args
            if (!pluginArgs.empty()) pluginArgs += " ";
            if (attr.key[0] == '@') {
                // @view, @font-size, etc. -> --view, --font-size
                pluginArgs += "--" + attr.key.substr(1) + " " + attr.value;
            } else {
                pluginArgs += "--" + attr.key + " " + attr.value;
            }
        }
    }
    
    // Build generic args
    std::string genericArgs = "create -p " + cardType;
    if (width > 0) genericArgs += " -w " + std::to_string(width);
    if (height > 0) genericArgs += " -h " + std::to_string(height);
    genericArgs += " -r";  // relative positioning
    
    // Handle content based on card type
    if (!span.content.empty()) {
        if (!pluginArgs.empty()) pluginArgs += " ";
        if (cardType == "plot") {
            // Plot: content is the expression
            pluginArgs += "--expr \"" + span.content + "\"";
        } else if (cardType == "ytext") {
            // Ytext: content is the text
            pluginArgs += "--text \"" + span.content + "\"";
        } else if (cardType == "qr") {
            // QR: content is the data
            pluginArgs += "--data \"" + span.content + "\"";
        } else {
            // Generic: pass as --content
            pluginArgs += "--content \"" + span.content + "\"";
        }
    }
    
    // Build OSC sequence
    std::string osc = "\033]666666;";
    osc += genericArgs;
    osc += ";";
    osc += pluginArgs;
    osc += ";\033\\";
    
    return osc;
}

void YEchoParser::appendUtf8(std::string& out, uint32_t cp) const {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
}

} // namespace yetty
