#include <yetty/yetty.h>
#include "input-handler.h"
#include "text-renderer.h"
#include "grid.h"

#if !YETTY_WEB
#include "terminal.h"
#include "plugin-manager.h"
#include "plugins/shader-glyph/shader-glyph.h"
#include <args.hxx>
#endif

#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include <filesystem>

namespace yetty {

// Helper macro to check Result in callbacks where we can't return errors
#define CHECK_RESULT(expr) \
    do { \
        if (auto _res = (expr); !_res) { \
            spdlog::error("{}: {}", #expr, _res.error().message()); \
        } \
    } while(0)

// Static instance for Emscripten
Yetty* Yetty::s_instance = nullptr;

// Colors for random text (RGB uint8)
struct RGB { uint8_t _r, _g, _b; };
static RGB g_colors[] = {
    {255, 255, 255},  // white
    {0, 255, 0},      // green
    {0, 255, 255},    // cyan
    {255, 255, 0}     // yellow
};

// Generate random line from dictionary
static std::string generateLine(const std::vector<std::string>& dict, uint32_t maxCols) {
    std::string line;
    while (line.length() < maxCols - 10) {
        const std::string& word = dict[std::rand() % dict.size()];
        if (!line.empty()) line += " ";
        line += word;
    }
    return line;
}

// Default paths
#if YETTY_WEB
static const char* DEFAULT_FONT = "/assets/DejaVuSansMono.ttf";
static const char* DEFAULT_ATLAS = "/assets/atlas.png";
static const char* DEFAULT_METRICS = "/assets/atlas.json";
#elif defined(_WIN32)
static const char* DEFAULT_FONT = "C:/Windows/Fonts/consola.ttf";
static const char* DEFAULT_ATLAS = "assets/atlas.png";
static const char* DEFAULT_METRICS = "assets/atlas.json";
#elif defined(__APPLE__)
static const char* DEFAULT_FONT = "/System/Library/Fonts/Monaco.ttf";
static const char* DEFAULT_ATLAS = "assets/atlas.png";
static const char* DEFAULT_METRICS = "assets/atlas.json";
#else
static const char* DEFAULT_FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
static const char* DEFAULT_ATLAS = "assets/atlas.png";
static const char* DEFAULT_METRICS = "assets/atlas.json";
#endif

//-----------------------------------------------------------------------------
// Yetty Implementation
//-----------------------------------------------------------------------------

Yetty::~Yetty() {
    shutdown();
}

Result<Yetty::Ptr> Yetty::create(int argc, char* argv[]) noexcept {
    auto p = Ptr(new Yetty());
    if (auto res = p->init(argc, argv); !res) {
        return Err<Ptr>("Failed to init Yetty", res);
    }
    return Ok(p);
}

Result<void> Yetty::init(int argc, char* argv[]) noexcept {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("yetty starting...");

    // Parse command line arguments
    if (auto res = parseArgs(argc, argv); !res) {
        return res;
    }

    // Initialize window
    if (auto res = initWindow(); !res) {
        return res;
    }

    // Initialize graphics (WebGPU)
    if (auto res = initGraphics(); !res) {
        return res;
    }

    // Initialize font manager
    if (auto fmRes = FontManager::create(_ctx); !fmRes) {
        return Err<void>("Failed to create FontManager", fmRes);
    } else {
        _fontManager = *fmRes;
    }

    // Initialize font
    if (auto res = initFont(); !res) {
        return res;
    }

    // Initialize renderer
    if (auto res = initRenderer(); !res) {
        return res;
    }

    // Initialize terminal or demo mode
    if (auto res = initTerminalOrDemo(); !res) {
        return res;
    }

    // Set up callbacks
    if (auto res = initCallbacks(); !res) {
        return res;
    }

    s_instance = this;
    _lastFpsTime = glfwGetTime();

    return Ok();
}

Result<void> Yetty::parseArgs(int argc, char* argv[]) noexcept {
#if !YETTY_WEB
    args::ArgumentParser parser("yetty - WebGPU Terminal Emulator");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    // Config and plugin options
    args::ValueFlag<std::string> configFile(parser, "path", "Config file path", {'c', "config"});
    args::ValueFlag<std::string> pluginPath(parser, "path", "Plugin search path (colon-separated)", {'p', "plugin-path"});

    // Display options
    args::ValueFlag<std::string> fontPathArg(parser, "font", "Path to TTF font", {'f', "font"});
    args::ValueFlag<uint32_t> widthArg(parser, "width", "Window width in pixels", {'W', "width"});
    args::ValueFlag<uint32_t> heightArg(parser, "height", "Window height in pixels", {'H', "height"});

    // Mode options
    args::Flag generateAtlasFlag(parser, "generate-atlas", "Generate font atlas and exit", {"generate-atlas"});
    args::Flag loadAtlasFlag(parser, "load-atlas", "Use pre-built atlas", {"load-atlas"});
    args::ValueFlag<int> demoFlag(parser, "scroll_ms", "Run scrolling text demo", {"demo"});
    args::Flag noDamageFlag(parser, "no-damage", "Disable damage tracking", {"no-damage"});
    args::Flag debugDamageFlag(parser, "debug-damage", "Log damage rectangle updates", {"debug-damage"});
    args::ValueFlag<std::string> executeArg(parser, "command", "Execute command instead of shell", {'e'});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return Err<void>("Help requested");
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return Err<void>(std::string("Parse error: ") + e.what());
    }

    // Build command line overrides for config
    YAML::Node cmdOverrides;
    if (pluginPath) {
        cmdOverrides["plugins"]["path"] = args::get(pluginPath);
    }
    if (noDamageFlag) {
        cmdOverrides["rendering"]["damage-tracking"] = false;
    }
    if (debugDamageFlag) {
        cmdOverrides["debug"]["damage-rects"] = true;
    }

    // Create and initialize config
    std::string configPath = configFile ? args::get(configFile) : "";
    auto configResult = Config::create(configPath, cmdOverrides);
    if (!configResult) {
        return Err<void>("Failed to create config", configResult);
    }
    _config = *configResult;

    // Extract final values
    _generateAtlasOnly = generateAtlasFlag;
    _usePrebuiltAtlas = YETTY_USE_PREBUILT_ATLAS || loadAtlasFlag;
    _demoMode = demoFlag;
    _scrollMs = demoFlag ? args::get(demoFlag) : 50;
    _fontPath = fontPathArg ? args::get(fontPathArg) : std::string(DEFAULT_FONT);
    _executeCommand = executeArg ? args::get(executeArg) : "";
    _initialWidth = widthArg ? args::get(widthArg) : 1024;
    _initialHeight = heightArg ? args::get(heightArg) : 768;

    if (_initialWidth == 0) _initialWidth = 1024;
    if (_initialHeight == 0) _initialHeight = 768;

    if (!_executeCommand.empty()) {
        spdlog::info("Execute command: {}", _executeCommand);
    }
#else
    (void)argc; (void)argv;

    // Web build: simplified config without command line parsing
    auto configResult = Config::create();
    if (!configResult) {
        return Err<void>("Failed to create config");
    }
    _config = *configResult;

    _generateAtlasOnly = false;
    _usePrebuiltAtlas = true;
    _demoMode = true;
    _scrollMs = 50;
    _fontPath = DEFAULT_FONT;
    _initialWidth = 1024;
    _initialHeight = 768;
#endif

    return Ok();
}

Result<void> Yetty::initWindow() noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    // Generate atlas only mode (no window needed)
    if (_generateAtlasOnly) {
        std::cout << "Generating font atlas from: " << _fontPath << std::endl;

        Font font;
        float fontSize = 32.0f;
        if (!font.generate(_fontPath, fontSize)) {
            return Err<void>("Failed to generate font atlas");
        }

        // Save to assets directory
        std::string atlasDir = std::string(CMAKE_SOURCE_DIR) + "/assets";
        std::string atlasPath = atlasDir + "/atlas.png";
        std::string metricsPath = atlasDir + "/atlas.json";

        if (!font.saveAtlas(atlasPath, metricsPath)) {
            return Err<void>("Failed to save atlas");
        }

        std::cout << "Atlas saved to:" << std::endl;
        std::cout << "  " << atlasPath << std::endl;
        std::cout << "  " << metricsPath << std::endl;

        return Err<void>("Atlas generation complete");  // Exit signal
    }
#endif

