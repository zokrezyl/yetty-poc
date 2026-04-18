//=============================================================================
// YHtml Tests - Main Entry Point
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>
#include <cstring>
#include <map>

#include <boost/ut.hpp>

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include "html-renderer.h"

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Helper: read u32 from float buffer at word index
//=============================================================================
static uint32_t readU32(const float* buf, uint32_t wordIdx) {
    uint32_t val;
    std::memcpy(&val, &buf[wordIdx], sizeof(uint32_t));
    return val;
}

//=============================================================================
// Helper: get primitive type name
//=============================================================================
static const char* primTypeName(uint32_t type) {
    switch (type) {
        case 0: return "Circle";
        case 1: return "Box";
        case 2: return "Segment";
        case 3: return "Triangle";
        case 4: return "Bezier2";
        case 5: return "Bezier3";
        case 6: return "Ellipse";
        case 7: return "Arc";
        case 8: return "RoundedBox";
        default: return "Unknown";
    }
}

//=============================================================================
// Helper: dump buffer primitives for debugging
//=============================================================================
static void dumpBufferPrimitives(YDrawBuffer::Ptr buffer) {
    std::map<uint32_t, int> typeCounts;

    buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t wc) {
        uint32_t type = readU32(data, 0);
        uint32_t layer = readU32(data, 1);
        typeCounts[type]++;

        std::printf("  Prim id=%u type=%u(%s) layer=%u wc=%u\n",
                    id, type, primTypeName(type), layer, wc);

        // Print coordinates based on type
        if (type == 1) { // Box
            std::printf("    Box: cx=%.1f cy=%.1f hw=%.1f hh=%.1f fill=0x%08X stroke=0x%08X sw=%.1f round=%.1f\n",
                        data[2], data[3], data[4], data[5],
                        readU32(data, 6), readU32(data, 7), data[8], data[9]);
        } else if (type == 2) { // Segment
            std::printf("    Segment: x0=%.1f y0=%.1f x1=%.1f y1=%.1f fill=0x%08X stroke=0x%08X sw=%.1f round=%.1f\n",
                        data[2], data[3], data[4], data[5],
                        readU32(data, 6), readU32(data, 7), data[8], data[9]);
        } else if (type == 0) { // Circle
            std::printf("    Circle: cx=%.1f cy=%.1f r=%.1f fill=0x%08X stroke=0x%08X sw=%.1f\n",
                        data[2], data[3], data[4],
                        readU32(data, 5), readU32(data, 6), data[7]);
        } else if (type == 3) { // Triangle
            std::printf("    Triangle: x0=%.1f y0=%.1f x1=%.1f y1=%.1f x2=%.1f y2=%.1f fill=0x%08X stroke=0x%08X sw=%.1f\n",
                        data[2], data[3], data[4], data[5], data[6], data[7],
                        readU32(data, 8), readU32(data, 9), data[10]);
        }
    });

    std::printf("  Primitive counts:\n");
    for (auto& [type, count] : typeCounts) {
        std::printf("    %s: %d\n", primTypeName(type), count);
    }

    std::printf("  Text spans: %u\n", buffer->textSpanCount());
    std::printf("  Images: %u\n", buffer->imageCount());
}

