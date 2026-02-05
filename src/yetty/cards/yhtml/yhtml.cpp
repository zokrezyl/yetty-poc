#include "yhtml.h"
#include "html-container.h"
#include "http-fetcher.h"
#include <litehtml.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

// Resolve a font family to TTF path via fontconfig
std::string resolveFontPath(const char* family, int weight = 400, bool italic = false) {
    std::string result;
#ifdef YETTY_USE_FONTCONFIG
    FcConfig* config = FcInitLoadConfigAndFonts();
    if (!config) return result;

    FcPattern* pattern = FcPatternCreate();
    if (!pattern) {
        FcConfigDestroy(config);
        return result;
    }

    FcPatternAddString(pattern, FC_FAMILY, reinterpret_cast<const FcChar8*>(family));

    int fcWeight = FC_WEIGHT_REGULAR;
    if (weight <= 300) fcWeight = FC_WEIGHT_LIGHT;
    else if (weight <= 400) fcWeight = FC_WEIGHT_REGULAR;
    else if (weight <= 500) fcWeight = FC_WEIGHT_MEDIUM;
    else if (weight <= 700) fcWeight = FC_WEIGHT_BOLD;
    else fcWeight = FC_WEIGHT_BLACK;
    FcPatternAddInteger(pattern, FC_WEIGHT, fcWeight);
    FcPatternAddInteger(pattern, FC_SLANT, italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN);

    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult fcResult;
    FcPattern* match = FcFontMatch(config, pattern, &fcResult);
    FcPatternDestroy(pattern);

    if (match && fcResult == FcResultMatch) {
        FcChar8* fontPath = nullptr;
        if (FcPatternGetString(match, FC_FILE, 0, &fontPath) == FcResultMatch) {
            result = reinterpret_cast<const char*>(fontPath);
        }
        FcPatternDestroy(match);
    }
    FcConfigDestroy(config);
#else
    (void)family; (void)weight; (void)italic;
#endif
    return result;
}
}

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YHtml::YHtml(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _payloadStr(payload)
    , _fontManager(ctx.fontManager)
{
    _shaderGlyph = SHADER_GLYPH;
}

YHtml::~YHtml() {
    dispose();
}

//=============================================================================
// Factory
//=============================================================================

Result<YHtml::Ptr> YHtml::createImpl(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YHtml::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto card = std::make_shared<YHtml>(ctx, x, y, widthCells, heightCells,
                                         args, payload);

    if (auto res = card->init(); !res) {
        yerror("YHtml::create: init FAILED: {}", error_msg(res));
        return Err<Ptr>("YHtml::create: init failed");
    }

    yinfo("YHtml::create: SUCCESS");
    return Ok(std::move(card));
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YHtml::init() {
    // Create own atlas for HTML fonts (NOT the terminal monospace font)
    base::ObjectFactoryContext factoryCtx;
    auto atlasRes = MsdfAtlas::create(factoryCtx);
    if (!atlasRes) {
        return Err<void>("YHtml::init: failed to create atlas");
    }
    _atlas = *atlasRes;

    // Load default sans-serif font as fontId=0
    std::string defaultFontPath = resolveFontPath("sans-serif");
    if (defaultFontPath.empty()) {
        return Err<void>("YHtml::init: fontconfig could not resolve 'sans-serif'");
    }
    int fontId = addFont(defaultFontPath);
    if (fontId < 0) {
        return Err<void>("YHtml::init: failed to load default sans-serif font");
    }
    yinfo("YHtml::init: loaded default sans-serif font: {} -> fontId={}", defaultFontPath, fontId);

    // Allocate metadata slot
    auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
    if (!metaResult) {
        return Err<void>("YHtml::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;
    _dirty = true;
    _metadataDirty = true;

    if (auto res = registerForEvents(); !res) {
        ywarn("YHtml::init: event registration failed: {}", error_msg(res));
    }

    // Create HTTP fetcher
    auto fetcherResult = HttpFetcher::create();
    if (!fetcherResult) {
        return Err<void>("YHtml: failed to create HttpFetcher");
    }
    _fetcher = std::move(*fetcherResult);

    parseArgs(_argsStr);

    // Default to white background
    _bgColor = 0xFFFFFFFF;

    // Load HTML content now, but defer rendering until setCellSize()
    if (auto res = loadContent(); !res) {
        ywarn("YHtml::init: failed to load content: {}", error_msg(res));
    }

    markDirty();
    return Ok();
}

//=============================================================================
// Args parsing
//=============================================================================

void YHtml::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--input") {
            std::string val;
            if (iss >> val) _inputSource = val;
        } else if (token == "--font-size") {
            float val;
            if (iss >> val) _fontSize = val;
        } else if (token == "--bg-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.size() > 2 &&
                    (colorStr.substr(0, 2) == "0x" ||
                     colorStr.substr(0, 2) == "0X")) {
                    colorStr = colorStr.substr(2);
                }
                _bgColor = static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16));
            }
        }
    }
}

