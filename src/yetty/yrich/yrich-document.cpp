#include "yrich-document.h"
#include "../ydraw/ydraw-buffer.h"

namespace yetty::yrich {

//=============================================================================
// Session management
//=============================================================================

void Document::initLocalSession(SessionId id, std::string_view userName) {
    auto session = std::make_shared<Session>(id);
    session->setUserName(userName);
    _sessions.setLocalSession(session);
}

//=============================================================================
// Operation handling
//=============================================================================

OperationPtr Document::createOperation(OperationType type, OperationData data) {
    auto session = localSession();
    if (!session) {
        // Create anonymous local session if none exists
        initLocalSession(LOCAL_SESSION, "Local");
        session = localSession();
    }

    auto ts = session->nextTimestamp();
    return std::make_shared<Operation>(type, std::move(data), ts, session->id());
}

void Document::applyOperation(OperationPtr op, bool local) {
    // Apply to document state
    doApplyOperation(*op);

    // Update session clock
    if (auto session = localSession()) {
        session->mergeClock(op->timestamp());
    }

    // Log non-presence operations
    if (!op->isPresence()) {
        _opLog.append(op);
    }

    // Sync to collaborators
    if (local && _syncCallback) {
        _syncCallback({op});
    }

    // Handle remote presence
    if (!local && op->isPresence()) {
        auto session = _sessions.findSession(op->author());
        if (session) {
            if (op->type() == OperationType::CursorMove) {
                auto& data = std::get<CursorMoveOp>(op->data());
                session->setCursor(data.elementId, data.position);
            } else if (op->type() == OperationType::SelectionSet) {
                auto& data = std::get<SelectionSetOp>(op->data());
                session->setSelection(data.selection);
            }
        }
    }

    markDirty();
}

void Document::applyOperations(const std::vector<OperationPtr>& ops, bool local) {
    for (const auto& op : ops) {
        doApplyOperation(*op);

        if (!op->isPresence()) {
            _opLog.append(op);
        }
    }

    // Sync all at once
    if (local && _syncCallback && !ops.empty()) {
        _syncCallback(ops);
    }

    markDirty();
}

void Document::receiveOperations(const std::vector<OperationPtr>& ops) {
    for (const auto& op : ops) {
        applyOperation(op, false);
    }

    // Invalidate redo stack on remote changes
    _history.onRemoteOperation(ops.empty() ? nullptr : ops.front());
}

//=============================================================================
// Default operation application
//=============================================================================

void Document::doApplyOperation(const Operation& op) {
    switch (op.type()) {
        case OperationType::Insert: {
            // Subclass should override to create proper element type
            break;
        }
        case OperationType::Delete: {
            auto& data = std::get<DeleteOp>(op.data());
            auto it = _elements.find(data.id);
            if (it != _elements.end()) {
                onElementRemoved(it->second);
                _elements.erase(it);
            }
            break;
        }
        case OperationType::Update: {
            // Subclass handles specific properties
            break;
        }
        default:
            // Other operations handled by subclass
            break;
    }
}

//=============================================================================
// Element management
//=============================================================================

void Document::addElement(ElementPtr element) {
    if (!element) return;
    ElementId id = element->id();

    // Generate operation
    InsertOp opData;
    opData.id = id;
    opData.data = element->serialize();

    auto op = createOperation(OperationType::Insert, opData);

    // Apply locally
    _elements[id] = element;
    onElementAdded(element);
    _opLog.append(op);

    // Sync
    if (_syncCallback) {
        _syncCallback({op});
    }

    markDirty();
}

void Document::removeElement(ElementId id) {
    auto it = _elements.find(id);
    if (it == _elements.end()) return;

    // Generate operation
    DeleteOp opData;
    opData.id = id;
    opData.data = it->second->serialize();

    auto op = createOperation(OperationType::Delete, opData);

    // Apply locally
    onElementRemoved(it->second);
    _elements.erase(it);
    _opLog.append(op);

    // Remove from local selection
    if (auto* sel = _localSelection.asElements()) {
        sel->remove(id);
    }

    // Sync
    if (_syncCallback) {
        _syncCallback({op});
    }

    markDirty();
}

ElementPtr Document::findElement(ElementId id) const {
    auto it = _elements.find(id);
    return it != _elements.end() ? it->second : nullptr;
}

ElementPtr Document::elementAt(float x, float y) const {
    for (auto it = _elements.rbegin(); it != _elements.rend(); ++it) {
        if (it->second->hitTest(x, y)) {
            return it->second;
        }
    }
    return nullptr;
}

//=============================================================================
// Selection
//=============================================================================

Selection& Document::selection() {
    return _localSelection;
}

const Selection& Document::selection() const {
    return _localSelection;
}

bool Document::isSelected(ElementId id) const {
    if (auto* sel = _localSelection.asElements()) {
        return sel->contains(id);
    }
    if (auto* sel = _localSelection.asText()) {
        return sel->elementId == id;
    }
    return false;
}

bool Document::isSelectedByAny(ElementId id) const {
    if (isSelected(id)) return true;

    bool found = false;
    _sessions.forEachSession([&](const SessionPtr& session) {
        if (session == localSession()) return;
        auto& sel = session->selection();
        if (auto* elemSel = std::get_if<ElementSelection>(&sel)) {
            if (elemSel->contains(id)) found = true;
        }
    });
    return found;
}

void Document::clearSelection() {
    _localSelection.clear();
    broadcastSelection();
    markDirty();
}

void Document::broadcastSelection() {
    if (!_syncCallback) return;

    SelectionSetOp opData;
    opData.selection = _localSelection.state();

    auto op = createOperation(OperationType::SelectionSet, opData);
    _syncCallback({op});
}

//=============================================================================
// Commands
//=============================================================================

void Document::executeCommand(CommandPtr cmd) {
    auto ops = _history.execute(std::move(cmd), *this);
    // Operations already synced via applyOperation
    markDirty();
}

void Document::undo() {
    auto ops = _history.undo(*this);
    // Inverse operations synced via applyOperation
    markDirty();
}

void Document::redo() {
    auto ops = _history.redo(*this);
    markDirty();
}

//=============================================================================
// Rendering
//=============================================================================

void Document::render() {
    if (!_buffer) return;

    _buffer->clear();
    _buffer->setSceneBounds(0, 0, contentWidth(), contentHeight());
    _buffer->setBgColor(_bgColor.toPacked());

    uint32_t layer = 0;
    for (const auto& [id, elem] : _elements) {
        elem->render(_buffer, layer++, isSelected(id));
    }

    // Render presence (cursors, selections of other users)
    renderPresence();

    _dirty = false;
}

void Document::renderPresence() {
    // Render other users' cursors and selections
    // Subclasses can override for specific rendering
    _sessions.forEachSession([&](const SessionPtr& session) {
        if (session == localSession()) return;

        // Render cursor indicator
        // This is a basic implementation - subclasses can customize
        ElementId cursorElem = session->cursorElement();
        if (cursorElem != INVALID_ELEMENT_ID) {
            if (auto elem = findElement(cursorElem)) {
                // Could render a colored cursor here
                // Using session->userColor()
            }
        }
    });
}

//=============================================================================
// Input handling
//=============================================================================

void Document::onMouseDown(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto elem = elementAt(x, y);
    if (!elem) {
        clearSelection();
        return;
    }

    if (mods.shift) {
        _localSelection.extendTo(elem->id());
    } else if (mods.ctrl) {
        if (auto* sel = _localSelection.asElements()) {
            sel->toggle(elem->id());
        } else {
            _localSelection.selectElement(elem->id());
        }
    } else {
        _localSelection.selectElement(elem->id());
    }

    broadcastSelection();
    markDirty();
}

void Document::onMouseUp(float x, float y, MouseButton button, InputModifiers mods) {
    // Default: nothing
}

void Document::onMouseMove(float x, float y, InputModifiers mods) {
    // Could broadcast cursor position for presence
}

void Document::onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto elem = elementAt(x, y);
    if (elem && mods.shift) {
        _localSelection.extendTo(elem->id());
        broadcastSelection();
        markDirty();
    }
}

