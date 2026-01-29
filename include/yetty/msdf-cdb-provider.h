#pragma once

#include <yetty/result.hpp>

#include <webgpu/webgpu.h>

#include <memory>
#include <string>
#include <vector>

namespace yetty {

struct MsdfCdbConfig {
    std::string fontName;                // e.g. "DejaVuSansMNerdFontMono"
    std::vector<std::string> ttfPaths;   // one per style [Regular, Bold, Oblique, BoldOblique]
    std::string outputDir;               // e.g. ~/.cache/yetty/msdf-font-cache
    float fontSize = 32.0f;
    float pixelRange = 4.0f;
};

class MsdfCdbProvider {
public:
    using Ptr = std::shared_ptr<MsdfCdbProvider>;

    virtual ~MsdfCdbProvider() = default;
    virtual std::string name() const = 0;
    virtual Result<void> generate(const MsdfCdbConfig& config) = 0;
};

// CPU provider — uses msdfgen library (multi-threaded)
class CpuMsdfCdbProvider : public MsdfCdbProvider {
public:
    std::string name() const override { return "cpu"; }
    Result<void> generate(const MsdfCdbConfig& config) override;
};

// GPU provider — uses msdf-wgsl compute shader
// Creates its own dedicated device with high buffer size limits
class GpuMsdfCdbProvider : public MsdfCdbProvider {
public:
    explicit GpuMsdfCdbProvider(WGPUInstance instance);

    std::string name() const override { return "gpu"; }
    Result<void> generate(const MsdfCdbConfig& config) override;

private:
    Result<void> initDevice();
    void releaseDevice();

    WGPUInstance _instance;
    WGPUAdapter _adapter = nullptr;
    WGPUDevice _device = nullptr;
};

} // namespace yetty
