#include "plot.h"
#include "plot-sampler-provider.h"
#include "plot-transformer-provider.h"
#include "plot-renderer-provider.h"
#include <yetty/base/event-loop.h>
#include <yetty/yetty-context.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/shader-manager.h>
#include <yetty/yast/yast.h>
#include "../../yecho/yecho-parser.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

// GLFW modifier constants
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

namespace yetty::card {

//=============================================================================
// PlotImpl - Implementation of Plot
//=============================================================================

class PlotImpl : public Plot {
public:
    PlotImpl(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
        : Plot(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _shaderMgr(ctx.shaderManager)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;

        // Look up glyph indices for axis label characters
        if (ctx.fontManager) {
            auto font = ctx.fontManager->getDefaultMsMsdfFont();
            if (font) {
                _glyphBase0 = font->getGlyphIndex('0');
                _glyphDot   = font->getGlyphIndex('.');
                _glyphMinus = font->getGlyphIndex('-');
                yinfo("Plot: glyph indices: '0'={} '.'={} '-'={}",
                      _glyphBase0, _glyphDot, _glyphMinus);
            }
        }
    }

    ~PlotImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface - resource declarations
    //=========================================================================

    bool needsBuffer() const override {
        // Plot needs buffer storage for data
        return true;
    }

    bool needsBufferRealloc() override {
        // Request realloc if we have data/buffers but no valid handle yet
        if (!_data.empty() && !_storageHandle.isValid()) return true;
        for (const auto& buf : _bufferDecls) {
            if (buf.size > 0 && !buf.handle.isValid()) return true;
        }
        return false;
    }

    //=========================================================================
    // Card interface - lifecycle
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Plot::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("Plot::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args (may set up expression mode)
        parseArgs(_argsStr);

        // If expression mode, register transforms with provider
        if ((_flags & FLAG_EXPR) && !_plotResult.plots.empty()) {
            auto& transformerProv = *PlotTransformerProvider::instance();
            _transformBase = 0xFFFFFFFF;
            for (size_t i = 0; i < _plotResult.plots.size() && i < 8; i++) {
                const auto& p = _plotResult.plots[i];
                uint32_t idx = transformerProv.registerTransform(p.name, p.expr);
                _transformIndices.push_back(idx);
                if (_transformBase == 0xFFFFFFFF) {
                    _transformBase = idx;
                }
            }
            _transformCount = static_cast<uint32_t>(_transformIndices.size());
            yinfo("Plot::init: expression mode, {} transforms, base={}", _transformCount, _transformBase);
        }

        // Parse payload if provided (for buffer mode)
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("Plot::init: failed to parse payload: {}", error_msg(res));
            }
        }

        // Upload initial metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("Plot::init: failed to upload metadata");
        }

        // Register for events - cards have priority 1000, above gpu-screen
        if (auto res = registerForEvents(); !res) {
            return Err<void>("Plot::init: failed to register for events", res);
        }

