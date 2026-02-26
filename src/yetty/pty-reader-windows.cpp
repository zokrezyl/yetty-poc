#include <yetty/pty-reader.h>

#if defined(_WIN32)

#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <thread>
#include <atomic>

namespace yetty {

class PtyReaderWindows : public PtyReader {
public:
    PtyReaderWindows() = default;

    ~PtyReaderWindows() override {
        stop();
    }

    Result<void> init(const PtyConfig& config) {
        _shell = config.shell;
        _cols = config.cols;
        _rows = config.rows;

        // Create pipes for PTY input/output
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

        if (!CreatePipe(&_pipeInRead, &_pipeInWrite, &sa, 0)) {
            return Err<void>("Failed to create input pipe");
        }
        if (!CreatePipe(&_pipeOutRead, &_pipeOutWrite, &sa, 0)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            return Err<void>("Failed to create output pipe");
        }

        // Create pseudo console (ConPTY)
        COORD size = { static_cast<SHORT>(_cols), static_cast<SHORT>(_rows) };
        HRESULT hr = CreatePseudoConsole(size, _pipeInRead, _pipeOutWrite, 0, &_hPC);
        if (FAILED(hr)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to create pseudo console");
        }

        // Set up startup info with pseudo console
        STARTUPINFOEXW siEx = {};
        siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);

        SIZE_T attrListSize = 0;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
        siEx.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(HeapAlloc(GetProcessHeap(), 0, attrListSize));

        if (!siEx.lpAttributeList) {
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to allocate attribute list");
        }

        if (!InitializeProcThreadAttributeList(siEx.lpAttributeList, 1, 0, &attrListSize)) {
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to init attribute list");
        }

        if (!UpdateProcThreadAttribute(siEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
                                        _hPC, sizeof(_hPC), nullptr, nullptr)) {
            DeleteProcThreadAttributeList(siEx.lpAttributeList);
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to update attribute list");
        }

        // Convert shell path to wide string
        std::wstring wShell(_shell.begin(), _shell.end());

        // Create process
        PROCESS_INFORMATION pi = {};
        if (!CreateProcessW(nullptr, wShell.data(), nullptr, nullptr, FALSE,
                           EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                           &siEx.StartupInfo, &pi)) {
            DeleteProcThreadAttributeList(siEx.lpAttributeList);
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
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

        // Start reader thread
        _readerThread = std::thread([this]() { readerThreadFunc(); });

        yinfo("PtyReaderWindows: Started ConPTY, shell={}", _shell);
        return Ok();
    }

    size_t read(char* buf, size_t maxLen) override {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        size_t toRead = std::min(maxLen, _readBuffer.size());
        if (toRead > 0) {
            std::memcpy(buf, _readBuffer.data(), toRead);
            _readBuffer.erase(_readBuffer.begin(), _readBuffer.begin() + toRead);
        }
        return toRead;
    }

    void write(const char* data, size_t len) override {
        if (_pipeInWrite != INVALID_HANDLE_VALUE && len > 0) {
            DWORD written;
            WriteFile(_pipeInWrite, data, static_cast<DWORD>(len), &written, nullptr);
        }
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;
        if (_hPC) {
            COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
            ResizePseudoConsole(_hPC, size);
        }
    }

    bool isRunning() const override {
        return _running;
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        yinfo("PtyReaderWindows: Stopping");

        if (_hPC) {
            ClosePseudoConsole(_hPC);
            _hPC = nullptr;
        }

        if (_pipeInWrite != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeInWrite);
            _pipeInWrite = INVALID_HANDLE_VALUE;
        }
        if (_pipeOutRead != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeOutRead);
            _pipeOutRead = INVALID_HANDLE_VALUE;
        }

        if (_readerThread.joinable()) {
            _readerThread.join();
        }

        if (_hProcess != INVALID_HANDLE_VALUE) {
            DWORD exitCode = 0;
            GetExitCodeProcess(_hProcess, &exitCode);
            CloseHandle(_hProcess);
            _hProcess = INVALID_HANDLE_VALUE;
            if (_exitCallback) {
                _exitCallback(static_cast<int>(exitCode));
            }
        }
    }

    void setDataAvailableCallback(DataAvailableCallback cb) override {
        _dataAvailableCallback = std::move(cb);
    }

    void setExitCallback(ExitCallback cb) override {
        _exitCallback = std::move(cb);
    }

private:
    void readerThreadFunc() {
        char buf[4096];
        while (_running) {
            DWORD bytesRead = 0;
            if (ReadFile(_pipeOutRead, buf, sizeof(buf), &bytesRead, nullptr) && bytesRead > 0) {
                {
                    std::lock_guard<std::mutex> lock(_bufferMutex);
                    _readBuffer.insert(_readBuffer.end(), buf, buf + bytesRead);
                }
                if (_dataAvailableCallback) {
                    _dataAvailableCallback();
                }
            } else {
                // Check if process exited
                DWORD exitCode;
                if (GetExitCodeProcess(_hProcess, &exitCode) && exitCode != STILL_ACTIVE) {
                    _running = false;
                    if (_exitCallback) {
                        _exitCallback(static_cast<int>(exitCode));
                    }
                    break;
                }
                Sleep(10);
            }
        }
    }

    HPCON _hPC = nullptr;
    HANDLE _hProcess = INVALID_HANDLE_VALUE;
    HANDLE _pipeInRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeInWrite = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutWrite = INVALID_HANDLE_VALUE;

    uint32_t _cols = 80;
    uint32_t _rows = 24;
    std::string _shell;
    std::atomic<bool> _running{false};

    std::thread _readerThread;
    std::mutex _bufferMutex;
    std::vector<char> _readBuffer;

    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

// Factory implementation for Windows
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config) {
    auto reader = std::make_shared<PtyReaderWindows>();
    if (auto res = reader->init(config); !res) {
        return Err<Ptr>("Failed to initialize PtyReaderWindows", res);
    }
    return Ok<Ptr>(reader);
}

} // namespace yetty

#endif // _WIN32