    // Initialize GLFW
    if (!glfwInit()) {
        return Err<void>("Failed to initialize GLFW");
    }

    // Don't create OpenGL context - we're using WebGPU
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(_initialWidth, _initialHeight, "yetty - WebGPU Terminal", nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        return Err<void>("Failed to create window");
    }

    return Ok();
}

Result<void> Yetty::initGraphics() noexcept {
    auto ctxResult = WebGPUContext::create(_window, _initialWidth, _initialHeight);
    if (!ctxResult) {
        glfwDestroyWindow(_window);
        glfwTerminate();
        return Err<void>("Failed to initialize WebGPU", ctxResult);
    }
    _ctx = *ctxResult;

    return Ok();
}

Result<void> Yetty::initFont() noexcept {
    _font_storage = std::make_unique<Font>();
    _font = _font_storage.get();
    float fontSize = 32.0f;

#if YETTY_USE_PREBUILT_ATLAS
    // Web build: always use pre-built atlas
    std::cout << "Loading pre-built atlas..." << std::endl;
    if (!_font->loadAtlas(DEFAULT_ATLAS, DEFAULT_METRICS)) {
        return Err<void>("Failed to load pre-built atlas");
    }
    fontSize = _font->getFontSize();
#else
    // Native build: try cache first, then generate
    std::string cacheDir;
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        cacheDir = std::string(homeDir) + "/.cache/yetty";
    } else {
        cacheDir = "/tmp/yetty-cache";
    }
    std::string cachedAtlas = cacheDir + "/atlas.png";
    std::string cachedMetrics = cacheDir + "/atlas.json";

    bool loadedFromCache = false;

    if (_usePrebuiltAtlas) {
        std::cout << "Loading pre-built atlas..." << std::endl;
        if (_font->loadAtlas(DEFAULT_ATLAS, DEFAULT_METRICS)) {
            fontSize = _font->getFontSize();
            loadedFromCache = true;
        } else {
            std::cerr << "Failed to load atlas, falling back to cache/generation" << std::endl;
        }
    }

    if (!loadedFromCache) {
        std::ifstream cacheTest(cachedAtlas);
        if (cacheTest.good()) {
            cacheTest.close();
            std::cout << "Loading cached atlas from: " << cachedAtlas << std::endl;
            if (_font->loadAtlas(cachedAtlas, cachedMetrics)) {
                fontSize = _font->getFontSize();
                loadedFromCache = true;
            } else {
                std::cout << "Cache invalid, regenerating..." << std::endl;
            }
        }
    }

    if (!loadedFromCache) {
        std::cout << "Generating font atlas from: " << _fontPath << std::endl;
        if (!_font->generate(_fontPath, fontSize)) {
            return Err<void>("Failed to generate font atlas from: " + _fontPath);
        }

        // Save to cache
        std::filesystem::create_directories(cacheDir);
        std::cout << "Caching atlas to: " << cachedAtlas << std::endl;
        if (!_font->saveAtlas(cachedAtlas, cachedMetrics)) {
            std::cerr << "Warning: Failed to cache atlas" << std::endl;
        }
    }
