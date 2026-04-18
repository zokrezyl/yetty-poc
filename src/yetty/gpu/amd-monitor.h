#pragma once

#include "gpu-monitor.h"
#include <filesystem>

namespace yetty::gpu {

// AMD GPU monitor using sysfs (gpu_busy_percent)
class AmdMonitor : public GpuMonitor {
public:
    // Try to create an AMD monitor for a specific DRM card path.
    // Returns nullptr if not an AMD GPU or sysfs not available.
    static std::shared_ptr<AmdMonitor> tryCreate(const std::filesystem::path& cardPath);

    GpuStats sample() override;
    std::string vendorName() const override { return "amd"; }

private:
    explicit AmdMonitor(const std::filesystem::path& cardPath, const std::string& deviceName);

    std::filesystem::path _gpuBusyPath;
    std::string _deviceName;
};

} // namespace yetty::gpu
