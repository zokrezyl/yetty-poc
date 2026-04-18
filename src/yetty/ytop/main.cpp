// ytop - A top-like tool using yetty plot cards for process CPU visualization
//
// Each process gets a small inline plot showing CPU history.

#include <args.hxx>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifdef __unix__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

//-----------------------------------------------------------------------------
// OSC helpers
//-----------------------------------------------------------------------------
namespace {

constexpr int VENDOR_ID = 666666;

static constexpr char BASE64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const std::string& data) {
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);
    size_t i = 0;
    const size_t len = data.size();
    auto bytes = reinterpret_cast<const uint8_t*>(data.data());

    while (i + 2 < len) {
        uint32_t triple = (static_cast<uint32_t>(bytes[i]) << 16) |
                          (static_cast<uint32_t>(bytes[i + 1]) << 8) |
                          static_cast<uint32_t>(bytes[i + 2]);
        result.push_back(BASE64_CHARS[(triple >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 6) & 0x3F]);
        result.push_back(BASE64_CHARS[triple & 0x3F]);
        i += 3;
    }
    if (i + 1 == len) {
        uint32_t val = static_cast<uint32_t>(bytes[i]) << 16;
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back('=');
        result.push_back('=');
    } else if (i + 2 == len) {
        uint32_t val = (static_cast<uint32_t>(bytes[i]) << 16) |
                       (static_cast<uint32_t>(bytes[i + 1]) << 8);
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 6) & 0x3F]);
        result.push_back('=');
    }
    return result;
}

std::string maybeWrapForTmux(const std::string& sequence) {
    if (std::getenv("TMUX")) {
        std::string escaped;
        escaped.reserve(sequence.size() * 2);
        for (char c : sequence) {
            if (c == '\033') escaped += "\033\033";
            else escaped += c;
        }
        return std::string("\033Ptmux;") + escaped + "\033\\";
    }
    return sequence;
}

void writeOsc(const std::string& seq) {
    auto out = maybeWrapForTmux(seq);
    std::fwrite(out.data(), 1, out.size(), stdout);
    std::fflush(stdout);
}

// Create plot at absolute position (x,y in cells)
std::string createPlot(const std::string& name, int x, int y, int w, int h,
                       const std::string& initialData) {
    std::ostringstream ss;
    ss << "\033]" << VENDOR_ID
       << ";run -c plot -x " << x << " -y " << y
       << " -w " << w << " -h " << h
       << " --name " << name
       << ";--min 0 --max 100 --no-axes --no-grid;"
       << base64Encode(initialData) << "\033\\";
    return ss.str();
}

// Update plot with advance
std::string updatePlot(const std::string& name, float value) {
    std::ostringstream payload;
    payload << value;
    std::ostringstream ss;
    ss << "\033]" << VENDOR_ID
       << ";update --name " << name
       << ";advance=1;"
       << base64Encode(payload.str()) << "\033\\";
    return ss.str();
}

}  // namespace

//-----------------------------------------------------------------------------
// Process tracking
//-----------------------------------------------------------------------------
namespace {

struct ProcessInfo {
    int pid;
    std::string name;
    uint64_t totalTime = 0;
    float cpuPercent = 0.0f;
};

uint64_t getSystemJiffies() {
    std::ifstream stat("/proc/stat");
    if (!stat) return 0;
    std::string line;
    std::getline(stat, line);
    uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
    char cpu[8];
    if (sscanf(line.c_str(), "%7s %lu %lu %lu %lu %lu %lu %lu %lu",
               cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) >= 5) {
        return user + nice + system + idle + iowait + irq + softirq + steal;
    }
    return 0;
}

bool readProcessStat(int pid, ProcessInfo& info) {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream stat(path);
    if (!stat) return false;
    std::string line;
    std::getline(stat, line);

    size_t start = line.find('(');
    size_t end = line.rfind(')');
    if (start == std::string::npos || end == std::string::npos) return false;

    info.pid = pid;
    info.name = line.substr(start + 1, end - start - 1);
    if (info.name.length() > 12) info.name = info.name.substr(0, 12);

    std::string rest = line.substr(end + 2);
    std::istringstream iss(rest);
    std::string skip;
    for (int i = 0; i < 11; i++) iss >> skip;
    uint64_t utime, stime;
    iss >> utime >> stime;
    info.totalTime = utime + stime;
    return true;
}

std::vector<int> enumerateProcesses() {
    std::vector<int> pids;
    DIR* dir = opendir("/proc");
    if (!dir) return pids;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* end;
            long pid = strtol(entry->d_name, &end, 10);
            if (*end == '\0' && pid > 0) pids.push_back(static_cast<int>(pid));
        }
    }
    closedir(dir);
    return pids;
}

