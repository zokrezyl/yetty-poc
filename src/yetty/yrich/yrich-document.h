#pragma once

#include "yrich-element.h"
#include "yrich-selection.h"
#include "yrich-command.h"
#include "yrich-operation.h"
#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <functional>
#include <map>
#include <memory>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::yrich {

//=============================================================================
// SyncCallback — called when operations need to be sent to collaborators
//=============================================================================
using SyncCallback = std::function<void(const std::vector<OperationPtr>&)>;

//=============================================================================
// Document — base class for collaborative YRich documents
//
// A Document manages:
// - Element storage and lookup
// - Operation log (source of truth)
// - Session management (local + remote collaborators)
// - Command history (undo/redo)
// - Rendering to YDrawBuffer
// - Input event handling
//
// Collaboration model:
// 1. Local changes -> Command -> Operations -> apply + sync
// 2. Remote operations -> apply + update cursors
// 3. Conflicts resolved via CRDT semantics (last-writer-wins for simple props)
//
// Subclasses (YSpreadsheet, YSlides, YDoc) implement:
// - Layout logic
// - Element creation
// - Domain-specific operations
//=============================================================================
class Document : public base::Object {
public:
    using Ptr = std::shared_ptr<Document>;
    using DirtyCallback = std::function<void()>;

    ~Document() override = default;

    //=========================================================================
    // Session management
    //=========================================================================

    SessionManager& sessions() { return _sessions; }
    const SessionManager& sessions() const { return _sessions; }

    // Initialize local session (call once at startup)
    void initLocalSession(SessionId id, std::string_view userName = "User");

    // Get local session
    SessionPtr localSession() const { return _sessions.localSession(); }

    //=========================================================================
    // Operation handling
    //=========================================================================

    OperationLog& operationLog() { return _opLog; }
    const OperationLog& operationLog() const { return _opLog; }

    // Apply operation (local or remote)
    // If local=true, also logs and syncs
    void applyOperation(OperationPtr op, bool local = true);

    // Apply multiple operations atomically
    void applyOperations(const std::vector<OperationPtr>& ops, bool local = true);

    // Receive operations from remote (calls applyOperation with local=false)
    void receiveOperations(const std::vector<OperationPtr>& ops);

    // Set callback for outgoing operations (for sync layer)
    void setSyncCallback(SyncCallback cb) { _syncCallback = std::move(cb); }

    //=========================================================================
    // Element management
    //=========================================================================

    // Add element (generates InsertOp)
    void addElement(ElementPtr element);

    // Remove element (generates DeleteOp)
    void removeElement(ElementId id);

    // Find element by ID
    ElementPtr findElement(ElementId id) const;

    // Find element at position (for hit testing)
    ElementPtr elementAt(float x, float y) const;

    // Iterate all elements
    template<typename F>
    void forEachElement(F&& fn) const {
        for (const auto& [id, elem] : _elements) {
            fn(elem);
        }
    }

    // Generate unique element ID
    ElementId nextElementId() { return ++_nextId; }

    //=========================================================================
    // Selection (local session's selection)
    //=========================================================================

    Selection& selection();
    const Selection& selection() const;

    // Check if element is selected by local session
    bool isSelected(ElementId id) const;

    // Check if element is selected by any session
    bool isSelectedByAny(ElementId id) const;

    // Clear local selection
    void clearSelection();

    // Broadcast selection change to collaborators
    void broadcastSelection();

    //=========================================================================
    // Commands (undo/redo)
    //=========================================================================

    CommandHistory& history() { return _history; }

    // Execute command (generates operations, syncs)
    void executeCommand(CommandPtr cmd);

    // Undo/redo (generates inverse operations, syncs)
    void undo();
    void redo();

    //=========================================================================
    // Rendering
    //=========================================================================

    void setBuffer(YDrawBuffer* buffer) { _buffer = buffer; }
    YDrawBuffer* buffer() const { return _buffer; }

    // Render document including collaborator cursors
    virtual void render();

    // Render collaborator presence (cursors, selections)
    virtual void renderPresence();

    // Document dimensions
    virtual float contentWidth() const = 0;
    virtual float contentHeight() const = 0;

    // Background color
    void setBgColor(Color color) { _bgColor = color; markDirty(); }
    Color bgColor() const { return _bgColor; }

    //=========================================================================
    // Dirty state
    //=========================================================================

    bool isDirty() const { return _dirty; }
    void markDirty() {
        _dirty = true;
        if (_dirtyCallback) _dirtyCallback();
    }
    void clearDirty() { _dirty = false; }

    void setDirtyCallback(DirtyCallback cb) { _dirtyCallback = std::move(cb); }

    //=========================================================================
    // Input handling
    //=========================================================================

    virtual void onMouseDown(float x, float y, MouseButton button, InputModifiers mods);
    virtual void onMouseUp(float x, float y, MouseButton button, InputModifiers mods);
    virtual void onMouseMove(float x, float y, InputModifiers mods);
    virtual void onMouseDrag(float x, float y, MouseButton button, InputModifiers mods);
    virtual void onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods);
    virtual void onKeyDown(Key key, InputModifiers mods);
    virtual void onTextInput(std::string_view text);
    virtual void onScroll(float x, float y, float dx, float dy, InputModifiers mods);

    //=========================================================================
    // Clipboard
    //=========================================================================

    virtual std::string copySelection() const { return {}; }
    virtual void paste(std::string_view data) {}
    virtual void cut();
    virtual void deleteSelection() {}

    // Create operation with proper timestamp (public for commands)
    OperationPtr createOperation(OperationType type, OperationData data);

protected:
    Document() = default;

    // Apply operation to document state (subclass implements specifics)
    virtual void doApplyOperation(const Operation& op);

    // Called after element is added
    virtual void onElementAdded(const ElementPtr& element) {}

    // Called before element is removed
    virtual void onElementRemoved(const ElementPtr& element) {}

    // Elements
    std::map<ElementId, ElementPtr> _elements;
    ElementId _nextId = 0;

    // Local selection (convenience - also stored in local session)
    Selection _localSelection;

    // Sessions
    SessionManager _sessions;

    // Operation log
    OperationLog _opLog;

    // Command history
    CommandHistory _history;

    // Sync
    SyncCallback _syncCallback;

    // Rendering
    YDrawBuffer* _buffer = nullptr;
    Color _bgColor = Color::white();
    bool _dirty = true;

    // Callback
    DirtyCallback _dirtyCallback;
};

} // namespace yetty::yrich
