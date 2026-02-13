#include "amd-monitor.h"
#include <ytrace/ytrace.hpp>
#include <fstream>

namespace yetty::gpu {

namespace fs = std::filesystem;

AmdMonitor::AmdMonitor(const fs::path& cardPath, const std::string& deviceName)
    : _gpuBusyPath(cardPath / "device" / "gpu_busy_percent")
    , _deviceName(deviceName) {}

std::shared_ptr<AmdMonitor> AmdMonitor::tryCreate(const fs::path& cardPath) {
    auto gpuBusyPath = cardPath / "device" / "gpu_busy_percent";

    // Check if gpu_busy_percent exists (AMD-specific)
    if (!fs::exists(gpuBusyPath)) {
        ytrace("AMD monitor: {} not found", gpuBusyPath.string());
        return nullptr;
    }

    // Try to read device name
    std::string deviceName = "AMD GPU";

    // Try product name
    auto productPath = cardPath / "device" / "product_name";
    if (fs::exists(productPath)) {
        std::ifstream file(productPath);
        if (file) {
            std::getline(file, deviceName);
        }
    }

    // Fallback: try to read from uevent
    if (deviceName == "AMD GPU") {
        auto ueventPath = cardPath / "device" / "uevent";
        std::ifstream uevent(ueventPath);
        std::string line;
        while (std::getline(uevent, line)) {
            if (line.find("PCI_SLOT_NAME=") == 0) {
                deviceName = "AMD GPU @ " + line.substr(14);
                break;
            }
        }
    }

    yinfo("AMD monitor: found GPU at {}", cardPath.string());
    return std::shared_ptr<AmdMonitor>(new AmdMonitor(cardPath, deviceName));
}

GpuStats AmdMonitor::sample() {
    GpuStats stats;
    stats.vendor = "amd";
    stats.deviceName = _deviceName;

    std::ifstream file(_gpuBusyPath);
    if (!file) {
        stats.available = false;
        return stats;
    }

    int percent = 0;
    file >> percent;

    if (file.fail()) {
        stats.available = false;
        return stats;
    }

    stats.utilization = static_cast<float>(percent);
    stats.available = true;
    return stats;
}

} // namespace yetty::gpu