#endif

    if (!_font->createTexture(_ctx->getDevice(), _ctx->getQueue())) {
        return Err<void>("Failed to create font texture");
    }

    // Calculate cell size
    _baseCellWidth = fontSize * 0.6f;
    _baseCellHeight = fontSize * 1.2f;

    return Ok();
}

Result<void> Yetty::initRenderer() noexcept {
    auto rendererResult = TextRenderer::create(_ctx, _font);
    if (!rendererResult) {
        return Err<void>("Failed to create text renderer", rendererResult);
    }
    _renderer = *rendererResult;
    _renderer->setCellSize(_baseCellWidth, _baseCellHeight);
    _renderer->resize(_initialWidth, _initialHeight);
    _renderer->setConfig(_config.get());

    // Calculate grid size
    _cols = static_cast<uint32_t>(_initialWidth / _baseCellWidth);
    _rows = static_cast<uint32_t>(_initialHeight / _baseCellHeight);

    return Ok();
}

Result<void> Yetty::initTerminalOrDemo() noexcept {
    if (_demoMode) {
        // Demo mode: scrolling text
        _demoGrid = new Grid(_cols, _rows);
        _lastScrollTime = glfwGetTime();

        // Load dictionary
#if !YETTY_WEB
        std::ifstream dictFile("/usr/share/dict/words");
        if (dictFile.is_open()) {
            std::string word;
            while (std::getline(dictFile, word)) {
                if (!word.empty() && word[0] >= 'a' && word[0] <= 'z') {
                    _dictionary.push_back(word);
                }
            }
            std::cout << "Loaded " << _dictionary.size() << " words from dictionary" << std::endl;
        } else
#endif
        {
            _dictionary = {"hello", "world", "terminal", "webgpu", "render", "scroll", "test",
                         "browser", "wasm", "gpu", "shader", "pixel", "font", "text", "grid",
                         "cell", "color", "alpha", "buffer", "vertex", "fragment", "compute"};
            std::cout << "Using fallback dictionary with " << _dictionary.size() << " words" << std::endl;
        }
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Fill initial content
        for (uint32_t row = 0; row < _rows; ++row) {
            std::string line = generateLine(_dictionary, _cols);
            RGB color = g_colors[std::rand() % 4];
            _demoGrid->writeString(0, row, line.c_str(), color._r, color._g, color._b, _font);
        }

        std::cout << "Demo mode: Grid " << _cols << "x" << _rows << ", scroll: " << _scrollMs << "ms" << std::endl;
    }
#if !YETTY_WEB
    else {
        // Terminal mode
        auto terminalResult = Terminal::create(_cols, _rows, _font);
        if (!terminalResult) {
            return Err<void>("Failed to create terminal", terminalResult);
        }
        _terminal = *terminalResult;
        _terminal->setConfig(_config.get());

        // Create and configure PluginManager
        auto pluginMgrResult = PluginManager::create();
        if (!pluginMgrResult) {
            return Err<void>("Failed to create plugin manager", pluginMgrResult);
        }
        _pluginManager = *pluginMgrResult;

        // Pass font and engine to plugins
        _pluginManager->setFont(_font);
        _pluginManager->setEngine(shared_from_this());

        // Register custom glyph plugins
        if (auto shaderGlyphResult = ShaderGlyphPlugin::create()) {
            _pluginManager->registerCustomGlyphPlugin(*shaderGlyphResult);
        }

        // Load plugins from configured paths
        auto pluginPaths = _config->pluginPaths();
        for (const auto& path : pluginPaths) {
            spdlog::info("Loading plugins from: {}", path);
            _pluginManager->loadPluginsFromDirectory(path);
        }

        // Wire up plugin manager to terminal
        _terminal->setPluginManager(_pluginManager.get());
        _terminal->setCellSize(static_cast<uint32_t>(_baseCellWidth), static_cast<uint32_t>(_baseCellHeight));

        if (auto result = _terminal->start(_executeCommand); !result) {
            return Err<void>("Failed to start terminal", result);
        }

        std::cout << "Terminal mode: Grid " << _cols << "x" << _rows
                  << " (damage tracking: " << (_config->useDamageTracking() ? "on" : "off") << ")" << std::endl;

        // Create input handler
        auto inputResult = InputHandler::create(this);
        if (!inputResult) {
            return Err<void>("Failed to create input handler", inputResult);
        }
        _inputHandler = *inputResult;
    }
#endif

    return Ok();
}

