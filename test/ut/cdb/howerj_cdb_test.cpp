//=============================================================================
// howerj/cdb Implementation Tests
//
// Tests for the portable CDB wrapper using howerj/cdb library.
// These tests verify that the CDB reader can:
// 1. Open valid CDB files
// 2. Read glyph data correctly
// 3. Handle missing keys gracefully
// 4. Handle invalid files correctly
//=============================================================================

// Include C++ standard headers before boost/ut.hpp (cdb version file conflict)
#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>
#include <yetty/cdb-wrapper.h>
#include <yetty/msdf-glyph-data.h>

#include <cstring>
#include <filesystem>

using namespace boost::ut;
using namespace yetty;

// Path to test CDB files (from build directory)
static std::string getTestCdbPath() {
    // Check various possible locations
    std::vector<std::string> paths = {
        "assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb",
        "../assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb",
        "../../assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb",
        "../../../build-desktop-dawn-release/assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb",
        "../../../build-webasm-dawn-release/assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb",
    };

    for (const auto& path : paths) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }

    return "";  // Will cause tests to be skipped
}

suite cdb_reader_open_tests = [] {
    "CdbReader::open returns nullptr for non-existent file"_test = [] {
        auto reader = CdbReader::open("/nonexistent/path/to/file.cdb");
        expect(reader == nullptr);
    };

    "CdbReader::open returns nullptr for directory"_test = [] {
        auto reader = CdbReader::open("/tmp");
        expect(reader == nullptr);
    };

    "CdbReader::open returns nullptr for empty path"_test = [] {
        auto reader = CdbReader::open("");
        expect(reader == nullptr);
    };

    "CdbReader::open succeeds for valid CDB file"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            // Skip test if no CDB file available
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr) << "Failed to open: " << path;
    };
};

suite cdb_reader_get_tests = [] {
    "CdbReader::get returns data for ASCII 'A' (0x41)"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        // Look up ASCII 'A' (codepoint 0x41 = 65)
        uint32_t codepoint = 'A';
        auto data = reader->get(codepoint);

        expect(data.has_value()) << "Glyph 'A' not found in CDB";
        if (data.has_value()) {
            expect(data->size() >= sizeof(MsdfGlyphData)) << "Data too small for MsdfGlyphData header";
        }
    };

    "CdbReader::get returns data for space (0x20)"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        uint32_t codepoint = ' ';
        auto data = reader->get(codepoint);

        expect(data.has_value()) << "Glyph ' ' not found in CDB";
    };

    "CdbReader::get returns nullopt for non-existent codepoint"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        // Use a very unlikely codepoint
        uint32_t codepoint = 0xFFFFFFFF;
        auto data = reader->get(codepoint);

        expect(!data.has_value()) << "Should not find invalid codepoint";
    };

    "CdbReader::get with raw key API works"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        // Use raw key API with 4-byte codepoint
        uint32_t codepoint = 'A';
        auto data = reader->get(&codepoint, sizeof(codepoint));

        expect(data.has_value()) << "Raw key API failed for 'A'";
    };
};

suite cdb_glyph_data_tests = [] {
    "MsdfGlyphData header is correctly parsed"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        uint32_t codepoint = 'A';
        auto data = reader->get(codepoint);
        expect(data.has_value());
        if (!data.has_value()) return;

        expect(data->size() >= sizeof(MsdfGlyphData));

        MsdfGlyphData header;
        std::memcpy(&header, data->data(), sizeof(header));

        // Verify header fields are reasonable
        expect(header.codepoint == codepoint) << "Codepoint mismatch: expected " << codepoint << ", got " << header.codepoint;
        expect(header.width > 0) << "Width should be > 0";
        expect(header.height > 0) << "Height should be > 0";
        expect(header.width < 256) << "Width should be < 256";
        expect(header.height < 256) << "Height should be < 256";
        expect(header.advance > 0.0f) << "Advance should be > 0";
    };

    "MsdfGlyphData pixel data size matches header"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        uint32_t codepoint = 'A';
        auto data = reader->get(codepoint);
        expect(data.has_value());
        if (!data.has_value()) return;

        MsdfGlyphData header;
        std::memcpy(&header, data->data(), sizeof(header));

        size_t expectedSize = sizeof(MsdfGlyphData) + (header.width * header.height * 4);
        expect(data->size() == expectedSize)
            << "Data size mismatch: expected " << expectedSize
            << ", got " << data->size();
    };

    "Multiple glyphs can be loaded sequentially"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        // Load basic Latin characters
        const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        int loadedCount = 0;

        for (const char* p = chars; *p; ++p) {
            uint32_t codepoint = static_cast<uint32_t>(*p);
            auto data = reader->get(codepoint);
            if (data.has_value()) {
                ++loadedCount;

                // Quick validation
                MsdfGlyphData header;
                std::memcpy(&header, data->data(), sizeof(header));
                expect(header.codepoint == codepoint);
            }
        }

        expect(loadedCount > 50) << "Should load most basic Latin glyphs, got " << loadedCount;
    };

    "Same glyph can be loaded multiple times"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);

        uint32_t codepoint = 'X';

        // Load the same glyph multiple times
        auto data1 = reader->get(codepoint);
        auto data2 = reader->get(codepoint);
        auto data3 = reader->get(codepoint);

        expect(data1.has_value());
        expect(data2.has_value());
        expect(data3.has_value());

        if (data1.has_value() && data2.has_value() && data3.has_value()) {
            // All should return identical data
            expect(data1->size() == data2->size());
            expect(data2->size() == data3->size());
            expect(std::memcmp(data1->data(), data2->data(), data1->size()) == 0);
        }
    };
};

