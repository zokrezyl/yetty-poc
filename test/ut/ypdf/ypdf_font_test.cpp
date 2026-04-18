//=============================================================================
// YPdf Font Extraction Tests
//
// Tests that embedded fonts can be extracted from PDFs via pdfio,
// validating the API chain used by YPdf::extractFonts().
//=============================================================================

#include <cstddef>
#include <cstdint>
#include <version>
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <boost/ut.hpp>

extern "C" {
#include <pdfio.h>
}

using namespace boost::ut;

static const std::string kSamplePdf =
    std::string(CMAKE_SOURCE_DIR) + "/demo/assets/sample-local-pdf.pdf";

static bool errorHandler(pdfio_file_t*, const char*, void*) { return true; }

//=============================================================================
// Helper: get Resources dict from a page (handles indirect references)
//=============================================================================
static pdfio_dict_t* getPageResources(pdfio_obj_t* pageObj) {
    auto* pageDict = pdfioObjGetDict(pageObj);
    if (!pageDict) return nullptr;
    auto* res = pdfioDictGetDict(pageDict, "Resources");
    if (!res) {
        auto* resObj = pdfioDictGetObj(pageDict, "Resources");
        if (resObj) res = pdfioObjGetDict(resObj);
    }
    return res;
}

//=============================================================================
// Helper: get Font dict from Resources (handles indirect references)
//=============================================================================
static pdfio_dict_t* getFontDict(pdfio_dict_t* resources) {
    if (!resources) return nullptr;
    auto* fontDict = pdfioDictGetDict(resources, "Font");
    if (!fontDict) {
        auto* fontDictObj = pdfioDictGetObj(resources, "Font");
        if (fontDictObj) fontDict = pdfioObjGetDict(fontDictObj);
    }
    return fontDict;
}

//=============================================================================
// Tests
//=============================================================================

suite ypdf_pdf_open_tests = [] {
    "sample PDF opens successfully"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr) << "failed to open " << kSamplePdf;
        if (pdf) {
            expect(pdfioFileGetNumPages(pdf) == 3_u);
            pdfioFileClose(pdf);
        }
    };
};

suite ypdf_resource_traversal_tests = [] {
    "page has Resources dictionary"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        expect(pageObj != nullptr);

        auto* resources = getPageResources(pageObj);
        expect(resources != nullptr) << "Resources dict not found";

        pdfioFileClose(pdf);
    };

    "page Resources has Font dictionary"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        auto* resources = getPageResources(pageObj);
        expect(resources != nullptr);

        auto* fontDict = getFontDict(resources);
        expect(fontDict != nullptr) << "Font dict not found";

        if (fontDict) {
            size_t numFonts = pdfioDictGetNumPairs(fontDict);
            expect(numFonts >= 1_u) << "expected at least 1 font, got " << numFonts;
        }

        pdfioFileClose(pdf);
    };
};

