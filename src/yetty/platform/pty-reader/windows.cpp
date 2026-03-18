#include "windows.h"

#include <ytrace/ytrace.hpp>
#include <process.h>
#include <cstring>

namespace yetty {

PtyReaderWindows::~PtyReaderWindows() {
    stop();
}

Result<void> PtyReaderWindows::init(const PtyConfig& config) {
    _shell = config.shell;
    _command = config.command;
    _cols = config.cols;
    _rows = config.rows;

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

    if (!CreatePipe(&_pipeInRead, &_pipeInWrite, &sa, 0)) {
        return Err<void>("Failed to create input pipe");
    }
    if (!CreatePipe(&_pipeOutRead, &_pipeOutWrite, &sa, 0)) {
        CloseHandle(_pipeInRead);
        CloseHandle(_pipeInWrite);
        return Err<void>("Failed to create output pipe");
    }

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    COORD size = { static_cast<SHORT>(_cols), static_cast<SHORT>(_rows) };
    HRESULT hr = CreatePseudoConsole(size, _pipeInRead, _pipeOutWrite, 0, &_hPC);
    if (FAILED(hr)) {
        CloseHandle(_pipeInRead);
        CloseHandle(_pipeInWrite);
        CloseHandle(_pipeOutRead);
        CloseHandle(_pipeOutWrite);
        return Err<void>("Failed to create pseudo console");
    }

    STARTUPINFOEXW siEx = {};
    siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);
    siEx.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    SIZE_T attrListSize = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
    siEx.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
        HeapAlloc(GetProcessHeap(), 0, attrListSize));

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

    std::string cmdLine;
    if (!_command.empty()) {
        cmdLine = "cmd.exe /c chcp 65001 >nul && " + _command;
    } else {
        cmdLine = "cmd.exe /c chcp 65001 >nul && " + _shell;
    }

    std::wstring wShell(cmdLine.begin(), cmdLine.end());

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

    CloseHandle(_pipeInRead);
    _pipeInRead = INVALID_HANDLE_VALUE;
    CloseHandle(_pipeOutWrite);
    _pipeOutWrite = INVALID_HANDLE_VALUE;

    _running = true;
    _readerThread = std::thread([this]() { readerThreadFunc(); });

    ydebug("PtyReaderWindows: Started ConPTY, shell={}", _shell);
    return Ok();
}

size_t PtyReaderWindows::read(char* buf, size_t maxLen) {
    std::lock_guard<std::mutex> lock(_bufferMutex);
    size_t toRead = std::min(maxLen, _readBuffer.size());
    if (toRead > 0) {
        std::memcpy(buf, _readBuffer.data(), toRead);
        _readBuffer.erase(_readBuffer.begin(), _readBuffer.begin() + toRead);
    }
    return toRead;
}

void PtyReaderWindows::write(const char* data, size_t len) {
    if (_pipeInWrite != INVALID_HANDLE_VALUE && len > 0) {
        DWORD written;
        WriteFile(_pipeInWrite, data, static_cast<DWORD>(len), &written, nullptr);
    }
}

void PtyReaderWindows::resize(uint32_t cols, uint32_t rows) {
    _cols = cols;
    _rows = rows;
    if (_hPC) {
        COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
        ResizePseudoConsole(_hPC, size);
    }
}

bool PtyReaderWindows::isRunning() const {
    return _running;
}

void PtyReaderWindows::stop() {
    if (!_running) return;
    _running = false;

    ydebug("PtyReaderWindows: Stopping");

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

void PtyReaderWindows::setDataAvailableCallback(DataAvailableCallback cb) {
    _dataAvailableCallback = std::move(cb);
}

void PtyReaderWindows::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

void PtyReaderWindows::readerThreadFunc() {
    char buf[4096];
    ydebug("PtyReaderWindows: reader thread started");

    while (_running) {
        DWORD bytesRead = 0;
        BOOL ok = ReadFile(_pipeOutRead, buf, sizeof(buf), &bytesRead, nullptr);
        if (ok && bytesRead > 0) {
            {
                std::lock_guard<std::mutex> lock(_bufferMutex);
                _readBuffer.insert(_readBuffer.end(), buf, buf + bytesRead);
            }
            if (_dataAvailableCallback) {
                _dataAvailableCallback();
            }
        } else {
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
    ydebug("PtyReaderWindows: reader thread exiting");
}

} // namespace yetty
