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

YEchoSpan YEchoParser::parse(const std::string& input) {
    errors_.clear();
    
    YEchoSpan root;
    root.type = YEchoSpan::Type::StyledBlock;
    
    size_t pos = 0;
    parseInto(input, pos, root.children);
    
    return root;
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
            flushText();
            YEchoSpan span;
            if (parseGlyph(input, pos, span)) {
                spans.push_back(std::move(span));
            } else {
                // Failed to parse glyph, treat @ as literal
                textAccum += '@';
                pos++;
            }
            continue;
        }
        
        // Styled block: {attrs: content}
        if (c == '{') {
            flushText();
            YEchoSpan span;
            if (parseStyledBlock(input, pos, span)) {
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
        // No name after @
        return false;
    }
    
    std::string name = input.substr(start, end - start);
    
    // Verify glyph exists if registry available
    if (registry_ && !registry_->hasGlyph(name)) {
        errors_.push_back("Unknown glyph: @" + name);
        // Still parse it, but mark as unknown
    }
    
    span.type = YEchoSpan::Type::Glyph;
    span.glyphName = std::move(name);
    pos = end;
    
    return true;
}

bool YEchoParser::parseStyledBlock(const std::string& input, size_t& pos, YEchoSpan& span) {
    // {attrs: content}
    if (pos >= input.size() || input[pos] != '{') {
        return false;
    }
    
    size_t start = pos + 1;
    
    // Find the first ':' which separates attrs from content
    size_t colonPos = input.find(':', start);
    if (colonPos == std::string::npos) {
        errors_.push_back("Missing ':' in styled block");
        return false;
    }
    
    // Parse attributes
    std::string attrsStr = input.substr(start, colonPos - start);
    size_t attrPos = 0;
    if (!parseAttributes(attrsStr, attrPos, span.attrs)) {
        return false;
    }
    
    // Skip ':' and optional single space
    size_t contentStart = colonPos + 1;
    if (contentStart < input.size() && input[contentStart] == ' ') {
        contentStart++;
    }
    
    // Parse content until '}'
    pos = contentStart;
    span.type = YEchoSpan::Type::StyledBlock;
    parseInto(input, pos, span.children, '}');
    
    if (pos >= input.size() || input[pos] != '}') {
        errors_.push_back("Unclosed styled block");
        return false;
    }
    
    pos++;  // Skip '}'
    return true;
}

bool YEchoParser::parseAttributes(const std::string& input, size_t& pos,
                                   std::vector<YEchoAttribute>& attrs) {
    // Attributes separated by whitespace: "font=32 color=#ff0000 bold"
    
    while (pos < input.size()) {
        // Skip whitespace
        while (pos < input.size() && std::isspace(input[pos])) {
            pos++;
        }
        
        if (pos >= input.size()) break;
        
        // Parse key
        size_t keyStart = pos;
        while (pos < input.size() && input[pos] != '=' && !std::isspace(input[pos])) {
            pos++;
        }
        
        if (pos == keyStart) break;
        
        YEchoAttribute attr;
        attr.key = input.substr(keyStart, pos - keyStart);
        
        // Check for '=' and value
        if (pos < input.size() && input[pos] == '=') {
            pos++;  // Skip '='
            
            size_t valStart = pos;
            while (pos < input.size() && !std::isspace(input[pos])) {
                pos++;
            }
            
            attr.value = input.substr(valStart, pos - valStart);
        }
        
        attrs.push_back(std::move(attr));
    }
    
    return true;
}

std::string YEchoParser::toTerminal(const YEchoSpan& root) const {
    std::string result;
    
    // Helper to build ANSI escape for styled block
    auto emitAnsiStart = [&](const std::vector<YEchoAttribute>& attrs) {
        std::string codes;
        
        for (const auto& attr : attrs) {
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
    };
    
    std::function<void(const YEchoSpan&)> visit = [&](const YEchoSpan& span) {
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
                    // Unknown glyph - output placeholder
                    result += "[?";
                    result += span.glyphName;
                    result += "]";
                } else {
                    appendUtf8(result, cp);
                }
                break;
            }
                
            case YEchoSpan::Type::StyledBlock: {
                // Emit ANSI start codes
                emitAnsiStart(span.attrs);
                
                // Render children
                for (const auto& child : span.children) {
                    visit(child);
                }
                
                // Reset ANSI
                result += "\033[0m";
                break;
            }
        }
    };
    
    if (root.type == YEchoSpan::Type::StyledBlock && root.attrs.empty()) {
        // Root container - just process children
        for (const auto& child : root.children) {
            visit(child);
        }
    } else {
        visit(root);
    }
    
    return result;
}

bool YEchoParser::needsYtext(const YEchoSpan& root) const {
    std::function<bool(const YEchoSpan&)> check = [&](const YEchoSpan& span) -> bool {
        if (span.type == YEchoSpan::Type::StyledBlock) {
            for (const auto& attr : span.attrs) {
                if (attr.key == "font-size") {
                    return true;
                }
            }
            for (const auto& child : span.children) {
                if (check(child)) return true;
            }
        }
        return false;
    };
    
    return check(root);
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