Result<void> Yetty::initCallbacks() noexcept {
    glfwSetWindowUserPointer(_window, this);

#if !YETTY_WEB
    if (!_demoMode && _inputHandler) {
        // Set up keyboard and mouse callbacks via lambdas that call InputHandler
        glfwSetKeyCallback(_window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
            auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
            if (engine && engine->_inputHandler) {
                engine->_inputHandler->onKey(key, scancode, action, mods);
            }
        });

        glfwSetCharCallback(_window, [](GLFWwindow* w, unsigned int codepoint) {
            auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
            if (engine && engine->_inputHandler) {
                engine->_inputHandler->onChar(codepoint);
            }
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* w, double xpos, double ypos) {
            auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
            if (engine && engine->_inputHandler) {
                engine->_inputHandler->onMouseMove(xpos, ypos);
            }
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* w, int button, int action, int mods) {
            auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
            if (engine && engine->_inputHandler) {
                engine->_inputHandler->onMouseButton(button, action, mods);
            }
        });
    }
#endif

    // Window resize callback
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* w, int newWidth, int newHeight) {
        auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
        if (engine) {
            engine->handleResize(newWidth, newHeight);
        }
    });

    // Scroll callback
    glfwSetScrollCallback(_window, [](GLFWwindow* w, double xoffset, double yoffset) {
        auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(w));
#if !YETTY_WEB
        if (engine && engine->_inputHandler) {
            engine->_inputHandler->onScroll(xoffset, yoffset);
        }
#else
        (void)engine; (void)xoffset; (void)yoffset;
#endif
    });

    return Ok();
}

