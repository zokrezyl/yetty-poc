// html2ydraw: Convert HTML to YDraw binary v2 format
//
// Uses the shared HtmlRenderer to convert HTML content, targeting
// YDrawBuilder/YDrawBuffer. Serializes buffer data for output.

#include "html-renderer.h"
#include "http-fetcher.h"
#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#include <args.hxx>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Internal header for buffer
namespace yetty { class YDrawBuffer; }
#include "../../src/yetty/ydraw/ydraw-buffer.h"

using namespace yetty;
using namespace yetty::card;

int main(int argc, char** argv) {
    args::ArgumentParser parser("html2ydraw - Convert HTML to YDraw binary v2");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Flag oscFlag(parser, "osc", "Wrap output in OSC escape sequence", {"osc"});
    args::ValueFlag<int> xFlag(parser, "X", "Card X position (cells)", {'x'}, 0);
    args::ValueFlag<int> yFlag(parser, "Y", "Card Y position (cells)", {'y'}, 0);
    args::ValueFlag<int> wFlag(parser, "W", "Card width (cells)", {'w'}, 80);
    args::ValueFlag<int> hFlag(parser, "H", "Card height (cells)", {'H'}, 25);
    args::ValueFlag<float> fontSizeFlag(parser, "SIZE", "Default font size", {"font-size"}, 16.0f);
    args::ValueFlag<float> viewWidthFlag(parser, "WIDTH", "View width in pixels", {"view-width"}, 600.0f);
    args::Positional<std::string> inputFile(parser, "file", "HTML file or URL (- for stdin)");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Redirect spdlog to file
    auto fileLogger = spdlog::basic_logger_mt("html2ydraw", "/tmp/html2ydraw.log", true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

    // Load HTML content
    std::string htmlContent;
    std::string source = inputFile ? args::get(inputFile) : "-";

    std::shared_ptr<HttpFetcher> fetcher;
    auto fetcherResult = HttpFetcher::create();
    if (fetcherResult) {
        fetcher = std::move(*fetcherResult);
    }

    if (source == "-") {
        // Read from stdin
        std::stringstream ss;
        ss << std::cin.rdbuf();
        htmlContent = ss.str();
    } else if (HttpFetcher::isUrl(source)) {
        if (!fetcher) {
            std::cerr << "Error: HTTP fetcher unavailable\n";
            return 1;
        }
        fetcher->setBaseUrl(source);
        auto body = fetcher->fetch(source);
        if (!body) {
            std::cerr << "Error: failed to fetch URL: " << source << "\n";
            return 1;
        }
        htmlContent = std::move(*body);
    } else {
        if (!std::filesystem::exists(source)) {
            std::cerr << "Error: file not found: " << source << "\n";
            return 1;
        }
        std::ifstream file(source);
        if (!file) {
            std::cerr << "Error: failed to open file: " << source << "\n";
            return 1;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        htmlContent = ss.str();
    }

    if (htmlContent.empty()) {
        std::cerr << "Error: empty HTML content\n";
        return 1;
    }

    auto buffer = *YDrawBuffer::create();

    float fontSize = args::get(fontSizeFlag);
    float viewWidth = args::get(viewWidthFlag);

    auto result = renderHtmlToBuffer(htmlContent, buffer,
                                      fontSize, viewWidth, fetcher.get());

    if (!result.document) {
        std::cerr << "Error: failed to render HTML\n";
        return 1;
    }

    yinfo("html2ydraw: {}x{}", result.documentWidth, result.documentHeight);

    // Serialize buffer data
    auto binary = buffer->serialize();

    if (oscFlag) {
        // Base64-encode for OSC
        static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string b64str;
        b64str.reserve((binary.size() + 2) / 3 * 4);
        for (size_t i = 0; i < binary.size(); i += 3) {
            uint32_t n = static_cast<uint32_t>(binary[i]) << 16;
            if (i + 1 < binary.size()) n |= static_cast<uint32_t>(binary[i + 1]) << 8;
            if (i + 2 < binary.size()) n |= static_cast<uint32_t>(binary[i + 2]);
            b64str += b64[(n >> 18) & 0x3F];
            b64str += b64[(n >> 12) & 0x3F];
            b64str += (i + 1 < binary.size()) ? b64[(n >> 6) & 0x3F] : '=';
            b64str += (i + 2 < binary.size()) ? b64[n & 0x3F] : '=';
        }

        char header[512];
        std::snprintf(header, sizeof(header),
                 "\033]666666;run -c ydraw -x %d -y %d -w %d -h %d -r;;",
                 args::get(xFlag), args::get(yFlag),
                 args::get(wFlag), args::get(hFlag));

        yinfo("binary={} bytes, b64={} bytes", binary.size(), b64str.size());
        std::cout << header << b64str << "\033\\" << std::endl;
    } else {
        std::cout.write(reinterpret_cast<const char*>(binary.data()),
                        static_cast<std::streamsize>(binary.size()));
    }

    return 0;
}
