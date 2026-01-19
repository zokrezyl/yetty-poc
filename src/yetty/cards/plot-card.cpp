#include "plot-card.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace yetty {

PlotCard::PlotCard(CardBufferManager* mgr, int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells,
                   const std::string& args, const std::string& payload)
    : Card(mgr, x, y, widthCells, heightCells)
    , argsStr_(args)
    , payloadStr_(payload)
{
    shaderGlyph_ = SHADER_GLYPH;
}

PlotCard::~PlotCard() {
    dispose();
}

Result<CardPtr> PlotCard::create(
    CardBufferManager* mgr,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("PlotCard::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("PlotCard::create: null CardBufferManager!");
        return Err<CardPtr>("PlotCard::create: null CardBufferManager");
    }

    // Use unique_ptr with custom constructor access
    auto card = std::unique_ptr<PlotCard>(
        new PlotCard(mgr, x, y, widthCells, heightCells, args, payload));

    yinfo("PlotCard::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("PlotCard::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("PlotCard::create: init failed");
    }

    yinfo("PlotCard::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok(CardPtr(card.release()));
}

Result<void> PlotCard::init() {
    // Allocate metadata slot (32 bytes)
    auto metaResult = cardMgr_->allocateMetadata(sizeof(Metadata));
    if (!metaResult) {
        return Err<void>("PlotCard::init: failed to allocate metadata");
    }
    metaHandle_ = *metaResult;

    yinfo("PlotCard::init: allocated metadata at offset {}", metaHandle_.offset);

    // Parse args
    parseArgs(argsStr_);

    // Parse payload if provided
    if (!payloadStr_.empty()) {
        if (auto res = parsePayload(payloadStr_); !res) {
            ywarn("PlotCard::init: failed to parse payload: {}", error_msg(res));
        }
    }

    // Upload initial metadata
    if (auto res = uploadMetadata(); !res) {
        return Err<void>("PlotCard::init: failed to upload metadata");
    }

    return Ok();
}

void PlotCard::dispose() {
    if (storageHandle_.isValid() && cardMgr_) {
        cardMgr_->deallocateStorage(storageHandle_);
        storageHandle_ = StorageHandle::invalid();
    }

    if (metaHandle_.isValid() && cardMgr_) {
        cardMgr_->deallocateMetadata(metaHandle_);
        metaHandle_ = MetadataHandle::invalid();
    }
}

void PlotCard::update(float time) {
    (void)time;

    if (metadataDirty_) {
        uploadMetadata();
        metadataDirty_ = false;
    }
}

void PlotCard::parseArgs(const std::string& args) {
    yinfo("PlotCard::parseArgs: args='{}'", args);

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--type" || token == "-t") {
            std::string typeStr;
            if (iss >> typeStr) {
                if (typeStr == "line") plotType_ = PlotType::Line;
                else if (typeStr == "bar") plotType_ = PlotType::Bar;
                else if (typeStr == "scatter") plotType_ = PlotType::Scatter;
                else if (typeStr == "area") plotType_ = PlotType::Area;
            }
        } else if (token == "--grid") {
            flags_ |= FLAG_GRID;
        } else if (token == "--no-grid") {
            flags_ &= ~FLAG_GRID;
        } else if (token == "--axes") {
            flags_ |= FLAG_AXES;
        } else if (token == "--no-axes") {
            flags_ &= ~FLAG_AXES;
        } else if (token == "--line-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                // Parse hex color like "0xFF00FF00" or "FF00FF00"
                if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                    colorStr = colorStr.substr(2);
                }
                lineColor_ = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
            }
        } else if (token == "--fill-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                    colorStr = colorStr.substr(2);
                }
                fillColor_ = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
            }
        } else if (token == "--min") {
            float val;
            if (iss >> val) {
                minValue_ = val;
                autoRange_ = false;
            }
        } else if (token == "--max") {
            float val;
            if (iss >> val) {
                maxValue_ = val;
                autoRange_ = false;
            }
        }
    }

    metadataDirty_ = true;
}

Result<void> PlotCard::parsePayload(const std::string& payload) {
    yinfo("PlotCard::parsePayload: payload length={}", payload.size());
    if (payload.size() > 0) {
        yinfo("PlotCard::parsePayload: first 100 chars: '{}'", payload.substr(0, 100));
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
                ywarn("PlotCard::parsePayload: invalid float '{}'", token);
            }
        }
    }

    yinfo("PlotCard::parsePayload: parsed {} values", values.size());
    if (values.size() >= 5) {
        yinfo("PlotCard::parsePayload: first 5 values: {} {} {} {} {}",
              values[0], values[1], values[2], values[3], values[4]);
    }

    if (values.empty()) {
        return Err<void>("No valid data points in payload");
    }

    return setData(values);
}

Result<void> PlotCard::setData(const std::vector<float>& data) {
    return setData(data.data(), static_cast<uint32_t>(data.size()));
}

