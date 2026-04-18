#pragma once

#include "gpu-monitor.h"
#include <filesystem>
#include <chrono>

namespace yetty::gpu {

// Intel GPU monitor using sysfs
// Uses RC6 residency counters to calculate actual GPU busy percentage
class IntelMonitor : public GpuMonitor {
public:
    // Try to create an Intel monitor for a specific DRM card path.
    // Returns nullptr if not an Intel GPU.
    static std::shared_ptr<IntelMonitor> tryCreate(const std::filesystem::path& cardPath);

    GpuStats sample() override;
    std::string vendorName() const override { return "intel"; }

private:
    explicit IntelMonitor(const std::filesystem::path& cardPath, const std::string& deviceName);

    // Read RC6 residency in milliseconds (time spent idle)
    bool readRc6Residency(uint64_t& rc6Ms, uint64_t& totalMs);

    std::filesystem::path _cardPath;
    std::string _deviceName;

    // For delta-based RC6 calculation
    std::chrono::steady_clock::time_point _lastSampleTime;
    uint64_t _lastRc6Ms = 0;
    bool _firstSample = true;
};

} // namespace yetty::gpu