int terminalColumns() {
#ifdef __unix__
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
#endif
    return 80;
}

int terminalRows() {
#ifdef __unix__
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0) return ws.ws_row;
#endif
    return 24;
}

}  // namespace

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main(int argc, const char** argv) {
    args::ArgumentParser parser("ytop", "Top-like process monitor with inline CPU graphs");
    parser.Prog("ytop");
    args::HelpFlag helpFlag(parser, "help", "Show this help", {'h', "help"});
    args::ValueFlag<int> intervalFlag(parser, "ms", "Update interval ms (default: 500)", {'i', "interval"}, 500);
    args::ValueFlag<int> numFlag(parser, "n", "Number of processes (default: 8)", {'n', "num"}, 8);
    args::ValueFlag<int> widthFlag(parser, "w", "Graph width in cols (default: 40)", {'w', "width"}, 40);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    int intervalMs = args::get(intervalFlag);
    int numProcs = args::get(numFlag);
    int graphWidth = args::get(widthFlag);
    int termCols = terminalColumns();
    int labelWidth = 22;  // "12. processname 99.9%"
    int graphX = labelWidth;

    // Initial data (zeros)
    std::string initData;
    for (int i = 0; i < graphWidth; i++) {
        if (i > 0) initData += ',';
        initData += '0';
    }

    // Print header and reserve lines
    printf("\033[2J\033[H");  // Clear screen, home
    printf("ytop - press Ctrl+C to exit\n\n");
    for (int i = 0; i < numProcs; i++) {
        printf("%2d. %-12s %5.1f%%\n", i + 1, "---", 0.0f);
    }
    fflush(stdout);

    // Create plots (absolute positioning, row 2 + i for each process)
    std::vector<std::string> plotNames(numProcs);
    for (int i = 0; i < numProcs; i++) {
        plotNames[i] = "p" + std::to_string(i);
        // Row 0 = header, row 1 = blank, row 2+ = processes
        writeOsc(createPlot(plotNames[i], graphX, 2 + i, graphWidth, 1, initData));
    }

    // Track processes
    std::map<int, ProcessInfo> prevProcs;
    uint64_t prevJiffies = getSystemJiffies();

    // Wait for first sample
    std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));

    while (true) {
        uint64_t currJiffies = getSystemJiffies();
        uint64_t sysDelta = currJiffies - prevJiffies;
        if (sysDelta == 0) sysDelta = 1;

        // Read all processes
        std::map<int, ProcessInfo> currProcs;
        for (int pid : enumerateProcesses()) {
            ProcessInfo info;
            if (readProcessStat(pid, info)) {
                auto it = prevProcs.find(pid);
                if (it != prevProcs.end()) {
                    uint64_t delta = info.totalTime - it->second.totalTime;
                    info.cpuPercent = 100.0f * static_cast<float>(delta) / static_cast<float>(sysDelta);
                    if (info.cpuPercent > 100.0f) info.cpuPercent = 100.0f;
                }
                currProcs[pid] = info;
            }
        }

        // Sort by CPU
        std::vector<ProcessInfo*> sorted;
        for (auto& kv : currProcs) sorted.push_back(&kv.second);
        std::sort(sorted.begin(), sorted.end(),
                  [](auto* a, auto* b) { return a->cpuPercent > b->cpuPercent; });

        // Update display
        printf("\033[3;1H");  // Move to row 3, col 1
        for (int i = 0; i < numProcs; i++) {
            if (i < static_cast<int>(sorted.size())) {
                auto* p = sorted[i];
                printf("%2d. %-12s %5.1f%%", i + 1, p->name.c_str(), p->cpuPercent);
                // Update plot
                writeOsc(updatePlot(plotNames[i], p->cpuPercent));
            } else {
                printf("%2d. %-12s %5.1f%%", i + 1, "---", 0.0f);
                writeOsc(updatePlot(plotNames[i], 0.0f));
            }
            printf("\033[K\n");  // Clear to EOL, newline
        }
        fflush(stdout);

        prevProcs = std::move(currProcs);
        prevJiffies = currJiffies;
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }

    return 0;
}
