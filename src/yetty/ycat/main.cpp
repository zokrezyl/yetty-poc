#include "osc.h"
#include "ts/highlight.h"

#include <args.hxx>
#include <cpr/cpr.h>

#ifdef YCAT_HAS_LIBMAGIC
#include <magic.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#ifdef __unix__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

// ──────────────────────────────────────────────────────────────────────────────
// URL detection and fetching
// ──────────────────────────────────────────────────────────────────────────────

static bool isUrl(const std::string& str) {
    return str.size() > 8 &&
           (str.substr(0, 7) == "http://" || str.substr(0, 8) == "https://");
}

struct FetchResult {
    std::vector<uint8_t> data;
    std::string contentType;  // MIME type from Content-Type header
    std::string url;          // Final URL after redirects
};

static std::optional<FetchResult> fetchUrl(const std::string& url) {
    cpr::Response r = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"User-Agent", "ycat/1.0"}},
        cpr::Timeout{30000},
        cpr::Redirect{10L}
    );

    if (r.status_code == 0) {
        std::cerr << "ycat: " << url << ": " << r.error.message << "\n";
        return std::nullopt;
    }
    if (r.status_code >= 400) {
        std::cerr << "ycat: " << url << ": HTTP " << r.status_code << "\n";
        return std::nullopt;
    }

    FetchResult result;
    result.data.assign(r.text.begin(), r.text.end());
    result.url = r.url.str();

    // Extract MIME type from Content-Type header (strip charset etc.)
    auto ctIt = r.header.find("Content-Type");
    if (ctIt != r.header.end()) {
        result.contentType = ctIt->second;
        auto semicolon = result.contentType.find(';');
        if (semicolon != std::string::npos) {
            result.contentType = result.contentType.substr(0, semicolon);
        }
        // Trim whitespace
        while (!result.contentType.empty() && result.contentType.back() == ' ') {
            result.contentType.pop_back();
        }
    }

    return result;
}

// ──────────────────────────────────────────────────────────────────────────────
// MIME → card mapping
// ──────────────────────────────────────────────────────────────────────────────

struct CardMapping {
    std::string_view card;     // yetty card type name
};

static const CardMapping* mimeToCard(std::string_view mime) {
    // SVG must be checked before generic image/*
    if (mime == "image/svg+xml") {
        static const CardMapping m{"thorvg"};
        return &m;
    }
    if (mime.starts_with("image/")) {
        static const CardMapping m{"image"};
        return &m;
    }
    if (mime == "application/pdf") {
        static const CardMapping m{"ypdf"};
        return &m;
    }
    if (mime == "text/markdown" || mime == "text/x-markdown") {
        static const CardMapping m{"markdown"};
        return &m;
    }
    if (mime == "text/x-ymery") {
        static const CardMapping m{"ymery"};
        return &m;
    }
    // HTML → yhtml
    if (mime == "text/html") {
        static const CardMapping m{"yhtml"};
        return &m;
    }
    // Python scripts
    if (mime == "text/x-python" || mime == "text/x-script.python") {
        static const CardMapping m{"python"};
        return &m;
    }
    return nullptr;
}

// Try extension when libmagic returns generic text/*
static const CardMapping* extensionToCard(const fs::path& path) {
    auto ext = path.extension().string();
    if (ext == ".md" || ext == ".markdown" || ext == ".mdown" || ext == ".mkd") {
        static const CardMapping m{"markdown"};
        return &m;
    }
    if (ext == ".ymery") {
        static const CardMapping m{"ymery"};
        return &m;
    }
    if (ext == ".svg") {
        static const CardMapping m{"thorvg"};
        return &m;
    }
    if (ext == ".html" || ext == ".htm") {
        static const CardMapping m{"yhtml"};
        return &m;
    }
    if (ext == ".py") {
        static const CardMapping m{"python"};
        return &m;
    }
    if (ext == ".lottie") {
        static const CardMapping m{"thorvg"};
        return &m;
    }
    return nullptr;
}

