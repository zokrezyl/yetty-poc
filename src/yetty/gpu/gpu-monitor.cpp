#include "gpu-monitor.h"

#ifndef _WIN32
#include "nvidia-monitor.h"
#include "amd-monitor.h"
#include "intel-monitor.h"
#endif

#include <ytrace/ytrace.hpp>
#include <filesystem>
#include <fstream>

namespace yetty::gpu {

namespace fs = std::filesystem;

#ifndef _WIN32
// PCI vendor IDs
constexpr uint16_t VENDOR_AMD = 0x1002;
constexpr uint16_t VENDOR_INTEL = 0x8086;

// Read PCI vendor ID from sysfs
static uint16_t readPciVendor(const fs::path& cardPath) {
    auto vendorPath = cardPath / "device" / "vendor";
    std::ifstream file(vendorPath);
    if (!file) return 0;

    std::string vendorStr;
    file >> vendorStr;

    // Parse hex string (e.g., "0x1002")
    try {
        return static_cast<uint16_t>(std::stoul(vendorStr, nullptr, 16));
    } catch (...) {
        return 0;
    }
}

// Read device name from sysfs
static std::string readDeviceName(const fs::path& cardPath) {
    // Try to read product name from PCI
    auto productPath = cardPath / "device" / "label";
    std::ifstream file(productPath);
    if (file) {
        std::string name;
        std::getline(file, name);
        if (!name.empty()) return name;
    }

    // Fallback to device ID
    auto devicePath = cardPath / "device" / "device";
    std::ifstream deviceFile(devicePath);
    if (deviceFile) {
        std::string deviceId;
        deviceFile >> deviceId;
        return "GPU " + deviceId;
    }

    return "Unknown GPU";
}
#endif

GpuMonitor::Ptr GpuMonitor::create() {
#ifdef _WIN32
    // GPU monitoring not yet implemented on Windows
    ywarn("GPU monitor: not supported on Windows");
    return std::make_shared<NullMonitor>();
#else
    yinfo("GPU monitor: detecting GPU vendor...");

    // 1. Try NVIDIA first (uses dlopen, independent of DRM)
    if (auto nvidia = NvidiaMonitor::tryCreate()) {
        yinfo("GPU monitor: NVIDIA GPU detected via NVML");
        return nvidia;
    }

    // 2. Scan DRM cards for AMD/Intel
    const fs::path drmPath = "/sys/class/drm";
    if (!fs::exists(drmPath)) {
        ywarn("GPU monitor: /sys/class/drm not found");
        return std::make_shared<NullMonitor>();
    }

    for (const auto& entry : fs::directory_iterator(drmPath)) {
        auto cardName = entry.path().filename().string();

        // Only look at card* entries (not renderD*, not card*-*)
        if (cardName.find("card") != 0) continue;
        if (cardName.find('-') != std::string::npos) continue;

        auto cardPath = entry.path();
        uint16_t vendor = readPciVendor(cardPath);

        if (vendor == VENDOR_AMD) {
            if (auto amd = AmdMonitor::tryCreate(cardPath)) {
                yinfo("GPU monitor: AMD GPU detected at {}", cardPath.string());
                return amd;
            }
        } else if (vendor == VENDOR_INTEL) {
            if (auto intel = IntelMonitor::tryCreate(cardPath)) {
                yinfo("GPU monitor: Intel GPU detected at {}", cardPath.string());
                return intel;
            }
        }
    }

    ywarn("GPU monitor: no supported GPU found");
    return std::make_shared<NullMonitor>();
#endif
}

} // namespace yetty::gpu
