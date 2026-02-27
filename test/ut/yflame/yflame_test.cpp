//=============================================================================
// yflame Unit Tests
//
// Tests FlameRenderer: collapsed stack parsing, tree building,
// rendering into YDrawBuffer, serialization, and OSC output format.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

#include "flame-renderer.h"
#include "yetty/ydraw/ydraw-buffer.h"
#include "osc.h"

#include <cstring>
#include <cmath>
#include <string>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::flame;

//=============================================================================
// Collapsed Stack Parsing Tests
//=============================================================================

suite parsing_tests = [] {

    "empty input produces zero samples"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("");
        expect(renderer.totalSamples() == 0_u);
    };

    "comment lines are skipped"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("# comment\n# another\n");
        expect(renderer.totalSamples() == 0_u);
    };

    "single stack line"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo;bar 100\n");
        expect(renderer.totalSamples() == 100_u);
    };

    "multiple stack lines accumulate"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );
        expect(renderer.totalSamples() == 180_u);
    };

    "duplicate stacks merge correctly"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo 10\n"
            "main;foo 20\n"
        );
        expect(renderer.totalSamples() == 30_u);
    };

    "lines without count are skipped"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo\n");
        expect(renderer.totalSamples() == 0_u);
    };

    "clearTree resets everything"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");
        expect(renderer.totalSamples() == 100_u);

        renderer.clearTree();
        expect(renderer.totalSamples() == 0_u);
    };

    "reload after clear"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");
        renderer.clearTree();
        renderer.loadCollapsedStacks("main;bar 50\n");
        expect(renderer.totalSamples() == 50_u);
    };

    "single frame stack"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main 42\n");
        expect(renderer.totalSamples() == 42_u);
    };

    "deep stack"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("a;b;c;d;e;f;g;h;i;j 5\n");
        expect(renderer.totalSamples() == 5_u);
    };
};

//=============================================================================
// Rendering Tests — verify FlameRenderer produces correct buffer contents
//=============================================================================

suite render_tests = [] {

    "render with zero samples shows message"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        ViewState view;

        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        // Should show a "no samples" message text span
        expect(buffer->primCount() == 0_u);
        expect(buffer->textSpanCount() == 1_u)
            << "should have 1 text span with no-samples message";

        bool foundMsg = false;
        buffer->forEachTextSpan([&](const TextSpanData& span) {
            if (span.text.find("No samples") != std::string::npos) foundMsg = true;
        });
        expect(foundMsg) << "should have 'No samples' message";
    };

    "render produces box primitives"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        // Should have boxes: root + main + foo + bar + baz + qux = 6
        expect(buffer->primCount() >= 6_u)
            << "expected >= 6 box prims, got " << buffer->primCount();
    };

    "render produces text spans for wide frames"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        // Root, main, foo should all be wide enough for text
        // bar and baz may or may not depending on proportions
        expect(buffer->textSpanCount() >= 3_u)
            << "expected >= 3 text spans, got " << buffer->textSpanCount();
    };

    "render sets scene bounds"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        expect(buffer->hasSceneBounds()) << "scene bounds should be set";
        expect(std::abs(buffer->sceneMinX() - 0.0f) < 0.01f) << "sceneMinX";
        expect(std::abs(buffer->sceneMinY() - 0.0f) < 0.01f) << "sceneMinY";
        expect(buffer->sceneMaxX() > 0.0f) << "sceneMaxX should be > 0";
        expect(buffer->sceneMaxY() > 0.0f) << "sceneMaxY should be > 0";
    };

    "render sets background color"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main 10\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        expect(buffer->bgColor() == cfg.bgColor)
            << "bgColor should match config";
    };

    "text spans contain function names"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;myFunction 100\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        bool foundMain = false;
        bool foundMyFunction = false;
        buffer->forEachTextSpan([&](const TextSpanData& span) {
            if (span.text.find("main") != std::string::npos) foundMain = true;
            if (span.text.find("myFunction") != std::string::npos) foundMyFunction = true;
        });
        expect(foundMain) << "should have text span containing 'main'";
        expect(foundMyFunction) << "should have text span containing 'myFunction'";
    };

    "zoom root re-layouts children to fill width"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo;bar 100\nmain;foo;baz 50\nmain;qux 30\n");

        // Without zoom root, scene bounds are fixed
        ViewState view;
        auto buf1 = *YDrawBuffer::create();
        renderer.render(buf1, view);
        auto bytes1 = buf1->serialize();

        // Set zoom root to a child — should produce different layout
        auto hit = renderer.hitTest(100.0f, 30.0f + renderer.dynamicFrameHeight() + 1.0f);
        expect(hit.node != nullptr);

        renderer.setZoomRoot(hit.node);
        auto buf2 = *YDrawBuffer::create();
        renderer.render(buf2, view);
        auto bytes2 = buf2->serialize();

        // Different layout = different serialized bytes
        expect(bytes1 != bytes2) << "zoom root should change rendered output";

        // Scene bounds should stay fixed
        expect(std::abs(buf2->sceneMinX()) < 0.01f);
        expect(std::abs(buf2->sceneMaxX() - cfg.sceneWidth) < 0.01f);
    };

    "zoom root reset restores original layout"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo;bar 100\nmain;foo;baz 50\nmain;qux 30\n");

        ViewState view;
        auto buf1 = *YDrawBuffer::create();
        renderer.render(buf1, view);
        auto bytes1 = buf1->serialize();

        // Zoom in then back out
        auto hit = renderer.hitTest(100.0f, 30.0f + renderer.dynamicFrameHeight() + 1.0f);
        renderer.setZoomRoot(hit.node);
        renderer.setZoomRoot(nullptr);

        auto buf2 = *YDrawBuffer::create();
        renderer.render(buf2, view);
        auto bytes2 = buf2->serialize();

        expect(bytes1 == bytes2) << "resetting zoom root should restore original layout";
    };

    "many stacks produce proportional boxes"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);

        // Create stacks where foo has 3x the samples of bar
        renderer.loadCollapsedStacks(
            "root;foo 300\n"
            "root;bar 100\n"
        );

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        // Should have at least 4 boxes: root-bar + root + foo + bar
        expect(buffer->primCount() >= 4_u);
    };
};

