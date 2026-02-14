#include "qrcode.h"
#include "qr-encoder.h"
#include <yetty/yetty-context.h>
#include <ytrace/ytrace.hpp>

#include <yetty/wgpu-compat.h>
#include <cstring>
#include <sstream>

namespace yetty::card {

//=============================================================================
// QRCodeImpl - Implementation of QRCode card
//=============================================================================

class QRCodeImpl : public QRCode {
public:
    QRCodeImpl(const YettyContext& ctx,
               int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells,
               const std::string& args, const std::string& payload)
        : QRCode(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~QRCodeImpl() override {
        dispose();
    }

    const char* typeName() const override { return "qrcode"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot (32 bytes)
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("QRCode::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;
        
        yinfo("QRCode::init: allocated metadata at offset {}", _metaHandle.offset);
        
        // Parse args
        parseArgs(_argsStr);
        
        // Encode QR code
        if (!_payloadStr.empty()) {
            if (auto res = encodeQR(_payloadStr); !res) {
                return Err<void>("QRCode::init: failed to encode QR", res);
            }
        }
        
        _metadataDirty = true;
        return Ok();
    }

    Result<void> dispose() override {
        _bufferHandle = BufferHandle::invalid();

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }
        
        _qrModules.clear();
        return Ok();
    }

    void suspend() override {
        _bufferHandle = BufferHandle::invalid();
    }

    void declareBufferNeeds() override {
        if (!_qrModules.empty()) {
            size_t numWords = (_qrModules.size() + 31) / 32;
            _cardMgr->bufferManager()->reserve(numWords * sizeof(uint32_t));
        }
    }

    Result<void> allocateBuffers() override {
        if (_qrModules.empty()) {
            return Ok();
        }

        // Pack QR modules as u32 (32 modules per word)
        size_t numWords = (_qrModules.size() + 31) / 32;
        std::vector<uint32_t> packedData(numWords, 0);

        for (size_t i = 0; i < _qrModules.size(); ++i) {
            if (_qrModules[i]) {
                packedData[i / 32] |= (1u << (i % 32));
            }
        }

        // Allocate buffer (commitReservations cleared all handles, must re-allocate)
        size_t bufferSize = numWords * sizeof(uint32_t);
        auto bufResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "data", bufferSize);
        if (!bufResult) {
            return Err<void>("QRCode::allocateBuffers: failed to allocate buffer");
        }
        _bufferHandle = *bufResult;

        // Write packed data directly to buffer
        std::memcpy(_bufferHandle.data, packedData.data(), bufferSize);
        _cardMgr->bufferManager()->markBufferDirty(_bufferHandle);

        _metadataDirty = true;

        yinfo("QRCode::allocateBuffers: allocated {} bytes at offset {}",
              bufferSize, _bufferHandle.offset);

        return Ok();
    }

    void setCellSize(float cellWidth, float cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _metadataDirty = true;
        }
    }

    Result<void> finalize() override {
        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("QRCode::render: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

private:
    // Metadata layout (64 bytes = 16 u32s)
    struct Metadata {
        uint32_t bufferOffset;   // Offset in cardStorage buffer
        uint32_t qrSize;         // Modules per side
        uint32_t fgColor;        // Foreground (black modules) RGBA
        uint32_t bgColor;        // Background (white modules) RGBA
        uint32_t widthCells;     // Card width in cells
        uint32_t heightCells;    // Card height in cells
        uint32_t cellWidth;      // Cell pixel width
        uint32_t cellHeight;     // Cell pixel height
        uint32_t reserved[8];    // Padding to 64 bytes
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;
    
    MetadataHandle _metaHandle;
    BufferHandle _bufferHandle;
    
    std::vector<bool> _qrModules;
    uint8_t _qrSize = 0;

    qr::ECLevel _ecLevel = qr::ECLevel::M;
    uint32_t _fgColor = 0x000000FF;  // Black, opaque
    uint32_t _bgColor = 0xFFFFFFFF;  // White, opaque

    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;

    bool _metadataDirty = true;

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        
        while (iss >> token) {
            size_t eq = token.find('=');
            if (eq == std::string::npos) continue;
            
            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);
            
            if (key == "ecl" || key == "ec") {
                if (val == "L" || val == "l") _ecLevel = qr::ECLevel::L;
                else if (val == "M" || val == "m") _ecLevel = qr::ECLevel::M;
                else if (val == "Q" || val == "q") _ecLevel = qr::ECLevel::Q;
                else if (val == "H" || val == "h") _ecLevel = qr::ECLevel::H;
            } else if (key == "fg" || key == "color") {
                _fgColor = parseColor(val);
            } else if (key == "bg") {
                _bgColor = parseColor(val);
            }
        }
    }
    
    uint32_t parseColor(const std::string& val) {
        if (val.empty()) return 0x000000FF;
        
        std::string hex = val;
        if (hex[0] == '#') hex = hex.substr(1);
        
        // Expand #RGB to #RRGGBB
        if (hex.size() == 3) {
            hex = std::string{hex[0], hex[0], hex[1], hex[1], hex[2], hex[2]};
        }
        
        if (hex.size() != 6) return 0x000000FF;
        
        uint32_t rgb = std::stoul(hex, nullptr, 16);
        return (rgb << 8) | 0xFF;  // RGBA with full alpha
    }

    Result<void> encodeQR(const std::string& data) {
        qr::QREncoder encoder(_ecLevel);
        _qrModules = encoder.encode(data);
        
        if (_qrModules.empty()) {
            return Err<void>("QRCode::encodeQR: encoding failed");
        }
        
        _qrSize = encoder.size();
        
        yinfo("QRCode::encodeQR: encoded '{}' as v{} {}x{} QR code",
              data.substr(0, 20), encoder.version(), _qrSize, _qrSize);
        
        return Ok();
    }

    Result<void> uploadMetadata() {
        Metadata meta{};
        meta.bufferOffset = _bufferHandle.isValid() ? _bufferHandle.offset : 0;
        meta.qrSize = _qrSize;
        meta.fgColor = _fgColor;
        meta.bgColor = _bgColor;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.cellWidth = _cellWidth;
        meta.cellHeight = _cellHeight;

        _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
        return Ok();
    }
};

//=============================================================================
// Factory
//=============================================================================

Result<QRCode::Ptr> QRCode::createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                       int32_t x, int32_t y, uint32_t widthCells,
                                       uint32_t heightCells, const std::string& args,
                                       const std::string& payload) noexcept {
    auto impl = std::make_shared<QRCodeImpl>(yettyCtx, x, y, widthCells, heightCells, args, payload);
    
    if (auto res = impl->init(); !res) {
        return Err<QRCode::Ptr>("QRCode::createImpl: init failed", res);
    }
    
    return Ok<QRCode::Ptr>(std::move(impl));
}

} // namespace yetty::card
