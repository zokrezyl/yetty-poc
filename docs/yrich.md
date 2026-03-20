# YRich - Document-Centric WYSIWYG Framework

YRich is an abstraction layer for building Google Docs/Sheets/Slides-style interactive editors inside the yetty terminal. It sits on top of YDraw and provides document models, selection, undo/redo, and real-time collaborative editing support.

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    Applications                                     │
│         YSpreadsheet  │  YDoc  │  YSlides (future)                 │
├─────────────────────────────────────────────────────────────────────┤
│                        YRich Core                                   │
│  ┌──────────┐ ┌───────────┐ ┌─────────┐ ┌─────────┐ ┌───────────┐  │
│  │ Element  │ │ Selection │ │ Command │ │Operation│ │  Session  │  │
│  │ (base)   │ │ (variant) │ │ (undo)  │ │ (sync)  │ │ (collab)  │  │
│  └──────────┘ └───────────┘ └─────────┘ └─────────┘ └───────────┘  │
│                        Document                                     │
├─────────────────────────────────────────────────────────────────────┤
│                       YDrawBuffer                                   │
├─────────────────────────────────────────────────────────────────────┤
│                    GPU Shader (SDF)                                 │
└─────────────────────────────────────────────────────────────────────┘
```

## Core Concepts

### Operations (Collaboration)

Every document mutation is an **Operation** - the unit of synchronization:

```cpp
// Operations are generated locally and synced to collaborators
Operation {
    OperationType type;      // Insert, Delete, TextInsert, CellSet, etc.
    OperationData data;      // Variant with operation-specific payload
    Timestamp timestamp;     // Lamport clock for ordering
    SessionId author;        // Who made this change
}
```

**Key properties:**
- Operations are invertible (for undo)
- Operations use Lamport timestamps for total ordering
- Last-writer-wins for simple properties (CRDT-friendly)
- Presence operations (cursor/selection) are broadcast but not persisted

### Sessions (Multi-user)

Each collaborating user has a Session:

```cpp
Session {
    SessionId id;
    std::string userName;
    Color userColor;           // For cursor/selection visualization
    ElementId cursorElement;   // Where cursor is
    int cursorPosition;
    Selection::State selection;
    Timestamp clock;           // Local Lamport clock
}
```

### Commands (Undo/Redo)

Commands are user-facing actions that generate Operations:

```cpp
// User clicks "Bold" -> BoldCommand -> StyleSetOp operations
// Undo = apply inverse operations in reverse order
Command::execute(Document& doc) -> vector<OperationPtr>
Command::undo(Document& doc)
Command::redo(Document& doc)
```

Commands can merge (e.g., consecutive typing keystrokes become one undo unit).

### Selection

Unified selection model supporting different document types:

```cpp
Selection::State = variant<
    NoSelection,
    ElementSelection,    // One or more shapes/elements
    CellSelection,       // Spreadsheet cell range (A1:C10)
    TextSelection        // Text cursor/range within an element
>
```

## Implemented Components

### Core (`src/yetty/yrich/`)

| File | Description |
|------|-------------|
| `yrich-types.h` | Core types: Rect, Color, CellAddress, Key, MouseButton, TextFormat, TextStyle |
| `yrich-element.h` | Element base class (selectable, editable content) |
| `yrich-selection.h` | Selection variants and state management |
| `yrich-operation.h` | Operation, Timestamp, Session, SessionManager, OperationLog |
| `yrich-command.h` | Command, CommandHistory, LambdaCommand, CompositeCommand |
| `yrich-serialize.h` | Msgpack serialization for all operation types |
| `yrich-sync.h/cpp` | SyncClient, SyncServer, DocumentSync for real-time collaboration |
| `yrich-document.h/cpp` | Document base class with operation handling, rendering, input |
| `yspreadsheet.h/cpp` | Spreadsheet document (grid, cells, formulas placeholder) |
| `ydoc.h/cpp` | Rich text document (paragraphs, TextRun formatting, word wrap) |

### Tools (`tools/`)

| Tool | Description |
|------|-------------|
| `yspreadsheet` | Interactive spreadsheet editor |
| `ydoc` | Interactive document editor |

## Event Flow

```
Terminal (yetty)                     Tool (ydoc/yspreadsheet)
      │                                        │
      │── OSC 777780 (pixel size) ────────────→│ store display dimensions
      │── OSC 777777 (mouse click) ───────────→│ transform → doc.onMouseDown()
      │── OSC 777778 (mouse move) ────────────→│ transform → doc.onMouseDrag()
      │── keyboard chars ─────────────────────→│ doc.onTextInput() / onKeyDown()
      │                                        │
      │                                        │ (dirty flag set)
      │                                        │
      │←── OSC 666666 update ──────────────────│ render timer: serialize + send
```

## Collaboration Flow

```
Client A                    Network                    Client B
    │                                                      │
    │ User types "Hello"                                   │
    │     ↓                                                │
    │ TextInsertOp(pos=0, text="Hello", ts=5, author=A)   │
    │     ↓                                                │
    │ Apply locally                                        │
    │ Log to OperationLog                                  │
    │ SyncCallback({op})  ─────────────────────────────→  │
    │                                                      │ Receive op
    │                                                      │ Merge clock
    │                                                      │ Apply to doc
    │                                                      │ Re-render
    │                                                      │
    │                     ←─────────────────────────────   │ User types "World"
    │ Receive op                                           │ TextInsertOp(...)
    │ Merge clock                                          │
    │ Apply to doc                                         │
    │ Re-render                                            │
