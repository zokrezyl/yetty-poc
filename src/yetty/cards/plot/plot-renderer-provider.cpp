#include "plot-renderer-provider.h"
#include <ytrace/ytrace.hpp>
#include <sstream>

namespace yetty::card {

std::shared_ptr<PlotRendererProvider> PlotRendererProvider::instance() {
    static auto inst = std::shared_ptr<PlotRendererProvider>(new PlotRendererProvider());
    return inst;
}

PlotRendererProvider::PlotRendererProvider() {
    initBuiltinRenderers();
}

void PlotRendererProvider::initBuiltinRenderers() {
    // Line renderer (index 0)
    _renderers[0] = {"line", R"(
    let lineWidth = 0.025;
    let dist = abs(plotY - value);
    if (dist < lineWidth) {
        let alpha = 1.0 - dist / lineWidth;
        return mix(color, lineColor, alpha);
    }
    return color;
)", true};

    // Bar renderer (index 1)
    _renderers[1] = {"bar", R"(
    let barWidth = 0.8 / f32(dataCount);
    let barCenter = (f32(dataIndex) + 0.5) / f32(dataCount);
    let barLeft = barCenter - barWidth * 0.5;
    let barRight = barCenter + barWidth * 0.5;
    if (zoomedUV.x >= barLeft && zoomedUV.x <= barRight && plotY <= value) {
        var c = fillColor;
        if (abs(zoomedUV.x - barLeft) < 0.01 || abs(zoomedUV.x - barRight) < 0.01 ||
            abs(plotY - value) < 0.015) {
            c = lineColor;
        }
        return c;
    }
    return color;
)", true};

    // Scatter renderer (index 2)
    _renderers[2] = {"scatter", R"(
    let pointRadius = 0.04;
    let pointX = zoomedUV.x;
    let dist = distance(vec2<f32>(pointX, plotY), vec2<f32>(pointX, value));
    if (dist < pointRadius) {
        let alpha = 1.0 - dist / pointRadius;
        return mix(color, lineColor, alpha * alpha);
    }
    return color;
)", true};

    // Area renderer (index 3)
    _renderers[3] = {"area", R"(
    var c = color;
    if (plotY <= value) {
        c = fillColor;
    }
    let lineWidth = 0.02;
    let dist = abs(plotY - value);
    if (dist < lineWidth) {
        let alpha = 1.0 - dist / lineWidth;
        c = mix(c, lineColor, alpha);
    }
    return c;
)", true};
}

uint32_t PlotRendererProvider::getRendererIndex(Style style) const {
    return static_cast<uint32_t>(style);
}

uint32_t PlotRendererProvider::registerRenderer(const std::string& name, const std::string& wgslCode) {
    std::lock_guard<std::mutex> lock(_mutex);
    uint32_t idx = _nextIndex.fetch_add(1);
    _renderers[idx] = {name, wgslCode, false};
    _dirty = true;
    yinfo("PlotRendererProvider: registered renderer[{}] = '{}'", idx, name);
    return idx;
}

void PlotRendererProvider::unregisterRenderer(uint32_t rendererIndex) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _renderers.find(rendererIndex);
    if (it != _renderers.end() && !it->second.builtin) {
        _renderers.erase(it);
        _dirty = true;
        yinfo("PlotRendererProvider: unregistered renderer[{}]", rendererIndex);
    }
}

void PlotRendererProvider::registerWith(std::shared_ptr<ShaderManager> shaderMgr) {
    if (_registered || !shaderMgr) return;
    _shaderMgr = shaderMgr;
    shaderMgr->addProvider(shared_from_this(), "plotRenderDispatch");
    _registered = true;
    _dirty = true;
    yinfo("PlotRendererProvider: registered with ShaderManager");
}

std::string PlotRendererProvider::getCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    for (const auto& [idx, entry] : _renderers) {
        ss << "// Renderer [" << idx << "]: " << entry.name << "\n";
        ss << "fn render_" << idx << "(value: f32, plotY: f32, zoomedUV: vec2<f32>, dataIndex: u32, dataCount: u32, color: vec3<f32>, lineColor: vec3<f32>, fillColor: vec3<f32>) -> vec3<f32> {\n";
        ss << entry.wgslCode << "\n";
        ss << "}\n\n";
    }
    
    return ss.str();
}

std::string PlotRendererProvider::getDispatchCode() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream ss;
    
    bool first = true;
    for (const auto& [idx, entry] : _renderers) {
        if (!first) ss << " else ";
        first = false;
        ss << "if (renderIndex == " << idx << "u) {\n";
        ss << "        return render_" << idx << "(value, plotY, zoomedUV, dataIndex, dataCount, color, lineColor, fillColor);\n";
        ss << "    }";
    }
    
    return ss.str();
}

uint32_t PlotRendererProvider::getFunctionCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return static_cast<uint32_t>(_renderers.size());
}

} // namespace yetty::card