//=============================================================================
// Content loading
//=============================================================================

Result<void> YHtml::loadContent() {
    if (!_inputSource.empty()) {
        if (HttpFetcher::isUrl(_inputSource)) {
            _fetcher->setBaseUrl(_inputSource);
            yinfo("YHtml::loadContent: fetching URL: {}", _inputSource);
            auto body = _fetcher->fetch(_inputSource);
            if (!body) {
                return Err<void>("YHtml::loadContent: failed to fetch URL");
            }
            _htmlContent = std::move(*body);
        } else {
            yinfo("YHtml::loadContent: reading file: {}", _inputSource);
            std::ifstream file(_inputSource);
            if (!file) {
                return Err<void>("YHtml::loadContent: failed to open file");
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            _htmlContent = buffer.str();
        }
    } else if (!_payloadStr.empty()) {
        _htmlContent = _payloadStr;
    }

    if (_htmlContent.empty()) {
        return Err<void>("YHtml::loadContent: empty content");
    }

    yinfo("YHtml::loadContent: {} bytes", _htmlContent.size());
    return Ok();
}

//=============================================================================
// setCellSize — called by GPUScreen at registration and on zoom
//=============================================================================

void YHtml::setCellSize(uint32_t cellWidth, uint32_t cellHeight) {
    if (_cellWidthPx == cellWidth && _cellHeightPx == cellHeight) return;
    _cellWidthPx = cellWidth;
    _cellHeightPx = cellHeight;

    float pixelWidth = static_cast<float>(widthCells() * cellWidth);
    if (pixelWidth > 0 && !_htmlContent.empty()) {
        _viewWidth = pixelWidth;
        _needsHtmlRender = true;
        yinfo("YHtml::setCellSize: {}x{} -> viewport {}px",
              cellWidth, cellHeight, _viewWidth);
    }
}

//=============================================================================
// allocateBuffers — render HTML here (after setCellSize provides px dims)
//=============================================================================

Result<void> YHtml::allocateBuffers() {
    if (_needsHtmlRender) {
        clear();
        if (auto res = renderHtml(); !res) {
            ywarn("YHtml::allocateBuffers: render failed: {}", error_msg(res));
        }
        _needsHtmlRender = false;
        markDirty();
    }

    if (!_primitives.empty() || !_glyphs.empty()) {
        uint32_t derivedSize = computeDerivedSize();
        if (derivedSize > 0 && !_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedSize);
            if (!storageResult) {
                return Err<void>("YHtml::allocateBuffers: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
        _dirty = true;
        yinfo("YHtml::allocateBuffers: {} prims, {} glyphs, derived {} bytes",
              _primitives.size(), _glyphs.size(), derivedSize);
    }
    return Ok();
}

//=============================================================================
// HTML rendering via litehtml
//=============================================================================

Result<void> YHtml::renderHtml() {
    if (_htmlContent.empty()) {
        return Err<void>("YHtml::renderHtml: no content loaded");
    }

    // Create container that targets this YHtml card
    auto containerResult = HtmlContainer::create(
        this, nullptr, _fontSize, _fetcher.get());
    if (!containerResult) {
        return Err<void>("YHtml::renderHtml: failed to create HtmlContainer");
    }
    _container = std::move(*containerResult);

    int viewW = static_cast<int>(_viewWidth);
    int viewH = static_cast<int>(_viewWidth * 1.5f);
    _container->setViewportSize(viewW, viewH);

    _document = litehtml::document::createFromString(
        _htmlContent.c_str(), _container.get());

    if (!_document) {
        return Err<void>("YHtml::renderHtml: failed to parse HTML");
    }

    _document->render(viewW);

    int docHeight = _document->height();
    _container->setViewportSize(viewW, docHeight);

    // Scene bounds cover full document
    setSceneBounds(0.0f, 0.0f,
                   static_cast<float>(viewW),
                   static_cast<float>(docHeight));

    litehtml::position clip(0, 0, viewW, docHeight);
    _document->draw(0, 0, 0, &clip);

    yinfo("YHtml::renderHtml: layout {}x{}, prims={} glyphs={}",
          viewW, docHeight, _primitives.size(), _glyphs.size());

    return Ok();
}

//=============================================================================
// Public API - Text (ypdf-style glyph placement)
//=============================================================================

void YHtml::addText(float x, float y, const char* text,
                    float fontSize, uint32_t color, uint32_t layer, int fontId) {
    if (!_atlas || !text || !*text) return;

    float fontBaseSize = _atlas->getFontSize();
    if (fontBaseSize <= 0.0f) fontBaseSize = 1.0f;

    float scale = fontSize / fontBaseSize;
    float cursorX = x;

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text);
    const uint8_t* end = ptr + std::strlen(text);

    while (ptr < end) {
        // Decode UTF-8 codepoint
        uint32_t codepoint = 0;
        if ((*ptr & 0x80) == 0) {
            codepoint = *ptr++;
        } else if ((*ptr & 0xE0) == 0xC0) {
            codepoint = (*ptr++ & 0x1F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF0) == 0xE0) {
            codepoint = (*ptr++ & 0x0F) << 12;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF8) == 0xF0) {
            codepoint = (*ptr++ & 0x07) << 18;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) codepoint |= (*ptr++ & 0x3F);
        } else {
            ptr++;
            continue;
        }

        // Load glyph from atlas
        uint32_t glyphIndex;
        if (fontId > 0) {
            glyphIndex = _atlas->loadGlyph(fontId, codepoint);
        } else if (_font) {
            glyphIndex = _font->getGlyphIndex(codepoint);
        } else {
            continue;
        }

        const auto& metadata = _atlas->getGlyphMetadata();
        if (glyphIndex >= metadata.size()) {
            cursorX += fontSize * 0.5f;
            continue;
        }

        const auto& glyph = metadata[glyphIndex];

        // Place glyph in scene coordinates using atlas bearingX/Y
        HtmlGlyph g = {};
        g.x = cursorX + glyph._bearingX * scale;
        g.y = y - glyph._bearingY * scale;
        g.width = glyph._sizeX * scale;
        g.height = glyph._sizeY * scale;
        if (codepoint == 'H' || codepoint == 'g' || codepoint == 'y' || codepoint == 'j') {
            ydebug("YHtml::addText glyph='{}' baseline={:.1f} bearingY={:.1f} sizeY={:.1f} scale={:.3f} -> g.y={:.1f} g.h={:.1f} bottom={:.1f}",
                   static_cast<char>(codepoint), y, glyph._bearingY, glyph._sizeY, scale, g.y, g.height, g.y + g.height);
        }
        g.glyphIndex = glyphIndex;
        g.color = color;
        g.layer = layer;
        _glyphs.push_back(g);

        cursorX += glyph._advance * scale;
    }
}

