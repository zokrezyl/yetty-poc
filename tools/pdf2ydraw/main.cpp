// pdf2ydraw: Convert PDF to YDraw binary v2 format
//
// Uses the shared PdfRenderer to convert PDF pages, targeting
// YDrawWriterBinary for serialization. Outputs raw binary or
// base64-wrapped OSC escape sequence.

#include "pdf-renderer.h"
#include <yetty/ydraw-writer.h>
#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>

extern "C" {
#include <pdfio.h>
}

#include <args.hxx>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>

using namespace yetty;
using namespace yetty::card;

static bool pdfioErrorHandler(pdfio_file_t*, const char* message, void*) {
    ywarn("pdfio: {}", message);
    return true;
}

int main(int argc, char** argv) {
    args::ArgumentParser parser("pdf2ydraw - Convert PDF to YDraw binary v2");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Flag oscFlag(parser, "osc", "Wrap output in OSC escape sequence", {"osc"});
    args::ValueFlag<int> xFlag(parser, "X", "Card X position (cells)", {'x'}, 0);
    args::ValueFlag<int> yFlag(parser, "Y", "Card Y position (cells)", {'y'}, 0);
    args::ValueFlag<int> wFlag(parser, "W", "Card width (cells)", {'w'}, 80);
    args::ValueFlag<int> hFlag(parser, "H", "Card height (cells)", {'H'}, 25);
    args::Positional<std::string> pdfFile(parser, "file", "PDF file to convert");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (!pdfFile) {
        std::cerr << "Error: no PDF file specified\n";
        return 1;
    }

    // Redirect spdlog to file (stderr corrupts terminal in OSC mode)
    auto fileLogger = spdlog::basic_logger_mt("pdf2ydraw", "/tmp/pdf2ydraw.log", true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

    std::string path = args::get(pdfFile);
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: file not found: " << path << "\n";
        return 1;
    }

    // Open PDF
    pdfio_file_t* pdf = pdfioFileOpen(path.c_str(), nullptr, nullptr,
                                       pdfioErrorHandler, nullptr);
    if (!pdf) {
        std::cerr << "Error: failed to open PDF: " << path << "\n";
        return 1;
    }

    // Create binary writer and render PDF into it
    auto writerRes = YDrawWriterBinary::create();
    if (!writerRes) {
        std::cerr << "Error: failed to create writer: "
                  << writerRes.error().message() << "\n";
        pdfioFileClose(pdf);
        return 1;
    }
    auto writer = *writerRes;

    auto result = renderPdfToWriter(pdf, writer.get());
    pdfioFileClose(pdf);

    yinfo("pdf2ydraw: {} pages, totalHeight={:.1f}, firstPageH={:.1f}",
          result.pageCount, result.totalHeight, result.firstPageHeight);

    // Build binary output
    auto binary = writer->buildBinary();

    if (oscFlag) {
        auto b64 = writer->buildBase64();

        float pageH = result.firstPageHeight > 0.0f
                      ? result.firstPageHeight : 792.0f;
        char header[512];
        std::snprintf(header, sizeof(header),
                 "\033]666666;run -c ydraw -x %d -y %d -w %d -h %d -r"
                 ";bin --fit-page-height %.1f;",
                 args::get(xFlag), args::get(yFlag),
                 args::get(wFlag), args::get(hFlag),
                 pageH);

        yinfo("binary={} bytes, b64={} bytes", binary.size(), b64.size());
        std::cout << header << b64 << "\033\\" << std::endl;
    } else {
        std::cout.write(reinterpret_cast<const char*>(binary.data()),
                        static_cast<std::streamsize>(binary.size()));
    }

    return 0;
}
