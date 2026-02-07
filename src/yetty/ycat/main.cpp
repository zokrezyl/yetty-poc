#include "osc.h"
#include "ts/highlight.h"

#include <args.hxx>

#ifdef YCAT_HAS_LIBMAGIC
#include <magic.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#ifdef __unix__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

// ──────────────────────────────────────────────────────────────────────────────
// MIME → plugin mapping
// ──────────────────────────────────────────────────────────────────────────────

struct PluginMapping {
    std::string_view plugin;     // yetty card plugin name
};

static const PluginMapping* mimeToPlugin(std::string_view mime) {
    if (mime.starts_with("image/")) {
        static const PluginMapping m{"image"};
        return &m;
    }
    if (mime == "application/pdf") {
        static const PluginMapping m{"pdf"};
        return &m;
    }
    if (mime == "text/markdown" || mime == "text/x-markdown") {
        static const PluginMapping m{"markdown"};
        return &m;
    }
    return nullptr;
}

// Try extension when libmagic returns generic text/*
static const PluginMapping* extensionToPlugin(const fs::path& path) {
    auto ext = path.extension().string();
    if (ext == ".md" || ext == ".markdown" || ext == ".mdown" || ext == ".mkd") {
        static const PluginMapping m{"markdown"};
        return &m;
    }
    return nullptr;
}

// ──────────────────────────────────────────────────────────────────────────────
// Terminal size
// ──────────────────────────────────────────────────────────────────────────────

static int terminalColumns() {
#ifdef __unix__
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        return ws.ws_col;
    }
#endif
    return 80;
}

// ──────────────────────────────────────────────────────────────────────────────
// MIME type detection
// ──────────────────────────────────────────────────────────────────────────────

#ifdef YCAT_HAS_LIBMAGIC
// Full libmagic-based MIME detection (Linux only)
class MagicDetector {
public:
    MagicDetector() {
        cookie_ = magic_open(MAGIC_MIME_TYPE | MAGIC_NO_CHECK_COMPRESS);
        if (!cookie_) return;

        // Try YCAT_MAGIC_MGC env override, then compiled-in path, then system default
        const char* mgcPath = std::getenv("YCAT_MAGIC_MGC");
        if (!mgcPath) mgcPath = YCAT_MAGIC_MGC_PATH;

        if (magic_load(cookie_, mgcPath) != 0) {
            // Fall back to system default
            magic_load(cookie_, nullptr);
        }
    }

    ~MagicDetector() {
        if (cookie_) magic_close(cookie_);
    }

    MagicDetector(const MagicDetector&) = delete;
    MagicDetector& operator=(const MagicDetector&) = delete;

    std::string detectFile(const fs::path& path) const {
        if (!cookie_) return "application/octet-stream";
        const char* mime = magic_file(cookie_, path.c_str());
        return mime ? mime : "application/octet-stream";
    }

    std::string detectBuffer(const void* data, size_t len) const {
        if (!cookie_) return "application/octet-stream";
        const char* mime = magic_buffer(cookie_, data, len);
        return mime ? mime : "application/octet-stream";
    }

private:
    magic_t cookie_ = nullptr;
};
#else
// Fallback: extension-based detection only (Windows/macOS)
class MagicDetector {
public:
    MagicDetector() = default;

    std::string detectFile(const fs::path& path) const {
        return detectByExtension(path.extension().string());
    }

    std::string detectBuffer(const void* /*data*/, size_t /*len*/) const {
        return "application/octet-stream";
    }

private:
    static std::string detectByExtension(const std::string& ext) {
        // Common extensions
        if (ext == ".png") return "image/png";
        if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
        if (ext == ".gif") return "image/gif";
        if (ext == ".webp") return "image/webp";
        if (ext == ".svg") return "image/svg+xml";
        if (ext == ".pdf") return "application/pdf";
        if (ext == ".md" || ext == ".markdown") return "text/markdown";
        if (ext == ".txt") return "text/plain";
        if (ext == ".html" || ext == ".htm") return "text/html";
        if (ext == ".json") return "application/json";
        if (ext == ".xml") return "application/xml";
        return "application/octet-stream";
    }
};
#endif

// ──────────────────────────────────────────────────────────────────────────────
// Output helpers
// ──────────────────────────────────────────────────────────────────────────────

static bool writeStdout(const void* data, size_t len) {
    return std::fwrite(data, 1, len, stdout) == len;
}

static bool catFile(const fs::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::cerr << "ycat: " << path.string() << ": No such file or directory\n";
        return false;
    }
    char buf[65536];
    while (f.read(buf, sizeof(buf)) || f.gcount() > 0) {
        if (!writeStdout(buf, static_cast<size_t>(f.gcount()))) return false;
    }
    return true;
}