//=============================================================================
// Serialization Tests — verify the buffer produces valid wire format
//=============================================================================

suite serialization_tests = [] {

    "rendered buffer serializes to valid YDRF"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
        );

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        auto data = buffer->serialize();
        expect(data.size() > 8_u) << "serialized data should have header + content";

        // Check YDRF magic
        uint32_t magic;
        std::memcpy(&magic, data.data(), 4);
        expect(magic == 0x59445246u) << "magic should be YDRF";

        // Check version
        uint32_t version;
        std::memcpy(&version, data.data() + 4, 4);
        expect(version == 1_u) << "version should be 1";
    };

    "serialized data deserializes correctly"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        auto data = buffer->serialize();

        // Deserialize into new buffer
        auto buf2 = *YDrawBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value()) << "deserialize should succeed";

        // Prim count should match
        expect(buf2->primCount() == buffer->primCount())
            << "deserialized primCount should match original";

        // Text span count should match
        expect(buf2->textSpanCount() == buffer->textSpanCount())
            << "deserialized textSpanCount should match original";

        // Scene metadata should match
        expect(buf2->hasSceneBounds()) << "should have scene bounds";
        expect(buf2->bgColor() == cfg.bgColor) << "bgColor should match";
    };

    "serialized data is non-trivial size"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo;bar 100\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        auto data = buffer->serialize();
        // With 4 boxes + text, serialized data should be > 100 bytes
        expect(data.size() > 100_u)
            << "serialized flamegraph should be substantial, got " << data.size();
    };
};

//=============================================================================
// OSC Output Format Tests
//=============================================================================

