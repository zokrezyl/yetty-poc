#include "terminal.h"
#include "emoji-atlas.h"
#include "grid-renderer.h"
#include "widget-factory.h"
#include "yetty/emoji.h"
#include <yetty/font-manager.h>
#include <ytrace/ytrace.hpp>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <ytrace/ytrace.hpp>

#ifndef _WIN32
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif
#endif

namespace yetty {

// libvterm screen callbacks - NO LONGER USED with GPUScreen
// Kept for reference during transition
#if 0
static VTermScreenCallbacks screenCallbacks = {
    .damage = Terminal::onDamage,
    .moverect = Terminal::onMoverect,
    .movecursor = Terminal::onMoveCursor,
    .settermprop = Terminal::onSetTermProp,
    .bell = Terminal::onBell,
    .resize = Terminal::onResize,
    .sb_pushline = Terminal::onSbPushline,
    .sb_popline = Terminal::onSbPopline,
    .sb_clear = nullptr,
};
#endif

// OSC fallbacks still needed for widget embedding
static VTermStateFallbacks stateFallbacks = {
    .control = nullptr,
    .csi = nullptr,
    .osc = Terminal::onOSC,
    .dcs = nullptr,
    .apc = nullptr,
    .pm = nullptr,
    .sos = nullptr,
};

//=============================================================================
// Factory
//=============================================================================

// Generic create (for WidgetFactory)
Result<WidgetPtr> Terminal::create(WidgetFactory *factory,
                                   const WidgetParams &params,
                                   const std::string &payload) noexcept {
  yfunc();
  yinfo("Terminal::create(generic) factory={} payload.size={}", (void *)factory,
        payload.size());

  if (!factory) {
    ywarn("Terminal::create: null factory");
    return Err<WidgetPtr>("Terminal::create: null factory");
  }

  auto *fontManager = factory->getFontManager();
  yinfo("Terminal::create: fontManager={}", (void *)fontManager);
  if (!fontManager) {
    ywarn("Terminal::create: null FontManager");
    return Err<WidgetPtr>("Terminal::create: null FontManager");
  }
  auto *font = fontManager->getDefaultFont();
  yinfo("Terminal::create: font={}", (void *)font);
  if (!font) {
    ywarn("Terminal::create: null Font");
    return Err<WidgetPtr>("Terminal::create: null Font");
  }

  auto *loop = params.loop;
  yinfo("Terminal::create: loop={}", (void *)loop);
  if (!loop) {
    ywarn("Terminal::create: null libuv loop in params");
    return Err<WidgetPtr>("Terminal::create: null libuv loop in params");
  }

  // Use params dimensions or defaults
  uint32_t cols = params.widthCells > 0 ? params.widthCells : 80;
  uint32_t rows = params.heightCells > 0 ? params.heightCells : 24;
  yinfo("Terminal::create: cols={} rows={}", cols, rows);

  // Get CardBufferManager from factory
  auto *cardBufferManager = factory->getCardBufferManager();
  yinfo("Terminal::create: cardBufferManager={}", (void *)cardBufferManager);

  // Create with id=0, Widget base constructor will assign the actual ID
  auto term = std::shared_ptr<Terminal>(
      new Terminal(0, cols, rows, font, loop, cardBufferManager));
  yinfo("Terminal::create: terminal constructed id={}", term->id());

  // Store WidgetFactory for child widget creation
  term->_widgetFactory = factory;

  // Store config if available
  term->_config = factory->getConfig();
  yinfo("Terminal::create: config={}", (void *)term->_config);

  if (auto res = term->init(); !res) {
    ywarn("Terminal::create: init failed: {}", error_msg(res));
    return Err<WidgetPtr>("Failed to initialize Terminal", res);
  }
  yinfo("Terminal::create: init succeeded");

  // Set up GPUScreen with WidgetFactory (must be after init() which creates
  // GPUScreen)
  if (term->_gpuScreen) {
    term->_gpuScreen->setWidgetFactory(factory);
  }

  // Parse shell from payload if provided
  if (!payload.empty()) {
    term->_shell = payload;
    yinfo("Terminal::create: shell={}", term->_shell);
  }

  yinfo("Terminal::create(generic): SUCCESS id={}", term->id());
  return Ok(std::static_pointer_cast<Widget>(term));
}

// Legacy create (for direct creation)
Result<Terminal::Ptr>
Terminal::create(uint32_t id, uint32_t cols, uint32_t rows, Font *font,
                 uv_loop_t *loop,
                 CardBufferManager *cardBufferManager) noexcept {
  yfunc();
  yinfo("Terminal::create(legacy) id={} cols={} rows={} font={} loop={} "
        "cardBufferManager={}",
        id, cols, rows, (void *)font, (void *)loop, (void *)cardBufferManager);

  if (!font) {
    ywarn("Terminal::create: null Font");
    return Err<Ptr>("Terminal::create: null Font");
  }
  if (!loop) {
    ywarn("Terminal::create: null libuv loop");
    return Err<Ptr>("Terminal::create: null libuv loop");
  }
  auto term = Ptr(new Terminal(id, cols, rows, font, loop, cardBufferManager));
  yinfo("Terminal::create: terminal constructed id={}", term->id());

  if (auto res = term->init(); !res) {
    ywarn("Terminal::create: init failed: {}", error_msg(res));
    return Err<Ptr>("Failed to initialize Terminal", res);
  }
  yinfo("Terminal::create(legacy): SUCCESS id={}", term->id());
  return Ok(std::move(term));
}

Terminal::Terminal(uint32_t id, uint32_t cols, uint32_t rows, Font *font,
                   uv_loop_t *loop,
                   CardBufferManager *cardBufferManager) noexcept
    : Widget() // Widget base constructor assigns id from nextId_
      ,
      _loop(loop), _grid(cols, rows), _font(font), _cols(cols), _rows(rows),
      _cardBufferManager(cardBufferManager) {
  // If explicit id provided, use it; otherwise keep Widget's auto-assigned id
  if (id > 0) {
    setId(id);
  }
  setName("terminal-" + std::to_string(_id));
}

Result<void> Terminal::init() noexcept {
  yfunc();
  yinfo("Terminal::init cols={} rows={}", _cols, _rows);

  _vterm = vterm_new(_rows, _cols);
  if (!_vterm) {
    ywarn("Terminal::init: Failed to create VTerm");
    return Err<void>("Failed to create VTerm");
  }
  yinfo("Terminal::init: vterm created");
  vterm_set_utf8(_vterm, 1);

  // Create GPUScreen - replaces vterm_screen with direct GPU buffer storage
  _gpuScreen = std::make_unique<GPUScreen>(_rows, _cols, _font);

  // Set up GPUScreen callbacks for Terminal integration
  _gpuScreen->setTermPropCallback([this](VTermProp prop, VTermValue *val) {
    switch (prop) {
    case VTERM_PROP_ALTSCREEN:
      _isAltScreen = val->boolean;
      break;
    case VTERM_PROP_CURSORVISIBLE:
      _cursorVisible = val->boolean;
      break;
    case VTERM_PROP_MOUSE:
      _mouseMode = val->number;
      break;
    default:
      break;
    }
  });

  _gpuScreen->setBellCallback([]() {
#ifndef _WIN32
    write(STDOUT_FILENO, "\a", 1);
#endif
  });

  // Set up scroll callback for widget position updates
  _gpuScreen->setScrollCallback(
      [this](int lines) { updateWidgetPositionsOnScroll(lines); });

  // Set up widget disposal callback for when markers are removed from
  // scrollback
  _gpuScreen->setWidgetDisposalCallback([this](uint16_t widgetId) {
    yinfo("Terminal: disposing widget {} (marker removed from scrollback)",
          widgetId);
    auto result = removeChildWidget(static_cast<uint32_t>(widgetId));
    if (!result) {
      ywarn("Terminal: failed to dispose widget {}: {}", widgetId,
            result.error().message());
    }
  });

  // Attach GPUScreen to vterm (registers State callbacks)
  _gpuScreen->attach(_vterm);
  yinfo("Terminal::init: GPUScreen attached to vterm");

  // Set up OSC fallbacks for widget embedding (still needed)
  VTermState *state = vterm_obtain_state(_vterm);
  vterm_state_set_unrecognised_fallbacks(state, &stateFallbacks, this);

  // Set up GPUScreen for widget management (OSC handling moved to GPUScreen)
  _gpuScreen->setVTerm(_vterm);
  _gpuScreen->setCardBufferManager(_cardBufferManager);
  _gpuScreen->setPtyWriteCallback(
      [this](const char *data, size_t len) { writeToPty(data, len); });
  _gpuScreen->setVTermInputCallback([this](const char *data, size_t len) {
    vterm_input_write(_vterm, data, len);
  });

  _ptyReadBuffer = std::make_unique<char[]>(PTY_READ_BUFFER_SIZE);

  yinfo("Terminal::init: SUCCESS");
  return Ok();
}

Terminal::~Terminal() {
  stop();

#ifndef _WIN32
  if (_ptyMaster >= 0) {
    close(_ptyMaster);
  }
  if (_childPid > 0) {
    kill(_childPid, SIGTERM);
    waitpid(_childPid, nullptr, 0);
  }
#endif

  if (_vterm) {
    vterm_free(_vterm);
  }
}

//=============================================================================
// Widget interface
//=============================================================================

void Terminal::start() {
  yfunc();
  yinfo("Terminal::start _running={} _shell={}", _running.load(), _shell);

  if (_running) {
    yinfo("Terminal::start: already running, skipping");
    return;
  }

  // Start shell
  yinfo("Terminal::start: calling startShell");
  if (auto res = startShell(_shell); !res) {
    ywarn("Terminal::start: startShell failed: {}", res.error().to_string());
    yerror("Terminal: failed to start shell: {}", res.error().to_string());
    return;
  }
  yinfo("Terminal::start: shell started successfully");

  _running = true;

  // Set up cursor blink timer (500ms)
  yinfo("Terminal::start: setting up cursor timer");
  _cursorTimer = new uv_timer_t;
  uv_timer_init(_loop, _cursorTimer);
  _cursorTimer->data = this;
  uv_timer_start(_cursorTimer, onTimer, 500, 500);

#ifndef _WIN32
  // Set up PTY poll handle on external loop
  yinfo("Terminal::start: setting up PTY poll, _ptyMaster={}", _ptyMaster);
  if (_ptyMaster >= 0) {
    _ptyPoll = new uv_poll_t;
    uv_poll_init(_loop, _ptyPoll, _ptyMaster);
    _ptyPoll->data = this;
    uv_poll_start(_ptyPoll, UV_READABLE, onPtyPoll);
    yinfo("Terminal::start: PTY poll started");
  }
#endif
  yinfo("Terminal::start: SUCCESS");
}

void Terminal::stop() {
  if (!_running)
    return;

  _running = false;

#ifndef _WIN32
  if (_ptyPoll) {
    uv_poll_stop(_ptyPoll);
    uv_close(reinterpret_cast<uv_handle_t *>(_ptyPoll),
             [](uv_handle_t *h) { delete reinterpret_cast<uv_poll_t *>(h); });
    _ptyPoll = nullptr;
  }
#endif

  if (_cursorTimer) {
    uv_timer_stop(_cursorTimer);
    uv_close(reinterpret_cast<uv_handle_t *>(_cursorTimer),
             [](uv_handle_t *h) { delete reinterpret_cast<uv_timer_t *>(h); });
    _cursorTimer = nullptr;
  }
}

void Terminal::prepareFrame(WebGPUContext &ctx, bool on) {
  (void)on; // Terminal is always on when called

  if (!_gpuScreen)
    return;

  const auto &childWidgets = _gpuScreen->getChildWidgets();

  // Scan for widget markers when terminal has damage - markers scroll with
  // content
  if (!childWidgets.empty()) {
    bool hasDamage = _gpuScreen->hasDamage();
    ydebug("prepareFrame: hasDamage={} childWidgets={}", hasDamage,
           childWidgets.size());

    if (hasDamage) {
      auto positions = _gpuScreen->scanWidgetPositions();
      ydebug("prepareFrame: scanWidgetPositions found {} positions",
             positions.size());

      for (const auto &pos : positions) {
        for (const auto &widget : childWidgets) {
          if (static_cast<uint16_t>(widget->id()) == pos.widgetId) {
            if (widget->getX() != pos.col || widget->getY() != pos.row) {
              ydebug(
                  "prepareFrame: widget {} position updated ({},{}) -> ({},{})",
                  pos.widgetId, widget->getX(), widget->getY(), pos.col,
                  pos.row);
              widget->setPosition(pos.col, pos.row);
            }
            break;
          }
        }
      }
    }
  }

  // Get current screen type for filtering
  ScreenType currentScreen =
      _isAltScreen ? ScreenType::Alternate : ScreenType::Main;

  // Compute and set pixel positions for child widgets
  for (const auto &widget : childWidgets) {
    if (!widget->isVisible())
      continue;
    if (widget->getScreenType() != currentScreen)
      continue;

    // Terminal computes pixel position from grid position
    float pixelX = static_cast<float>(widget->getX()) * _cellWidth;
    float pixelY = static_cast<float>(widget->getY()) * _cellHeight;

    // Adjust for scroll offset if widget is relative
    int scrollOff = getScrollOffset();
    if (widget->getPositionMode() == PositionMode::Relative && scrollOff > 0) {
      pixelY += scrollOff * _cellHeight;
    }

    // Compute pixel size from cell size
    uint32_t pixelW = widget->getWidthCells() * _cellWidth;
    uint32_t pixelH = widget->getHeightCells() * _cellHeight;

    yinfo("Terminal::prepareFrame widget '{}' grid=({},{}) cellSize=({},{}) -> "
          "pixel=({},{}) size={}x{}",
          widget->name(), widget->getX(), widget->getY(), _cellWidth,
          _cellHeight, pixelX, pixelY, pixelW, pixelH);

    // Set pixel position and size on widget
    widget->setPixelPosition(pixelX, pixelY);
    widget->setPixelSize(pixelW, pixelH);

    // TODO: Calculate if widget is visible based on scroll position
    bool widgetOn = true;

    // Call prepareFrame on child (for texture-based widgets)
    widget->prepareFrame(ctx, widgetOn);
  }
}

Result<void> Terminal::render(WGPURenderPassEncoder pass, WebGPUContext &ctx,
                              bool on) {
  (void)on; // Terminal is always on when called

  if (!_running || !_gpuScreen) {
    return Ok();
  }

  // GPUScreen already has data in GPU-ready format from State callbacks
  // No syncToGrid needed!
  bool needsUpload = _gpuScreen->hasDamage() || _fullDamage;

  // Render grid from GPUScreen Cell buffer (zero-copy)
  if (_renderer) {
    if (auto res = _renderer->renderToPassFromCells(
            pass, static_cast<uint32_t>(_gpuScreen->getCols()),
            static_cast<uint32_t>(_gpuScreen->getRows()),
            _gpuScreen->getCellData(), needsUpload, _gpuScreen->getCursorCol(),
            _gpuScreen->getCursorRow(),
            _gpuScreen->isCursorVisible() && _cursorBlink);
        !res) {
      return Err<void>("Terminal::render failed");
    }
  }

  // Clear damage tracking
  _gpuScreen->clearDamage();
  _fullDamage = false;

  // Child widgets decide themselves if they need to render
  ScreenType currentScreen =
      _isAltScreen ? ScreenType::Alternate : ScreenType::Main;
  const auto &childWidgets = _gpuScreen->getChildWidgets();
  for (const auto &widget : childWidgets) {
    if (!widget->isVisible())
      continue;
    if (widget->getScreenType() != currentScreen)
      continue;

    // TODO: Calculate if widget is visible based on scroll position
    bool widgetOn = true;

    if (auto res = widget->render(pass, ctx, widgetOn); !res) {
      yerror("Terminal: widget '{}' render failed: {}", widget->name(),
             res.error().message());
    }
  }

  return Ok();
}

//=============================================================================
// Shell startup
//=============================================================================

Result<void> Terminal::startShell(const std::string &shell) {
  yfunc();
  yinfo("Terminal::startShell shell={}", shell);

#ifndef _WIN32
  std::string shellPath =
      shell.empty() ? (getenv("SHELL") ? getenv("SHELL") : "/bin/sh") : shell;
  yinfo("Terminal::startShell: shellPath={}", shellPath);

  ydebug("Starting shell: {}", shellPath);

  struct winsize ws;
  ws.ws_row = _rows;
  ws.ws_col = _cols;
  ws.ws_xpixel = 0;
  ws.ws_ypixel = 0;
  yinfo("Terminal::startShell: winsize rows={} cols={}", ws.ws_row, ws.ws_col);

  yinfo("Terminal::startShell: calling forkpty");
  _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);
  yinfo("Terminal::startShell: forkpty returned _childPid={} _ptyMaster={}",
        _childPid, _ptyMaster);

