#pragma once

#include <yetty/shader-provider.h>
#include <yetty/shader-manager.h>
#include "../../yast/yast.h"
#include <memory>
#include <map>
#include <mutex>
#include <atomic>

namespace yetty::card {

/**
 * PlotTransformerProvider - singleton that provides transform_N functions.
 * 
 * Transformers compute output values from sampled inputs.
 * Generated from expressions: sin(x) -> return sin(s0);
 * 
 * Signature: transform_N(x: f32, t: f32, s0..s7: f32) -> f32
 */
class PlotTransformerProvider : public ShaderProvider,
                                public std::enable_shared_from_this<PlotTransformerProvider> {
public:
    static std::shared_ptr<PlotTransformerProvider> instance();

    // Register transform expression, returns transformIndex
    uint32_t registerTransform(const std::string& expression, const yast::WGSLGenerator::Result& wgsl);
    void unregisterTransform(uint32_t transformIndex);

    // Register with ShaderManager (call once at startup)
    void registerWith(std::shared_ptr<ShaderManager> shaderMgr);

    // ShaderProvider interface
    std::string getCode() const override;
    std::string getDispatchCode() const override;
    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }
    uint32_t getFunctionCount() const override;

private:
    PlotTransformerProvider() = default;

    struct Entry {
        std::string expression;
        std::string wgslCode;
        bool usesTime;
    };

    mutable std::mutex _mutex;
    std::map<uint32_t, Entry> _transforms;
    std::atomic<uint32_t> _nextIndex{0};
    std::weak_ptr<ShaderManager> _shaderMgr;
    bool _registered = false;
    mutable bool _dirty = false;
};

} // namespace yetty::card
