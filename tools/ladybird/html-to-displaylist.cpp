// Simple HTML to DisplayList converter
// This is a demo/fallback that generates DisplayList JSON from basic HTML
// For full web rendering, use Ladybird's LibWeb

#include "displaylist-json.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <cstring>

namespace {

// Simple HTML parser for basic pages
// Only handles: headings, paragraphs, divs, spans
// For real web rendering, use LibWeb

struct Style {
    displaylist::Color color{0, 0, 0, 255};
    displaylist::Color background{255, 255, 255, 255};
    int fontSize = 16;
    bool bold = false;
};

class SimpleHTMLRenderer {
public:
    explicit SimpleHTMLRenderer(int width, int height)
        : _width(width), _height(height), _curX(10), _curY(20)
    {
        _list.width = width;
        _list.height = height;

        // White background
        displaylist::FillRect bg;
        bg.rect = {0, 0, width, height};
        bg.color = {255, 255, 255, 255};
        _list.commands.push_back(bg);
    }

    void render(const std::string& html) {
        // Strip tags and extract text content with basic styling
        std::string content = html;

        // Handle title
        std::regex titleRe("<title[^>]*>([^<]*)</title>", std::regex::icase);
        std::smatch m;
        if (std::regex_search(content, m, titleRe)) {
            std::string title = m[1];
            addText(title, 10, 30, 24, {0, 0, 128, 255});
            _curY = 70;
        }

        // Handle headings h1-h6
        std::regex h1Re("<h1[^>]*>([^<]*)</h1>", std::regex::icase);
        while (std::regex_search(content, m, h1Re)) {
            addText(m[1], 10, _curY, 32, {0, 0, 0, 255});
            _curY += 45;
            content = m.suffix();
        }

        // Handle paragraphs
        content = html;
        std::regex pRe("<p[^>]*>([^<]*)</p>", std::regex::icase);
        while (std::regex_search(content, m, pRe)) {
            std::string text = m[1];
            addWrappedText(text, 10, _curY, 16, {0, 0, 0, 255});
            content = m.suffix();
        }

        // Handle links
        content = html;
        std::regex aRe("<a[^>]*>([^<]*)</a>", std::regex::icase);
        while (std::regex_search(content, m, aRe)) {
            addText(m[1], _curX, _curY, 16, {0, 0, 255, 255});
            _curY += 22;
            content = m.suffix();
        }

        // Handle divs with text
        content = html;
        std::regex divRe("<div[^>]*>([^<]+)</div>", std::regex::icase);
        while (std::regex_search(content, m, divRe)) {
            std::string text = m[1];
            // Skip if just whitespace
            if (text.find_first_not_of(" \t\n\r") != std::string::npos) {
                addWrappedText(text, 10, _curY, 14, {51, 51, 51, 255});
            }
            content = m.suffix();
        }

        // Plain text if nothing else
        if (_list.commands.size() == 1) {
            // Just background, no content found - show raw text
            std::string plain = stripTags(html);
            if (!plain.empty()) {
                addWrappedText(plain, 10, 40, 14, {0, 0, 0, 255});
            }
        }
    }

    displaylist::DisplayList getDisplayList() const {
        return _list;
    }

private:
    void addText(const std::string& text, int x, int y, int fontSize, displaylist::Color color) {
        displaylist::DrawGlyphRun cmd;
        cmd.rect = {x, y, static_cast<int>(text.length() * fontSize * 0.6), fontSize};
        cmd.translation = {0, 0};
        cmd.color = color;
        cmd.text = text;
        cmd.font_size = static_cast<float>(fontSize);
        _list.commands.push_back(cmd);
    }

    void addWrappedText(const std::string& text, int x, int y, int fontSize, displaylist::Color color) {
        int maxWidth = _width - 20;
        int charWidth = fontSize * 0.6;
        int charsPerLine = maxWidth / charWidth;

        size_t pos = 0;
        while (pos < text.length()) {
            size_t end = std::min(pos + charsPerLine, text.length());
            // Try to break at word boundary
            if (end < text.length()) {
                size_t space = text.rfind(' ', end);
                if (space != std::string::npos && space > pos) {
                    end = space + 1;
                }
            }
            std::string line = text.substr(pos, end - pos);
            addText(line, x, _curY, fontSize, color);
            _curY += fontSize + 6;
            pos = end;
        }
    }

    std::string stripTags(const std::string& html) {
        std::string result;
        bool inTag = false;
        for (char c : html) {
            if (c == '<') inTag = true;
            else if (c == '>') inTag = false;
            else if (!inTag) result += c;
        }
        // Collapse whitespace
        std::regex ws("\\s+");
        result = std::regex_replace(result, ws, " ");
        // Trim
        size_t start = result.find_first_not_of(" \t\n\r");
        size_t end = result.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return result.substr(start, end - start + 1);
    }

    displaylist::DisplayList _list;
    int _width, _height;
    int _curX, _curY;
};

// Fetch URL content (if CURL available)
#ifdef HAS_CURL
#include <curl/curl.h>

size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string fetchURL(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libweb-to-ydraw/1.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        return "";
    }
    return response;
}
#else
std::string fetchURL(const std::string& url) {
    std::cerr << "CURL not available, cannot fetch: " << url << "\n";
    return "";
}
#endif

} // anonymous namespace

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " [options] [URL]\n"
              << "\n"
              << "Simple HTML to DisplayList JSON converter.\n"
              << "For full web rendering, use Ladybird's LibWeb.\n"
              << "\n"
              << "Options:\n"
              << "  --width <n>     Viewport width (default: 800)\n"
              << "  --height <n>    Viewport height (default: 600)\n"
              << "  --stdin         Read HTML from stdin instead of URL\n"
              << "  --help          Show this help\n"
              << "\n"
              << "Output: JSON DisplayList to stdout\n"
              << "\n"
              << "Example:\n"
              << "  " << progName << " https://example.com | displaylist-to-ydraw\n";
}

int main(int argc, char* argv[]) {
    int width = 800;
    int height = 600;
    bool fromStdin = false;
    std::string url;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (std::strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
            width = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--height") == 0 && i + 1 < argc) {
            height = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--stdin") == 0) {
            fromStdin = true;
        } else if (argv[i][0] != '-') {
            url = argv[i];
        } else {
            std::cerr << "Unknown option: " << argv[i] << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    std::string html;
    if (fromStdin) {
        std::ostringstream ss;
        ss << std::cin.rdbuf();
        html = ss.str();
    } else if (!url.empty()) {
        html = fetchURL(url);
        if (html.empty()) {
            std::cerr << "Failed to fetch URL\n";
            return 1;
        }
    } else {
        // Demo HTML
        html = R"(
<!DOCTYPE html>
<html>
<head><title>Demo Page</title></head>
<body>
<h1>Hello from html-to-displaylist!</h1>
<p>This is a simple HTML to DisplayList converter.</p>
<p>For full web rendering with CSS support, use Ladybird's LibWeb.</p>
<a href="#">Click here</a>
</body>
</html>
)";
    }

    SimpleHTMLRenderer renderer(width, height);
    renderer.render(html);

    std::cout << displaylist::toJSON(renderer.getDisplayList());
    return 0;
}
