#pragma once

#include <memory>
#include <string>

namespace yetty::gpu {

struct GpuStats {
    float utilization = 0.0f;   // 0.0 - 100.0 (GPU processor usage %)
    bool available = false;     // true if stats could be read
    std::string vendor;         // "nvidia", "amd", "intel", "unknown"
    std::string deviceName;     // e.g., "NVIDIA GeForce RTX 3080"
};

class GpuMonitor {
public:
    using Ptr = std::shared_ptr<GpuMonitor>;

    // Auto-detect vendor and create appropriate backend
    static Ptr create();

    virtual ~GpuMonitor() = default;

    // Get current GPU stats (call periodically, e.g., once per second)
    virtual GpuStats sample() = 0;

    // Get vendor name for this monitor
    virtual std::string vendorName() const = 0;
};

} // namespace yetty::gpu
