#pragma once

#include "yrich-operation.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace yetty::yrich {

class Document;

//=============================================================================
// Command — user-facing action that generates operations
//
// Commands are the user-level abstraction for undo/redo:
// - User clicks "Bold" -> BoldCommand
// - Command generates StyleSetOp operations
// - Operations are applied and logged
// - Undo = apply inverse operations
//
// Commands group multiple operations into one undo unit.
// The underlying operations are what get synced to collaborators.
//=============================================================================
class Command {
public:
    virtual ~Command() = default;

    // Human-readable description (for undo menu)
    virtual std::string description() const = 0;

    // Execute: generate and apply operations
    // Returns the operations that were applied (for sync)
    virtual std::vector<OperationPtr> execute(Document& doc) = 0;

    // Undo: apply inverse operations
    // Default implementation uses stored ops
    virtual void undo(Document& doc);

    // Redo: re-apply operations
    virtual void redo(Document& doc);

    // Can this command merge with another of the same type?
    // Used for coalescing typing commands.
    virtual bool canMergeWith(const Command& other) const { return false; }
    virtual void mergeWith(Command& other) {}

protected:
    // Operations generated during execute (for undo/redo)
    std::vector<OperationPtr> _operations;
};

using CommandPtr = std::shared_ptr<Command>;

//=============================================================================
// CommandHistory — undo/redo stack backed by operations
//=============================================================================
class CommandHistory {
public:
    // Execute a command and add to history
    // Returns operations for sync
    std::vector<OperationPtr> execute(CommandPtr cmd, Document& doc);

    // Undo last command (returns inverse operations for sync)
    std::vector<OperationPtr> undo(Document& doc);

    // Redo last undone command (returns operations for sync)
    std::vector<OperationPtr> redo(Document& doc);

    bool canUndo() const { return !_undoStack.empty(); }
    bool canRedo() const { return !_redoStack.empty(); }

    std::string undoDescription() const {
        return _undoStack.empty() ? "" : _undoStack.back()->description();
    }

    std::string redoDescription() const {
        return _redoStack.empty() ? "" : _redoStack.back()->description();
    }

    void clear() {
        _undoStack.clear();
        _redoStack.clear();
    }

    void setMaxSize(size_t size) { _maxSize = size; }

    // Handle remote operation (may invalidate redo stack)
    void onRemoteOperation(const OperationPtr& op);

private:
    std::vector<CommandPtr> _undoStack;
    std::vector<CommandPtr> _redoStack;
    size_t _maxSize = 100;
};

//=============================================================================
// Common command implementations
//=============================================================================

// Simple operation command (wraps a single operation)
class SingleOpCommand : public Command {
public:
    SingleOpCommand(std::string desc, std::function<OperationPtr(Document&)> createOp)
        : _description(std::move(desc)), _createOp(std::move(createOp)) {}

    std::string description() const override { return _description; }

    std::vector<OperationPtr> execute(Document& doc) override;

private:
    std::string _description;
    std::function<OperationPtr(Document&)> _createOp;
};

// Composite command (batch multiple operations as one undo unit)
class CompositeCommand : public Command {
public:
    explicit CompositeCommand(std::string desc) : _description(std::move(desc)) {}

    void addOperation(std::function<OperationPtr(Document&)> createOp) {
        _createOps.push_back(std::move(createOp));
    }

    std::string description() const override { return _description; }

    std::vector<OperationPtr> execute(Document& doc) override;

private:
    std::string _description;
    std::vector<std::function<OperationPtr(Document&)>> _createOps;
};

// Text typing command (mergeable for coalescing keystrokes)
class TextTypeCommand : public Command {
public:
    TextTypeCommand(ElementId elementId, int position, std::string text)
        : _elementId(elementId), _position(position), _text(std::move(text)) {}

    std::string description() const override { return "Type"; }

    std::vector<OperationPtr> execute(Document& doc) override;

    bool canMergeWith(const Command& other) const override;
    void mergeWith(Command& other) override;

private:
    ElementId _elementId;
    int _position;
    std::string _text;
};

} // namespace yetty::yrich
