// libweb-to-ydraw: Render HTML using LibWeb to YDraw buffer
// Outputs OSC 666666 sequences for yetty terminal display

#include "DisplayListPlayerYDraw.h"
#include "HeadlessPageClient.h"
#include "ydraw-buffer-standalone.h"

#include <AK/LexicalPath.h>
#include <LibCore/EventLoop.h>
#include <LibCore/File.h>
#include <LibCore/Timer.h>
#include <LibGfx/Font/FontDatabase.h>
#include <LibJS/Runtime/VM.h>
#include <LibWeb/Bindings/MainThreadVM.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/HTML/BrowsingContext.h>
#include <LibWeb/HTML/TraversableNavigable.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/Layout/Viewport.h>
#include <LibWeb/Page/Page.h>
#include <LibWeb/Painting/DisplayList.h>
#include <LibWeb/Painting/PaintableBox.h>
#include <LibWeb/Painting/ViewportPaintable.h>
#include <LibWeb/Platform/EventLoopPlugin.h>
#include <LibWeb/Platform/FontPlugin.h>
#include <LibURL/URL.h>

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

// CURL callback for fetching URLs
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

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool isURL(const std::string& input) {
    return input.find("://") != std::string::npos ||
           input.find("http") == 0 ||
           input.find("https") == 0;
}

} // anonymous namespace

void printUsage(const char* progname) {
    std::cerr << "Usage: " << progname << " <url-or-file> [options]\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  --width W      Set viewport width (default: 800)\n";
    std::cerr << "  --height H     Set viewport height (default: 600)\n";
    std::cerr << "  --cells-w W    Set terminal cell width (default: 80)\n";
    std::cerr << "  --cells-h H    Set terminal cell height (default: 30)\n";
    std::cerr << "  --help         Show this help\n";
    std::cerr << "\nRenders HTML from URL or local file using LibWeb.\n";
    std::cerr << "Outputs OSC 666666 sequence for yetty terminal.\n";
}

int main(int argc, char** argv) {
    std::string input;
    int viewWidth = 800;
    int viewHeight = 600;
    int cellsW = 80;
    int cellsH = 30;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--width" && i + 1 < argc) {
            viewWidth = std::stoi(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            viewHeight = std::stoi(argv[++i]);
        } else if (arg == "--cells-w" && i + 1 < argc) {
            cellsW = std::stoi(argv[++i]);
        } else if (arg == "--cells-h" && i + 1 < argc) {
            cellsH = std::stoi(argv[++i]);
        } else if (arg[0] != '-') {
            input = arg;
        }
    }

    if (input.empty()) {
        std::cerr << "Error: URL or file required\n";
        printUsage(argv[0]);
        return 1;
    }

    // Fetch or read HTML content
    std::string html;
    URL::URL url;

    if (isURL(input)) {
        std::cerr << "Fetching: " << input << "\n";
        html = fetchURL(input);
        url = URL::URL(input);
    } else {
        std::cerr << "Reading: " << input << "\n";
        html = readFile(input);
        url = URL::URL::create_with_file_scheme(input);
    }

    if (html.empty()) {
        std::cerr << "Error: Failed to load content\n";
        return 1;
    }

    std::cerr << "Loaded " << html.size() << " bytes\n";

    // Initialize LibWeb
    Core::EventLoop event_loop;

    // Initialize the font database
    Gfx::FontDatabase::set_default_font_query("Katica 10 400 0"sv);
    Gfx::FontDatabase::set_fixed_width_font_query("Csilla 10 400 0"sv);

    // Install platform plugins
    Web::Platform::EventLoopPlugin::install(*new Web::Platform::EventLoopPlugin);
    Web::Platform::FontPlugin::install(*new Web::Platform::FontPlugin(false, nullptr));

    // Create VM and heap
    auto& vm = Web::Bindings::main_thread_vm();
    auto& heap = vm.heap();

    // Create headless page client
    auto client = ydraw::HeadlessPageClient::create(vm, viewWidth, viewHeight);

    // Create page
    auto page = Web::Page::create(vm, *client);
    client->set_page(page);

    // Load HTML
    page->load_html(html);

    // Run event loop briefly to allow parsing and layout
    int max_iterations = 100;
    for (int i = 0; i < max_iterations && !client->is_layout_done(); ++i) {
        event_loop.pump(Core::EventLoop::WaitMode::PollForEvents);
    }

    // Get the document
    auto& browsing_context = page->top_level_browsing_context();
    auto* document = browsing_context.active_document();

    if (!document) {
        std::cerr << "Error: No document loaded\n";
        return 1;
    }

    std::cerr << "Document loaded: " << document->title().to_byte_string().characters() << "\n";

    // Force layout
    document->update_layout();

    // Get viewport paintable
    auto* viewport = document->layout_node();
    if (!viewport) {
        std::cerr << "Error: No layout tree\n";
        return 1;
    }

    auto* viewport_paintable = document->paintable();
    if (!viewport_paintable) {
        std::cerr << "Error: No paintable tree\n";
        return 1;
    }

    // Create display list
    auto display_list = Web::Painting::DisplayList::create();

    // Build the display list from paintable tree
    Web::Painting::DisplayListRecorder recorder(*display_list);
    viewport_paintable->paint_all_phases(recorder);

    std::cerr << "DisplayList has " << display_list->commands().size() << " commands\n";

    // Create YDraw buffer
    ydraw::Buffer buffer;
    buffer.setSize(static_cast<float>(viewWidth), static_cast<float>(viewHeight));

    // White background
    buffer.addBox(0, 0, static_cast<float>(viewWidth), static_cast<float>(viewHeight),
                  ydraw::packColor(255, 255, 255));

    // Execute display list to YDraw buffer
    ydraw::DisplayListPlayerYDraw player(buffer);
    player.execute_to_ydraw(*display_list);

    // Output as OSC sequence
    ydraw::outputOSC(buffer, 1, cellsW, cellsH);

    std::cerr << "Rendered " << buffer.primitiveCount() << " primitives, "
              << buffer.textCount() << " text spans\n";

    return 0;
}