//=============================================================================
// Public API - SDF Primitives
//=============================================================================

void YHtml::computeAABB(HtmlSDFPrimitive& prim) {
    float expand = prim.strokeWidth * 0.5f;

    switch (static_cast<SDFType>(prim.type)) {
        case SDFType::Circle: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = prim.params[2] + prim.round + expand;
            float hh = prim.params[3] + prim.round + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Segment: {
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - expand;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - expand;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + expand;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + expand;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({prim.params[4], prim.params[5],
                                   prim.params[6], prim.params[7]});
            float hw = prim.params[2] + maxR + expand;
            float hh = prim.params[3] + maxR + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        default: {
            // Generic AABB from first two params (center) + some size
            float size = expand + 10.0f;
            prim.aabbMinX = prim.params[0] - size;
            prim.aabbMinY = prim.params[1] - size;
            prim.aabbMaxX = prim.params[0] + size;
            prim.aabbMaxY = prim.params[1] + size;
            break;
        }
    }
}

uint32_t YHtml::addPrimitive(HtmlSDFPrimitive& prim) {
    if (prim.aabbMinX == 0 && prim.aabbMaxX == 0) {
        computeAABB(prim);
    }
    uint32_t idx = static_cast<uint32_t>(_primitives.size());
    _primitives.push_back(prim);
    return idx;
}

uint32_t YHtml::addBox(float cx, float cy, float halfW, float halfH,
                       uint32_t fillColor, uint32_t strokeColor,
                       float strokeWidth, float round, uint32_t layer) {
    HtmlSDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Box);
    prim.layer = layer;
    prim.params[0] = cx;
    prim.params[1] = cy;
    prim.params[2] = halfW;
    prim.params[3] = halfH;
    prim.fillColor = fillColor;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    prim.round = round;
    return addPrimitive(prim);
}