void Yetty::shutdown() noexcept {
    delete _demoGrid;
    _demoGrid = nullptr;

    // shared_ptrs are cleaned up automatically

    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    glfwTerminate();

    s_instance = nullptr;
}

int Yetty::run() noexcept {
    std::cout << "Starting render loop... (use mouse scroll to zoom, ESC to exit)" << std::endl;

#if YETTY_WEB
    emscripten_set_main_loop(emscriptenMainLoop, 0, false);
    return 0;
#else
    while (!glfwWindowShouldClose(_window)) {
        mainLoopIteration();
    }

    std::cout << "Shutting down..." << std::endl;
    return 0;
#endif
}

#if YETTY_WEB
void Yetty::emscriptenMainLoop() noexcept {
    if (s_instance) {
        s_instance->mainLoopIteration();
    }
}
#endif

void Yetty::mainLoopIteration() noexcept {
    glfwPollEvents();

#if !YETTY_WEB
    // Terminal mode: update terminal and render its grid
    if (!_demoMode && _terminal) {
        CHECK_RESULT(_terminal->update());

        // Update decorators
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (_pluginManager) {
            if (auto res = _pluginManager->update(deltaTime); !res) {
                spdlog::error("Plugin update failed: {}", error_msg(res));
            }
            _pluginManager->updateCustomGlyphs(deltaTime);
        }

        if (!_terminal->isRunning()) {
            glfwSetWindowShouldClose(_window, GLFW_TRUE);
            return;
        }

        // Check if we need to render this frame
        auto cursorBlinkResult = _terminal->updateCursorBlink(currentTime);
        bool cursorChanged = cursorBlinkResult && *cursorBlinkResult;
        bool hasDamage = _terminal->hasDamage();
        bool hasPlugins = _pluginManager && !_pluginManager->getAllLayers().empty();

        // Skip rendering if nothing needs update
        if (!cursorChanged && !hasDamage && !hasPlugins) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            return;
        }

        // Upload any pending fallback glyphs to GPU
        if (_font && _font->hasPendingGlyphs()) {
            _font->uploadPendingGlyphs(_ctx->getDevice(), _ctx->getQueue());
            _renderer->updateFontBindings(*_font);
        }

        // Get current window size
        int w, h;
        glfwGetFramebufferSize(_window, &w, &h);
        if (w > 0 && h > 0) {
            _renderer->resize(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
        }

        // Render terminal grid with cursor and damage tracking
        if (_config && _config->useDamageTracking()) {
            _renderer->render(_terminal->getGrid(),
                             _terminal->getDamageRects(),
                             _terminal->hasFullDamage(),
                             _terminal->getCursorCol(),
                             _terminal->getCursorRow(),
                             _terminal->isCursorVisible());
            _terminal->clearDamageRects();
            _terminal->clearFullDamage();
        } else {
            _renderer->render(_terminal->getGrid(),
                             _terminal->getCursorCol(),
                             _terminal->getCursorRow(),
                             _terminal->isCursorVisible());
        }

        // Render custom glyph layers
        if (_pluginManager) {
            auto targetViewResult = _ctx->getCurrentTextureView();
            if (targetViewResult) {
                float cellW = cellWidth();
                float cellH = cellHeight();
                int scrollOffset = _terminal ? _terminal->getScrollOffset() : 0;
                if (auto res = _pluginManager->renderCustomGlyphs(
                        *_ctx, *targetViewResult,
                        static_cast<uint32_t>(w), static_cast<uint32_t>(h),
                        cellW, cellH, scrollOffset); !res) {
                    spdlog::error("Custom glyph render failed: {}", error_msg(res));
                }
            }
        }

        // Render plugin overlays
        if (hasPlugins) {
            auto targetViewResult = _ctx->getCurrentTextureView();
            if (targetViewResult) {
                float cellW = cellWidth();
                float cellH = cellHeight();
                int scrollOffset = _terminal ? _terminal->getScrollOffset() : 0;
                uint32_t termRows = _terminal ? _terminal->getGrid().getRows() : 0;
                if (auto res = _pluginManager->render(*_ctx, *targetViewResult,
                    static_cast<uint32_t>(w), static_cast<uint32_t>(h),
                    cellW, cellH, scrollOffset, termRows); !res) {
                    spdlog::error("Plugin render failed: {}", error_msg(res));
                }
            }
        }

        // Present the frame
        _ctx->present();
    } else
#endif
    {
        // Demo mode: scrolling text
        if (_demoMode && _demoGrid) {
            // Check for ESC key
            if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(_window, GLFW_TRUE);
#if YETTY_WEB
                emscripten_cancel_main_loop();
#endif
                return;
            }

            // Scrolling logic
            double currentTime = glfwGetTime();
            if (_scrollMs > 0 && !_dictionary.empty() &&
                (currentTime - _lastScrollTime) * 1000.0 >= _scrollMs) {
                _demoGrid->scrollUp();
                std::string newLine = generateLine(_dictionary, _cols);
                RGB color = g_colors[std::rand() % 4];
                _demoGrid->writeString(0, _rows - 1, newLine.c_str(),
                                      color._r, color._g, color._b, _font);
                _lastScrollTime = currentTime;
            }

            // Get current window size
            int w, h;
            glfwGetFramebufferSize(_window, &w, &h);
            if (w > 0 && h > 0) {
                _renderer->resize(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
            }

            // Render demo grid
            _renderer->render(*_demoGrid);
        }
    }

    // FPS counter
    double currentTime = glfwGetTime();
    _frameCount++;
    if (currentTime - _lastFpsTime >= 1.0) {
        std::cout << "FPS: " << _frameCount << std::endl;
        _frameCount = 0;
        _lastFpsTime = currentTime;
    }
}