suite cdb_writer_tests = [] {
    "CdbWriter::create creates new file"_test = [] {
        std::string testPath = "/tmp/test_cdb_writer.cdb";

        // Clean up any existing file
        std::filesystem::remove(testPath);

        auto writer = CdbWriter::create(testPath);
        expect(writer != nullptr) << "Failed to create CDB writer";
        if (!writer) return;

        // Add a test entry
        uint32_t key = 0x12345678;
        std::vector<uint8_t> value = {1, 2, 3, 4, 5};
        bool added = writer->add(&key, sizeof(key), value.data(), value.size());
        expect(added) << "Failed to add entry";

        // Finalize
        bool finished = writer->finish();
        expect(finished) << "Failed to finish CDB";

        // Verify file was created
        expect(std::filesystem::exists(testPath));

        // Verify we can read it back
        auto reader = CdbReader::open(testPath);
        expect(reader != nullptr) << "Failed to open written CDB";
        if (!reader) return;

        auto readData = reader->get(&key, sizeof(key));
        expect(readData.has_value()) << "Failed to read back written data";
        if (readData.has_value()) {
            expect(readData->size() == value.size());
            expect(std::memcmp(readData->data(), value.data(), value.size()) == 0);
        }

        // Clean up
        std::filesystem::remove(testPath);
    };

    "CdbWriter::create fails for invalid path"_test = [] {
        auto writer = CdbWriter::create("/nonexistent/directory/test.cdb");
        expect(writer == nullptr);
    };

    "CdbWriter can write multiple entries"_test = [] {
        std::string testPath = "/tmp/test_cdb_multi.cdb";
        std::filesystem::remove(testPath);

        auto writer = CdbWriter::create(testPath);
        expect(writer != nullptr);
        if (!writer) return;

        // Write 100 entries
        for (uint32_t i = 0; i < 100; ++i) {
            std::vector<uint8_t> value(i + 1, static_cast<uint8_t>(i & 0xFF));
            bool added = writer->add(&i, sizeof(i), value.data(), value.size());
            expect(added);
        }

        expect(writer->finish());

        // Verify we can read all entries back
        auto reader = CdbReader::open(testPath);
        expect(reader != nullptr);
        if (!reader) return;

        for (uint32_t i = 0; i < 100; ++i) {
            auto data = reader->get(&i, sizeof(i));
            expect(data.has_value()) << "Missing entry " << i;
            if (data.has_value()) {
                expect(data->size() == i + 1) << "Wrong size for entry " << i;
            }
        }

        std::filesystem::remove(testPath);
    };

    "CdbWriter uint32_t key convenience overload works"_test = [] {
        std::string testPath = "/tmp/test_cdb_uint32.cdb";
        std::filesystem::remove(testPath);

        auto writer = CdbWriter::create(testPath);
        expect(writer != nullptr);
        if (!writer) return;

        uint32_t key = 42;
        std::vector<uint8_t> value = {0xDE, 0xAD, 0xBE, 0xEF};

        // Use the uint32_t convenience overload
        bool added = writer->add(key, value.data(), value.size());
        expect(added);
        expect(writer->finish());

        // Read back using uint32_t convenience overload
        auto reader = CdbReader::open(testPath);
        expect(reader != nullptr);
        if (!reader) return;

        auto data = reader->get(key);
        expect(data.has_value());
        if (data.has_value()) {
            expect(data->size() == 4u);
            expect((*data)[0] == 0xDE);
            expect((*data)[1] == 0xAD);
            expect((*data)[2] == 0xBE);
            expect((*data)[3] == 0xEF);
        }

        std::filesystem::remove(testPath);
    };
};

suite cdb_stress_tests = [] {
    "Reader handles rapid sequential lookups"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);
        if (!reader) return;

        // Perform 1000 lookups
        for (int i = 0; i < 1000; ++i) {
            uint32_t codepoint = 'A' + (i % 26);
            auto data = reader->get(codepoint);
            // Just verify no crashes - data may or may not exist
            (void)data;
        }
        // If we get here without crashing, test passes
        expect(true);
    };

    "Reader survives lookup after failed lookup"_test = [] {
        auto path = getTestCdbPath();
        if (path.empty()) {
            expect(true) << "SKIPPED: No test CDB file found";
            return;
        }

        auto reader = CdbReader::open(path);
        expect(reader != nullptr);
        if (!reader) return;

        // Look up non-existent key
        uint32_t badKey = 0xDEADBEEF;
        auto bad = reader->get(badKey);
        expect(!bad.has_value());

        // Reader should still work for valid keys
        uint32_t goodKey = 'A';
        auto good = reader->get(goodKey);
        expect(good.has_value()) << "Reader broken after failed lookup";
    };
};
