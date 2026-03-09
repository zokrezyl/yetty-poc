// Standalone YDraw buffer for OSC serialization
// Matches yetty's YDrawBuffer serialization format

#include "ydraw-buffer-standalone.h"
#include <cstring>
#include <iostream>

namespace ydraw {

namespace {

// Primitive type IDs matching yetty's ydraw-prim-writer.gen.h
constexpr uint32_t TYPE_CIRCLE = 0;
constexpr uint32_t TYPE_BOX = 1;
constexpr uint32_t TYPE_SEGMENT = 2;

void writeU32(float* buf, uint32_t off, uint32_t val) {
    std::memcpy(&buf[off], &val, sizeof(uint32_t));
}

} // namespace

void Buffer::addBox(float x, float y, float w, float h, uint32_t color) {
    Primitive prim;
    float* buf = prim.data;
    // Box: type(0), layer(1), cx(2), cy(3), hw(4), hh(5), fillColor(6), strokeColor(7), strokeWidth(8), round(9)
    writeU32(buf, 0, TYPE_BOX);
    writeU32(buf, 1, 0); // layer
    buf[2] = x + w / 2;  // cx
    buf[3] = y + h / 2;  // cy
    buf[4] = w / 2;      // half width
    buf[5] = h / 2;      // half height
    writeU32(buf, 6, color);
    writeU32(buf, 7, 0); // no stroke
    buf[8] = 0;          // stroke width
    buf[9] = 0;          // round
    // Mark end with word count in data[10]
    writeU32(buf, 10, 10);
    _primitives.push_back(prim);
}

void Buffer::addRoundedBox(float x, float y, float w, float h, float radius, uint32_t color) {
    Primitive prim;
    float* buf = prim.data;
    writeU32(buf, 0, TYPE_BOX);
    writeU32(buf, 1, 0); // layer
    buf[2] = x + w / 2;  // cx
    buf[3] = y + h / 2;  // cy
    buf[4] = w / 2;      // half width
    buf[5] = h / 2;      // half height
    writeU32(buf, 6, color);
    writeU32(buf, 7, 0); // no stroke
    buf[8] = 0;          // stroke width
    buf[9] = radius;     // round
    writeU32(buf, 10, 10);
    _primitives.push_back(prim);
}

void Buffer::addCircle(float cx, float cy, float radius, uint32_t color) {
    Primitive prim;
    float* buf = prim.data;
    // Circle: type(0), layer(1), cx(2), cy(3), r(4), fillColor(5), strokeColor(6), strokeWidth(7), round(8), unused(9)
    writeU32(buf, 0, TYPE_CIRCLE);
    writeU32(buf, 1, 0); // layer
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = radius;
    writeU32(buf, 5, color);
    writeU32(buf, 6, 0); // no stroke
    buf[7] = 0;          // stroke width
    buf[8] = 0;          // round
    buf[9] = 0;          // unused
    writeU32(buf, 10, 10); // word count at consistent position
    _primitives.push_back(prim);
}

void Buffer::addSegment(float x1, float y1, float x2, float y2, float thickness, uint32_t color) {
    Primitive prim;
    float* buf = prim.data;
    // Segment: type(0), layer(1), x0(2), y0(3), x1(4), y1(5), fillColor(6), strokeColor(7), strokeWidth(8), round(9)
    writeU32(buf, 0, TYPE_SEGMENT);
    writeU32(buf, 1, 0); // layer
    buf[2] = x1;
    buf[3] = y1;
    buf[4] = x2;
    buf[5] = y2;
    writeU32(buf, 6, color);
    writeU32(buf, 7, 0);
    buf[8] = thickness;
    buf[9] = 0;
    writeU32(buf, 10, 10);
    _primitives.push_back(prim);
}

void Buffer::addText(const std::string& text, float x, float y, float fontSize, uint32_t color) {
    _texts.push_back({text, x, y, fontSize, color});
}

std::vector<uint8_t> Buffer::serialize() const {
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

    // Magic & version (matching yetty)
    writeU32(0x59445246); // "YDRF"
    writeU32(1);          // version

    // Build compact primitive data
    std::vector<float> compact;
    for (const auto& prim : _primitives) {
        uint32_t wordCount;
        std::memcpy(&wordCount, &prim.data[10], sizeof(uint32_t));
        for (uint32_t i = 0; i < wordCount; ++i) {
            compact.push_back(prim.data[i]);
        }
    }

    // Primitives
    writeU32(static_cast<uint32_t>(_primitives.size()));
    writeU32(static_cast<uint32_t>(compact.size()));
    if (!compact.empty()) {
        writeBytes(reinterpret_cast<const uint8_t*>(compact.data()),
                   compact.size() * sizeof(float));
    }

    // Fonts (none for now)
    writeU32(0);

    // Text spans
    writeU32(static_cast<uint32_t>(_texts.size()));
    for (const auto& t : _texts) {
        writeF32(t.x);
        writeF32(t.y);
        writeF32(t.fontSize);
        writeU32(t.color);
        writeU32(0);  // layer
        writeU32(0);  // fontId
        writeF32(0);  // rotation
        writeU32(static_cast<uint32_t>(t.text.size()));
        writeBytes(reinterpret_cast<const uint8_t*>(t.text.data()), t.text.size());
    }

    // Scene metadata
    writeU32(0xFFFFFFFF); // bgColor (white)
    writeU32(0);          // flags
    writeU8(1);           // hasSceneBounds
    writeF32(0);          // minX
    writeF32(0);          // minY
    writeF32(_width);     // maxX
    writeF32(_height);    // maxY

    return out;
}

// Base64 encoding for OSC transfer
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64Encode(const std::vector<uint8_t>& data) {
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i < data.size()) {
        uint32_t octet_a = i < data.size() ? data[i++] : 0;
        uint32_t octet_b = i < data.size() ? data[i++] : 0;
        uint32_t octet_c = i < data.size() ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        result += base64_chars[(triple >> 18) & 0x3F];
        result += base64_chars[(triple >> 12) & 0x3F];
        result += base64_chars[(triple >> 6) & 0x3F];
        result += base64_chars[triple & 0x3F];
    }

    // Padding
    size_t mod = data.size() % 3;
    if (mod == 1) {
        result[result.size() - 2] = '=';
        result[result.size() - 1] = '=';
    } else if (mod == 2) {
        result[result.size() - 1] = '=';
    }

    return result;
}

void outputOSC(const Buffer& buffer, int cardId, int cellWidth, int cellHeight) {
    // Serialize buffer
    auto data = buffer.serialize();

    // Base64 encode
    std::string encoded = base64Encode(data);

    // Output OSC 666666 sequence
    // Format: ESC ] 666666 ; run -c ydraw -x X -y Y -w W -h H ; card-args ; BASE64_PAYLOAD ESC \
    // -r flag for relative positioning
    std::cout << "\033]666666;run -c ydraw -x 0 -y 0 -w " << cellWidth
              << " -h " << cellHeight << " -r;;" << encoded << "\033\\";
    std::cout.flush();
}

} // namespace ydraw
