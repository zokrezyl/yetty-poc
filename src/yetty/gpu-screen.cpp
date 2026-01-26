#include "gpu-screen.h"
#include <yetty/card-factory.h>
#include <yetty/card.h>
#include "widget-factory.h"
#include <algorithm>
#include <cstdlib>
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

// Forward declarations
class Widget;
class WidgetFactory;
using WidgetPtr = std::shared_ptr<Widget>;
// CardPtr and Card come from <yetty/card.h> via card-factory.h

// =============================================================================
// Glyph constants (formerly in grid.h)
// =============================================================================
constexpr uint32_t GLYPH_PLUGIN = 0xFFFF;    // Cell occupied by a widget
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

// Widget position tracking
struct WidgetPosition {
  uint16_t widgetId;
  int y;
  int col;
};

// Scrolled-out widget tracking
struct ScrolledOutWidget {
  uint16_t widgetId;
  int y;
  int col;
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
  float getCellWidth() const override { return _baseCellWidth * _zoomLevel; }
  float getCellHeight() const override { return _baseCellHeight * _zoomLevel; }

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

  // Widget marker methods
  void setWidgetMarker(int row, int col, uint16_t widgetId);
  void setWidgetMarker(int row, int col, uint16_t widgetId, uint32_t width,
                       uint32_t height, uint32_t shaderGlyph, uint32_t fg,
                       uint32_t bg);
  void clearWidgetMarker(int row, int col);
  void clearWidgetMarker(int row, int col, uint32_t width, uint32_t height);
  void trackScrolledOutWidget(uint16_t widgetId, int col);
  std::vector<WidgetPosition> scanWidgetPositions() const;

  // Widget management
  void addChildWidget(WidgetPtr widget);
  Result<void> removeChildWidget(uint32_t id);
  WidgetPtr getChildWidget(uint32_t id) const;
  WidgetPtr getChildWidgetByHashId(const std::string &hashId) const;
  Result<void> removeChildWidgetByHashId(const std::string &hashId);
  void markWidgetGridCells(Widget *widget);
  void clearWidgetGridCells(Widget *widget);

  // OSC handling
  bool handleOSCSequence(const std::string &sequence, std::string *response,
                         uint32_t *linesToAdvance);
  bool handleCardOSCSequence(const std::string &sequence, std::string *response,
                             uint32_t *linesToAdvance);

  // Card registry
  void registerCard(Card *card);
  void unregisterCard(Card *card);
  Card *getCardByMetadataOffset(uint32_t offset) const;
  Card *getCardAtCell(int row, int col) const;

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
  std::function<void(int)> scrollCallback_;
  std::function<void(VTermProp, VTermValue *)> termPropCallback_;
  std::function<void()> bellCallback_;
  std::function<void(uint16_t)> widgetDisposalCallback_;
  std::function<void(const char *, size_t)> vtermInputCallback_;

  // Widgets
  std::vector<WidgetPtr> childWidgets_;
  std::unordered_map<std::string, WidgetPtr> childWidgetsByHashId_;
  uint32_t nextChildWidgetId_ = 1;
  std::vector<ScrolledOutWidget> _scrolledOutWidgets;

  // Cards
  std::vector<CardPtr> _cards;
  std::unordered_map<uint32_t, Card *> _cardRegistry;

  // Factories
  WidgetFactory *_widgetFactory = nullptr;
  // CardFactory comes from _ctx.cardFactory

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
    float pad1;               // 4 bytes
    glm::vec2 viewportOrigin; // 8 bytes
    glm::vec2 pad2;           // 8 bytes
  }; // Total: 128 bytes

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

  return Ok();
}

