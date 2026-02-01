#include "gpu-screen.h"
#include <yetty/card-manager.h>
#include <yetty/card-factory.h>
#include <yetty/card.h>
#include <algorithm>
#include <cstdlib>
#include <unordered_set>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <string>
#include <yetty/osc-command.h>
#include <yetty/result.hpp>
#include <yetty/shader-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/font-manager.h>
#include <ytrace/ytrace.hpp>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

#if defined(__SSE2__)
#include <emmintrin.h>
#endif

#include <deque>
#include <unordered_map>
#include <vterm.h>

namespace yetty {

// CardPtr and Card come from <yetty/card.h> via card-factory.h

// =============================================================================
// Glyph constants
// =============================================================================
constexpr uint32_t GLYPH_WIDE_CONT = 0xFFFE; // Wide char continuation

// Font type encoding for cell attributes (bits 5-7 of style byte)
constexpr uint8_t FONT_TYPE_MSDF = 0;   // Default text rendering
constexpr uint8_t FONT_TYPE_BITMAP = 1; // Bitmap fonts (emoji, color fonts)
constexpr uint8_t FONT_TYPE_SHADER = 2; // Single-cell shader glyphs
constexpr uint8_t FONT_TYPE_CARD = 3;   // Multi-cell card glyphs

// Card glyph range - multi-cell widgets (U+100000 - U+100FFF)
constexpr uint32_t CARD_GLYPH_BASE = 0x100000;
constexpr uint32_t CARD_GLYPH_END = 0x100FFF;

// Shader glyph range - single-cell procedural glyphs (U+101000 - U+10FFFD)
constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;
constexpr uint32_t SHADER_GLYPH_END = 0x10FFFD;

// Helper functions for glyph type detection
inline bool isCardGlyph(uint32_t codepoint) {
  return codepoint >= CARD_GLYPH_BASE && codepoint <= CARD_GLYPH_END;
}

inline bool isShaderGlyph(uint32_t codepoint) {
  return codepoint >= SHADER_GLYPH_BASE && codepoint <= SHADER_GLYPH_END;
}

// Check if codepoint is an emoji (common ranges)
inline bool isEmoji(uint32_t cp) {
  if (cp >= 0x1F600 && cp <= 0x1F64F)
    return true; // Emoticons
  if (cp >= 0x1F300 && cp <= 0x1F5FF)
    return true; // Misc Symbols/Pictographs
  if (cp >= 0x1F680 && cp <= 0x1F6FF)
    return true; // Transport/Map
  if (cp >= 0x1F900 && cp <= 0x1F9FF)
    return true; // Supplemental Symbols
  if (cp >= 0x1FA00 && cp <= 0x1FAFF)
    return true; // Symbols Extended
  if (cp >= 0x2600 && cp <= 0x26FF)
    return true; // Misc Symbols
  if (cp >= 0x2700 && cp <= 0x27BF)
    return true; // Dingbats
  if (cp >= 0x1F1E0 && cp <= 0x1F1FF)
    return true; // Regional Indicators
  return false;
}

// Scrollback line storage
struct ScrollbackLineGPU {
  std::vector<Cell> cells;
};


// Pen state for current text attributes
struct Pen {
  VTermColor fg;
  VTermColor bg;
  bool bold = false;
  bool italic = false;
  uint8_t underline = 0;
  bool strike = false;
  bool reverse = false;
  bool blink = false;
};

// OSC parsing is included via osc-command.h

class GPUScreenImpl : public GPUScreen {
public:
  explicit GPUScreenImpl(const YettyContext &ctx);
  ~GPUScreenImpl() override;

  Result<void> onShutdown() override;

  // Initialize fonts and cell metrics - called from factory after construction
  Result<void> init() noexcept;

  // GPUScreen interface
  void write(const char *data, size_t len) override;
  void resize(uint32_t cols, uint32_t rows) override;
  uint32_t getCols() const override;
  uint32_t getRows() const override;
  bool hasDamage() const override { return _hasDamage; }
  void clearDamage() override { _hasDamage = false; }
  void setOutputCallback(OutputCallback cb) override;
  Result<void> render(WGPURenderPassEncoder pass) override;
  void setViewport(float x, float y, float width, float height) override;
  uint32_t getCellWidth() const override { return static_cast<uint32_t>(_baseCellWidth * _zoomLevel); }
  uint32_t getCellHeight() const override { return static_cast<uint32_t>(_baseCellHeight * _zoomLevel); }

  // EventListener interface
  Result<bool> onEvent(const base::Event &event) override;

  // GPUScreen interface - test methods
  const Cell *getCellData() const override;
  Cell getCell(int row, int col) const override;
  int getCursorCol() const { return _cursorCol; }
  int getCursorRow() const { return _cursorRow; }
  bool isCursorVisible() const { return _cursorVisible; }
  void scrollUp(int lines);
  void scrollDown(int lines);
  void scrollToBottom();
  bool isScrolledBack() const { return _scrollOffset > 0; }
  void sendKey(uint32_t codepoint, int mods);
  void sendSpecialKey(int key, int mods);
  base::ObjectId id() const { return _id; }
  bool isFocused() const { return _focused; }
  void registerForFocus();

  // VTerm state callbacks (static)
  static int onPutglyph(VTermGlyphInfo *info, VTermPos pos, void *user);
  static int onMoveCursor(VTermPos pos, VTermPos oldpos, int visible,
                          void *user);
  static int onScrollRect(VTermRect rect, int downward, int rightward,
                          void *user);
  static int onMoveRect(VTermRect dest, VTermRect src, void *user);
  static int onErase(VTermRect rect, int selective, void *user);
  static int onInitPen(void *user);
  static int onSetPenAttr(VTermAttr attr, VTermValue *val, void *user);
  static int onSetTermProp(VTermProp prop, VTermValue *val, void *user);
  static int onBell(void *user);
  static int onResize(int rows, int cols, VTermStateFields *fields, void *user);
  static int onOSC(int command, VTermStringFragment frag, void *user);
  static int onSetLineInfo(int row, const VTermLineInfo *newinfo,
                           const VTermLineInfo *oldinfo, void *user);

private:
  Result<void> initPipeline();
  void createVTerm();
  void attach(VTerm *vt);
  void resizeInternal(int rows, int cols, VTermStateFields *fields);
  void resizeBuffer(int bufidx, int newRows, int newCols, bool active,
                    VTermStateFields *fields);
  int linePopcount(const std::vector<Cell> &buffer, int row, int cols) const;
  void clearBuffer(std::vector<Cell> &buffer);
  void switchToScreen(bool alt);
  void reset();
  void composeViewBuffer();
  void decompressLine(const ScrollbackLineGPU &line, int viewRow);
  void pushLineToScrollback(int row);
  void scrollToTop();

  void setCell(int row, int col, uint32_t glyph, uint8_t fgR, uint8_t fgG,
               uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB,
               uint8_t attrsByte);
  void clearCell(int row, int col);
  void colorToRGB(const VTermColor &color, uint8_t &r, uint8_t &g, uint8_t &b);

  size_t cellIndex(int row, int col) const {
    return static_cast<size_t>(row * _cols + col);
  }

  // Card dormancy — scan visible buffer for a given slotIndex
  bool isCardSlotVisibleInBuffer(uint32_t slotIndex, int excludeRow = -1) const;
  void suspendOffscreenCards(int scrolledRow);
  void reactivateVisibleCards();
  void gcInactiveCards();

  // OSC handling
  bool handleOSCSequence(const std::string &sequence, std::string *response,
                         uint32_t *linesToAdvance);
  bool handleCardOSCSequence(const std::string &sequence, std::string *response,
                             uint32_t *linesToAdvance);

  // Text selection
  void clearSelection();
  std::string extractSelectedText() const;

  // Card registry
  void registerCard(CardPtr card);
  void unregisterCard(uint32_t metadataOffset);
  Card *getCardBySlotIndex(uint32_t offset) const;
  Card *getCardAtCell(int row, int col) const;

  // Compute card-local pixel coords from viewport-local position and cell row/col.
  // Uses bg encoding to find card's actual top-left corner in the grid.
  void cardLocalCoords(float viewLocalX, float viewLocalY, int row, int col,
                       float &cardX, float &cardY) const {
    uint32_t cellW = getCellWidth();
    uint32_t cellH = getCellHeight();
    // bg encodes relative position within card: (relRow << 12) | relCol
    size_t idx = cellIndex(row, col);
    const Cell &cell = (*_visibleBuffer)[idx];
    uint32_t bg24 = static_cast<uint32_t>(cell.bgR) |
                    (static_cast<uint32_t>(cell.bgG) << 8) |
                    (static_cast<uint32_t>(cell.bgB) << 16);
    int relCol = bg24 & 0xFFF;
    int relRow = (bg24 >> 12) & 0xFFF;
    int cardTopCol = col - relCol;
    int cardTopRow = row - relRow;
    cardX = viewLocalX - static_cast<float>(cardTopCol) * cellW;
    cardY = viewLocalY - static_cast<float>(cardTopRow) * cellH;
  }

  // VTerm
  VTerm *_vterm = nullptr;
  VTermState *state_ = nullptr;

  // Buffers
  std::vector<Cell> _primaryBuffer;
  std::vector<Cell> _altBuffer;
  std::vector<Cell> viewBuffer_;
  std::vector<Cell> scratchBuffer_;
  std::vector<Cell> *_visibleBuffer = nullptr;

  // Scrollback
  std::deque<ScrollbackLineGPU> _scrollback;
  size_t maxScrollback_;
  int _scrollOffset = 0;
  bool viewBufferDirty_ = false;

  // Screen state
  int _rows = 0;
  int _cols = 0;
  int _cursorRow = 0;
  int _cursorCol = 0;
  bool _cursorVisible = true;
  bool _cursorBlink = true;
  int _cursorShape = 1; // VTERM_PROP_CURSORSHAPE_BLOCK
  bool _isAltScreen = false;
  bool isAltScreenExternal_ = false;
  bool _hasDamage = false;
  bool _reflow = true; // Enable reflow on resize (like screen.c)

  // Pen state
  Pen _pen;
  VTermColor defaultFg_;
  VTermColor defaultBg_;

  // Fonts
  MsMsdfFont::Ptr _msdfFont;
  BmFont::Ptr _bitmapFont;
  ShaderFont::Ptr _shaderGlyphFont;
  ShaderFont::Ptr _cardFont;
  uint32_t _cachedSpaceGlyph = 0;

  // Callbacks
  OutputCallback _outputCallback;
  std::function<void(VTermProp, VTermValue *)> termPropCallback_;
  std::function<void()> bellCallback_;
  std::function<void(const char *, size_t)> vtermInputCallback_;

  // Cards — active (visible, buffers allocated) and inactive (suspended)
  std::unordered_map<uint32_t, CardPtr> _cards;
  std::unordered_map<uint32_t, CardPtr> _inactiveCards;
  CardManager::Ptr _cardManager;
  base::ObjectId _cardMouseTarget = 0;  // card that received last CardMouseDown
  uint32_t _gcFrameCounter = 0;

  // Text selection state
  bool _selecting = false;
  int _selStartCol = 0, _selStartRow = 0;
  int _selEndCol = 0, _selEndRow = 0;
  bool _hasSelection = false;

  // OSC parsing
  OscCommandParser _oscParser;
  std::string _oscBuffer;
  int _oscCommand = -1;

  // Rendering - GPU resources (shared resources come from ShaderManager)
  YettyContext _ctx;
  // Per-instance resources only:
  WGPUBindGroup _bindGroup = nullptr;
  WGPUBuffer _uniformBuffer = nullptr;
  WGPUBuffer _cellBuffer = nullptr;
  size_t _cellBufferSize = 0;
  uint32_t _textureCols = 0;
  uint32_t _textureRows = 0;
  bool _pipelineInitialized = false;
  bool _needsBindGroupRecreation = false;
  uint32_t _lastFontResourceVersion = 0;

  // Grid uniforms (must match shader GridUniforms - group 1 binding 0)
  struct Uniforms {
    glm::mat4 projection;     // 64 bytes
    glm::vec2 screenSize;     // 8 bytes
    glm::vec2 cellSize;       // 8 bytes
    glm::vec2 gridSize;       // 8 bytes (cols, rows)
    float pixelRange;         // 4 bytes
    float scale;              // 4 bytes
    glm::vec2 cursorPos;      // 8 bytes (col, row)
    float cursorVisible;      // 4 bytes
    float cursorShape;        // 4 bytes (1=block, 2=underline, 3=bar)
    glm::vec2 viewportOrigin; // 8 bytes
    float cursorBlink;        // 4 bytes (0=no blink, 1=blink)
    float hasSelection;       // 4 bytes (0=no, 1=yes)
    glm::vec2 selStart;       // 8 bytes (col, row)
    glm::vec2 selEnd;         // 8 bytes (col, row)
  }; // Total: 144 bytes

  Uniforms _uniforms;

  // Cell size from font metrics
  float _baseCellWidth = 10.0f;  // from font advance
  float _baseCellHeight = 20.0f; // from font line height
  float _zoomLevel = 0.5f;       // adjustable with ctrl+wheel

  // Viewport (pixel coordinates within window)
  float _viewportX = 0.0f;
  float _viewportY = 0.0f;
  float _viewportWidth = 0.0f;
  float _viewportHeight = 0.0f;

