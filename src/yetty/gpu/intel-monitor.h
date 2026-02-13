#pragma once

#include "gpu-monitor.h"
#include <filesystem>

namespace yetty::gpu {

// Intel GPU monitor using sysfs/fdinfo
class IntelMonitor : public GpuMonitor {
public:
    // Try to create an Intel monitor for a specific DRM card path.
    // Returns nullptr if not an Intel GPU.
    static std::shared_ptr<IntelMonitor> tryCreate(const std::filesystem::path& cardPath);

    GpuStats sample() override;
    std::string vendorName() const override { return "intel"; }

private:
    explicit IntelMonitor(const std::filesystem::path& cardPath, const std::string& deviceName);

    // Read aggregate GPU busy from all clients
    float readClientsBusy();

    std::filesystem::path _cardPath;
    std::string _deviceName;
};

} // namespace yetty::gpu