```

## Usage

### Running the editors

```bash
# Document editor (inside yetty terminal)
./build-desktop-ytrace-release/tools/ydoc/ydoc -h 25

# Spreadsheet editor
./build-desktop-ytrace-release/tools/yspreadsheet/yspreadsheet -h 20
```

### Keyboard shortcuts

**Document (ydoc):**
- Arrow keys: navigate
- Shift+arrows: select
- Ctrl+arrows: word navigation
- Ctrl+B: toggle bold
- Ctrl+I: toggle italic
- Ctrl+U: toggle underline
- Ctrl+C/V/X: copy/paste/cut
- Ctrl+Z/Y: undo/redo
- Enter: new paragraph
- Backspace/Delete: delete
- Home/End: line start/end
- q: quit

**Spreadsheet (yspreadsheet):**
- Arrow keys: navigate cells
- Shift+arrows: extend selection
- Ctrl+arrows: jump to edge
- Enter/Tab: confirm and move
- F2: enter edit mode
- Escape: exit edit mode
- Delete: clear cells
- q: quit

### Programmatic usage

```cpp
#include "yrich/ydoc.h"

// Create document
auto doc = YDoc::create().value();
doc->setBuffer(buffer.get());

// Add content
doc->addParagraph("Hello World");
doc->addParagraph("Second paragraph");

// Set up collaboration sync
doc->setSyncCallback([&](const std::vector<OperationPtr>& ops) {
    // Send ops to other clients via network
    sendToCollaborators(ops);
});

// Receive from collaborators
doc->receiveOperations(incomingOps);

// Render
doc->render();
auto bytes = buffer->serialize();
```

## Implementation Roadmap

### Phase 1: Core Framework (DONE)
- [x] Element base class
- [x] Selection model (cells, elements, text)
- [x] Operation/Timestamp/Session for collaboration
- [x] Command/CommandHistory for undo/redo
- [x] Document base class

### Phase 2: Basic Editors (DONE)
- [x] YSpreadsheet: grid, cells, navigation, editing
- [x] YDoc: paragraphs, word wrap, text editing
- [x] tools/yspreadsheet: interactive spreadsheet
- [x] tools/ydoc: interactive document editor

### Phase 3: Enhanced Editing (DONE)
- [x] Text formatting (bold, italic, underline, strikethrough)
- [x] TextRun model for mixed formatting within paragraphs
- [x] Font selection and sizing
- [x] Keyboard shortcuts (Ctrl+B/I/U)
- [x] Local clipboard (Ctrl+C/V/X)
- [ ] Cell formatting (colors, borders, alignment)
- [ ] Formula support for spreadsheet
- [ ] System clipboard integration

### Phase 4: Collaboration (DONE)
- [x] Operation serialization (msgpack)
- [x] SyncClient: TCP client for operation sync
- [x] SyncServer: TCP server for broadcasting
- [x] DocumentSync: connects Document to SyncClient
- [x] TextFormatOp/CellFormatOp serialization
- [ ] Conflict resolution testing
- [ ] Presence indicators (colored cursors)
- [ ] User list UI

### Phase 5: YSlides (TODO)
- [ ] Slide document model
- [ ] Shape elements (rectangles, circles, text boxes)
- [ ] Slide navigation
- [ ] Presentation mode

### Phase 6: Advanced Features (TODO)
- [ ] Images in documents
- [ ] Tables in documents
- [ ] Charts in spreadsheets
- [ ] Comments/annotations
- [ ] Version history

## File Structure

```
src/yetty/yrich/
├── yrich.h              # Main include
├── yrich-types.h        # Core types (TextFormat, TextStyle, etc.)
├── yrich-element.h      # Element base class
├── yrich-selection.h    # Selection model
├── yrich-operation.h    # Collaboration primitives (Operation, Timestamp, Session)
├── yrich-operation.cpp
├── yrich-command.h      # Undo/redo (Command, CommandHistory)
├── yrich-serialize.h    # Msgpack serialization for operations
├── yrich-sync.h         # Network sync (SyncClient, SyncServer, DocumentSync)
├── yrich-sync.cpp
├── yrich-document.h     # Document base class
├── yrich-document.cpp
├── yspreadsheet.h       # Spreadsheet document
├── yspreadsheet.cpp
├── ydoc.h               # Rich text document (Paragraph, TextRun)
├── ydoc.cpp
└── CMakeLists.txt

tools/
├── yspreadsheet/
│   ├── main.cpp
│   └── CMakeLists.txt
└── ydoc/
    ├── main.cpp
    └── CMakeLists.txt
```

## Design Decisions

### Why Operations instead of direct mutations?

1. **Collaboration**: Operations can be sent to other clients
2. **Undo/redo**: Inverse operations provide natural undo
3. **History**: Operation log is the source of truth
4. **Conflict resolution**: Timestamps enable ordering

### Why Lamport timestamps?

- Simple, no central server needed
- Total ordering with session ID as tie-breaker
- Works well with CRDT approach

### Why variant-based Selection?

Different document types need different selection models:
- Spreadsheet: cell ranges
- Document: text ranges
- Slides: element selection

A single unified type allows the framework to handle selection generically while each document type uses what it needs.
