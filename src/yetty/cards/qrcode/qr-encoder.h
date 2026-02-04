#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <array>

namespace yetty::card::qr {

// Error correction levels
enum class ECLevel : uint8_t { L = 0, M = 1, Q = 2, H = 3 };

// QR Code version info
struct VersionInfo {
    uint8_t version;        // 1-40
    uint8_t size;           // modules per side (21 + 4*(version-1))
    uint16_t totalCodewords;
    uint16_t dataCodewords[4];  // Per EC level
    uint8_t ecCodewordsPerBlock[4];
    uint8_t numBlocks[4];
};

// Get version info for a given version number
const VersionInfo& getVersionInfo(uint8_t version);

// Find minimum version that can encode data with given EC level
uint8_t findMinVersion(size_t dataBytes, ECLevel ecl);

// Encode data to QR code bit matrix
// Returns module matrix (true = black, false = white)
// Size is (21 + 4*(version-1))^2
class QREncoder {
public:
    QREncoder(ECLevel ecl = ECLevel::M) : _ecl(ecl) {}
    
    // Encode string data, returns bit matrix (row-major, true=black)
    // Returns empty vector on failure
    std::vector<bool> encode(const std::string& data);
    
    // Get the version used for last encode
    uint8_t version() const { return _version; }
    
    // Get matrix size (modules per side)
    uint8_t size() const { return 21 + 4 * (_version - 1); }

private:
    ECLevel _ecl;
    uint8_t _version = 0;
    
    // Encoding steps
    std::vector<uint8_t> encodeData(const std::string& data);
    std::vector<uint8_t> generateECC(const std::vector<uint8_t>& data);
    void placeModules(std::vector<bool>& matrix, const std::vector<uint8_t>& codewords);
    void applyMask(std::vector<bool>& matrix, uint8_t maskPattern);
    void addFinderPatterns(std::vector<bool>& matrix);
    void addTimingPatterns(std::vector<bool>& matrix);
    void addAlignmentPatterns(std::vector<bool>& matrix);
    void addFormatInfo(std::vector<bool>& matrix, uint8_t maskPattern);
    void addVersionInfo(std::vector<bool>& matrix);
    uint8_t selectBestMask(std::vector<bool>& matrix);
    int evaluateMask(const std::vector<bool>& matrix);
};

// Reed-Solomon encoder for QR codes (GF(2^8) with polynomial 0x11D)
class ReedSolomon {
public:
    explicit ReedSolomon(uint8_t numEccCodewords);
    std::vector<uint8_t> encode(const std::vector<uint8_t>& data);
    
private:
    std::vector<uint8_t> _generator;
    
    static uint8_t gfMul(uint8_t a, uint8_t b);
    static uint8_t gfPow(uint8_t base, uint8_t exp);
    static void initTables();
    
    static std::array<uint8_t, 256> _expTable;
    static std::array<uint8_t, 256> _logTable;
    static bool _tablesInitialized;
};

} // namespace yetty::card::qr