Result<void> PlotCard::setData(const float* data, uint32_t count) {
    yinfo("PlotCard::setData: count={}", count);

    if (count == 0 || !data) {
        return Err<void>("PlotCard::setData: empty data");
    }

    // Copy data locally
    data_.assign(data, data + count);

    yinfo("PlotCard::setData: copied {} floats, first few: {} {} {} {} {}",
          data_.size(),
          count > 0 ? data_[0] : 0.0f,
          count > 1 ? data_[1] : 0.0f,
          count > 2 ? data_[2] : 0.0f,
          count > 3 ? data_[3] : 0.0f,
          count > 4 ? data_[4] : 0.0f);

    // Calculate range if auto
    if (autoRange_) {
        calculateRange();
    }

    // Deallocate old storage if exists
    if (storageHandle_.isValid()) {
        cardMgr_->deallocateStorage(storageHandle_);
        storageHandle_ = StorageHandle::invalid();
    }

    // Allocate new storage
    uint32_t storageSize = count * sizeof(float);
    yinfo("PlotCard::setData: allocating {} bytes for {} floats", storageSize, count);

    auto storageResult = cardMgr_->allocateStorage(storageSize);
    if (!storageResult) {
        yerror("PlotCard::setData: failed to allocate storage");
        return Err<void>("PlotCard::setData: failed to allocate storage");
    }
    storageHandle_ = *storageResult;

    yinfo("PlotCard::setData: storage allocated at byte offset {}, float index {}",
          storageHandle_.offset, storageHandle_.offset / sizeof(float));

    // Write data to storage
    if (auto res = cardMgr_->writeStorage(storageHandle_, data_.data(), storageSize); !res) {
        yerror("PlotCard::setData: failed to write storage");
        return Err<void>("PlotCard::setData: failed to write storage");
    }

    yinfo("PlotCard::setData: {} floats written to storage", count);

    metadataDirty_ = true;
    return Ok();
}

void PlotCard::calculateRange() {
    if (data_.empty()) {
        minValue_ = 0.0f;
        maxValue_ = 1.0f;
        return;
    }

    minValue_ = *std::min_element(data_.begin(), data_.end());
    maxValue_ = *std::max_element(data_.begin(), data_.end());

    // Add 5% padding
    float range = maxValue_ - minValue_;
    if (range < 0.0001f) {
        range = 1.0f;
    }
    minValue_ -= range * 0.05f;
    maxValue_ += range * 0.05f;
}

void PlotCard::setPlotType(PlotType type) {
    plotType_ = type;
    metadataDirty_ = true;
}

void PlotCard::setRange(float minVal, float maxVal) {
    minValue_ = minVal;
    maxValue_ = maxVal;
    autoRange_ = false;
    metadataDirty_ = true;
}

void PlotCard::setLineColor(uint32_t color) {
    lineColor_ = color;
    metadataDirty_ = true;
}

void PlotCard::setFillColor(uint32_t color) {
    fillColor_ = color;
    metadataDirty_ = true;
}

void PlotCard::setShowGrid(bool show) {
    if (show) flags_ |= FLAG_GRID;
    else flags_ &= ~FLAG_GRID;
    metadataDirty_ = true;
}

void PlotCard::setShowAxes(bool show) {
    if (show) flags_ |= FLAG_AXES;
    else flags_ &= ~FLAG_AXES;
    metadataDirty_ = true;
}

Result<void> PlotCard::uploadMetadata() {
    if (!metaHandle_.isValid()) {
        return Err<void>("PlotCard::uploadMetadata: invalid metadata handle");
    }

    Metadata meta = {};  // Zero-initialize including reserved
    meta.plotType = static_cast<uint32_t>(plotType_);
    // dataOffset is in float units (not bytes) since cardStorage is array<f32>
    meta.dataOffset = storageHandle_.isValid() ? (storageHandle_.offset / sizeof(float)) : 0;
    meta.dataCount = static_cast<uint32_t>(data_.size());
    meta.minValue = minValue_;
    meta.maxValue = maxValue_;
    meta.lineColor = lineColor_;
    meta.fillColor = fillColor_;
    meta.flags = flags_;
    // Widget bounds for computing UV across entire widget
    meta.widgetCol = static_cast<uint32_t>(x_);
    meta.widgetRow = static_cast<uint32_t>(y_);
    meta.widthCells = widthCells_;
    meta.heightCells = heightCells_;

    yinfo("PlotCard::uploadMetadata: metaOffset={} plotType={} dataOffset={} dataCount={} min={} max={} flags={} widget=({},{}) size={}x{}",
          metaHandle_.offset, meta.plotType, meta.dataOffset, meta.dataCount,
          meta.minValue, meta.maxValue, meta.flags,
          meta.widgetCol, meta.widgetRow, meta.widthCells, meta.heightCells);

    if (auto res = cardMgr_->writeMetadata(metaHandle_, &meta, sizeof(meta)); !res) {
        return Err<void>("PlotCard::uploadMetadata: write failed");
    }

    return Ok();
}

void registerPlotCard(CardFactory& factory) {
    factory.registerCard("plot", PlotCard::create);
}

} // namespace yetty
