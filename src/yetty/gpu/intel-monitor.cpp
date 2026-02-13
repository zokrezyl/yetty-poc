#include "intel-monitor.h"
#include <ytrace/ytrace.hpp>
#include <fstream>

namespace yetty::gpu {

namespace fs = std::filesystem;

IntelMonitor::IntelMonitor(const fs::path& cardPath, const std::string& deviceName)
    : _cardPath(cardPath)
    , _deviceName(deviceName)
    , _lastSampleTime(std::chrono::steady_clock::now()) {}

std::shared_ptr<IntelMonitor> IntelMonitor::tryCreate(const fs::path& cardPath) {
    // Check if this is an Intel GPU by verifying i915 driver
    auto driverLink = cardPath / "device" / "driver";
    if (fs::exists(driverLink)) {
        auto driverPath = fs::read_symlink(driverLink);
        auto driverName = driverPath.filename().string();
        if (driverName != "i915" && driverName != "xe") {
            ytrace("Intel monitor: driver is {}, not i915/xe", driverName);
            return nullptr;
        }
    }

    // Check if RC6 residency is available (our primary metric)
    auto rc6Path = cardPath / "gt" / "gt0" / "rc6_residency_ms";
    auto rc6PathAlt = cardPath / "power" / "rc6_residency_ms";
    if (!fs::exists(rc6Path) && !fs::exists(rc6PathAlt)) {
        ytrace("Intel monitor: RC6 residency not available at {}", cardPath.string());
        // Still create the monitor, but it will report unavailable
    }

    // Try to read device name
    std::string deviceName = "Intel GPU";

    // Try product name from uevent
    auto ueventPath = cardPath / "device" / "uevent";
    std::ifstream uevent(ueventPath);
    std::string line;
    while (std::getline(uevent, line)) {
        if (line.find("PCI_SLOT_NAME=") == 0) {
            deviceName = "Intel GPU @ " + line.substr(14);
            break;
        }
    }

    yinfo("Intel monitor: found GPU at {}", cardPath.string());
    return std::shared_ptr<IntelMonitor>(new IntelMonitor(cardPath, deviceName));
}

bool IntelMonitor::readRc6Residency(uint64_t& rc6Ms, uint64_t& /*totalMs*/) {
    // Try multiple possible paths for RC6 residency
    std::vector<fs::path> rc6Paths = {
        _cardPath / "gt" / "gt0" / "rc6_residency_ms",
        _cardPath / "power" / "rc6_residency_ms",
    };

    for (const auto& path : rc6Paths) {
        if (!fs::exists(path)) continue;

        std::ifstream file(path);
        if (!file) continue;

        file >> rc6Ms;
        if (!file.fail()) {
            return true;
        }
    }

    return false;
}

GpuStats IntelMonitor::sample() {
    GpuStats stats;
    stats.vendor = "intel";
    stats.deviceName = _deviceName;
    stats.available = false;

    auto now = std::chrono::steady_clock::now();

    // Read current RC6 residency (cumulative idle time in ms)
    uint64_t rc6Ms = 0, totalMs = 0;
    if (!readRc6Residency(rc6Ms, totalMs)) {
        // RC6 not available - can't measure utilization accurately
        ytrace("Intel monitor: RC6 residency read failed");
        return stats;
    }

    if (_firstSample) {
        // First sample - just store baseline, can't calculate yet
        _lastRc6Ms = rc6Ms;
        _lastSampleTime = now;
        _firstSample = false;
        // Return 0% for first sample
        stats.utilization = 0.0f;
        stats.available = true;
        return stats;
    }

    // Calculate delta since last sample
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastSampleTime);
    uint64_t elapsedMs = static_cast<uint64_t>(elapsed.count());

    if (elapsedMs == 0) {
        // Too fast, return last value
        stats.utilization = 0.0f;
        stats.available = true;
        return stats;
    }

    // RC6 is idle time, so busy = elapsed - idle_delta
    uint64_t rc6Delta = rc6Ms - _lastRc6Ms;

    // Clamp rc6Delta to elapsed time (can exceed due to timing)
    if (rc6Delta > elapsedMs) {
        rc6Delta = elapsedMs;
    }

    // Busy percentage = (elapsed - idle) / elapsed * 100
    float busyMs = static_cast<float>(elapsedMs - rc6Delta);
    stats.utilization = (busyMs / static_cast<float>(elapsedMs)) * 100.0f;
    stats.available = true;

    // Clamp to valid range
    if (stats.utilization < 0.0f) stats.utilization = 0.0f;
    if (stats.utilization > 100.0f) stats.utilization = 100.0f;

    // Store for next sample
    _lastRc6Ms = rc6Ms;
    _lastSampleTime = now;

    return stats;
}

} // namespace yetty::gpu