GPUScreenImpl::~GPUScreenImpl() {
  if (_vterm) {
    vterm_free(_vterm);
    _vterm = nullptr;
  }
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

  // Calculate cols/rows from viewport size and cell size
  float cellWidth = _baseCellWidth * _zoomLevel;
  float cellHeight = _baseCellHeight * _zoomLevel;

  uint32_t newCols = static_cast<uint32_t>(width / cellWidth);
  uint32_t newRows = static_cast<uint32_t>(height / cellHeight);

  // Only resize if size actually changed
  if (newCols > 0 && newRows > 0 && (newCols != _cols || newRows != _rows)) {
    resize(newCols, newRows);
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

  // Scan this row for widget markers - if found, track in _scrolledOutWidgets
  yinfo("GPUScreenImpl::pushLineToScrollback: scanning row {} for markers",
        row);
  for (int col = 0; col < _cols; col++) {
    const Cell &cell = (*_visibleBuffer)[srcOffset + col];
    if (cell.glyph == GLYPH_PLUGIN) {
      yinfo("GPUScreenImpl::pushLineToScrollback: found GLYPH_PLUGIN at row={} "
            "col={}, checking validation",
            row, col);
      // Validate marker pattern: fgB=0xFF, bgR=0xAA, bgG=0xAA
      if (cell.fgB == 0xFF && cell.bgR == 0xAA && cell.bgG == 0xAA) {
        uint16_t widgetId = static_cast<uint16_t>(cell.fgR) |
                            (static_cast<uint16_t>(cell.fgG) << 8);
        yinfo("GPUScreenImpl::pushLineToScrollback: VALID marker! widget {} at "
              "row={} col={} -> _scrolledOutWidgets with y=-1",
              widgetId, row, col);
        // Add with Y = -1 (just scrolled out)
        _scrolledOutWidgets.push_back({widgetId, -1, col});
      } else {
        yinfo("GPUScreenImpl::pushLineToScrollback: marker validation FAILED "
              "fgB={} bgR={} bgG={}",
              cell.fgB, cell.bgR, cell.bgG);
      }
    }
  }

  _scrollback.push_back(std::move(line));

  // If scrolled back, increment scroll offset to maintain view position
  // (otherwise the view would shift as new lines push into scrollback)
  if (_scrollOffset > 0) {
    _scrollOffset++;
  }

  // Decrement Y for all scrolled-out widgets (they move up with each scroll)
  int maxNegativeY = -static_cast<int>(maxScrollback_);
  if (!_scrolledOutWidgets.empty()) {
    yinfo("GPUScreenImpl::pushLineToScrollback: decrementing Y for {} "
          "scrolled-out "
          "widgets",
          _scrolledOutWidgets.size());
  }
  auto it = _scrolledOutWidgets.begin();
  while (it != _scrolledOutWidgets.end()) {
    int oldY = it->y;
    it->y--;
    yinfo("GPUScreenImpl::pushLineToScrollback: widget {} y: {} -> {}",
          it->widgetId, oldY, it->y);
    if (it->y < maxNegativeY) {
      // This widget's marker has been removed from scrollback
      yinfo("GPUScreen: widget {} y={} below threshold {}, disposing",
            it->widgetId, it->y, maxNegativeY);
      if (widgetDisposalCallback_) {
        widgetDisposalCallback_(it->widgetId);
      }
      it = _scrolledOutWidgets.erase(it);
    } else {
      ++it;
    }
  }

  // Limit scrollback size
  while (_scrollback.size() > maxScrollback_) {
    _scrollback.pop_front();
    // Adjust scroll offset if we removed lines from the beginning
    if (_scrollOffset > static_cast<int>(_scrollback.size())) {
      _scrollOffset = static_cast<int>(_scrollback.size());
    }
  }

  // Notify callback (for widget position updates - but we already updated Y
  // above)
  if (scrollCallback_) {
    scrollCallback_(1);
  }
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
    static int cardDebugCount = 0;
    if (cardDebugCount < 5) {
      cardDebugCount++;
      yinfo(">>> CARD CELL: pos=({},{}) glyph={} fg=({},{},{}) bg=({},{},{}) attrs={:#x}",
            pos.row, pos.col, glyphIdx, fgR, fgG, fgB, bgR, bgG, bgB, attrsByte);
    }
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
    } else if (isFullWidth) {
      // Scroll within region (not from row 0) - still notify for widget
      // position updates
      if (self->scrollCallback_) {
        self->scrollCallback_(upAmount);
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

  // Handle both legacy plugins and new cards
  if (command != YETTY_OSC_VENDOR_ID && command != YETTY_OSC_VENDOR_CARD_ID) {
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
// Widget marker methods
//=============================================================================

void GPUScreenImpl::setWidgetMarker(int row, int col, uint16_t widgetId) {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
    ydebug(
        "GPUScreenImpl::setWidgetMarker: OUT OF BOUNDS row={} col={} (screen "
        "{}x{})",
        row, col, _rows, _cols);
    return;
  }
  if (!_visibleBuffer)
    return; // Safety check

  size_t idx = cellIndex(row, col);
  if (idx >= _visibleBuffer->size())
    return;

  ydebug("GPUScreenImpl::setWidgetMarker: row={} col={} widgetId={} idx={}",
         row, col, widgetId, idx);

  Cell &cell = (*_visibleBuffer)[idx];

  // Set glyph to GLYPH_PLUGIN (0xFFFF) to mark as widget cell
  cell.glyph = GLYPH_PLUGIN;

  // Encode widget ID using fg RGB
  // Use: fgR=bits0-7, fgG=bits8-15, fgB=validation marker (0xFF)
  cell.fgR = static_cast<uint8_t>(widgetId & 0xFF);        // bits 0-7
  cell.fgG = static_cast<uint8_t>((widgetId >> 8) & 0xFF); // bits 8-15
  cell.fgB = 0xFF;   // Marker validation byte
  cell.alpha = 0xFF; // Marker validation

  // Also set bg to distinguish from erased cells
  cell.bgR = 0xAA; // Validation pattern
  cell.bgG = 0xAA;
  cell.bgB = 0xAA;
  cell.style = 0;

  _hasDamage = true;
}

void GPUScreenImpl::clearWidgetMarker(int row, int col) {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols)
    return;
  if (!_visibleBuffer)
    return; // Safety check

  size_t idx = cellIndex(row, col);
  if (idx >= _visibleBuffer->size())
    return;

  // Only clear if it's actually a widget marker
  if ((*_visibleBuffer)[idx].glyph == GLYPH_PLUGIN) {
    clearCell(row, col);
  }
}

void GPUScreenImpl::trackScrolledOutWidget(uint16_t widgetId, int col) {
  // Widget marker just scrolled from row 0 into scrollback
  // Start tracking with y = -1 (just above visible area)
  ydebug("GPUScreenImpl::trackScrolledOutWidget: widget {} col={} y=-1",
         widgetId, col);
  _scrolledOutWidgets.push_back({widgetId, -1, col});
}

std::vector<WidgetPosition> GPUScreenImpl::scanWidgetPositions() const {
  std::vector<WidgetPosition> positions;

  // When scrolled back, scan the composed view buffer (includes scrollback
  // content) Otherwise scan the visible buffer directly
  const std::vector<Cell> *buffer;

  if (_scrollOffset > 0) {
    // Ensure view buffer is composed
    const_cast<GPUScreenImpl *>(this)->composeViewBuffer();
    buffer = &viewBuffer_;
  } else if (_visibleBuffer) {
    buffer = _visibleBuffer;
  } else {
    return positions; // No buffers available
  }

  int numCells = _rows * _cols;

  // Helper lambda to validate and extract widget marker
  auto extractWidget = [&](int cellIdx) -> bool {
    const Cell &cell = (*buffer)[cellIdx];
    // Validate marker pattern: fgB=0xFF, bgR=0xAA, bgG=0xAA
    if (cell.fgB != 0xFF)
      return false;
    if (cell.bgR != 0xAA || cell.bgG != 0xAA)
      return false;

    int row = cellIdx / _cols;
    int col = cellIdx % _cols;
    uint16_t widgetId = static_cast<uint16_t>(cell.fgR) |
                        (static_cast<uint16_t>(cell.fgG) << 8);
    ydebug(
        "GPUScreenImpl::scanWidgetPositions: found widget {} at row={} col={}",
        widgetId, row, col);
    positions.push_back({widgetId, row, col});
    return true;
  };

  // Scan for widget markers
  for (int i = 0; i < numCells; i++) {
    if ((*buffer)[i].glyph == GLYPH_PLUGIN) {
      extractWidget(i);
    }
  }

  // Include widgets from helper struct (markers scrolled into scrollback)
  if (!_scrolledOutWidgets.empty()) {
    yinfo("GPUScreenImpl::scanWidgetPositions: adding {} scrolled-out widgets",
          _scrolledOutWidgets.size());
  }
  for (const auto &sow : _scrolledOutWidgets) {
    yinfo("GPUScreenImpl::scanWidgetPositions: scrolled-out widget {} at y={} "
          "col={}",
          sow.widgetId, sow.y, sow.col);
    positions.push_back({sow.widgetId, sow.y, sow.col});
  }

  yinfo("GPUScreenImpl::scanWidgetPositions: returning {} total positions",
        positions.size());
  return positions;
}

//=============================================================================
// Extended widget marker methods - mark ALL cells in widget region
//=============================================================================

void GPUScreenImpl::setWidgetMarker(int row, int col, uint16_t widgetId,
                                    uint32_t width, uint32_t height,
                                    uint32_t shaderGlyph, uint32_t fg,
                                    uint32_t /* bg unused */) {
  if (!_visibleBuffer)
    return;

  ydebug("GPUScreenImpl::setWidgetMarker: row={} col={} widgetId={} size={}x{} "
         "glyph={:#x} fg={:#x}",
         row, col, widgetId, width, height, shaderGlyph, fg);

  // Mark ALL cells in the widget region with the same glyph and attributes
  // Each cell's bg encodes its relative position using 12 bits each:
  //   bg24 = (relRow << 12) | relCol
  // This matches the ANSI true-color encoding used for vterm output
  for (uint32_t dy = 0; dy < height; dy++) {
    int cellRow = row + static_cast<int>(dy);
    if (cellRow < 0 || cellRow >= _rows)
      continue;

    for (uint32_t dx = 0; dx < width; dx++) {
      int cellCol = col + static_cast<int>(dx);
      if (cellCol < 0 || cellCol >= _cols)
        continue;

      size_t idx = cellIndex(cellRow, cellCol);
      if (idx >= _visibleBuffer->size())
        continue;

      Cell &cell = (*_visibleBuffer)[idx];
      cell.glyph = shaderGlyph;

      // Unpack fg u32 into cell bytes (slot index for new cards, byte offset
      // for legacy)
      cell.fgR = static_cast<uint8_t>(fg & 0xFF);
      cell.fgG = static_cast<uint8_t>((fg >> 8) & 0xFF);
      cell.fgB = static_cast<uint8_t>((fg >> 16) & 0xFF);
      cell.alpha = static_cast<uint8_t>((fg >> 24) & 0xFF);

      // Encode relative position in bg: (relRow << 12) | relCol (12 bits each)
      // This makes the widget scroll-independent and matches ANSI true-color
      // encoding
      uint32_t cellBg = (dy << 12) | dx;
      cell.bgR = static_cast<uint8_t>(cellBg & 0xFF);
      cell.bgG = static_cast<uint8_t>((cellBg >> 8) & 0xFF);
      cell.bgB = static_cast<uint8_t>((cellBg >> 16) & 0xFF);
      cell.style = 0; // Upper byte unused in 24-bit encoding
    }
  }

  _hasDamage = true;
}

void GPUScreenImpl::clearWidgetMarker(int row, int col, uint32_t width,
                                      uint32_t height) {
  if (!_visibleBuffer)
    return;

  for (uint32_t dy = 0; dy < height; dy++) {
    int cellRow = row + static_cast<int>(dy);
    if (cellRow < 0 || cellRow >= _rows)
      continue;

    for (uint32_t dx = 0; dx < width; dx++) {
      int cellCol = col + static_cast<int>(dx);
      if (cellCol < 0 || cellCol >= _cols)
        continue;

      clearCell(cellRow, cellCol);
    }
  }

  _hasDamage = true;
}

//=============================================================================
// Widget management implementation
//=============================================================================

void GPUScreenImpl::addChildWidget(WidgetPtr widget) {
  if (!widget)
    return;
  childWidgets_.push_back(widget);
  // Sort by zOrder for correct render order
  std::sort(childWidgets_.begin(), childWidgets_.end(),
            [](const WidgetPtr &a, const WidgetPtr &b) {
              return a->zOrder() < b->zOrder();
            });
}

void GPUScreenImpl::markWidgetGridCells(Widget *widget) {
  if (!widget)
    return;

  int32_t x = widget->getX();
  int32_t y = widget->getY();
  uint16_t id = static_cast<uint16_t>(widget->id());

  ydebug("GPUScreenImpl::markWidgetGridCells: widget {} at ({},{}) id={}",
         widget->name(), x, y, id);

  // Only mark the top-left cell with the widget marker (for tracking)
  if (x >= 0 && x < _cols && y >= 0 && y < _rows) {
    setWidgetMarker(y, x, id);
  }
}

void GPUScreenImpl::clearWidgetGridCells(Widget *widget) {
  if (!widget)
    return;

  int32_t x = widget->getX();
  int32_t y = widget->getY();

  // Only the top-left cell has the marker
  if (x >= 0 && x < _cols && y >= 0 && y < _rows) {
    clearWidgetMarker(y, x);
  }
}

Result<void> GPUScreenImpl::removeChildWidget(uint32_t id) {
  for (auto it = childWidgets_.begin(); it != childWidgets_.end(); ++it) {
    if ((*it)->id() == id) {
      clearWidgetGridCells(it->get());
      if (auto res = (*it)->dispose(); !res) {
        return Err<void>("Failed to dispose widget " + std::to_string(id));
      }
      childWidgets_.erase(it);
      return Ok();
    }
  }
  return Err<void>("Widget not found: " + std::to_string(id));
}

WidgetPtr GPUScreenImpl::getChildWidget(uint32_t id) const {
  for (const auto &widget : childWidgets_) {
    if (widget->id() == id)
      return widget;
  }
  return nullptr;
}

WidgetPtr
GPUScreenImpl::getChildWidgetByHashId(const std::string &hashId) const {
  auto it = childWidgetsByHashId_.find(hashId);
  return (it != childWidgetsByHashId_.end()) ? it->second : nullptr;
}

Result<void>
GPUScreenImpl::removeChildWidgetByHashId(const std::string &hashId) {
  auto it = childWidgetsByHashId_.find(hashId);
  if (it == childWidgetsByHashId_.end()) {
    return Err<void>("Widget not found: " + hashId);
  }

  WidgetPtr widget = it->second;
  childWidgetsByHashId_.erase(it);

  // Clear grid cells and remove from childWidgets_ vector
  clearWidgetGridCells(widget.get());
  for (auto vit = childWidgets_.begin(); vit != childWidgets_.end(); ++vit) {
    if ((*vit)->id() == widget->id()) {
      if (auto res = (*vit)->dispose(); !res) {
        return Err<void>("Failed to dispose widget");
      }
      childWidgets_.erase(vit);
      break;
    }
  }
  return Ok();
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

  yinfo("GPUScreenImpl::handleOSCSequence: vendorId={} (CARD_ID={}, "
        "LEGACY_ID={})",
        vendorId, YETTY_OSC_VENDOR_CARD_ID, YETTY_OSC_VENDOR_ID);

  // Route to card handling for new vendor ID
  if (vendorId == YETTY_OSC_VENDOR_CARD_ID) {
    yinfo("GPUScreenImpl::handleOSCSequence: routing to CARD handler");
    return handleCardOSCSequence(sequence, response, linesToAdvance);
  }

  // Legacy plugin handling (YETTY_OSC_VENDOR_ID)
  if (!_widgetFactory) {
    yerror("GPUScreenImpl::handleOSCSequence: no WidgetFactory!");
    if (response)
      *response = OscResponse::error("No WidgetFactory");
    return false;
  }

  auto parseResult = _oscParser.parse(sequence);
  if (!parseResult) {
    if (response)
      *response = OscResponse::error(error_msg(parseResult));
    return false;
  }

  OscCommand cmd = *parseResult;
  if (!cmd.isValid()) {
    if (response)
      *response = OscResponse::error(cmd.error);
    return false;
  }

  switch (cmd.type) {
  case OscCommandType::Create: {
    int32_t x = cmd.create.x;
    int32_t y = cmd.create.y;

    ydebug("OSC Create: cmd.x={} cmd.y={} relative={}", x, y,
           cmd.create.relative);

    if (cmd.create.relative) {
      int cursorCol = getCursorCol();
      int cursorRow = getCursorRow();
      ydebug("OSC Create: cursor at ({},{}) before adding", cursorCol,
             cursorRow);
      x += cursorCol;
      y += cursorRow;
      ydebug("OSC Create: final position ({},{})", x, y);
    }

    // Build widget name: "plugin" or "plugin.type"
    std::string widgetName = cmd.create.plugin;

    // Build plugin args string
    std::string pluginArgs = cmd.pluginArgs;
    if (cmd.create.relative) {
      if (!pluginArgs.empty()) {
        pluginArgs += " ";
      }
      pluginArgs += "--relative";
    }

    // TODO: WidgetFactory disabled during refactoring
    // auto result = _widgetFactory->createWidget(
    //     widgetName, x, y, static_cast<uint32_t>(cmd.create.width),
    //     static_cast<uint32_t>(cmd.create.height), pluginArgs, cmd.payload);
    // if (!result) {
    //   yerror("GPUScreen: createWidget failed: {}", error_msg(result));
    //   if (response)
    //     *response = OscResponse::error(error_msg(result));
    //   return false;
    // }
    // WidgetPtr widget = *result;
    if (response)
      *response = OscResponse::error("WidgetFactory disabled");
    return false;

    WidgetPtr widget = nullptr; // placeholder

    // Configure widget position
    widget->setPosition(x, y);
    widget->setId(nextChildWidgetId_++);
    widget->setHashId(_oscParser.generateId());
    widget->setScreenType(isAltScreenExternal_ ? ScreenType::Alternate
                                               : ScreenType::Main);
    if (cmd.create.relative) {
      widget->setPositionMode(PositionMode::Relative);
    }

    ydebug("GPUScreenImpl::handleOSCSequence: widget created at ({},{}) cursor "
           "was "
           "({},{})",
           x, y, getCursorCol(), getCursorRow());

    // Add to GPUScreen
    addChildWidget(widget);
    childWidgetsByHashId_[widget->hashId()] = widget;

    // Mark grid cells with widget ID for mouse hit testing
    markWidgetGridCells(widget.get());

    yinfo("GPUScreen: Created widget {} plugin={} at ({},{}) size {}x{}",
          widget->hashId(), cmd.create.plugin, x, y, widget->getWidthCells(),
          widget->getHeightCells());

    if (linesToAdvance && cmd.create.relative) {
      *linesToAdvance = std::abs(static_cast<int>(widget->getHeightCells()));
    }
    return true;
  }

  case OscCommandType::List: {
    if (response) {
      std::vector<
          std::tuple<std::string, std::string, int, int, int, int, bool>>
          widgetList;
      for (const auto &widget : childWidgets_) {
        if (!cmd.list.all && !widget->isRunning())
          continue;
        widgetList.emplace_back(widget->hashId(), widget->name(),
                                widget->getX(), widget->getY(),
                                widget->getWidthCells(),
                                widget->getHeightCells(), widget->isRunning());
      }
      *response = OscResponse::widgetList(widgetList);
    }
    return true;
  }

  case OscCommandType::Kill: {
    if (!cmd.target.id.empty()) {
      auto res = removeChildWidgetByHashId(cmd.target.id);
      if (!res) {
        if (response)
          *response = OscResponse::error(error_msg(res));
        return false;
      }
    } else {
      if (response)
        *response = OscResponse::error("kill: --id required");
      return false;
    }
    return true;
  }

  case OscCommandType::Stop: {
    if (!cmd.target.id.empty()) {
      auto widget = getChildWidgetByHashId(cmd.target.id);
      if (widget) {
        widget->stop();
      } else {
        if (response)
          *response = OscResponse::error("Widget not found: " + cmd.target.id);
        return false;
      }
    } else {
      if (response)
        *response = OscResponse::error("stop: --id required");
      return false;
    }
    return true;
  }

  case OscCommandType::Start: {
    if (!cmd.target.id.empty()) {
      auto widget = getChildWidgetByHashId(cmd.target.id);
      if (widget) {
        widget->start();
      } else {
        if (response)
          *response = OscResponse::error("Widget not found: " + cmd.target.id);
        return false;
      }
    } else {
      if (response)
        *response = OscResponse::error("start: --id required");
      return false;
    }
    return true;
  }

  case OscCommandType::Update: {
    if (cmd.target.id.empty()) {
      if (response)
        *response = OscResponse::error("update: --id required");
      return false;
    }
    auto widget = getChildWidgetByHashId(cmd.target.id);
    if (!widget) {
      if (response)
        *response = OscResponse::error("Widget not found: " + cmd.target.id);
      return false;
    }
    if (auto res = widget->dispose(); !res) {
      if (response)
        *response = OscResponse::error("Failed to dispose widget");
      return false;
    }
    widget->setPayload(cmd.payload);
    if (auto res = widget->reinit(); !res) {
      if (response)
        *response = OscResponse::error("Widget re-init failed");
      return false;
    }
    return true;
  }

  case OscCommandType::Plugins: {
    // TODO: WidgetFactory disabled during refactoring
    if (response) {
      *response = OscResponse::error("WidgetFactory disabled");
    }
    return true;
  }

  default:
    if (response)
      *response = OscResponse::error("unknown command");
    return false;
  }
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

    // Create the card
    auto result = _ctx.cardFactory->createCard(
        cmd.create.plugin, x, y, static_cast<uint32_t>(cmd.create.width),
        static_cast<uint32_t>(cmd.create.height), cmd.pluginArgs, cmd.payload);

    if (!result) {
      yerror("GPUScreen: createCard failed: {}", error_msg(result));
      if (response)
        *response = OscResponse::error(error_msg(result));
      return false;
    }

    CardPtr cardPtr = std::move(*result);
    Card *card = cardPtr.get();

    // Register the card for input routing
    registerCard(card);

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
      ywarn("GPUScreen: No vtermInputCallback_ set, falling back to direct "
            "buffer write");
      // Fallback: direct buffer write (old behavior, breaks scrolling)
      // Note: Using slot index for consistency with ANSI encoding
      uint32_t fg = card->metadataSlotIndex();
      uint32_t bg = 0; // bg is computed per-cell in setWidgetMarker
      setWidgetMarker(y, x, 0, card->widthCells(), card->heightCells(),
                      card->shaderGlyph(), fg, bg);
    }

    yinfo("GPUScreen: Created card '{}' at ({},{}) size {}x{} slotIndex={} "
          "shaderGlyph={:#x}",
          card->typeName(), x, y, card->widthCells(), card->heightCells(),
          card->metadataSlotIndex(), card->shaderGlyph());

    // Store the card (transfer ownership)
    _cards.push_back(std::move(cardPtr));

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

    // TODO: Implement Kill, Stop, Start, Update, List for cards

  default:
    if (response)
      *response = OscResponse::error("Card: unknown or unsupported command");
    return false;
  }
}

