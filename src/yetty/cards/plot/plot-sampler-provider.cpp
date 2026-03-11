#include "plot-sampler-provider.h"
#include <ytrace/ytrace.hpp>
#include <sstream>

namespace yetty::card {

std::shared_ptr<PlotSamplerProvider> PlotSamplerProvider::instance() {
    static auto inst = std::shared_ptr<PlotSamplerProvider>(new PlotSamplerProvider());
    return inst;
}

uint32_t PlotSamplerProvider::registerSampler(const Config& config) {
    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t idx = _nextIndex.fetch_add(1);
    _samplers[idx] = config;
    _dirty = true;
    yinfo("PlotSamplerProvider: registered sampler[{}]", idx);
    return idx;
}

void PlotSamplerProvider::unregisterSampler(uint32_t samplerIndex) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_samplers.erase(samplerIndex) > 0) {
        _dirty = true;
        yinfo("PlotSamplerProvider: unregistered sampler[{}]", samplerIndex);
    }
}

void PlotSamplerProvider::registerWith(std::shared_ptr<ShaderManager> shaderMgr) {
    if (_registered || !shaderMgr) return;
    _shaderMgr = shaderMgr;
    shaderMgr->addProvider(shared_from_this(), "plotSampleDispatch");
    _registered = true;
    yinfo("PlotSamplerProvider: registered with ShaderManager");
}

std::string PlotSamplerProvider::getCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    for (const auto& [idx, config] : _samplers) {
        ss << "// Sampler [" << idx << "]\n";
        ss << "fn sample_" << idx << "(x: f32, t: f32, idx: u32, dataOffset: u32) -> array<f32, 8> {\n";
        ss << "    var s: array<f32, 8>;\n";
        
        for (int i = 0; i < 8; i++) {
            ss << "    s[" << i << "] = ";
            switch (config.sources[i]) {
                case Source::DOMAIN_X:
                    ss << "x";
                    break;
                case Source::TIME:
                    ss << "t";
                    break;
                case Source::BUFFER1:
                case Source::BUFFER2:
                case Source::BUFFER3:
                case Source::BUFFER4:
                case Source::BUFFER5:
                case Source::BUFFER6:
                case Source::BUFFER7:
                case Source::BUFFER8: {
                    int bufIdx = static_cast<int>(config.sources[i]) - static_cast<int>(Source::BUFFER1);
                    ss << "cardStorage[dataOffset + idx * 8u + " << bufIdx << "u]";
                    break;
                }
                case Source::CONSTANT:
                    ss << config.constants[i] << "f";
                    break;
            }
            ss << ";\n";
        }
        
        ss << "    return s;\n";
        ss << "}\n\n";
    }
    
    return ss.str();
}

std::string PlotSamplerProvider::getDispatchCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    bool first = true;
    for (const auto& [idx, config] : _samplers) {
        if (!first) ss << " else ";
        first = false;
        ss << "if (samplerIndex == " << idx << "u) {\n";
        ss << "        return sample_" << idx << "(x, t, idx, dataOffset);\n";
        ss << "    }";
    }
    
    return ss.str();
}

uint32_t PlotSamplerProvider::getFunctionCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return static_cast<uint32_t>(_samplers.size());
}

} // namespace yetty::card
