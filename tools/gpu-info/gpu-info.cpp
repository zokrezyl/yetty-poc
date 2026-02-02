#include <dawn/native/DawnNative.h>
#include <dawn/webgpu_cpp.h>
#include <iostream>
#include <string>

static std::string toStr(wgpu::StringView sv) {
    if (sv.data && sv.length > 0) {
        return std::string(sv.data, sv.length);
    }
    return "(unknown)";
}

static const char* backendName(wgpu::BackendType type) {
    switch (type) {
        case wgpu::BackendType::Undefined: return "Undefined";
        case wgpu::BackendType::Null:      return "Null";
        case wgpu::BackendType::WebGPU:    return "WebGPU";
        case wgpu::BackendType::D3D11:     return "D3D11";
        case wgpu::BackendType::D3D12:     return "D3D12";
        case wgpu::BackendType::Metal:     return "Metal";
        case wgpu::BackendType::Vulkan:    return "Vulkan";
        case wgpu::BackendType::OpenGL:    return "OpenGL";
        case wgpu::BackendType::OpenGLES:  return "OpenGLES";
        default:                           return "Unknown";
    }
}

static const char* adapterTypeName(wgpu::AdapterType type) {
    switch (type) {
        case wgpu::AdapterType::DiscreteGPU:   return "Discrete GPU";
        case wgpu::AdapterType::IntegratedGPU: return "Integrated GPU";
        case wgpu::AdapterType::CPU:           return "CPU";
        case wgpu::AdapterType::Unknown:       return "Unknown";
        default:                               return "Unknown";
    }
}

int main() {
    dawn::native::Instance instance;

    auto adapters = instance.EnumerateAdapters();
    if (adapters.empty()) {
        std::cerr << "No GPU adapters found!" << std::endl;
        return 1;
    }

    std::cout << "Found " << adapters.size() << " adapter(s):\n\n";

    for (size_t i = 0; i < adapters.size(); ++i) {
        wgpu::Adapter wgpuAdapter(adapters[i].Get());
        wgpu::AdapterInfo info;
        if (wgpuAdapter.GetInfo(&info) != wgpu::Status::Success) {
            std::cerr << "  [" << i << "] Failed to get adapter info\n";
            continue;
        }

        std::cout << "  [" << i << "] " << toStr(info.device) << "\n";
        std::cout << "      Vendor:       " << toStr(info.vendor) << "\n";
        std::cout << "      Architecture: " << toStr(info.architecture) << "\n";
        std::cout << "      Description:  " << toStr(info.description) << "\n";
        std::cout << "      Backend:      " << backendName(info.backendType) << "\n";
        std::cout << "      Type:         " << adapterTypeName(info.adapterType) << "\n";
        std::cout << "      Vendor ID:    0x" << std::hex << info.vendorID << std::dec << "\n";
        std::cout << "      Device ID:    0x" << std::hex << info.deviceID << std::dec << "\n";
        std::cout << "      Subgroup:     " << info.subgroupMinSize << "-" << info.subgroupMaxSize << "\n";
        std::cout << "\n";
    }

    return 0;
}