suite ypdf_font_extraction_tests = [] {
    "font entries have FontDescriptor"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        auto* resources = getPageResources(pageObj);
        auto* fontDict = getFontDict(resources);
        expect(fontDict != nullptr);
        if (!fontDict) { pdfioFileClose(pdf); return; }

        size_t numFonts = pdfioDictGetNumPairs(fontDict);
        size_t fontsWithDescriptor = 0;

        for (size_t i = 0; i < numFonts; i++) {
            const char* tag = pdfioDictGetKey(fontDict, i);
            if (!tag) continue;

            auto* fontObj = pdfioDictGetObj(fontDict, tag);
            if (!fontObj) continue;

            auto* fontObjDict = pdfioObjGetDict(fontObj);
            if (!fontObjDict) continue;

            auto* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
            if (fontDescObj) fontsWithDescriptor++;
        }

        expect(fontsWithDescriptor >= 1_u)
            << "expected at least 1 font with FontDescriptor, got "
            << fontsWithDescriptor;

        pdfioFileClose(pdf);
    };

    "FontDescriptor has FontFile2 stream"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        auto* resources = getPageResources(pageObj);
        auto* fontDict = getFontDict(resources);
        expect(fontDict != nullptr);
        if (!fontDict) { pdfioFileClose(pdf); return; }

        size_t fontsWithFile = 0;
        size_t numFonts = pdfioDictGetNumPairs(fontDict);

        for (size_t i = 0; i < numFonts; i++) {
            const char* tag = pdfioDictGetKey(fontDict, i);
            if (!tag) continue;

            auto* fontObj = pdfioDictGetObj(fontDict, tag);
            if (!fontObj) continue;
            auto* fontObjDict = pdfioObjGetDict(fontObj);
            if (!fontObjDict) continue;

            auto* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
            if (!fontDescObj) continue;
            auto* fontDescDict = pdfioObjGetDict(fontDescObj);
            if (!fontDescDict) continue;

            auto* fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile2");
            if (!fontFileObj)
                fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile3");
            if (fontFileObj) fontsWithFile++;
        }

        expect(fontsWithFile >= 1_u)
            << "expected at least 1 font with embedded file, got "
            << fontsWithFile;

        pdfioFileClose(pdf);
    };

    "can read FontFile2 stream data"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        auto* resources = getPageResources(pageObj);
        auto* fontDict = getFontDict(resources);
        expect(fontDict != nullptr);
        if (!fontDict) { pdfioFileClose(pdf); return; }

        size_t numFonts = pdfioDictGetNumPairs(fontDict);
        bool extractedAny = false;

        for (size_t i = 0; i < numFonts; i++) {
            const char* tag = pdfioDictGetKey(fontDict, i);
            if (!tag) continue;

            auto* fontObj = pdfioDictGetObj(fontDict, tag);
            if (!fontObj) continue;
            auto* fontObjDict = pdfioObjGetDict(fontObj);
            if (!fontObjDict) continue;

            auto* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
            if (!fontDescObj) continue;
            auto* fontDescDict = pdfioObjGetDict(fontDescObj);
            if (!fontDescDict) continue;

            auto* fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile2");
            if (!fontFileObj) continue;

            auto* stream = pdfioObjOpenStream(fontFileObj, true);
            expect(stream != nullptr) << "failed to open stream for " << tag;
            if (!stream) continue;

            std::vector<uint8_t> data;
            uint8_t buf[4096];
            ssize_t n;
            while ((n = pdfioStreamRead(stream, buf, sizeof(buf))) > 0) {
                data.insert(data.end(), buf, buf + n);
            }
            pdfioStreamClose(stream);

            expect(data.size() > 0_u)
                << "font " << tag << " stream is empty";

            // Verify TrueType magic: first 4 bytes should be
            // 0x00010000 (TrueType) or 0x74727565 ("true") or OTTO (CFF)
            if (data.size() >= 4) {
                uint32_t magic = (uint32_t(data[0]) << 24) |
                                 (uint32_t(data[1]) << 16) |
                                 (uint32_t(data[2]) << 8) |
                                  uint32_t(data[3]);
                bool validMagic = (magic == 0x00010000) ||  // TrueType
                                  (magic == 0x74727565) ||  // "true"
                                  (magic == 0x4F54544F);    // "OTTO" (CFF)
                expect(validMagic)
                    << "font " << tag << " has unexpected magic: 0x"
                    << std::hex << magic;
            }

            extractedAny = true;
        }

        expect(extractedAny) << "no fonts were extracted";

        pdfioFileClose(pdf);
    };

    "extracted font can be written to file"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        auto* pageObj = pdfioFileGetPage(pdf, 0);
        auto* resources = getPageResources(pageObj);
        auto* fontDict = getFontDict(resources);
        if (!fontDict) { pdfioFileClose(pdf); return; }

        // Extract first font with FontFile2
        size_t numFonts = pdfioDictGetNumPairs(fontDict);
        for (size_t i = 0; i < numFonts; i++) {
            const char* tag = pdfioDictGetKey(fontDict, i);
            if (!tag) continue;

            auto* fontObj = pdfioDictGetObj(fontDict, tag);
            if (!fontObj) continue;
            auto* fontObjDict = pdfioObjGetDict(fontObj);
            if (!fontObjDict) continue;
            auto* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
            if (!fontDescObj) continue;
            auto* fontDescDict = pdfioObjGetDict(fontDescObj);
            if (!fontDescDict) continue;
            auto* fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile2");
            if (!fontFileObj) continue;

            auto* stream = pdfioObjOpenStream(fontFileObj, true);
            if (!stream) continue;

            std::vector<uint8_t> data;
            uint8_t buf[4096];
            ssize_t n;
            while ((n = pdfioStreamRead(stream, buf, sizeof(buf))) > 0) {
                data.insert(data.end(), buf, buf + n);
            }
            pdfioStreamClose(stream);

            // Write to temp file
            auto tmpDir = std::filesystem::temp_directory_path() / "yetty-test";
            std::filesystem::create_directories(tmpDir);
            auto outPath = tmpDir / (std::string(tag) + ".ttf");

            std::ofstream out(outPath, std::ios::binary);
            expect(out.good()) << "failed to create " << outPath.string();
            out.write(reinterpret_cast<const char*>(data.data()),
                      static_cast<std::streamsize>(data.size()));
            out.close();

            expect(std::filesystem::exists(outPath));
            expect(std::filesystem::file_size(outPath) == data.size());

            // Cleanup
            std::filesystem::remove(outPath);
            break;  // Only test first font
        }

        pdfioFileClose(pdf);
    };
};

suite ypdf_all_pages_fonts = [] {
    "all pages have extractable fonts"_test = [] {
        auto* pdf = pdfioFileOpen(kSamplePdf.c_str(), nullptr, nullptr,
                                  errorHandler, nullptr);
        expect(pdf != nullptr);
        if (!pdf) return;

        size_t numPages = pdfioFileGetNumPages(pdf);
        size_t pagesWithFonts = 0;

        for (size_t p = 0; p < numPages; p++) {
            auto* pageObj = pdfioFileGetPage(pdf, p);
            if (!pageObj) continue;

            auto* resources = getPageResources(pageObj);
            if (!resources) continue;

            auto* fontDict = getFontDict(resources);
            if (!fontDict) continue;

            size_t numFonts = pdfioDictGetNumPairs(fontDict);
            if (numFonts > 0) pagesWithFonts++;
        }

        expect(pagesWithFonts == numPages)
            << "expected all " << numPages << " pages to have fonts, got "
            << pagesWithFonts;

        pdfioFileClose(pdf);
    };
};
