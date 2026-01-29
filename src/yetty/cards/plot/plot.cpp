#include "plot.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace yetty::card {

//=============================================================================
// PlotImpl - Implementation of Plot
//=============================================================================

class PlotImpl : public Plot {
public:
    PlotImpl(CardBufferManager::Ptr mgr, const GPUContext& gpu,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
        : Plot(std::move(mgr), gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~PlotImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("Plot::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("Plot::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Parse payload if provided
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("Plot::init: failed to parse payload: {}", error_msg(res));
            }
        }

        // Upload initial metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("Plot::init: failed to upload metadata");
        }

        return Ok();
    }

    Result<void> dispose() override {
        if (_storageHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateStorage(_storageHandle);
            _storageHandle = StorageHandle::invalid();
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        return Ok();
    }

    Result<void> update(float time) override {
        (void)time;

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("Plot::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

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

        // Deallocate old storage if exists
        if (_storageHandle.isValid()) {
            _cardMgr->deallocateStorage(_storageHandle);
            _storageHandle = StorageHandle::invalid();
        }

        // Allocate new storage
        uint32_t storageSize = count * sizeof(float);
        yinfo("Plot::setData: allocating {} bytes for {} floats", storageSize, count);

        auto storageResult = _cardMgr->allocateStorage(storageSize);
        if (!storageResult) {
            yerror("Plot::setData: failed to allocate storage");
            return Err<void>("Plot::setData: failed to allocate storage");
        }
        _storageHandle = *storageResult;

        yinfo("Plot::setData: storage allocated at byte offset {}, float index {}",
              _storageHandle.offset, _storageHandle.offset / sizeof(float));

        // Write data to storage
        if (auto res = _cardMgr->writeStorage(_storageHandle, _data.data(), storageSize); !res) {
            yerror("Plot::setData: failed to write storage");
            return Err<void>("Plot::setData: failed to write storage");
        }

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

private:
    void parseArgs(const std::string& args) {
        yinfo("Plot::parseArgs: args='{}'", args);

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
            } else if (token == "--line-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    _lineColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
                }
            } else if (token == "--fill-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    _fillColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
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

        return setData(values);
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

        Metadata meta = {};  // Zero-initialize including reserved
        meta.plotType = static_cast<uint32_t>(_plotType);
        // dataOffset is in float units (not bytes) since cardStorage is array<f32>
        meta.dataOffset = _storageHandle.isValid() ? (_storageHandle.offset / sizeof(float)) : 0;
        meta.dataCount = static_cast<uint32_t>(_data.size());
        meta.minValue = _minValue;
        meta.maxValue = _maxValue;
        meta.lineColor = _lineColor;
        meta.fillColor = _fillColor;
        meta.flags = _flags;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.bgColor = _bgColor;

        yinfo("Plot::uploadMetadata: metaOffset={} plotType={} dataOffset={} "
              "dataCount={} min={} max={} flags={} size={}x{} bgColor={:#x}",
              _metaHandle.offset, meta.plotType, meta.dataOffset, meta.dataCount,
              meta.minValue, meta.maxValue, meta.flags,
              meta.widthCells, meta.heightCells, meta.bgColor);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("Plot::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout)
    struct Metadata {
        uint32_t plotType;
        uint32_t dataOffset;
        uint32_t dataCount;
        float minValue;
        float maxValue;
        uint32_t lineColor;
        uint32_t fillColor;
        uint32_t flags;
        uint32_t widthCells;
        uint32_t heightCells;
        uint32_t bgColor;
        uint32_t _reserved[5];
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    PlotType _plotType = PlotType::Line;
    std::vector<float> _data;
    float _minValue = 0.0f;
    float _maxValue = 1.0f;
    bool _autoRange = true;
    uint32_t _lineColor = 0xFF00FF00;   // Green
    uint32_t _fillColor = 0x8000FF00;   // Semi-transparent green
    uint32_t _bgColor = 0xFF1A1A2E;     // Dark blue-ish background
    uint32_t _flags = FLAG_GRID | FLAG_AXES;

    StorageHandle _storageHandle = StorageHandle::invalid();
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> Plot::create(
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("Plot::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("Plot::create: null CardBufferManager!");
        return Err<CardPtr>("Plot::create: null CardBufferManager");
    }

    auto card = std::make_shared<PlotImpl>(
        std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);

    yinfo("Plot::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("Plot::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("Plot::create: init failed");
    }

    yinfo("Plot::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<Plot::Ptr> Plot::createImpl(
    ContextType& ctx,
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx; // ObjectFactory context marker

    auto result = create(std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create Plot", result);
    }
    // Dynamic cast from CardPtr to Plot::Ptr
    auto plotCard = std::dynamic_pointer_cast<Plot>(*result);
    if (!plotCard) {
        return Err<Ptr>("Created card is not a Plot");
    }
    return Ok(plotCard);
}

} // namespace yetty::card
