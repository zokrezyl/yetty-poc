#include "intel-monitor.h"
#include <ytrace/ytrace.hpp>
#include <fstream>
#include <sstream>

namespace yetty::gpu {

namespace fs = std::filesystem;

IntelMonitor::IntelMonitor(const fs::path& cardPath, const std::string& deviceName)
    : _cardPath(cardPath)
    , _deviceName(deviceName) {}

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

float IntelMonitor::readClientsBusy() {
    // Try to read from clients/* (kernel 5.19+)
    auto clientsPath = _cardPath / "clients";
    if (!fs::exists(clientsPath)) {
        return 0.0f;
    }

    float totalBusy = 0.0f;
    int clientCount = 0;

    for (const auto& entry : fs::directory_iterator(clientsPath)) {
        auto busyPath = entry.path() / "busy";
        if (!fs::exists(busyPath)) continue;

        std::ifstream file(busyPath);
        if (!file) continue;

        // Format: "render: X ns\nvideo: Y ns\n..."
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("render:") == 0 || line.find("rcs0:") == 0) {
                // Parse percentage or nanoseconds
                std::istringstream iss(line);
                std::string label;
                float value;
                iss >> label >> value;
                totalBusy += value;
                clientCount++;
            }
        }
    }

    // Return average if we found clients
    if (clientCount > 0) {
        // Note: this is a rough approximation since values are cumulative ns
        // For a proper implementation, we'd track deltas over time
        return totalBusy / static_cast<float>(clientCount);
    }

    return 0.0f;
}

GpuStats IntelMonitor::sample() {
    GpuStats stats;
    stats.vendor = "intel";
    stats.deviceName = _deviceName;

    // Try the gt/gt0/rps_cur_freq_mhz approach for frequency-based estimation
    // This isn't true utilization but indicates GPU activity
    auto freqPath = _cardPath / "gt" / "gt0" / "rps_cur_freq_mhz";
    auto maxFreqPath = _cardPath / "gt" / "gt0" / "rps_max_freq_mhz";

    if (fs::exists(freqPath) && fs::exists(maxFreqPath)) {
        std::ifstream curFile(freqPath);
        std::ifstream maxFile(maxFreqPath);

        int curFreq = 0, maxFreq = 0;
        curFile >> curFreq;
        maxFile >> maxFreq;

        if (maxFreq > 0) {
            // Estimate utilization from frequency scaling
            // This is a rough approximation
            stats.utilization = (static_cast<float>(curFreq) / static_cast<float>(maxFreq)) * 100.0f;
            stats.available = true;
            return stats;
        }
    }

    // Try reading from clients (kernel 5.19+)
    float clientsBusy = readClientsBusy();
    if (clientsBusy > 0) {
        stats.utilization = clientsBusy;
        stats.available = true;
        return stats;
    }

    // Fallback: Intel GPU detected but no utilization available
    // User should use intel_gpu_top for accurate readings
    stats.available = false;
    ytrace("Intel monitor: no utilization source available (try intel_gpu_top)");
    return stats;
}

} // namespace yetty::gpu
