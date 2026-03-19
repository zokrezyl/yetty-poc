#pragma once

//=============================================================================
// YRich — Document-centric abstraction for WYSIWYG rich content
//
// YRich provides the building blocks for spreadsheets, slides, and documents:
// - Element: base class for all selectable/editable content
// - Selection: unified selection model (cells, shapes, text ranges)
// - Command: undo/redo support via command pattern
// - Document: ties it all together with rendering to YDrawBuffer
//
// Architecture:
//
//   ┌─────────────────────────────────────────────────────┐
//   │         YSpreadsheet │ YSlides │ YDoc              │
//   ├─────────────────────────────────────────────────────┤
//   │                      YRich                          │
//   │  ┌──────────┐ ┌──────────┐ ┌──────────┐            │
//   │  │ Element  │ │ Selection│ │ Command  │            │
//   │  └──────────┘ └──────────┘ └──────────┘            │
//   │                    Document                         │
//   ├─────────────────────────────────────────────────────┤
//   │                   YDrawBuffer                       │
//   └─────────────────────────────────────────────────────┘
//
// Usage:
//   1. Create a Document subclass (YSpreadsheet, YSlides, YDoc)
//   2. Add elements to the document
//   3. Set the YDrawBuffer to render to
//   4. Call render() to draw the document
//   5. Forward input events from the card layer
//
//=============================================================================

#include "yrich-types.h"
#include "yrich-element.h"
#include "yrich-selection.h"
#include "yrich-operation.h"
#include "yrich-command.h"
#include "yrich-document.h"
