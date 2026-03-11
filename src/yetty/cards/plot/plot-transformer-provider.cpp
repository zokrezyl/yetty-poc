#include "plot-transformer-provider.h"
#include <ytrace/ytrace.hpp>
#include <sstream>

namespace yetty::card {

std::shared_ptr<PlotTransformerProvider> PlotTransformerProvider::instance() {
    static auto inst = std::shared_ptr<PlotTransformerProvider>(new PlotTransformerProvider());
    return inst;
}

uint32_t PlotTransformerProvider::registerTransform(const std::string& expression, const yast::WGSLGenerator::Result& wgsl) {
    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t idx = _nextIndex.fetch_add(1);
    _transforms[idx] = {expression, wgsl.code, wgsl.usesTime};
    _dirty = true;
    yinfo("PlotTransformerProvider: registered transform[{}] = '{}' -> '{}'", idx, expression, wgsl.code);
    return idx;
}

void PlotTransformerProvider::unregisterTransform(uint32_t transformIndex) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_transforms.erase(transformIndex) > 0) {
        _dirty = true;
        yinfo("PlotTransformerProvider: unregistered transform[{}]", transformIndex);
    }
}

void PlotTransformerProvider::registerWith(std::shared_ptr<ShaderManager> shaderMgr) {
    if (_registered || !shaderMgr) return;
    _shaderMgr = shaderMgr;
    shaderMgr->addProvider(shared_from_this(), "plotTransformDispatch");
    _registered = true;
    yinfo("PlotTransformerProvider: registered with ShaderManager");
}

std::string PlotTransformerProvider::getCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    for (const auto& [idx, entry] : _transforms) {
        ss << "// Transform [" << idx << "]: " << entry.expression << "\n";
        ss << "fn transform_" << idx << "(x: f32, t: f32, s0: f32, s1: f32, s2: f32, s3: f32, s4: f32, s5: f32, s6: f32, s7: f32) -> f32 {\n";
        ss << "    let time = t;\n";
        ss << "    return " << entry.wgslCode << ";\n";
        ss << "}\n\n";
    }
    
    return ss.str();
}

std::string PlotTransformerProvider::getDispatchCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    bool first = true;
    for (const auto& [idx, entry] : _transforms) {
        if (!first) ss << " else ";
        first = false;
        ss << "if (transformIndex == " << idx << "u) {\n";
        ss << "        return transform_" << idx << "(x, t, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);\n";
        ss << "    }";
    }
    
    return ss.str();
}

uint32_t PlotTransformerProvider::getFunctionCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return static_cast<uint32_t>(_transforms.size());
}

} // namespace yetty::card