static std::string readFileAsString(const fs::path& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f) return {};
    auto size = f.tellg();
    if (size <= 0) return {};
    std::string content(static_cast<size_t>(size), '\0');
    f.seekg(0);
    f.read(content.data(), size);
    return content;
}

static bool catStdin() {
    char buf[65536];
    while (!std::feof(stdin)) {
        size_t n = std::fread(buf, 1, sizeof(buf), stdin);
        if (n > 0 && !writeStdout(buf, n)) return false;
        if (std::ferror(stdin)) return false;
    }
    return true;
}

static std::vector<uint8_t> readAllStdin() {
    std::vector<uint8_t> data;
    char buf[65536];
    while (!std::feof(stdin)) {
        size_t n = std::fread(buf, 1, sizeof(buf), stdin);
        if (n > 0) data.insert(data.end(), buf, buf + n);
        if (std::ferror(stdin)) break;
    }
    return data;
}

// ──────────────────────────────────────────────────────────────────────────────
// Process a single file / stdin
// ──────────────────────────────────────────────────────────────────────────────

static bool processFile(
    const std::string& fileArg,
    const MagicDetector& magic,
    int width, int height,
    bool absolute,
    bool forceRaw)
{
    const bool isStdin = (fileArg == "-");

    // Force raw cat mode
    if (forceRaw) {
        if (isStdin) return catStdin();
        return catFile(fileArg);
    }

    std::string mime;
    const PluginMapping* mapping = nullptr;

    if (isStdin) {
        // Must read all data to detect type
        auto data = readAllStdin();
        if (data.empty()) return true;

        mime = magic.detectBuffer(data.data(), data.size());
        mapping = mimeToPlugin(mime);

        if (!mapping) {
            // Plain cat fallback — write raw bytes to stdout
            return writeStdout(data.data(), data.size());
        }

        // Stdin: send binary payload (base64-encoded bytes)
        auto seq = ycat::createSequenceBytes(
            mapping->plugin, 0, 0, width, height, !absolute, data);
        auto out = ycat::maybeWrapForTmux(seq);
        return writeStdout(out.data(), out.size());
    }

    // ── File path ──────────────────────────────────────────────────────────

    auto path = fs::absolute(fileArg);
    if (!fs::exists(path)) {
        std::cerr << "ycat: " << fileArg << ": No such file or directory\n";
        return false;
    }

    mime = magic.detectFile(path);
    mapping = mimeToPlugin(mime);

    // For text/plain, try extension-based detection
    if (!mapping && (mime == "text/plain" || mime.starts_with("text/"))) {
        mapping = extensionToPlugin(path);
    }

    if (!mapping) {
        // Try syntax highlighting via tree-sitter
        auto grammar = ycat::ts::resolveGrammar(mime, path.extension().string());
        if (!grammar.empty()) {
            auto source = readFileAsString(path);
            if (!source.empty() && ycat::ts::highlightToStdout(source, grammar)) {
                return true;
            }
        }
        // Plain cat fallback
        return catFile(path);
    }

    // File: send absolute path as payload (base64-encoded text)
    // The card plugin is responsible for loading from this path
    auto seq = ycat::createSequence(
        mapping->plugin, 0, 0, width, height, !absolute, path.string());
    auto out = ycat::maybeWrapForTmux(seq);
    return writeStdout(out.data(), out.size());
}

// ──────────────────────────────────────────────────────────────────────────────
// main
// ──────────────────────────────────────────────────────────────────────────────

int main(int argc, const char** argv) {
    args::ArgumentParser parser("ycat", "Cat with yetty card rendering for images, PDFs, and more.");
    parser.Prog("ycat");

    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});

    args::ValueFlag<int> widthFlag(parser, "cols",
        "Card width in columns (default: terminal width)", {'w', "width"});
    args::ValueFlag<int> heightFlag(parser, "rows",
        "Card height in rows (default: auto)", {'H', "height"}, 0);
    args::Flag absoluteFlag(parser, "absolute",
        "Use absolute positioning (default: relative to cursor)", {'a', "absolute"});
    args::Flag rawFlag(parser, "raw",
        "Force plain cat (no card rendering)", {'r', "raw"});

    args::PositionalList<std::string> files(parser, "files",
        "Files to display (use - for stdin)");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << "\n";
        std::cerr << parser;
        return 1;
    }

    int width = widthFlag ? args::get(widthFlag) : terminalColumns();
    int height = args::get(heightFlag);
    bool absolute = absoluteFlag;
    bool forceRaw = rawFlag;

    auto fileList = args::get(files);
    if (fileList.empty()) {
        // No files: read from stdin (like cat)
        fileList.push_back("-");
    }

    MagicDetector magic;

    bool ok = true;
    for (const auto& file : fileList) {
        if (!processFile(file, magic, width, height, absolute, forceRaw)) {
            ok = false;
        }
    }

    return ok ? 0 : 1;
}
