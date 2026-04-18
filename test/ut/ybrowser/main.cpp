//=============================================================================
// ybrowser Unit Tests
//
// Tests the full browser pipeline: fetch → render → verify ydraw buffer →
// click → fetch next page → verify new buffer content.
//
// Every test verifies the YDrawBuffer output (primitives, text spans) to
// ensure the rendering pipeline actually produces correct draw commands.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

#include <boost/ut.hpp>

#include "yetty/ydraw/ydraw-buffer.h"
#include "html-renderer.h"
#include "html-container.h"
#include "http-fetcher.h"

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Test server management
//=============================================================================
static pid_t g_serverPid = -1;
static int   g_serverPort = 0;

static std::string serverUrl(const std::string& path = "/") {
    return "http://127.0.0.1:" + std::to_string(g_serverPort) + path;
}

static bool startServer() {
    const char* paths[] = {
        CMAKE_SOURCE_DIR "/test/ut/ybrowser/server.py",
    };
    std::string serverScript;
    for (auto p : paths) {
        if (access(p, R_OK) == 0) { serverScript = p; break; }
    }
    if (serverScript.empty()) {
        std::fprintf(stderr, "ERROR: Cannot find server.py\n");
        return false;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) return false;

    g_serverPid = fork();
    if (g_serverPid < 0) { close(pipefd[0]); close(pipefd[1]); return false; }

    if (g_serverPid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("python3", "python3", serverScript.c_str(), nullptr);
        _exit(1);
    }

    close(pipefd[1]);
    char buf[128] = {};
    ssize_t n = read(pipefd[0], buf, sizeof(buf) - 1);
    close(pipefd[0]);

    if (n <= 0) {
        kill(g_serverPid, SIGTERM);
        waitpid(g_serverPid, nullptr, 0);
        g_serverPid = -1;
        return false;
    }

    char* colon = std::strchr(buf, ':');
    if (!colon) {
        kill(g_serverPid, SIGTERM);
        waitpid(g_serverPid, nullptr, 0);
        g_serverPid = -1;
        return false;
    }
    g_serverPort = std::atoi(colon + 1);
    usleep(50000);
    return g_serverPort > 0;
}

static void stopServer() {
    if (g_serverPid > 0) {
        kill(g_serverPid, SIGTERM);
        waitpid(g_serverPid, nullptr, 0);
        g_serverPid = -1;
    }
}

//=============================================================================
// Buffer inspection helpers
//=============================================================================

// Collapse runs of whitespace into single spaces, trim leading/trailing
static std::string normalizeWS(const std::string& s) {
    std::string r;
    bool sp = true;  // skip leading spaces
    for (char c : s) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!sp) { r += ' '; sp = true; }
        } else {
            r += c; sp = false;
        }
    }
    if (!r.empty() && r.back() == ' ') r.pop_back();
    return r;
}

// Concatenate all text spans into one whitespace-normalized string
static std::string allBufferText(const std::shared_ptr<YDrawBuffer>& buf) {
    std::string all;
    buf->forEachTextSpan([&](const TextSpanData& span) {
        if (!all.empty()) all += " ";
        all += span.text;
    });
    return normalizeWS(all);
}

// Check if the combined buffer text contains the needle (whitespace-normalized)
static bool bufferHasText(const std::shared_ptr<YDrawBuffer>& buf,
                          const std::string& needle) {
    return allBufferText(buf).find(normalizeWS(needle)) != std::string::npos;
}

// Count primitives by type (0=Circle, 1=Box, 2=Segment, etc.)
static uint32_t countPrimType(const std::shared_ptr<YDrawBuffer>& buf,
                              uint32_t targetType) {
    uint32_t count = 0;
    buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
        uint32_t type;
        std::memcpy(&type, &data[0], sizeof(uint32_t));
        if (type == targetType) count++;
    });
    return count;
}

//=============================================================================
// Click + render helpers
//=============================================================================

struct ClickTarget {
    int x = -1, y = -1, w = 0, h = 0;
    bool found = false;
};

static ClickTarget findElement(
    const std::shared_ptr<litehtml::document>& doc,
    const std::string& selector) {
    ClickTarget target;
    auto root = doc->root();
    if (!root) return target;
    auto el = root->select_one(selector);
    if (!el) return target;
    auto pos = el->get_placement();
    target.w = pos.width;
    target.h = pos.height;
    target.x = pos.x + (pos.width > 0 ? pos.width / 2 : 1);
    target.y = pos.y + (pos.height > 0 ? pos.height / 2 : 1);
    target.found = true;
    return target;
}

static void simulateClick(
    const std::shared_ptr<litehtml::document>& doc,
    int docX, int docY) {
    litehtml::position::vector redraw;
    doc->on_mouse_over(docX, docY, docX, docY, redraw);
    doc->on_lbutton_down(docX, docY, docX, docY, redraw);
    doc->on_lbutton_up(docX, docY, docX, docY, redraw);
}

static HtmlRenderResult renderInline(
    const char* html,
    std::shared_ptr<YDrawBuffer>& buffer,
    float fontSize = 16.0f, float viewWidth = 800.0f,
    HttpFetcher* fetcher = nullptr) {
    auto bufferRes = YDrawBuffer::create();
    buffer = *bufferRes;
    return renderHtmlToBuffer(html, buffer, fontSize, viewWidth, fetcher);
}

