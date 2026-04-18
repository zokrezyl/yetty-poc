#include <boost/ut.hpp>

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

using namespace boost::ut;

namespace {

std::string extractPrintable(const std::vector<char>& data) {
    std::string result;
    bool inEsc = false;
    for (char c : data) {
        if (c == '\033') { inEsc = true; continue; }
        if (inEsc) {
            if (c >= 0x40 && c <= 0x7E) inEsc = false;
            continue;
        }
        if (c >= 0x20 && c < 0x7F) result += c;
    }
    return result;
}

std::string hexDump(const std::vector<char>& data, size_t maxBytes = 512) {
    std::string result;
    size_t len = (std::min)(data.size(), maxBytes);
    for (size_t i = 0; i < len; i++) {
        char h[4];
        snprintf(h, sizeof(h), "%02x ", (unsigned char)data[i]);
        result += h;
        if ((i + 1) % 32 == 0) result += "\n";
    }
    return result;
}

struct ConPtySession {
    HANDLE hPipeInRd  = INVALID_HANDLE_VALUE;
    HANDLE hPipeInWr  = INVALID_HANDLE_VALUE;
    HANDLE hPipeOutRd = INVALID_HANDLE_VALUE;
    HANDLE hPipeOutWr = INVALID_HANDLE_VALUE;
    HPCON hPC = nullptr;
    HANDLE hProcess = INVALID_HANDLE_VALUE;
    LPPROC_THREAD_ATTRIBUTE_LIST attrList = nullptr;

    std::thread readerThread;
    std::mutex bufMutex;
    std::vector<char> readBuf;
    std::atomic<bool> running{false};

    ~ConPtySession() { cleanup(); }

    bool start(const wchar_t* cmdLine, COORD size = {80, 24}) {
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

        if (!CreatePipe(&hPipeInRd, &hPipeInWr, &sa, 0)) return false;
        if (!CreatePipe(&hPipeOutRd, &hPipeOutWr, &sa, 0)) {
            CloseHandle(hPipeInRd); CloseHandle(hPipeInWr);
            return false;
        }

        HRESULT hr = CreatePseudoConsole(size, hPipeInRd, hPipeOutWr, 0, &hPC);
        if (FAILED(hr)) {
            CloseHandle(hPipeInRd); CloseHandle(hPipeInWr);
            CloseHandle(hPipeOutRd); CloseHandle(hPipeOutWr);
            return false;
        }

        STARTUPINFOEXW siEx = {};
        siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);
        // Prevent child from inheriting parent's redirected stdio handles.
        // Without this, child output goes to parent's stdout instead of ConPTY.
        // See: https://github.com/microsoft/terminal/issues/11276
        siEx.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;

        SIZE_T attrListSize = 0;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
        attrList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
            HeapAlloc(GetProcessHeap(), 0, attrListSize));
        if (!attrList) { cleanup(); return false; }

        if (!InitializeProcThreadAttributeList(attrList, 1, 0, &attrListSize)) { cleanup(); return false; }
        if (!UpdateProcThreadAttribute(attrList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
                                        hPC, sizeof(hPC), nullptr, nullptr)) { cleanup(); return false; }
        siEx.lpAttributeList = attrList;

        std::wstring mutableCmd(cmdLine);
        PROCESS_INFORMATION pi = {};
        if (!CreateProcessW(nullptr, mutableCmd.data(), nullptr, nullptr, FALSE,
                           EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                           &siEx.StartupInfo, &pi)) {
            cleanup(); return false;
        }
        hProcess = pi.hProcess;
        CloseHandle(pi.hThread);

        CloseHandle(hPipeInRd);  hPipeInRd = INVALID_HANDLE_VALUE;
        CloseHandle(hPipeOutWr); hPipeOutWr = INVALID_HANDLE_VALUE;

        running = true;
        readerThread = std::thread([this]() {
            char buf[4096];
            int readCount = 0;
            while (running) {
                DWORD bytesRead = 0;
                BOOL ok = ReadFile(hPipeOutRd, buf, sizeof(buf), &bytesRead, nullptr);
                if (ok && bytesRead > 0) {
                    std::lock_guard<std::mutex> lock(bufMutex);
                    readBuf.insert(readBuf.end(), buf, buf + bytesRead);
                    readCount++;
                } else {
                    DWORD err = GetLastError();
                    if (err == ERROR_BROKEN_PIPE || err == ERROR_INVALID_HANDLE) break;
                    Sleep(10);
                }
            }
        });