//=============================================================================
// Tests run inside main to avoid static init issues with litehtml
//=============================================================================
int main() {
    // Test 1: Simple table
    "simple_table_produces_boxes_and_segments"_test = [] {
        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        const char* html = R"(
            <html>
            <head><style>
                table { border-collapse: collapse; }
                td { border: 1px solid black; background: #cccccc; padding: 5px; }
            </style></head>
            <body>
                <table>
                    <tr><td>Cell1</td><td>Cell2</td></tr>
                </table>
            </body>
            </html>
        )";

        auto result = renderHtmlToBuffer(html, buffer, 16.0f, 400.0f, nullptr);

        std::printf("\n=== Simple Table Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        uint32_t boxCount = 0, segmentCount = 0, circleCount = 0, triangleCount = 0, otherCount = 0;
        buffer->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type = readU32(data, 0);
            uint32_t layer = readU32(data, 1);

            expect(layer == 0_u) << "Layer should be 0, got " << layer;

            if (type == 0) circleCount++;
            else if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else if (type == 3) triangleCount++;
            else otherCount++;
        });

        std::printf("  Boxes: %u, Segments: %u, Circles: %u, Triangles: %u, Other: %u\n",
                    boxCount, segmentCount, circleCount, triangleCount, otherCount);

        // Simple 2-cell table expected counts (verified working output)
        expect(boxCount == 2_u) << "Expected 2 boxes, got " << boxCount;
        expect(segmentCount == 8_u) << "Expected 8 segments, got " << segmentCount;
        expect(circleCount == 0_u) << "Expected 0 circles, got " << circleCount;
        expect(triangleCount == 0_u) << "Expected 0 triangles, got " << triangleCount;
        expect(otherCount == 0_u) << "Expected 0 other primitives, got " << otherCount;
    };

    // Test 2: table.html content (copied from demo/assets/yhtml/table.html)
    "table_html_test"_test = [] {
        const char* html = R"(<!DOCTYPE html>
<html>
<head>
  <title>Table Demo</title>
  <style>
    body { font-size: 13px; color: #d0d0d0; margin: 10px; }
    h2 { color: #89b4fa; }
    table {
      border-collapse: collapse;
      width: 100%;
      margin-bottom: 15px;
    }
    th {
      background-color: #313244;
      color: #cdd6f4;
      padding: 8px 12px;
      border: 1px solid #45475a;
      text-align: left;
    }
    td {
      padding: 6px 12px;
      border: 1px solid #45475a;
    }
    tr:nth-child(even) td {
      background-color: #1e1e2e;
    }
    tr:nth-child(odd) td {
      background-color: #181825;
    }
    .status-ok { color: #a6e3a1; }
    .status-warn { color: #f9e2af; }
    .status-err { color: #f38ba8; }
    .mono { font-family: monospace; font-size: 12px; }
  </style>
</head>
<body>
  <h2>System Status</h2>
  <table>
    <tr>
      <th>Service</th>
      <th>Port</th>
      <th>Status</th>
      <th>Uptime</th>
    </tr>
    <tr>
      <td>Web Server</td>
      <td class="mono">8080</td>
      <td class="status-ok">Running</td>
      <td>14d 6h 23m</td>
    </tr>
    <tr>
      <td>Database</td>
      <td class="mono">5432</td>
      <td class="status-ok">Running</td>
      <td>14d 6h 20m</td>
    </tr>
    <tr>
      <td>Cache</td>
      <td class="mono">6379</td>
      <td class="status-warn">Degraded</td>
      <td>2d 11h 45m</td>
    </tr>
    <tr>
      <td>Worker</td>
      <td class="mono">9090</td>
      <td class="status-err">Stopped</td>
      <td>-</td>
    </tr>
    <tr>
      <td>Monitor</td>
      <td class="mono">3000</td>
      <td class="status-ok">Running</td>
      <td>7d 3h 12m</td>
    </tr>
  </table>

  <h2>Resource Usage</h2>
  <table>
    <tr>
      <th>Resource</th>
      <th>Used</th>
      <th>Total</th>
      <th>Percent</th>
    </tr>
    <tr>
      <td>CPU</td>
      <td>4 cores</td>
      <td>8 cores</td>
      <td class="status-warn">50%</td>
    </tr>
    <tr>
      <td>Memory</td>
      <td>12.4 GB</td>
      <td>32 GB</td>
      <td class="status-ok">39%</td>
    </tr>
    <tr>
      <td>Disk</td>
      <td>186 GB</td>
      <td>512 GB</td>
      <td class="status-ok">36%</td>
    </tr>
    <tr>
      <td>Network</td>
      <td>824 Mbps</td>
      <td>1 Gbps</td>
      <td class="status-err">82%</td>
    </tr>
  </table>
</body>
</html>
)";

        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        auto result = renderHtmlToBuffer(html, buffer, 13.0f, 600.0f, nullptr);

        std::printf("\n=== table.html Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        uint32_t boxCount = 0, segmentCount = 0, circleCount = 0, triangleCount = 0;
        bool hasInvalidType = false;
        bool hasInvalidLayer = false;

        buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t) {
            uint32_t type = readU32(data, 0);
            uint32_t layer = readU32(data, 1);

            if (layer != 0) {
                hasInvalidLayer = true;
                std::printf("  ERROR: Prim %u has layer=%u (expected 0)\n", id, layer);
            }

            if (type == 0) circleCount++;
            else if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else if (type == 3) triangleCount++;
            else {
                hasInvalidType = true;
                std::printf("  ERROR: Prim %u has unexpected type=%u (%s)\n",
                            id, type, primTypeName(type));
            }
        });

        expect(!hasInvalidType) << "Should not have unexpected primitive types";
        expect(!hasInvalidLayer) << "All layers should be 0";

        std::printf("  Boxes: %u, Segments: %u, Circles: %u, Triangles: %u\n",
                    boxCount, segmentCount, circleCount, triangleCount);

        // table.html expected counts (verified working output)
        expect(boxCount == 44_u) << "Expected 44 boxes, got " << boxCount;
        expect(segmentCount == 176_u) << "Expected 176 segments, got " << segmentCount;
        expect(circleCount == 0_u) << "Expected 0 circles, got " << circleCount;
        expect(triangleCount == 0_u) << "Expected 0 triangles, got " << triangleCount;
    };

    // Test 3: form.html content (copied from demo/assets/yhtml/form.html)
    "form_html_test"_test = [] {
        const char* html = R"(<!DOCTYPE html>
<html>
<head>
  <title>Form Layout Test</title>
  <style>
    body {
      font-size: 13px;
      color: #cdd6f4;
      background-color: #313244;
      margin: 15px;
    }
    h2 { color: #89b4fa; margin-bottom: 10px; }
    .form-group {
      margin-bottom: 10px;
    }
    label {
      display: block;
      color: #a6adc8;
      margin-bottom: 3px;
      font-size: 12px;
    }
    .input-field {
      background-color: #1e1e2e;
      border: 1px solid #45475a;
      border-radius: 4px;
      padding: 8px 10px;
      color: #cdd6f4;
      width: 90%;
    }
    .row {
      margin-bottom: 8px;
    }
    .btn {
      display: inline-block;
      padding: 8px 16px;
      border-radius: 4px;
      color: #ffffff;
      margin-right: 8px;
    }
    .btn-primary {
      background-color: #6c63ff;
      border: 1px solid #7c73ff;
    }
    .btn-secondary {
      background-color: #45475a;
      border: 1px solid #585b70;
    }
    .divider {
      border-top: 1px solid #313244;
      margin: 15px 0;
    }
    .note {
      background-color: #1e1e2e;
      border-left: 3px solid #f9e2af;
      padding: 8px 12px;
      color: #a6adc8;
      font-size: 12px;
      margin-top: 10px;
    }
    blockquote {
      border-left: 3px solid #6c63ff;
      margin: 10px 0;
      padding: 5px 15px;
      color: #9399b2;
    }
  </style>
</head>
<body>
  <h2>Contact Form</h2>

  <div class="form-group">
    <label>Full Name</label>
    <div class="input-field">John Doe</div>
  </div>

  <div class="form-group">
    <label>Email Address</label>
    <div class="input-field">john@example.com</div>
  </div>

  <div class="form-group">
    <label>Subject</label>
    <div class="input-field">HTML rendering in terminal</div>
  </div>

  <div class="form-group">
    <label>Message</label>
    <div class="input-field" style="min-height: 60px;">
      This is a test of CSS layout rendering using litehtml
      within the yetty terminal emulator. The form elements
      are pure HTML/CSS styled boxes.
    </div>
  </div>

  <div class="row">
    <span class="btn btn-primary">Send Message</span>
    <span class="btn btn-secondary">Cancel</span>
  </div>

  <div class="divider"></div>

  <blockquote>
    Forms are rendered as styled boxes. Interactive input requires
    JavaScript integration (planned via QuickJS).
  </blockquote>

  <div class="note">
    Note: This demonstrates CSS box model rendering including padding,
    margins, borders, border-radius, and background colors.
  </div>
</body>
</html>
)";

        auto bufferRes = YDrawBuffer::create();
        expect(bufferRes.has_value());
        auto buffer = *bufferRes;

        auto result = renderHtmlToBuffer(html, buffer, 16.0f, 600.0f, nullptr);

        std::printf("\n=== form.html Test ===\n");
        std::printf("Document: %dx%d\n", result.documentWidth, result.documentHeight);
        std::printf("Primitives in buffer: %u\n", buffer->primCount());

        dumpBufferPrimitives(buffer);

        expect(buffer->primCount() > 0_u) << "Buffer should have primitives";

        uint32_t boxCount = 0, segmentCount = 0, circleCount = 0, triangleCount = 0;
        bool hasInvalidType = false;
        bool hasInvalidLayer = false;

        buffer->forEachPrim([&](uint32_t id, const float* data, uint32_t) {
            uint32_t type = readU32(data, 0);
            uint32_t layer = readU32(data, 1);

            if (layer != 0) {
                hasInvalidLayer = true;
                std::printf("  ERROR: Prim %u has layer=%u (expected 0)\n", id, layer);
            }

            if (type == 0) circleCount++;
            else if (type == 1) boxCount++;
            else if (type == 2) segmentCount++;
            else if (type == 3) triangleCount++;
            else {
                hasInvalidType = true;
                std::printf("  ERROR: Prim %u has unexpected type=%u (%s)\n",
                            id, type, primTypeName(type));
            }
        });

        expect(!hasInvalidType) << "Should not have unexpected primitive types";
        expect(!hasInvalidLayer) << "All layers should be 0";
        std::printf("  Boxes: %u, Segments: %u, Circles: %u, Triangles: %u\n",
                    boxCount, segmentCount, circleCount, triangleCount);

        // form.html expected counts (verified working output)
        expect(boxCount == 7_u) << "Expected 7 boxes, got " << boxCount;
        expect(segmentCount == 27_u) << "Expected 27 segments, got " << segmentCount;
        expect(circleCount == 0_u) << "Expected 0 circles, got " << circleCount;
        expect(triangleCount == 0_u) << "Expected 0 triangles, got " << triangleCount;
    };
}