void Document::onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto elem = elementAt(x, y);
    if (elem && elem->isEditable()) {
        elem->beginEdit();
        markDirty();
    }
}

void Document::onKeyDown(Key key, InputModifiers mods) {
    if (mods.ctrl) {
        switch (key) {
            case Key::Z:
                if (mods.shift) redo();
                else undo();
                return;
            case Key::Y:
                redo();
                return;
            case Key::C:
                copySelection();
                return;
            case Key::X:
                cut();
                return;
            default:
                break;
        }
    }

    if (key == Key::Delete || key == Key::Backspace) {
        deleteSelection();
    }
}

void Document::onTextInput(std::string_view text) {
    if (auto* sel = _localSelection.asText()) {
        if (auto elem = findElement(sel->elementId)) {
            elem->insertText(text);
            markDirty();
        }
    }
}

void Document::onScroll(float x, float y, float dx, float dy, InputModifiers mods) {
    // Default: nothing
}

void Document::cut() {
    copySelection();
    deleteSelection();
}

//=============================================================================
// Command implementations
//=============================================================================

void Command::undo(Document& doc) {
    // Apply inverse operations in reverse order
    for (auto it = _operations.rbegin(); it != _operations.rend(); ++it) {
        auto inverse = (*it)->inverse();
        doc.applyOperation(std::make_shared<Operation>(inverse), true);
    }
}

void Command::redo(Document& doc) {
    // Re-apply operations
    for (const auto& op : _operations) {
        doc.applyOperation(op, true);
    }
}

std::vector<OperationPtr> CommandHistory::execute(CommandPtr cmd, Document& doc) {
    auto ops = cmd->execute(doc);

    if (!_undoStack.empty() && _undoStack.back()->canMergeWith(*cmd)) {
        _undoStack.back()->mergeWith(*cmd);
    } else {
        _undoStack.push_back(std::move(cmd));
        if (_undoStack.size() > _maxSize) {
            _undoStack.erase(_undoStack.begin());
        }
    }

    _redoStack.clear();
    return ops;
}