        return Ok();
    }

    Result<void> dispose() override {
        // Unregister all transforms from provider
        if (!_transformIndices.empty()) {
            auto& transformerProv = *PlotTransformerProvider::instance();
            for (uint32_t idx : _transformIndices) {
                transformerProv.unregisterTransform(idx);
            }
            _transformIndices.clear();
            _transformBase = 0xFFFFFFFF;
            _transformCount = 0;
        }

        // Deregister from events
        if (auto res = deregisterFromEvents(); !res) {
            return Err<void>("Plot::dispose: failed to deregister", res);
        }

        _storageHandle = StorageHandle::invalid();

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        return Ok();
    }

    void suspend() override {
        _storageHandle = StorageHandle::invalid();

        // Invalidate declared buffer handles (data is preserved in buf.data)
        for (auto& buf : _bufferDecls) {
            buf.handle = StorageHandle::invalid();
        }

        yinfo("Plot::suspend: deallocated storage, _data has {} floats, {} declared buffers",
              _data.size(), _bufferDecls.size());
    }

    void declareBufferNeeds() override {
        // Save live data from GPU handles back to local storage before repack
        // (_data is always kept in sync for legacy buffer, no save needed)
        _storageHandle = StorageHandle::invalid();

        for (auto& buf : _bufferDecls) {
            if (buf.handle.isValid() && buf.size > 0) {
                // Save streaming data from GPU buffer back to local storage
                buf.data.resize(buf.size);
                std::memcpy(buf.data.data(), buf.handle.data, buf.size * sizeof(float));
            }
            buf.handle = StorageHandle::invalid();
        }

        // Reserve for legacy _data buffer
        if (!_data.empty()) {
            uint32_t storageSize = static_cast<uint32_t>(_data.size() * sizeof(float));
            _cardMgr->bufferManager()->reserve(storageSize);
        }

        // Reserve for declared buffers
        for (auto& buf : _bufferDecls) {
            if (buf.size > 0) {
                uint32_t storageSize = buf.size * sizeof(float);
                _cardMgr->bufferManager()->reserve(storageSize);
            }
        }
    }

    Result<void> allocateBuffers() override {
        // Allocate legacy _data buffer
        if (!_data.empty()) {
            uint32_t storageSize = static_cast<uint32_t>(_data.size() * sizeof(float));
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "storage", storageSize);
            if (!storageResult) {
                return Err<void>("Plot::allocateBuffers: failed to allocate storage");
            }
            _storageHandle = *storageResult;
            std::memcpy(_storageHandle.data, _data.data(), storageSize);
            _cardMgr->bufferManager()->markBufferDirty(_storageHandle);
            _metadataDirty = true;
            yinfo("Plot::allocateBuffers: allocated storage at offset {}", _storageHandle.offset);
        }

        // Allocate declared buffers
        for (auto& buf : _bufferDecls) {
            if (buf.size > 0) {
                uint32_t storageSize = buf.size * sizeof(float);
                auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), buf.name, storageSize);
                if (!storageResult) {
                    return Err<void>("Plot::allocateBuffers: failed to allocate buffer '" + buf.name + "'");
                }
                buf.handle = *storageResult;

                // Restore data saved during declareBufferNeeds
                if (!buf.data.empty()) {
                    size_t copySize = std::min(buf.data.size(), static_cast<size_t>(buf.size)) * sizeof(float);
                    std::memcpy(buf.handle.data, buf.data.data(), copySize);
                } else {
                    // Zero-initialize for streaming
                    std::memset(buf.handle.data, 0, storageSize);
                }
                _cardMgr->bufferManager()->markBufferDirty(buf.handle);
                yinfo("Plot::allocateBuffers: allocated buffer '{}' at offset {}, size={}",
                      buf.name, buf.handle.offset, buf.size);
            }
        }

        return Ok();
    }

    Result<void> finalize() override {
        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("Plot::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // Update API (OSC update command)
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
        yinfo("Plot::update: args='{}' payload_len={}", args, payload.size());

        // Parse args for advance=N and buffer=name
        uint32_t advanceCount = 0;
        std::string targetBuffer;

        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token.rfind("advance=", 0) == 0) {
                advanceCount = static_cast<uint32_t>(std::stoul(token.substr(8)));
            } else if (token.rfind("buffer=", 0) == 0) {
                targetBuffer = token.substr(7);
            }
        }

        // Parse payload as comma-separated floats
        std::vector<float> newValues;
        if (!payload.empty()) {
            std::istringstream payloadStream(payload);
            std::string valToken;
            while (std::getline(payloadStream, valToken, ',')) {
                size_t start = valToken.find_first_not_of(" \t\n\r");
                size_t end = valToken.find_last_not_of(" \t\n\r");
                if (start != std::string::npos && end != std::string::npos) {
                    try {
                        newValues.push_back(std::stof(valToken.substr(start, end - start + 1)));
                    } catch (...) {}
                }
            }
        }

        yinfo("Plot::update: advance={} buffer='{}' newValues={}",
              advanceCount, targetBuffer, newValues.size());

        // Resolve target buffer - named buffer, first declared buffer, or default storage
        StorageHandle* targetHandle = nullptr;
        uint32_t bufferSize = 0;

        if (!targetBuffer.empty()) {
            auto* buf = findBuffer(targetBuffer);
            if (!buf || !buf->handle.isValid()) {
                return Err<void>("Plot::update: buffer '" + targetBuffer + "' not found or not allocated");
            }
            targetHandle = &buf->handle;
            bufferSize = buf->size;
        } else if (!_bufferDecls.empty() && _bufferDecls[0].handle.isValid()) {
            targetHandle = &_bufferDecls[0].handle;
            bufferSize = _bufferDecls[0].size;
        } else if (_storageHandle.isValid()) {
            targetHandle = &_storageHandle;
            bufferSize = static_cast<uint32_t>(_data.size());
        }

        if (!targetHandle || !targetHandle->isValid() || bufferSize == 0) {
            return Err<void>("Plot::update: no buffer available");
        }

        // Apply advance+append to the buffer
        applyBufferUpdate(*targetHandle, bufferSize, advanceCount, newValues);

        // Recalculate range if auto and this affects the default buffer
        if (_autoRange && targetHandle == &_storageHandle) {
            // Sync GPU data back to _data for range calculation
            float* bufData = reinterpret_cast<float*>(_storageHandle.data);
            _data.assign(bufData, bufData + bufferSize);
            calculateRange();
        }

        _metadataDirty = true;
        return Ok();
    }

    //=========================================================================
    // Plot-specific API
    //=========================================================================

    void setPlotType(PlotType type) override {
        _plotType = type;
        _metadataDirty = true;
    }

    PlotType plotType() const override { return _plotType; }

    Result<void> setData(const std::vector<float>& data) override {
        return setData(data.data(), static_cast<uint32_t>(data.size()));
    }

    Result<void> setData(const float* data, uint32_t count) override {
        yinfo("Plot::setData: count={}", count);

        if (count == 0 || !data) {
            return Err<void>("Plot::setData: empty data");
        }

        // Copy data locally
        _data.assign(data, data + count);

        yinfo("Plot::setData: copied {} floats, first few: {} {} {} {} {}",
              _data.size(),
              count > 0 ? _data[0] : 0.0f,
              count > 1 ? _data[1] : 0.0f,
              count > 2 ? _data[2] : 0.0f,
              count > 3 ? _data[3] : 0.0f,
              count > 4 ? _data[4] : 0.0f);

        // Calculate range if auto
        if (_autoRange) {
            calculateRange();
        }

        // Allocate new storage
        uint32_t storageSize = count * sizeof(float);
        yinfo("Plot::setData: allocating {} bytes for {} floats", storageSize, count);

        auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "storage", storageSize);
        if (!storageResult) {
            yerror("Plot::setData: failed to allocate storage");
            return Err<void>("Plot::setData: failed to allocate storage");
        }
        _storageHandle = *storageResult;

        yinfo("Plot::setData: storage allocated at byte offset {}, float index {}",
              _storageHandle.offset, _storageHandle.offset / sizeof(float));

        // Write data directly to buffer
        std::memcpy(_storageHandle.data, _data.data(), storageSize);
        _cardMgr->bufferManager()->markBufferDirty(_storageHandle);

        yinfo("Plot::setData: {} floats written to storage", count);

        _metadataDirty = true;
        return Ok();
    }

    void setRange(float minVal, float maxVal) override {
        _minValue = minVal;
        _maxValue = maxVal;
        _autoRange = false;
        _metadataDirty = true;
    }

    void setAutoRange(bool enabled) override {
        _autoRange = enabled;
        if (_autoRange && !_data.empty()) {
            calculateRange();
            _metadataDirty = true;
        }
    }

    float minValue() const override { return _minValue; }
    float maxValue() const override { return _maxValue; }

    void setLineColor(uint32_t color) override {
        _lineColor = color;
        _metadataDirty = true;
    }

    void setFillColor(uint32_t color) override {
        _fillColor = color;
        _metadataDirty = true;
    }

    void setBgColor(uint32_t color) override {
        _bgColor = color;
        _metadataDirty = true;
    }

    uint32_t lineColor() const override { return _lineColor; }
    uint32_t fillColor() const override { return _fillColor; }
    uint32_t bgColor() const override { return _bgColor; }

    void setShowGrid(bool show) override {
        if (show) _flags |= FLAG_GRID;
        else _flags &= ~FLAG_GRID;
        _metadataDirty = true;
    }

    void setShowAxes(bool show) override {
        if (show) _flags |= FLAG_AXES;
        else _flags &= ~FLAG_AXES;
        _metadataDirty = true;
    }

    uint32_t dataCount() const override {
        return static_cast<uint32_t>(_data.size());
    }

    //=========================================================================
    // EventListener interface
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        // Handle SetFocus events
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("Plot::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("Plot::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // Handle scroll events (only when focused)
        if (_focused && event.type == base::Event::Type::Scroll) {
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+Scroll: zoom
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_zoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _zoom) {
                    _zoom = newZoom;
                    _metadataDirty = true;
                    yinfo("Plot::onEvent: zoom={:.2f}", _zoom);
                }
                return Ok(true);
            } else if (event.scroll.mods & GLFW_MOD_SHIFT) {
                // Shift+Scroll: pan Y
                float dy = event.scroll.dy * 0.05f / _zoom;
                _centerY = std::clamp(_centerY + dy, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            } else {
                // Plain scroll: pan X
                float dx = event.scroll.dy * 0.05f / _zoom;
                _centerX = std::clamp(_centerX + dx, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Event registration
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Plot::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("Plot::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("Plot::registerForEvents: failed to register Scroll", res);
        }

        yinfo("Plot card {} registered for events (priority 1000)", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) {
            return Ok();
        }

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("Plot::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("Plot::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    // Parse a value that might contain pi constants
    float parseValue(const std::string& str) {
        if (str == "pi") return 3.14159265f;
        if (str == "-pi") return -3.14159265f;
        try {
            return std::stof(str);
        } catch (...) {
            return 0.0f;
        }
    }

    // Parse a range value: min..max
    void parseDomainRange(const std::string& value, float& minVal, float& maxVal) {
        auto dots = value.find("..");
        if (dots != std::string::npos) {
            minVal = parseValue(value.substr(0, dots));
            maxVal = parseValue(value.substr(dots + 2));
        }
    }

    // Parse braced notation using YEchoParser
    void parseBracedArgs(const std::string& args) {
        yinfo("Plot::parseBracedArgs: args='{}'", args);

        YEchoParser parser;
        auto spans = parser.parse(args);

        if (spans.empty() || spans[0].type != YEchoSpan::Type::Block) {
            ywarn("Plot::parseBracedArgs: failed to parse braced notation");
            return;
        }

        const auto& span = spans[0];

        // Process attributes
        for (const auto& attr : span.attrs) {
            if (attr.key == "@range") {
                // @range=xmin..xmax,ymin..ymax
                auto comma = attr.value.find(',');
                if (comma != std::string::npos) {
                    parseDomainRange(attr.value.substr(0, comma), _domainMin, _domainMax);
                    float ymin = _minValue, ymax = _maxValue;
                    parseDomainRange(attr.value.substr(comma + 1), ymin, ymax);
                    _minValue = ymin;
                    _maxValue = ymax;
                    _autoRange = false;
                }
            } else if (attr.key == "x") {
                // x=-pi..pi (x domain)
                parseDomainRange(attr.value, _domainMin, _domainMax);
            } else if (attr.key == "@axes") {
                if (attr.value == "on" || attr.value == "true" || attr.value == "1") {
                    _flags |= FLAG_AXES;
                } else {
                    _flags &= ~FLAG_AXES;
                }
            } else if (attr.key == "@grid") {
                if (attr.value == "on" || attr.value == "true" || attr.value == "1") {
                    _flags |= FLAG_GRID;
                } else {
                    _flags &= ~FLAG_GRID;
                }
            } else if (attr.value == "buffer") {
                // f=buffer declares a buffer named 'f'
                auto* buf = findOrCreateBuffer(attr.key);
                yinfo("Plot::parseBracedArgs: declared buffer '{}'", attr.key);
            } else if (attr.key.size() > 1 && attr.key[0] == '@') {
                // Check for @name.property pattern
                std::string fullKey = attr.key.substr(1);  // Remove leading @
                auto dotPos = fullKey.find('.');
                if (dotPos != std::string::npos) {
                    std::string bufName = fullKey.substr(0, dotPos);
                    std::string prop = fullKey.substr(dotPos + 1);

                    auto* buf = findOrCreateBuffer(bufName);
                    if (prop == "size") {
                        buf->size = parseSizeWithSuffix(attr.value);
                        yinfo("Plot::parseBracedArgs: buffer '{}' size={}", bufName, buf->size);
                    } else if (prop == "values") {
                        if (attr.value.empty()) {
                            // @f.values= means take from payload
                            buf->valuesFromPayload = true;
                            yinfo("Plot::parseBracedArgs: buffer '{}' values from payload", bufName);
                        } else if (attr.value.front() == '"' || attr.value.front() == '\'') {
                            // @f.values="file.csv" means load from file
                            buf->valuesSource = attr.value.substr(1, attr.value.size() - 2);
                            yinfo("Plot::parseBracedArgs: buffer '{}' values from file '{}'", bufName, buf->valuesSource);
                        } else {
                            // @f.values=1,2,3 means inline values
                            buf->valuesSource = attr.value;
                            yinfo("Plot::parseBracedArgs: buffer '{}' inline values", bufName);
                        }
                    }
                }
            }
        }

        // Parse content as expression
        if (!span.content.empty()) {
            _plotResult = yast::plotExpressionToWGSL(span.content);
            if (!_plotResult.plots.empty()) {
                _flags |= FLAG_EXPR;
                yinfo("Plot::parseBracedArgs: parsed {} functions", _plotResult.plots.size());
                for (size_t i = 0; i < _plotResult.plots.size(); i++) {
                    const auto& p = _plotResult.plots[i];
                    yinfo("  [{}] {} = '{}' color='{}' text='{}'",
                          i, p.name, p.expr.code, p.color, p.text);
                }

                // Extract colors for each function
                uint32_t defaultColors[8] = {
                    0xFFFF3333,  // Red
                    0xFF3399FF,  // Blue
                    0xFF33CC33,  // Green
                    0xFFFFCC33,  // Yellow
                    0xFFCC33CC,  // Magenta
                    0xFF33CCCC,  // Cyan
                    0xFFFF8800,  // Orange
                    0xFF9955FF   // Purple
                };

                for (size_t i = 0; i < _plotResult.plots.size() && i < 8; i++) {
                    const auto& p = _plotResult.plots[i];
                    uint32_t color = defaultColors[i];

                    // Parse hex color if provided: #RRGGBB -> 0xFFRRGGBB
                    if (!p.color.empty() && p.color[0] == '#' && p.color.size() >= 7) {
                        uint32_t rgb = static_cast<uint32_t>(std::stoul(p.color.substr(1), nullptr, 16));
                        color = 0xFF000000 | rgb;
                    }
                    _functionColors.push_back(color);
                }

                // Set sensible defaults for Y range if auto
                if (_autoRange) {
                    _minValue = -1.5f;
                    _maxValue = 1.5f;
                }
            } else {
                ywarn("Plot::parseBracedArgs: failed to parse expression '{}'", span.content);
            }
        }

        _metadataDirty = true;
    }

    void parseArgs(const std::string& args) {
        yinfo("Plot::parseArgs: args='{}'", args);

        // Check for braced notation
        if (!args.empty() && args.front() == '{') {
            parseBracedArgs(args);
            return;
        }

        // Legacy flag-based parsing
        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--type" || token == "-t") {
                std::string typeStr;
                if (iss >> typeStr) {
                    if (typeStr == "line") _plotType = PlotType::Line;
                    else if (typeStr == "bar") _plotType = PlotType::Bar;
                    else if (typeStr == "scatter") _plotType = PlotType::Scatter;
                    else if (typeStr == "area") _plotType = PlotType::Area;
                }
            } else if (token == "--grid") {
                _flags |= FLAG_GRID;
            } else if (token == "--no-grid") {
                _flags &= ~FLAG_GRID;
            } else if (token == "--axes") {
                _flags |= FLAG_AXES;
            } else if (token == "--no-axes") {
                _flags &= ~FLAG_AXES;
            } else if (token == "--expr" || token == "-e") {
                // Expression mode: --expr "f1 = sin(x); @f1.color = #FF0000; f2 = cos(x)"
                std::string expr;
                if (iss >> std::quoted(expr) || iss >> expr) {
                    _plotResult = yast::plotExpressionToWGSL(expr);
                    if (!_plotResult.plots.empty()) {
                        _flags |= FLAG_EXPR;
                        yinfo("Plot::parseArgs: parsed {} functions", _plotResult.plots.size());
                        for (size_t i = 0; i < _plotResult.plots.size(); i++) {
                            const auto& p = _plotResult.plots[i];
                            yinfo("  [{}] {} = '{}' color='{}' text='{}'",
                                  i, p.name, p.expr.code, p.color, p.text);
                        }
                    } else {
                        ywarn("Plot::parseArgs: failed to parse expression '{}'", expr);
                    }
                }
            } else if (token == "--domain" || token == "-d") {
                // Domain: --domain -pi,pi or --domain -3.14,3.14
                std::string domainStr;
                if (iss >> domainStr) {
                    auto comma = domainStr.find(',');
                    if (comma != std::string::npos) {
                        auto minStr = domainStr.substr(0, comma);
                        auto maxStr = domainStr.substr(comma + 1);
                        _domainMin = parseValue(minStr);
                        _domainMax = parseValue(maxStr);
                    }
                }
            } else if (token == "--min") {
                float val;
                if (iss >> val) {
                    _minValue = val;
                    _autoRange = false;
                }
            } else if (token == "--max") {
                float val;
                if (iss >> val) {
                    _maxValue = val;
                    _autoRange = false;
                }
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    _bgColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
                }
            } else if (token == "--buffer" || token == "-b") {
                // Buffer declaration: --buffer name=size or --buffer name=size,name2=size2
                // Also: --buffer name="file.csv"
                std::string bufSpec;
                if (iss >> bufSpec) {
                    // Split by comma for multiple buffers
                    std::istringstream bufStream(bufSpec);
                    std::string singleBuf;
                    while (std::getline(bufStream, singleBuf, ',')) {
                        auto eqPos = singleBuf.find('=');
                        if (eqPos != std::string::npos) {
                            std::string bufName = singleBuf.substr(0, eqPos);
                            std::string bufValue = singleBuf.substr(eqPos + 1);

                            auto* buf = findOrCreateBuffer(bufName);

                            if (!bufValue.empty() && (bufValue.front() == '"' || bufValue.front() == '\'')) {
                                // File source: name="file.csv"
                                buf->valuesSource = bufValue.substr(1, bufValue.size() - 2);
                                yinfo("Plot::parseArgs: buffer '{}' from file '{}'", bufName, buf->valuesSource);
                            } else {
                                // Size: name=1024 or name=1k
                                buf->size = parseSizeWithSuffix(bufValue);
                                yinfo("Plot::parseArgs: buffer '{}' size={}", bufName, buf->size);
                            }
                        }
                    }
                }
            } else if (token.rfind("--buffer.", 0) == 0) {
                // Buffer property: --buffer.name.prop=value
                std::string rest = token.substr(9);  // Remove "--buffer."
                auto dotPos = rest.find('.');
                if (dotPos != std::string::npos) {
                    std::string bufName = rest.substr(0, dotPos);
                    std::string propAndValue = rest.substr(dotPos + 1);
                    auto eqPos = propAndValue.find('=');

                    std::string prop = (eqPos != std::string::npos) ?
                        propAndValue.substr(0, eqPos) : propAndValue;
                    std::string value = (eqPos != std::string::npos) ?
                        propAndValue.substr(eqPos + 1) : "";

                    auto* buf = findOrCreateBuffer(bufName);
                    if (prop == "size") {
                        buf->size = parseSizeWithSuffix(value);
                        yinfo("Plot::parseArgs: buffer '{}' size={}", bufName, buf->size);
                    } else if (prop == "values") {
                        if (value.empty()) {
                            buf->valuesFromPayload = true;
                            yinfo("Plot::parseArgs: buffer '{}' values from payload", bufName);
                        } else {
                            buf->valuesSource = value;
                            yinfo("Plot::parseArgs: buffer '{}' values='{}'", bufName, value);
                        }
                    }
                }
            }
        }

        // If expression mode, extract colors and set defaults
        if ((_flags & FLAG_EXPR) && !_plotResult.plots.empty()) {
            // Default color palette (ARGB format)
            uint32_t defaultColors[8] = {
                0xFFFF3333,  // Red
                0xFF3399FF,  // Blue
                0xFF33CC33,  // Green
                0xFFFFCC33,  // Yellow
                0xFFCC33CC,  // Magenta
                0xFF33CCCC,  // Cyan
                0xFFFF8800,  // Orange
                0xFF9955FF   // Purple
            };
            
            for (size_t i = 0; i < _plotResult.plots.size() && i < 8; i++) {
                const auto& p = _plotResult.plots[i];
                uint32_t color = defaultColors[i];
                
                // Parse hex color if provided: #RRGGBB -> 0xFFRRGGBB
                if (!p.color.empty() && p.color[0] == '#' && p.color.size() >= 7) {
                    uint32_t rgb = static_cast<uint32_t>(std::stoul(p.color.substr(1), nullptr, 16));
                    color = 0xFF000000 | rgb;
                }
                _functionColors.push_back(color);
            }
            
            // Set sensible defaults for Y range if auto
            if (_autoRange) {
                _minValue = -1.5f;
                _maxValue = 1.5f;
            }
        }

        _metadataDirty = true;
    }

    Result<void> parsePayload(const std::string& payload) {
        yinfo("Plot::parsePayload: payload length={}", payload.size());
        if (payload.size() > 0) {
            yinfo("Plot::parsePayload: first 100 chars: '{}'", payload.substr(0, 100));
        }

        // Parse comma or space separated float values
        std::vector<float> values;
        std::istringstream iss(payload);
        std::string token;

        // Split by comma first
        while (std::getline(iss, token, ',')) {
            // Trim whitespace
            size_t start = token.find_first_not_of(" \t\n\r");
            size_t end = token.find_last_not_of(" \t\n\r");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
                try {
                    values.push_back(std::stof(token));
                } catch (const std::exception& e) {
                    ywarn("Plot::parsePayload: invalid float '{}'", token);
                }
            }
        }

        yinfo("Plot::parsePayload: parsed {} values", values.size());
        if (values.size() >= 5) {
            yinfo("Plot::parsePayload: first 5 values: {} {} {} {} {}",
                  values[0], values[1], values[2], values[3], values[4]);
        }

        if (values.empty()) {
            return Err<void>("No valid data points in payload");
        }

        // Distribute payload across buffers that have valuesFromPayload=true
        // The order of buffer declarations determines payload order
        size_t payloadOffset = 0;
        bool usedByDeclaredBuffers = false;

        for (auto& buf : _bufferDecls) {
            if (buf.valuesFromPayload && buf.size > 0) {
                usedByDeclaredBuffers = true;
                size_t count = std::min(static_cast<size_t>(buf.size), values.size() - payloadOffset);
                if (count > 0) {
                    buf.data.assign(values.begin() + payloadOffset, values.begin() + payloadOffset + count);
                    yinfo("Plot::parsePayload: buffer '{}' got {} values from offset {}",
                          buf.name, count, payloadOffset);
                    payloadOffset += count;
                }
                if (payloadOffset >= values.size()) break;
            }
        }

        // If no declared buffers consumed the payload, use legacy _data
        if (!usedByDeclaredBuffers) {
            _data = std::move(values);

            // Calculate range if auto
            if (_autoRange) {
                calculateRange();
            }
        }

        _metadataDirty = true;
        return Ok();
    }

    void calculateRange() {
        if (_data.empty()) {
            _minValue = 0.0f;
            _maxValue = 1.0f;
            return;
        }

        _minValue = *std::min_element(_data.begin(), _data.end());
        _maxValue = *std::max_element(_data.begin(), _data.end());

        // Add 5% padding
        float range = _maxValue - _minValue;
        if (range < 0.0001f) {
            range = 1.0f;
        }
        _minValue -= range * 0.05f;
        _maxValue += range * 0.05f;
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("Plot::uploadMetadata: invalid metadata handle");
        }

        // Map plot type to render index
        uint32_t renderIdx = static_cast<uint32_t>(_plotType);  // Line=0, Bar=1, Scatter=2, Area=3

        Metadata meta = {};
        meta.plotType = static_cast<uint8_t>(_plotType);
        meta.flags = static_cast<uint8_t>(_flags);
        meta.widthCells = static_cast<uint16_t>(_widthCells);
        meta.heightCells = static_cast<uint16_t>(_heightCells);

        // Determine data offset and count from:
        // 1. Declared buffers (streaming mode) - use first valid buffer
        // 2. Legacy _storageHandle and _data
        if (!_bufferDecls.empty() && _bufferDecls[0].handle.isValid()) {
            // Use first declared buffer for data
            meta.dataOffset = _bufferDecls[0].handle.offset / sizeof(float);
            meta.dataCount = _bufferDecls[0].size;
        } else if (_storageHandle.isValid()) {
            meta.dataOffset = _storageHandle.offset / sizeof(float);
            meta.dataCount = static_cast<uint32_t>(_data.size());
        } else {
            meta.dataOffset = 0;
            meta.dataCount = 0;
        }
        meta.minValue = _minValue;
        meta.maxValue = _maxValue;
        meta.lineColor = _lineColor;
        meta.fillColor = _fillColor;
        meta.bgColor = _bgColor;
        meta.zoom = _zoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;
        meta.glyphBase0 = static_cast<uint8_t>(_glyphBase0);
        meta.glyphDot = static_cast<uint8_t>(_glyphDot);
        meta.glyphMinus = static_cast<uint8_t>(_glyphMinus);
        meta.domainMin = _domainMin;
        meta.domainMax = _domainMax;
        // Pack dispatch indices: samplerIndex(8) | transformBase(8) | transformCount(8) | renderIndex(8)
        uint32_t samplerIdx = (_samplerIndex == 0xFFFFFFFF) ? 0u : _samplerIndex;
        uint32_t transformBaseIdx = (_transformBase == 0xFFFFFFFF) ? 0u : _transformBase;
        meta.dispatchSlot = (samplerIdx & 0xFF)
                          | ((transformBaseIdx & 0xFF) << 8)
                          | ((_transformCount & 0xFF) << 16)
                          | ((renderIdx & 0xFF) << 24);

        yinfo("Plot::uploadMetadata: metaOffset={} plotType={} dataOffset={} "
              "dataCount={} min={} max={} flags={} size={}x{} bgColor={:#x} domain=[{},{}] dispatch=[s={},tBase={},tCount={},r={}]",
              _metaHandle.offset, meta.plotType, meta.dataOffset,
              meta.dataCount, meta.minValue, meta.maxValue, meta.flags,
              meta.widthCells, meta.heightCells, meta.bgColor,
              meta.domainMin, meta.domainMax,
              samplerIdx, transformBaseIdx, _transformCount, renderIdx);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("Plot::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (64 bytes = 16 u32 slots)
    // Shader reads as cardMetadata[metaOffset + N] (raw u32s).
    // Extended data (function colors) goes in cardStorage via dataOffset.
    struct Metadata {
        // [0]: plotType(8) | flags(8) | padding(16)
        uint8_t plotType;
        uint8_t flags;
        uint16_t _pad0;
        // [1]: widthCells(16) | heightCells(16)
        uint16_t widthCells;
        uint16_t heightCells;
        // [2-3]
        uint32_t dataOffset;    // float index into cardStorage (data OR colors)
        uint32_t dataCount;
        // [4-5]
        float minValue;
        float maxValue;
        // [6-8]
        uint32_t lineColor;     // RGBA (legacy, used for single function)
        uint32_t fillColor;     // RGBA
        uint32_t bgColor;       // RGBA
        // [9-11]
        float zoom;
        float centerX;
        float centerY;
        // [12]: glyphBase0(8) | glyphDot(8) | glyphMinus(8) | padding(8)
        uint8_t glyphBase0;
        uint8_t glyphDot;
        uint8_t glyphMinus;
        uint8_t _pad1;
        // [13]: domainMin (f32) - x domain start (for expression mode)
        float domainMin;
        // [14]: domainMax (f32) - x domain end (for expression mode)
        float domainMax;
        // [15]: samplerIndex(8) | transformBase(8) | transformCount(8) | renderIndex(8)
        uint32_t dispatchSlot;
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    // ShaderManager reference (for registering providers once)
    std::shared_ptr<ShaderManager> _shaderMgr;

    PlotType _plotType = PlotType::Line;
    std::vector<float> _data;
    float _minValue = 0.0f;
    float _maxValue = 1.0f;
    bool _autoRange = true;
    uint32_t _lineColor = 0xFF00FF00;   // Green
    uint32_t _fillColor = 0x8000FF00;   // Semi-transparent green
    uint32_t _bgColor = 0xFF1A1A2E;     // Dark blue-ish background
    uint32_t _flags = FLAG_GRID | FLAG_AXES;

    // Zoom and pan
    float _zoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;

    // Font glyph indices for axis labels
    uint32_t _glyphBase0 = 0;
    uint32_t _glyphDot = 0;
    uint32_t _glyphMinus = 0;

    // Expression mode (supports up to 8 functions)
    yast::WGSLGenerator::MultiResult _plotResult;  // Parsed plot expressions with colors
    std::vector<uint32_t> _transformIndices;
    std::vector<uint32_t> _functionColors;  // Per-function colors (ARGB)
    float _domainMin = -3.14159265f;  // Default: -pi
    float _domainMax = 3.14159265f;   // Default: pi
    uint32_t _samplerIndex = 0xFFFFFFFF;    // Not registered
    uint32_t _transformBase = 0xFFFFFFFF;   // First transform index
    uint32_t _transformCount = 0;           // Number of transforms
    uint32_t _renderIndex = 0;              // Default: line (0)

    StorageHandle _storageHandle = StorageHandle::invalid();
    bool _focused = false;
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;

    //=========================================================================
    // Buffer declarations
    //=========================================================================
    struct BufferDeclaration {
        std::string name;           // Buffer name (e.g., "audio", "x", "y")
        uint32_t size = 0;          // Size in floats (0 = deduce from source)
        std::string valuesSource;   // "" = streaming, "=" = payload, "file.csv" = file
        bool valuesFromPayload = false;  // True if @name.values= was specified
        StorageHandle handle = StorageHandle::invalid();
        std::vector<float> data;    // Local copy of buffer data
    };
    std::vector<BufferDeclaration> _bufferDecls;

    // Parse size with optional suffix (1k=1024, 1m=1048576)
    uint32_t parseSizeWithSuffix(const std::string& str) {
        if (str.empty()) return 0;

        size_t len = str.size();
        uint32_t multiplier = 1;
        std::string numPart = str;

        char suffix = std::tolower(str.back());
        if (suffix == 'k') {
            multiplier = 1024;
            numPart = str.substr(0, len - 1);
        } else if (suffix == 'm') {
            multiplier = 1024 * 1024;
            numPart = str.substr(0, len - 1);
        }

        try {
            return static_cast<uint32_t>(std::stoul(numPart)) * multiplier;
        } catch (...) {
            return 0;
        }
    }

    // Find or create buffer declaration by name
    BufferDeclaration* findOrCreateBuffer(const std::string& name) {
        for (auto& buf : _bufferDecls) {
            if (buf.name == name) return &buf;
        }
        _bufferDecls.push_back({name, 0, "", false, StorageHandle::invalid(), {}});
        return &_bufferDecls.back();
    }

    // Find buffer declaration by name (read-only)
    BufferDeclaration* findBuffer(const std::string& name) {
        for (auto& buf : _bufferDecls) {
            if (buf.name == name) return &buf;
        }
        return nullptr;
    }

    // Apply advance+append to a buffer
    // Shifts existing data left by advanceCount, appends newValues at end
    void applyBufferUpdate(StorageHandle& handle, uint32_t bufferSize,
                           uint32_t advanceCount, const std::vector<float>& newValues) {
        if (!handle.isValid() || bufferSize == 0) return;

        float* data = reinterpret_cast<float*>(handle.data);

        if (advanceCount > 0 && advanceCount < bufferSize) {
            // Shift data left by advanceCount
            std::memmove(data, data + advanceCount, (bufferSize - advanceCount) * sizeof(float));
        }

        // Append new values at the end
        if (!newValues.empty()) {
            uint32_t writeStart = (advanceCount > 0) ? (bufferSize - advanceCount) : 0;
            uint32_t writeCount = std::min(static_cast<uint32_t>(newValues.size()),
                                           bufferSize - writeStart);
            std::memcpy(data + writeStart, newValues.data(), writeCount * sizeof(float));
        }

        _cardMgr->bufferManager()->markBufferDirty(handle);
        yinfo("Plot::applyBufferUpdate: shifted by {}, appended {} values",
              advanceCount, newValues.size());
    }
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> Plot::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("Plot::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!ctx.cardManager) {
        yerror("Plot::create: null CardBufferManager!");
        return Err<CardPtr>("Plot::create: null CardBufferManager");
    }

    auto card = std::make_shared<PlotImpl>(
        ctx, x, y, widthCells, heightCells, args, payload);

    yinfo("Plot::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("Plot::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("Plot::create: init failed");
    }

    yinfo("Plot::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<Plot::Ptr> Plot::createImpl(
    ContextType& factoryCtx,
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)factoryCtx;

    auto result = create(ctx, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create Plot", result);
    }
    auto plotCard = std::dynamic_pointer_cast<Plot>(*result);
    if (!plotCard) {
        return Err<Ptr>("Created card is not a Plot");
    }
    return Ok(plotCard);
}

} // namespace yetty::card