  // Focus management
  base::ObjectId _id = 0;
  bool _focused = false;
  static base::ObjectId _nextId;
};

// State callbacks struct
static VTermStateCallbacks stateCallbacks = {
    .putglyph = GPUScreenImpl::onPutglyph,
    .movecursor = GPUScreenImpl::onMoveCursor,
    .scrollrect = GPUScreenImpl::onScrollRect,
    .moverect = GPUScreenImpl::onMoveRect,
    .erase = GPUScreenImpl::onErase,
    .initpen = GPUScreenImpl::onInitPen,
    .setpenattr = GPUScreenImpl::onSetPenAttr,
    .settermprop = GPUScreenImpl::onSetTermProp,
    .bell = GPUScreenImpl::onBell,
    .resize = GPUScreenImpl::onResize,
    .setlineinfo = GPUScreenImpl::onSetLineInfo,
    .sb_clear = nullptr,
};

// State fallbacks for unrecognized sequences (OSC, etc.)
static VTermStateFallbacks stateFallbacks = {
    .control = nullptr,
    .csi = nullptr,
    .osc = GPUScreenImpl::onOSC,
    .dcs = nullptr,
    .apc = nullptr,
    .pm = nullptr,
    .sos = nullptr,
};

// Static ID counter for GPUScreen instances
base::ObjectId GPUScreenImpl::_nextId = 1;

GPUScreenImpl::GPUScreenImpl(const YettyContext &ctx)
    : maxScrollback_(10000), _ctx(ctx), _id(_nextId++) {
  // Initialize default colors
  vterm_color_rgb(&defaultFg_, 204, 204, 204); // Light gray
  vterm_color_rgb(&defaultBg_, 15, 15, 35);    // Dark blue-ish

  _pen.fg = defaultFg_;
  _pen.bg = defaultBg_;

  // Start on primary screen
  _isAltScreen = false;

  // Fonts, cell size, vterm and buffers initialized in init()
}

Result<void> GPUScreenImpl::init() noexcept {
  // Get fonts from context
  if (_ctx.fontManager) {
    _msdfFont = _ctx.fontManager->getDefaultMsMsdfFont();
    _bitmapFont = _ctx.fontManager->getDefaultBmFont();
    _shaderGlyphFont = _ctx.fontManager->getDefaultShaderGlyphFont();
    _cardFont = _ctx.fontManager->getDefaultCardFont();
  }

  // Cache space glyph index to avoid repeated lookups in hot paths
  _cachedSpaceGlyph = _msdfFont ? _msdfFont->getGlyphIndex(' ') : 0;

  // Calculate base cell size from font metrics
  if (_msdfFont) {
    _baseCellHeight = _msdfFont->getLineHeight();
    const auto &metadata = _msdfFont->getGlyphMetadata();
    uint32_t mIndex = _msdfFont->getGlyphIndex('M');
    if (mIndex > 0 && mIndex < metadata.size()) {
      _baseCellWidth = metadata[mIndex]._advance;
    } else {
      _baseCellWidth = _baseCellHeight * 0.5f;
    }
  }

  // Create per-screen CardManager (owns CardBufferManager, CardTextureManager, and bind group).
  // Starts with tiny placeholder buffers (~20KB for metadata pool + 4 bytes each
  // for buffer/atlas). Real buffers and atlas are allocated lazily
  // on first card creation, avoiding ~81MB GPU allocation when no cards are used.
  if (_ctx.gpu.device && _ctx.gpu.sharedUniformBuffer) {
    auto cmResult = CardManager::create(&_ctx.gpu, _ctx.gpu.sharedUniformBuffer, _ctx.gpu.sharedUniformSize);
    if (!cmResult) {
      ywarn("GPUScreen: failed to create CardManager: {}", error_msg(cmResult));
    } else {
      _cardManager = *cmResult;
      _ctx.cardManager = _cardManager;
      yinfo("GPUScreen[{}]: created per-screen CardManager (lazy)", _id);
    }
  }

  return Ok();
}

GPUScreenImpl::~GPUScreenImpl() {
  if (_vterm) {
    vterm_free(_vterm);
    _vterm = nullptr;
  }
}

Result<void> GPUScreenImpl::onShutdown() {
  Result<void> result = Ok();

  // Deregister from all EventLoop events while shared_ptr is still alive
  auto loop = *base::EventLoop::instance();
  loop->deregisterListener(sharedAs<base::EventListener>());

  // Dispose active cards
  for (auto& [slotIndex, card] : _cards) {
    if (auto res = card->dispose(); !res) {
      result = Err<void>("Failed to dispose active card", res);
    }
  }
  _cards.clear();

  // Dispose inactive (suspended) cards
  for (auto& [slotIndex, card] : _inactiveCards) {
    if (auto res = card->dispose(); !res) {
      result = Err<void>("Failed to dispose inactive card", res);
    }
  }
  _inactiveCards.clear();

  // Release CardManager while shared_ptr is still alive
  _cardManager.reset();

  yinfo("GPUScreen[{}]: onShutdown complete", _id);
  return result;
}

void GPUScreenImpl::attach(VTerm *vt) {
  yinfo(">>> GPUScreenImpl::attach: ENTERED vt={}", (void*)vt);
  _vterm = vt;
  state_ = vterm_obtain_state(vt);
  yinfo(">>> GPUScreenImpl::attach: state_={}", (void*)state_);

  // Register our callbacks with State layer
  vterm_state_set_callbacks(state_, &stateCallbacks, this);
  yinfo(">>> GPUScreenImpl::attach: state callbacks registered");

  // Register fallbacks for unrecognized sequences (OSC, etc.)
  vterm_state_set_unrecognised_fallbacks(state_, &stateFallbacks, this);
  yinfo(">>> GPUScreenImpl::attach: OSC fallbacks registered (stateFallbacks.osc={})",
        (void*)stateFallbacks.osc);

  // Get default colors from vterm (white fg, black bg)
  vterm_state_get_default_colors(state_, &defaultFg_, &defaultBg_);

  yinfo("attach: defaultFg_ type={} idx={} rgb=({},{},{})",
        VTERM_COLOR_IS_DEFAULT_FG(&defaultFg_) ? "DEFAULT_FG" :
        VTERM_COLOR_IS_DEFAULT_BG(&defaultFg_) ? "DEFAULT_BG" :
        VTERM_COLOR_IS_INDEXED(&defaultFg_) ? "INDEXED" : "RGB",
        defaultFg_.indexed.idx,
        defaultFg_.rgb.red, defaultFg_.rgb.green, defaultFg_.rgb.blue);
  yinfo("attach: defaultBg_ type={} idx={} rgb=({},{},{})",
        VTERM_COLOR_IS_DEFAULT_BG(&defaultBg_) ? "DEFAULT_BG" :
        VTERM_COLOR_IS_DEFAULT_FG(&defaultBg_) ? "DEFAULT_FG" :
        VTERM_COLOR_IS_INDEXED(&defaultBg_) ? "INDEXED" : "RGB",
        defaultBg_.indexed.idx,
        defaultBg_.rgb.red, defaultBg_.rgb.green, defaultBg_.rgb.blue);

  // Convert to RGB if they're indexed
  if (VTERM_COLOR_IS_INDEXED(&defaultFg_)) {
    vterm_state_convert_color_to_rgb(state_, &defaultFg_);
    yinfo("attach: converted defaultFg_ to RGB: ({},{},{})",
          defaultFg_.rgb.red, defaultFg_.rgb.green, defaultFg_.rgb.blue);
  }
  if (VTERM_COLOR_IS_INDEXED(&defaultBg_)) {
    vterm_state_convert_color_to_rgb(state_, &defaultBg_);
    yinfo("attach: converted defaultBg_ to RGB: ({},{},{})",
          defaultBg_.rgb.red, defaultBg_.rgb.green, defaultBg_.rgb.blue);
  }

  _pen.fg = defaultFg_;
  _pen.bg = defaultBg_;

  // Reset state (triggers initpen, clears screen)
  vterm_state_reset(state_, 1);
}

void GPUScreenImpl::createVTerm() {
  if (_vterm)
    return; // Already have vterm

  _vterm = vterm_new(_rows, _cols);
  if (!_vterm)
    return;

  vterm_set_utf8(_vterm, 1);

  // Attach to our own vterm
  attach(_vterm);

  // Set output callback if it was provided before vterm was created
  if (_outputCallback) {
    vterm_output_set_callback(
        _vterm,
        [](const char *data, size_t len, void *user) {
          auto *self = static_cast<GPUScreenImpl *>(user);
          if (self->_outputCallback) {
            self->_outputCallback(data, len);
          }
        },
        this);
  }

  // Set up vterm input callback for card ANSI output
  // This allows cards to inject their ANSI-encoded cells into vterm
  vtermInputCallback_ = [this](const char *data, size_t len) {
    yinfo(">>> vtermInputCallback_: injecting {} bytes into vterm", len);
    if (_vterm) {
      vterm_input_write(_vterm, data, len);
    }
  };
  yinfo(">>> GPUScreenImpl::createVTerm: vtermInputCallback_ SET");
}

void GPUScreenImpl::write(const char *data, size_t len) {
  ydebug("GPUScreenImpl::write: len={} _vterm={}", len, (void *)_vterm);
  if (_vterm && len > 0) {
    vterm_input_write(_vterm, data, len);
  }
}

void GPUScreenImpl::resize(uint32_t cols, uint32_t rows) {
  // Create vterm on first resize if needed
  if (!_vterm) {
    resizeInternal(static_cast<int>(rows), static_cast<int>(cols), nullptr);
    createVTerm();
    return;
  }

  // Only resize if size actually changed
  if (static_cast<int>(rows) != _rows || static_cast<int>(cols) != _cols) {
    // vterm_set_size triggers our onResize callback which handles everything
    vterm_set_size(_vterm, static_cast<int>(rows), static_cast<int>(cols));
  }
}

void GPUScreenImpl::setViewport(float x, float y, float width, float height) {
  ydebug("GPUScreen {} setViewport({}, {}, {}, {})", _id, x, y, width, height);
  _viewportX = x;
  _viewportY = y;
  _viewportWidth = width;
  _viewportHeight = height;

  // Calculate cols/rows from viewport size and float cell size
  // Must use the same float cell dimensions the shader uses (baseCellSize * zoom)
  // to avoid integer truncation giving more cols than actually fit in the viewport
  float cellWidthF = _baseCellWidth * _zoomLevel;
  float cellHeightF = _baseCellHeight * _zoomLevel;

  uint32_t newCols = cellWidthF > 0 ? static_cast<uint32_t>(width / cellWidthF) : 0;
  uint32_t newRows = cellHeightF > 0 ? static_cast<uint32_t>(height / cellHeightF) : 0;

  // Only resize if size actually changed
  if (newCols > 0 && newRows > 0 && (newCols != _cols || newRows != _rows)) {
    resize(newCols, newRows);
  }

  // Update cards with viewport origin
  for (auto& [slotIndex, card] : _cards) {
    card->setScreenOrigin(x, y);
  }
}

// Helper: count non-blank cells from end of row (like screen.c line_popcount)
int GPUScreenImpl::linePopcount(const std::vector<Cell> &buffer, int row,
                                int cols) const {
  int col = cols - 1;
  while (col >= 0) {
    size_t idx = static_cast<size_t>(row * cols + col);
    if (idx < buffer.size() && buffer[idx].glyph != 0 &&
        buffer[idx].glyph != _cachedSpaceGlyph) {
      break;
    }
    col--;
  }
  return col + 1;
}

// Resize a single buffer with proper reflow (mirrors screen.c resize_buffer)
void GPUScreenImpl::resizeBuffer(int bufidx, int newRows, int newCols,
                                 bool active, VTermStateFields *fields) {
  yfunc();
  int oldRows = _rows;
  int oldCols = _cols;

  std::vector<Cell> &oldBuffer = (bufidx == 0) ? _primaryBuffer : _altBuffer;
  VTermLineInfo *oldLineinfo = fields ? fields->lineinfos[bufidx] : nullptr;

  // Allocate new buffer
  std::vector<Cell> newBuffer(static_cast<size_t>(newRows * newCols));
  clearBuffer(newBuffer);

  // Allocate new lineinfo
  VTermLineInfo *newLineinfo = nullptr;
  if (fields) {
    newLineinfo = static_cast<VTermLineInfo *>(
        calloc(static_cast<size_t>(newRows), sizeof(VTermLineInfo)));
  }

  int oldRow = oldRows - 1;
  int newRow = newRows - 1;

  VTermPos oldCursor = {_cursorRow, _cursorCol};
  VTermPos newCursor = {-1, -1};

  int finalBlankRow = newRows;

  // Walk backwards through old buffer, processing logical lines
  while (oldRow >= 0 && !oldBuffer.empty()) {
    int oldRowEnd = oldRow;

    // Find start of logical line by following continuation markers backwards
    while (_reflow && oldLineinfo && oldRow >= 0 &&
           oldLineinfo[oldRow].continuation) {
      oldRow--;
    }
    int oldRowStart = oldRow;

    // Calculate total width of logical line
    int width = 0;
    for (int row = oldRowStart; row <= oldRowEnd; row++) {
      if (_reflow && row < (oldRows - 1) && oldLineinfo &&
          oldLineinfo[row + 1].continuation) {
        width += oldCols; // Full row if next row is continuation
      } else {
        width += linePopcount(oldBuffer, row, oldCols);
      }
    }

    if (finalBlankRow == (newRow + 1) && width == 0) {
      finalBlankRow = newRow;
    }

    // Calculate how many new rows needed for this logical line
    int newHeight = _reflow ? (width ? (width + newCols - 1) / newCols : 1) : 1;

    int newRowEnd = newRow;
    int newRowStart = newRow - newHeight + 1;

    oldRow = oldRowStart;
    int oldCol = 0;

    int spareRows = newRows - finalBlankRow;

    // If we'd fall off top, try to scroll content down into blank rows
    if (newRowStart < 0 && spareRows >= 0 &&
        (!active || newCursor.row == -1 ||
         (newCursor.row - newRowStart) < newRows)) {
      int downwards = -newRowStart;
      if (downwards > spareRows)
        downwards = spareRows;
      int rowcount = newRows - downwards;

      // Move existing content down
      for (int r = rowcount - 1; r >= 0; r--) {
        for (int c = 0; c < newCols; c++) {
          newBuffer[(r + downwards) * newCols + c] = newBuffer[r * newCols + c];
        }
        if (newLineinfo) {
          newLineinfo[r + downwards] = newLineinfo[r];
        }
      }

      newRow += downwards;
      newRowStart += downwards;
      newRowEnd += downwards;

      if (newCursor.row >= 0)
        newCursor.row += downwards;
      finalBlankRow += downwards;
    }

    if (newRowStart < 0) {
      // Content falls off top - update cursor if it was in this logical line
      if (oldRowStart <= oldCursor.row && oldCursor.row <= oldRowEnd) {
        newCursor.row = 0;
        newCursor.col = oldCursor.col;
        if (newCursor.col >= newCols)
          newCursor.col = newCols - 1;
      }
      break;
    }

    // Copy content with reflow
    for (newRow = newRowStart, oldRow = oldRowStart; newRow <= newRowEnd;
         newRow++) {
      int count = (width >= newCols) ? newCols : width;
      width -= count;

      int newCol = 0;

      while (count > 0) {
        size_t oldIdx = static_cast<size_t>(oldRow * oldCols + oldCol);
        size_t newIdx = static_cast<size_t>(newRow * newCols + newCol);

        if (oldIdx < oldBuffer.size() && newIdx < newBuffer.size()) {
          newBuffer[newIdx] = oldBuffer[oldIdx];
        }

        // Track cursor
        if (oldCursor.row == oldRow && oldCursor.col == oldCol) {
          newCursor.row = newRow;
          newCursor.col = newCol;
        }

        oldCol++;
        if (oldCol == oldCols) {
          oldRow++;
          if (!_reflow) {
            newCol++;
            break;
          }
          oldCol = 0;
        }

        newCol++;
        count--;
      }

      // Handle cursor at end of copied content
      if (oldCursor.row == oldRow && oldCursor.col >= oldCol) {
        newCursor.row = newRow;
        newCursor.col = oldCursor.col - oldCol + newCol;
        if (newCursor.col >= newCols)
          newCursor.col = newCols - 1;
      }

      // Set continuation marker for new lineinfo
      if (newLineinfo) {
        newLineinfo[newRow].continuation = (newRow > newRowStart) ? 1 : 0;
      }
    }

    oldRow = oldRowStart - 1;
    newRow = newRowStart - 1;
  }

  // Handle cursor that would be off top
  if (oldCursor.row <= oldRow) {
    newCursor.row = 0;
    newCursor.col = oldCursor.col;
    if (newCursor.col >= newCols)
      newCursor.col = newCols - 1;
  }

  // Fallback cursor position
  if (active && (newCursor.row == -1 || newCursor.col == -1)) {
    newCursor.row = 0;
    newCursor.col = 0;
  }

  // Handle scrollback for primary buffer
  if (oldRow >= 0 && bufidx == 0) {
    // Push remaining top lines to scrollback
    _visibleBuffer = &oldBuffer;
    for (int row = 0; row <= oldRow; row++) {
      pushLineToScrollback(row);
    }
    if (active) {
      newCursor.row -= (oldRow + 1);
      if (newCursor.row < 0)
        newCursor.row = 0;
    }
  }

  // If there's space at top, scroll content up
  if (newRow >= 0) {
    int moveRows = newRows - newRow - 1;
    for (int r = 0; r < moveRows; r++) {
      for (int c = 0; c < newCols; c++) {
        newBuffer[r * newCols + c] = newBuffer[(r + newRow + 1) * newCols + c];
      }
      if (newLineinfo) {
        newLineinfo[r] = newLineinfo[r + newRow + 1];
      }
    }

    newCursor.row -= (newRow + 1);
    if (newCursor.row < 0)
      newCursor.row = 0;

    // Clear bottom rows
    for (int r = moveRows; r < newRows; r++) {
      for (int c = 0; c < newCols; c++) {
        size_t idx = static_cast<size_t>(r * newCols + c);
        newBuffer[idx].glyph = _cachedSpaceGlyph;
      }
      if (newLineinfo) {
        newLineinfo[r] = VTermLineInfo{};
      }
    }
  }

  // Store new buffer
  if (bufidx == 0) {
    _primaryBuffer = std::move(newBuffer);
  } else {
    _altBuffer = std::move(newBuffer);
  }

  // Update lineinfo in fields
  if (fields && oldLineinfo) {
    free(oldLineinfo);
    fields->lineinfos[bufidx] = newLineinfo;
  }

  // Update cursor if this is active buffer
  if (active) {
    _cursorRow = newCursor.row;
    _cursorCol = newCursor.col;
  }
}

void GPUScreenImpl::resizeInternal(int rows, int cols,
                                   VTermStateFields *fields) {
  // Check if this is a no-op (same size AND buffers already allocated)
  if (rows == _rows && cols == _cols && !_primaryBuffer.empty()) {
    return;
  }

  yinfo("GPUScreenImpl::resize: {}x{} -> {}x{} isAltScreen={}", _rows, _cols,
        rows, cols, _isAltScreen);

  int oldRows = _rows;
  int oldCols = _cols;
  bool altscreenActive = _isAltScreen;

  // Resize primary buffer (active if not on altscreen)
  resizeBuffer(0, rows, cols, !altscreenActive, fields);

  // Resize alt buffer if it exists
  if (!_altBuffer.empty()) {
    resizeBuffer(1, rows, cols, altscreenActive, fields);
  } else if (rows != oldRows && fields && fields->lineinfos[1]) {
    // Alt buffer not in use but we need to resize its lineinfo
    free(fields->lineinfos[1]);
    VTermLineInfo *newLineinfo = static_cast<VTermLineInfo *>(
        calloc(static_cast<size_t>(rows), sizeof(VTermLineInfo)));
    fields->lineinfos[1] = newLineinfo;
  }

  // Update dimensions
  _rows = rows;
  _cols = cols;

  // Resize view buffer and scratch buffer
  size_t numCells = static_cast<size_t>(rows * cols);
  viewBuffer_.clear();
  viewBuffer_.resize(numCells);
  scratchBuffer_.resize(cols);

  // Re-establish visible buffer pointer
  _visibleBuffer = altscreenActive ? &_altBuffer : &_primaryBuffer;

  // Reset scroll offset
  if (!altscreenActive) {
    _scrollOffset = 0;
  }

  _hasDamage = true;
  viewBufferDirty_ = true;
}

void GPUScreenImpl::clearBuffer(std::vector<Cell> &buffer) {
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  colorToRGB(defaultFg_, fgR, fgG, fgB);
  colorToRGB(defaultBg_, bgR, bgG, bgB);

  Cell defaultCell;
  defaultCell.glyph = _cachedSpaceGlyph;
  defaultCell.fgR = fgR;
  defaultCell.fgG = fgG;
  defaultCell.fgB = fgB;
  defaultCell.bgR = bgR;
  defaultCell.bgG = bgG;
  defaultCell.bgB = bgB;
  defaultCell.alpha = 255;
  defaultCell.style = 0;

  std::fill(buffer.begin(), buffer.end(), defaultCell);
}

void GPUScreenImpl::switchToScreen(bool alt) {
  if (_isAltScreen == alt)
    return; // Already on the requested screen

  yinfo("GPUScreenImpl::switchToScreen: {} -> {}",
        _isAltScreen ? "alt" : "primary", alt ? "alt" : "primary");

  _isAltScreen = alt;

  if (alt) {
    // Allocate alt buffer if not yet allocated
    if (_altBuffer.empty()) {
      size_t numCells = static_cast<size_t>(_rows * _cols);
      _altBuffer.resize(numCells);
    }

    // Switch to alternate screen
    _visibleBuffer = &_altBuffer;

    // Clear alternate screen on entry (traditional terminal behavior)
    clearBuffer(_altBuffer);

    // Alternate screen doesn't use scrollback
    _scrollOffset = 0;
  } else {
    // Switch back to primary screen
    _visibleBuffer = &_primaryBuffer;

    // Primary screen content is preserved (was just hidden)
  }

  _hasDamage = true;
  viewBufferDirty_ = true;
}

void GPUScreenImpl::reset() {
  // Clear current screen with space and default colors
  if (_visibleBuffer) {
    clearBuffer(*_visibleBuffer);
  }

  _cursorRow = 0;
  _cursorCol = 0;
  _cursorVisible = true;
  _hasDamage = true;
  viewBufferDirty_ = true;
  if (!_isAltScreen) {
    _scrollOffset = 0;
  }
}

//=============================================================================
// Buffer access - returns Cell buffer directly for GPU upload (zero-copy)
//=============================================================================

const Cell *GPUScreenImpl::getCellData() const {
  if (_scrollOffset > 0) {
    const_cast<GPUScreenImpl *>(this)->composeViewBuffer();
    return viewBuffer_.data();
  }
  if (_visibleBuffer) {
    return _visibleBuffer->data();
  }
  return nullptr;
}

Cell GPUScreenImpl::getCell(int row, int col) const {
  if (!_visibleBuffer || row < 0 || col < 0 || row >= _rows || col >= _cols) {
    Cell empty = {};
    return empty;
  }
  size_t idx = static_cast<size_t>(row) * static_cast<size_t>(_cols) +
               static_cast<size_t>(col);
  return (*_visibleBuffer)[idx];
}

//=============================================================================
// Scrollback control
//=============================================================================

void GPUScreenImpl::scrollUp(int lines) {
  int maxOffset = static_cast<int>(_scrollback.size());
  int newOffset = std::min(_scrollOffset + lines, maxOffset);
  if (newOffset != _scrollOffset) {
    _scrollOffset = newOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollDown(int lines) {
  int newOffset = std::max(_scrollOffset - lines, 0);
  if (newOffset != _scrollOffset) {
    _scrollOffset = newOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollToTop() {
  int maxOffset = static_cast<int>(_scrollback.size());
  if (_scrollOffset != maxOffset) {
    _scrollOffset = maxOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollToBottom() {
  if (_scrollOffset != 0) {
    _scrollOffset = 0;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

//=============================================================================
// View buffer composition (when scrolled back)
//=============================================================================

void GPUScreenImpl::composeViewBuffer() {
  if (!viewBufferDirty_)
    return;

  // _scrollOffset lines come from scrollback, rest from visible
  // _scrollback[size-1] is newest, _scrollback[0] is oldest
  // When _scrollOffset = N, we show scrollback lines [size-N, size-1] at top
  // plus visible lines [0, rows-N-1] shifted down

  int sbSize = static_cast<int>(_scrollback.size());
  int sbLinesToShow = std::min(_scrollOffset, _rows);
  int visibleLinesToShow = _rows - sbLinesToShow;

  // Fill top rows from scrollback (newest first in view)
  for (int viewRow = 0; viewRow < sbLinesToShow; viewRow++) {
    // scrollback index: newest lines are at end
    // viewRow 0 should get _scrollback[sbSize - _scrollOffset]
    int sbIndex = sbSize - _scrollOffset + viewRow;
    if (sbIndex >= 0 && sbIndex < sbSize) {
      decompressLine(_scrollback[sbIndex], viewRow);
    }
  }

  // Fill bottom rows from visible buffer - single memcpy per row with Cell
  // struct
  if (_visibleBuffer) {
    for (int viewRow = sbLinesToShow; viewRow < _rows; viewRow++) {
      int visRow = viewRow - sbLinesToShow; // Source row in visible buffer
      size_t numCells = static_cast<size_t>(_cols);
      size_t srcOffset = static_cast<size_t>(visRow * _cols);
      size_t dstOffset = static_cast<size_t>(viewRow * _cols);

      // Single memcpy for entire row of cells
      std::memcpy(&viewBuffer_[dstOffset], &(*_visibleBuffer)[srcOffset],
                  numCells * sizeof(Cell));
    }
  }

  viewBufferDirty_ = false;
}

void GPUScreenImpl::decompressLine(const ScrollbackLineGPU &line, int viewRow) {
  size_t dstOffset = static_cast<size_t>(viewRow * _cols);

  // Copy cells directly from scrollback
  int lineCols = std::min(static_cast<int>(line.cells.size()), _cols);
  for (int col = 0; col < lineCols; col++) {
    viewBuffer_[dstOffset + col] = line.cells[col];
  }

  // Fill remainder with default cells
  uint8_t fgR = defaultFg_.rgb.red, fgG = defaultFg_.rgb.green,
          fgB = defaultFg_.rgb.blue;
  uint8_t bgR = defaultBg_.rgb.red, bgG = defaultBg_.rgb.green,
          bgB = defaultBg_.rgb.blue;

  Cell defaultCell;
  defaultCell.glyph = _cachedSpaceGlyph;
  defaultCell.fgR = fgR;
  defaultCell.fgG = fgG;
  defaultCell.fgB = fgB;
  defaultCell.bgR = bgR;
  defaultCell.bgG = bgG;
  defaultCell.bgB = bgB;
  defaultCell.alpha = 255;
  defaultCell.style = 0;

  for (int col = lineCols; col < _cols; col++) {
    viewBuffer_[dstOffset + col] = defaultCell;
  }
}

//=============================================================================
// Scrollback push (compress line and store)
//=============================================================================

void GPUScreenImpl::pushLineToScrollback(int row) {
  if (!_visibleBuffer)
    return; // Safety check

  ScrollbackLineGPU line;
  line.cells.resize(_cols);

  // Copy cell data directly
  size_t srcOffset = static_cast<size_t>(row * _cols);
  std::memcpy(line.cells.data(), &(*_visibleBuffer)[srcOffset],
              _cols * sizeof(Cell));

  // Scan this row for card glyphs — check if any card's top-left cell is
  // leaving the visible area.  Top-left is identified by bg=0 (relRow=0, relCol=0).
  suspendOffscreenCards(row);

  _scrollback.push_back(std::move(line));

  // If scrolled back, increment scroll offset to maintain view position
  if (_scrollOffset > 0) {
    _scrollOffset++;
  }

  // Limit scrollback size — dispose cards whose scrollback lines are gone
  while (_scrollback.size() > maxScrollback_) {
    _scrollback.pop_front();
    if (_scrollOffset > static_cast<int>(_scrollback.size())) {
      _scrollOffset = static_cast<int>(_scrollback.size());
    }
  }
}

//=============================================================================
// Text selection helpers
//=============================================================================

void GPUScreenImpl::clearSelection() {
  _selecting = false;
  _hasSelection = false;
  _hasDamage = true;
}

// Encode a Unicode codepoint as UTF-8 and append to string
static void appendUtf8(std::string& out, uint32_t cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else if (cp < 0x800) {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else if (cp < 0x10000) {
    out += static_cast<char>(0xE0 | (cp >> 12));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else if (cp < 0x110000) {
    out += static_cast<char>(0xF0 | (cp >> 18));
    out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

std::string GPUScreenImpl::extractSelectedText() const {
  if (!_hasSelection || !_msdfFont) return "";

  // Normalize selection direction (start <= end)
  int r0 = _selStartRow, c0 = _selStartCol;
  int r1 = _selEndRow, c1 = _selEndCol;
  if (r0 > r1 || (r0 == r1 && c0 > c1)) {
    std::swap(r0, r1);
    std::swap(c0, c1);
  }

  std::string result;
  int sbSize = static_cast<int>(_scrollback.size());
  int sbLines = std::min(_scrollOffset, sbSize);

  for (int row = r0; row <= r1; row++) {
    int startCol = (row == r0) ? c0 : 0;
    int endCol = (row == r1) ? c1 : _cols - 1;
    std::string line;

    if (row < sbLines) {
      // Row comes from scrollback buffer
      int sbIdx = sbSize - sbLines + row;
      if (sbIdx >= 0 && sbIdx < sbSize) {
        const auto& sbLine = _scrollback[sbIdx];
        for (int col = startCol; col <= endCol; col++) {
          if (col < static_cast<int>(sbLine.cells.size())) {
            uint32_t glyphIdx = sbLine.cells[col].glyph;
            uint32_t cp = _msdfFont->getCodepoint(glyphIdx);
            if (cp == 0) cp = ' ';
            appendUtf8(line, cp);
          }
        }
      }
    } else if (_visibleBuffer) {
      // Row comes from visible buffer - use cell glyph indices
      int bufRow = row - sbLines;
      if (bufRow >= 0 && bufRow < static_cast<int>(_rows)) {
        size_t rowOffset = static_cast<size_t>(bufRow * _cols);
        for (int col = startCol; col <= endCol; col++) {
          size_t idx = rowOffset + col;
          if (idx < _visibleBuffer->size()) {
            uint32_t glyphIdx = (*_visibleBuffer)[idx].glyph;
            uint32_t cp = _msdfFont->getCodepoint(glyphIdx);
            if (cp == 0) cp = ' ';
            appendUtf8(line, cp);
          }
        }
      }
    }

    while (!line.empty() && line.back() == ' ') line.pop_back();
    if (row > r0) result += '\n';
    result += line;
  }

  return result;
}

//=============================================================================
// Cell manipulation (writes to visible buffer)
//=============================================================================

void GPUScreenImpl::setCell(int row, int col, uint32_t glyph, uint8_t fgR,
                            uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG,
                            uint8_t bgB, uint8_t attrsByte) {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols)
    return;
  if (!_visibleBuffer)
    return; // Safety check

  size_t idx = cellIndex(row, col);

  // Bounds check on buffer
  if (idx >= _visibleBuffer->size())
    return;

  Cell &cell = (*_visibleBuffer)[idx];
  cell.glyph = glyph;
  cell.fgR = fgR;
  cell.fgG = fgG;
  cell.fgB = fgB;
  cell.bgR = bgR;
  cell.bgG = bgG;
  cell.bgB = bgB;
  cell.alpha = 255;
  cell.style = attrsByte;
}

void GPUScreenImpl::clearCell(int row, int col) {
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  colorToRGB(_pen.fg, fgR, fgG, fgB);
  colorToRGB(_pen.bg, bgR, bgG, bgB);

  if (_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  setCell(row, col, _cachedSpaceGlyph, fgR, fgG, fgB, bgR, bgG, bgB, 0);
}

void GPUScreenImpl::colorToRGB(const VTermColor &color, uint8_t &r, uint8_t &g,
                               uint8_t &b) {
  const char* colorType = "UNKNOWN";
  if (VTERM_COLOR_IS_DEFAULT_FG(&color)) {
    colorType = "DEFAULT_FG";
    r = defaultFg_.rgb.red;
    g = defaultFg_.rgb.green;
    b = defaultFg_.rgb.blue;
  } else if (VTERM_COLOR_IS_DEFAULT_BG(&color)) {
    colorType = "DEFAULT_BG";
    r = defaultBg_.rgb.red;
    g = defaultBg_.rgb.green;
    b = defaultBg_.rgb.blue;
  } else if (VTERM_COLOR_IS_INDEXED(&color)) {
    colorType = "INDEXED";
    VTermColor rgb = color;
    if (state_) {
      vterm_state_convert_color_to_rgb(state_, &rgb);
    }
    r = rgb.rgb.red;
    g = rgb.rgb.green;
    b = rgb.rgb.blue;
  } else {
    colorType = "RGB";
    r = color.rgb.red;
    g = color.rgb.green;
    b = color.rgb.blue;
  }

  // Debug: log non-default color conversions
  static int colorDebugCount = 0;
  bool isNonDefault = !VTERM_COLOR_IS_DEFAULT_FG(&color) && !VTERM_COLOR_IS_DEFAULT_BG(&color);
  if (colorDebugCount < 50 && isNonDefault) {
    colorDebugCount++;
    yinfo("colorToRGB: type={} idx={} -> RGB({},{},{})",
          colorType, color.indexed.idx, r, g, b);
  }
}

//=============================================================================
// State callbacks
//=============================================================================

int GPUScreenImpl::onPutglyph(VTermGlyphInfo *info, VTermPos pos, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  uint32_t cp = info->chars[0];

  // Debug: log all high codepoints (above BMP)
  if (cp >= 0x10000) {
    yinfo(">>> onPutglyph: HIGH CODEPOINT cp={:#x} pos=({},{})", cp, pos.row, pos.col);
  }

  if (cp == 0)
    cp = ' ';

  // Determine font type and glyph index based on codepoint
  uint32_t glyphIdx = 0;
  uint8_t fontType = FONT_TYPE_MSDF; // Default to MSDF text

  if (isCardGlyph(cp)) {
    // Card glyphs (multi-cell widgets)
    fontType = FONT_TYPE_CARD;
    glyphIdx = self->_cardFont ? self->_cardFont->getGlyphIndex(cp) : cp;
    yinfo(">>> CARD GLYPH: cp={:#x} glyphIdx={} _cardFont={} pos=({},{})",
          cp, glyphIdx, (void*)self->_cardFont.get(), pos.row, pos.col);
  } else if (isShaderGlyph(cp)) {
    // Shader glyphs (single-cell animated)
    fontType = FONT_TYPE_SHADER;
    glyphIdx =
        self->_shaderGlyphFont ? self->_shaderGlyphFont->getGlyphIndex(cp) : cp;
  } else if (isEmoji(cp) && self->_bitmapFont) {
    // Emoji - render from bitmap font atlas
    fontType = FONT_TYPE_BITMAP;
    glyphIdx = self->_bitmapFont->getGlyphIndex(cp);
  } else if (self->_msdfFont) {
    // Regular text - get glyph from MSDF font
    fontType = FONT_TYPE_MSDF;
    glyphIdx =
        self->_msdfFont->getGlyphIndex(cp, self->_pen.bold, self->_pen.italic);
  } else {
    fontType = FONT_TYPE_MSDF;
    glyphIdx = cp;
  }

  // Get colors from current pen
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  self->colorToRGB(self->_pen.fg, fgR, fgG, fgB);
  self->colorToRGB(self->_pen.bg, bgR, bgG, bgB);

  // Debug: log first few colored glyphs with bold state
  static int debugCount = 0;
  bool hasColor = (bgR != 0 || bgG != 0 || bgB != 0) ||
                  (fgR != 240 || fgG != 240 || fgB != 240);
  if (debugCount < 10 && hasColor) {
    debugCount++;
    yinfo("onPutglyph: '{}' glyph={} bold={} fg=({},{},{}) fontType={}",
          (cp >= 32 && cp < 127) ? static_cast<char>(cp) : '?', glyphIdx,
          self->_pen.bold, fgR, fgG, fgB, fontType);
  }

  if (self->_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  // Pack attributes (bits 0-4) with font type (bits 5-7)
  uint8_t attrsByte = 0;
  if (self->_pen.bold)
    attrsByte |= 0x01;
  if (self->_pen.italic)
    attrsByte |= 0x02;
  attrsByte |= (self->_pen.underline & 0x03) << 2;
  if (self->_pen.strike)
    attrsByte |= 0x10;
  attrsByte |= (fontType & 0x07) << 5; // Pack font type into bits 5-7

  // Debug card glyph cells
  if (fontType == FONT_TYPE_CARD) {
    uint32_t cellSlotIndex = fgR | (static_cast<uint32_t>(fgG) << 8) | (static_cast<uint32_t>(fgB) << 16);
    uint32_t bg24 = bgR | (static_cast<uint32_t>(bgG) << 8) | (static_cast<uint32_t>(bgB) << 16);
    int relCol = bg24 & 0xFFF;
    int relRow = (bg24 >> 12) & 0xFFF;
    ydebug("CARD CELL: pos=({},{}) glyph={:#x} glyphIdx={} slotIndex={} relPos=({},{}) attrs={:#x}",
           pos.row, pos.col, cp, glyphIdx, cellSlotIndex, relRow, relCol, attrsByte);
  }

  self->setCell(pos.row, pos.col, glyphIdx, fgR, fgG, fgB, bgR, bgG, bgB,
                attrsByte);

  // Handle wide characters (width > 1)
  for (int i = 1; i < info->width; i++) {
    self->setCell(pos.row, pos.col + i, GLYPH_WIDE_CONT, fgR, fgG, fgB, bgR,
                  bgG, bgB, attrsByte);
  }

  self->_hasDamage = true;
  return 1;
}

int GPUScreenImpl::onMoveCursor(VTermPos pos, VTermPos oldpos, int visible,
                                void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  ydebug("GPUScreenImpl::onMoveCursor: ({},{}) -> ({},{})", oldpos.row,
         oldpos.col, pos.row, pos.col);
  self->_cursorRow = pos.row;
  self->_cursorCol = pos.col;
  self->_cursorVisible = visible != 0;
  self->_hasDamage = true;
  return 1;
}

int GPUScreenImpl::onScrollRect(VTermRect rect, int downward, int rightward,
                                void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  (void)rightward;

  bool isFullWidth = (rect.start_col == 0 && rect.end_col == self->_cols);

  // Save lines to scrollback BEFORE vterm moves content (via moverect/erase)
  // downward > 0: content moves down, top rows pushed to scrollback
  // downward < 0: content moves up, bottom rows cleared (less common)
  if (downward > 0 && rect.start_row == 0 && isFullWidth) {
    for (int i = 0; i < downward && i < rect.end_row; i++) {
      self->pushLineToScrollback(i);
    }
  } else if (downward < 0) {
    // Scroll up: top lines will be overwritten
    int upAmount = -downward;

    if (rect.start_row == 0 && isFullWidth) {
      // Full-screen scroll from top - push to scrollback
      for (int i = 0; i < upAmount && i < rect.end_row; i++) {
        self->pushLineToScrollback(i);
      }
    }
  }

  // Return 0 to let vterm handle the actual scroll via moverect/erase callbacks
  return 0;
}

int GPUScreenImpl::onMoveRect(VTermRect dest, VTermRect src, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  int height = src.end_row - src.start_row;
  int width = src.end_col - src.start_col;
  int cols = self->_cols;
  int rows = self->_rows;

  // Bounds checking to prevent memory corruption
  if (height <= 0 || width <= 0)
    return 1;
  if (src.start_row < 0 || src.end_row > rows)
    return 1;
  if (dest.start_row < 0 || dest.start_row + height > rows)
    return 1;
  if (src.start_col < 0 || src.end_col > cols)
    return 1;
  if (dest.start_col < 0 || dest.start_col + width > cols)
    return 1;

  if (!self->_visibleBuffer)
    return 1; // Safety check

  // Ultra-fast path: full-width move starting at column 0
  // Memory is fully contiguous - single memmove for entire region of Cells
  if (src.start_col == 0 && dest.start_col == 0 && width == cols) {
    size_t srcIdx = self->cellIndex(src.start_row, 0);
    size_t dstIdx = self->cellIndex(dest.start_row, 0);
    size_t totalCells = static_cast<size_t>(height) * cols;

    // Single memmove for all cell data
    std::memmove(&(*self->_visibleBuffer)[dstIdx],
                 &(*self->_visibleBuffer)[srcIdx], totalCells * sizeof(Cell));
  }
  // Fast path: full-width but not starting at column 0 (scroll regions)
  // Still contiguous per row, do row-by-row with single memmove per row
  else if (src.start_col == dest.start_col && width == cols) {
    if (dest.start_row < src.start_row) {
      for (int row = 0; row < height; row++) {
        size_t srcIdx = self->cellIndex(src.start_row + row, 0);
        size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

        std::memmove(&(*self->_visibleBuffer)[dstIdx],
                     &(*self->_visibleBuffer)[srcIdx], width * sizeof(Cell));
      }
    } else {
      for (int row = height - 1; row >= 0; row--) {
        size_t srcIdx = self->cellIndex(src.start_row + row, 0);
        size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

        std::memmove(&(*self->_visibleBuffer)[dstIdx],
                     &(*self->_visibleBuffer)[srcIdx], width * sizeof(Cell));
      }
    }
  } else {
    // General case: use pre-allocated scratch buffer, copy row-by-row
    // Ensure scratch buffer is large enough
    if (static_cast<int>(self->scratchBuffer_.size()) < width) {
      self->scratchBuffer_.resize(width);
    }

    // Determine copy direction
    bool copyForward =
        (dest.start_row < src.start_row) ||
        (dest.start_row == src.start_row && dest.start_col <= src.start_col);

    if (copyForward) {
      for (int row = 0; row < height; row++) {
        size_t srcRowStart =
            self->cellIndex(src.start_row + row, src.start_col);
        size_t dstRowStart =
            self->cellIndex(dest.start_row + row, dest.start_col);

        // Copy to scratch, then to dest (single memcpy for all cell data)
        std::memcpy(self->scratchBuffer_.data(),
                    &(*self->_visibleBuffer)[srcRowStart],
                    width * sizeof(Cell));
        std::memcpy(&(*self->_visibleBuffer)[dstRowStart],
                    self->scratchBuffer_.data(), width * sizeof(Cell));
      }
    } else {
      for (int row = height - 1; row >= 0; row--) {
        size_t srcRowStart =
            self->cellIndex(src.start_row + row, src.start_col);
        size_t dstRowStart =
            self->cellIndex(dest.start_row + row, dest.start_col);

        // Copy to scratch, then to dest (single memcpy for all cell data)
        std::memcpy(self->scratchBuffer_.data(),
                    &(*self->_visibleBuffer)[srcRowStart],
                    width * sizeof(Cell));
        std::memcpy(&(*self->_visibleBuffer)[dstRowStart],
                    self->scratchBuffer_.data(), width * sizeof(Cell));
      }
    }
  }

  self->_hasDamage = true;
  self->viewBufferDirty_ = true;
  return 1;
}

int GPUScreenImpl::onErase(VTermRect rect, int, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (!self->_visibleBuffer)
    return 1; // Safety check

  // Bounds checking to prevent memory corruption
  int startRow = std::max(0, rect.start_row);
  int endRow = std::min(self->_rows, rect.end_row);
  int startCol = std::max(0, rect.start_col);
  int endCol = std::min(self->_cols, rect.end_col);
  int width = endCol - startCol;
  int cols = self->_cols;

  if (width <= 0 || startRow >= endRow)
    return 1;

  // Pre-compute colors once (not per cell)
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  self->colorToRGB(self->_pen.fg, fgR, fgG, fgB);
  self->colorToRGB(self->_pen.bg, bgR, bgG, bgB);
  if (self->_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  // Build default cell
  Cell defaultCell;
  defaultCell.glyph = self->_cachedSpaceGlyph;
  defaultCell.fgR = fgR;
  defaultCell.fgG = fgG;
  defaultCell.fgB = fgB;
  defaultCell.bgR = bgR;
  defaultCell.bgG = bgG;
  defaultCell.bgB = bgB;
  defaultCell.alpha = 255;
  defaultCell.style = 0;

  // Fast path: full-width erase - use std::fill for entire row
  if (startCol == 0 && endCol == cols) {
    for (int row = startRow; row < endRow; row++) {
      size_t rowOffset = static_cast<size_t>(row * cols);
      std::fill(&(*self->_visibleBuffer)[rowOffset],
                &(*self->_visibleBuffer)[rowOffset + cols], defaultCell);
    }
  } else {
    // Partial width
    for (int row = startRow; row < endRow; row++) {
      size_t rowOffset = static_cast<size_t>(row * cols);
      for (int col = startCol; col < endCol; col++) {
        (*self->_visibleBuffer)[rowOffset + col] = defaultCell;
      }
    }
  }

  self->_hasDamage = true;
  if (self->_scrollOffset > 0) {
    self->viewBufferDirty_ = true;
  }
  return 1;
}

int GPUScreenImpl::onInitPen(void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  // Reset pen to defaults
  self->_pen.fg = self->defaultFg_;
  self->_pen.bg = self->defaultBg_;
  self->_pen.bold = false;
  self->_pen.italic = false;
  self->_pen.underline = 0;
  self->_pen.strike = false;
  self->_pen.reverse = false;
  self->_pen.blink = false;

  return 1;
}

int GPUScreenImpl::onSetPenAttr(VTermAttr attr, VTermValue *val, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  switch (attr) {
  case VTERM_ATTR_BOLD:
    self->_pen.bold = val->boolean != 0;
    yinfo("VTERM_ATTR_BOLD: val->boolean={} pen.bold={}", val->boolean, self->_pen.bold);
    break;
  case VTERM_ATTR_ITALIC:
    self->_pen.italic = val->boolean != 0;
    yinfo("VTERM_ATTR_ITALIC: val->boolean={} pen.italic={}", val->boolean, self->_pen.italic);
    break;
  case VTERM_ATTR_UNDERLINE:
    self->_pen.underline = static_cast<uint8_t>(val->number & 0x03);
    break;
  case VTERM_ATTR_STRIKE:
    self->_pen.strike = val->boolean != 0;
    break;
  case VTERM_ATTR_REVERSE:
    self->_pen.reverse = val->boolean != 0;
    yinfo("VTERM_ATTR_REVERSE: val->boolean={} pen.reverse={}", val->boolean,
          self->_pen.reverse);
    break;
  case VTERM_ATTR_BLINK:
    self->_pen.blink = val->boolean != 0;
    break;
  case VTERM_ATTR_FOREGROUND:
    self->_pen.fg = val->color;
    yinfo("VTERM_ATTR_FOREGROUND: type={} idx={} rgb=({},{},{})",
          VTERM_COLOR_IS_DEFAULT_FG(&val->color) ? "DEFAULT_FG" :
          VTERM_COLOR_IS_DEFAULT_BG(&val->color) ? "DEFAULT_BG" :
          VTERM_COLOR_IS_INDEXED(&val->color) ? "INDEXED" : "RGB",
          val->color.indexed.idx,
          val->color.rgb.red, val->color.rgb.green, val->color.rgb.blue);
    break;
  case VTERM_ATTR_BACKGROUND:
    self->_pen.bg = val->color;
    yinfo("VTERM_ATTR_BACKGROUND: type={} idx={} rgb=({},{},{})",
          VTERM_COLOR_IS_DEFAULT_BG(&val->color) ? "DEFAULT_BG" :
          VTERM_COLOR_IS_DEFAULT_FG(&val->color) ? "DEFAULT_FG" :
          VTERM_COLOR_IS_INDEXED(&val->color) ? "INDEXED" : "RGB",
          val->color.indexed.idx,
          val->color.rgb.red, val->color.rgb.green, val->color.rgb.blue);
    break;
  default:
    break;
  }

  return 1;
}

int GPUScreenImpl::onSetTermProp(VTermProp prop, VTermValue *val, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (prop == VTERM_PROP_CURSORVISIBLE) {
    self->_cursorVisible = val->boolean != 0;
  } else if (prop == VTERM_PROP_CURSORBLINK) {
    self->_cursorBlink = val->boolean != 0;
  } else if (prop == VTERM_PROP_CURSORSHAPE) {
    self->_cursorShape = val->number;
  } else if (prop == VTERM_PROP_ALTSCREEN) {
    // Switch between primary and alternate screen
    self->switchToScreen(val->boolean != 0);
  }

  if (self->termPropCallback_) {
    self->termPropCallback_(prop, val);
  }

  return 1;
}

int GPUScreenImpl::onBell(void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (self->bellCallback_) {
    self->bellCallback_();
  }

  return 1;
}

int GPUScreenImpl::onResize(int rows, int cols, VTermStateFields *fields,
                            void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  // resizeInternal handles buffer reflow AND lineinfo reallocation (like
  // screen.c)
  self->resizeInternal(rows, cols, fields);
  return 1;
}

int GPUScreenImpl::onSetLineInfo(int, const VTermLineInfo *,
                                 const VTermLineInfo *, void *) {
  // Line info (double-width, double-height) - not commonly used
  return 1;
}

int GPUScreenImpl::onOSC(int command, VTermStringFragment frag, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  yinfo(">>> onOSC: command={} initial={} final={} len={}", command,
         (bool)frag.initial, (bool)frag.final, (size_t)frag.len);

  if (command != YETTY_OSC_VENDOR_ID) {
    yinfo(">>> onOSC: ignoring non-yetty command {}", command);
    return 0;
  }

  yinfo(">>> onOSC: *** YETTY OSC DETECTED *** command={}", command);

  if (frag.initial) {
    self->_oscBuffer.clear();
    self->_oscCommand = command;
    yinfo(">>> onOSC: started new OSC buffer");
  }

  if (frag.len > 0) {
    self->_oscBuffer.append(frag.str, frag.len);
    yinfo(">>> onOSC: appended {} bytes, total={}", (size_t)frag.len,
           self->_oscBuffer.size());
  }

  if (frag.final) {
    std::string fullSeq = std::to_string(command) + ";" + self->_oscBuffer;
    yinfo("onOSC: FINAL - fullSeq len={} first100chars='{}'",
          fullSeq.size(), fullSeq.substr(0, 100));

    std::string response;
    uint32_t linesToAdvance = 0;

    // Forward OSC handling to handleOSCSequence
    bool handled = self->handleOSCSequence(fullSeq, &response, &linesToAdvance);

    yinfo("onOSC: handled={} response_len={} linesToAdvance={}", handled,
           response.size(), linesToAdvance);

    if (handled) {
      self->viewBufferDirty_ = true;
      if (!response.empty() && self->_outputCallback) {
        // Send response back to the shell
        self->_outputCallback(response.c_str(), response.size());
      }
      // Inject newlines immediately to advance cursor past the widget
      if (linesToAdvance > 0 && self->_vterm) {
        std::string nl(linesToAdvance, '\n');
        vterm_input_write(self->_vterm, nl.c_str(), nl.size());
      }
    }
    self->_oscBuffer.clear();
    self->_oscCommand = -1;
  }

  return 1;
}

//=============================================================================
// Card dormancy — suspend off-screen cards, reactivate when scrolled back
//=============================================================================

bool GPUScreenImpl::isCardSlotVisibleInBuffer(uint32_t slotIndex, int excludeRow) const {
  if (!_visibleBuffer)
    return false;

  const Cell *cells = _visibleBuffer->data();
  const uint8_t targetR = static_cast<uint8_t>(slotIndex & 0xFF);
  const uint8_t targetG = static_cast<uint8_t>((slotIndex >> 8) & 0xFF);
  const uint8_t targetB = static_cast<uint8_t>((slotIndex >> 16) & 0xFF);

  for (int row = 0; row < _rows; row++) {
    if (row == excludeRow)
      continue;
    size_t base = static_cast<size_t>(row) * _cols;
    for (int col = 0; col < _cols; col++) {
      const Cell &c = cells[base + col];
      if (isCardGlyph(c.glyph) &&
          c.fgR == targetR &&
          c.fgG == targetG &&
          c.fgB == targetB) {
        return true;
      }
    }
  }
  return false;
}

void GPUScreenImpl::suspendOffscreenCards(int scrolledRow) {
  if (!_visibleBuffer)
    return;

  // Scan the row being pushed to scrollback for ANY card glyph cell.
  // Collect unique slotIndices found on this row.
  size_t srcOffset = static_cast<size_t>(scrolledRow * _cols);
  std::unordered_set<uint32_t> slotsOnRow;

  for (int col = 0; col < _cols; col++) {
    const Cell &cell = (*_visibleBuffer)[srcOffset + col];
    if (!isCardGlyph(cell.glyph))
      continue;

    uint32_t slotIndex = static_cast<uint32_t>(cell.fgR) |
                         (static_cast<uint32_t>(cell.fgG) << 8) |
                         (static_cast<uint32_t>(cell.fgB) << 16);
    slotsOnRow.insert(slotIndex);
  }

  for (uint32_t slotIndex : slotsOnRow) {
    // Only consider cards we're actually tracking
    if (_cards.find(slotIndex) == _cards.end())
      continue;

    // Check if this card still has cells visible on OTHER rows in the buffer
    // (exclude the row being scrolled out — it's about to leave)
    if (isCardSlotVisibleInBuffer(slotIndex, scrolledRow)) {
      // Card still has visible cells on other rows — don't suspend yet.
      continue;
    }

    // Card is fully off-screen — suspend and move to inactive
    auto it = _cards.find(slotIndex);
    if (it != _cards.end()) {
      yinfo("GPUScreen::suspendOffscreenCards: suspending card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      it->second->suspend();
      _inactiveCards[slotIndex] = std::move(it->second);
      _cards.erase(it);
    }
  }
}

void GPUScreenImpl::reactivateVisibleCards() {
  if (_inactiveCards.empty())
    return;

  // Scan the composed view buffer for card glyphs that belong to inactive cards.
  // If found, move them back to _cards so update() runs and reconstructs buffers.
  const Cell *cells = getCellData();
  if (!cells)
    return;

  const size_t numCells = static_cast<size_t>(_rows) * _cols;

  // Collect slotIndices to reactivate (can't modify _inactiveCards during iteration)
  std::vector<uint32_t> toReactivate;

  for (size_t i = 0; i < numCells; i++) {
    if (!isCardGlyph(cells[i].glyph))
      continue;

    uint32_t slotIndex = static_cast<uint32_t>(cells[i].fgR) |
                         (static_cast<uint32_t>(cells[i].fgG) << 8) |
                         (static_cast<uint32_t>(cells[i].fgB) << 16);

    if (_inactiveCards.count(slotIndex)) {
      toReactivate.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toReactivate) {
    auto it = _inactiveCards.find(slotIndex);
    if (it != _inactiveCards.end()) {
      yinfo("GPUScreen::reactivateVisibleCards: reactivating card slotIndex={}", slotIndex);
      _cards[slotIndex] = std::move(it->second);
      _inactiveCards.erase(it);
    }
  }
}

void GPUScreenImpl::gcInactiveCards() {
  if (_inactiveCards.empty())
    return;

  // Collect all card slotIndices referenced in visible buffer + scrollback
  std::unordered_set<uint32_t> liveSlots;

  // Scan visible buffer
  if (_visibleBuffer) {
    const Cell *cells = _visibleBuffer->data();
    const size_t numCells = static_cast<size_t>(_rows) * _cols;
    for (size_t i = 0; i < numCells; i++) {
      if (isCardGlyph(cells[i].glyph)) {
        uint32_t slot = static_cast<uint32_t>(cells[i].fgR) |
                        (static_cast<uint32_t>(cells[i].fgG) << 8) |
                        (static_cast<uint32_t>(cells[i].fgB) << 16);
        liveSlots.insert(slot);
      }
    }
  }

  // Scan scrollback
  for (const auto &line : _scrollback) {
    for (const auto &cell : line.cells) {
      if (isCardGlyph(cell.glyph)) {
        uint32_t slot = static_cast<uint32_t>(cell.fgR) |
                        (static_cast<uint32_t>(cell.fgG) << 8) |
                        (static_cast<uint32_t>(cell.fgB) << 16);
        liveSlots.insert(slot);
      }
    }
  }

  // Dispose inactive cards not referenced anywhere
  std::vector<uint32_t> toDispose;
  for (const auto &[slotIndex, card] : _inactiveCards) {
    if (liveSlots.find(slotIndex) == liveSlots.end()) {
      toDispose.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toDispose) {
    auto it = _inactiveCards.find(slotIndex);
    if (it != _inactiveCards.end()) {
      yinfo("GPUScreen::gcInactiveCards: disposing orphaned inactive card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      it->second->dispose();
      _inactiveCards.erase(it);
    }
  }

  // Also dispose active cards not referenced anywhere
  std::vector<uint32_t> toDisposeActive;
  for (const auto &[slotIndex, card] : _cards) {
    if (liveSlots.find(slotIndex) == liveSlots.end()) {
      toDisposeActive.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toDisposeActive) {
    auto it = _cards.find(slotIndex);
    if (it != _cards.end()) {
      yinfo("GPUScreen::gcInactiveCards: disposing orphaned active card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      it->second->dispose();
      _cards.erase(it);
    }
  }

  if (!toDispose.empty() || !toDisposeActive.empty()) {
    yinfo("GPUScreen::gcInactiveCards: disposed {} inactive + {} active orphaned cards",
          toDispose.size(), toDisposeActive.size());
  }
}

//=============================================================================
// OSC Sequence Handling
//=============================================================================

bool GPUScreenImpl::handleOSCSequence(const std::string &sequence,
                                      std::string *response,
                                      uint32_t *linesToAdvance) {
  yinfo("GPUScreenImpl::handleOSCSequence: ENTERED, sequence len={}",
        sequence.size());

  // Parse vendor ID from sequence (format: "vendorId;...")
  size_t semicolon = sequence.find(';');
  if (semicolon == std::string::npos) {
    yerror("GPUScreenImpl::handleOSCSequence: no semicolon found!");
    if (response)
      *response = OscResponse::error("Invalid OSC sequence: no vendor ID");
    return false;
  }

  int vendorId = 0;
  try {
    vendorId = std::stoi(sequence.substr(0, semicolon));
  } catch (...) {
    yerror("GPUScreenImpl::handleOSCSequence: failed to parse vendor ID");
    if (response)
      *response = OscResponse::error("Invalid vendor ID");
    return false;
  }

  if (vendorId != YETTY_OSC_VENDOR_ID) {
    yerror("GPUScreenImpl::handleOSCSequence: unknown vendor ID {}", vendorId);
    if (response)
      *response = OscResponse::error("Unknown vendor ID");
    return false;
  }

  return handleCardOSCSequence(sequence, response, linesToAdvance);
}

//=============================================================================
// Card OSC Handling
//=============================================================================

bool GPUScreenImpl::handleCardOSCSequence(const std::string &sequence,
                                          std::string *response,
                                          uint32_t *linesToAdvance) {
  yinfo("GPUScreenImpl::handleCardOSCSequence: ENTERED with sequence length={}",
        sequence.size());
  yinfo("GPUScreenImpl::handleCardOSCSequence: sequence={}",
        sequence.substr(0, 200));

  if (!_ctx.cardFactory) {
    yerror("GPUScreenImpl::handleCardOSCSequence: no CardFactory!");
    if (response)
      *response = OscResponse::error("No CardFactory");
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: cardFactory is available");

  // Parse using the same OSC parser (same argument format)
  auto parseResult = _oscParser.parse(sequence);
  if (!parseResult) {
    yerror("GPUScreenImpl::handleCardOSCSequence: parse FAILED: {}",
           error_msg(parseResult));
    if (response)
      *response = OscResponse::error(error_msg(parseResult));
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: parse succeeded");

  OscCommand cmd = *parseResult;
  if (!cmd.isValid()) {
    yerror("GPUScreenImpl::handleCardOSCSequence: cmd invalid: {}", cmd.error);
    if (response)
      *response = OscResponse::error(cmd.error);
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: cmd valid, type={}, plugin='{}'",
        static_cast<int>(cmd.type), cmd.create.plugin);

  switch (cmd.type) {
  case OscCommandType::Create: {
    yinfo(
        "GPUScreenImpl::handleCardOSCSequence: CREATE command for plugin '{}'",
        cmd.create.plugin);
    int32_t x = cmd.create.x;
    int32_t y = cmd.create.y;

    ydebug("Card OSC Create: cmd.x={} cmd.y={} relative={}", x, y,
           cmd.create.relative);

    if (cmd.create.relative) {
      x += getCursorCol();
      y += getCursorRow();
      ydebug("Card OSC Create: final position ({},{})", x, y);
    }

    // Run GC before creation to free resources from orphaned inactive cards
    gcInactiveCards();

    // Resolve "stretch to edge" dimensions (0 = fill remaining terminal)
    // vterm handles scrolling automatically when writing past the bottom
    uint32_t widthCells = static_cast<uint32_t>(cmd.create.width);
    uint32_t heightCells = static_cast<uint32_t>(cmd.create.height);
    if (widthCells == 0 && _cols > 0) {
      widthCells = static_cast<uint32_t>(_cols);
    }
    if (heightCells == 0 && _rows > 0) {
      heightCells = static_cast<uint32_t>(_rows);
    }
    ydebug("Card OSC Create: resolved size {}x{} (terminal {}x{})", widthCells, heightCells, _cols, _rows);

    // Create the card (pass full YettyContext for font access etc)
    auto result = _ctx.cardFactory->createCard(
        _ctx, cmd.create.plugin, x, y, widthCells, heightCells,
        cmd.pluginArgs, cmd.payload);

    if (!result) {
      yerror("GPUScreen: createCard failed: {}", error_msg(result));
      if (response)
        *response = OscResponse::error(error_msg(result));
      return false;
    }

    CardPtr cardPtr = std::move(*result);
    Card *card = cardPtr.get();

    // =====================================================================
    // Generate ANSI escape sequences for card cells
    // =====================================================================
    // Instead of directly writing to the grid buffer (which breaks scrolling),
    // we generate standard ANSI true-color escape sequences that flow through
    // vterm's normal line handling. This ensures:
    //   - Proper scrolling behavior
    //   - Correct scrollback buffer management
    //   - Widget cells move naturally with terminal content
    //
    // Encoding (fits in 24-bit ANSI true-color):
    //   fg RGB: metadata slot index (offset/32) - addresses 512MB
    //   bg RGB: (relRow << 12) | relCol - supports 4096x4096 widgets
    // =====================================================================

    if (vtermInputCallback_) {
      std::string ansiOutput;
      ansiOutput.reserve(card->widthCells() * card->heightCells() *
                         40); // Estimate

      uint32_t slotIndex = card->metadataSlotIndex();
      uint32_t shaderGlyph = card->shaderGlyph();
      uint32_t metaOffset = card->metadataOffset();

      ydebug("Card ANSI gen: type='{}' slotIndex={} shaderGlyph={:#x} metaOffset={} size={}x{}",
             card->typeName(), slotIndex, shaderGlyph, metaOffset,
             card->widthCells(), card->heightCells());

      // UTF-8 encode the shader glyph (Plane 16: U+100000 range)
      // For codepoints >= 0x10000: 4-byte UTF-8
      char glyphUtf8[5];
      glyphUtf8[0] = static_cast<char>(0xF0 | ((shaderGlyph >> 18) & 0x07));
      glyphUtf8[1] = static_cast<char>(0x80 | ((shaderGlyph >> 12) & 0x3F));
      glyphUtf8[2] = static_cast<char>(0x80 | ((shaderGlyph >> 6) & 0x3F));
      glyphUtf8[3] = static_cast<char>(0x80 | (shaderGlyph & 0x3F));
      glyphUtf8[4] = '\0';

      // fg color from slot index (24 bits)
      uint8_t fgR = slotIndex & 0xFF;
      uint8_t fgG = (slotIndex >> 8) & 0xFF;
      uint8_t fgB = (slotIndex >> 16) & 0xFF;

      ydebug("Card ANSI gen: fg=({},{},{}) for slotIndex={}", fgR, fgG, fgB, slotIndex);

      // ESC character - use explicit char to avoid any fmt::format issues
      const char ESC = '\033'; // 0x1B

      // Start with newline + reset to ensure parser is in ground state
      // The OSC ST (ESC \) may leave parser expecting more, so we
      // need to ensure we start fresh. The newline puts us on a new
      // line and resets parser state.
      // ansiOutput += "\n";
      ansiOutput += ESC;
      ansiOutput += "[0m";

      for (uint32_t row = 0; row < card->heightCells(); row++) {
        // Carriage return to start at column 0
        ansiOutput += '\r';

        for (uint32_t col = 0; col < card->widthCells(); col++) {
          // bg encodes relative position: (row << 12) | col
          uint32_t bg24 = (row << 12) | col;
          uint8_t bgR = bg24 & 0xFF;
          uint8_t bgG = (bg24 >> 8) & 0xFF;
          uint8_t bgB = (bg24 >> 16) & 0xFF;

          // ANSI true-color escape sequences
          // ESC[38;2;R;G;Bm - set fg color
          // ESC[48;2;R;G;Bm - set bg color
          ansiOutput += ESC;
          ansiOutput += fmt::format("[38;2;{};{};{}m", fgR, fgG, fgB);
          ansiOutput += ESC;
          ansiOutput += fmt::format("[48;2;{};{};{}m", bgR, bgG, bgB);
          ansiOutput += glyphUtf8;
        }
        // Reset attributes and newline - vterm handles scrolling!
        ansiOutput += ESC;
        ansiOutput += "[0m\n";
      }

      yinfo("GPUScreen: Writing {} bytes of ANSI sequences to vterm for card "
            "'{}'",
            ansiOutput.size(), card->typeName());

      // Write to vterm input - this flows through normal terminal processing
      vtermInputCallback_(ansiOutput.c_str(), ansiOutput.size());
    } else {
      yerror("GPUScreen: No vtermInputCallback_ set, cannot create card");
      return false;
    }

    yinfo("GPUScreen: Created card '{}' at ({},{}) size {}x{} slotIndex={} "
          "shaderGlyph={:#x}",
          card->typeName(), x, y, card->widthCells(), card->heightCells(),
          card->metadataSlotIndex(), card->shaderGlyph());

    // Store and register the card (transfer ownership)
    registerCard(std::move(cardPtr));

    // Don't set linesToAdvance - vterm handles cursor movement via the newlines
    // we output The ANSI sequences include newlines which advance the cursor
    // naturally

    _hasDamage = true;
    return true;
  }

  case OscCommandType::Plugins: {
    if (response) {
      *response = OscResponse::pluginList(_ctx.cardFactory->getRegisteredCards());
    }
    return true;
  }

  case OscCommandType::List: {
    if (response) {
      std::vector<
          std::tuple<std::string, std::string, int, int, int, int, bool>>
          cardList;
      for (const auto &[slotIndex, card] : _cards) {
        cardList.emplace_back(
            std::to_string(slotIndex), card->typeName(),
            card->x(), card->y(),
            static_cast<int>(card->widthCells()),
            static_cast<int>(card->heightCells()), true);
      }
      for (const auto &[slotIndex, card] : _inactiveCards) {
        cardList.emplace_back(
            std::to_string(slotIndex), card->typeName(),
            card->x(), card->y(),
            static_cast<int>(card->widthCells()),
            static_cast<int>(card->heightCells()), false);
      }
      *response = OscResponse::cardList(cardList);
    }
    return true;
  }

  case OscCommandType::Kill: {
    if (cmd.target.id.empty()) {
      if (response)
        *response = OscResponse::error("kill: --id required");
      return false;
    }
    uint32_t slotIndex = 0;
    try { slotIndex = static_cast<uint32_t>(std::stoul(cmd.target.id)); }
    catch (...) {
      if (response)
        *response = OscResponse::error("kill: invalid card id");
      return false;
    }
    auto it = _cards.find(slotIndex);
    if (it == _cards.end())
      it = _inactiveCards.find(slotIndex);
    if (it == _cards.end() && _inactiveCards.find(slotIndex) == _inactiveCards.end()) {
      if (response)
        *response = OscResponse::error("Card not found: " + cmd.target.id);
      return false;
    }
    // Find in active or inactive and dispose
    {
      auto ait = _cards.find(slotIndex);
      if (ait != _cards.end()) {
        ait->second->dispose();
        _cards.erase(ait);
      } else {
        auto iit = _inactiveCards.find(slotIndex);
        if (iit != _inactiveCards.end()) {
          iit->second->dispose();
          _inactiveCards.erase(iit);
        }
      }
    }
    return true;
  }

  default:
    if (response)
      *response = OscResponse::error("Card: unknown or unsupported command");
    return false;
  }
}

//=============================================================================
// Card Registry Management
//=============================================================================

void GPUScreenImpl::registerCard(CardPtr card) {
  if (!card)
    return;

  card->setScreenOrigin(_viewportX, _viewportY);
  card->setCellSize(getCellWidth(), getCellHeight());
  uint32_t slotIndex = card->metadataSlotIndex();
  ydebug("GPUScreenImpl::registerCard: registered card at slotIndex {}", slotIndex);
  _cards[slotIndex] = std::move(card);
}

void GPUScreenImpl::unregisterCard(uint32_t slotIndex) {
  _cards.erase(slotIndex);
  ydebug("GPUScreenImpl::unregisterCard: unregistered card at slotIndex {}",
         slotIndex);
}

Card *GPUScreenImpl::getCardBySlotIndex(uint32_t offset) const {
  auto it = _cards.find(offset);
  return (it != _cards.end()) ? it->second.get() : nullptr;
}

Card *GPUScreenImpl::getCardAtCell(int row, int col) const {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
    return nullptr;
  }

  if (!_visibleBuffer)
    return nullptr;

  size_t idx = cellIndex(row, col);
  if (idx >= _visibleBuffer->size())
    return nullptr;

  const Cell &cell = (*_visibleBuffer)[idx];

  // Check if this cell has a shader glyph (Plane 16 PUA-B: U+100000 - U+10FFFD)
  if (cell.glyph < 0x100000 || cell.glyph > 0x10FFFD) {
    return nullptr;
  }

  // The fg RGB channels encode the metadata slot index (3 bytes = 24 bits)
  uint32_t slotIndex = static_cast<uint32_t>(cell.fgR) |
                       (static_cast<uint32_t>(cell.fgG) << 8) |
                       (static_cast<uint32_t>(cell.fgB) << 16);

  return getCardBySlotIndex(slotIndex);
}

//=============================================================================
// Interface method implementations
//=============================================================================

uint32_t GPUScreenImpl::getCols() const { return static_cast<uint32_t>(_cols); }

uint32_t GPUScreenImpl::getRows() const { return static_cast<uint32_t>(_rows); }

void GPUScreenImpl::sendKey(uint32_t codepoint, int mods) {
  if (!_vterm)
    return;

  // Convert GLFW modifiers to VTermModifier
  // GLFW: GLFW_MOD_SHIFT=0x0001, GLFW_MOD_CONTROL=0x0002, GLFW_MOD_ALT=0x0004
  VTermModifier vtMods = VTERM_MOD_NONE;
  if (mods & 0x0001)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_SHIFT);
  if (mods & 0x0002)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_CTRL);
  if (mods & 0x0004)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_ALT);

  vterm_keyboard_unichar(_vterm, codepoint, vtMods);
}

void GPUScreenImpl::sendSpecialKey(int key, int mods) {
  if (!_vterm)
    return;

  ydebug("sendSpecialKey: key={} mods={}", key, mods);

  // Convert GLFW modifiers to VTermModifier
  // GLFW: GLFW_MOD_SHIFT=0x0001, GLFW_MOD_CONTROL=0x0002, GLFW_MOD_ALT=0x0004
  VTermModifier vtMods = VTERM_MOD_NONE;
  if (mods & 0x0001)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_SHIFT);
  if (mods & 0x0002)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_CTRL);
  if (mods & 0x0004)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_ALT);

  // Convert GLFW key to VTermKey
  VTermKey vtKey = VTERM_KEY_NONE;
  switch (key) {
  case 257:
    vtKey = VTERM_KEY_ENTER;
    break; // GLFW_KEY_ENTER
  case 259:
    vtKey = VTERM_KEY_BACKSPACE;
    break; // GLFW_KEY_BACKSPACE
  case 258:
    vtKey = VTERM_KEY_TAB;
    break; // GLFW_KEY_TAB
  case 256:
    vtKey = VTERM_KEY_ESCAPE;
    break; // GLFW_KEY_ESCAPE
  case 265:
    vtKey = VTERM_KEY_UP;
    break; // GLFW_KEY_UP
  case 264:
    vtKey = VTERM_KEY_DOWN;
    break; // GLFW_KEY_DOWN
  case 263:
    vtKey = VTERM_KEY_LEFT;
    break; // GLFW_KEY_LEFT
  case 262:
    vtKey = VTERM_KEY_RIGHT;
    break; // GLFW_KEY_RIGHT
  case 268:
    vtKey = VTERM_KEY_HOME;
    break; // GLFW_KEY_HOME
  case 269:
    vtKey = VTERM_KEY_END;
    break; // GLFW_KEY_END
  case 266:
    vtKey = VTERM_KEY_PAGEUP;
    break; // GLFW_KEY_PAGE_UP
  case 267:
    vtKey = VTERM_KEY_PAGEDOWN;
    break; // GLFW_KEY_PAGE_DOWN
  case 260:
    vtKey = VTERM_KEY_INS;
    break; // GLFW_KEY_INSERT
  case 261:
    vtKey = VTERM_KEY_DEL;
    break; // GLFW_KEY_DELETE
  case 290:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(1));
    break; // GLFW_KEY_F1
  case 291:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(2));
    break; // GLFW_KEY_F2
  case 292:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(3));
    break; // GLFW_KEY_F3
  case 293:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(4));
    break; // GLFW_KEY_F4
  case 294:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(5));
    break; // GLFW_KEY_F5
  case 295:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(6));
    break; // GLFW_KEY_F6
  case 296:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(7));
    break; // GLFW_KEY_F7
  case 297:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(8));
    break; // GLFW_KEY_F8
  case 298:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(9));
    break; // GLFW_KEY_F9
  case 299:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(10));
    break; // GLFW_KEY_F10
  case 300:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(11));
    break; // GLFW_KEY_F11
  case 301:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(12));
    break; // GLFW_KEY_F12
  default:
    // Handle Ctrl/Alt + character combinations
    // GLFW key codes: SPACE=32, 0-9=48-57, A-Z=65-90
    if (mods & (0x0002 | 0x0004)) { // CTRL or ALT
      uint32_t ch = 0;
      if (key == 32) { // GLFW_KEY_SPACE
        ch = ' ';
      } else if (key >= 48 && key <= 57) { // GLFW_KEY_0 - GLFW_KEY_9
        ch = static_cast<uint32_t>(key);
      } else if (key >= 65 && key <= 90) {    // GLFW_KEY_A - GLFW_KEY_Z
        ch = static_cast<uint32_t>(key + 32); // lowercase
      }
      if (ch != 0) {
        ydebug("sendSpecialKey: sending ctrl/alt char '{}' ({})", (char)ch, ch);
        vterm_keyboard_unichar(_vterm, ch, vtMods);
      }
    }
    return;
  }

  vterm_keyboard_key(_vterm, vtKey, vtMods);
}

void GPUScreenImpl::setOutputCallback(OutputCallback cb) {
  _outputCallback = std::move(cb);
  if (_vterm && _outputCallback) {
    vterm_output_set_callback(
        _vterm,
        [](const char *data, size_t len, void *user) {
          auto *self = static_cast<GPUScreenImpl *>(user);
          if (self->_outputCallback) {
            self->_outputCallback(data, len);
          }
        },
        this);
  }
}

void GPUScreenImpl::registerForFocus() {
  auto loop = *base::EventLoop::instance();
  loop->registerListener(base::Event::Type::SetFocus,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseDown,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseUp,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseMove,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::Scroll,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::Paste,
                         sharedAs<base::EventListener>());
  yinfo("GPUScreen {} registered for SetFocus, Mouse, Scroll and Paste events", _id);

  // Auto-focus on startup so keyboard works immediately
  loop->dispatch(base::Event::focusEvent(_id));
}

Result<bool> GPUScreenImpl::onEvent(const base::Event &event) {
  // Handle mouse click - dispatch SetFocus if clicked on our surface
  if (event.type == base::Event::Type::MouseDown) {
    float mx = event.mouse.x;
    float my = event.mouse.y;
    int button = event.mouse.button;
    ydebug("GPUScreen {} MouseDown at ({}, {}), button={}, viewport=({},{} {}x{})", _id,
           mx, my, button, _viewportX, _viewportY, _viewportWidth, _viewportHeight);

    // Only check bounds if viewport is set
    if (_viewportWidth > 0 && _viewportHeight > 0) {
      // Check if click is within our viewport
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        // Right-click: open context menu
        if (button == 1 && _ctx.imguiManager) {  // GLFW_MOUSE_BUTTON_RIGHT = 1
          // Calculate cell position from mouse coordinates
          float localX = mx - _viewportX;
          float localY = my - _viewportY;
          int col = static_cast<int>(localX / getCellWidth());
          int row = static_cast<int>(localY / getCellHeight());

          // Clamp to valid range
          col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
          row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

          // Add context menu items - menu opens automatically on render
          _ctx.imguiManager->addContextMenuItem({
            "Copy cell info",
            base::Event::contextMenuAction(_id, "copy_cell_info", row, col)
          });
          _ctx.imguiManager->addContextMenuItem({
            "Inspect glyph",
            base::Event::contextMenuAction(_id, "inspect_glyph", row, col)
          });

          yinfo("GPUScreen {} right-click at cell ({}, {}), opening context menu", _id, row, col);
          return Ok(true);
        }

        // Left-click: focus + start selection or card interaction
        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        float cellWidthF = _baseCellWidth * _zoomLevel;
        float cellHeightF = _baseCellHeight * _zoomLevel;
        int col = static_cast<int>(localX / cellWidthF);
        int row = static_cast<int>(localY / cellHeightF);
        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

        auto loop = *base::EventLoop::instance();

        // Check if clicked cell is a card
        Card* card = getCardAtCell(row, col);
        if (card) {
          clearSelection();
          yinfo("GPUScreen {} clicked on card '{}' (id={}) at cell ({},{}), dispatching SetFocus",
                _id, card->typeName(), card->id(), row, col);
          loop->dispatch(base::Event::focusEvent(card->id()));
          float cardX, cardY;
          cardLocalCoords(localX, localY, row, col, cardX, cardY);
          _cardMouseTarget = card->id();
          loop->dispatch(base::Event::cardMouseDown(card->id(), cardX, cardY, button));
        } else if (button == 0) {
          // Left-click on terminal: start text selection
          _selecting = true;
          _hasSelection = false;
          _selStartCol = col;
          _selStartRow = row;
          _selEndCol = col;
          _selEndRow = row;
          _hasDamage = true;
          loop->dispatch(base::Event::focusEvent(_id));
        } else {
          loop->dispatch(base::Event::focusEvent(_id));
        }
        return Ok(true);
      }
    }
    return Ok(false);
  }

  // Handle mouse move - update statusbar and forward to focused card
  if (event.type == base::Event::Type::MouseMove) {
    float mx = event.mouse.x;
    float my = event.mouse.y;

    if (_viewportWidth > 0 && _viewportHeight > 0) {
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        int col = static_cast<int>(localX / getCellWidth());
        int row = static_cast<int>(localY / getCellHeight());

        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

        if (_ctx.imguiManager) {
          Cell cell = getCell(row, col);
          char statusBuf[256];
          snprintf(statusBuf, sizeof(statusBuf),
                   "Row:%d Col:%d Glyph:U+%04X FG:(%d,%d,%d) BG:(%d,%d,%d) Style:0x%02X",
                   row, col, cell.glyph,
                   cell.fgR, cell.fgG, cell.fgB,
                   cell.bgR, cell.bgG, cell.bgB,
                   cell.style);
          _ctx.imguiManager->setStatusText(statusBuf);
        }

        // Extend text selection while dragging
        if (_selecting) {
          _selEndCol = col;
          _selEndRow = row;
          _hasSelection = (_selStartCol != _selEndCol || _selStartRow != _selEndRow);
          _hasDamage = true;
        }

        // Forward mouse move to active card with local coordinates
        if (_cardMouseTarget != 0) {
          Card* card = getCardAtCell(row, col);
          if (card && card->id() == _cardMouseTarget) {
            float cardX, cardY;
            cardLocalCoords(localX, localY, row, col, cardX, cardY);
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::cardMouseMove(_cardMouseTarget, cardX, cardY));
          } else {
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::cardMouseMove(_cardMouseTarget, localX, localY));
          }
        }

      }
    }
    return Ok(false);  // Don't consume
  }