void Yetty::handleResize(int newWidth, int newHeight) noexcept {
    if (newWidth == 0 || newHeight == 0) return;

    // Resize WebGPU context
    if (_ctx) {
        _ctx->resize(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight));
    }

    // Recalculate grid size based on current cell size
    float cellW = cellWidth();
    float cellH = cellHeight();
    uint32_t newCols = static_cast<uint32_t>(newWidth / cellW);
    uint32_t newRows = static_cast<uint32_t>(newHeight / cellH);

    // Ensure minimum grid size
    if (newCols < 1) newCols = 1;
    if (newRows < 1) newRows = 1;

    if (newCols != _cols || newRows != _rows) {
        updateGridSize(newCols, newRows);
        std::cout << "Window resize -> Grid " << newCols << "x" << newRows << std::endl;
    }
}

uint32_t Yetty::windowWidth() const noexcept {
    int w, h;
    glfwGetFramebufferSize(_window, &w, &h);
    return static_cast<uint32_t>(w);
}

uint32_t Yetty::windowHeight() const noexcept {
    int w, h;
    glfwGetFramebufferSize(_window, &w, &h);
    return static_cast<uint32_t>(h);
}

void Yetty::setZoomLevel(float zoom) noexcept {
    _zoomLevel = zoom;

    float newCellWidth = _baseCellWidth * _zoomLevel;
    float newCellHeight = _baseCellHeight * _zoomLevel;
    _renderer->setCellSize(newCellWidth, newCellHeight);
    _renderer->setScale(_zoomLevel);
}

void Yetty::updateGridSize(uint32_t cols, uint32_t rows) noexcept {
    _cols = cols;
    _rows = rows;

#if !YETTY_WEB
    if (_terminal) {
        _terminal->resize(cols, rows);
    }
#endif
    if (_demoGrid) {
        _demoGrid->resize(cols, rows);
    }
}

} // namespace yetty