        return true;
    }

    std::vector<char> waitAndRead(int timeoutMs = 5000) {
        auto start = std::chrono::steady_clock::now();
        size_t lastSize = 0;
        int stableCount = 0;

        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count() < timeoutMs) {
            Sleep(100);
            std::lock_guard<std::mutex> lock(bufMutex);
            if (readBuf.size() == lastSize) {
                stableCount++;
                if (stableCount >= 20 && readBuf.size() > 0) break;
            } else {
                lastSize = readBuf.size();
                stableCount = 0;
            }
        }

        std::lock_guard<std::mutex> lock(bufMutex);
        return readBuf;
    }

    void clearBuf() {
        std::lock_guard<std::mutex> lock(bufMutex);
        readBuf.clear();
    }

    void writeInput(const char* data, size_t len) {
        DWORD written = 0;
        WriteFile(hPipeInWr, data, (DWORD)len, &written, nullptr);
    }

    void cleanup() {
        running = false;
        if (hPC) { ClosePseudoConsole(hPC); hPC = nullptr; }
        if (hPipeInWr != INVALID_HANDLE_VALUE) { CloseHandle(hPipeInWr); hPipeInWr = INVALID_HANDLE_VALUE; }
        if (hPipeOutRd != INVALID_HANDLE_VALUE) { CloseHandle(hPipeOutRd); hPipeOutRd = INVALID_HANDLE_VALUE; }
        if (readerThread.joinable()) readerThread.join();
        if (hProcess != INVALID_HANDLE_VALUE) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            hProcess = INVALID_HANDLE_VALUE;
        }
        if (hPipeInRd != INVALID_HANDLE_VALUE) { CloseHandle(hPipeInRd); hPipeInRd = INVALID_HANDLE_VALUE; }
        if (hPipeOutWr != INVALID_HANDLE_VALUE) { CloseHandle(hPipeOutWr); hPipeOutWr = INVALID_HANDLE_VALUE; }
        if (attrList) {
            DeleteProcThreadAttributeList(attrList);
            HeapFree(GetProcessHeap(), 0, attrList);
            attrList = nullptr;
        }
    }
};

} // anonymous namespace

suite conpty_tests = [] {
    "conpty_creates_session"_test = [] {
        ConPtySession session;
        expect(session.start(L"cmd.exe") >> fatal);
        expect(session.hPC != nullptr);
        expect(session.hProcess != INVALID_HANDLE_VALUE);
    };

    "conpty_cmd_produces_output"_test = [] {
        ConPtySession session;
        expect(session.start(L"cmd.exe") >> fatal);

        auto output = session.waitAndRead(10000);

        printf("ConPTY cmd.exe output: %zu bytes\n", output.size());
        printf("Printable: [%s]\n", extractPrintable(output).c_str());

        expect(output.size() > 0_u) << "ConPTY should produce some output";
        // With STARTF_USESTDHANDLES fix, we should get actual text
        expect(output.size() > 16_u) << "Should get more than just init sequences";
    };

    "conpty_cmd_echo_produces_text"_test = [] {
        ConPtySession session;
        expect(session.start(L"cmd.exe /c echo CONPTY_TEST_OUTPUT") >> fatal);

        auto output = session.waitAndRead(10000);

        std::string printable = extractPrintable(output);
        printf("ConPTY echo: %zu bytes, printable: [%s]\n", output.size(), printable.c_str());

        bool hasText = printable.find("CONPTY_TEST_OUTPUT") != std::string::npos;
        expect(hasText) << "Output should contain the echoed text";
    };

    "conpty_cmd_interactive_prompt"_test = [] {
        ConPtySession session;
        expect(session.start(L"cmd.exe") >> fatal);

        auto output = session.waitAndRead(10000);

        std::string printable = extractPrintable(output);
        printf("Interactive: %zu bytes, printable: [%s]\n", output.size(), printable.c_str());

        bool hasPrompt = printable.find(">") != std::string::npos ||
                         printable.find("Microsoft") != std::string::npos ||
                         printable.find("Windows") != std::string::npos;
        expect(hasPrompt) << "Interactive cmd.exe should show a prompt or banner";
    };

    "conpty_write_then_read"_test = [] {
        ConPtySession session;
        expect(session.start(L"cmd.exe") >> fatal);

        auto initial = session.waitAndRead(3000);
        session.clearBuf();

        const char* cmd = "echo WRITE_TEST_HELLO\r\n";
        session.writeInput(cmd, strlen(cmd));

        auto response = session.waitAndRead(5000);
        std::string printable = extractPrintable(response);
        printf("Write/read: %zu bytes, printable: [%s]\n", response.size(), printable.c_str());

        bool hasEcho = printable.find("WRITE_TEST_HELLO") != std::string::npos;
        expect(hasEcho) << "Writing to cmd.exe should produce echoed output";
    };
};

#else

suite conpty_tests = [] {
    using namespace boost::ut;
    "conpty_not_available"_test = [] {
        expect(true) << "ConPTY tests only run on Windows";
    };
};

#endif
