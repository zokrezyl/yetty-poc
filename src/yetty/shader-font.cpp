#include <yetty/shader-font.h>
#include <ytrace/ytrace.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>

namespace yetty {

// Base codepoints for each category
static constexpr uint32_t CARD_GLYPH_BASE = 0x100000;
static constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;

ShaderFont::ShaderFont(Category category, const std::string& shaderDir)
    : _category(category), _shaderDir(shaderDir) {
    // Set base codepoint based on category
    _baseCodepoint = (category == Category::Card) ? CARD_GLYPH_BASE : SHADER_GLYPH_BASE;

    // Ensure path ends with separator
    if (!_shaderDir.empty() && _shaderDir.back() != '/' && _shaderDir.back() != '\\') {
        _shaderDir += '/';
    }
}

Result<ShaderFont::Ptr> ShaderFont::create(Category category, const std::string& shaderDir) {
    auto font = Ptr(new ShaderFont(category, shaderDir));
    if (auto res = font->init(); !res) {
        return Err<Ptr>("Failed to initialize ShaderFont", res);
    }
    return Ok(std::move(font));
}

Result<void> ShaderFont::init() {
    return loadShaders();
}

bool ShaderFont::parseShaderFilename(const std::string& filename, uint32_t& offset, std::string& name) {
    // Try hex format: 0xNNNN-name.wgsl
    std::regex hexPattern(R"(^0x([0-9a-fA-F]+)-(.+)\.wgsl$)");
    std::smatch hexMatch;
    if (std::regex_match(filename, hexMatch, hexPattern)) {
        try {
            offset = static_cast<uint32_t>(std::stoul(hexMatch[1].str(), nullptr, 16));
            name = hexMatch[2].str();
            return true;
        } catch (...) {
            return false;
        }
    }

    // Try decimal format (legacy): NNNNNN-name.wgsl (e.g., 1048577-spinner.wgsl)
    std::regex decPattern(R"(^(\d+)-(.+)\.wgsl$)");
    std::smatch decMatch;
    if (std::regex_match(filename, decMatch, decPattern)) {
        try {
            uint32_t codepoint = static_cast<uint32_t>(std::stoul(decMatch[1].str()));
            // For legacy files, the number is the full codepoint, not offset
            // Calculate offset from the category's base
            if (codepoint >= _baseCodepoint) {
                offset = codepoint - _baseCodepoint;
                name = decMatch[2].str();
                return true;
            }
            // If codepoint is less than base, check if it's in the card range for glyph category
            if (_category == Category::Glyph && codepoint >= CARD_GLYPH_BASE) {
                // Legacy: full codepoint was used, convert to offset from CARD_GLYPH_BASE
                // for backward compatibility with existing shader files
                offset = codepoint - CARD_GLYPH_BASE;
                name = decMatch[2].str();
                return true;
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    return false;
}

Result<void> ShaderFont::loadShaders() {
    _entries.clear();
    _codepointToIndex.clear();

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
    // Limited filesystem support on web/Android
    yinfo("ShaderFont: filesystem limited on this platform, using embedded shaders");
    return Ok();
#else
    if (_shaderDir.empty()) {
        yinfo("ShaderFont: no shader directory specified");
        return Ok();
    }

    // Determine subdirectory based on category
    std::string searchDir = _shaderDir;
    if (_category == Category::Card) {
        searchDir += "cards/";
    } else {
        searchDir += "glyphs/";
    }

    // If the subdirectory doesn't exist, fall back to main directory
    // (for backward compatibility with old file structure)
    if (!std::filesystem::exists(searchDir)) {
        searchDir = _shaderDir;
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(searchDir)) {
            if (!entry.is_regular_file()) continue;

            std::string filename = entry.path().filename().string();
            uint32_t offset;
            std::string name;

            if (!parseShaderFilename(filename, offset, name)) {
                continue;
            }

            // Read shader file
            std::ifstream file(entry.path());
            if (!file.is_open()) {
                ywarn("ShaderFont: failed to open {}", entry.path().string());
                continue;
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string code = buffer.str();

            if (code.empty()) {
                ywarn("ShaderFont: empty shader file {}", filename);
                continue;
            }

            // Calculate full codepoint
            uint32_t codepoint = _baseCodepoint + offset;

            // Store entry
            ShaderEntry shaderEntry;
            shaderEntry.offset = offset;
            shaderEntry.codepoint = codepoint;
            shaderEntry.name = name;
            shaderEntry.code = code;

            uint32_t index = static_cast<uint32_t>(_entries.size());
            _entries.push_back(std::move(shaderEntry));
            _codepointToIndex[codepoint] = index;

            yinfo("ShaderFont: loaded {} at offset 0x{:04X} (codepoint 0x{:06X})",
                  filename, offset, codepoint);
        }
    } catch (const std::exception& e) {
        yerror("ShaderFont: error scanning shaders: {}", e.what());
        // Don't fail - just continue with whatever we have
    }

    // Sort entries by offset for consistent ordering
    std::sort(_entries.begin(), _entries.end(),
              [](const auto& a, const auto& b) { return a.offset < b.offset; });

    // Rebuild index map after sorting
    _codepointToIndex.clear();
    for (uint32_t i = 0; i < _entries.size(); i++) {
        _codepointToIndex[_entries[i].codepoint] = i;
    }

    yinfo("ShaderFont: loaded {} {} shaders from {}",
          _entries.size(),
          (_category == Category::Card) ? "card" : "glyph",
          searchDir);

    _dirty = true;
    return Ok();
#endif
}

Result<void> ShaderFont::reload() {
    return loadShaders();
}

// =========================================================================
// YettyFont interface
// =========================================================================

uint32_t ShaderFont::getGlyphIndex(uint32_t codepoint) {
    auto it = _codepointToIndex.find(codepoint);
    if (it != _codepointToIndex.end()) {
        // Return the codepoint itself - shader glyphs use codepoint as glyph index
        return codepoint;
    }
    // Return default (first shader in category, or 0 if none)
    if (!_entries.empty()) {
        return _entries[0].codepoint;
    }
    return 0;
}

uint32_t ShaderFont::getGlyphIndex(uint32_t codepoint, Style) {
    // Shader fonts don't have style variants
    return getGlyphIndex(codepoint);
}

uint32_t ShaderFont::getGlyphIndex(uint32_t codepoint, bool, bool) {
    // Shader fonts don't have style variants
    return getGlyphIndex(codepoint);
}

void ShaderFont::uploadToGpu() {
    // Shader fonts don't have texture data to upload
    // They contribute code to the shader module instead
}

// =========================================================================
// ShaderProvider interface
// =========================================================================

std::string ShaderFont::getCode() const {
    std::string code;
    code.reserve(_entries.size() * 1024); // Estimate

    for (const auto& entry : _entries) {
        code += "// Shader ";
        code += (_category == Category::Card) ? "card" : "glyph";
        code += ": ";
        code += entry.name;
        code += " (codepoint 0x";
        // Format as hex
        char buf[16];
        snprintf(buf, sizeof(buf), "%06X", entry.codepoint);
        code += buf;
        code += ")\n";
        code += entry.code;
        code += "\n\n";
    }

    return code;
}

std::string ShaderFont::getDispatchCode() const {
    if (_entries.empty()) {
        return "";
    }

    std::string dispatch;
    dispatch.reserve(_entries.size() * 128);

    bool first = true;
    for (const auto& entry : _entries) {
        if (!first) {
            dispatch += " else ";
        }
        first = false;

        // Generate dispatch: if (glyphIndex == CODEPOINT) { return shaderGlyph_CODEPOINT(...); }
        dispatch += "if (glyphIndex == ";
        dispatch += std::to_string(entry.codepoint);
        dispatch += "u) {\n        return shaderGlyph_";
        dispatch += std::to_string(entry.codepoint);
        dispatch += "(localUV, time, fg, bg, pixelPos, mousePos);\n    }";
    }

    if (!dispatch.empty()) {
        dispatch += "\n    ";
    }

    return dispatch;
}

uint32_t ShaderFont::getFunctionCount() const {
    return static_cast<uint32_t>(_entries.size());
}

std::vector<std::string> ShaderFont::getShaderNames() const {
    std::vector<std::string> names;
    names.reserve(_entries.size());
    for (const auto& entry : _entries) {
        names.push_back(entry.name);
    }
    return names;
}

} // namespace yetty
