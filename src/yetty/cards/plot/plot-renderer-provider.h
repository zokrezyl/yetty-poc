#pragma once

#include <yetty/shader-provider.h>
#include <yetty/shader-manager.h>
#include <memory>
#include <map>
#include <mutex>
#include <atomic>

namespace yetty::card {

/**
 * PlotRendererProvider - singleton that provides render_N functions.
 * 
 * Renderers draw transformed values using different styles:
 * - Line
 * - Bar
 * - Scatter
 * - Area
 * 
 * Base renderers are built-in. Custom renderers can be registered.
 */
class PlotRendererProvider : public ShaderProvider,
                             public std::enable_shared_from_this<PlotRendererProvider> {
public:
    enum class Style : uint8_t {
        LINE = 0,
        BAR = 1,
        SCATTER = 2,
        AREA = 3,
    };

    static std::shared_ptr<PlotRendererProvider> instance();

    // Get renderer index for a style (built-in styles have fixed indices)
    uint32_t getRendererIndex(Style style) const;

    // Register custom renderer, returns rendererIndex
    uint32_t registerRenderer(const std::string& name, const std::string& wgslCode);
    void unregisterRenderer(uint32_t rendererIndex);

    // Register with ShaderManager (call once at startup)
    void registerWith(std::shared_ptr<ShaderManager> shaderMgr);

    // ShaderProvider interface
    std::string getCode() const override;
    std::string getDispatchCode() const override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }
    uint32_t getFunctionCount() const override;

private:
    PlotRendererProvider();

    struct Entry {
        std::string name;
        std::string wgslCode;
        bool builtin;
    };

    mutable std::mutex _mutex;
    std::map<uint32_t, Entry> _renderers;
    std::atomic<uint32_t> _nextIndex{4};  // 0-3 reserved for built-in
    std::weak_ptr<ShaderManager> _shaderMgr;
    bool _registered = false;
    mutable bool _dirty = false;

    void initBuiltinRenderers();
};

} // namespace yetty::card
