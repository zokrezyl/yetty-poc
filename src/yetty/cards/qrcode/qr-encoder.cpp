#include "qr-encoder.h"
#include <algorithm>
#include <cstring>

namespace yetty::card::qr {

//=============================================================================
// Version tables (simplified - versions 1-10 for now)
//=============================================================================

static const VersionInfo VERSION_INFO[] = {
    // v1: 21x21, 26 total codewords
    {1, 21, 26, {19, 16, 13, 9}, {7, 10, 13, 17}, {1, 1, 1, 1}},
    // v2: 25x25, 44 total codewords
    {2, 25, 44, {34, 28, 22, 16}, {10, 16, 22, 28}, {1, 1, 1, 1}},
    // v3: 29x29, 70 total codewords
    {3, 29, 70, {55, 44, 34, 26}, {15, 26, 18, 22}, {1, 1, 2, 2}},
    // v4: 33x33, 100 total codewords
    {4, 33, 100, {80, 64, 48, 36}, {20, 18, 26, 16}, {1, 2, 2, 4}},
    // v5: 37x37, 134 total codewords
    {5, 37, 134, {108, 86, 62, 46}, {26, 24, 18, 22}, {1, 2, 4, 4}},
    // v6: 41x41, 172 total codewords
    {6, 41, 172, {136, 108, 76, 60}, {18, 16, 24, 28}, {2, 4, 4, 4}},
    // v7: 45x45, 196 total codewords
    {7, 45, 196, {156, 124, 88, 66}, {20, 18, 18, 26}, {2, 4, 6, 5}},
    // v8: 49x49, 242 total codewords
    {8, 49, 242, {194, 154, 110, 86}, {24, 22, 22, 26}, {2, 4, 6, 6}},
    // v9: 53x53, 292 total codewords
    {9, 53, 292, {232, 182, 132, 100}, {30, 22, 20, 24}, {2, 5, 8, 8}},
    // v10: 57x57, 346 total codewords
    {10, 57, 346, {274, 216, 154, 122}, {18, 26, 24, 28}, {4, 5, 8, 11}},
};

const VersionInfo& getVersionInfo(uint8_t version) {
    if (version < 1 || version > 10) version = 1;
    return VERSION_INFO[version - 1];
}

uint8_t findMinVersion(size_t dataBytes, ECLevel ecl) {
    for (uint8_t v = 1; v <= 10; ++v) {
        const auto& info = getVersionInfo(v);
        if (info.dataCodewords[static_cast<int>(ecl)] >= dataBytes) {
            return v;
        }
    }
    return 10;  // Max supported
}

// Alignment pattern positions for versions 2-10
static const uint8_t ALIGNMENT_POSITIONS[][7] = {
    {},                       // v1 (none)
    {6, 18},                  // v2
    {6, 22},                  // v3
    {6, 26},                  // v4
    {6, 30},                  // v5
    {6, 34},                  // v6
    {6, 22, 38},              // v7
    {6, 24, 42},              // v8
    {6, 26, 46},              // v9
    {6, 28, 52},              // v10
};

//=============================================================================
// Reed-Solomon implementation
//=============================================================================

std::array<uint8_t, 256> ReedSolomon::_expTable;
std::array<uint8_t, 256> ReedSolomon::_logTable;
bool ReedSolomon::_tablesInitialized = false;

void ReedSolomon::initTables() {
    if (_tablesInitialized) return;
    
    // Generate exp and log tables for GF(2^8) with primitive polynomial 0x11D
    uint16_t x = 1;
    for (int i = 0; i < 256; ++i) {
        _expTable[i] = static_cast<uint8_t>(x);
        _logTable[x] = static_cast<uint8_t>(i);
        x <<= 1;
        if (x & 0x100) x ^= 0x11D;
    }
    _tablesInitialized = true;
}

uint8_t ReedSolomon::gfMul(uint8_t a, uint8_t b) {
    if (a == 0 || b == 0) return 0;
    return _expTable[(_logTable[a] + _logTable[b]) % 255];
}

uint8_t ReedSolomon::gfPow(uint8_t base, uint8_t exp) {
    return _expTable[(_logTable[base] * exp) % 255];
}

ReedSolomon::ReedSolomon(uint8_t numEccCodewords) {
    initTables();
    
    // Build generator polynomial
    _generator.resize(numEccCodewords + 1);
    _generator[0] = 1;
    
    for (int i = 0; i < numEccCodewords; ++i) {
        // Multiply by (x - alpha^i)
        for (int j = numEccCodewords; j > 0; --j) {
            _generator[j] = gfMul(_generator[j], _expTable[i]) ^ _generator[j - 1];
        }
        _generator[0] = gfMul(_generator[0], _expTable[i]);
    }
}

std::vector<uint8_t> ReedSolomon::encode(const std::vector<uint8_t>& data) {
    size_t numEcc = _generator.size() - 1;
    std::vector<uint8_t> remainder(numEcc, 0);
    
    for (uint8_t byte : data) {
        uint8_t factor = byte ^ remainder[0];
        // Shift remainder left
        for (size_t i = 0; i < numEcc - 1; ++i) {
            remainder[i] = remainder[i + 1] ^ gfMul(_generator[numEcc - 1 - i], factor);
        }
        remainder[numEcc - 1] = gfMul(_generator[0], factor);
    }
    
    return remainder;
}

//=============================================================================
// QR Encoder implementation
//=============================================================================

std::vector<uint8_t> QREncoder::encodeData(const std::string& data) {
    // Use byte mode (0100) for simplicity
    const auto& info = getVersionInfo(_version);
    size_t dataCapacity = info.dataCodewords[static_cast<int>(_ecl)];
    
    std::vector<uint8_t> result;
    result.reserve(dataCapacity);
    
    // Mode indicator: 0100 (byte mode)
    // Character count: 8 bits for v1-9, 16 bits for v10+
    uint8_t countBits = (_version <= 9) ? 8 : 16;
    
    // Build bit stream
    std::vector<bool> bits;
    
    // Mode indicator (4 bits)
    bits.push_back(false);
    bits.push_back(true);
    bits.push_back(false);
    bits.push_back(false);
    
    // Character count
    size_t len = data.size();
    for (int i = countBits - 1; i >= 0; --i) {
        bits.push_back((len >> i) & 1);
    }
    
    // Data bytes
    for (char c : data) {
        for (int i = 7; i >= 0; --i) {
            bits.push_back((static_cast<uint8_t>(c) >> i) & 1);
        }
    }
    
    // Terminator (up to 4 bits)
    for (int i = 0; i < 4 && bits.size() < dataCapacity * 8; ++i) {
        bits.push_back(false);
    }
    
    // Pad to byte boundary
    while (bits.size() % 8 != 0 && bits.size() < dataCapacity * 8) {
        bits.push_back(false);
    }
    
    // Convert bits to bytes
    for (size_t i = 0; i < bits.size(); i += 8) {
        uint8_t byte = 0;
        for (int j = 0; j < 8 && i + j < bits.size(); ++j) {
            byte = (byte << 1) | (bits[i + j] ? 1 : 0);
        }
        result.push_back(byte);
    }
    
    // Pad with alternating 0xEC, 0x11
    bool padToggle = true;
    while (result.size() < dataCapacity) {
        result.push_back(padToggle ? 0xEC : 0x11);
        padToggle = !padToggle;
    }
    
    return result;
}

std::vector<uint8_t> QREncoder::generateECC(const std::vector<uint8_t>& data) {
    const auto& info = getVersionInfo(_version);
    uint8_t eccPerBlock = info.ecCodewordsPerBlock[static_cast<int>(_ecl)];
    uint8_t numBlocks = info.numBlocks[static_cast<int>(_ecl)];
    
    ReedSolomon rs(eccPerBlock);
    
    // Simple case: single block
    if (numBlocks == 1) {
        auto ecc = rs.encode(data);
        std::vector<uint8_t> result = data;
        result.insert(result.end(), ecc.begin(), ecc.end());
        return result;
    }
    
    // Multi-block: interleave data and ECC
    size_t dataPerBlock = data.size() / numBlocks;
    size_t extraData = data.size() % numBlocks;
    
    std::vector<std::vector<uint8_t>> dataBlocks(numBlocks);
    std::vector<std::vector<uint8_t>> eccBlocks(numBlocks);
    
    size_t offset = 0;
    for (int b = 0; b < numBlocks; ++b) {
        size_t blockSize = dataPerBlock + (b >= numBlocks - extraData ? 1 : 0);
        dataBlocks[b].assign(data.begin() + offset, data.begin() + offset + blockSize);
        eccBlocks[b] = rs.encode(dataBlocks[b]);
        offset += blockSize;
    }
    
    // Interleave
    std::vector<uint8_t> result;
    size_t maxDataLen = dataPerBlock + (extraData > 0 ? 1 : 0);
    for (size_t i = 0; i < maxDataLen; ++i) {
        for (int b = 0; b < numBlocks; ++b) {
            if (i < dataBlocks[b].size()) {
                result.push_back(dataBlocks[b][i]);
            }
        }
    }
    for (size_t i = 0; i < eccPerBlock; ++i) {
        for (int b = 0; b < numBlocks; ++b) {
            result.push_back(eccBlocks[b][i]);
        }
    }
    
    return result;
}

void QREncoder::addFinderPatterns(std::vector<bool>& matrix) {
    uint8_t sz = size();
    
    auto drawFinder = [&](int cx, int cy) {
        for (int dy = -4; dy <= 4; ++dy) {
            for (int dx = -4; dx <= 4; ++dx) {
                int x = cx + dx;
                int y = cy + dy;
                if (x < 0 || x >= sz || y < 0 || y >= sz) continue;
                
                int dist = std::max(std::abs(dx), std::abs(dy));
                bool black = (dist <= 3 && dist != 2);
                matrix[y * sz + x] = black;
            }
        }
    };
    
    drawFinder(3, 3);           // Top-left
    drawFinder(sz - 4, 3);      // Top-right
    drawFinder(3, sz - 4);      // Bottom-left
}

void QREncoder::addTimingPatterns(std::vector<bool>& matrix) {
    uint8_t sz = size();
    
    // Horizontal timing (row 6)
    for (int x = 8; x < sz - 8; ++x) {
        matrix[6 * sz + x] = (x % 2 == 0);
    }
    
    // Vertical timing (column 6)
    for (int y = 8; y < sz - 8; ++y) {
        matrix[y * sz + 6] = (y % 2 == 0);
    }
}

void QREncoder::addAlignmentPatterns(std::vector<bool>& matrix) {
    if (_version == 1) return;
    
    const uint8_t* positions = ALIGNMENT_POSITIONS[_version - 1];
    int numPos = (_version <= 6) ? 2 : 3;
    
    for (int i = 0; i < numPos; ++i) {
        for (int j = 0; j < numPos; ++j) {
            int cx = positions[i];
            int cy = positions[j];
            
            // Skip if overlaps with finder patterns
            if ((cx <= 8 && cy <= 8) ||
                (cx <= 8 && cy >= size() - 9) ||
                (cx >= size() - 9 && cy <= 8)) {
                continue;
            }
            
            // Draw 5x5 alignment pattern
            for (int dy = -2; dy <= 2; ++dy) {
                for (int dx = -2; dx <= 2; ++dx) {
                    int dist = std::max(std::abs(dx), std::abs(dy));
                    bool black = (dist != 1);
                    matrix[(cy + dy) * size() + (cx + dx)] = black;
                }
            }
        }
    }
}

// Format info bits for each EC level and mask pattern
static const uint16_t FORMAT_INFO[4][8] = {
    // L (00)
    {0x77C4, 0x72F3, 0x7DAA, 0x789D, 0x662F, 0x6318, 0x6C41, 0x6976},
    // M (01)
    {0x5412, 0x5125, 0x5E7C, 0x5B4B, 0x45F9, 0x40CE, 0x4F97, 0x4AA0},
    // Q (10)
    {0x355F, 0x3068, 0x3F31, 0x3A06, 0x24B4, 0x2183, 0x2EDA, 0x2BED},
    // H (11)
    {0x1689, 0x13BE, 0x1CE7, 0x19D0, 0x0762, 0x0255, 0x0D0C, 0x083B},
};

void QREncoder::addFormatInfo(std::vector<bool>& matrix, uint8_t maskPattern) {
    uint16_t info = FORMAT_INFO[static_cast<int>(_ecl)][maskPattern];
    uint8_t sz = size();
    
    // Place format info around top-left finder
    // Horizontal: right of top-left finder
    for (int i = 0; i < 6; ++i) {
        matrix[8 * sz + i] = (info >> (14 - i)) & 1;
    }
    matrix[8 * sz + 7] = (info >> 8) & 1;
    matrix[8 * sz + 8] = (info >> 7) & 1;
    
    // Vertical: below top-left finder
    matrix[7 * sz + 8] = (info >> 6) & 1;
    for (int i = 0; i < 6; ++i) {
        matrix[(5 - i) * sz + 8] = (info >> (5 - i)) & 1;
    }
    
    // Horizontal: left of top-right finder
    for (int i = 0; i < 8; ++i) {
        matrix[8 * sz + (sz - 8 + i)] = (info >> (7 - i)) & 1;
    }
    
    // Vertical: above bottom-left finder
    for (int i = 0; i < 7; ++i) {
        matrix[(sz - 7 + i) * sz + 8] = (info >> (14 - i)) & 1;
    }
    
    // Dark module (always present)
    matrix[(sz - 8) * sz + 8] = true;
}

void QREncoder::addVersionInfo(std::vector<bool>& matrix) {
    if (_version < 7) return;
    // Version info for v7+ (not implemented yet)
}

// Check if module is reserved (finder, timing, format, etc.)
static bool isReserved(int x, int y, uint8_t sz, uint8_t version) {
    // Finder patterns + separators
    if (x <= 8 && y <= 8) return true;          // Top-left
    if (x >= sz - 8 && y <= 8) return true;     // Top-right
    if (x <= 8 && y >= sz - 8) return true;     // Bottom-left
    
    // Timing patterns
    if (x == 6 || y == 6) return true;
    
    // Dark module
    if (x == 8 && y == sz - 8) return true;
    
    // Version info (v7+)
    if (version >= 7) {
        if (x < 6 && y >= sz - 11) return true;
        if (y < 6 && x >= sz - 11) return true;
    }
    
    return false;
}

void QREncoder::placeModules(std::vector<bool>& matrix, const std::vector<uint8_t>& codewords) {
    uint8_t sz = size();
    
    // Convert codewords to bits
    std::vector<bool> bits;
    for (uint8_t cw : codewords) {
        for (int i = 7; i >= 0; --i) {
            bits.push_back((cw >> i) & 1);
        }
    }
    
    // Place bits in zigzag pattern
    size_t bitIdx = 0;
    int x = sz - 1;
    bool upward = true;
    
    while (x >= 0 && bitIdx < bits.size()) {
        // Skip vertical timing pattern
        if (x == 6) {
            --x;
            continue;
        }
        
        for (int y = upward ? sz - 1 : 0;
             upward ? y >= 0 : y < sz;
             upward ? --y : ++y) {
            
            for (int dx = 0; dx <= 1 && bitIdx < bits.size(); ++dx) {
                int col = x - dx;
                if (col < 0) continue;
                
                if (!isReserved(col, y, sz, _version)) {
                    // Check alignment patterns
                    bool isAlign = false;
                    if (_version > 1) {
                        const uint8_t* pos = ALIGNMENT_POSITIONS[_version - 1];
                        int numPos = (_version <= 6) ? 2 : 3;
                        for (int i = 0; i < numPos && !isAlign; ++i) {
                            for (int j = 0; j < numPos && !isAlign; ++j) {
                                int cx = pos[i], cy = pos[j];
                                if ((cx <= 8 && cy <= 8) ||
                                    (cx <= 8 && cy >= sz - 9) ||
                                    (cx >= sz - 9 && cy <= 8)) continue;
                                if (std::abs(col - cx) <= 2 && std::abs(y - cy) <= 2) {
                                    isAlign = true;
                                }
                            }
                        }
                    }
                    
                    if (!isAlign) {
                        matrix[y * sz + col] = bits[bitIdx++];
                    }
                }
            }
        }
        
        x -= 2;
        upward = !upward;
    }
}

// Mask patterns
static bool getMaskBit(int x, int y, uint8_t pattern) {
    switch (pattern) {
        case 0: return (y + x) % 2 == 0;
        case 1: return y % 2 == 0;
        case 2: return x % 3 == 0;
        case 3: return (y + x) % 3 == 0;
        case 4: return (y / 2 + x / 3) % 2 == 0;
        case 5: return ((y * x) % 2 + (y * x) % 3) == 0;
        case 6: return ((y * x) % 2 + (y * x) % 3) % 2 == 0;
        case 7: return ((y + x) % 2 + (y * x) % 3) % 2 == 0;
        default: return false;
    }
}

void QREncoder::applyMask(std::vector<bool>& matrix, uint8_t maskPattern) {
    uint8_t sz = size();
    
    for (int y = 0; y < sz; ++y) {
        for (int x = 0; x < sz; ++x) {
            if (isReserved(x, y, sz, _version)) continue;
            
            // Check alignment patterns
            bool isAlign = false;
            if (_version > 1) {
                const uint8_t* pos = ALIGNMENT_POSITIONS[_version - 1];
                int numPos = (_version <= 6) ? 2 : 3;
                for (int i = 0; i < numPos && !isAlign; ++i) {
                    for (int j = 0; j < numPos && !isAlign; ++j) {
                        int cx = pos[i], cy = pos[j];
                        if ((cx <= 8 && cy <= 8) ||
                            (cx <= 8 && cy >= sz - 9) ||
                            (cx >= sz - 9 && cy <= 8)) continue;
                        if (std::abs(x - cx) <= 2 && std::abs(y - cy) <= 2) {
                            isAlign = true;
                        }
                    }
                }
            }
            
            if (!isAlign && getMaskBit(x, y, maskPattern)) {
                matrix[y * sz + x] = !matrix[y * sz + x];
            }
        }
    }
}

int QREncoder::evaluateMask(const std::vector<bool>& matrix) {
    uint8_t sz = size();
    int penalty = 0;
    
    // Rule 1: Consecutive same-color modules (5+ in row/col)
    for (int y = 0; y < sz; ++y) {
        int runLen = 1;
        bool runColor = matrix[y * sz];
        for (int x = 1; x < sz; ++x) {
            if (matrix[y * sz + x] == runColor) {
                ++runLen;
            } else {
                if (runLen >= 5) penalty += 3 + (runLen - 5);
                runLen = 1;
                runColor = matrix[y * sz + x];
            }
        }
        if (runLen >= 5) penalty += 3 + (runLen - 5);
    }
    
    for (int x = 0; x < sz; ++x) {
        int runLen = 1;
        bool runColor = matrix[x];
        for (int y = 1; y < sz; ++y) {
            if (matrix[y * sz + x] == runColor) {
                ++runLen;
            } else {
                if (runLen >= 5) penalty += 3 + (runLen - 5);
                runLen = 1;
                runColor = matrix[y * sz + x];
            }
        }
        if (runLen >= 5) penalty += 3 + (runLen - 5);
    }
    
    // Rule 2: 2x2 blocks of same color
    for (int y = 0; y < sz - 1; ++y) {
        for (int x = 0; x < sz - 1; ++x) {
            bool c = matrix[y * sz + x];
            if (c == matrix[y * sz + x + 1] &&
                c == matrix[(y + 1) * sz + x] &&
                c == matrix[(y + 1) * sz + x + 1]) {
                penalty += 3;
            }
        }
    }
    
    return penalty;
}

uint8_t QREncoder::selectBestMask(std::vector<bool>& matrix) {
    int bestPenalty = INT32_MAX;
    uint8_t bestMask = 0;
    
    std::vector<bool> testMatrix = matrix;
    
    for (uint8_t m = 0; m < 8; ++m) {
        testMatrix = matrix;
        applyMask(testMatrix, m);
        addFormatInfo(testMatrix, m);
        
        int penalty = evaluateMask(testMatrix);
        if (penalty < bestPenalty) {
            bestPenalty = penalty;
            bestMask = m;
        }
    }
    
    return bestMask;
}

std::vector<bool> QREncoder::encode(const std::string& data) {
    if (data.empty()) return {};
    
    // Find minimum version
    size_t dataBytes = data.size() + 3;  // mode + count + data (rough estimate)
    _version = findMinVersion(dataBytes, _ecl);
    
    // Encode data
    auto dataCodewords = encodeData(data);
    
    // Generate error correction
    auto allCodewords = generateECC(dataCodewords);
    
    // Create matrix
    uint8_t sz = size();
    std::vector<bool> matrix(sz * sz, false);
    
    // Add function patterns
    addFinderPatterns(matrix);
    addTimingPatterns(matrix);
    addAlignmentPatterns(matrix);
    
    // Place data
    placeModules(matrix, allCodewords);
    
    // Select and apply best mask
    uint8_t mask = selectBestMask(matrix);
    applyMask(matrix, mask);
    addFormatInfo(matrix, mask);
    addVersionInfo(matrix);
    
    return matrix;
}

} // namespace yetty::card::qr
