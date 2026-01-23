#pragma once

#include "yetty/emoji-atlas.h"
#include "yetty/grid.h"
#include "yetty/gpu-screen.h"  // For Cell struct
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <yetty/yetty-font-manager.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/shader-font.h>
#include <yetty/result.hpp>
#include <yetty/webgpu-context.h>

namespace yetty {

class Config;
struct DamageRect;

class GridRenderer {
public:
  using Ptr = std::shared_ptr<GridRenderer>;

  static Result<Ptr> create(WebGPUContext::Ptr ctx,
                            YettyFontManager::Ptr fontManager,
                            WGPUBindGroupLayout sharedBindGroupLayout,
                            ShaderFont::Ptr shaderGlyphFont,
                            ShaderFont::Ptr cardFont,
                            const std::string& fontFamily = "default") noexcept;

  ~GridRenderer();

  // Non-copyable
  GridRenderer(const GridRenderer &) = delete;
  GridRenderer &operator=(const GridRenderer &) = delete;

  void resize(uint32_t width, uint32_t height) noexcept;
  void render(const Grid &grid, int cursorCol = -1, int cursorRow = -1,
              bool cursorVisible = false) noexcept;

  void setCellSize(float width, float height) noexcept;
  void setScale(float scale) noexcept { scale_ = scale; }
  void setSharedBindGroupLayout(WGPUBindGroupLayout layout) noexcept;
  glm::vec2 getCellSize() const noexcept { return cellSize_; }
  float getScale() const noexcept { return scale_; }

  void setConfig(const Config *config) noexcept { config_ = config; }

  // Get emoji atlas for dynamic loading
  EmojiAtlas *getEmojiAtlas() const noexcept { return emojiAtlas_.get(); }

  // Get cell buffer for compute shader access (image atlas)
  WGPUBuffer getCellBuffer() const noexcept { return cellBuffer_; }
  uint32_t getGridCols() const noexcept { return gridCols_; }
  uint32_t getGridRows() const noexcept { return gridRows_; }

  // Upload cells to GPU buffer (for compute shader access before render)
  Result<void> uploadCells(uint32_t cols, uint32_t rows, const Cell* cells) noexcept;

  // Update font bindings (call after font atlas/metadata changes)
  void updateFontBindings(MsMsdfFont &font) noexcept;

  // Render with damage tracking (only updates changed regions)
  void render(const Grid &grid, const std::vector<DamageRect> &damageRects,
              bool fullDamage, int cursorCol = -1, int cursorRow = -1,
              bool cursorVisible = false) noexcept;

  // Render to provided pass (for batched rendering - no encoder/submit)
  Result<void> renderToPass(WGPURenderPassEncoder pass, const Grid &grid,
                            const std::vector<DamageRect> &damageRects,
                            bool fullDamage, int cursorCol = -1, int cursorRow = -1,
                            bool cursorVisible = false) noexcept;

  // Render to existing pass from Cell buffer (zero-copy path)
  Result<void> renderToPassFromCells(WGPURenderPassEncoder pass,
                                     uint32_t cols, uint32_t rows,
                                     const Cell* cells,
                                     bool fullDamage,
                                     int cursorCol, int cursorRow,
                                     bool cursorVisible) noexcept;

  // Render from Cell buffer (creates its own pass, used by RenderGridCmd)
  void renderFromCells(uint32_t cols, uint32_t rows,
                       const Cell* cells,
                       int cursorCol, int cursorRow,
                       bool cursorVisible) noexcept;

private:
  GridRenderer(WebGPUContext::Ptr ctx, YettyFontManager::Ptr fontManager,
               ShaderFont::Ptr shaderGlyphFont, ShaderFont::Ptr cardFont,
               const std::string& fontFamily) noexcept;
  Result<void> init() noexcept;
  Result<void> createShaderModule(WGPUDevice device);
  Result<void> createPipeline(WGPUDevice device, WGPUTextureFormat format);
  Result<void> createBuffers(WGPUDevice device);
  Result<void> createCellBuffer(WGPUDevice device, uint32_t cols, uint32_t rows);
  Result<void> createBindGroupLayout(WGPUDevice device);
  Result<void> createBindGroup(WGPUDevice device, MsMsdfFont &font);

  void updateUniformBuffer(WGPUQueue queue, const Grid &grid, int cursorCol,
                           int cursorRow, bool cursorVisible);
  void updateCellBuffer(WGPUQueue queue, const Cell* cells, uint32_t cols, uint32_t rows);

  // Uniforms struct - must match shader
  struct Uniforms {
    glm::mat4 projection; // 64 bytes, offset 0
    glm::vec2 screenSize; // 8 bytes, offset 64
    glm::vec2 cellSize;   // 8 bytes, offset 72
    glm::vec2 gridSize;   // 8 bytes, offset 80 (cols, rows)
    float pixelRange;     // 4 bytes, offset 88
    float scale;          // 4 bytes, offset 92
    glm::vec2 cursorPos;  // 8 bytes, offset 96 (col, row)
    float cursorVisible;  // 4 bytes, offset 104
    float _pad;           // 4 bytes, offset 108 - padding for alignment
  }; // Total: 112 bytes

  WGPUShaderModule shaderModule_ = nullptr;
  WGPURenderPipeline pipeline_ = nullptr;
  WGPUBindGroupLayout bindGroupLayout_ = nullptr;
  WGPUBindGroupLayout sharedBindGroupLayout_ = nullptr;  // From Yetty (group 0)
  WGPUBindGroup bindGroup_ = nullptr;
  WGPUPipelineLayout pipelineLayout_ = nullptr;

  // Buffers
  WGPUBuffer uniformBuffer_ = nullptr;
  WGPUBuffer quadVertexBuffer_ = nullptr; // Fullscreen quad vertices

  // Cell data storage buffer (12 bytes per cell: glyph u32 + fg RGBA + bg RGB + style)
  WGPUBuffer cellBuffer_ = nullptr;
  size_t cellBufferSize_ = 0;  // Current buffer capacity in bytes

  Uniforms uniforms_;
  glm::vec2 cellSize_ = {10.0f, 20.0f};
  float scale_ = 1.0f;
  uint32_t screenWidth_ = 800;
  uint32_t screenHeight_ = 600;

  uint32_t textureCols_ = 0; // Current texture dimensions
  uint32_t textureRows_ = 0;
  uint32_t gridCols_ = 0;
  uint32_t gridRows_ = 0;

  WebGPUContext::Ptr _ctx;
  YettyFontManager::Ptr fontManager_;
  ShaderFont::Ptr shaderGlyphFont_;  // Single-cell shader glyphs
  ShaderFont::Ptr cardFont_;          // Multi-cell card glyphs
  std::string fontFamily_;
  std::shared_ptr<MsMsdfFont> font_;  // From YettyFontManager
  EmojiAtlas::Ptr emojiAtlas_; // Color emoji atlas
  const Config *config_ = nullptr;
  bool needsBindGroupRecreation_ = false; // Deferred bind group recreation
  uint32_t lastFontResourceVersion_ = 0;  // Track font resource changes
};

} // namespace yetty
