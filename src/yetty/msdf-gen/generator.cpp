#include "generator.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <ytrace/ytrace.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>

extern "C" {
#include <cdb.h>
#include <cdb_make.h>
}

namespace yetty {
namespace msdfgen {

// Internal glyph result from worker thread
struct GlyphResult {
    uint32_t codepoint;
    MsdfGlyphData header;
    std::vector<uint8_t> pixels;  // RGBA data
    bool success;
};

// Thread-safe work queue
class WorkQueue {
public:
    void push(uint32_t codepoint) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(codepoint);
        _cv.notify_one();
    }

    bool pop(uint32_t& codepoint) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.empty()) return false;
        codepoint = _queue.front();
        _queue.pop();
        return true;
    }

    void finish() {
        std::lock_guard<std::mutex> lock(_mutex);
        _done = true;
        _cv.notify_all();
    }

    bool isDone() const { return _done && _queue.empty(); }

private:
    std::queue<uint32_t> _queue;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _done = false;
};

// Worker context for a single thread
struct WorkerContext {
    ::msdfgen::FreetypeHandle* ft;
    ::msdfgen::FontHandle* font;
    float fontSize;
    float pixelRange;
};

// Generate MSDF for a single glyph
static GlyphResult generateGlyph(WorkerContext& ctx, uint32_t codepoint) {
    GlyphResult result;
    result.codepoint = codepoint;
    result.success = false;

    // Load glyph shape - NO FONT_SCALING_NONE to match old Font class behavior
    ::msdfgen::Shape shape;
    double advance;
    if (!::msdfgen::loadGlyph(shape, ctx.font, codepoint, &advance)) {
        return result;
    }

    // Get font metrics for scaling - NO FONT_SCALING_NONE to match old Font class
    ::msdfgen::FontMetrics metrics;
    ::msdfgen::getFontMetrics(metrics, ctx.font);
    double unitsPerEm = metrics.emSize > 0 ? metrics.emSize :
                        (metrics.ascenderY - metrics.descenderY);
    double fontScale = ctx.fontSize / unitsPerEm;

    // Initialize header
    result.header.codepoint = codepoint;
    result.header.advance = static_cast<float>(advance * fontScale);

    // Handle empty glyphs (like space)
    if (shape.contours.empty()) {
        result.header.width = 0;
        result.header.height = 0;
        result.header.bearingX = 0;
        result.header.bearingY = 0;
        result.header.sizeX = 0;
        result.header.sizeY = 0;
        result.success = true;
        return result;
    }

    // Normalize and get bounds
    shape.normalize();
    ::msdfgen::Shape::Bounds bounds = shape.getBounds();

    // Calculate logical glyph size and bitmap size with padding
    int padding = static_cast<int>(std::ceil(ctx.pixelRange));
    double logicalSizeX = (bounds.r - bounds.l) * fontScale;
    double logicalSizeY = (bounds.t - bounds.b) * fontScale;
    int bitmapW = static_cast<int>(std::ceil(logicalSizeX)) + padding * 2;
    int bitmapH = static_cast<int>(std::ceil(logicalSizeY)) + padding * 2;

    result.header.width = static_cast<uint16_t>(bitmapW);
    result.header.height = static_cast<uint16_t>(bitmapH);

    // Store FINAL values matching old Font class behavior:
    // - sizeX/sizeY = bitmap dimensions (atlasW/atlasH)
    // - bearingX = bounds.l * scale - padding
    // - bearingY = bounds.t * scale + padding
    result.header.sizeX = static_cast<float>(bitmapW);
    result.header.sizeY = static_cast<float>(bitmapH);
    result.header.bearingX = static_cast<float>(bounds.l * fontScale - padding);
    result.header.bearingY = static_cast<float>(bounds.t * fontScale + padding);

    // Apply edge coloring
    ::msdfgen::edgeColoringSimple(shape, 3.0);

    // Generate MSDF bitmap
    ::msdfgen::Bitmap<float, 3> msdf(bitmapW, bitmapH);
    ::msdfgen::Vector2 translate(
        padding - bounds.l * fontScale,
        padding - bounds.b * fontScale
    );
    ::msdfgen::generateMSDF(msdf, shape, ctx.pixelRange, fontScale, translate);

    // Convert to RGBA with Y-flip
    result.pixels.resize(bitmapW * bitmapH * 4);
    for (int y = 0; y < bitmapH; ++y) {
        for (int x = 0; x < bitmapW; ++x) {
            int srcY = bitmapH - 1 - y;  // Flip Y
            size_t idx = (y * bitmapW + x) * 4;
            result.pixels[idx + 0] = static_cast<uint8_t>(std::clamp(msdf(x, srcY)[0] * 255.0f, 0.0f, 255.0f));
            result.pixels[idx + 1] = static_cast<uint8_t>(std::clamp(msdf(x, srcY)[1] * 255.0f, 0.0f, 255.0f));
            result.pixels[idx + 2] = static_cast<uint8_t>(std::clamp(msdf(x, srcY)[2] * 255.0f, 0.0f, 255.0f));
            result.pixels[idx + 3] = 255;
        }
    }

    result.success = true;
    return result;
}