  if (_childPid < 0) {
    ywarn("Terminal::startShell: forkpty failed: {}", strerror(errno));
    return Err<void>(std::string("forkpty failed: ") + strerror(errno));
  }

  if (_childPid == 0) {
    // Child
    setenv("TERM", "xterm-256color", 1);
    setenv("COLORTERM", "truecolor", 1);

    for (int fd = 3; fd < 1024; fd++)
      close(fd);

    if (shellPath.find(' ') != std::string::npos) {
      const char *sh = getenv("SHELL") ? getenv("SHELL") : "/bin/sh";
      execl(sh, sh, "-c", shellPath.c_str(), nullptr);
    } else {
      execl(shellPath.c_str(), shellPath.c_str(), nullptr);
    }
    _exit(1);
  }

  // Parent - set non-blocking
  int flags = fcntl(_ptyMaster, F_GETFL, 0);
  fcntl(_ptyMaster, F_SETFL, flags | O_NONBLOCK);
  yinfo("Terminal::startShell: PTY set to non-blocking");

  yinfo("Terminal::startShell: SUCCESS _ptyMaster={} _childPid={}", _ptyMaster,
        _childPid);
  yinfo("Terminal started: PTY fd={}, PID={}", _ptyMaster, _childPid);
  return Ok();
#else
  ywarn("Terminal::startShell: Windows not implemented yet");
  return Err<void>("Windows not implemented yet");
#endif
}