// Check for #!ymery shebang at start of buffer
static bool hasYmeryShebang(const void* data, size_t len) {
    constexpr std::string_view magic = "#!ymery";
    if (len < magic.size()) return false;
    return std::string_view(static_cast<const char*>(data), magic.size()) == magic;
}

// Check if buffer looks like ydraw YAML (has "body:" key with shape primitives)
static bool looksLikeYdraw(const void* data, size_t len) {
    std::string_view sv(static_cast<const char*>(data), len);
    // Must have "body:" at start of line
    auto bodyPos = sv.find("\nbody:");
    if (bodyPos == std::string_view::npos) {
        // Check if it starts with "body:"
        if (!sv.starts_with("body:")) return false;
    }
    // Should have at least one shape primitive
    return sv.find("circle:") != std::string_view::npos ||
           sv.find("box:") != std::string_view::npos ||
           sv.find("ellipse:") != std::string_view::npos ||
           sv.find("triangle:") != std::string_view::npos ||
           sv.find("line:") != std::string_view::npos ||
           sv.find("polygon:") != std::string_view::npos ||
           sv.find("text:") != std::string_view::npos;
}

// Check if buffer looks like Lottie JSON animation
static bool looksLikeLottie(const void* data, size_t len) {
    std::string_view sv(static_cast<const char*>(data), len);
    // Lottie files have "v" (version), "fr" (framerate), "layers" keys
    return sv.find("\"layers\"") != std::string_view::npos &&
           sv.find("\"v\"") != std::string_view::npos &&
           (sv.find("\"fr\"") != std::string_view::npos ||
            sv.find("\"ip\"") != std::string_view::npos);
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
// Full libmagic-based MIME detection (Unix: Linux/macOS)
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
// Fallback: extension-based detection only (Windows)
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
    bool forceRaw,
    bool forceShow,
    const std::string& forceCard)
{
    const bool isStdin = (fileArg == "-");

    // Force raw cat mode
    if (forceRaw) {
        if (isStdin) return catStdin();
        return catFile(fileArg);
    }

    std::string mime;
    const CardMapping* mapping = nullptr;

    if (isStdin) {
        // Must read all data to detect type
        auto data = readAllStdin();
        if (data.empty()) return true;

        mime = magic.detectBuffer(data.data(), data.size());

        // --show: force syntax highlighting, skip card rendering
        if (forceShow) {
            auto grammar = ycat::ts::resolveGrammar(mime, "");
            if (!grammar.empty()) {
                std::string source(data.begin(), data.end());
                if (ycat::ts::highlightToStdout(source, grammar)) {
                    return true;
                }
            }
            // Fallback to plain cat
            return writeStdout(data.data(), data.size());
        }

        // Use forced card type if specified
        if (!forceCard.empty()) {
            auto seq = ycat::createSequenceBytes(
                forceCard, 0, 0, width, height, !absolute, data, "-i -");
            auto out = ycat::maybeWrapForTmux(seq);
            return writeStdout(out.data(), out.size());
        }

        // Check for #!ymery shebang first
        if (hasYmeryShebang(data.data(), data.size())) {
            static const CardMapping m{"ymery"};
            mapping = &m;
        } else if (looksLikeLottie(data.data(), data.size())) {
            static const CardMapping m{"thorvg"};
            mapping = &m;
        } else if (looksLikeYdraw(data.data(), data.size())) {
            static const CardMapping m{"ydraw"};
            mapping = &m;
        } else {
            mapping = mimeToCard(mime);
        }

        if (!mapping) {
            // Plain cat fallback — write raw bytes to stdout
            return writeStdout(data.data(), data.size());
        }

        // Stdin: send binary payload (base64-encoded bytes) with -i - to indicate payload input
        auto seq = ycat::createSequenceBytes(
            mapping->card, 0, 0, width, height, !absolute, data, "-i -");
        auto out = ycat::maybeWrapForTmux(seq);
        return writeStdout(out.data(), out.size());
    }

    // ── URL ───────────────────────────────────────────────────────────────

    if (isUrl(fileArg)) {
        auto fetched = fetchUrl(fileArg);
        if (!fetched) return false;

        // Use Content-Type from HTTP headers as primary MIME source
        if (!fetched->contentType.empty()) {
            mime = fetched->contentType;
        } else {
            mime = magic.detectBuffer(fetched->data.data(), fetched->data.size());
        }

        // --show: force syntax highlighting, skip card rendering
        if (forceShow) {
            auto grammar = ycat::ts::resolveGrammar(mime, "");
            if (!grammar.empty()) {
                std::string source(fetched->data.begin(), fetched->data.end());
                if (ycat::ts::highlightToStdout(source, grammar)) {
                    return true;
                }
            }
            // Fallback to plain cat
            return writeStdout(fetched->data.data(), fetched->data.size());
        }

        // Use forced card type if specified
        if (!forceCard.empty()) {
            auto seq = ycat::createSequenceBytes(
                forceCard, 0, 0, width, height, !absolute, fetched->data, "-i -");
            auto out = ycat::maybeWrapForTmux(seq);
            return writeStdout(out.data(), out.size());
        }

        mapping = mimeToCard(mime);

        // Fall back to content-based detection
        if (!mapping) {
            // Check for #!ymery shebang
            if (hasYmeryShebang(fetched->data.data(), fetched->data.size())) {
                static const CardMapping m{"ymery"};
                mapping = &m;
            } else if (looksLikeLottie(fetched->data.data(), fetched->data.size())) {
                static const CardMapping m{"thorvg"};
                mapping = &m;
            } else if (looksLikeYdraw(fetched->data.data(), fetched->data.size())) {
                static const CardMapping m{"ydraw"};
                mapping = &m;
            } else {
                mime = magic.detectBuffer(fetched->data.data(), fetched->data.size());
                mapping = mimeToCard(mime);
            }
        }

        if (!mapping) {
            // Plain cat fallback — write raw bytes to stdout
            return writeStdout(fetched->data.data(), fetched->data.size());
        }

        // URL: send binary payload (base64-encoded bytes) with -i - to indicate payload input
        auto seq = ycat::createSequenceBytes(
            mapping->card, 0, 0, width, height, !absolute, fetched->data, "-i -");
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

    // --show: force syntax highlighting, skip card rendering
    if (forceShow) {
        auto grammar = ycat::ts::resolveGrammar(mime, path.extension().string());
        if (!grammar.empty()) {
            auto source = readFileAsString(path);
            if (!source.empty() && ycat::ts::highlightToStdout(source, grammar)) {
                return true;
            }
        }
        // Fallback to plain cat
        return catFile(path);
    }

    // Use forced card type if specified
    if (!forceCard.empty()) {
        auto content = readFileAsString(path);
        if (content.empty()) {
            std::cerr << "ycat: " << fileArg << ": Failed to read file\n";
            return false;
        }
        std::vector<uint8_t> data(content.begin(), content.end());
        auto seq = ycat::createSequenceBytes(
            forceCard, 0, 0, width, height, !absolute, data, "-i -");
        auto out = ycat::maybeWrapForTmux(seq);
        return writeStdout(out.data(), out.size());
    }

    // Check for #!ymery shebang first (read first 8 bytes)
    {
        std::ifstream f(path, std::ios::binary);
        char buf[8] = {};
        f.read(buf, sizeof(buf));
        if (hasYmeryShebang(buf, static_cast<size_t>(f.gcount()))) {
            static const CardMapping m{"ymery"};
            mapping = &m;
        }
    }

    // Extension-based detection first (libmagic often misidentifies text files)
    if (!mapping) {
        mapping = extensionToCard(path);
    }

    if (!mapping) {
        mapping = mimeToCard(mime);
    }

    if (!mapping && path.extension() == ".ymery") {
        static const CardMapping m{"ymery"};
        mapping = &m;
    }

    // Content-based detection for ydraw YAML and Lottie JSON
    if (!mapping && (mime == "text/plain" || mime == "application/json" ||
                     path.extension() == ".yaml" || path.extension() == ".yml" ||
                     path.extension() == ".json")) {
        auto content = readFileAsString(path);
        if (!content.empty()) {
            if (looksLikeLottie(content.data(), content.size())) {
                static const CardMapping m{"thorvg"};
                mapping = &m;
            } else if (looksLikeYdraw(content.data(), content.size())) {
                static const CardMapping m{"ydraw"};
                mapping = &m;
            }
        }
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

    // Read file and send as payload (works both locally and remotely)
    auto content = readFileAsString(path);
    if (content.empty()) {
        std::cerr << "ycat: " << fileArg << ": Failed to read file\n";
        return false;
    }
    std::vector<uint8_t> data(content.begin(), content.end());
    auto seq = ycat::createSequenceBytes(
        mapping->card, 0, 0, width, height, !absolute, data, "-i -");
    auto out = ycat::maybeWrapForTmux(seq);
    return writeStdout(out.data(), out.size());
}

// ──────────────────────────────────────────────────────────────────────────────
// main
// ──────────────────────────────────────────────────────────────────────────────

int main(int argc, const char** argv) {
    args::ArgumentParser parser("ycat", "Cat with yetty card rendering for images, PDFs, and more.");
    parser.Prog("ycat");

    args::Flag helpFlag(parser, "help", "Show this help (or card help with --card)", {'h', "help"});

    args::ValueFlag<int> widthFlag(parser, "cols",
        "Card width in columns (default: terminal width)", {'w', "width"});
    args::ValueFlag<int> heightFlag(parser, "rows",
        "Card height in rows (default: auto)", {'H', "height"}, 0);
    args::Flag absoluteFlag(parser, "absolute",
        "Use absolute positioning (default: relative to cursor)", {'a', "absolute"});
    args::Flag rawFlag(parser, "raw",
        "Force plain cat (no card rendering)", {'r', "raw"});
    args::Flag showFlag(parser, "show",
        "Show source with syntax highlighting (skip card rendering)", {'s', "show"});
    args::ValueFlag<std::string> cardFlag(parser, "type",
        "Force card type (e.g., image, pdf, ypdf, ydraw, thorvg, ymery, python)", {'c', "card"});

    args::PositionalList<std::string> files(parser, "files",
        "Files to display (use - for stdin)");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Error& e) {
        std::cerr << e.what() << "\n";
        std::cerr << parser;
        return 1;
    }

    std::string forceCard = cardFlag ? args::get(cardFlag) : "";

    // Handle --help: with --card emit card help OSC, otherwise show ycat help
    if (helpFlag) {
        if (!forceCard.empty()) {
            // Emit help OSC for the specified card
            auto seq = ycat::createHelpSequence(forceCard);
            auto out = ycat::maybeWrapForTmux(seq);
            writeStdout(out.data(), out.size());
            return 0;
        }
        std::cout << parser;
        return 0;
    }

    int width = widthFlag ? args::get(widthFlag) : terminalColumns();
    int height = args::get(heightFlag);
    bool absolute = absoluteFlag;
    bool forceRaw = rawFlag;
    bool forceShow = showFlag;

    auto fileList = args::get(files);
    if (fileList.empty()) {
        // No files: read from stdin (like cat)
        fileList.push_back("-");
    }

    MagicDetector magic;

    bool ok = true;
    for (const auto& file : fileList) {
        if (!processFile(file, magic, width, height, absolute, forceRaw, forceShow, forceCard)) {
            ok = false;
        }
    }

    return ok ? 0 : 1;
}
