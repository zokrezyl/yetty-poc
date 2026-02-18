#include "ydraw-buffer.h"
#include "ydraw-prim-writer.gen.h"

#include <ytrace/ytrace.hpp>
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
// Image storage
//=============================================================================

void YDrawBuffer::addImage(float x, float y, float w, float h,
                           const uint8_t* pixels, uint32_t pixelWidth, uint32_t pixelHeight,
                           uint32_t layer) {
    ImageData img;
    img.x = x;
    img.y = y;
    img.w = w;
    img.h = h;
    img.pixelWidth = pixelWidth;
    img.pixelHeight = pixelHeight;
    img.layer = layer;
    img.pixels.assign(pixels, pixels + pixelWidth * pixelHeight * 4);
    _images.push_back(std::move(img));
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

// Wire format:
// [magic:4][version:4]
// [primCount:4][totalPrimWords:4][prim_data...]
// [fontCount:4][font0_id:4][font0_nameLen:4][font0_name...][font0_dataLen:4][font0_data...]...
// [textSpanCount:4][span0...]...
//   span: [x:4][y:4][fontSize:4][color:4][layer:4][fontId:4][rotation:4][textLen:4][text...]
// [bgColor:4][flags:4][hasSceneBounds:1][minX:4][minY:4][maxX:4][maxY:4]

static constexpr uint32_t SERIALIZE_MAGIC = 0x59445246; // "YDRF"
static constexpr uint32_t SERIALIZE_VERSION = 1;

std::vector<uint8_t> YDrawBuffer::serialize() {
    std::vector<uint8_t> out;

    auto writeU32 = [&](uint32_t v) {
        size_t pos = out.size();
        out.resize(pos + 4);
        std::memcpy(out.data() + pos, &v, 4);
    };
    auto writeF32 = [&](float v) {
        size_t pos = out.size();
        out.resize(pos + 4);
        std::memcpy(out.data() + pos, &v, 4);
    };
    auto writeU8 = [&](uint8_t v) {
        out.push_back(v);
    };
    auto writeBytes = [&](const uint8_t* data, size_t len) {
        size_t pos = out.size();
        out.resize(pos + len);
        std::memcpy(out.data() + pos, data, len);
    };

    // Magic & version
    writeU32(SERIALIZE_MAGIC);
    writeU32(SERIALIZE_VERSION);

    // Primitives
    std::vector<float> compact;
    buildCompact(compact);
    writeU32(static_cast<uint32_t>(_prims.size()));
    writeU32(static_cast<uint32_t>(compact.size()));
    if (!compact.empty()) {
        writeBytes(reinterpret_cast<const uint8_t*>(compact.data()),
                   compact.size() * sizeof(float));
    }

    // Fonts
    writeU32(static_cast<uint32_t>(_fonts.size()));
    for (const auto& f : _fonts) {
        writeU32(static_cast<uint32_t>(f.fontId));
        writeU32(static_cast<uint32_t>(f.name.size()));
        writeBytes(reinterpret_cast<const uint8_t*>(f.name.data()), f.name.size());
        writeU32(static_cast<uint32_t>(f.ttfData.size()));
        writeBytes(f.ttfData.data(), f.ttfData.size());
    }

    // Text spans
    writeU32(static_cast<uint32_t>(_textSpans.size()));
    for (const auto& s : _textSpans) {
        writeF32(s.x);
        writeF32(s.y);
        writeF32(s.fontSize);
        writeU32(s.color);
        writeU32(s.layer);
        writeU32(static_cast<uint32_t>(s.fontId));
        writeF32(s.rotation);
        writeU32(static_cast<uint32_t>(s.text.size()));
        writeBytes(reinterpret_cast<const uint8_t*>(s.text.data()), s.text.size());
    }

    // Scene metadata
    writeU32(_bgColor);
    writeU32(_flags);
    writeU8(_hasSceneBounds ? 1 : 0);
    writeF32(_sceneMinX);
    writeF32(_sceneMinY);
    writeF32(_sceneMaxX);
    writeF32(_sceneMaxY);

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
    ydebug("deserialize: size={}", size);
    if (size < 8) return Err("deserialize: too small");

    size_t pos = 0;
    auto readU32 = [&]() -> uint32_t {
        if (pos + 4 > size) return 0;
        uint32_t v;
        std::memcpy(&v, data + pos, 4);
        pos += 4;
        return v;
    };
    auto readF32 = [&]() -> float {
        if (pos + 4 > size) return 0.0f;
        float v;
        std::memcpy(&v, data + pos, 4);
        pos += 4;
        return v;
    };
    auto readU8 = [&]() -> uint8_t {
        if (pos >= size) return 0;
        return data[pos++];
    };

    // Magic & version
    uint32_t magic = readU32();
    uint32_t version = readU32();
    ydebug("deserialize: magic=0x{:08X} (expect 0x{:08X}), version={}", magic, SERIALIZE_MAGIC, version);
    if (magic != SERIALIZE_MAGIC) {
        yerror("deserialize: invalid magic 0x{:08X}, first 8 bytes: {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}",
               magic, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        return Err("deserialize: invalid magic");
    }
    if (version != SERIALIZE_VERSION) {
        return Err("deserialize: unsupported version");
    }

    clear();

    // Primitives
    uint32_t primCount = readU32();
    uint32_t totalWords = readU32();
    ydebug("deserialize: primCount={}, totalWords={}", primCount, totalWords);
    if (pos + totalWords * sizeof(float) > size) {
        return Err("deserialize: prim data truncated");
    }
    const float* primBuf = reinterpret_cast<const float*>(data + pos);
    pos += totalWords * sizeof(float);

    uint32_t wordPos = 0;
    for (uint32_t i = 0; i < primCount && wordPos < totalWords; i++) {
        uint32_t type = sdf::detail::read_u32(primBuf + wordPos, 0);
        uint32_t wc = sdf::wordCountForType(type);
        if (wc == 0) break;
        if (wordPos + wc > totalWords) break;
        addPrim(AUTO_ID, primBuf + wordPos, wc);
        wordPos += wc;
    }
    ydebug("deserialize: loaded {} prims", _prims.size());

    // Fonts
    uint32_t fontCount = readU32();
    ydebug("deserialize: fontCount={}", fontCount);
    for (uint32_t i = 0; i < fontCount; i++) {
        int fontId = static_cast<int>(readU32());
        uint32_t nameLen = readU32();
        if (pos + nameLen > size) return Err("deserialize: font name truncated");
        std::string name(reinterpret_cast<const char*>(data + pos), nameLen);
        pos += nameLen;

        uint32_t dataLen = readU32();
        ydebug("deserialize: font {} name='{}' dataLen={}", fontId, name, dataLen);
        if (pos + dataLen > size) return Err("deserialize: font data truncated");
        std::vector<uint8_t> ttfData(data + pos, data + pos + dataLen);
        pos += dataLen;

        FontBlob fb;
        fb.fontId = fontId;
        fb.name = std::move(name);
        fb.ttfData = std::move(ttfData);
        _fonts.push_back(std::move(fb));
        if (fontId >= _nextFontId) _nextFontId = fontId + 1;
    }

    // Text spans
    uint32_t spanCount = readU32();
    ydebug("deserialize: spanCount={}", spanCount);
    for (uint32_t i = 0; i < spanCount; i++) {
        TextSpanData s;
        s.x = readF32();
        s.y = readF32();
        s.fontSize = readF32();
        s.color = readU32();
        s.layer = readU32();
        s.fontId = static_cast<int>(readU32());
        s.rotation = readF32();
        uint32_t textLen = readU32();
        if (pos + textLen > size) return Err("deserialize: text truncated");
        s.text = std::string(reinterpret_cast<const char*>(data + pos), textLen);
        pos += textLen;
        _textSpans.push_back(std::move(s));
    }

    // Scene metadata
    _bgColor = readU32();
    _flags = readU32();
    _hasSceneBounds = (readU8() != 0);
    _sceneMinX = readF32();
    _sceneMinY = readF32();
    _sceneMaxX = readF32();
    _sceneMaxY = readF32();

    ydebug("deserialize: done - {} prims, {} fonts, {} spans, bounds=[{},{},{},{}]",
           _prims.size(), _fonts.size(), _textSpans.size(),
           _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);

    return Ok();
}

} // namespace yetty
