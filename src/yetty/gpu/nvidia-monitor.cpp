#include "nvidia-monitor.h"
#include <ytrace/ytrace.hpp>
#include <dlfcn.h>
#include <cstring>

namespace yetty::gpu {

// Minimal NVML types and constants (from nvml.h)
// We define these inline to avoid header dependencies
constexpr unsigned int NVML_SUCCESS = 0;
constexpr unsigned int NVML_DEVICE_NAME_BUFFER_SIZE = 96;

struct nvmlUtilization_t {
    unsigned int gpu;
    unsigned int memory;
};

NvidiaMonitor::NvidiaMonitor() = default;

NvidiaMonitor::~NvidiaMonitor() {
    if (_nvmlShutdown) {
        _nvmlShutdown();
    }
    if (_nvmlHandle) {
        dlclose(_nvmlHandle);
    }
}

std::shared_ptr<NvidiaMonitor> NvidiaMonitor::tryCreate() {
    auto monitor = std::shared_ptr<NvidiaMonitor>(new NvidiaMonitor());
    if (monitor->initialize()) {
        return monitor;
    }
    return nullptr;
}

bool NvidiaMonitor::initialize() {
    // Try to load NVML library
    _nvmlHandle = dlopen("libnvidia-ml.so.1", RTLD_LAZY);
    if (!_nvmlHandle) {
        _nvmlHandle = dlopen("libnvidia-ml.so", RTLD_LAZY);
    }
    if (!_nvmlHandle) {
        ytrace("NVML: library not found");
        return false;
    }

    // Load function pointers
    _nvmlInit = reinterpret_cast<nvmlInit_t>(dlsym(_nvmlHandle, "nvmlInit_v2"));
    if (!_nvmlInit) {
        _nvmlInit = reinterpret_cast<nvmlInit_t>(dlsym(_nvmlHandle, "nvmlInit"));
    }
    _nvmlShutdown = reinterpret_cast<nvmlShutdown_t>(dlsym(_nvmlHandle, "nvmlShutdown"));
    _nvmlDeviceGetCount = reinterpret_cast<nvmlDeviceGetCount_t>(dlsym(_nvmlHandle, "nvmlDeviceGetCount_v2"));
    if (!_nvmlDeviceGetCount) {
        _nvmlDeviceGetCount = reinterpret_cast<nvmlDeviceGetCount_t>(dlsym(_nvmlHandle, "nvmlDeviceGetCount"));
    }
    _nvmlDeviceGetHandleByIndex = reinterpret_cast<nvmlDeviceGetHandleByIndex_t>(dlsym(_nvmlHandle, "nvmlDeviceGetHandleByIndex_v2"));
    if (!_nvmlDeviceGetHandleByIndex) {
        _nvmlDeviceGetHandleByIndex = reinterpret_cast<nvmlDeviceGetHandleByIndex_t>(dlsym(_nvmlHandle, "nvmlDeviceGetHandleByIndex"));
    }
    _nvmlDeviceGetName = reinterpret_cast<nvmlDeviceGetName_t>(dlsym(_nvmlHandle, "nvmlDeviceGetName"));
    _nvmlDeviceGetUtilizationRates = reinterpret_cast<nvmlDeviceGetUtilizationRates_t>(dlsym(_nvmlHandle, "nvmlDeviceGetUtilizationRates"));

    if (!_nvmlInit || !_nvmlShutdown || !_nvmlDeviceGetCount ||
        !_nvmlDeviceGetHandleByIndex || !_nvmlDeviceGetUtilizationRates) {
        ytrace("NVML: failed to load required functions");
        dlclose(_nvmlHandle);
        _nvmlHandle = nullptr;
        return false;
    }

    // Initialize NVML
    nvmlReturn_t result = _nvmlInit();
    if (result != NVML_SUCCESS) {
        ytrace("NVML: nvmlInit failed with code {}", result);
        dlclose(_nvmlHandle);
        _nvmlHandle = nullptr;
        return false;
    }

    // Get device count
    unsigned int deviceCount = 0;
    result = _nvmlDeviceGetCount(&deviceCount);
    if (result != NVML_SUCCESS || deviceCount == 0) {
        ytrace("NVML: no devices found");
        _nvmlShutdown();
        dlclose(_nvmlHandle);
        _nvmlHandle = nullptr;
        return false;
    }

    // Get first device handle
    result = _nvmlDeviceGetHandleByIndex(0, &_device);
    if (result != NVML_SUCCESS) {
        ytrace("NVML: failed to get device handle");
        _nvmlShutdown();
        dlclose(_nvmlHandle);
        _nvmlHandle = nullptr;
        return false;
    }

    // Get device name
    if (_nvmlDeviceGetName) {
        char name[NVML_DEVICE_NAME_BUFFER_SIZE];
        if (_nvmlDeviceGetName(_device, name, sizeof(name)) == NVML_SUCCESS) {
            _deviceName = name;
        }
    }
    if (_deviceName.empty()) {
        _deviceName = "NVIDIA GPU";
    }

    yinfo("NVML: initialized, device: {}", _deviceName);
    return true;
}

GpuStats NvidiaMonitor::sample() {
    GpuStats stats;
    stats.vendor = "nvidia";
    stats.deviceName = _deviceName;

    if (!_device || !_nvmlDeviceGetUtilizationRates) {
        stats.available = false;
        return stats;
    }

    nvmlUtilization_t utilization;
    nvmlReturn_t result = _nvmlDeviceGetUtilizationRates(_device, &utilization);
    if (result == NVML_SUCCESS) {
        stats.utilization = static_cast<float>(utilization.gpu);
        stats.available = true;
    } else {
        stats.available = false;
    }

    return stats;
}

} // namespace yetty::gpu