suite osc_tests = [] {

    "base64 encode produces valid output"_test = [] {
        std::vector<uint8_t> data = {0x59, 0x44, 0x52, 0x46}; // "YDRF"
        auto b64 = ycat::base64Encode(data);
        expect(!b64.empty()) << "base64 should produce output";
        // "YDRF" in base64 is "WURSRA=="  (well, let's just check non-empty and valid chars)
        for (char c : b64) {
            bool valid = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                         (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=';
            expect(valid) << "invalid base64 char: " << c;
        }
    };

    "full OSC create sequence format"_test = [] {
        // Simulate what yflame does for the create sequence
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        auto binary = buffer->serialize();
        auto b64 = ycat::base64Encode(binary);

        int cols = 80, rows = 24;
        const char* cardName = "yflame-app";

        // Build the OSC create sequence (same as main.cpp sendOscCreate)
        char header[512];
        std::snprintf(header, sizeof(header),
            "\033]666666;run -c ydraw -x 0 -y 0 -w %d -h %d -r --name %s;;",
            cols, rows, cardName);

        std::string seq;
        seq += header;
        seq += b64;
        seq += "\033\\";

        // Verify sequence structure
        expect(seq.substr(0, 2) == "\033]") << "should start with ESC ]";
        expect(seq.find("666666") != std::string::npos) << "should contain vendor ID";
        expect(seq.find("run -c ydraw") != std::string::npos) << "should contain run command";
        expect(seq.find("-w 80") != std::string::npos) << "should contain width";
        expect(seq.find("-h 24") != std::string::npos) << "should contain height";
        expect(seq.find("--name yflame-app") != std::string::npos) << "should contain name";
        expect(seq.find(";;") != std::string::npos) << "should have ;; separator";
        expect(seq.substr(seq.size() - 2) == "\033\\") << "should end with ST";

        // The base64 payload should be between ;; and ST
        auto payloadStart = seq.find(";;") + 2;
        auto payloadEnd = seq.size() - 2;
        expect(payloadEnd > payloadStart) << "payload should be non-empty";
    };

    "full OSC update sequence format"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        auto binary = buffer->serialize();
        auto b64 = ycat::base64Encode(binary);

        const char* cardName = "yflame-app";

        // Build the OSC update sequence (same as main.cpp sendOscUpdate)
        char header[512];
        std::snprintf(header, sizeof(header),
            "\033]666666;update --name %s;;", cardName);

        std::string seq;
        seq += header;
        seq += b64;
        seq += "\033\\";

        expect(seq.find("update --name yflame-app") != std::string::npos)
            << "should contain update command with name";
        expect(seq.substr(seq.size() - 2) == "\033\\") << "should end with ST";
    };

    "OSC kill sequence format"_test = [] {
        const char* cardName = "yflame-app";
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "\033]666666;kill --name %s\033\\", cardName);
        std::string seq(buf);

        expect(seq.find("kill --name yflame-app") != std::string::npos)
            << "should contain kill command";
        expect(seq.substr(seq.size() - 2) == "\033\\") << "should end with ST";
    };
};

//=============================================================================
// ViewState Tests
//=============================================================================

suite view_state_tests = [] {

    "default view state"_test = [] {
        ViewState view;
        expect(view.offsetX == 0.0_f);
        expect(view.offsetY == 0.0_f);
        expect(view.zoom == 1.0_f);
    };

    "zoom in increases zoom"_test = [] {
        ViewState view;
        view.zoomIn();
        expect(view.zoom > 1.0_f);
    };

    "zoom out does not go below 1.0"_test = [] {
        ViewState view;
        view.zoomOut();
        expect(view.zoom >= 1.0_f);
    };

    "pan changes offset"_test = [] {
        ViewState view;
        view.panRight(50.0f);
        expect(view.offsetX == 50.0_f);
        view.panDown(30.0f);
        expect(view.offsetY == 30.0_f);
    };

    "reset restores defaults"_test = [] {
        ViewState view;
        view.panRight(100.0f);
        view.panDown(200.0f);
        view.zoomIn();
        view.reset();
        expect(view.offsetX == 0.0_f);
        expect(view.offsetY == 0.0_f);
        expect(view.zoom == 1.0_f);
    };

    "pan scales with zoom"_test = [] {
        ViewState view;
        view.zoom = 2.0f;
        view.panRight(100.0f);
        // With zoom=2, panRight(100) should add 100/2 = 50 to offsetX
        expect(std::abs(view.offsetX - 50.0f) < 0.01f)
            << "pan should be scaled by zoom: " << view.offsetX;
    };

    "zoomToFrame sets correct zoom and offset"_test = [] {
        ViewState view;
        // Frame at x=300, width=200, scene=1200
        view.zoomToFrame(300.0f, 200.0f, 1200.0f);
        expect(std::abs(view.zoom - 6.0f) < 0.01f)
            << "zoom should be 1200/200 = 6.0, got " << view.zoom;
        expect(std::abs(view.offsetX - 300.0f) < 0.01f)
            << "offsetX should be 300, got " << view.offsetX;
        expect(std::abs(view.offsetY - 0.0f) < 0.01f)
            << "offsetY should be 0, got " << view.offsetY;
    };

    "zoomToFrame then reset"_test = [] {
        ViewState view;
        view.zoomToFrame(300.0f, 200.0f, 1200.0f);
        view.reset();
        expect(view.offsetX == 0.0_f);
        expect(view.offsetY == 0.0_f);
        expect(view.zoom == 1.0_f);
    };
};

