#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include <yetty/base/factory.h>
#include <yetty/base/object.h>
#include <yetty/result.hpp>

namespace yetty::ypaint {

//=============================================================================
// Canvas - Spatial grid representation for ypaint primitives
//
// The Canvas represents a 2D spatial grid where:
//   - Scene is divided into cells of configurable size
//   - Each cell contains references to primitives that overlap it
//   - Supports two modes:
//       * Non-scrolling: primitives at absolute scene coordinates
//       * Scrolling: primitives relative to cursor, lines scroll off top
//
// The Canvas receives primitives with pre-computed AABBs (from Painter)
// and handles grid placement based on mode.
//
// Grid layout:
//   - Lines stored in deque (front = top, back = bottom)
//   - Each line contains primitives whose "base" (bottom edge) is on that line
//   - Grid cells store references to primitives
//
// Coordinate system:
//   - Scene coordinates: world space (sceneMin -> sceneMax)
//   - Grid coordinates: cell indices (0,0) = top-left
//
//=============================================================================
class Canvas : public base::Object, public base::ObjectFactory<Canvas> {
public:
  using Ptr = std::shared_ptr<Canvas>;

  // High bit marker for glyph indices in packed grid
  static constexpr uint32_t GLYPH_BIT = 0x80000000u;

  // Default grid parameters
  static constexpr uint32_t DEFAULT_MAX_PRIMS_PER_CELL = 16;

  //===========================================================================
  // Factory
  //===========================================================================

  /// Create a canvas
  /// @param scrollingMode If true, primitives are cursor-relative and scroll
  static Result<Ptr> create(bool scrollingMode = false);

  ~Canvas() override = default;
  const char *typeName() const override { return "Canvas"; }

  //===========================================================================
  // Mode
  //===========================================================================

  /// Whether canvas is in scrolling mode
  virtual bool scrollingMode() const = 0;

  //===========================================================================
  // Configuration
  //===========================================================================

  /// Set scene bounds (world coordinates)
  virtual void setSceneBounds(float minX, float minY, float maxX,
                              float maxY) = 0;

  /// Set grid cell size (scene units)
  virtual void setCellSize(float sizeX, float sizeY) = 0;

  /// Set maximum primitives per cell (for GPU culling efficiency)
  virtual void setMaxPrimsPerCell(uint32_t max) = 0;

  //===========================================================================
  // Scene bounds accessors
  //===========================================================================

  virtual float sceneMinX() const = 0;
  virtual float sceneMinY() const = 0;
  virtual float sceneMaxX() const = 0;
  virtual float sceneMaxY() const = 0;

  //===========================================================================
  // Grid dimension accessors
  //===========================================================================

  virtual float cellSizeX() const = 0;
  virtual float cellSizeY() const = 0;
  virtual uint32_t gridWidth() const = 0;
  virtual uint32_t gridHeight() const = 0;
  virtual uint32_t maxPrimsPerCell() const = 0;

  /// Get number of lines (rows) in the grid
  virtual size_t lineCount() const = 0;

  /// Get height in lines (based on scene height / cell size)
  virtual uint32_t heightInLines() const = 0;

  //===========================================================================
  // Cursor (scrolling mode only)
  //===========================================================================

  /// Set cursor position in grid cells (column, row)
  virtual void setCursorPosition(uint16_t col, uint16_t row) = 0;
  virtual uint16_t cursorCol() const = 0;
  virtual uint16_t cursorRow() const = 0;

  //===========================================================================
  // Primitive management
  //===========================================================================

  /// Add a primitive with pre-computed AABB
  /// In scrolling mode: positioned relative to cursor
  /// In non-scrolling mode: positioned at absolute scene coordinates
  /// @param primData The primitive data (GPU format)
  /// @param aabbMinX/Y/MaxX/Y The pre-computed axis-aligned bounding box
  virtual void addPrimitive(std::vector<float> primData, float aabbMinX,
                            float aabbMinY, float aabbMaxX, float aabbMaxY) = 0;

  //===========================================================================
  // Scrolling support
  //===========================================================================

  /// Remove N lines from the top (front) of the grid
  /// Only valid in scrolling mode
  virtual void scrollLines(uint16_t numLines) = 0;

  //===========================================================================
  // Packed GPU format
  //===========================================================================

  /// Mark grid staging as dirty (needs rebuild)
  virtual void markDirty() = 0;

  /// Check if grid needs rebuild
  virtual bool isDirty() const = 0;

  /// Rebuild packed grid format for GPU upload (without glyphs)
  virtual void rebuildPackedGrid() = 0;

  /// Rebuild packed grid with glyphs
  /// @param glyphCount Number of glyphs
  /// @param glyphBoundsFunc Callback: (idx, minX, minY, maxX, maxY)
  virtual void rebuildPackedGrid(
      uint32_t glyphCount,
      std::function<void(uint32_t, float &, float &, float &, float &)>
          glyphBoundsFunc) = 0;

  /// Get packed grid data for GPU upload
  virtual const std::vector<uint32_t> &gridStaging() const = 0;

  /// Clear packed grid staging
  virtual void clearStaging() = 0;

  //===========================================================================
  // Primitive staging for GPU
  //===========================================================================

  /// Build primitive staging data for GPU upload
  /// Returns (offset_table, primitive_data) packed as u32 words
  virtual void buildPrimStaging(std::vector<uint32_t> &out) const = 0;

  /// Get total GPU size for primitives (in bytes)
  virtual uint32_t primGpuSize() const = 0;

  //===========================================================================
  // State management
  //===========================================================================

  /// Clear all lines and primitives
  virtual void clear() = 0;

  /// Check if canvas has any content
  virtual bool empty() const = 0;

  /// Get total primitive count across all lines
  virtual uint32_t primitiveCount() const = 0;

protected:
  Canvas() = default;
};

} // namespace yetty::ypaint