//=============================================================================
// libuv callbacks
//=============================================================================

void Terminal::onTimer(uv_timer_t *handle) {
  auto *self = static_cast<Terminal *>(handle->data);
  double now = std::chrono::duration<double>(
                   std::chrono::steady_clock::now().time_since_epoch())
                   .count();
  self->updateCursorBlink(now);
}

void Terminal::onPtyPoll(uv_poll_t *handle, int status, int events) {
  auto *self = static_cast<Terminal *>(handle->data);

  if (status < 0) {
    yerror("Terminal[{}]: PTY poll error: {}", self->_id, uv_strerror(status));
    return;
  }

  if (events & UV_READABLE) {
    if (auto res = self->readPty(); !res) {
      yerror("Terminal[{}]: PTY read error: {}", self->_id,
             res.error().to_string());
    }
  }
}

//=============================================================================
// Keyboard input (direct write, no queueing)
//=============================================================================

void Terminal::sendKey(uint32_t codepoint, VTermModifier mod) {
  if (_gpuScreen && _gpuScreen->isScrolledBack()) {
    _gpuScreen->scrollToBottom();
  }
  vterm_keyboard_unichar(_vterm, codepoint, mod);
  flushVtermOutput();
}

void Terminal::sendSpecialKey(VTermKey key, VTermModifier mod) {
  if (_gpuScreen && _gpuScreen->isScrolledBack()) {
    _gpuScreen->scrollToBottom();
  }
  vterm_keyboard_key(_vterm, key, mod);
  flushVtermOutput();
}