//=============================================================================
// Hit Test Tests
//=============================================================================

suite hit_test_tests = [] {

    "hitTest returns null for empty tree"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        auto hit = renderer.hitTest(100.0f, 100.0f);
        expect(hit.node == nullptr) << "empty tree should return null";
    };

    "hitTest finds root bar"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");
        float fh = renderer.dynamicFrameHeight();
        // Root bar is at y=30, height=fh
        auto hit = renderer.hitTest(600.0f, 30.0f + fh * 0.5f);
        expect(hit.node != nullptr) << "should hit root";
        expect(hit.depth == 0_i) << "root is depth 0";
        expect(std::abs(hit.width - cfg.sceneWidth) < 0.01f)
            << "root should span full width";
    };

    "hitTest finds child frame"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo 100\n");
        float fh = renderer.dynamicFrameHeight();
        // main is at depth 1: y = 30 + fh
        auto hit = renderer.hitTest(600.0f, 30.0f + fh * 1.5f);
        expect(hit.node != nullptr) << "should hit main";
        expect(hit.node->name == "main") << "should be 'main', got " << hit.node->name;
        expect(hit.depth == 1_i) << "main is depth 1";
    };

    "hitTest finds deepest frame"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;foo;bar 100\n");
        float fh = renderer.dynamicFrameHeight();
        // bar is at depth 3: y = 30 + 3*fh
        auto hit = renderer.hitTest(600.0f, 30.0f + fh * 3.5f);
        expect(hit.node != nullptr) << "should hit bar";
        expect(hit.node->name == "bar") << "should be 'bar', got " << hit.node->name;
        expect(hit.depth == 3_i) << "bar is depth 3";
    };

    "hitTest returns null for empty space below frames"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main 100\n");
        // clicking at y=790 should be well below the frames
        auto hit = renderer.hitTest(600.0f, 790.0f);
        expect(hit.node == nullptr) << "should be empty space below frames";
    };

    "hitTest finds correct sibling by x position"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo 100\n"
            "main;bar 100\n"
        );
        float fh = renderer.dynamicFrameHeight();
        // Both children of main have equal width (600px each)
        // depth 2: y = 30 + 2*fh
        auto hitLeft = renderer.hitTest(300.0f, 30.0f + fh * 2.5f);
        auto hitRight = renderer.hitTest(900.0f, 30.0f + fh * 2.5f);
        expect(hitLeft.node != nullptr) << "should hit left child";
        expect(hitRight.node != nullptr) << "should hit right child";
        expect(hitLeft.node->name != hitRight.node->name)
            << "left and right should be different frames";
    };
};

//=============================================================================
// PerfProfiler collapseStacks Tests (using public snapshot interface indirectly)
//=============================================================================

