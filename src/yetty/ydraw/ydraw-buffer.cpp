#include "ydraw-buffer.h"
#include "ydraw-prim-writer.gen.h"

#include <cstring>

namespace yetty {

Result<YDrawBuffer::Ptr> YDrawBuffer::createImpl() {
    return Ok(Ptr(new YDrawBuffer()));
}

//=============================================================================
// Font blob storage (raw data only, no FreeType)
//=============================================================================

int YDrawBuffer::addFontBlob(const uint8_t* data, size_t size,
                              const std::string& name) {
    int id = _nextFontId++;
    _fonts.push_back({id, name, {data, data + size}});
    return id;
}

//=============================================================================
// Text span storage
//=============================================================================

void YDrawBuffer::addText(float x, float y, const std::string& text,
                           float fontSize, uint32_t color,
                           uint32_t layer, int fontId) {
    _textSpans.push_back({x, y, text, fontSize, color, layer, fontId, 0.0f});
}

void YDrawBuffer::addRotatedText(float x, float y, const std::string& text,
                                  float fontSize, uint32_t color,
                                  float rotation, int fontId) {
    _textSpans.push_back({x, y, text, fontSize, color, 0, fontId, rotation});
}

//=============================================================================
// Scene metadata
//=============================================================================

void YDrawBuffer::setSceneBounds(float minX, float minY, float maxX, float maxY) {
    _sceneMinX = minX;
    _sceneMinY = minY;
    _sceneMaxX = maxX;
    _sceneMaxY = maxY;
    _hasSceneBounds = true;
}

//=============================================================================
// Primitive operations (unchanged)
//=============================================================================

uint32_t YDrawBuffer::nextAutoId() {
    return _nextAutoId++;
}

Result<uint32_t> YDrawBuffer::addPrim(uint32_t id, const float* data, uint32_t wordCount) {
    if (id == AUTO_ID) {
        id = nextAutoId();
    } else if (_prims.count(id)) {
        return Err<uint32_t>("YDrawBuffer::addPrim: id " + std::to_string(id) + " already exists");
    }

    PrimData pd;
    pd.words.assign(data, data + wordCount);

    if (_deltaMode) {
        _delta.push_back({DeltaOp::UPSERT, id, pd.words});
    } else {
        _prims.emplace(id, std::move(pd));
    }
    return Ok(id);
}

Result<void> YDrawBuffer::updatePrim(uint32_t id, const float* data, uint32_t wordCount) {
    if (_deltaMode) {
        if (!_prims.count(id)) {
            bool foundInDelta = false;
            for (const auto& op : _delta) {
                if (op.id == id && op.type == DeltaOp::UPSERT) foundInDelta = true;
                if (op.id == id && op.type == DeltaOp::REMOVE) foundInDelta = false;
            }
            if (!foundInDelta) {
                return Err<void>("YDrawBuffer::updatePrim: id " + std::to_string(id) + " not found");
            }
        }
        std::vector<float> words(data, data + wordCount);
        _delta.push_back({DeltaOp::UPSERT, id, std::move(words)});
    } else {
        auto it = _prims.find(id);
        if (it == _prims.end()) {
            return Err<void>("YDrawBuffer::updatePrim: id " + std::to_string(id) + " not found");
        }
        it->second.words.assign(data, data + wordCount);
    }
    return Ok();
}

Result<void> YDrawBuffer::remove(uint32_t id) {
    if (_deltaMode) {
        if (!_prims.count(id)) {
            bool foundInDelta = false;
            for (const auto& op : _delta) {
                if (op.id == id && op.type == DeltaOp::UPSERT) foundInDelta = true;
                if (op.id == id && op.type == DeltaOp::REMOVE) foundInDelta = false;
            }
            if (!foundInDelta) {
                return Err<void>("YDrawBuffer::remove: id " + std::to_string(id) + " not found");
            }
        }
        _delta.push_back({DeltaOp::REMOVE, id, {}});
    } else {
        auto it = _prims.find(id);
        if (it == _prims.end()) {
            return Err<void>("YDrawBuffer::remove: id " + std::to_string(id) + " not found");
        }
        _prims.erase(it);
    }
    return Ok();
}

void YDrawBuffer::clear() {
    _prims.clear();
    _delta.clear();
    _deltaMode = false;
    _nextAutoId = AUTO_ID_BASE;
    _textSpans.clear();
    // Keep fonts — they're reusable across clears
    // Keep scene metadata
}

//=============================================================================
// Output
//=============================================================================

uint32_t YDrawBuffer::buildCompact(std::vector<float>& out) const {
    out.clear();
    for (const auto& [id, pd] : _prims) {
        out.insert(out.end(), pd.words.begin(), pd.words.end());
    }
    return static_cast<uint32_t>(out.size());
}

uint32_t YDrawBuffer::gpuBufferSize() const {
    uint32_t count = static_cast<uint32_t>(_prims.size());
    uint32_t dataWords = 0;
    for (const auto& [id, pd] : _prims) {
        dataWords += static_cast<uint32_t>(pd.words.size());
    }
    return (count + dataWords) * sizeof(float);
}

void YDrawBuffer::writeGPU(float* buf, uint32_t bufBytes,
                           std::vector<uint32_t>& wordOffsets) const {
    uint32_t count = static_cast<uint32_t>(_prims.size());
    wordOffsets.resize(count);

    float* dataBase = buf + count;
    uint32_t dataOffset = 0;
    uint32_t i = 0;

    for (const auto& [id, pd] : _prims) {
        wordOffsets[i] = dataOffset;
        uint32_t off = dataOffset;
        std::memcpy(&buf[i], &off, sizeof(uint32_t));
        std::memcpy(dataBase + dataOffset, pd.words.data(),
                    pd.words.size() * sizeof(float));
        dataOffset += static_cast<uint32_t>(pd.words.size());
        i++;
    }
}

//=============================================================================
// Serialization
//=============================================================================

std::vector<uint8_t> YDrawBuffer::serialize() {
    std::vector<float> compact;
    buildCompact(compact);

    uint32_t pc = static_cast<uint32_t>(_prims.size());
    uint32_t totalWords = static_cast<uint32_t>(compact.size());

    // Binary format: [primCount:u32][totalWords:u32][compact_data]
    std::vector<uint8_t> out;
    out.resize(8 + totalWords * sizeof(float));
    std::memcpy(out.data(), &pc, 4);
    std::memcpy(out.data() + 4, &totalWords, 4);
    if (totalWords > 0) {
        std::memcpy(out.data() + 8, compact.data(), totalWords * sizeof(float));
    }

    _deltaMode = true;
    return out;
}

std::vector<uint8_t> YDrawBuffer::serializeDelta() {
    uint32_t opCount = static_cast<uint32_t>(_delta.size());
    std::vector<uint8_t> out;

    out.resize(4);
    std::memcpy(out.data(), &opCount, 4);

    for (const auto& op : _delta) {
        size_t pos = out.size();
        if (op.type == DeltaOp::UPSERT) {
            uint32_t wordCount = static_cast<uint32_t>(op.data.size());
            out.resize(pos + 1 + 4 + 4 + wordCount * sizeof(float));
            out[pos] = 0;
            std::memcpy(out.data() + pos + 1, &op.id, 4);
            std::memcpy(out.data() + pos + 5, &wordCount, 4);
            if (wordCount > 0) {
                std::memcpy(out.data() + pos + 9, op.data.data(),
                            wordCount * sizeof(float));
            }
        } else {
            out.resize(pos + 1 + 4);
            out[pos] = 1;
            std::memcpy(out.data() + pos + 1, &op.id, 4);
        }
    }

    for (const auto& op : _delta) {
        if (op.type == DeltaOp::UPSERT) {
            PrimData pd;
            pd.words = op.data;
            _prims[op.id] = std::move(pd);
        } else {
            _prims.erase(op.id);
        }
    }
    _delta.clear();

    return out;
}

Result<void> YDrawBuffer::deserialize(const uint8_t* data, size_t size) {
    if (size < 8) return Err<void>("YDrawBuffer::deserialize: data too small");

    uint32_t pc, totalWords;
    std::memcpy(&pc, data, 4);
    std::memcpy(&totalWords, data + 4, 4);

    size_t expectedSize = 8 + static_cast<size_t>(totalWords) * sizeof(float);
    if (size < expectedSize) {
        return Err<void>("YDrawBuffer::deserialize: data truncated");
    }

    clear();

    const float* buf = reinterpret_cast<const float*>(data + 8);
    uint32_t wordPos = 0;
    for (uint32_t i = 0; i < pc && wordPos < totalWords; i++) {
        uint32_t type = sdf::detail::read_u32(buf + wordPos, 0);
        uint32_t wc = sdf::wordCountForType(type);
        if (wc == 0) break;
        if (wordPos + wc > totalWords) break;
        addPrim(AUTO_ID, buf + wordPos, wc);
        wordPos += wc;
    }

    return Ok();
}

} // namespace yetty