uint32_t YHtml::addSegment(float x0, float y0, float x1, float y1,
                           uint32_t strokeColor, float strokeWidth,
                           uint32_t layer) {
    HtmlSDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Segment);
    prim.layer = layer;
    prim.params[0] = x0;
    prim.params[1] = y0;
    prim.params[2] = x1;
    prim.params[3] = y1;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    return addPrimitive(prim);
}

uint32_t YHtml::addCircle(float cx, float cy, float radius,
                          uint32_t fillColor, uint32_t strokeColor,
                          float strokeWidth, uint32_t layer) {
    HtmlSDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(SDFType::Circle);
    prim.layer = layer;
    prim.params[0] = cx;
    prim.params[1] = cy;
    prim.params[2] = radius;
    prim.fillColor = fillColor;
    prim.strokeColor = strokeColor;
    prim.strokeWidth = strokeWidth;
    return addPrimitive(prim);
}

void YHtml::setBgColor(uint32_t color) {
    _bgColor = color;
    _metadataDirty = true;
}

void YHtml::setSceneBounds(float minX, float minY, float maxX, float maxY) {
    _sceneMinX = minX;
    _sceneMinY = minY;
    _sceneMaxX = maxX;
    _sceneMaxY = maxY;
    _hasExplicitBounds = true;
}

void YHtml::clear() {
    _primitives.clear();
    _glyphs.clear();
    _hasExplicitBounds = false;
}

//=============================================================================
// Font registration
//=============================================================================

int YHtml::addFont(const std::string& ttfPath) {
    if (!_atlas || !_fontManager) return -1;

    auto stem = std::filesystem::path(ttfPath).stem().string();
    auto cacheDir = _fontManager->getCacheDir();
    auto cdbPath = cacheDir + "/" + stem + ".cdb";

    // Check local cache
    auto it = _fontIdCache.find(cdbPath);
    if (it != _fontIdCache.end()) return it->second;

    // Generate CDB if missing
    if (!std::filesystem::exists(cdbPath)) {
        auto provider = _fontManager->getCdbProvider();
        if (!provider) {
            yerror("YHtml::addFont: no CDB provider available");
            return -1;
        }

        MsdfCdbConfig cfg;
        cfg.ttfPath = ttfPath;
        cfg.cdbPath = cdbPath;

        if (auto res = provider->generate(cfg); !res) {
            yerror("YHtml::addFont: CDB generation failed for {}: {}",
                   ttfPath, res.error().message());
            return -1;
        }
    }

    int fontId = _atlas->openCdb(cdbPath);
    if (fontId >= 0) {
        _fontIdCache[cdbPath] = fontId;
    }
    return fontId;
}

