#include "engine.h"
#include <ytrace/ytrace.hpp>
#include <pybind11/embed.h>
#include <filesystem>
#include <cstdlib>

namespace py = pybind11;
namespace fs = std::filesystem;

namespace yetty::python {

//=============================================================================
// Ref-counted Python interpreter
//=============================================================================

static int s_pythonRefCount = 0;
static PyThreadState* s_mainThreadState = nullptr;

std::string venvPath() {
    const char* cacheHome = std::getenv("XDG_CACHE_HOME");
    if (cacheHome && cacheHome[0] != '\0') {
        return std::string(cacheHome) + "/yetty/python";
    }
    const char* home = std::getenv("HOME");
    if (!home) home = "/tmp";
    return std::string(home) + "/.cache/yetty/python";
}

static Result<void> ensureVenv() {
    std::string vp = venvPath();

    // Check if venv already exists (look for pyvenv.cfg or site-packages)
    std::string sitePackages = vp + "/lib";
    if (fs::exists(vp) && fs::exists(sitePackages)) {
        yinfo("Python venv already exists at {}", vp);
        return Ok();
    }

    yinfo("Creating Python venv at {}...", vp);
    fs::create_directories(vp);

    // Use the embedded Python to create a venv
    std::string pythonBin = std::string(CMAKE_BINARY_DIR) + "/python/install/bin/python3";
    std::string cmd = pythonBin + " -m venv " + vp + " 2>&1";

    yinfo("Running: {}", cmd);
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        ywarn("Failed to create venv (exit {}), will use embedded Python directly", ret);
        // Not fatal — we can still run without a venv
    } else {
        yinfo("Python venv created successfully");
    }

    return Ok();
}

static void addVenvSitePackages() {
    std::string vp = venvPath();

    // Find the site-packages directory inside the venv
    // Typical path: venv/lib/python3.XX/site-packages
    std::string libDir = vp + "/lib";
    if (!fs::exists(libDir)) return;

    for (auto& entry : fs::directory_iterator(libDir)) {
        if (entry.is_directory()) {
            std::string sp = entry.path().string() + "/site-packages";
            if (fs::exists(sp)) {
                yinfo("Adding venv site-packages to sys.path: {}", sp);
                py::module_::import("sys").attr("path").cast<py::list>().insert(0, sp);
                return;
            }
        }
    }
}

Result<void> engineRef() {
    if (s_pythonRefCount == 0) {
        yinfo("Python engine: initializing interpreter (refcount=0->1)");

        // Create venv if needed (non-fatal if it fails)
        if (auto res = ensureVenv(); !res) {
            ywarn("Python engine: venv setup failed: {}", error_msg(res));
        }

        // Point Python at the embedded installation so it finds stdlib (math, os, etc.)
        std::string pythonHome = std::string(CMAKE_BINARY_DIR) + "/python/install";
        setenv("PYTHONHOME", pythonHome.c_str(), 1);

        // Initialize the interpreter via pybind11
        // PYBIND11_EMBEDDED_MODULE modules are auto-registered before Py_Initialize
        py::initialize_interpreter();
        yinfo("Python engine: interpreter initialized ({})", Py_GetVersion());

        // Add venv site-packages to sys.path
        addVenvSitePackages();

        // Add packages installed via ensure_packages (--target= dir)
        std::string pkgDir = venvPath() + "/packages";
        if (fs::exists(pkgDir)) {
            yinfo("Adding packages dir to sys.path: {}", pkgDir);
            py::module_::import("sys").attr("path").cast<py::list>().insert(0, pkgDir);
        }

        // Also add embedded Python's site-packages (find version dynamically)
        std::string libDir = std::string(CMAKE_BINARY_DIR) + "/python/install/lib";
        if (fs::exists(libDir)) {
            for (auto& entry : fs::directory_iterator(libDir)) {
                if (entry.is_directory() && entry.path().filename().string().starts_with("python")) {
                    std::string sp = entry.path().string() + "/site-packages";
                    if (fs::exists(sp)) {
                        py::module_::import("sys").attr("path").cast<py::list>().insert(0, sp);
                        break;
                    }
                }
            }
        }

        // Add yetty's built-in Python modules (result.py etc.) to sys.path
        // Located at src/yetty/python/ — the 'python' subpackage lives there
        std::string yettyPyDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/python";
        if (fs::exists(yettyPyDir)) {
            py::module_::import("sys").attr("path").cast<py::list>().insert(0, yettyPyDir);
        }

        // Release the GIL so other threads can acquire it via GILGuard
        s_mainThreadState = PyEval_SaveThread();
        yinfo("Python engine: GIL released after init");
    }

    ++s_pythonRefCount;
    yinfo("Python engine: refcount now {}", s_pythonRefCount);
    return Ok();
}

void engineUnref() {
    if (s_pythonRefCount <= 0) {
        yerror("Python engine: unref called with refcount={}", s_pythonRefCount);
        return;
    }

    --s_pythonRefCount;
    yinfo("Python engine: refcount now {}", s_pythonRefCount);

    if (s_pythonRefCount == 0) {
        yinfo("Python engine: finalizing interpreter");

        // Re-acquire GIL before finalizing
        if (s_mainThreadState) {
            PyEval_RestoreThread(s_mainThreadState);
            s_mainThreadState = nullptr;
        }

        // Finalize — may crash with some Python extensions, so we catch
        try {
            py::finalize_interpreter();
            yinfo("Python engine: interpreter finalized");
        } catch (const std::exception& e) {
            ywarn("Python engine: finalize_interpreter threw: {}", e.what());
        } catch (...) {
            ywarn("Python engine: finalize_interpreter threw unknown exception");
        }
    }
}

//=============================================================================
// GILGuard
//=============================================================================

GILGuard::GILGuard() {
    _state = static_cast<int>(PyGILState_Ensure());
}

GILGuard::~GILGuard() {
    PyGILState_Release(static_cast<PyGILState_STATE>(_state));
}

//=============================================================================
// GILRelease
//=============================================================================

GILRelease::GILRelease() {
    _state = PyEval_SaveThread();
}

GILRelease::~GILRelease() {
    PyEval_RestoreThread(_state);
}

}  // namespace yetty::python