  // Handle mouse up - finalize selection or dispatch CardMouseUp
  if (event.type == base::Event::Type::MouseUp) {
    // Finalize text selection on left button release
    if (_selecting && event.mouse.button == 0) {
      _selecting = false;
      if (_hasSelection) {
        std::string text = extractSelectedText();
        if (!text.empty()) {
          auto payload = std::make_shared<std::string>(std::move(text));
          auto loop = *base::EventLoop::instance();
          loop->dispatch(base::Event::copyEvent(std::move(payload)));
          ydebug("GPUScreen {}: selection copied ({} bytes)", _id, text.size());
        }
      }
      return Ok(true);
    }

    if (_cardMouseTarget != 0) {
      float mx = event.mouse.x;
      float my = event.mouse.y;
      int button = event.mouse.button;
      float localX = mx - _viewportX;
      float localY = my - _viewportY;
      int col = static_cast<int>(localX / getCellWidth());
      int row = static_cast<int>(localY / getCellHeight());
      col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
      row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

      float cardX, cardY;
      Card* card = getCardAtCell(row, col);
      if (card && card->id() == _cardMouseTarget) {
        cardLocalCoords(localX, localY, row, col, cardX, cardY);
      } else {
        cardX = localX;
        cardY = localY;
      }
      auto loop = *base::EventLoop::instance();
      loop->dispatch(base::Event::cardMouseUp(_cardMouseTarget, cardX, cardY, button));
      _cardMouseTarget = 0;
      return Ok(true);
    }
    return Ok(false);
  }