void Terminal::sendRaw(const char *data, size_t len) {
  if (_gpuScreen && _gpuScreen->isScrolledBack()) {
    _gpuScreen->scrollToBottom();
  }
  writeToPty(data, len);
}

void Terminal::resize(uint32_t cols, uint32_t rows) {
  _cols = cols;
  _rows = rows;
  // CRITICAL: Resize GPUScreen BEFORE vterm_set_size!
  // vterm_set_size triggers callbacks (scroll, moverect) that access GPUScreen
  // buffers. If GPUScreen has old dimensions, we get heap corruption.
  _grid.resize(_cols, _rows);
  vterm_set_size(_vterm, _rows, _cols);
#ifndef _WIN32
  if (_ptyMaster >= 0) {
    struct winsize ws = {static_cast<unsigned short>(_rows),
                         static_cast<unsigned short>(_cols), 0, 0};
    ioctl(_ptyMaster, TIOCSWINSZ, &ws);
  }
#endif
  _fullDamage = true;
}

//=============================================================================
// PTY I/O
//=============================================================================

Result<void> Terminal::readPty() {
#ifndef _WIN32
  // Check child status
  int status;
  if (waitpid(_childPid, &status, WNOHANG) > 0) {
    _running = false;
    yinfo("Shell exited");
    return Ok();
  }

  // Don't try to read if not running
  if (!_running) {
    return Ok();
  }

  // Drain PTY - read as much as available up to 40KB
  size_t totalRead = 0;
  ssize_t n;
  while ((n = read(_ptyMaster, _ptyReadBuffer.get() + totalRead,
                   PTY_READ_BUFFER_SIZE - totalRead)) > 0) {
    totalRead += n;
    if (totalRead >= PTY_READ_BUFFER_SIZE)
      break;
  }

  // Handle read errors
  if (totalRead == 0 && n < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // No data available, this is normal for non-blocking read
      return Ok();
    }
    if (errno == EIO) {
      // EIO means the slave side was closed (shell exited)
      _running = false;
      yinfo("Shell exited (PTY closed)");
      return Ok();
    }
    // Other errors are real problems
    return Err<void>(std::string("PTY read error: ") + strerror(errno));
  }

  if (totalRead > 0) {
    vterm_input_write(_vterm, _ptyReadBuffer.get(), totalRead);
    // GPUScreen handles damage tracking automatically via State callbacks
    // No need for vterm_screen_flush_damage
    flushVtermOutput();
  }

  return Ok();
