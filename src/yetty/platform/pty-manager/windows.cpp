#include <yetty/platform/pty-manager.h>

#if defined(_WIN32)

#include <yetty/pty-provider.h>
#include <yetty/base/event-loop.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <thread>
#include <atomic>
#include <vector>

namespace yetty {

static std::atomic<uint32_t> g_nextPtyId{1};

class ConPTY : public PTYProvider {
public:
    ConPTY() : _id(g_nextPtyId++) {}

    ~ConPTY() override {
        stop();
    }

    Result<void> start(const std::string& shell, uint32_t cols, uint32_t rows) override {
        _shell = shell;
        _cols = cols;
        _rows = rows;

        ydebug("ConPTY[{}]: Starting shell '{}' ({}x{})", _id, shell, cols, rows);

        // Create pipes
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

        if (!CreatePipe(&_pipeInRead, &_pipeInWrite, &sa, 0)) {
            return Err<void>("Failed to create input pipe");
        }
        if (!CreatePipe(&_pipeOutRead, &_pipeOutWrite, &sa, 0)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            return Err<void>("Failed to create output pipe");
        }

        // UTF-8 code page
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

        // Create pseudo console
        COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
        HRESULT hr = CreatePseudoConsole(size, _pipeInRead, _pipeOutWrite, 0, &_hPC);
        if (FAILED(hr)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to create pseudo console");
        }

        // Setup startup info
        STARTUPINFOEXW siEx = {};
        siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);

        SIZE_T attrListSize = 0;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
        siEx.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
            HeapAlloc(GetProcessHeap(), 0, attrListSize));

        if (!siEx.lpAttributeList ||
            !InitializeProcThreadAttributeList(siEx.lpAttributeList, 1, 0, &attrListSize) ||
            !UpdateProcThreadAttribute(siEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
                                       _hPC, sizeof(_hPC), nullptr, nullptr)) {
            cleanup();
            return Err<void>("Failed to setup process attributes");
        }

        // Wrap shell with UTF-8 code page
        std::string cmdLine = "cmd.exe /c chcp 65001 >nul && " + shell;
        std::wstring wShell(cmdLine.begin(), cmdLine.end());

        // Create process
        PROCESS_INFORMATION pi = {};
        if (!CreateProcessW(nullptr, wShell.data(), nullptr, nullptr, FALSE,
                           EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                           &siEx.StartupInfo, &pi)) {
            DeleteProcThreadAttributeList(siEx.lpAttributeList);
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            cleanup();
            return Err<void>("Failed to create process");
        }

        _hProcess = pi.hProcess;
        CloseHandle(pi.hThread);

        DeleteProcThreadAttributeList(siEx.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);

        // Close pipe ends we don't need
        CloseHandle(_pipeInRead);
        _pipeInRead = INVALID_HANDLE_VALUE;
        CloseHandle(_pipeOutWrite);
        _pipeOutWrite = INVALID_HANDLE_VALUE;

        _running = true;
        _readerThread = std::thread([this]() { readerThreadFunc(); });

        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        ydebug("ConPTY[{}]: Stopping", _id);

        if (_hPC) {
            ClosePseudoConsole(_hPC);
            _hPC = nullptr;
        }

        if (_readerThread.joinable()) {
            _readerThread.join();
        }

        cleanup();

        if (_exitCallback) {
            _exitCallback(0);
        }
    }

    void write(const char* data, size_t len) override {
        if (!_running || _pipeInWrite == INVALID_HANDLE_VALUE) return;

        DWORD written;
        WriteFile(_pipeInWrite, data, static_cast<DWORD>(len), &written, nullptr);
    }

    void resize(uint32_t cols, uint32_t rows) override {
        if (!_hPC) return;
        _cols = cols;
        _rows = rows;

        COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
        ResizePseudoConsole(_hPC, size);
    }

    bool isRunning() const override { return _running; }
    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

private:
    void readerThreadFunc() {
        std::vector<char> buffer(4096);

        while (_running) {
            DWORD bytesRead = 0;
            if (!ReadFile(_pipeOutRead, buffer.data(), static_cast<DWORD>(buffer.size()),
                         &bytesRead, nullptr)) {
                break;
            }

            if (bytesRead > 0 && _dataCallback) {
                _dataCallback(buffer.data(), bytesRead);
            }
        }
    }

    void cleanup() {
        if (_pipeInRead != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeInRead);
            _pipeInRead = INVALID_HANDLE_VALUE;
        }
        if (_pipeInWrite != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeInWrite);
            _pipeInWrite = INVALID_HANDLE_VALUE;
        }
        if (_pipeOutRead != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeOutRead);
            _pipeOutRead = INVALID_HANDLE_VALUE;
        }
        if (_pipeOutWrite != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeOutWrite);
            _pipeOutWrite = INVALID_HANDLE_VALUE;
        }
        if (_hProcess != INVALID_HANDLE_VALUE) {
            CloseHandle(_hProcess);
            _hProcess = INVALID_HANDLE_VALUE;
        }
    }

    uint32_t _id;
    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _shell;
    std::atomic<bool> _running{false};
    std::thread _readerThread;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;

    HPCON _hPC = nullptr;
    HANDLE _hProcess = INVALID_HANDLE_VALUE;
    HANDLE _pipeInRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeInWrite = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutWrite = INVALID_HANDLE_VALUE;
};

class WindowsPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<ConPTY>()));
    }
};

// Factory
Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new WindowsPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WindowsPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // _WIN32