static HtmlRenderResult fetchAndRender(
    HttpFetcher* fetcher, const std::string& url,
    std::shared_ptr<YDrawBuffer>& buffer,
    float fontSize = 16.0f, float viewWidth = 800.0f) {
    auto body = fetcher->fetch(url);
    if (!body || body->empty()) return {};
    auto bufferRes = YDrawBuffer::create();
    buffer = *bufferRes;
    fetcher->setBaseUrl(url);
    return renderHtmlToBuffer(*body, buffer, fontSize, viewWidth, fetcher);
}

//=============================================================================
// Tests
//=============================================================================
int main() {

    //=========================================================================
    // SECTION 1: Render → verify ydraw buffer content
    //=========================================================================

    "render_produces_text_spans_in_buffer"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <h1>Hello World</h1>
              <p>Some paragraph text.</p>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(result.documentHeight > 0_i);

        // Buffer must have text spans with actual rendered text
        expect(buffer->textSpanCount() > 0_u) << "Buffer should have text spans";
        expect(bufferHasText(buffer, "Hello World"))
            << "Buffer should contain 'Hello World', got: " << allBufferText(buffer);
        expect(bufferHasText(buffer, "Some paragraph text"))
            << "Buffer should contain paragraph text";
    };

    "render_produces_primitives_for_styled_elements"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:10px; font-size:16px;">
              <div style="background:#cccccc; border:1px solid black; padding:10px;">
                Box with background and border
              </div>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);

        // Should have Box primitive for the background
        uint32_t boxes = countPrimType(buffer, 1);
        expect(boxes >= 1_u) << "Should have box for background, got " << boxes;

        // Should have Segment primitives for borders (4 sides)
        uint32_t segments = countPrimType(buffer, 2);
        expect(segments >= 4_u) << "Should have segments for borders, got " << segments;

        // Should have text span for content
        expect(bufferHasText(buffer, "Box with background"))
            << "Should render text inside styled div";
    };

    "render_link_text_appears_in_buffer"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:24px;">
              <p><a href="/target" style="display:inline-block; padding:20px; font-size:32px;">
                CLICK ME
              </a></p>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "CLICK ME"))
            << "Link text should be in buffer, got: " << allBufferText(buffer);
    };

    "render_form_elements_appear_in_buffer"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <h1>Contact Form</h1>
              <form action="/submit" method="POST">
                <input type="hidden" name="token" value="abc123">
                <button type="submit"
                        style="display:inline-block; padding:20px; font-size:24px;">
                  Send Message
                </button>
              </form>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Contact Form"))
            << "Form heading should be in buffer";
        expect(bufferHasText(buffer, "Send Message"))
            << "Button text should be in buffer, got: " << allBufferText(buffer);
    };

    "serialized_buffer_is_non_empty"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <p>Content to serialize</p>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);

        auto serialized = buffer->serialize();
        expect(serialized.size() > 100_u)
            << "Serialized buffer should be substantial, got " << serialized.size() << " bytes";
    };

    //=========================================================================
    // SECTION 2: Click → callback fires + verify ydraw buffer
    //=========================================================================

    "link_click_triggers_navigation_and_buffer_has_link_text"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:24px;">
              <p><a href="https://example.com/target"
                    style="display:inline-block; padding:20px; font-size:32px;">
                CLICK ME
              </a></p>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "CLICK ME")) << "Link text in buffer before click";

        std::string navigatedUrl;
        result.container->setNavigateCallback([&](const std::string& url) {
            navigatedUrl = url;
        });

        auto target = findElement(result.document, "a");
        expect(target.found) << "Should find <a> element";
        simulateClick(result.document, target.x, target.y);

        expect(navigatedUrl == "https://example.com/target")
            << "Navigate callback should fire, got: " << navigatedUrl;
    };

    "form_submit_callback_fires_with_correct_data"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <form action="/submit" method="POST">
                <input type="hidden" name="token" value="abc123">
                <input type="hidden" name="action" value="save">
                <input type="submit" value="Submit" name="btn"
                       style="display:inline-block; padding:20px; font-size:32px;">
              </form>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);

        std::string submitAction, submitMethod, submitData;
        result.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string& m, const std::string& d) {
                submitAction = a; submitMethod = m; submitData = d;
            });

        auto target = findElement(result.document, "input[type=submit]");
        expect(target.found) << "Should find submit input";
        simulateClick(result.document, target.x, target.y);

        expect(submitAction == "/submit") << "action: " << submitAction;
        expect(submitMethod == "POST") << "method: " << submitMethod;
        expect(submitData.find("token=abc123") != std::string::npos) << "data: " << submitData;
        expect(submitData.find("action=save") != std::string::npos) << "data: " << submitData;
    };

    "button_submit_collects_hidden_fields"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <form action="/save" method="POST">
                <input type="hidden" name="f1" value="v1">
                <input type="hidden" name="f2" value="v2">
                <input type="hidden" name="f3" value="v3">
                <button type="submit"
                        style="display:inline-block; padding:20px; font-size:32px;">
                  Go
                </button>
              </form>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Go")) << "Button text should be in buffer";

        std::string data;
        result.container->setFormSubmitCallback(
            [&](const std::string&, const std::string&, const std::string& d) { data = d; });

        auto target = findElement(result.document, "button");
        expect(target.found);
        simulateClick(result.document, target.x, target.y);

        expect(data.find("f1=v1") != std::string::npos) << "data: " << data;
        expect(data.find("f2=v2") != std::string::npos) << "data: " << data;
        expect(data.find("f3=v3") != std::string::npos) << "data: " << data;
    };

    "click_on_plain_text_does_not_navigate"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <p>Just plain text, no links here.</p>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "plain text")) << "Plain text in buffer";

        std::string navigatedUrl;
        result.container->setNavigateCallback([&](const std::string& url) {
            navigatedUrl = url;
        });
        simulateClick(result.document, 400, 30);
        expect(navigatedUrl.empty()) << "Should NOT navigate";
    };

    "form_submit_does_not_trigger_navigate"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:20px; font-size:16px;">
              <form action="/x" method="POST">
                <input type="hidden" name="k" value="v">
                <button type="submit"
                        style="display:inline-block; padding:20px; font-size:32px;">Go</button>
              </form>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);

        bool navFired = false, formFired = false;
        result.container->setNavigateCallback([&](const std::string&) { navFired = true; });
        result.container->setFormSubmitCallback(
            [&](const std::string&, const std::string&, const std::string&) { formFired = true; });

        auto target = findElement(result.document, "button");
        expect(target.found);
        simulateClick(result.document, target.x, target.y);
        expect(formFired) << "Form submit should fire";
        expect(!navFired) << "Navigate should NOT fire on form submit";
    };

    //=========================================================================
    // SECTION 3: HTTP integration — full pipeline with buffer verification
    //=========================================================================

    "server_starts"_test = [] {
        expect(startServer()) << "Python test server should start";
        std::printf("  Server running on port %d\n", g_serverPort);
    };

    "fetch_and_render_index_produces_correct_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        std::shared_ptr<YDrawBuffer> buffer;
        auto result = fetchAndRender(fetcher.get(), serverUrl("/"), buffer);
        expect(result.document != nullptr);
        expect(result.documentHeight > 0_i);

        // Buffer must have text spans with the page title
        expect(buffer->textSpanCount() > 0_u) << "Should have text spans";
        expect(bufferHasText(buffer, "Test Index Page"))
            << "Buffer should contain index title, got: " << allBufferText(buffer);

        // Buffer must have primitives (at least the rendered text)
        auto serialized = buffer->serialize();
        expect(serialized.size() > 100_u) << "Serialized buffer should be substantial";

        // Should find links in the document
        auto link = findElement(result.document, "a");
        expect(link.found) << "Index page should have links";
    };

    "click_link_navigates_and_renders_new_page_in_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // 1. Render index page
        std::shared_ptr<YDrawBuffer> buf1;
        auto r1 = fetchAndRender(fetcher.get(), serverUrl("/"), buf1);
        expect(r1.document != nullptr);
        expect(bufferHasText(buf1, "Test Index Page")) << "Index rendered";

        // 2. Click first link (Go to Page 1)
        std::string navigatedUrl;
        r1.container->setNavigateCallback([&](const std::string& u) { navigatedUrl = u; });
        auto link = findElement(r1.document, "a");
        expect(link.found);
        simulateClick(r1.document, link.x, link.y);
        expect(!navigatedUrl.empty()) << "Navigate callback should fire";

        // 3. Fetch and render the new page
        std::shared_ptr<YDrawBuffer> buf2;
        auto r2 = fetchAndRender(fetcher.get(), fetcher->resolveUrl(navigatedUrl), buf2);
        expect(r2.document != nullptr);
        expect(r2.documentHeight > 0_i);

        // 4. Verify NEW buffer has Page 1 content, NOT index content
        expect(bufferHasText(buf2, "Page One"))
            << "New buffer should have Page 1 text, got: " << allBufferText(buf2);
        expect(!bufferHasText(buf2, "Test Index Page"))
            << "New buffer should NOT have index text";
    };

    "form_post_renders_server_response_in_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // 1. Render form-post page
        std::shared_ptr<YDrawBuffer> buf1;
        auto r1 = fetchAndRender(fetcher.get(), serverUrl("/form-post"), buf1);
        expect(r1.document != nullptr);
        expect(bufferHasText(buf1, "POST Form Test"))
            << "Form page title in buffer, got: " << allBufferText(buf1);

        // 2. Click submit → get form data
        std::string action, method, data;
        r1.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string& m, const std::string& d) {
                action = a; method = m; data = d;
            });

        auto target = findElement(r1.document, "input[type=submit]");
        expect(target.found) << "Should find submit button";
        simulateClick(r1.document, target.x, target.y);

        expect(method == "POST") << "method: " << method;
        expect(data.find("token=abc123") != std::string::npos) << "data: " << data;
        expect(data.find("session=xyz789") != std::string::npos) << "data: " << data;

        // 3. POST to server and render the response
        auto response = fetcher->post(fetcher->resolveUrl(action), data);
        expect(response.has_value()) << "POST should succeed";

        std::shared_ptr<YDrawBuffer> buf2;
        auto bufferRes = YDrawBuffer::create();
        buf2 = *bufferRes;
        fetcher->setBaseUrl(fetcher->resolveUrl(action));
        auto r2 = renderHtmlToBuffer(*response, buf2, 16.0f, 800.0f, fetcher.get());
        expect(r2.document != nullptr);

        // 4. Verify response page is rendered in the buffer
        expect(bufferHasText(buf2, "POST Received"))
            << "Response buffer should show 'POST Received', got: " << allBufferText(buf2);
        expect(!bufferHasText(buf2, "POST Form Test"))
            << "Response buffer should NOT have original form title";
    };

    "form_get_renders_search_results_in_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // 1. Render GET form page
        std::shared_ptr<YDrawBuffer> buf1;
        auto r1 = fetchAndRender(fetcher.get(), serverUrl("/form-get"), buf1);
        expect(r1.document != nullptr);
        expect(bufferHasText(buf1, "GET Form Test"))
            << "Form page title in buffer, got: " << allBufferText(buf1);

        // 2. Click search button → get form data
        std::string action, method, data;
        r1.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string& m, const std::string& d) {
                action = a; method = m; data = d;
            });

        auto target = findElement(r1.document, "button");
        expect(target.found);
        simulateClick(r1.document, target.x, target.y);
        expect(method == "GET") << "method: " << method;
        expect(data.find("q=test_query") != std::string::npos) << "data: " << data;

        // 3. Build GET URL and fetch + render
        std::string getUrl = fetcher->resolveUrl(action) + "?" + data;
        std::shared_ptr<YDrawBuffer> buf2;
        auto r2 = fetchAndRender(fetcher.get(), getUrl, buf2);
        expect(r2.document != nullptr);

        // 4. Verify search results are in the buffer
        expect(bufferHasText(buf2, "Search Results"))
            << "Buffer should show search results, got: " << allBufferText(buf2);
        expect(bufferHasText(buf2, "test_query"))
            << "Buffer should show the query term";
    };

    "post_303_redirect_renders_target_in_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();
        fetcher->setBaseUrl(serverUrl("/"));

        // POST to redirect endpoint → should follow 303 → get target page
        auto response = fetcher->post(serverUrl("/redirect-submit"), "action=redirect_test");
        expect(response.has_value()) << "POST+redirect should succeed";

        // Render the response into a buffer
        std::shared_ptr<YDrawBuffer> buffer;
        auto bufferRes = YDrawBuffer::create();
        buffer = *bufferRes;
        auto result = renderHtmlToBuffer(*response, buffer, 16.0f, 800.0f, fetcher.get());
        expect(result.document != nullptr);

        // Verify the redirect target page is rendered
        expect(bufferHasText(buffer, "Redirect Target"))
            << "Buffer should show redirect target, got: " << allBufferText(buffer);
    };

    "multi_link_click_each_produces_different_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // Render multi-link page
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = fetchAndRender(fetcher.get(), serverUrl("/multi-link"), buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Link A")) << "Multi-link page rendered";
        expect(bufferHasText(buffer, "Link B"));
        expect(bufferHasText(buffer, "Link C"));

        std::vector<std::string> urls;
        result.container->setNavigateCallback([&](const std::string& u) { urls.push_back(u); });

        // Click link A
        auto a = findElement(result.document, "#link-a");
        expect(a.found);
        simulateClick(result.document, a.x, a.y);

        // Click link B
        auto b = findElement(result.document, "#link-b");
        expect(b.found);
        simulateClick(result.document, b.x, b.y);

        // Click link C
        auto c = findElement(result.document, "#link-c");
        expect(c.found);
        simulateClick(result.document, c.x, c.y);

        expect(urls.size() == 3_u) << "Three links clicked";
        expect(urls[0] == "/link-a") << "First: " << urls[0];
        expect(urls[1] == "/link-b") << "Second: " << urls[1];
        expect(urls[2] == "/link-c") << "Third: " << urls[2];

        // Fetch link-a and verify its buffer is different
        std::shared_ptr<YDrawBuffer> bufA;
        auto rA = fetchAndRender(fetcher.get(), fetcher->resolveUrl(urls[0]), bufA);
        expect(rA.document != nullptr);
        expect(bufferHasText(bufA, "/link-a"))
            << "Link A page should show path, got: " << allBufferText(bufA);
        expect(!bufferHasText(bufA, "Link B"))
            << "Link A page should NOT have multi-link content";
    };

    "multi_form_click_correct_form_and_render_response"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        std::shared_ptr<YDrawBuffer> buffer;
        auto result = fetchAndRender(fetcher.get(), serverUrl("/multi-form"), buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Submit Form 1"))
            << "Multi-form page rendered, got: " << allBufferText(buffer);

        // Click form 1 button
        std::string action, data;
        result.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string&, const std::string& d) {
                action = a; data = d;
            });

        auto btn1 = findElement(result.document, "#form1 button");
        expect(btn1.found) << "Should find button in form1";
        simulateClick(result.document, btn1.x, btn1.y);
        expect(action == "/action1") << "action: " << action;
        expect(data.find("form_id=1") != std::string::npos) << "data: " << data;
        expect(data.find("data=first") != std::string::npos) << "data: " << data;

        // POST form1 data and render response
        auto resp1 = fetcher->post(fetcher->resolveUrl(action), data);
        expect(resp1.has_value());
        std::shared_ptr<YDrawBuffer> buf1;
        auto b1 = YDrawBuffer::create();
        buf1 = *b1;
        auto r1 = renderHtmlToBuffer(*resp1, buf1, 16.0f, 800.0f, fetcher.get());
        expect(r1.document != nullptr);
        expect(bufferHasText(buf1, "POST Received"))
            << "Form1 response in buffer, got: " << allBufferText(buf1);
        expect(bufferHasText(buf1, "/action1"))
            << "Form1 response should show action1 path";

        // Click form 2 button
        action.clear(); data.clear();
        auto btn2 = findElement(result.document, "#form2 button");
        expect(btn2.found) << "Should find button in form2";
        simulateClick(result.document, btn2.x, btn2.y);
        expect(action == "/action2") << "action: " << action;
        expect(data.find("form_id=2") != std::string::npos) << "data: " << data;
    };

    "nested_dom_click_link_and_render_target"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        std::shared_ptr<YDrawBuffer> buffer;
        auto result = fetchAndRender(fetcher.get(), serverUrl("/nested"), buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Deep Nested Link"))
            << "Nested page rendered, got: " << allBufferText(buffer);

        // Click deeply nested link
        std::string nav;
        result.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto link = findElement(result.document, "a");
        expect(link.found);
        simulateClick(result.document, link.x, link.y);
        expect(nav == "/deep-link") << "nav: " << nav;

        // Fetch the target and render it
        std::shared_ptr<YDrawBuffer> buf2;
        auto r2 = fetchAndRender(fetcher.get(), fetcher->resolveUrl(nav), buf2);
        expect(r2.document != nullptr);
        expect(bufferHasText(buf2, "/deep-link"))
            << "Target page buffer should show path, got: " << allBufferText(buf2);

        // Also test nested form submit
        std::string action, data;
        result.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string&, const std::string& d) {
                action = a; data = d;
            });
        auto btn = findElement(result.document, "button");
        expect(btn.found);
        simulateClick(result.document, btn.x, btn.y);
        expect(action == "/nested-submit") << "action: " << action;
        expect(data.find("level=nested") != std::string::npos) << "data: " << data;
    };

    "mixed_content_click_link_then_form_renders_both"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        std::shared_ptr<YDrawBuffer> buffer;
        auto result = fetchAndRender(fetcher.get(), serverUrl("/mixed"), buffer);
        expect(result.document != nullptr);
        expect(bufferHasText(buffer, "Navigation Link"))
            << "Mixed page rendered, got: " << allBufferText(buffer);
        expect(bufferHasText(buffer, "Submit Button"));

        // Click the link
        std::string nav;
        std::string formAction;
        result.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        result.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string&, const std::string&) { formAction = a; });

        auto link = findElement(result.document, "a");
        expect(link.found);
        simulateClick(result.document, link.x, link.y);
        expect(nav == "/nav-link") << "nav: " << nav;
        expect(formAction.empty()) << "Form should NOT fire on link click";

        // Fetch link target and verify buffer
        std::shared_ptr<YDrawBuffer> bufNav;
        auto rNav = fetchAndRender(fetcher.get(), fetcher->resolveUrl(nav), bufNav);
        expect(rNav.document != nullptr);
        expect(bufferHasText(bufNav, "/nav-link"))
            << "Nav target in buffer, got: " << allBufferText(bufNav);

        // Click the form button
        nav.clear();
        auto btn = findElement(result.document, "button");
        expect(btn.found);
        simulateClick(result.document, btn.x, btn.y);
        expect(formAction == "/mixed-submit") << "formAction: " << formAction;

        // POST and verify response buffer
        auto resp = fetcher->post(fetcher->resolveUrl(formAction), "ctx=mixed");
        expect(resp.has_value());
        std::shared_ptr<YDrawBuffer> bufForm;
        auto bfr = YDrawBuffer::create();
        bufForm = *bfr;
        auto rForm = renderHtmlToBuffer(*resp, bufForm, 16.0f, 800.0f, fetcher.get());
        expect(rForm.document != nullptr);
        expect(bufferHasText(bufForm, "POST Received"))
            << "Form response in buffer, got: " << allBufferText(bufForm);
    };

    "full_navigation_roundtrip_index_to_page1_and_back"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // Step 1: Render index
        std::shared_ptr<YDrawBuffer> buf1;
        auto r1 = fetchAndRender(fetcher.get(), serverUrl("/"), buf1);
        expect(r1.document != nullptr);
        expect(bufferHasText(buf1, "Test Index Page")) << "Index in buffer";
        auto idx_texts = allBufferText(buf1);
        auto idx_primCount = buf1->primCount();
        auto idx_textCount = buf1->textSpanCount();

        // Step 2: Click "Go to Page 1"
        std::string nav;
        r1.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto link = findElement(r1.document, "a");
        expect(link.found);
        simulateClick(r1.document, link.x, link.y);
        expect(!nav.empty());

        // Step 3: Render page 1
        std::shared_ptr<YDrawBuffer> buf2;
        auto r2 = fetchAndRender(fetcher.get(), fetcher->resolveUrl(nav), buf2);
        expect(r2.document != nullptr);
        expect(bufferHasText(buf2, "Page One"))
            << "Page 1 in buffer, got: " << allBufferText(buf2);
        expect(!bufferHasText(buf2, "Test Index Page"))
            << "Page 1 buffer should NOT contain index text";

        // Step 4: Click "Back to Index" on page 1
        nav.clear();
        r2.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto backLink = findElement(r2.document, "a");
        expect(backLink.found);
        simulateClick(r2.document, backLink.x, backLink.y);
        expect(!nav.empty()) << "Should navigate back";

        // Step 5: Render index again
        std::shared_ptr<YDrawBuffer> buf3;
        auto r3 = fetchAndRender(fetcher.get(), fetcher->resolveUrl(nav), buf3);
        expect(r3.document != nullptr);
        expect(bufferHasText(buf3, "Test Index Page"))
            << "Index re-rendered in buffer, got: " << allBufferText(buf3);
        expect(!bufferHasText(buf3, "Page One"))
            << "Re-rendered index should NOT have page 1 text";
    };

    "tall_page_renders_all_content_with_positions"_test = [] {
        // Render a tall page and verify text spans cover the full document height
        auto bufferRes = YDrawBuffer::create();
        auto buffer = *bufferRes;

        std::string html =
            "<html><body style='margin:0; padding:10px; font-size:16px;'>";
        for (int i = 0; i < 50; i++) {
            html += "<p>Line " + std::to_string(i) + " of long page</p>";
        }
        html += "</body></html>";

        auto result = renderHtmlToBuffer(html, buffer, 16.0f, 600.0f, nullptr);
        expect(result.document != nullptr);
        expect(result.documentHeight > 500_i)
            << "Tall page should have large height: " << result.documentHeight;

        // All 50 lines should be rendered as text spans
        expect(bufferHasText(buffer, "Line 0")) << "First line rendered";
        expect(bufferHasText(buffer, "Line 49")) << "Last line rendered";
        expect(buffer->textSpanCount() >= 50_u)
            << "Should have at least 50 text spans, got " << buffer->textSpanCount();

        // Verify text spans are positioned across the document height
        float minY = 999999.0f, maxY = 0.0f;
        buffer->forEachTextSpan([&](const TextSpanData& span) {
            if (span.y < minY) minY = span.y;
            if (span.y > maxY) maxY = span.y;
        });
        expect(minY < 50.0f) << "Top text near y=0, got " << minY;
        expect(maxY > 400.0f) << "Bottom text at large y, got " << maxY;

        // Serialized buffer for a tall page should be substantial
        auto serialized = buffer->serialize();
        expect(serialized.size() > 500_u) << "Tall page serialization: " << serialized.size();
    };

    "viewport_coordinate_mapping"_test = [] {
        float cellW = 9.0f, cellH = 20.0f, scrollY = 200.0f;
        auto cellToDoc = [&](int col, int row, int& dx, int& dy) {
            dx = static_cast<int>((static_cast<float>(col) - 0.5f) * cellW);
            dy = static_cast<int>((static_cast<float>(row) - 0.5f) * cellH + scrollY);
        };

        int dx, dy;
        cellToDoc(1, 1, dx, dy);
        expect(dx == 4) << "got " << dx;
        expect(dy == 210) << "got " << dy;

        cellToDoc(50, 25, dx, dy);
        expect(dx == 445) << "got " << dx;
        expect(dy == 690) << "got " << dy;

        scrollY = 0.0f;
        cellToDoc(1, 1, dx, dy);
        expect(dy == 10) << "got " << dy;
    };

    "http_fetch_404_returns_nullopt"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();
        auto body = fetcher->fetch(serverUrl("/nonexistent-page"));
        expect(!body.has_value()) << "404 should return nullopt";
    };

    "url_resolution"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();
        fetcher->setBaseUrl(serverUrl("/some/path/page.html"));

        auto resolved = fetcher->resolveUrl("other.html");
        expect(resolved.find("/some/path/other.html") != std::string::npos)
            << "Relative: " << resolved;

        resolved = fetcher->resolveUrl("/absolute");
        expect(resolved.find("127.0.0.1") != std::string::npos)
            << "Absolute path: " << resolved;
        expect(resolved.find("/absolute") != std::string::npos);

        resolved = fetcher->resolveUrl("http://other.example.com/page");
        expect(resolved == "http://other.example.com/page");
    };

    //=========================================================================
    // SECTION 4: Exact ybrowser flow — SINGLE buffer, clear, re-render,
    // serialize (= OSC update), deserialize (= yetty card receives)
    //=========================================================================

    "ybrowser_flow_single_buffer_page_navigation"_test = [] {
        // This exactly replicates what ybrowser does:
        // ONE buffer used for the entire session. On navigation:
        //   renderPage: document.reset, container.reset, buffer.clear,
        //               renderHtmlToBuffer(html, buffer), then:
        //   redraw:     buffer.clear, document.draw(0,-scrollY,0,&clip)
        //   send:       buffer.serialize() → OSC update
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();

        // Create ONE buffer (like BrowserState.buffer)
        auto buffer = *YDrawBuffer::create();
        float viewWidth = 800.0f;

        // === Page 1: Render index ===
        auto body1 = fetcher->fetch(serverUrl("/"));
        expect(body1.has_value());
        fetcher->setBaseUrl(serverUrl("/"));

        // renderPage: clear → renderHtmlToBuffer
        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        auto r1 = renderHtmlToBuffer(*body1, buffer, 16.0f, viewWidth, fetcher.get());
        expect(r1.document != nullptr);

        // redraw: clear → draw with viewport clip
        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        int vpH = 600;
        litehtml::position clip1(0, 0, static_cast<int>(viewWidth), vpH);
        r1.document->draw(0, 0, 0, &clip1);
        buffer->setSceneBounds(0, 0, viewWidth, static_cast<float>(vpH));

        // sendToTerminal: serialize (= OSC create)
        auto wire1 = buffer->serialize();
        expect(wire1.size() > 100_u) << "Create wire should be substantial";

        // Verify yetty side: deserialize and check content
        auto card1 = *YDrawBuffer::create();
        expect(card1->deserialize(wire1.data(), wire1.size()).has_value());
        expect(bufferHasText(card1, "Test Index Page"))
            << "Card should show index, got: " << allBufferText(card1);
        expect(card1->textSpanCount() > 0_u) << "Card should have text spans";

        // === Click link → navigate ===
        std::string nav;
        r1.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto link = findElement(r1.document, "a");
        expect(link.found);
        simulateClick(r1.document, link.x, link.y);
        expect(!nav.empty()) << "Navigate callback should fire";

        // === Page 2: Render page 1 into SAME buffer ===
        std::string resolved = fetcher->resolveUrl(nav);
        fetcher->setBaseUrl(resolved);
        auto body2 = fetcher->fetch(resolved);
        expect(body2.has_value());

        // renderPage: destroy old doc/container, clear SAME buffer, re-render
        r1.document.reset();
        r1.container.reset();

        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        auto r2 = renderHtmlToBuffer(*body2, buffer, 16.0f, viewWidth, fetcher.get());
        expect(r2.document != nullptr);

        // redraw: clear SAME buffer again, draw with viewport clip
        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        litehtml::position clip2(0, 0, static_cast<int>(viewWidth), vpH);
        r2.document->draw(0, 0, 0, &clip2);
        buffer->setSceneBounds(0, 0, viewWidth, static_cast<float>(vpH));

        // sendToTerminal: serialize (= OSC update sent to SAME card)
        auto wire2 = buffer->serialize();
        expect(wire2.size() > 100_u) << "Update wire should be substantial";
        expect(wire1 != wire2) << "Update wire must differ from create wire";

        // Verify yetty side: card receives update, deserializes
        auto card2 = *YDrawBuffer::create();
        expect(card2->deserialize(wire2.data(), wire2.size()).has_value())
            << "Card should deserialize update";
        expect(bufferHasText(card2, "Page One"))
            << "Card should now show Page 1, got: " << allBufferText(card2);
        expect(!bufferHasText(card2, "Test Index Page"))
            << "Card should NOT have old index text";
        expect(card2->textSpanCount() > 0_u);
    };

    "ybrowser_flow_form_post_updates_same_buffer"_test = [] {
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();
        auto buffer = *YDrawBuffer::create();
        float viewWidth = 800.0f;
        int vpH = 600;

        // Render form-post page
        fetcher->setBaseUrl(serverUrl("/"));
        auto body1 = fetcher->fetch(serverUrl("/form-post"));
        expect(body1.has_value());

        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        auto r1 = renderHtmlToBuffer(*body1, buffer, 16.0f, viewWidth, fetcher.get());
        expect(r1.document != nullptr);

        // redraw + serialize (= create)
        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        litehtml::position clip(0, 0, static_cast<int>(viewWidth), vpH);
        r1.document->draw(0, 0, 0, &clip);
        buffer->setSceneBounds(0, 0, viewWidth, static_cast<float>(vpH));
        auto wireCreate = buffer->serialize();

        auto cardCreate = *YDrawBuffer::create();
        expect(cardCreate->deserialize(wireCreate.data(), wireCreate.size()).has_value());
        expect(bufferHasText(cardCreate, "POST Form Test"))
            << "Card shows form page, got: " << allBufferText(cardCreate);

        // Click submit → get form data
        std::string action, method, data;
        r1.container->setFormSubmitCallback(
            [&](const std::string& a, const std::string& m, const std::string& d) {
                action = a; method = m; data = d;
            });
        auto target = findElement(r1.document, "input[type=submit]");
        expect(target.found);
        simulateClick(r1.document, target.x, target.y);
        expect(method == "POST");
        expect(data.find("token=abc123") != std::string::npos);

        // POST form data → get response HTML
        auto response = fetcher->post(fetcher->resolveUrl(action), data);
        expect(response.has_value());

        // Render response into SAME buffer (ybrowser renderPage flow)
        r1.document.reset();
        r1.container.reset();

        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        auto r2 = renderHtmlToBuffer(*response, buffer, 16.0f, viewWidth, fetcher.get());
        expect(r2.document != nullptr);

        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        r2.document->draw(0, 0, 0, &clip);
        buffer->setSceneBounds(0, 0, viewWidth, static_cast<float>(vpH));
        auto wireUpdate = buffer->serialize();

        expect(wireCreate != wireUpdate) << "POST response wire must differ";

        // Verify card receives update with POST response
        auto cardUpdate = *YDrawBuffer::create();
        expect(cardUpdate->deserialize(wireUpdate.data(), wireUpdate.size()).has_value());
        expect(bufferHasText(cardUpdate, "POST Received"))
            << "Card should show POST response, got: " << allBufferText(cardUpdate);
        expect(!bufferHasText(cardUpdate, "POST Form Test"))
            << "Card should NOT have old form text";
    };

    "ybrowser_flow_three_page_navigation_same_buffer"_test = [] {
        // Index → Page 1 → back to Index, all on same buffer
        if (!g_serverPort) return;
        auto fetcher = *HttpFetcher::create();
        auto buffer = *YDrawBuffer::create();
        float viewWidth = 800.0f;
        int vpH = 600;
        litehtml::position clip(0, 0, static_cast<int>(viewWidth), vpH);

        auto renderPage = [&](const std::string& url) -> HtmlRenderResult {
            fetcher->setBaseUrl(url);
            auto body = fetcher->fetch(url);
            if (!body) return {};

            buffer->clear();
            buffer->setBgColor(0xFFFFFFFF);
            auto result = renderHtmlToBuffer(*body, buffer, 16.0f, viewWidth, fetcher.get());
            if (!result.document) return {};

            // redraw
            buffer->clear();
            buffer->setBgColor(0xFFFFFFFF);
            result.document->draw(0, 0, 0, &clip);
            buffer->setSceneBounds(0, 0, viewWidth, static_cast<float>(vpH));
            return result;
        };

        auto serializeAndVerify = [&](const std::string& expectedText,
                                       const std::string& unexpectedText) {
            auto wire = buffer->serialize();
            expect(wire.size() > 100_u);
            auto card = *YDrawBuffer::create();
            expect(card->deserialize(wire.data(), wire.size()).has_value());
            expect(bufferHasText(card, expectedText))
                << "Card should show '" << expectedText << "', got: " << allBufferText(card);
            if (!unexpectedText.empty()) {
                expect(!bufferHasText(card, unexpectedText))
                    << "Card should NOT show '" << unexpectedText << "'";
            }
        };

        // Page 1: Index
        auto r1 = renderPage(serverUrl("/"));
        expect(r1.document != nullptr);
        serializeAndVerify("Test Index Page", "");

        // Click link → navigate
        std::string nav;
        r1.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto link1 = findElement(r1.document, "a");
        expect(link1.found);
        simulateClick(r1.document, link1.x, link1.y);
        expect(!nav.empty());

        // Page 2: Page One
        r1.document.reset();
        r1.container.reset();
        auto r2 = renderPage(fetcher->resolveUrl(nav));
        expect(r2.document != nullptr);
        serializeAndVerify("Page One", "Test Index Page");

        // Click "Back to Index"
        nav.clear();
        r2.container->setNavigateCallback([&](const std::string& u) { nav = u; });
        auto link2 = findElement(r2.document, "a");
        expect(link2.found);
        simulateClick(r2.document, link2.x, link2.y);
        expect(!nav.empty());

        // Page 3: Back to Index
        r2.document.reset();
        r2.container.reset();
        auto r3 = renderPage(fetcher->resolveUrl(nav));
        expect(r3.document != nullptr);
        serializeAndVerify("Test Index Page", "Page One");
    };

    "serialize_roundtrip_preserves_prims_and_text"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        auto result = renderInline(R"(
            <html><body style="margin:0; padding:10px; font-size:16px;">
              <h1>Round Trip</h1>
              <div style="background:#ccc; border:1px solid black; padding:5px;">
                Styled box
              </div>
            </body></html>
        )", buffer);
        expect(result.document != nullptr);

        uint32_t origPrims = buffer->primCount();
        uint32_t origTexts = buffer->textSpanCount();
        expect(origPrims > 0_u);
        expect(origTexts > 0_u);

        auto wire = buffer->serialize();
        auto dbuf = *YDrawBuffer::create();
        expect(dbuf->deserialize(wire.data(), wire.size()).has_value());

        expect(dbuf->primCount() == origPrims)
            << "Prim: " << dbuf->primCount() << " vs " << origPrims;
        expect(dbuf->textSpanCount() == origTexts)
            << "Text: " << dbuf->textSpanCount() << " vs " << origTexts;
        expect(bufferHasText(dbuf, "Round Trip"));
        expect(bufferHasText(dbuf, "Styled box"));
    };

    "scene_bounds_and_bgcolor_survive_serialization"_test = [] {
        std::shared_ptr<YDrawBuffer> buffer;
        renderInline("<html><body><p>test</p></body></html>", buffer);
        buffer->setBgColor(0xFF112233);
        buffer->setSceneBounds(0, 0, 800.0f, 600.0f);

        auto wire = buffer->serialize();
        auto dbuf = *YDrawBuffer::create();
        expect(dbuf->deserialize(wire.data(), wire.size()).has_value());

        expect(dbuf->bgColor() == uint32_t(0xFF112233));
        expect(dbuf->hasSceneBounds());
        expect(dbuf->sceneMaxX() > 799.0f);
        expect(dbuf->sceneMaxY() > 599.0f);
    };

    // Cleanup
    "server_stops"_test = [] {
        stopServer();
        expect(g_serverPid == -1_i);
    };
}
