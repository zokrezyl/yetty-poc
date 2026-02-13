#pragma once

#include "gpu-monitor.h"

// Forward declare NVML types to avoid header dependency in public interface
struct nvmlDevice_st;
typedef struct nvmlDevice_st* nvmlDevice_t;

namespace yetty::gpu {

// NVML function pointer types (matches nvml.h signatures)
using nvmlReturn_t = unsigned int;
using nvmlInit_t = nvmlReturn_t(*)();
using nvmlShutdown_t = nvmlReturn_t(*)();
using nvmlDeviceGetCount_t = nvmlReturn_t(*)(unsigned int*);
using nvmlDeviceGetHandleByIndex_t = nvmlReturn_t(*)(unsigned int, nvmlDevice_t*);
using nvmlDeviceGetName_t = nvmlReturn_t(*)(nvmlDevice_t, char*, unsigned int);
using nvmlDeviceGetUtilizationRates_t = nvmlReturn_t(*)(nvmlDevice_t, void*);

// NVIDIA GPU monitor using NVML (dynamically loaded)
class NvidiaMonitor : public GpuMonitor {
public:
    // Try to create an NVIDIA monitor. Returns nullptr if NVML not available.
    static std::shared_ptr<NvidiaMonitor> tryCreate();

    ~NvidiaMonitor() override;

    GpuStats sample() override;
    std::string vendorName() const override { return "nvidia"; }

private:
    NvidiaMonitor();
    bool initialize();

    void* _nvmlHandle = nullptr;  // dlopen handle
    nvmlDevice_t _device = nullptr;
    std::string _deviceName;

    // NVML function pointers
    nvmlInit_t _nvmlInit = nullptr;
    nvmlShutdown_t _nvmlShutdown = nullptr;
    nvmlDeviceGetCount_t _nvmlDeviceGetCount = nullptr;
    nvmlDeviceGetHandleByIndex_t _nvmlDeviceGetHandleByIndex = nullptr;
    nvmlDeviceGetName_t _nvmlDeviceGetName = nullptr;
    nvmlDeviceGetUtilizationRates_t _nvmlDeviceGetUtilizationRates = nullptr;
};

} // namespace yetty::gpu