#else
  return Err<void>("Windows not implemented");
#endif
}

Result<void> Terminal::writeToPty(const char *data, size_t len) {
  if (len == 0)
    return Ok();
#ifndef _WIN32
  if (_ptyMaster < 0)
    return Err("PTY not open");
  ssize_t written = write(_ptyMaster, data, len);
  if (written < 0) {
    return Err("PTY write failed: " + std::string(strerror(errno)));
  }
#endif
  return Ok();
}

Result<void> Terminal::flushVtermOutput() {
  size_t outlen = vterm_output_get_buffer_current(_vterm);
  char buf[65536];
  while (outlen > 0) {
    size_t n = vterm_output_read(_vterm, buf, sizeof(buf));
    if (n > 0) {
      if (auto res = writeToPty(buf, n); !res)
        return res;
    }
    outlen = vterm_output_get_buffer_current(_vterm);
  }
  return Ok();
}

//=============================================================================
// Scrollback - delegated to GPUScreen
//=============================================================================

void Terminal::scrollUp(int lines) {
  if (_gpuScreen) {
    _gpuScreen->scrollUp(lines);
  }
}

void Terminal::scrollDown(int lines) {
  if (_gpuScreen) {
    _gpuScreen->scrollDown(lines);
  }
}

void Terminal::scrollToTop() {
  if (_gpuScreen) {
    _gpuScreen->scrollToTop();
  }
}