  // Handle scroll - scrollback or zoom (only when focused)
  if (_focused && event.type == base::Event::Type::Scroll) {
    float mx = event.scroll.x;
    float my = event.scroll.y;

    if (_viewportWidth > 0 && _viewportHeight > 0) {
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        // Check if scroll is over a card - forward as CardScroll
        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        int col = static_cast<int>(localX / getCellWidth());
        int row = static_cast<int>(localY / getCellHeight());
        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));
        Card* scrollCard = getCardAtCell(row, col);
        if (scrollCard) {
          float cardX, cardY;
          cardLocalCoords(localX, localY, row, col, cardX, cardY);
          auto loop = *base::EventLoop::instance();
          loop->dispatch(base::Event::cardScrollEvent(
              scrollCard->id(), cardX, cardY, event.scroll.dx, event.scroll.dy, event.scroll.mods));
          return Ok(true);
        }

        bool ctrlPressed = (event.scroll.mods & 0x0002) != 0;  // GLFW_MOD_CONTROL

        if (ctrlPressed) {
          // Ctrl+scroll: zoom
          float newZoom = _zoomLevel + event.scroll.dy * 0.02f;
          newZoom = std::max(0.2f, std::min(newZoom, 5.0f));
          if (newZoom != _zoomLevel) {
            _zoomLevel = newZoom;
            // Recalculate cols/rows from viewport and new zoom
            setViewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
            // Notify cards of new cell size
            for (auto& [slotIndex, card] : _cards) {
              card->setCellSize(getCellWidth(), getCellHeight());
            }
            yinfo("GPUScreen {} zoom: {:.0f}%", _id, _zoomLevel * 100.0f);
          }
        } else {
          // Normal scroll: scrollback
          int lines = static_cast<int>(event.scroll.dy * 3);  // 3 lines per notch
          if (lines > 0) {
            scrollUp(lines);
          } else if (lines < 0) {
            scrollDown(-lines);
          }
        }
        return Ok(true);  // Consume
      }
    }
    return Ok(false);
  }

  if (event.type == base::Event::Type::SetFocus) {
    auto loop = *base::EventLoop::instance();
    if (event.setFocus.objectId == _id) {
      // This GPUScreen is being focused
      if (!_focused) {
        _focused = true;
        // Register for keyboard events
        loop->registerListener(base::Event::Type::Char,
                               sharedAs<base::EventListener>());
        loop->registerListener(base::Event::Type::KeyDown,
                               sharedAs<base::EventListener>());
        yinfo("GPUScreen {} gained focus, registered for keyboard events", _id);
      }
    } else {
      // Another element is being focused, we lose focus
      if (_focused) {
        _focused = false;
        // Deregister from keyboard events (but keep SetFocus registration)
        loop->deregisterListener(base::Event::Type::Char,
                                 sharedAs<base::EventListener>());
        loop->deregisterListener(base::Event::Type::KeyDown,
                                 sharedAs<base::EventListener>());
        yinfo("GPUScreen {} lost focus, deregistered from keyboard events",
              _id);
      }
    }
    // Don't consume - all GPUScreens need to see SetFocus
    return Ok(false);
  }

  // Handle Paste event — write pasted text to PTY
  if (event.type == base::Event::Type::Paste && _focused && event.payload) {
    auto text = std::static_pointer_cast<std::string>(event.payload);
    if (text && !text->empty() && _outputCallback) {
      _outputCallback(text->c_str(), text->size());
      ydebug("GPUScreen {}: pasted {} bytes", _id, text->size());
    }
    return Ok(true);
  }

  // Handle keyboard events (only when focused)
  if (_focused) {
    // When scrolled back, Enter exits scrollback mode (like tmux copy mode)
    if (_scrollOffset > 0) {
      if (event.type == base::Event::Type::KeyDown && event.key.key == 257) {
        ydebug("GPUScreen::onEvent: Enter pressed in scrollback - scrolling to bottom");
        scrollToBottom();
        return Ok(true);
      }
    }

    // Clear selection on any keyboard input
    if (_hasSelection &&
        (event.type == base::Event::Type::Char || event.type == base::Event::Type::KeyDown)) {
      clearSelection();
    }

    if (event.type == base::Event::Type::Char) {
      ydebug("GPUScreen::onEvent: Char codepoint={} mods={}",
             event.chr.codepoint, event.chr.mods);
      sendKey(event.chr.codepoint, event.chr.mods);
      return Ok(true);
    }

    if (event.type == base::Event::Type::KeyDown) {
      ydebug("GPUScreen::onEvent: KeyDown key={} mods={}", event.key.key,
             event.key.mods);
      sendSpecialKey(event.key.key, event.key.mods);
      return Ok(true);
    }
  }

  return Ok(false);
}