//=============================================================================
// Card Registry Management
//=============================================================================

void GPUScreenImpl::registerCard(Card *card) {
  if (!card)
    return;

  uint32_t offset = card->metadataOffset();
  _cardRegistry[offset] = card;
  ydebug("GPUScreenImpl::registerCard: registered card at offset {}", offset);
}

void GPUScreenImpl::unregisterCard(Card *card) {
  if (!card)
    return;

  uint32_t offset = card->metadataOffset();
  _cardRegistry.erase(offset);
  ydebug("GPUScreenImpl::unregisterCard: unregistered card at offset {}",
         offset);
}

Card *GPUScreenImpl::getCardByMetadataOffset(uint32_t offset) const {
  auto it = _cardRegistry.find(offset);
  return (it != _cardRegistry.end()) ? it->second : nullptr;
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

  // The fg value encodes the metadata offset
  uint32_t fg = static_cast<uint32_t>(cell.fgR) |
                (static_cast<uint32_t>(cell.fgG) << 8) |
                (static_cast<uint32_t>(cell.fgB) << 16) |
                (static_cast<uint32_t>(cell.alpha) << 24);

  return getCardByMetadataOffset(fg);
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
  yinfo("GPUScreen {} registered for SetFocus and MouseDown events", _id);

  // Auto-focus on startup so keyboard works immediately
  loop->dispatch(base::Event::focusEvent(_id));
}