void Terminal::scrollToBottom() {
  if (_gpuScreen) {
    _gpuScreen->scrollToBottom();
  }
}

//=============================================================================
// Selection
//=============================================================================

void Terminal::startSelection(int row, int col, SelectionMode mode) {
  _selectionStart = {row, col};
  _selectionEnd = {row, col};
  _selectionMode = mode;
  _fullDamage = true;
}

void Terminal::extendSelection(int row, int col) {
  if (_selectionMode == SelectionMode::None)
    return;
  _selectionEnd = {row, col};
  _fullDamage = true;
}

void Terminal::clearSelection() {
  _selectionMode = SelectionMode::None;
  _fullDamage = true;
}

bool Terminal::isInSelection(int row, int col) const {
  if (_selectionMode == SelectionMode::None)
    return false;

  VTermPos start = _selectionStart, end = _selectionEnd;
  if (vterm_pos_cmp(start, end) > 0)
    std::swap(start, end);

  VTermPos pos = {row, col};
  return vterm_pos_cmp(pos, start) >= 0 && vterm_pos_cmp(pos, end) <= 0;
}

std::string Terminal::getSelectedText() {
  if (_selectionMode == SelectionMode::None)
    return "";

  // TODO: Implement text selection without codepoints
  // Since we removed codepoints from GPUScreen for efficiency,
  // we need to query vterm directly for cell contents when needed.
  // For now, return empty string.
  (void)_selectionStart;
  (void)_selectionEnd;
  return "";
}

