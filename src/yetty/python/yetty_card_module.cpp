#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card-texture-manager.h>
#include <yetty/gpu-context.h>
#include "engine.h"
#include <ytrace/ytrace.hpp>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <filesystem>

namespace py = pybind11;

namespace yetty::python {

// Thread-local pointers, set by the card before each Python callback
static thread_local CardManager* s_cardMgr = nullptr;
static thread_local CardTextureManager* s_textureMgr = nullptr;
static thread_local const GPUContext* s_gpuCtx = nullptr;
static thread_local uint32_t s_slotIndex = 0;

void setCardManager(CardManager* mgr) { s_cardMgr = mgr; }
CardManager* getCardManager() { return s_cardMgr; }

void setCardTextureManager(CardTextureManager* mgr) { s_textureMgr = mgr; }
CardTextureManager* getCardTextureManager() { return s_textureMgr; }

void setGPUContext(const GPUContext* ctx) { s_gpuCtx = ctx; }
const GPUContext* getGPUContext() { return s_gpuCtx; }

void setMetadataSlotIndex(uint32_t idx) { s_slotIndex = idx; }
uint32_t getMetadataSlotIndex() { return s_slotIndex; }

//=============================================================================
// Handle wrappers with buffer protocol
//=============================================================================

struct PyTextureHandle {
    TextureHandle handle;
    std::vector<uint8_t> pixels;  // Local CPU pixel buffer
    uint32_t width = 0;
    uint32_t height = 0;

    bool isValid() const { return handle.isValid(); }
    uint32_t getId() const { return handle.id; }
    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }
};

struct PyBufferHandle {
    BufferHandle handle;

    bool isValid() const { return handle.isValid(); }
    uint32_t offset() const { return handle.offset; }
    uint32_t size() const { return handle.size; }
};

struct PyMetadataHandle {
    MetadataHandle handle;

    bool isValid() const { return handle.isValid(); }
    uint32_t offset() const { return handle.offset; }
    uint32_t size() const { return handle.size; }
};

//=============================================================================
// Embedded module — auto-registered before Py_Initialize
//=============================================================================

