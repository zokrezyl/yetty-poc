// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "triangulate.h"
#include "ypaint-buffer.h"
#include "ypaint-types.gen.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <unordered_map>
#include <yetty/card-texture-manager.h>
#include <yetty/msdf-glyph-data.h> // For GlyphMetadataGPU
#include <yetty/ypaint/painter.h>
#include <ytrace/ytrace.hpp>

namespace yetty::ypaint {

// Free functions: TTF metrics reading
#include "painter-ttf.inc"

// Free functions: AABB computation
#include "painter-aabb.inc"

//=============================================================================
// PainterImpl
//=============================================================================

class PainterImpl : public Painter {
public:
  PainterImpl(FontManager::Ptr fontManager, GpuAllocator::Ptr allocator,
              CardManager::Ptr cardMgr, uint32_t metaSlotIndex,
              bool scrollingMode = false)
      : _fontManager(std::move(fontManager)),
        _gpuAllocator(std::move(allocator)), _cardMgr(std::move(cardMgr)),
        _metaSlotIndex(metaSlotIndex), _scrollingMode(scrollingMode) {
    if (_fontManager) {
      _font = _fontManager->getDefaultMsMsdfFont();
      if (_font) {
        _atlas = _font->atlas();
      }
    }
  }

  ~PainterImpl() override = default;

  // Text API (addText, addRotatedText, measureTextWidth, fontAscent, fontDescent)
#include "painter-text.inc"

  // Font API (registerFont, addFont, addFontData, mapFontId, atlas access)
#include "painter-font.inc"

  // State management (primitiveCount, setBgColor, setSceneBounds, etc.)
  // Staging data access (gridStaging, glyphs, buildPrimStaging, hasContent)
  // Scrolling feature (scrollingMode, setCursorPosition, etc.)
  // Grid computation (sceneMinX, sceneMaxX, cellSizeX, gridWidth, etc.)
#include "painter-state.inc"

  // Buffer management (addYpaintBuffer, clear)
#include "painter-buffer.inc"

  // Grid helper methods (cellX, cellY, scrollLines, rebuildPackedGrid)
#include "painter-grid.inc"

  // GPU buffer lifecycle (primGpuSize, writePrimsGPU, declareBufferNeeds,
  // allocateBuffers, allocateTextures, writeTextures, writeBuffers,
  // needsBufferRealloc, needsTextureRealloc, setViewport, setView)
#include "painter-gpu.inc"

  // Text selection (buildGlyphSortedOrder, findNearestGlyphSorted,
  // setSelectionRange, getSelectedText)
#include "painter-selection.inc"

private:
  // Private helper methods (flushMetadata, computeDerivedSize, writeDerived)
  // Private member variables and internal structures
#include "painter-private.inc"
};

//=============================================================================
// Factory
//=============================================================================

Result<Painter::Ptr> Painter::createImpl(FontManager::Ptr fontManager,
                                         GpuAllocator::Ptr allocator,
                                         CardManager::Ptr cardMgr,
                                         uint32_t metaSlotIndex,
                                         bool scrollingMode) {
  return Ok(
      Ptr(new PainterImpl(std::move(fontManager), std::move(allocator),
                          std::move(cardMgr), metaSlotIndex, scrollingMode)));
}

Result<Painter::Ptr> Painter::createImpl(FontManager::Ptr fontManager,
                                         GpuAllocator::Ptr allocator,
                                         bool scrollingMode) {
  return Ok(Ptr(new PainterImpl(std::move(fontManager), std::move(allocator),
                                CardManager::Ptr{}, 0, scrollingMode)));
}

} // namespace yetty::ypaint