Result<bool> GPUScreenImpl::onEvent(const base::Event &event) {
  // Handle mouse click - dispatch SetFocus if clicked on our surface
  if (event.type == base::Event::Type::MouseDown) {
    float mx = event.mouse.x;
    float my = event.mouse.y;
    ydebug("GPUScreen {} MouseDown at ({}, {}), viewport=({},{} {}x{})", _id,
           mx, my, _viewportX, _viewportY, _viewportWidth, _viewportHeight);

    // Only check bounds if viewport is set
    if (_viewportWidth > 0 && _viewportHeight > 0) {
      // Check if click is within our viewport
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {
        yinfo("GPUScreen {} clicked at ({}, {}), dispatching SetFocus", _id, mx,
              my);
        auto loop = *base::EventLoop::instance();
        loop->dispatch(base::Event::focusEvent(_id));
        return Ok(true);
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

  // Handle keyboard events (only when focused)
  if (_focused) {
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

  // Debug: log first few non-default colored cells during render
  static int renderDebugCount = 0;
  if (renderDebugCount < 5) {
    for (size_t i = 0; i < std::min(static_cast<size_t>(50), static_cast<size_t>(_cols * _rows)); i++) {
      const Cell& c = cells[i];
      bool hasColor = (c.fgR != 240 || c.fgG != 240 || c.fgB != 240) ||
                      (c.bgR != 0 || c.bgG != 0 || c.bgB != 0);
      if (hasColor && c.glyph != 0 && c.glyph != ' ') {
        yinfo("render[{}]: glyph={} fg=({},{},{}) bg=({},{},{}) style={}",
              i, c.glyph, c.fgR, c.fgG, c.fgB, c.bgR, c.bgG, c.bgB, c.style);
        renderDebugCount++;
        if (renderDebugCount >= 5) break;
      }
    }
  }


  // Calculate cell size from font base size and zoom level
  float cellWidth = _baseCellWidth * _zoomLevel;
  float cellHeight = _baseCellHeight * _zoomLevel;

  // Screen size is grid * cell size
  float screenWidth = static_cast<float>(_cols) * cellWidth;
  float screenHeight = static_cast<float>(_rows) * cellHeight;

  // Update uniforms
  _uniforms.projection =
      glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
  _uniforms.screenSize = {screenWidth, screenHeight};
  _uniforms.cellSize = {cellWidth, cellHeight};
  _uniforms.gridSize = {static_cast<float>(_cols), static_cast<float>(_rows)};
  _uniforms.pixelRange = _msdfFont ? _msdfFont->getPixelRange() : 2.0f;
  _uniforms.scale = _zoomLevel;
  _uniforms.cursorPos = {static_cast<float>(_cursorCol),
                         static_cast<float>(_cursorRow)};
  _uniforms.cursorVisible = _cursorVisible ? 1.0f : 0.0f;
  _uniforms.viewportOrigin = {_viewportX, _viewportY};
  wgpuQueueWriteBuffer(queue, _uniformBuffer, 0, &_uniforms, sizeof(Uniforms));

  // Set viewport to this terminal's tile bounds
  if (_viewportWidth > 0 && _viewportHeight > 0) {
    wgpuRenderPassEncoderSetViewport(pass, _viewportX, _viewportY,
                                     _viewportWidth, _viewportHeight, 0.0f,
                                     1.0f);
  }

  // Draw using shared pipeline and quad vertex buffer from ShaderManager
  wgpuRenderPassEncoderSetPipeline(pass, shaderMgr->getGridPipeline());
  wgpuRenderPassEncoderSetBindGroup(pass, 0, _ctx.gpu.sharedBindGroup, 0,
                                    nullptr);
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