suite collapse_tests = [] {

    "collapsed format roundtrip"_test = [] {
        // Verify that loading collapsed stacks and re-rendering
        // produces consistent results
        FlameConfig cfg;
        FlameRenderer r1(cfg);
        r1.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );

        FlameRenderer r2(cfg);
        r2.loadCollapsedStacks(
            "main;foo;bar 100\n"
            "main;foo;baz 50\n"
            "main;qux 30\n"
        );

        expect(r1.totalSamples() == r2.totalSamples())
            << "same input should produce same total";

        ViewState view;
        auto buf1 = *YDrawBuffer::create();
        auto buf2 = *YDrawBuffer::create();
        r1.render(buf1, view);
        r2.render(buf2, view);

        expect(buf1->primCount() == buf2->primCount())
            << "same input should produce same prim count";
        expect(buf1->textSpanCount() == buf2->textSpanCount())
            << "same input should produce same text count";
    };

    "stacks with spaces in function names"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        // The last space separates the count
        renderer.loadCollapsedStacks("main;std::vector<int>::push_back 42\n");
        expect(renderer.totalSamples() == 42_u);
    };

    "large sample counts"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks("main;hot_function 1000000\n");
        expect(renderer.totalSamples() == 1000000_u);
    };

    "mixed valid and invalid lines"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;foo 100\n"
            "# comment\n"
            "\n"
            "invalid_line_no_count\n"
            "main;bar 50\n"
        );
        expect(renderer.totalSamples() == 150_u);
    };
};

//=============================================================================
// End-to-end: parse → render → serialize → base64 → OSC
//=============================================================================

suite end_to_end_tests = [] {

    "full pipeline produces valid OSC output"_test = [] {
        // 1. Parse collapsed stacks
        FlameConfig cfg;
        FlameRenderer renderer(cfg);
        renderer.loadCollapsedStacks(
            "main;render;drawFrame 200\n"
            "main;render;uploadTexture 80\n"
            "main;input;pollEvents 30\n"
            "main;gc;sweep 15\n"
        );
        expect(renderer.totalSamples() == 325_u);

        // 2. Render into buffer
        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);
        expect(buffer->primCount() > 0_u) << "should have primitives";
        expect(buffer->textSpanCount() > 0_u) << "should have text";

        // 3. Serialize
        auto binary = buffer->serialize();
        expect(binary.size() > 0_u) << "serialized data should be non-empty";

        // Check YDRF header
        uint32_t magic;
        std::memcpy(&magic, binary.data(), 4);
        expect(magic == 0x59445246u) << "should have YDRF magic";

        // 4. Base64 encode
        auto b64 = ycat::base64Encode(binary);
        expect(!b64.empty()) << "base64 should be non-empty";

        // 5. Build OSC
        char header[512];
        std::snprintf(header, sizeof(header),
            "\033]666666;run -c ydraw -x 0 -y 0 -w 80 -h 24 -r --name yflame-app;;");
        std::string osc;
        osc += header;
        osc += b64;
        osc += "\033\\";

        expect(osc.size() > 100_u)
            << "full OSC sequence should be substantial";
        expect(osc.substr(0, 2) == "\033]")
            << "should start with ESC ]";
        expect(osc.substr(osc.size() - 2) == "\033\\")
            << "should end with ST";
    };

    "pipeline handles realistic perf-like data"_test = [] {
        FlameConfig cfg;
        FlameRenderer renderer(cfg);

        // Simulate realistic collapsed stacks from a C++ application
        renderer.loadCollapsedStacks(
            "__libc_start_main;main;app::run;app::processFrame;render::draw 150\n"
            "__libc_start_main;main;app::run;app::processFrame;render::draw;gpu::submit 80\n"
            "__libc_start_main;main;app::run;app::processFrame;render::draw;gpu::wait 45\n"
            "__libc_start_main;main;app::run;app::processFrame;input::poll 30\n"
            "__libc_start_main;main;app::run;app::processFrame;input::poll;epoll_wait 25\n"
            "__libc_start_main;main;app::run;app::processFrame;physics::step 60\n"
            "__libc_start_main;main;app::run;app::processFrame;physics::step;collide 40\n"
            "__libc_start_main;main;app::run;app::gc 20\n"
            "__libc_start_main;main;app::run;app::gc;mark 12\n"
            "__libc_start_main;main;app::run;app::gc;sweep 8\n"
        );
        expect(renderer.totalSamples() > 0_u);

        ViewState view;
        auto buffer = *YDrawBuffer::create();
        renderer.render(buffer, view);

        expect(buffer->primCount() > 0_u) << "should render prims";

        auto data = buffer->serialize();
        expect(data.size() > 0_u) << "should serialize";

        // Verify roundtrip
        auto buf2 = *YDrawBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value()) << "deserialize should succeed";
        expect(buf2->primCount() == buffer->primCount())
            << "roundtrip should preserve prims";
    };
};
