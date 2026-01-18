#include "damage-rect.h"  // For DamageRect (separate to avoid libuv on web)
#include "yetty/grid-renderer.h"
#include "yetty/config.h"
#include <cstdlib> // for getenv
#include <cstring>
#include <filesystem>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <regex>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <yetty/wgpu-compat.h>
#if !YETTY_WEB
#include <webgpu/wgpu.h>  // For wgpuDevicePoll
#endif

#if YETTY_ANDROID
#include <android/log.h>
#define TR_LOGI(...)                                                           \
  __android_log_print(ANDROID_LOG_INFO, "yetty-tr", __VA_ARGS__)
#define TR_LOGE(...)                                                           \
  __android_log_print(ANDROID_LOG_ERROR, "yetty-tr", __VA_ARGS__)
#else
#define TR_LOGI(...)                                                           \
  do {                                                                         \
    yinfo(__VA_ARGS__);                                                 \
  } while (0)
#define TR_LOGE(...)                                                           \
  do {                                                                         \
    yerror(__VA_ARGS__);                                                \
  } while (0)
#endif

namespace yetty {

Result<GridRenderer::Ptr>
GridRenderer::create(WebGPUContext::Ptr ctx,
                     FontManager::Ptr fontManager,
                     WGPUBindGroupLayout sharedBindGroupLayout,
                     const std::string& fontFamily) noexcept {
  if (!ctx) {
    return Err<Ptr>("GridRenderer::create: null WebGPUContext");
  }
  if (!fontManager) {
    return Err<Ptr>("GridRenderer::create: null FontManager");
  }
  if (!sharedBindGroupLayout) {
    return Err<Ptr>("GridRenderer::create: null sharedBindGroupLayout");
  }
  auto renderer = Ptr(new GridRenderer(std::move(ctx), std::move(fontManager), fontFamily));
  renderer->sharedBindGroupLayout_ = sharedBindGroupLayout;
  if (auto res = renderer->init(); !res) {
    return Err<Ptr>("Failed to initialize GridRenderer", res);
  }
  return Ok(std::move(renderer));
}

GridRenderer::GridRenderer(WebGPUContext::Ptr ctx,
                           FontManager::Ptr fontManager,
                           const std::string& fontFamily) noexcept
    : _ctx(std::move(ctx)), fontManager_(std::move(fontManager)), fontFamily_(fontFamily) {}

GridRenderer::~GridRenderer() {
#if !YETTY_WEB
  if (cellBuffer_)
    wgpuBufferRelease(cellBuffer_);
  if (bindGroup_)
    wgpuBindGroupRelease(bindGroup_);
#endif
  if (quadVertexBuffer_)
    wgpuBufferRelease(quadVertexBuffer_);
  if (uniformBuffer_)
    wgpuBufferRelease(uniformBuffer_);
  if (pipelineLayout_)
    wgpuPipelineLayoutRelease(pipelineLayout_);
  if (bindGroupLayout_)
    wgpuBindGroupLayoutRelease(bindGroupLayout_);
  if (pipeline_)
    wgpuRenderPipelineRelease(pipeline_);
  if (shaderModule_)
    wgpuShaderModuleRelease(shaderModule_);
}

Result<void> GridRenderer::init() noexcept {
  WGPUDevice device = _ctx->getDevice();

  // Get terminal font from FontManager
  auto fontResult = fontManager_->getFont(fontFamily_, Font::Regular, 32.0f);
  if (!fontResult) {
    return Err<void>("Failed to get terminal font: " +
                     fontResult.error().message());
  }
  font_ = *fontResult;

  // Create glyph metadata buffer in Font
  if (!font_->createGlyphMetadataBuffer(device)) {
    return Err<void>("Failed to create glyph metadata buffer");
  }

  // Create emoji atlas (NotoColorEmoji uses 136x128 bitmaps, use 136px cells)
  auto emojiResult = EmojiAtlas::create(device, 136);
  if (!emojiResult) {
    // Emoji atlas is optional - log warning but continue
    TR_LOGE("Failed to create emoji atlas: %s (emoji rendering disabled)",
            emojiResult.error().message().c_str());
  } else {
    emojiAtlas_ = *emojiResult;
    // Load common emojis
    if (auto res = emojiAtlas_->loadCommonEmojis(); !res) {
      TR_LOGE("Failed to load common emojis: %s",
              res.error().message().c_str());
    } else {
      TR_LOGI("Emoji atlas initialized with {} emojis",
              emojiAtlas_->getGlyphCount());
    }
  }

  if (auto res = createShaderModule(device); !res) {
    return Err<void>("Failed to create shader module", res);
  }
  if (auto res = createBuffers(device); !res) {
    return Err<void>("Failed to create buffers", res);
  }
  if (auto res = createBindGroupLayout(device); !res) {
    return Err<void>("Failed to create bind group layout", res);
  }
  if (auto res = createPipeline(device, _ctx->getSurfaceFormat()); !res) {
    return Err<void>("Failed to create pipeline", res);
  }

  return Ok();
}

// Scan for shader glyph files and build concatenated shader source
static std::string loadShaderGlyphs(const std::string& shaderDir) {
  std::string functions;
  std::string dispatch;

  // Pattern: decimal codepoint followed by name, e.g. "1048577-spinner.wgsl"
  std::regex pattern(R"(^(\d+)-.*\.wgsl$)");

  std::vector<std::pair<uint32_t, std::string>> glyphs;

#if !YETTY_WEB && !YETTY_ANDROID
  try {
    for (const auto& entry : std::filesystem::directory_iterator(shaderDir)) {
      if (!entry.is_regular_file()) continue;

      std::string filename = entry.path().filename().string();
      std::smatch match;
      if (std::regex_match(filename, match, pattern)) {
        uint32_t codepoint = static_cast<uint32_t>(std::stoul(match[1].str()));

        // Read shader glyph file
        std::ifstream file(entry.path());
        if (file.is_open()) {
          std::stringstream buf;
          buf << file.rdbuf();
          glyphs.emplace_back(codepoint, buf.str());
          yinfo("Loaded shader glyph: {} (codepoint {})", filename, codepoint);
        }
      }
    }
  } catch (const std::exception& e) {
    yerror("Error scanning shader glyphs: {}", e.what());
  }
#endif

  // Sort by codepoint for consistent ordering
  std::sort(glyphs.begin(), glyphs.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

  // Build functions and dispatch
  for (const auto& [codepoint, code] : glyphs) {
    functions += "// Shader glyph codepoint " + std::to_string(codepoint) + "\n";
    functions += code + "\n\n";

    // Add dispatch case
    if (!dispatch.empty()) {
      dispatch += " else ";
    }
    dispatch += "if (glyphIndex == " + std::to_string(codepoint) + "u) {\n";
    dispatch += "        return shaderGlyph_" + std::to_string(codepoint) +
                "(localUV, time, fgColor, bgColor, pixelPos);\n    }";
  }

  if (!dispatch.empty()) {
    dispatch += "\n    ";
  }

  yinfo("Loaded {} shader glyphs", glyphs.size());

  return "FUNCTIONS:" + functions + "\nDISPATCH:" + dispatch;
}

Result<void> GridRenderer::createShaderModule(WGPUDevice device) {
#if YETTY_WEB
  const char *shaderPath = "/shaders.wgsl";
  const char *shaderDir = "/";
#elif YETTY_ANDROID
  // On Android, shader is extracted to app's data directory
  const char *envPath = std::getenv("YETTY_SHADER_PATH");
  const char *shaderPath = envPath ? envPath : "/data/local/tmp/shaders.wgsl";
  const char *shaderDir = "/data/local/tmp/";
  yinfo("Loading shader from: {}", shaderPath);
#else
  const char *shaderPath = CMAKE_SOURCE_DIR "/src/yetty/shaders/shaders.wgsl";
  const char *shaderDir = CMAKE_SOURCE_DIR "/src/yetty/shaders/";
#endif

  std::ifstream file(shaderPath);
  if (!file.is_open()) {
    return Err<void>(std::string("Failed to open shader: ") + shaderPath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string shaderSource = buffer.str();

  // Load and inject shader glyphs
  std::string glyphData = loadShaderGlyphs(shaderDir);
  size_t funcSep = glyphData.find("\nDISPATCH:");
  std::string functions = glyphData.substr(10, funcSep - 10);  // Skip "FUNCTIONS:"
  std::string dispatch = glyphData.substr(funcSep + 10);       // Skip "\nDISPATCH:"

  // Replace placeholders
  size_t funcPos = shaderSource.find("// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER");
  if (funcPos != std::string::npos) {
    shaderSource.replace(funcPos, 37, functions);
  }

  size_t dispPos = shaderSource.find("// SHADER_GLYPH_DISPATCH_PLACEHOLDER");
  if (dispPos != std::string::npos) {
    shaderSource.replace(dispPos, 36, dispatch);
  }

  WGPUShaderSourceWGSL wgslDesc = {};
  wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
  WGPU_SHADER_CODE(wgslDesc, shaderSource);

  WGPUShaderModuleDescriptor moduleDesc = {};
  moduleDesc.nextInChain = &wgslDesc.chain;
  moduleDesc.label = WGPU_STR("text shader");

  shaderModule_ = wgpuDeviceCreateShaderModule(device, &moduleDesc);
  if (!shaderModule_) {
    return Err<void>("Failed to create shader module");
  }

  return Ok();
}

Result<void> GridRenderer::createBuffers(WGPUDevice device) {
  // Uniform buffer
  WGPUBufferDescriptor uniformDesc = {};
  uniformDesc.label = WGPU_STR("uniforms");
  uniformDesc.size = sizeof(Uniforms);
  uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
  uniformBuffer_ = wgpuDeviceCreateBuffer(device, &uniformDesc);
  if (!uniformBuffer_) {
    return Err<void>("Failed to create uniform buffer");
  }

  // Fullscreen quad vertices (2 triangles, 6 vertices)
  float quadVertices[] = {
      -1.0f, -1.0f, // bottom-left
      1.0f,  -1.0f, // bottom-right
      -1.0f, 1.0f,  // top-left
      -1.0f, 1.0f,  // top-left
      1.0f,  -1.0f, // bottom-right
      1.0f,  1.0f,  // top-right
  };

  WGPUBufferDescriptor quadDesc = {};
  quadDesc.label = WGPU_STR("quad vertices");
  quadDesc.size = sizeof(quadVertices);
  quadDesc.usage = WGPUBufferUsage_Vertex;
  quadDesc.mappedAtCreation = true;
  quadVertexBuffer_ = wgpuDeviceCreateBuffer(device, &quadDesc);
  if (!quadVertexBuffer_) {
    return Err<void>("Failed to create quad vertex buffer");
  }
  void *mapped =
      wgpuBufferGetMappedRange(quadVertexBuffer_, 0, sizeof(quadVertices));
  memcpy(mapped, quadVertices, sizeof(quadVertices));
  wgpuBufferUnmap(quadVertexBuffer_);

  return Ok();
}

Result<void> GridRenderer::createCellBuffer(WGPUDevice device, uint32_t cols,
                                              uint32_t rows) {
  // Calculate required buffer size (12 bytes per Cell)
  size_t requiredSize = static_cast<size_t>(cols) * rows * sizeof(Cell);

#if YETTY_WEB
  // On web, only create buffer once with large fixed size
  if (cellBuffer_) {
    textureCols_ = cols;
    textureRows_ = rows;
    return Ok();
  }
  // First creation - use large fixed size to avoid needing recreation
  cols = 200;
  rows = 100;
  requiredSize = static_cast<size_t>(cols) * rows * sizeof(Cell);
#else
  // Only recreate if buffer is too small
  if (cellBuffer_ && cellBufferSize_ >= requiredSize) {
    textureCols_ = cols;
    textureRows_ = rows;
    return Ok();
  }

  // Wait for GPU to finish using old buffer before releasing
  if (cellBuffer_) {
    wgpuDevicePoll(device, true, nullptr);
    wgpuBufferRelease(cellBuffer_);
    cellBuffer_ = nullptr;
  }
#endif

  textureCols_ = cols;
  textureRows_ = rows;

  // Create storage buffer for cell data (zero-copy upload from CPU)
  WGPUBufferDescriptor bufferDesc = {};
  bufferDesc.label = WGPU_STR("cell buffer");
  bufferDesc.size = requiredSize;
  bufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
  cellBuffer_ = wgpuDeviceCreateBuffer(device, &bufferDesc);
  if (!cellBuffer_) {
    return Err<void>("Failed to create cell buffer");
  }
  cellBufferSize_ = requiredSize;

  return Ok();
}

Result<void> GridRenderer::createBindGroupLayout(WGPUDevice device) {
  // Bind group layout: 8 bindings
  // 0: Uniforms, 1: Font texture, 2: Font sampler, 3: Glyph metadata
  // 4: Cell buffer (storage), 5: Emoji texture, 6: Emoji sampler, 7: Emoji metadata
  WGPUBindGroupLayoutEntry entries[8] = {};

  // 0: Uniforms
  entries[0].binding = 0;
  entries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
  entries[0].buffer.type = WGPUBufferBindingType_Uniform;
  entries[0].buffer.minBindingSize = sizeof(Uniforms);

  // 1: Font atlas texture
  entries[1].binding = 1;
  entries[1].visibility = WGPUShaderStage_Fragment;
  entries[1].texture.sampleType = WGPUTextureSampleType_Float;
  entries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

  // 2: Font sampler
  entries[2].binding = 2;
  entries[2].visibility = WGPUShaderStage_Fragment;
  entries[2].sampler.type = WGPUSamplerBindingType_Filtering;

  // 3: Glyph metadata SSBO
  entries[3].binding = 3;
  entries[3].visibility = WGPUShaderStage_Fragment;
  entries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
  entries[3].buffer.minBindingSize = sizeof(GlyphMetadataGPU);

  // 4: Cell buffer SSBO (12 bytes per cell: glyph u32 + fg RGBA + bg RGB + style)
  entries[4].binding = 4;
  entries[4].visibility = WGPUShaderStage_Fragment;
  entries[4].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
  entries[4].buffer.minBindingSize = sizeof(Cell);  // Minimum 1 cell

  // 5: Emoji atlas texture (RGBA8Unorm - color emoji bitmap)
  entries[5].binding = 5;
  entries[5].visibility = WGPUShaderStage_Fragment;
  entries[5].texture.sampleType = WGPUTextureSampleType_Float;
  entries[5].texture.viewDimension = WGPUTextureViewDimension_2D;

  // 6: Emoji sampler
  entries[6].binding = 6;
  entries[6].visibility = WGPUShaderStage_Fragment;
  entries[6].sampler.type = WGPUSamplerBindingType_Filtering;

  // 7: Emoji metadata SSBO
  entries[7].binding = 7;
  entries[7].visibility = WGPUShaderStage_Fragment;
  entries[7].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;
  entries[7].buffer.minBindingSize = sizeof(EmojiGlyphMetadata);

  WGPUBindGroupLayoutDescriptor layoutDesc = {};
  layoutDesc.entryCount = 8;
  layoutDesc.entries = entries;
  bindGroupLayout_ = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);
  if (!bindGroupLayout_) {
    return Err<void>("Failed to create bind group layout");
  }

  // Pipeline layout - includes both shared (group 0) and grid (group 1) layouts
  // Note: sharedBindGroupLayout_ must be set via setSharedBindGroupLayout before pipeline creation
  if (!sharedBindGroupLayout_) {
    return Err<void>("sharedBindGroupLayout_ not set - call setSharedBindGroupLayout first");
  }
  WGPUBindGroupLayout layouts[2] = { sharedBindGroupLayout_, bindGroupLayout_ };
  WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
  pipelineLayoutDesc.bindGroupLayoutCount = 2;
  pipelineLayoutDesc.bindGroupLayouts = layouts;
  pipelineLayout_ = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);
  if (!pipelineLayout_) {
    return Err<void>("Failed to create pipeline layout");
  }

  return Ok();
}

void GridRenderer::setSharedBindGroupLayout(WGPUBindGroupLayout layout) noexcept {
  sharedBindGroupLayout_ = layout;
}

Result<void> GridRenderer::createBindGroup(WGPUDevice device, Font &font) {
#if YETTY_WEB
  // On web, only create bind group once to avoid Emscripten WebGPU manager issues
  if (bindGroup_) {
    return Ok();
  }
#else
  // Release old bind group if it exists (for recreation when buffer changes)
  if (bindGroup_) {
    wgpuBindGroupRelease(bindGroup_);
    bindGroup_ = nullptr;
  }
#endif

  // Validate required resources exist
  if (!bindGroupLayout_)
    return Err<void>("bindGroupLayout_ is null");
  if (!uniformBuffer_)
    return Err<void>("uniformBuffer_ is null");
  if (!font.getTextureView())
    return Err<void>("font texture view is null");
  if (!font.getSampler())
    return Err<void>("font sampler is null");
  if (!font.getGlyphMetadataBuffer())
    return Err<void>("glyph metadata buffer is null");
  if (!cellBuffer_)
    return Err<void>("cellBuffer_ is null - call render() first");

  // Check emoji atlas resources (required for bind group)
  if (!emojiAtlas_ || !emojiAtlas_->getTextureView() ||
      !emojiAtlas_->getSampler() || !emojiAtlas_->getMetadataBuffer()) {
    return Err<void>("emoji atlas resources not ready");
  }

  // Bind group entries (8 entries)
  WGPUBindGroupEntry bgEntries[8] = {};

  bgEntries[0].binding = 0;
  bgEntries[0].buffer = uniformBuffer_;
  bgEntries[0].size = sizeof(Uniforms);

  bgEntries[1].binding = 1;
  bgEntries[1].textureView = font.getTextureView();

  bgEntries[2].binding = 2;
  bgEntries[2].sampler = font.getSampler();

  bgEntries[3].binding = 3;
  bgEntries[3].buffer = font.getGlyphMetadataBuffer();
  bgEntries[3].size = font.getBufferGlyphCount() * sizeof(GlyphMetadataGPU);

  // Cell buffer - storage buffer with Cell structs (12 bytes each)
  bgEntries[4].binding = 4;
  bgEntries[4].buffer = cellBuffer_;
  bgEntries[4].size = cellBufferSize_;

  // Emoji atlas resources
  bgEntries[5].binding = 5;
  bgEntries[5].textureView = emojiAtlas_->getTextureView();

  bgEntries[6].binding = 6;
  bgEntries[6].sampler = emojiAtlas_->getSampler();

  bgEntries[7].binding = 7;
  bgEntries[7].buffer = emojiAtlas_->getMetadataBuffer();
  // Use full buffer size to allow dynamic emoji loading
  uint32_t maxEmojis =
      (emojiAtlas_->getAtlasSize() / emojiAtlas_->getGlyphSize());
  maxEmojis = maxEmojis * maxEmojis; // glyphsPerRow^2
  bgEntries[7].size = maxEmojis * sizeof(EmojiGlyphMetadata);

  WGPUBindGroupDescriptor bindGroupDesc = {};
  bindGroupDesc.layout = bindGroupLayout_;
  bindGroupDesc.entryCount = 8;
  bindGroupDesc.entries = bgEntries;
  bindGroup_ = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
  if (!bindGroup_) {
    return Err<void>("Failed to create bind group");
  }

  return Ok();
}

Result<void> GridRenderer::createPipeline(WGPUDevice device,
                                          WGPUTextureFormat format) {
  // Vertex attributes: just position (vec2)
  WGPUVertexAttribute posAttr = {};
  posAttr.format = WGPUVertexFormat_Float32x2;
  posAttr.offset = 0;
  posAttr.shaderLocation = 0;

  WGPUVertexBufferLayout vertexLayout = {};
  vertexLayout.arrayStride = 2 * sizeof(float);
  vertexLayout.stepMode = WGPUVertexStepMode_Vertex;
  vertexLayout.attributeCount = 1;
  vertexLayout.attributes = &posAttr;

  // Pipeline descriptor
  WGPURenderPipelineDescriptor pipelineDesc = {};
  pipelineDesc.label = WGPU_STR("text pipeline");
  pipelineDesc.layout = pipelineLayout_;

  pipelineDesc.vertex.module = shaderModule_;
  pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
  pipelineDesc.vertex.bufferCount = 1;
  pipelineDesc.vertex.buffers = &vertexLayout;

  // Fragment state with blending (for MSDF alpha)
  WGPUBlendState blendState = {};
  blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
  blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
  blendState.color.operation = WGPUBlendOperation_Add;
  blendState.alpha.srcFactor = WGPUBlendFactor_One;
  blendState.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
  blendState.alpha.operation = WGPUBlendOperation_Add;

  WGPUColorTargetState colorTarget = {};
  colorTarget.format = format;
  colorTarget.blend = &blendState;
  colorTarget.writeMask = WGPUColorWriteMask_All;

  WGPUFragmentState fragState = {};
  fragState.module = shaderModule_;
  fragState.entryPoint = WGPU_STR("fs_main");
  fragState.targetCount = 1;
  fragState.targets = &colorTarget;
  pipelineDesc.fragment = &fragState;

  pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
  pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
  pipelineDesc.primitive.cullMode = WGPUCullMode_None;

  pipelineDesc.multisample.count = 1;
  pipelineDesc.multisample.mask = 0xFFFFFFFF;

  pipeline_ = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
  if (!pipeline_) {
    return Err<void>("Failed to create render pipeline");
  }

  return Ok();
}

void GridRenderer::resize(uint32_t width, uint32_t height) noexcept {
  screenWidth_ = width;
  screenHeight_ = height;
}

void GridRenderer::setCellSize(float width, float height) noexcept {
  cellSize_ = {width, height};
}

void GridRenderer::updateFontBindings(Font &font) noexcept {
  if (!_ctx)
    return;

  font_ = &font;

  // Only recreate bind group if cell buffer already exists
  // Otherwise, render() will create both buffer and bind group
  if (!cellBuffer_) {
    // Mark that bind group needs recreation on next render
    needsBindGroupRecreation_ = true;
    return;
  }

  // Recreate bind group with updated font resources
  auto result = createBindGroup(_ctx->getDevice(), font);
  if (!result) {
    std::cerr << "Failed to update font bindings: " << error_msg(result)
              << std::endl;
  } else {
    needsBindGroupRecreation_ = false;
  }
}

void GridRenderer::updateUniformBuffer(WGPUQueue queue, const Grid &grid,
                                       int cursorCol, int cursorRow,
                                       bool cursorVisible) {
  uniforms_.projection =
      glm::ortho(0.0f, static_cast<float>(screenWidth_),
                 static_cast<float>(screenHeight_), 0.0f, -1.0f, 1.0f);
  uniforms_.screenSize = {static_cast<float>(screenWidth_),
                          static_cast<float>(screenHeight_)};
  uniforms_.cellSize = cellSize_;
  uniforms_.gridSize = {static_cast<float>(grid.getCols()),
                        static_cast<float>(grid.getRows())};
  uniforms_.pixelRange = font_ ? font_->getPixelRange() : 2.0f;
  uniforms_.scale = scale_;
  uniforms_.cursorPos = {static_cast<float>(cursorCol),
                         static_cast<float>(cursorRow)};
  uniforms_.cursorVisible = cursorVisible ? 1.0f : 0.0f;
  // Note: time is set by Yetty::mainLoopIteration via setTime()

  wgpuQueueWriteBuffer(queue, uniformBuffer_, 0, &uniforms_, sizeof(Uniforms));
}

void GridRenderer::updateCellBuffer(WGPUQueue queue, const Cell* cells,
                                    uint32_t cols, uint32_t rows) {
  // Guard against zero-sized grids
  if (cols == 0 || rows == 0 || !cells) {
    return;
  }

  gridCols_ = cols;
  gridRows_ = rows;

  // Zero-copy upload: Cell buffer goes directly to GPU storage buffer
  size_t dataSize = static_cast<size_t>(cols) * rows * sizeof(Cell);
  wgpuQueueWriteBuffer(queue, cellBuffer_, 0, cells, dataSize);
}

// Helper: convert Grid's separate buffers to Cell format for legacy Grid-based rendering
static void convertGridToCells(const Grid& grid, std::vector<Cell>& cells) {
  const uint32_t cols = grid.getCols();
  const uint32_t rows = grid.getRows();
  const size_t cellCount = static_cast<size_t>(cols) * rows;

  cells.resize(cellCount);

  const uint16_t* glyphs = grid.getGlyphData();
  const uint8_t* fg = grid.getFgColorData();
  const uint8_t* bg = grid.getBgColorData();
  const uint8_t* attrs = grid.getAttrsData();

  for (size_t i = 0; i < cellCount; ++i) {
    cells[i].glyph = glyphs[i];  // 16-bit to 32-bit
    cells[i].fgR = fg[i * 4 + 0];
    cells[i].fgG = fg[i * 4 + 1];
    cells[i].fgB = fg[i * 4 + 2];
    cells[i].alpha = fg[i * 4 + 3];
    cells[i].bgR = bg[i * 4 + 0];
    cells[i].bgG = bg[i * 4 + 1];
    cells[i].bgB = bg[i * 4 + 2];
    cells[i].style = attrs[i];
  }
}

void GridRenderer::render(const Grid &grid, int cursorCol, int cursorRow,
                          bool cursorVisible) noexcept {
  static int frameCount = 0;
  frameCount++;

  WGPUDevice device = _ctx->getDevice();
  WGPUQueue queue = _ctx->getQueue();

  // Upload any newly loaded emojis to GPU
  if (emojiAtlas_) {
    emojiAtlas_->uploadToGPU();
  }
  const uint32_t cols = grid.getCols();
  const uint32_t rows = grid.getRows();

#if YETTY_ANDROID
  if (frameCount <= 3) {
    TR_LOGI("render() frame %d: grid %ux%u, cursor (%d,%d)", frameCount, cols,
            rows, cursorCol, cursorRow);
  }
#endif

  // Recreate buffer and bind group if grid size changed
  if (cols != textureCols_ || rows != textureRows_) {
    TR_LOGI("Creating cell buffer: %ux%u", cols, rows);
    if (auto res = createCellBuffer(device, cols, rows); !res) {
      TR_LOGE("GridRenderer: %s", error_msg(res).c_str());
      return;
    }
    if (auto res = createBindGroup(device, *font_); !res) {
      TR_LOGE("GridRenderer: %s", error_msg(res).c_str());
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    TR_LOGI("Cell buffer and bind group created successfully");
  } else if (needsBindGroupRecreation_ ||
             font_->getResourceVersion() != lastFontResourceVersion_) {
    if (auto res = createBindGroup(device, *font_); !res) {
      TR_LOGE("GridRenderer: %s", error_msg(res).c_str());
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
  }

  updateUniformBuffer(queue, grid, cursorCol, cursorRow, cursorVisible);

  // Convert Grid's separate arrays to Cell format and upload
  static thread_local std::vector<Cell> cellBuffer;
  convertGridToCells(grid, cellBuffer);
  updateCellBuffer(queue, cellBuffer.data(), cols, rows);

  auto targetViewResult = _ctx->getCurrentTextureView();
  if (!targetViewResult) {
    TR_LOGE("GridRenderer: getCurrentTextureView failed: %s",
            error_msg(targetViewResult).c_str());
    return;
  }
  WGPUTextureView targetView = *targetViewResult;

#if YETTY_ANDROID
  if (frameCount <= 3) {
    TR_LOGI("Frame %d: targetView=%p, pipeline=%p, bindGroup=%p", frameCount,
            (void *)targetView, (void *)pipeline_, (void *)bindGroup_);
  }
#else
  if (frameCount <= 3) {
    yinfo("Frame {}: targetView={}, pipeline={}, bindGroup={}",
                 frameCount, (void *)targetView, (void *)pipeline_,
                 (void *)bindGroup_);
  }
#endif

  WGPUCommandEncoderDescriptor encoderDesc = {};
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  WGPURenderPassColorAttachment colorAttachment = {};
  colorAttachment.view = targetView;
  colorAttachment.loadOp = WGPULoadOp_Clear;
  colorAttachment.storeOp = WGPUStoreOp_Store;
  WGPU_COLOR_ATTACHMENT_CLEAR(colorAttachment, 0.1, 0.1, 0.1, 1.0);
  colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

  WGPURenderPassDescriptor passDesc = {};
  passDesc.colorAttachmentCount = 1;
  passDesc.colorAttachments = &colorAttachment;

  WGPURenderPassEncoder pass =
      wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);

  wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, bindGroup_, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(pass, 0, quadVertexBuffer_, 0,
                                       WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

  wgpuRenderPassEncoderEnd(pass);
  wgpuRenderPassEncoderRelease(pass);

  WGPUCommandBufferDescriptor cmdDesc = {};
  WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
  wgpuCommandEncoderRelease(encoder);

  wgpuQueueSubmit(queue, 1, &cmdBuffer);
  wgpuCommandBufferRelease(cmdBuffer);

#if YETTY_ANDROID
  if (frameCount <= 3) {
    TR_LOGI("Frame %d: render pass completed, command buffer submitted",
            frameCount);
  }
#endif
}

void GridRenderer::render(const Grid &grid,
                          const std::vector<DamageRect> &damageRects,
                          bool fullDamage, int cursorCol, int cursorRow,
                          bool cursorVisible) noexcept {
  WGPUDevice device = _ctx->getDevice();
  WGPUQueue queue = _ctx->getQueue();

  // Upload any newly loaded emojis to GPU
  if (emojiAtlas_) {
    emojiAtlas_->uploadToGPU();
  }
  const uint32_t cols = grid.getCols();
  const uint32_t rows = grid.getRows();

  // Recreate buffer and bind group if grid size changed
  if (cols != textureCols_ || rows != textureRows_) {
    if (auto res = createCellBuffer(device, cols, rows); !res) {
      std::cerr << "GridRenderer: " << error_msg(res) << std::endl;
      return;
    }
    if (auto res = createBindGroup(device, *font_); !res) {
      std::cerr << "GridRenderer: " << error_msg(res) << std::endl;
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;  // Force full update when buffer is recreated
  } else if (needsBindGroupRecreation_ ||
             font_->getResourceVersion() != lastFontResourceVersion_) {
    if (auto res = createBindGroup(device, *font_); !res) {
      std::cerr << "GridRenderer: " << error_msg(res) << std::endl;
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;
  }

  // Convert Grid to Cell format and upload when there's damage
  // Note: With storage buffer, partial damage tracking is complex, so always do full upload
  if (fullDamage || !damageRects.empty()) {
    static thread_local std::vector<Cell> cellBuffer;
    convertGridToCells(grid, cellBuffer);
    updateCellBuffer(queue, cellBuffer.data(), cols, rows);
  }

  updateUniformBuffer(queue, grid, cursorCol, cursorRow, cursorVisible);

  auto targetViewResult = _ctx->getCurrentTextureView();
  if (!targetViewResult) {
    std::cerr << "GridRenderer: " << error_msg(targetViewResult) << std::endl;
    return;
  }
  WGPUTextureView targetView = *targetViewResult;

  WGPUCommandEncoderDescriptor encoderDesc = {};
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  WGPURenderPassColorAttachment colorAttachment = {};
  colorAttachment.view = targetView;
  colorAttachment.loadOp = WGPULoadOp_Clear;
  colorAttachment.storeOp = WGPUStoreOp_Store;
  WGPU_COLOR_ATTACHMENT_CLEAR(colorAttachment, 0.1, 0.1, 0.1, 1.0);
  colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

  WGPURenderPassDescriptor passDesc = {};
  passDesc.colorAttachmentCount = 1;
  passDesc.colorAttachments = &colorAttachment;

  WGPURenderPassEncoder pass =
      wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);

  wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, bindGroup_, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(pass, 0, quadVertexBuffer_, 0,
                                       WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

  wgpuRenderPassEncoderEnd(pass);
  wgpuRenderPassEncoderRelease(pass);

  WGPUCommandBufferDescriptor cmdDesc = {};
  WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
  wgpuCommandEncoderRelease(encoder);

  wgpuQueueSubmit(queue, 1, &cmdBuffer);
  wgpuCommandBufferRelease(cmdBuffer);
}

Result<void> GridRenderer::renderToPass(WGPURenderPassEncoder pass, const Grid &grid,
                                        const std::vector<DamageRect> &damageRects,
                                        bool fullDamage, int cursorCol, int cursorRow,
                                        bool cursorVisible) noexcept {
  if (!_ctx) return Err<void>("GridRenderer::renderToPass: context is null");

  WGPUDevice device = _ctx->getDevice();
  WGPUQueue queue = _ctx->getQueue();

  // Upload any newly loaded emojis to GPU
  if (emojiAtlas_) {
    emojiAtlas_->uploadToGPU();
  }
  const uint32_t cols = grid.getCols();
  const uint32_t rows = grid.getRows();

  // Recreate buffer and bind group if grid size changed
  if (cols != textureCols_ || rows != textureRows_) {
    if (auto res = createCellBuffer(device, cols, rows); !res) {
      return Err<void>("GridRenderer::renderToPass: createCellBuffer failed", res);
    }
    if (auto res = createBindGroup(device, *font_); !res) {
      return Err<void>("GridRenderer::renderToPass: createBindGroup failed", res);
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;
  } else if (needsBindGroupRecreation_ ||
             font_->getResourceVersion() != lastFontResourceVersion_) {
    if (auto res = createBindGroup(device, *font_); !res) {
      return Err<void>("GridRenderer::renderToPass: createBindGroup failed", res);
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;
  }

  // Convert Grid to Cell format and upload when there's damage
  if (fullDamage || !damageRects.empty()) {
    static thread_local std::vector<Cell> cellBuffer;
    convertGridToCells(grid, cellBuffer);
    updateCellBuffer(queue, cellBuffer.data(), cols, rows);
  }

  updateUniformBuffer(queue, grid, cursorCol, cursorRow, cursorVisible);

  // Draw to provided pass
  if (!pipeline_) return Err<void>("GridRenderer::renderToPass: pipeline not initialized");
  if (!bindGroup_) return Err<void>("GridRenderer::renderToPass: bindGroup not initialized");

  wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, bindGroup_, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(pass, 0, quadVertexBuffer_, 0,
                                       WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
  return Ok();
}

Result<void> GridRenderer::renderToPassFromCells(WGPURenderPassEncoder pass,
                                                 uint32_t cols, uint32_t rows,
                                                 const Cell* cells,
                                                 bool fullDamage,
                                                 int cursorCol, int cursorRow,
                                                 bool cursorVisible) noexcept {
  if (!_ctx) return Err<void>("GridRenderer::renderToPassFromCells: context is null");
  if (!font_) return Err<void>("GridRenderer::renderToPassFromCells: font is null");
  if (!cells) return Err<void>("GridRenderer::renderToPassFromCells: cells is null");

  WGPUDevice device = _ctx->getDevice();
  WGPUQueue queue = _ctx->getQueue();

  // Upload any newly loaded emojis to GPU
  if (emojiAtlas_) {
    emojiAtlas_->uploadToGPU();
  }

  // Recreate buffer and bind group if grid size changed
  if (cols != textureCols_ || rows != textureRows_) {
    if (auto res = createCellBuffer(device, cols, rows); !res) {
      return Err<void>("GridRenderer::renderToPassFromCells: createCellBuffer failed", res);
    }
    if (auto res = createBindGroup(device, *font_); !res) {
      return Err<void>("GridRenderer::renderToPassFromCells: createBindGroup failed", res);
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;  // Force full upload on resize
  } else if (needsBindGroupRecreation_ ||
             font_->getResourceVersion() != lastFontResourceVersion_) {
    if (auto res = createBindGroup(device, *font_); !res) {
      return Err<void>("GridRenderer::renderToPassFromCells: createBindGroup failed", res);
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
    fullDamage = true;
  }

  // Only upload cell buffer when there's damage (zero-copy from GPUScreen)
  if (fullDamage) {
    updateCellBuffer(queue, cells, cols, rows);
  }

  // Update uniforms (time is set by Yetty::mainLoopIteration via setTime())
  uniforms_.projection = glm::ortho(0.0f, static_cast<float>(screenWidth_),
                                    static_cast<float>(screenHeight_), 0.0f, -1.0f, 1.0f);
  uniforms_.screenSize = {static_cast<float>(screenWidth_), static_cast<float>(screenHeight_)};
  uniforms_.cellSize = cellSize_;
  uniforms_.gridSize = {static_cast<float>(cols), static_cast<float>(rows)};
  uniforms_.pixelRange = font_ ? font_->getPixelRange() : 2.0f;
  uniforms_.scale = scale_;
  uniforms_.cursorPos = {static_cast<float>(cursorCol), static_cast<float>(cursorRow)};
  uniforms_.cursorVisible = cursorVisible ? 1.0f : 0.0f;
  wgpuQueueWriteBuffer(queue, uniformBuffer_, 0, &uniforms_, sizeof(Uniforms));

  // Draw to provided pass
  if (!pipeline_) return Err<void>("GridRenderer::renderToPassFromCells: pipeline not initialized");
  if (!bindGroup_) return Err<void>("GridRenderer::renderToPassFromCells: bindGroup not initialized");

  wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, bindGroup_, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(pass, 0, quadVertexBuffer_, 0,
                                       WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
  return Ok();
}

void GridRenderer::renderFromCells(uint32_t cols, uint32_t rows,
                                   const Cell* cells,
                                   int cursorCol, int cursorRow,
                                   bool cursorVisible) noexcept {
  if (!_ctx || !font_ || !cells) return;

  WGPUDevice device = _ctx->getDevice();
  WGPUQueue queue = _ctx->getQueue();

  // Upload any newly loaded emojis to GPU
  if (emojiAtlas_) {
    emojiAtlas_->uploadToGPU();
  }

  // Recreate buffer and bind group if grid size changed
  if (cols != textureCols_ || rows != textureRows_) {
    if (auto res = createCellBuffer(device, cols, rows); !res) {
      yerror("GridRenderer::renderFromCells: {}", error_msg(res));
      return;
    }
    if (auto res = createBindGroup(device, *font_); !res) {
      yerror("GridRenderer::renderFromCells: {}", error_msg(res));
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
  } else if (needsBindGroupRecreation_ ||
             font_->getResourceVersion() != lastFontResourceVersion_) {
    if (auto res = createBindGroup(device, *font_); !res) {
      yerror("GridRenderer::renderFromCells: {}", error_msg(res));
      return;
    }
    needsBindGroupRecreation_ = false;
    lastFontResourceVersion_ = font_->getResourceVersion();
  }

  // Zero-copy upload of Cell buffer
  updateCellBuffer(queue, cells, cols, rows);

  // Update uniforms (time is set by Yetty::mainLoopIteration via setTime())
  uniforms_.projection = glm::ortho(0.0f, static_cast<float>(screenWidth_),
                                    static_cast<float>(screenHeight_), 0.0f, -1.0f, 1.0f);
  uniforms_.screenSize = {static_cast<float>(screenWidth_), static_cast<float>(screenHeight_)};
  uniforms_.cellSize = cellSize_;
  uniforms_.gridSize = {static_cast<float>(cols), static_cast<float>(rows)};
  uniforms_.pixelRange = font_ ? font_->getPixelRange() : 2.0f;
  uniforms_.scale = scale_;
  uniforms_.cursorPos = {static_cast<float>(cursorCol), static_cast<float>(cursorRow)};
  uniforms_.cursorVisible = cursorVisible ? 1.0f : 0.0f;
  wgpuQueueWriteBuffer(queue, uniformBuffer_, 0, &uniforms_, sizeof(Uniforms));

  // Render
  auto targetViewResult = _ctx->getCurrentTextureView();
  if (!targetViewResult) {
    yerror("GridRenderer::renderFromCells: getCurrentTextureView failed");
    return;
  }
  WGPUTextureView targetView = *targetViewResult;

  WGPUCommandEncoderDescriptor encoderDesc = {};
  WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  WGPURenderPassColorAttachment colorAttachment = {};
  colorAttachment.view = targetView;
  colorAttachment.loadOp = WGPULoadOp_Clear;
  colorAttachment.storeOp = WGPUStoreOp_Store;
#if YETTY_WEB
  WGPU_COLOR_ATTACHMENT_CLEAR(colorAttachment, 0.0, 0.0, 0.0, 1.0);
#else
  colorAttachment.clearValue = {0.0, 0.0, 0.0, 1.0};
  colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif

  WGPURenderPassDescriptor passDesc = {};
  passDesc.colorAttachmentCount = 1;
  passDesc.colorAttachments = &colorAttachment;

  WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
  wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, bindGroup_, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(pass, 0, quadVertexBuffer_, 0, WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
  wgpuRenderPassEncoderEnd(pass);
  wgpuRenderPassEncoderRelease(pass);

  WGPUCommandBufferDescriptor cmdDesc = {};
  WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
  wgpuCommandEncoderRelease(encoder);

  wgpuQueueSubmit(queue, 1, &cmdBuffer);
  wgpuCommandBufferRelease(cmdBuffer);
}

} // namespace yetty