std::vector<OperationPtr> CommandHistory::undo(Document& doc) {
    if (_undoStack.empty()) return {};

    auto cmd = std::move(_undoStack.back());
    _undoStack.pop_back();
    cmd->undo(doc);
    _redoStack.push_back(std::move(cmd));

    return {};  // Inverse ops already applied
}

std::vector<OperationPtr> CommandHistory::redo(Document& doc) {
    if (_redoStack.empty()) return {};

    auto cmd = std::move(_redoStack.back());
    _redoStack.pop_back();
    cmd->redo(doc);
    _undoStack.push_back(std::move(cmd));

    return {};
}

void CommandHistory::onRemoteOperation(const OperationPtr& op) {
    // Remote changes invalidate redo stack
    _redoStack.clear();
}

std::vector<OperationPtr> SingleOpCommand::execute(Document& doc) {
    auto op = _createOp(doc);
    if (op) {
        _operations.push_back(op);
        doc.applyOperation(op, true);
    }
    return _operations;
}

std::vector<OperationPtr> CompositeCommand::execute(Document& doc) {
    for (auto& createOp : _createOps) {
        auto op = createOp(doc);
        if (op) {
            _operations.push_back(op);
            doc.applyOperation(op, true);
        }
    }
    return _operations;
}

std::vector<OperationPtr> TextTypeCommand::execute(Document& doc) {
    TextInsertOp opData;
    opData.id = _elementId;
    opData.position = _position;
    opData.text = _text;

    auto op = doc.createOperation(OperationType::TextInsert, opData);
    _operations.push_back(op);
    doc.applyOperation(op, true);

    return _operations;
}

bool TextTypeCommand::canMergeWith(const Command& other) const {
    auto* typingCmd = dynamic_cast<const TextTypeCommand*>(&other);
    if (!typingCmd) return false;

    // Merge if same element and consecutive position
    return typingCmd->_elementId == _elementId &&
           typingCmd->_position == _position + static_cast<int>(_text.size());
}

void TextTypeCommand::mergeWith(Command& other) {
    auto& typingCmd = dynamic_cast<TextTypeCommand&>(other);
    _text += typingCmd._text;
    // Merge operations
    for (auto& op : typingCmd._operations) {
        _operations.push_back(op);
    }
}

//=============================================================================
// Operation
//=============================================================================

Operation Operation::inverse() const {
    OperationData inverseData;

    switch (_type) {
        case OperationType::Insert: {
            auto& data = std::get<InsertOp>(_data);
            inverseData = DeleteOp{data.id, data.data};
            return Operation(OperationType::Delete, inverseData, _timestamp, _author);
        }
        case OperationType::Delete: {
            auto& data = std::get<DeleteOp>(_data);
            inverseData = InsertOp{data.id, "", data.data};
            return Operation(OperationType::Insert, inverseData, _timestamp, _author);
        }
        case OperationType::TextInsert: {
            auto& data = std::get<TextInsertOp>(_data);
            inverseData = TextDeleteOp{data.id, data.position,
                                       static_cast<int>(data.text.size()), data.text};
            return Operation(OperationType::TextDelete, inverseData, _timestamp, _author);
        }
        case OperationType::TextDelete: {
            auto& data = std::get<TextDeleteOp>(_data);
            inverseData = TextInsertOp{data.id, data.position, data.deletedText};
            return Operation(OperationType::TextInsert, inverseData, _timestamp, _author);
        }
        case OperationType::CellSet: {
            auto& data = std::get<CellSetOp>(_data);
            inverseData = CellSetOp{data.addr, data.newValue, data.oldValue};
            return Operation(OperationType::CellSet, inverseData, _timestamp, _author);
        }
        case OperationType::StyleSet: {
            auto& data = std::get<StyleSetOp>(_data);
            inverseData = StyleSetOp{data.id, data.property, data.newValue, data.oldValue};
            return Operation(OperationType::StyleSet, inverseData, _timestamp, _author);
        }
        default:
            // Presence operations and others don't need inverse
            return *this;
    }
}

//=============================================================================
// OperationLog
//=============================================================================

void OperationLog::append(OperationPtr op) {
    _ops.push_back(op);
    if (_currentTs < op->timestamp()) {
        _currentTs = op->timestamp();
    }
}

std::vector<OperationPtr> OperationLog::since(Timestamp ts) const {
    std::vector<OperationPtr> result;
    for (const auto& op : _ops) {
        if (ts < op->timestamp()) {
            result.push_back(op);
        }
    }
    return result;
}

void OperationLog::compact() {
    // TODO: Merge consecutive operations from same author
    // This reduces memory usage for long-running sessions
}

//=============================================================================
// SessionManager
//=============================================================================

void SessionManager::addSession(SessionPtr session) {
    _remote[session->id()] = session;
}

void SessionManager::removeSession(SessionId id) {
    _remote.erase(id);
}

SessionPtr SessionManager::findSession(SessionId id) const {
    if (_local && _local->id() == id) return _local;
    auto it = _remote.find(id);
    return it != _remote.end() ? it->second : nullptr;
}

} // namespace yetty::yrich
