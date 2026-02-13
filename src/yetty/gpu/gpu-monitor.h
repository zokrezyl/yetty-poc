#pragma once

// Internal header - extends the public interface with NullMonitor
#include <yetty/gpu-monitor.h>

namespace yetty::gpu {

// Null monitor that always returns unavailable stats
class NullMonitor : public GpuMonitor {
public:
    GpuStats sample() override {
        return GpuStats{0.0f, false, "unknown", "No GPU detected"};
    }
    std::string vendorName() const override { return "unknown"; }
};

} // namespace yetty::gpu