//=============================================================================
// Scene bounds computation
//=============================================================================

void YHtml::computeSceneBounds() {
    if (_hasExplicitBounds) return;

    _sceneMinX = 1e10f;
    _sceneMinY = 1e10f;
    _sceneMaxX = -1e10f;
    _sceneMaxY = -1e10f;

    for (const auto& p : _primitives) {
        _sceneMinX = std::min(_sceneMinX, p.aabbMinX);
        _sceneMinY = std::min(_sceneMinY, p.aabbMinY);
        _sceneMaxX = std::max(_sceneMaxX, p.aabbMaxX);
        _sceneMaxY = std::max(_sceneMaxY, p.aabbMaxY);
    }
    for (const auto& g : _glyphs) {
        _sceneMinX = std::min(_sceneMinX, g.x);
        _sceneMinY = std::min(_sceneMinY, g.y);
        _sceneMaxX = std::max(_sceneMaxX, g.x + g.width);
        _sceneMaxY = std::max(_sceneMaxY, g.y + g.height);
    }

    float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
    float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
    _sceneMinX -= padX;
    _sceneMinY -= padY;
    _sceneMaxX += padX;
    _sceneMaxY += padY;

    if (_sceneMinX >= _sceneMaxX) { _sceneMinX = 0; _sceneMaxX = 100; }
    if (_sceneMinY >= _sceneMaxY) { _sceneMinY = 0; _sceneMaxY = 100; }
}

//=============================================================================
// Grid building — both SDF primitives and MSDF glyphs
//=============================================================================