//=============================================================================
// Cell size / zoom
//=============================================================================

void Terminal::setCellSize(uint32_t width, uint32_t height) {
  _cellWidth = width;
  _cellHeight = height;
}

void Terminal::setBaseCellSize(float width, float height) {
  _baseCellWidth = width;
  _baseCellHeight = height;
}

void Terminal::setZoomLevel(float zoom) {
  _zoomLevel = zoom;
  if (_renderer) {
    _renderer->setCellSize(_baseCellWidth * zoom, _baseCellHeight * zoom);
    _renderer->setScale(zoom);
  }
}

//=============================================================================
// Cursor blink
//=============================================================================

void Terminal::updateCursorBlink(double currentTime) {
  if (currentTime - _lastBlinkTime >= _blinkInterval) {
    _cursorBlink = !_cursorBlink;
    _lastBlinkTime = currentTime;
    // Mark damage so cursor gets re-rendered with new blink state
    if (_cursorVisible && _gpuScreen) {
      _gpuScreen->markDamage();
    }
  }
}

void Terminal::colorToRGB(const VTermColor &color, uint8_t &r, uint8_t &g,
                          uint8_t &b) {
  if (VTERM_COLOR_IS_DEFAULT_FG(&color)) {
    r = 255;
    g = 255;
    b = 255;
  } else if (VTERM_COLOR_IS_DEFAULT_BG(&color)) {
    r = 0;
    g = 0;
    b = 0;
  } else if (VTERM_COLOR_IS_RGB(&color)) {
    r = color.rgb.red;
    g = color.rgb.green;
    b = color.rgb.blue;
  } else {
    r = 128;
    g = 128;
    b = 128;
  }
}

void Terminal::updateWidgetPositionsOnScroll(int lines) {
  (void)lines;
  // Widget marker tracking is handled by GPUScreen::pushLineToScrollback
  // (scans row 0 for markers before pushing to scrollback)
  // Widget positions are updated by prepareFrame via scanWidgetPositions
  // which includes both visible buffer markers AND scrolledOutWidgets_
}

//=============================================================================
// libvterm callbacks - DEPRECATED: GPUScreen handles most of these now
// Only onOSC is still needed for widget embedding
//=============================================================================

#if 0 // No longer used - GPUScreen handles via State callbacks
int Terminal::onDamage(VTermRect rect, void* user) {
    auto* term = static_cast<Terminal*>(user);
    DamageRect d;
    d._startCol = rect.start_col;
    d._startRow = rect.start_row;
    d._endCol = rect.end_col;
    d._endRow = rect.end_row;
    term->_damageRects.push_back(d);
    return 0;
}

int Terminal::onMoveCursor(VTermPos pos, VTermPos, int, void* user) {
    auto* term = static_cast<Terminal*>(user);
    term->_cursorRow = pos.row;
    term->_cursorCol = pos.col;
    return 0;
}

int Terminal::onSetTermProp(VTermProp prop, VTermValue* val, void* user) {
    auto* term = static_cast<Terminal*>(user);
    switch (prop) {
        case VTERM_PROP_ALTSCREEN:
            term->_isAltScreen = val->boolean;
            break;
        case VTERM_PROP_CURSORVISIBLE:
            term->_cursorVisible = val->boolean;
            break;
        case VTERM_PROP_MOUSE:
            term->_mouseMode = val->number;
            break;
        default: break;
    }
    return 0;
}

int Terminal::onResize(int, int, void*) { return 0; }

int Terminal::onBell(void*) {
#ifndef _WIN32
    write(STDOUT_FILENO, "\a", 1);
#endif
    return 0;
}

int Terminal::onSbPushline(int cols, const VTermScreenCell* cells, void* user) {
    // Scrollback is handled by GPUScreen callback now
    return 1;
}

int Terminal::onSbPopline(int cols, VTermScreenCell* cells, void* user) {
    // Scrollback pop is not used with GPUScreen
    return 0;
}