Result<void> GPUScreenImpl::initPipeline() {
  if (_pipelineInitialized)
    return Ok();

  WGPUDevice device = _ctx.gpu.device;
  yinfo("GPUScreenImpl::initPipeline: initializing per-instance resources");

  // Verify ShaderManager has shared resources ready
  auto shaderMgr = _ctx.shaderManager;
  if (!shaderMgr->getGridPipeline() || !shaderMgr->getGridBindGroupLayout()) {
    return Err<void>("ShaderManager not initialized - shared pipeline "
                     "resources not available");
  }

  // Create per-instance uniform buffer only
  WGPUBufferDescriptor uniformDesc = {};
  uniformDesc.label = WGPU_STR("grid uniforms");
  uniformDesc.size = sizeof(Uniforms);
  uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
  _uniformBuffer = wgpuDeviceCreateBuffer(device, &uniformDesc);
  if (!_uniformBuffer) {
    return Err<void>("Failed to create uniform buffer");
  }

  _pipelineInitialized = true;
  yinfo("GPUScreenImpl::initPipeline: per-instance resources initialized");
  return Ok();
}

Result<void> GPUScreenImpl::render(WGPURenderPassEncoder pass) {
  // Reactivate any suspended cards whose cells are now visible (user scrolled back)
  reactivateVisibleCards();

  // Periodic GC: dispose inactive cards no longer referenced in any buffer
  if (!_inactiveCards.empty() && ++_gcFrameCounter >= 180) {
    _gcFrameCounter = 0;
    gcInactiveCards();
  }

  // Update all active cards before rendering (suspended cards are in _inactiveCards)
  for (auto& [slotIndex, card] : _cards) {
    ydebug("GPUScreen::render: updating card '{}' slotIndex={} metaOffset={}",
           card->typeName(), slotIndex, card->metadataOffset());
    if (auto res = card->update(0.0f); !res) {
      yerror("GPUScreen::render: card '{}' update failed: {}", card->typeName(), error_msg(res));
      return Err<void>("GPUScreen::render: card update failed", res);
    }
  }

  // Flush CardManager (packs atlas, updates bind group if needed, uploads buffers)
  if (_cardManager) {
    if (auto res = _cardManager->flush(_ctx.gpu.queue); !res) {
      yerror("GPUScreen: CardManager flush failed: {}", error_msg(res));
      return Err<void>("GPUScreen: CardManager flush failed", res);
    }
  }

  const Cell *cells = getCellData();
  if (!cells || _cols == 0 || _rows == 0) {
    return Ok(); // Nothing to render
  }

  // Initialize pipeline on first render
  if (!_pipelineInitialized) {
    if (auto res = initPipeline(); !res) {
      return res;
    }
  }

  WGPUDevice device = _ctx.gpu.device;
  WGPUQueue queue = _ctx.gpu.queue;
  auto shaderMgr = _ctx.shaderManager;

  // Hot-recompile shader if providers are dirty (e.g. new glyph/card enabled)
  shaderMgr->update();

  // Recreate cell buffer if grid size changed
  size_t requiredSize = static_cast<size_t>(_cols) * _rows * sizeof(Cell);
  if (_cols != _textureCols || _rows != _textureRows || !_cellBuffer ||
      _cellBufferSize < requiredSize) {
    if (_cellBuffer) {
      wgpuBufferRelease(_cellBuffer);
      _cellBuffer = nullptr;
    }

    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.label = WGPU_STR("cell buffer");
    bufferDesc.size = requiredSize;
    bufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    _cellBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
    if (!_cellBuffer) {
      return Err<void>("Failed to create cell buffer");
    }
    _cellBufferSize = requiredSize;
    _textureCols = _cols;
    _textureRows = _rows;
    _needsBindGroupRecreation = true;
    yinfo("GPUScreenImpl::render: created cell buffer {}x{} ({} bytes)", _cols,
          _rows, requiredSize);
  }

  // Recreate bind group if needed
  if (_needsBindGroupRecreation || !_bindGroup ||
      (_msdfFont &&
       _msdfFont->getResourceVersion() != _lastFontResourceVersion)) {
    if (_bindGroup) {
      wgpuBindGroupRelease(_bindGroup);
      _bindGroup = nullptr;
    }

    if (!_msdfFont || !_msdfFont->getTextureView() ||
        !_msdfFont->getSampler() || !_msdfFont->getGlyphMetadataBuffer()) {
      return Err<void>("MSDF font resources not ready");
    }
    if (!_bitmapFont || !_bitmapFont->getTextureView() ||
        !_bitmapFont->getSampler() || !_bitmapFont->getMetadataBuffer()) {
      return Err<void>("Bitmap font resources not ready");
    }

    WGPUBindGroupEntry bgEntries[8] = {};

    bgEntries[0].binding = 0;
    bgEntries[0].buffer = _uniformBuffer;
    bgEntries[0].size = sizeof(Uniforms);

    bgEntries[1].binding = 1;
    bgEntries[1].textureView = _msdfFont->getTextureView();

    bgEntries[2].binding = 2;
    bgEntries[2].sampler = _msdfFont->getSampler();

    bgEntries[3].binding = 3;
    bgEntries[3].buffer = _msdfFont->getGlyphMetadataBuffer();
    bgEntries[3].size =
        _msdfFont->getBufferGlyphCount() * sizeof(GlyphMetadataGPU);

    bgEntries[4].binding = 4;
    bgEntries[4].buffer = _cellBuffer;
    bgEntries[4].size = _cellBufferSize;

    bgEntries[5].binding = 5;
    bgEntries[5].textureView = _bitmapFont->getTextureView();

    bgEntries[6].binding = 6;
    bgEntries[6].sampler = _bitmapFont->getSampler();

    bgEntries[7].binding = 7;
    bgEntries[7].buffer = _bitmapFont->getMetadataBuffer();
    uint32_t glyphsPerRow =
        _bitmapFont->getAtlasWidth() / _bitmapFont->getGlyphSize();
    bgEntries[7].size =
        glyphsPerRow * glyphsPerRow * sizeof(BitmapGlyphMetadata);

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = shaderMgr->getGridBindGroupLayout();
    bindGroupDesc.entryCount = 8;
    bindGroupDesc.entries = bgEntries;
    _bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
    if (!_bindGroup) {
      return Err<void>("Failed to create bind group");
    }

    _needsBindGroupRecreation = false;
    if (_msdfFont) {
      _lastFontResourceVersion = _msdfFont->getResourceVersion();
    }
  }

  // Upload cells to GPU
  wgpuQueueWriteBuffer(queue, _cellBuffer, 0, cells,
                       _cols * _rows * sizeof(Cell));

  // Debug: scan uploaded cells for card glyphs
  {
    int cardCellCount = 0;
    for (size_t i = 0; i < static_cast<size_t>(_cols * _rows); i++) {
      const Cell& c = cells[i];
      uint8_t fontType = (c.style >> 5) & 0x07;
      if (fontType == FONT_TYPE_CARD) {
        if (cardCellCount < 3) {
          uint32_t slotIdx = c.fgR | (static_cast<uint32_t>(c.fgG) << 8) | (static_cast<uint32_t>(c.fgB) << 16);
          uint32_t bg24 = c.bgR | (static_cast<uint32_t>(c.bgG) << 8) | (static_cast<uint32_t>(c.bgB) << 16);
          ydebug("render cell[{}]: CARD glyph={:#x} slotIdx={} bg24={:#x} style={:#x}",
                 i, c.glyph, slotIdx, bg24, c.style);
        }
        cardCellCount++;
      }
    }
    if (cardCellCount > 0) {
      ydebug("render: {} card cells in buffer ({}x{})", cardCellCount, _cols, _rows);
    }
  }


  // Precise float cell size for GPU shaders
  float cellWidthF = _baseCellWidth * _zoomLevel;
  float cellHeightF = _baseCellHeight * _zoomLevel;

  // Screen size is grid * cell size
  float screenWidth = static_cast<float>(_cols) * cellWidthF;
  float screenHeight = static_cast<float>(_rows) * cellHeightF;

  // Update uniforms
  _uniforms.projection =
      glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
  _uniforms.screenSize = {screenWidth, screenHeight};
  _uniforms.cellSize = {cellWidthF, cellHeightF};
  _uniforms.gridSize = {static_cast<float>(_cols), static_cast<float>(_rows)};
  _uniforms.pixelRange = _msdfFont ? _msdfFont->getPixelRange() : 2.0f;
  _uniforms.scale = _zoomLevel;
  _uniforms.cursorPos = {static_cast<float>(_cursorCol),
                         static_cast<float>(_cursorRow)};
  _uniforms.cursorVisible = _cursorVisible ? 1.0f : 0.0f;
  _uniforms.cursorShape = static_cast<float>(_cursorShape);
  _uniforms.viewportOrigin = {_viewportX, _viewportY};
  _uniforms.cursorBlink = _cursorBlink ? 1.0f : 0.0f;

  // Selection uniforms — normalize direction so start <= end
  if (_hasSelection) {
    int r0 = _selStartRow, c0 = _selStartCol;
    int r1 = _selEndRow, c1 = _selEndCol;
    if (r0 > r1 || (r0 == r1 && c0 > c1)) {
      std::swap(r0, r1);
      std::swap(c0, c1);
    }
    _uniforms.hasSelection = 1.0f;
    _uniforms.selStart = {static_cast<float>(c0), static_cast<float>(r0)};
    _uniforms.selEnd = {static_cast<float>(c1), static_cast<float>(r1)};
  } else {
    _uniforms.hasSelection = 0.0f;
    _uniforms.selStart = {0.0f, 0.0f};
    _uniforms.selEnd = {0.0f, 0.0f};
  }

  wgpuQueueWriteBuffer(queue, _uniformBuffer, 0, &_uniforms, sizeof(Uniforms));

  // Set viewport and scissor to this terminal's tile bounds
  if (_viewportWidth > 0 && _viewportHeight > 0) {
    wgpuRenderPassEncoderSetViewport(pass, _viewportX, _viewportY,
                                     _viewportWidth, _viewportHeight, 0.0f,
                                     1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass,
        static_cast<uint32_t>(_viewportX), static_cast<uint32_t>(_viewportY),
        static_cast<uint32_t>(_viewportWidth), static_cast<uint32_t>(_viewportHeight));
  }

  // Draw using shared pipeline and quad vertex buffer from ShaderManager
  wgpuRenderPassEncoderSetPipeline(pass, shaderMgr->getGridPipeline());
  // Use per-screen CardManager bind group (uniforms + card buffers),
  // falling back to the minimal shared bind group (uniforms only)
  WGPUBindGroup bindGroup0 = _cardManager
      ? _cardManager->sharedBindGroup()
      : nullptr;
  if (!bindGroup0) bindGroup0 = _ctx.gpu.sharedBindGroup;
  ydebug("render: bindGroup0={} (cardMgr={} sharedBG={} fallback={})",
         (void*)bindGroup0,
         (void*)(_cardManager ? _cardManager->sharedBindGroup() : nullptr),
         (void*)_ctx.gpu.sharedBindGroup,
         !_cardManager || !_cardManager->sharedBindGroup());
  wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup0, 0, nullptr);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, _bindGroup, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(
      pass, 0, shaderMgr->getQuadVertexBuffer(), 0, WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

  return Ok();
}

//=============================================================================
// Factory
//=============================================================================

Result<GPUScreen::Ptr>
GPUScreen::create(const YettyContext &ctx) noexcept {
  auto screen = std::make_shared<GPUScreenImpl>(ctx);
  if (auto res = screen->init(); !res) {
    return Err<Ptr>("GPUScreen init failed", res);
  }
  screen->registerForFocus();
  return Ok<Ptr>(screen);
}

Result<GPUScreen::Ptr> GPUScreen::createForTest(uint32_t cols,
                                                uint32_t rows) noexcept {
  YettyContext nullContext = {}; // Empty context - no GPU, no managers
  auto screen = std::make_shared<GPUScreenImpl>(nullContext);
  // Don't call init() - no font manager in test context
  // Don't register for focus - no event loop in tests
  // Resize to requested dimensions
  screen->resize(cols, rows);
  return Ok<Ptr>(screen);
}

} // namespace yetty
