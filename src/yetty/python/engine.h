#pragma once

#include <yetty/result.hpp>
#include <string>

// Forward declare to avoid including pybind11 everywhere
namespace pybind11 { class gil_scoped_acquire; class gil_scoped_release; }

struct _ts;  // PyThreadState

namespace yetty::python {

/// Ref-counted Python interpreter lifecycle (same pattern as ThorVG engine).
/// First ref: create venv if needed, initialize interpreter, release GIL.
/// Last unref: re-acquire GIL, finalize interpreter.
Result<void> engineRef();
void engineUnref();

/// Returns path to the yetty Python venv/packages directory.
/// Uses $XDG_CACHE_HOME/yetty/python or $HOME/.cache/yetty/python.
std::string venvPath();

/// RAII GIL acquire — wraps PyGILState_Ensure / Release.
class GILGuard {
public:
    GILGuard();
    ~GILGuard();
    GILGuard(const GILGuard&) = delete;
    GILGuard& operator=(const GILGuard&) = delete;
private:
    int _state;  // PyGILState_STATE stored as int to avoid Python.h include
};

/// RAII GIL release — wraps PyEval_SaveThread / RestoreThread.
class GILRelease {
public:
    GILRelease();
    ~GILRelease();
    GILRelease(const GILRelease&) = delete;
    GILRelease& operator=(const GILRelease&) = delete;
private:
    _ts* _state;
};

}  // namespace yetty::python
