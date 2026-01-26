#include <yetty/shader-font.h>
#include <yetty/shader-manager.h>
#include <ytrace/ytrace.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <unordered_map>

namespace yetty {

// Base codepoints for each category
static constexpr uint32_t CARD_GLYPH_BASE = 0x100000;
static constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;

class ShaderFontImpl : public ShaderFont {
public:
    ShaderFontImpl(Category category, const std::string& shaderDir);
    ~ShaderFontImpl() override = default;

    Result<void> init(std::shared_ptr<ShaderManager> shaderMgr);

    // Font::init() - not used for ShaderFont (we use init with shaderMgr)
    Result<void> init() override { return Ok(); }

    // Font interface
    uint32_t getGlyphIndex(uint32_t codepoint) override;
    uint32_t getGlyphIndex(uint32_t codepoint, Style style) override;
    uint32_t getGlyphIndex(uint32_t codepoint, bool bold, bool italic) override;
    void uploadToGpu() override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }
    uint32_t getAtlasWidth() const override { return 0; }
    uint32_t getAtlasHeight() const override { return 0; }
    const std::vector<uint8_t>& getAtlasData() const override { return _empty; }

    // ShaderProvider interface
    std::string getCode() const override;
    std::string getDispatchCode() const override;
    uint32_t getFunctionCount() const override;

    // ShaderFont specific
    Category getCategory() const override { return _category; }
    uint32_t getBaseCodepoint() const override { return _baseCodepoint; }
    Result<void> reload() override;
    std::vector<std::string> getShaderNames() const override;

private:
    Result<void> loadShaders();
    bool parseShaderFilename(const std::string& filename, uint32_t& offset, std::string& name);

    Category _category;
    std::string _shaderDir;
    uint32_t _baseCodepoint;

    struct ShaderEntry {
        uint32_t offset;
        uint32_t codepoint;
        std::string name;
        std::string code;
    };
    std::vector<ShaderEntry> _entries;
    std::unordered_map<uint32_t, uint32_t> _codepointToIndex;

    bool _dirty = true;
    std::vector<uint8_t> _empty;
};

// Factory implementation
Result<ShaderFont::Ptr> ShaderFont::createImpl(ContextType&,
                                                std::shared_ptr<ShaderManager> shaderMgr,
                                                Category category,
                                                const std::string& shaderDir) {
    auto impl = Ptr(new ShaderFontImpl(category, shaderDir));
    if (auto res = static_cast<ShaderFontImpl*>(impl.get())->init(shaderMgr); !res) {
        return Err<Ptr>("ShaderFont init failed", res);
    }
    return Ok(std::move(impl));
}

ShaderFontImpl::ShaderFontImpl(Category category, const std::string& shaderDir)
    : _category(category), _shaderDir(shaderDir) {
    _baseCodepoint = (category == Category::Card) ? CARD_GLYPH_BASE : SHADER_GLYPH_BASE;

    if (!_shaderDir.empty() && _shaderDir.back() != '/' && _shaderDir.back() != '\\') {
        _shaderDir += '/';
    }
}

Result<void> ShaderFontImpl::init(std::shared_ptr<ShaderManager> shaderMgr) {
    if (auto res = loadShaders(); !res) {
        return res;
    }

    // Register with ShaderManager
    if (shaderMgr) {
        shaderMgr->addProvider(shared_from_this());
    }

    return Ok();
}

bool ShaderFontImpl::parseShaderFilename(const std::string& filename, uint32_t& offset, std::string& name) {
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

    // Try decimal format (legacy): NNNNNN-name.wgsl
    std::regex decPattern(R"(^(\d+)-(.+)\.wgsl$)");
    std::smatch decMatch;
    if (std::regex_match(filename, decMatch, decPattern)) {
        try {
            uint32_t codepoint = static_cast<uint32_t>(std::stoul(decMatch[1].str()));
            if (codepoint >= _baseCodepoint) {
                offset = codepoint - _baseCodepoint;
                name = decMatch[2].str();
                return true;
            }
            if (_category == Category::Glyph && codepoint >= CARD_GLYPH_BASE) {
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

Result<void> ShaderFontImpl::loadShaders() {
    _entries.clear();
    _codepointToIndex.clear();

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
    yinfo("ShaderFont: filesystem limited on this platform, using embedded shaders");
    return Ok();
#else
    if (_shaderDir.empty()) {
        yinfo("ShaderFont: no shader directory specified");
        return Ok();
    }

    std::string searchDir = _shaderDir;
    if (_category == Category::Card) {
        searchDir += "cards/";
    } else {
        searchDir += "glyphs/";
    }

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

            uint32_t codepoint = _baseCodepoint + offset;

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
    }

    std::sort(_entries.begin(), _entries.end(),
              [](const auto& a, const auto& b) { return a.offset < b.offset; });

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

Result<void> ShaderFontImpl::reload() {
    return loadShaders();
}

// Font interface
uint32_t ShaderFontImpl::getGlyphIndex(uint32_t codepoint) {
    auto it = _codepointToIndex.find(codepoint);
    if (it != _codepointToIndex.end()) {
        return codepoint;
    }
    if (!_entries.empty()) {
        return _entries[0].codepoint;
    }
    return 0;
}

uint32_t ShaderFontImpl::getGlyphIndex(uint32_t codepoint, Style) {
    return getGlyphIndex(codepoint);
}

uint32_t ShaderFontImpl::getGlyphIndex(uint32_t codepoint, bool, bool) {
    return getGlyphIndex(codepoint);
}

void ShaderFontImpl::uploadToGpu() {
    // Shader fonts contribute code to shader module, no texture upload
}

// ShaderProvider interface
std::string ShaderFontImpl::getCode() const {
    std::string code;
    code.reserve(_entries.size() * 1024);

    for (const auto& entry : _entries) {
        code += "// Shader ";
        code += (_category == Category::Card) ? "card" : "glyph";
        code += ": ";
        code += entry.name;
        code += " (codepoint 0x";
        char buf[16];
        snprintf(buf, sizeof(buf), "%06X", entry.codepoint);
        code += buf;
        code += ")\n";
        code += entry.code;
        code += "\n\n";
    }

    return code;
}

std::string ShaderFontImpl::getDispatchCode() const {
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

uint32_t ShaderFontImpl::getFunctionCount() const {
    return static_cast<uint32_t>(_entries.size());
}

std::vector<std::string> ShaderFontImpl::getShaderNames() const {
    std::vector<std::string> names;
    names.reserve(_entries.size());
    for (const auto& entry : _entries) {
        names.push_back(entry.name);
    }
    return names;
}

} // namespace yetty