// Worker thread function
static void workerThread(
    WorkQueue& queue,
    std::vector<GlyphResult>& results,
    std::mutex& resultsMutex,
    std::atomic<size_t>& completed,
    const std::string& fontPath,
    float fontSize,
    float pixelRange
) {
    // Each thread gets its own FreeType/font handles
    auto ft = ::msdfgen::initializeFreetype();
    if (!ft) {
        yerror("Worker: Failed to initialize FreeType");
        return;
    }

    auto font = ::msdfgen::loadFont(ft, fontPath.c_str());
    if (!font) {
        yerror("Worker: Failed to load font: {}", fontPath);
        ::msdfgen::deinitializeFreetype(ft);
        return;
    }

    WorkerContext ctx{ft, font, fontSize, pixelRange};

    uint32_t codepoint;
    while (queue.pop(codepoint)) {
        auto result = generateGlyph(ctx, codepoint);

        {
            std::lock_guard<std::mutex> lock(resultsMutex);
            results.push_back(std::move(result));
        }

        ++completed;
    }

    ::msdfgen::destroyFont(font);
    ::msdfgen::deinitializeFreetype(ft);
}

// Write results to CDB file
static bool writeCdb(const std::string& path, const std::vector<GlyphResult>& results) {
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        yerror("Failed to create CDB file: {}", path);
        return false;
    }

    struct cdb_make cdbm;
    if (cdb_make_start(&cdbm, fd) < 0) {
        yerror("cdb_make_start failed");
        close(fd);
        return false;
    }

    for (const auto& r : results) {
        if (!r.success) continue;

        // Key: 4-byte codepoint (little-endian)
        char key[4];
        key[0] = r.codepoint & 0xFF;
        key[1] = (r.codepoint >> 8) & 0xFF;
        key[2] = (r.codepoint >> 16) & 0xFF;
        key[3] = (r.codepoint >> 24) & 0xFF;

        // Value: MsdfGlyphData header + pixel data
        size_t valueSize = sizeof(MsdfGlyphData) + r.pixels.size();
        std::vector<char> value(valueSize);
        std::memcpy(value.data(), &r.header, sizeof(MsdfGlyphData));
        if (!r.pixels.empty()) {
            std::memcpy(value.data() + sizeof(MsdfGlyphData), r.pixels.data(), r.pixels.size());
        }

        if (cdb_make_add(&cdbm, key, 4, value.data(), valueSize) < 0) {
            yerror("cdb_make_add failed for codepoint {}", r.codepoint);
            close(fd);
            return false;
        }
    }

    if (cdb_make_finish(&cdbm) < 0) {
        yerror("cdb_make_finish failed");
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

std::vector<uint32_t> getDefaultCharset(bool includeNerdFonts) {
    std::vector<uint32_t> charset;

    // Basic Latin (ASCII printable)
    for (uint32_t c = 0x20; c <= 0x7E; ++c) charset.push_back(c);

    // Latin-1 Supplement
    for (uint32_t c = 0xA0; c <= 0xFF; ++c) charset.push_back(c);

    // Latin Extended-A
    for (uint32_t c = 0x100; c <= 0x17F; ++c) charset.push_back(c);

    // Latin Extended-B (partial)
    for (uint32_t c = 0x180; c <= 0x24F; ++c) charset.push_back(c);

    // Greek and Coptic
    for (uint32_t c = 0x370; c <= 0x3FF; ++c) charset.push_back(c);

    // Cyrillic
    for (uint32_t c = 0x400; c <= 0x4FF; ++c) charset.push_back(c);

    // General Punctuation
    for (uint32_t c = 0x2000; c <= 0x206F; ++c) charset.push_back(c);

    // Currency Symbols
    for (uint32_t c = 0x20A0; c <= 0x20CF; ++c) charset.push_back(c);

    // Arrows
    for (uint32_t c = 0x2190; c <= 0x21FF; ++c) charset.push_back(c);

    // Mathematical Operators
    for (uint32_t c = 0x2200; c <= 0x22FF; ++c) charset.push_back(c);

    // Box Drawing
    for (uint32_t c = 0x2500; c <= 0x257F; ++c) charset.push_back(c);

    // Block Elements
    for (uint32_t c = 0x2580; c <= 0x259F; ++c) charset.push_back(c);

    // Geometric Shapes
    for (uint32_t c = 0x25A0; c <= 0x25FF; ++c) charset.push_back(c);

    if (includeNerdFonts) {
        // Powerline symbols
        for (uint32_t c = 0xE0A0; c <= 0xE0D7; ++c) charset.push_back(c);

        // Seti-UI + Custom
        for (uint32_t c = 0xE5FA; c <= 0xE6AC; ++c) charset.push_back(c);

        // Devicons
        for (uint32_t c = 0xE700; c <= 0xE7C5; ++c) charset.push_back(c);

        // Font Awesome Extension
        for (uint32_t c = 0xE200; c <= 0xE2A9; ++c) charset.push_back(c);

        // Material Design Icons
        for (uint32_t c = 0xF0001; c <= 0xF1AF0; ++c) charset.push_back(c);

        // Weather Icons
        for (uint32_t c = 0xE300; c <= 0xE3E3; ++c) charset.push_back(c);

        // Octicons
        for (uint32_t c = 0xF400; c <= 0xF532; ++c) charset.push_back(c);

        // Codicons
        for (uint32_t c = 0xEA60; c <= 0xEBEB; ++c) charset.push_back(c);
    }

    return charset;
}

GeneratorResult generate(const GeneratorConfig& config, ProgressCallback progress) {
    GeneratorResult result;

    // Validate config
    if (config.fontPath.empty()) {
        result.error = "Font path is empty";
        return result;
    }

    if (config.outputDir.empty()) {
        result.error = "Output directory is empty";
        return result;
    }

    // Create output directory if needed
    std::filesystem::create_directories(config.outputDir);

    // Determine thread count
    int threadCount = config.threadCount;
    if (threadCount <= 0) {
        threadCount = std::max(1, static_cast<int>(std::thread::hardware_concurrency()));
    }

    yinfo("Using {} threads for MSDF generation", threadCount);

    // Get charset
    auto charset = getDefaultCharset(config.includeNerdFonts);
    yinfo("Charset size: {} codepoints", charset.size());

    // Set up work queue
    WorkQueue queue;
    for (uint32_t cp : charset) {
        queue.push(cp);
    }
    queue.finish();

    // Results collection
    std::vector<GlyphResult> results;
    std::mutex resultsMutex;
    std::atomic<size_t> completed{0};

    // Progress reporting
    auto reportProgress = [&]() {
        if (progress) {
            progress(completed.load(), charset.size(), "Generating MSDF glyphs...");
        }
    };

    // Launch worker threads
    std::vector<std::thread> workers;
    for (int i = 0; i < threadCount; ++i) {
        workers.emplace_back(workerThread,
            std::ref(queue),
            std::ref(results),
            std::ref(resultsMutex),
            std::ref(completed),
            config.fontPath,
            config.fontSize,
            config.pixelRange
        );
    }

    // Wait for completion with progress reporting
    while (completed.load() < charset.size()) {
        reportProgress();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Join all threads
    for (auto& t : workers) {
        t.join();
    }

    reportProgress();

    // Count successful glyphs
    size_t successCount = 0;
    for (const auto& r : results) {
        if (r.success) ++successCount;
    }
    yinfo("Generated {} / {} glyphs successfully", successCount, charset.size());

    // Extract font name for output file
    std::filesystem::path fontPath(config.fontPath);
    std::string fontName = fontPath.stem().string();

    // Write CDB file
    std::string cdbPath = config.outputDir + "/" + fontName + ".cdb";
    if (!writeCdb(cdbPath, results)) {
        result.error = "Failed to write CDB file: " + cdbPath;
        return result;
    }

    result.success = true;
    result.glyphsGenerated = successCount;
    result.outputFiles.push_back(cdbPath);

    yinfo("CDB written to: {}", cdbPath);
    return result;
}

} // namespace msdfgen
} // namespace yetty