PYBIND11_EMBEDDED_MODULE(yetty_card, m) {
    m.doc() = "yetty card buffer access for Python cards";

    // --- GPU handle accessors (for sharing device with pygfx/wgpu-py) ---
    m.def("get_device_handle", []() -> uintptr_t {
        auto* ctx = s_gpuCtx;
        if (!ctx || !ctx->device) throw std::runtime_error("No GPU context set");
        return reinterpret_cast<uintptr_t>(ctx->device);
    }, "Get raw WGPUDevice handle as integer (for wgpu-py FFI)");

    m.def("get_queue_handle", []() -> uintptr_t {
        auto* ctx = s_gpuCtx;
        if (!ctx || !ctx->queue) throw std::runtime_error("No GPU context set");
        return reinterpret_cast<uintptr_t>(ctx->queue);
    }, "Get raw WGPUQueue handle as integer (for wgpu-py FFI)");

    // --- PyTextureHandle with buffer protocol ---
    // Python scripts write pixels into the local buffer, then call write_texture()
    py::class_<PyTextureHandle>(m, "TextureHandle", py::buffer_protocol())
        .def_property_readonly("valid", &PyTextureHandle::isValid)
        .def_property_readonly("id", &PyTextureHandle::getId)
        .def_property_readonly("width", &PyTextureHandle::getWidth)
        .def_property_readonly("height", &PyTextureHandle::getHeight)
        .def_buffer([](PyTextureHandle& h) -> py::buffer_info {
            return py::buffer_info(
                h.pixels.data(),
                sizeof(uint8_t),
                py::format_descriptor<uint8_t>::format(),
                1,
                { static_cast<py::ssize_t>(h.pixels.size()) },
                { sizeof(uint8_t) }
            );
        });

    // --- PyBufferHandle with buffer protocol (float access) ---
    py::class_<PyBufferHandle>(m, "BufferHandle", py::buffer_protocol())
        .def_property_readonly("valid", &PyBufferHandle::isValid)
        .def_property_readonly("offset", &PyBufferHandle::offset)
        .def_property_readonly("size", &PyBufferHandle::size)
        .def_buffer([](PyBufferHandle& h) -> py::buffer_info {
            return py::buffer_info(
                h.handle.data,
                sizeof(float),
                py::format_descriptor<float>::format(),
                1,
                { static_cast<py::ssize_t>(h.handle.size / sizeof(float)) },
                { sizeof(float) }
            );
        });

    // --- PyMetadataHandle (offset/size only, no buffer) ---
    py::class_<PyMetadataHandle>(m, "MetadataHandle")
        .def_property_readonly("valid", &PyMetadataHandle::isValid)
        .def_property_readonly("offset", &PyMetadataHandle::offset)
        .def_property_readonly("size", &PyMetadataHandle::size);

    // --- Texture handle functions ---
    m.def("allocate_texture", [](uint32_t width, uint32_t height) -> PyTextureHandle {
        auto* mgr = s_textureMgr;
        if (!mgr) throw std::runtime_error("No CardTextureManager set");
        auto result = mgr->allocate(width, height);
        if (!result) throw std::runtime_error("allocate failed");
        PyTextureHandle pyh;
        pyh.handle = *result;
        pyh.width = width;
        pyh.height = height;
        pyh.pixels.resize(static_cast<size_t>(width) * height * 4);
        return pyh;
    }, py::arg("width"), py::arg("height"),
       "Allocate texture handle with local pixel buffer (RGBA8, width*height*4 bytes)");

    m.def("write_texture", [](PyTextureHandle& h) {
        auto* mgr = s_textureMgr;
        if (!mgr) throw std::runtime_error("No CardTextureManager set");
        if (!h.isValid()) throw std::runtime_error("Invalid texture handle");
        mgr->write(h.handle, h.pixels.data());
    }, py::arg("handle"),
       "Write the local pixel buffer to the atlas for this handle");

    // --- Buffer functions (renamed from storage) ---
    m.def("allocate_buffer", [](const std::string& scope, uint32_t size) -> PyBufferHandle {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        auto result = mgr->bufferManager()->allocateBuffer(s_slotIndex, scope, size);
        if (!result) throw std::runtime_error("allocateBuffer failed");
        return PyBufferHandle{*result};
    }, py::arg("scope"), py::arg("size"), "Allocate buffer (float data)");

    m.def("mark_buffer_dirty", [](PyBufferHandle& h) {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        mgr->bufferManager()->markBufferDirty(h.handle);
    }, py::arg("handle"), "Mark buffer as dirty for GPU upload");

    // Backward compat aliases
    m.def("allocate_storage", [](const std::string& scope, uint32_t size) -> PyBufferHandle {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        auto result = mgr->bufferManager()->allocateBuffer(s_slotIndex, scope, size);
        if (!result) throw std::runtime_error("allocateBuffer failed");
        return PyBufferHandle{*result};
    }, py::arg("scope"), py::arg("size"), "Allocate storage buffer (deprecated, use allocate_buffer)");

    m.def("mark_storage_dirty", [](PyBufferHandle& h) {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        mgr->bufferManager()->markBufferDirty(h.handle);
    }, py::arg("handle"), "Mark storage as dirty (deprecated, use mark_buffer_dirty)");

    // --- Metadata functions ---
    m.def("allocate_metadata", [](uint32_t size) -> PyMetadataHandle {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        auto result = mgr->allocateMetadata(size);
        if (!result) throw std::runtime_error("allocateMetadata failed");
        return PyMetadataHandle{*result};
    }, py::arg("size"), "Allocate metadata slot");

    m.def("write_metadata", [](PyMetadataHandle& h, py::bytes data) {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        std::string_view sv = data;
        auto result = mgr->writeMetadata(h.handle, sv.data(), static_cast<uint32_t>(sv.size()));
        if (!result) throw std::runtime_error("writeMetadata failed");
    }, py::arg("handle"), py::arg("data"), "Write raw bytes to metadata slot");

    m.def("write_metadata_at", [](PyMetadataHandle& h, uint32_t offset, py::bytes data) {
        auto* mgr = s_cardMgr;
        if (!mgr) throw std::runtime_error("No CardManager set");
        std::string_view sv = data;
        auto result = mgr->writeMetadataAt(h.handle, offset, sv.data(), static_cast<uint32_t>(sv.size()));
        if (!result) throw std::runtime_error("writeMetadataAt failed");
    }, py::arg("handle"), py::arg("offset"), py::arg("data"),
       "Write raw bytes at offset within metadata slot");

    // --- Package management ---
    m.def("ensure_packages", [](std::vector<std::string> packages) {
        namespace fs = std::filesystem;

        // Try importing each package, collect missing ones
        py::module_ importlib = py::module_::import("importlib");
        std::vector<std::string> missing;
        for (const auto& pkg : packages) {
            try {
                importlib.attr("import_module")(pkg);
            } catch (py::error_already_set&) {
                missing.push_back(pkg);
            }
        }

        if (missing.empty()) return;

        // Use the embedded Python's pip with --target= to ensure ABI match
        std::string vp = venvPath();
        std::string targetDir = vp + "/packages";
        fs::create_directories(targetDir);

        std::string pip = std::string(CMAKE_BINARY_DIR) + "/python/install/bin/python3 -m pip";

        std::string cmd = pip + " install --target=" + targetDir + " --quiet";
        std::string pkgList;
        for (const auto& pkg : missing) {
            cmd += " " + pkg;
            pkgList += (pkgList.empty() ? "" : ", ") + pkg;
        }
        cmd += " 2>&1";

        yinfo("yetty_card: installing missing packages: {}", pkgList);

        // Release GIL while pip runs (it can take a while)
        int ret;
        {
            py::gil_scoped_release release;
            ret = std::system(cmd.c_str());
        }

        if (ret != 0) {
            throw std::runtime_error("pip install failed for: " + pkgList);
        }

        yinfo("yetty_card: packages installed successfully");

        // Add target dir to sys.path so imports find the new packages
        py::module_::import("sys").attr("path").cast<py::list>().insert(0, targetDir);

        // Reimport to verify
        for (const auto& pkg : missing) {
            try {
                importlib.attr("import_module")(pkg);
            } catch (py::error_already_set& e) {
                throw std::runtime_error("Failed to import '" + pkg + "' after install: " + std::string(e.what()));
            }
        }
    }, py::arg("packages"),
       "Install packages via pip if not already importable. Call at top of init().");
}

}  // namespace yetty::python
