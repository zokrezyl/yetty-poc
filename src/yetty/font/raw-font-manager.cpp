#include <yetty/font/raw-font-manager.h>
#include <yetty/font/freetype.h>

#include <fstream>
#include <vector>

namespace yetty::font {

class RawFontManagerImpl : public RawFontManager {
public:
    RawFontManagerImpl() = default;
    ~RawFontManagerImpl() override = default;

    Result<void> init() {
        // Ensure FreeType is initialized for this thread
        if (!ftLibrary()) {
            return Err("RawFontManager: Failed to initialize FreeType");
        }
        return Ok();
    }

    Result<RawFont::Ptr> createFromData(const uint8_t* data, size_t size,
                                         const std::string& name) override {
        return RawFont::create(data, size, name);
    }

    Result<RawFont::Ptr> createFromFile(const std::string& path) override {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) {
            return Err<RawFont::Ptr>("RawFontManager: Failed to open file: " + path);
        }

        auto fileSize = file.tellg();
        if (fileSize <= 0) {
            return Err<RawFont::Ptr>("RawFontManager: Empty or invalid file: " + path);
        }

        file.seekg(0, std::ios::beg);
        std::vector<uint8_t> data(static_cast<size_t>(fileSize));
        if (!file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
            return Err<RawFont::Ptr>("RawFontManager: Failed to read file: " + path);
        }

        // Extract font name from path
        std::string name = path;
        auto lastSlash = name.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            name = name.substr(lastSlash + 1);
        }
        auto lastDot = name.find_last_of('.');
        if (lastDot != std::string::npos) {
            name = name.substr(0, lastDot);
        }

        return RawFont::create(data.data(), data.size(), name);
    }
};

Result<RawFontManager::Ptr> RawFontManager::createImpl() {
    auto impl = Ptr(new RawFontManagerImpl());
    auto res = static_cast<RawFontManagerImpl*>(impl.get())->init();
    if (!res) return Err<Ptr>("RawFontManager creation failed", res);
    return Ok(std::move(impl));
}

} // namespace yetty::font
