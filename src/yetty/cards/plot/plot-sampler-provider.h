#pragma once

#include <yetty/shader-provider.h>
#include <yetty/shader-manager.h>
#include <memory>
#include <map>
#include <mutex>
#include <atomic>

namespace yetty::card {

/**
 * PlotSamplerProvider - singleton that provides sample_N functions.
 * 
 * Samplers read input values from various sources:
 * - Domain x value
 * - Time uniform
 * - Buffer values (@b1..@b8)
 * 
 * Each plot card registers its sampler configuration and gets a samplerIndex.
 */
class PlotSamplerProvider : public ShaderProvider,
                            public std::enable_shared_from_this<PlotSamplerProvider> {
public:
    enum class Source : uint8_t {
        DOMAIN_X = 0,   // x from domain range
        TIME = 1,       // time uniform
        BUFFER1 = 2,    // cardStorage buffer 1
        BUFFER2 = 3,
        BUFFER3 = 4,
        BUFFER4 = 5,
        BUFFER5 = 6,
        BUFFER6 = 7,
        BUFFER7 = 8,
        BUFFER8 = 9,
        CONSTANT = 10,  // constant value
    };

    struct Config {
        Source sources[8] = {Source::DOMAIN_X};
        float constants[8] = {0.0f};
    };

    static std::shared_ptr<PlotSamplerProvider> instance();

    // Register sampler config, returns samplerIndex
    uint32_t registerSampler(const Config& config);
    void unregisterSampler(uint32_t samplerIndex);

    // Register with ShaderManager (call once at startup)
    void registerWith(std::shared_ptr<ShaderManager> shaderMgr);

    // ShaderProvider interface
    std::string getCode() const override;
    std::string getDispatchCode() const override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }
    uint32_t getFunctionCount() const override;

private:
    PlotSamplerProvider() = default;

    mutable std::mutex _mutex;
    std::map<uint32_t, Config> _samplers;
    std::atomic<uint32_t> _nextIndex{0};
    std::weak_ptr<ShaderManager> _shaderMgr;
    bool _registered = false;
    mutable bool _dirty = false;
};

} // namespace yetty::card