int Terminal::onMoverect(VTermRect, VTermRect, void* user) {
    auto* term = static_cast<Terminal*>(user);
    term->_fullDamage = true;
    return 0;
}
#endif // #if 0 - deprecated callbacks

// OSC callback - forwards to GPUScreen for widget handling
int Terminal::onOSC(int command, VTermStringFragment frag, void *user) {
  auto *term = static_cast<Terminal *>(user);

  ydebug("onOSC: command={} initial={} final={} len={}", command,
         (bool)frag.initial, (bool)frag.final, (size_t)frag.len);

  // Handle both legacy plugins and new cards
  if (command != YETTY_OSC_VENDOR_ID && command != YETTY_OSC_VENDOR_CARD_ID) {
    ydebug("onOSC: ignoring non-yetty command {}", command);
    return 0;
  }

  ydebug("onOSC: *** YETTY OSC DETECTED *** command={}", command);

  if (frag.initial) {
    term->_oscBuffer.clear();
    term->_oscCommand = command;
    yinfo("onOSC: started new OSC buffer");
  }

  if (frag.len > 0) {
    term->_oscBuffer.append(frag.str, frag.len);
    yinfo("onOSC: appended {} bytes, total={}", (size_t)frag.len,
           term->_oscBuffer.size());
  }

  if (frag.final && term->_gpuScreen) {
    std::string fullSeq = std::to_string(command) + ";" + term->_oscBuffer;
    yinfo("onOSC: FINAL - fullSeq len={} first100chars='{}'",
          fullSeq.size(), fullSeq.substr(0, 100));

    std::string response;
    uint32_t linesToAdvance = 0;

    // Update GPUScreen's alt screen state
    term->_gpuScreen->setIsAltScreen(term->_isAltScreen);

    // Forward OSC handling to GPUScreen
    bool handled = term->_gpuScreen->handleOSCSequence(fullSeq, &response,
                                                       &linesToAdvance);

    yinfo("onOSC: handled={} response_len={} linesToAdvance={}", handled,
           response.size(), linesToAdvance);

    if (handled) {
      term->_fullDamage = true;
      if (!response.empty() && term->_ptyMaster >= 0) {
        term->writeToPty(response.c_str(), response.size());
      }
      // Inject newlines immediately to advance cursor past the widget
      if (linesToAdvance > 0) {
        std::string nl(linesToAdvance, '\n');
        vterm_input_write(term->_vterm, nl.c_str(), nl.size());
      }
    }
    term->_oscBuffer.clear();
    term->_oscCommand = -1;
  } else if (frag.final) {
    ywarn("onOSC: FINAL but no gpuScreen!");
  }

  return 1;
}

//=============================================================================
// Widget ownership - delegates to GPUScreen
//=============================================================================

void Terminal::addChildWidget(WidgetPtr widget) {
  if (!widget || !_gpuScreen)
    return;
  _gpuScreen->addChildWidget(widget);
}

void Terminal::markWidgetGridCells(Widget *widget) {
  if (!widget || !_gpuScreen)
    return;
  _gpuScreen->markWidgetGridCells(widget);
}

void Terminal::clearWidgetGridCells(Widget *widget) {
  if (!widget || !_gpuScreen)
    return;
  _gpuScreen->clearWidgetGridCells(widget);
}

Result<void> Terminal::removeChildWidget(uint32_t id) {
  if (!_gpuScreen)
    return Err<void>("No GPUScreen");
  return _gpuScreen->removeChildWidget(id);
}

WidgetPtr Terminal::getChildWidget(uint32_t id) const {
  if (!_gpuScreen)
    return nullptr;
  return _gpuScreen->getChildWidget(id);
}

WidgetPtr Terminal::getChildWidgetByHashId(const std::string &hashId) const {
  if (!_gpuScreen)
    return nullptr;
  return _gpuScreen->getChildWidgetByHashId(hashId);
}

Result<void> Terminal::removeChildWidgetByHashId(const std::string &hashId) {
  if (!_gpuScreen)
    return Err<void>("No GPUScreen");
  return _gpuScreen->removeChildWidgetByHashId(hashId);
}

} // namespace yetty