void YHtml::buildGrid() {
    if (!_derivedStorage.isValid() || _gridWidth == 0 || _gridHeight == 0) return;

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t primBytes = static_cast<uint32_t>(_primitives.size() * sizeof(HtmlSDFPrimitive));
    uint32_t gridTotalU32 = _gridWidth * _gridHeight * cellStride;

    // Grid starts after primitives in derived storage
    uint32_t* grid = reinterpret_cast<uint32_t*>(_derivedStorage.data + primBytes);
    std::memset(grid, 0, gridTotalU32 * sizeof(uint32_t));

    uint32_t primsPlaced = 0, primsDropped = 0;
    uint32_t glyphsPlaced = 0, glyphsDropped = 0;
    uint32_t maxCellCount = 0;

    // Insert SDF primitives (bit 31 clear)
    for (uint32_t pi = 0; pi < static_cast<uint32_t>(_primitives.size()); pi++) {
        const auto& p = _primitives[pi];

        uint32_t cellMinX = static_cast<uint32_t>(
            std::clamp((p.aabbMinX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMaxX = static_cast<uint32_t>(
            std::clamp((p.aabbMaxX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMinY = static_cast<uint32_t>(
            std::clamp((p.aabbMinY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
        uint32_t cellMaxY = static_cast<uint32_t>(
            std::clamp((p.aabbMaxY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

        bool placed = false;
        for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
            for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * _gridWidth + cx;
                uint32_t cellOffset = cellIndex * cellStride;
                uint32_t count = grid[cellOffset];
                if (count < MAX_ENTRIES_PER_CELL) {
                    grid[cellOffset + 1 + count] = pi;  // No GLYPH_BIT
                    grid[cellOffset] = count + 1;
                    placed = true;
                    if (count + 1 > maxCellCount) maxCellCount = count + 1;
                }
            }
        }
        if (placed) primsPlaced++;
        else primsDropped++;
    }

    // Insert MSDF glyphs (bit 31 set)
    for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
        const auto& g = _glyphs[gi];

        uint32_t cellMinX = static_cast<uint32_t>(
            std::clamp((g.x - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMaxX = static_cast<uint32_t>(
            std::clamp((g.x + g.width - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMinY = static_cast<uint32_t>(
            std::clamp((g.y - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
        uint32_t cellMaxY = static_cast<uint32_t>(
            std::clamp((g.y + g.height - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

        bool placed = false;
        for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
            for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * _gridWidth + cx;
                uint32_t cellOffset = cellIndex * cellStride;
                uint32_t count = grid[cellOffset];
                if (count < MAX_ENTRIES_PER_CELL) {
                    grid[cellOffset + 1 + count] = gi | GLYPH_BIT;
                    grid[cellOffset] = count + 1;
                    placed = true;
                    if (count + 1 > maxCellCount) maxCellCount = count + 1;
                }
            }
        }
        if (placed) glyphsPlaced++;
        else glyphsDropped++;
    }

    yinfo("YHtml::buildGrid: grid={}x{} cellSize={:.1f} prims={}/{} glyphs={}/{} "
          "maxCellCount={}/{}",
          _gridWidth, _gridHeight, _cellSize,
          primsPlaced, _primitives.size(), glyphsPlaced, _glyphs.size(),
          maxCellCount, MAX_ENTRIES_PER_CELL);
}

//=============================================================================
// Compute derived storage size (primitives + grid + glyphs)
//=============================================================================

uint32_t YHtml::computeDerivedSize() const {
    if (_primitives.empty() && _glyphs.empty()) return 0;

    float sceneWidth = _sceneMaxX - _sceneMinX;
    float sceneHeight = _sceneMaxY - _sceneMinY;
    float sceneArea = sceneWidth * sceneHeight;

    uint32_t gridW = 0, gridH = 0;
    if ((!_primitives.empty() || !_glyphs.empty()) && sceneArea > 0) {
        float cs = _cellSize;
        if (cs <= 0.0f) {
            // Estimate from average glyph height
            float avgH = 0.0f;
            for (const auto& g : _glyphs) avgH += g.height;
            if (!_glyphs.empty()) avgH /= _glyphs.size();
            else avgH = 10.0f;
            cs = avgH * 2.0f;
            float minCS = std::sqrt(sceneArea / 65536.0f);
            float maxCS = std::sqrt(sceneArea / 16.0f);
            cs = std::clamp(cs, minCS, maxCS);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
        const uint32_t MAX_GRID_DIM = 2048;
        gridW = std::min(gridW, MAX_GRID_DIM);
        gridH = std::min(gridH, MAX_GRID_DIM);
    }

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t primBytes = static_cast<uint32_t>(_primitives.size() * sizeof(HtmlSDFPrimitive));
    uint32_t gridBytes = gridW * gridH * cellStride * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(HtmlGlyph));
    return primBytes + gridBytes + glyphBytes;
}

//=============================================================================
// Rebuild and upload — compute grid, copy data to GPU buffer
//=============================================================================

Result<void> YHtml::rebuildAndUpload() {
    computeSceneBounds();

    float sceneWidth = _sceneMaxX - _sceneMinX;
    float sceneHeight = _sceneMaxY - _sceneMinY;

    uint32_t gridW = 0, gridH = 0;
    float cellSize = _cellSize;

    if (!_primitives.empty() || !_glyphs.empty()) {
        float sceneArea = sceneWidth * sceneHeight;
        if (cellSize <= 0.0f) {
            float avgH = 0.0f;
            for (const auto& g : _glyphs) avgH += g.height;
            if (!_glyphs.empty()) avgH /= _glyphs.size();
            else avgH = 10.0f;
            cellSize = avgH * 2.0f;
            float minCellSize = std::sqrt(sceneArea / 65536.0f);
            float maxCellSize = std::sqrt(sceneArea / 16.0f);
            cellSize = std::clamp(cellSize, minCellSize, maxCellSize);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cellSize)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cellSize)));
        const uint32_t MAX_GRID_DIM = 2048;
        if (gridW > MAX_GRID_DIM) { gridW = MAX_GRID_DIM; cellSize = sceneWidth / gridW; }
        if (gridH > MAX_GRID_DIM) { gridH = MAX_GRID_DIM; cellSize = std::max(cellSize, sceneHeight / gridH); }
    }

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t primBytes = static_cast<uint32_t>(_primitives.size() * sizeof(HtmlSDFPrimitive));
    uint32_t gridTotalU32 = gridW * gridH * cellStride;
    uint32_t gridBytes = gridTotalU32 * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(HtmlGlyph));
    uint32_t derivedTotalSize = primBytes + gridBytes + glyphBytes;

    yinfo("YHtml::rebuild: grid={}x{} cellSize={:.1f} derivedTotal={} "
          "prims={} glyphs={} zoom={:.2f}",
          gridW, gridH, cellSize, derivedTotalSize,
          _primitives.size(), _glyphs.size(), _viewZoom);

    // Allocate or reallocate derived storage if needed
    if (derivedTotalSize > 0) {
        if (!_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedTotalSize);
            if (!storageResult) {
                return Err<void>("YHtml::rebuild: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        } else if (derivedTotalSize > _derivedStorage.size) {
            _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedTotalSize);
            if (!storageResult) {
                _derivedStorage = StorageHandle::invalid();
                return Err<void>("YHtml::rebuild: failed to reallocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
    }

    if (_derivedStorage.isValid() && derivedTotalSize > 0) {
        uint8_t* base = _derivedStorage.data;
        uint32_t offset = 0;

        // Zero entire buffer
        std::memset(base, 0, _derivedStorage.size);

        // Copy primitives
        _primitiveOffset = (_derivedStorage.offset + offset) / sizeof(float);
        if (!_primitives.empty()) {
            std::memcpy(base + offset, _primitives.data(), primBytes);
        }
        offset += primBytes;

        // Build grid
        _gridWidth = gridW;
        _gridHeight = gridH;
        _cellSize = cellSize;
        _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
        offset += gridBytes;

        buildGrid();

        // Copy glyphs
        _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
        if (!_glyphs.empty()) {
            std::memcpy(base + offset, _glyphs.data(), glyphBytes);
        }

        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _metadataDirty = true;
    return Ok();
}

//=============================================================================
// Metadata upload — 64-byte struct with f16 zoom, i16 pan encoding
//=============================================================================

Result<void> YHtml::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YHtml::uploadMetadata: invalid metadata handle");
    }

    // Pack zoom as IEEE 754 half-float
    uint32_t zoomBits;
    {
        uint32_t f32bits;
        std::memcpy(&f32bits, &_viewZoom, sizeof(float));
        uint32_t sign = (f32bits >> 16) & 0x8000;
        int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
        uint32_t mant = (f32bits >> 13) & 0x3FF;
        if (exp <= 0) { exp = 0; mant = 0; }
        else if (exp >= 31) { exp = 31; mant = 0; }
        zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
    }

    float contentW = _sceneMaxX - _sceneMinX;
    float contentH = _sceneMaxY - _sceneMinY;
    int16_t panXi16 = static_cast<int16_t>(std::clamp(
        _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
    int16_t panYi16 = static_cast<int16_t>(std::clamp(
        _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

    Metadata meta = {};
    meta.primitiveOffset = _primitiveOffset;
    meta.primitiveCount = static_cast<uint32_t>(_primitives.size());
    meta.gridOffset = _gridOffset;
    meta.gridWidth = _gridWidth;
    meta.gridHeight = _gridHeight;
    std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
    std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
    meta.widthCells  = (_widthCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
    meta.heightCells = (_heightCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
    meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
    meta.bgColor = _bgColor;

    ydebug("YHtml::uploadMetadata: grid={}x{} cellSize={} "
           "scene=[{},{},{},{}] zoom={:.2f} pan=({:.1f},{:.1f}) size={}x{} bgColor={:#010x}",
           meta.gridWidth, meta.gridHeight, _cellSize,
           _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY,
           _viewZoom, _viewPanX, _viewPanY,
           _widthCells, _heightCells, meta.bgColor);

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YHtml::uploadMetadata: write failed");
    }

    return Ok();
}

//=============================================================================
// Card lifecycle
//=============================================================================

void YHtml::declareBufferNeeds() {
    uint32_t lastDerivedSize = _derivedStorage.size;

    if (_derivedStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
        _derivedStorage = StorageHandle::invalid();
    }

    if (!_primitives.empty() || !_glyphs.empty()) {
        if (lastDerivedSize > 0) {
            _cardMgr->bufferManager()->reserve(lastDerivedSize);
        } else {
            uint32_t estDerived = computeDerivedSize();
            _cardMgr->bufferManager()->reserve(estDerived);
        }
    }
}

Result<void> YHtml::render(float /*time*/) {
    if (_dirty) {
        if (auto res = rebuildAndUpload(); !res) {
            return Err<void>("YHtml::render: rebuildAndUpload failed", res);
        }
        _dirty = false;
    }

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("YHtml::render: metadata upload failed", res);
        }
        _metadataDirty = false;
    }

    return Ok();
}

Result<void> YHtml::dispose() {
    deregisterFromEvents();

    if (_derivedStorage.isValid() && _cardMgr) {
        if (auto res = _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage); !res) {
            yerror("YHtml::dispose: deallocateBuffer failed: {}", error_msg(res));
        }
        _derivedStorage = StorageHandle::invalid();
    }

    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YHtml::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

void YHtml::suspend() {
    if (_derivedStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
        _derivedStorage = StorageHandle::invalid();
    }
}

//=============================================================================
// Event handling - zoom/pan
//=============================================================================

Result<void> YHtml::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Err<void>("YHtml::registerForEvents: no EventLoop instance", loopResult);
    }
    auto loop = *loopResult;
    auto self = sharedAs<base::EventListener>();

    if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
        return Err<void>("YHtml::registerForEvents: failed to register SetFocus", res);
    }
    if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) {
        return Err<void>("YHtml::registerForEvents: failed to register CardScroll", res);
    }
    yinfo("YHtml card {} registered for events (priority 1000)", id());
    return Ok();
}

Result<void> YHtml::deregisterFromEvents() {
    if (weak_from_this().expired()) {
        return Ok();
    }
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Ok();
    }
    auto loop = *loopResult;
    if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
        ywarn("YHtml::deregisterFromEvents: {}", error_msg(res));
    }
    return Ok();
}

Result<bool> YHtml::onEvent(const base::Event& event) {
    if (event.type == base::Event::Type::SetFocus) {
        if (event.setFocus.objectId == id()) {
            if (!_focused) {
                _focused = true;
                ydebug("YHtml::onEvent: focused (id={})", id());
            }
            return Ok(true);
        } else if (_focused) {
            _focused = false;
            ydebug("YHtml::onEvent: unfocused (id={})", id());
        }
        return Ok(false);
    }

    if (event.type == base::Event::Type::CardScroll &&
        event.cardScroll.targetId == id()) {

        float sceneW = _sceneMaxX - _sceneMinX;
        float sceneH = _sceneMaxY - _sceneMinY;

        if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
            float zoomFactor = std::exp(event.cardScroll.dy * 0.1f);
            float newZoom = std::clamp(_viewZoom * zoomFactor, 0.1f, 5000.0f);
            if (newZoom != _viewZoom) {
                _viewZoom = newZoom;
                _metadataDirty = true;
            }
            return Ok(true);
        } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
            float dx = event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            _viewPanX += dx;
            _metadataDirty = true;
            return Ok(true);
        } else {
            float dy = event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            _viewPanY += dy;
            _metadataDirty = true;
            return Ok(true);
        }
    }

    return Ok(false);
}

} // namespace yetty::card
