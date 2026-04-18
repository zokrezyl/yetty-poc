#pragma once

#include <yetty/result.hpp>
#include <yetty/base/factory.h>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace yetty {

/**
 * IncbinAssets - Singleton for accessing embedded binary assets.
 *
 * Assets are embedded at compile time via incbin and accessible at runtime.
 * On first run, assets can be extracted to the filesystem for components
 * that require file paths (e.g., FreeType font loading).
 *
 * Usage:
 *   auto assets = IncbinAssets::instance()?;
 *   auto shaderCode = assets->getString("shader/gpu-screen.wgsl");
 *   assets->extractTo("~/.cache/yetty/assets");
 */
class IncbinAssets : public base::Singleton<IncbinAssets> {
public:
    using Ptr = std::shared_ptr<IncbinAssets>;

    static Result<Ptr> createImpl();

    virtual ~IncbinAssets() = default;

    // Get raw asset data by name (e.g., "shader/gpu-screen.wgsl")
    virtual std::span<const uint8_t> get(std::string_view name) const = 0;

    // Get asset as string view (for text assets like shaders)
    virtual std::string_view getString(std::string_view name) const = 0;

    // Check if asset exists
    virtual bool has(std::string_view name) const = 0;

    // List all embedded asset names
    virtual std::vector<std::string> list() const = 0;

    // Extract all assets to directory
    // Creates subdirectories as needed (e.g., "shaders/effects/")
    virtual Result<void> extractTo(const std::filesystem::path& dir) const = 0;

    // Extract single asset to file
    virtual Result<void> extractAsset(std::string_view name,
                                       const std::filesystem::path& path) const = 0;

    // Get the extraction marker path for version checking
    static std::filesystem::path getExtractionMarker(const std::filesystem::path& cacheDir);

    // Check if assets need extraction (first run or version upgrade)
    static bool needsExtraction(const std::filesystem::path& cacheDir);

protected:
    IncbinAssets() = default;
};

}  // namespace yetty
