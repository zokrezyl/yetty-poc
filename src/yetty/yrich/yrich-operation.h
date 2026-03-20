#pragma once

#include "yrich-types.h"
#include "yrich-selection.h"
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace yetty::yrich {

//=============================================================================
// SessionId — unique identifier for a collaborating user/client
//=============================================================================
using SessionId = uint64_t;
constexpr SessionId LOCAL_SESSION = 0;

//=============================================================================
// Timestamp — logical clock for ordering operations
//
// Uses Lamport timestamps + session ID for total ordering.
// Two operations from different sessions with same counter are ordered by
// session ID (deterministic tie-breaker).
//=============================================================================
struct Timestamp {
    uint64_t counter = 0;
    SessionId session = LOCAL_SESSION;

    bool operator<(const Timestamp& other) const {
        if (counter != other.counter) return counter < other.counter;
        return session < other.session;
    }

    bool operator==(const Timestamp& other) const {
        return counter == other.counter && session == other.session;
    }

    // Advance clock (call after receiving remote operation)
    Timestamp next() const {
        return {counter + 1, session};
    }

    // Merge with remote timestamp (take max + 1)
    Timestamp merge(const Timestamp& remote) const {
        return {std::max(counter, remote.counter) + 1, session};
    }
};

//=============================================================================
// OperationType — what kind of change
//=============================================================================
enum class OperationType {
    // Element operations
    Insert,         // Add new element
    Delete,         // Remove element
    Update,         // Modify element properties

    // Text operations (for text elements)
    TextInsert,     // Insert text at position
    TextDelete,     // Delete text range
    TextFormat,     // Apply formatting to text range

    // Cell operations (for spreadsheet)
    CellSet,        // Set cell value
    CellClear,      // Clear cell
    CellFormat,     // Format cell (color, borders, etc.)

    // Style operations
    StyleSet,       // Set style property

    // Selection/cursor (presence, not persisted)
    CursorMove,     // Move cursor position
    SelectionSet,   // Set selection range
};

//=============================================================================
// Operation — atomic change to document state
//
// Operations are the unit of collaboration:
// - Generated locally when user makes changes
// - Sent to other clients
// - Received and applied from other clients
// - Stored in operation log for history/undo
//
// Operations must be:
// - Commutative where possible (CRDT-friendly)
// - Transformable (for OT fallback)
// - Invertible (for undo)
//=============================================================================

// Operation payloads for different types
struct InsertOp {
    ElementId id;
    std::string elementType;  // "cell", "shape", "textbox", etc.
    std::string data;         // serialized element data
};

struct DeleteOp {
    ElementId id;
    std::string data;  // serialized element (for undo)
};

struct UpdateOp {
    ElementId id;
    std::string property;  // property name
    std::string oldValue;  // for undo
    std::string newValue;
};

struct TextInsertOp {
    ElementId id;
    int position;
    std::string text;
};

struct TextDeleteOp {
    ElementId id;
    int position;
    int length;
    std::string deletedText;  // for undo
};

struct TextFormatOp {
    ElementId id;
    int startPos;
    int endPos;
    uint32_t addFormat;     // TextFormat flags to add
    uint32_t removeFormat;  // TextFormat flags to remove
    uint32_t oldFormat;     // Previous format (for undo)
};

struct CellSetOp {
    CellAddress addr;
    std::string oldValue;
    std::string newValue;
};

struct CellClearOp {
    CellRange range;
    std::vector<std::pair<CellAddress, std::string>> oldValues;
};

struct CellFormatOp {
    CellRange range;
    std::string property;   // "bgColor", "textColor", "bold", "fontSize", etc.
    std::string newValue;
    std::vector<std::pair<CellAddress, std::string>> oldValues;
};

struct StyleSetOp {
    ElementId id;
    std::string property;
    std::string oldValue;
    std::string newValue;
};

struct CursorMoveOp {
    ElementId elementId;  // element containing cursor (or INVALID for document-level)
    int position;
    // For spreadsheet: position encodes row/col
    // For text: position is character index
};

struct SelectionSetOp {
    // Variant: element selection, cell range, or text selection
    Selection::State selection;
};

using OperationData = std::variant<
    InsertOp, DeleteOp, UpdateOp,
    TextInsertOp, TextDeleteOp, TextFormatOp,
    CellSetOp, CellClearOp, CellFormatOp,
    StyleSetOp,
    CursorMoveOp, SelectionSetOp
>;

//=============================================================================
// Operation
//=============================================================================
class Operation {
public:
    Operation(OperationType type, OperationData data, Timestamp ts, SessionId author)
        : _type(type), _data(std::move(data)), _timestamp(ts), _author(author) {}

    OperationType type() const { return _type; }
    const OperationData& data() const { return _data; }
    Timestamp timestamp() const { return _timestamp; }
    SessionId author() const { return _author; }

    // Is this a presence operation (cursor/selection)?
    // Presence ops are not persisted, only broadcast
    bool isPresence() const {
        return _type == OperationType::CursorMove ||
               _type == OperationType::SelectionSet;
    }

    // Create inverse operation (for undo)
    Operation inverse() const;

    // Serialize for network/storage
    std::string serialize() const;
    static Operation deserialize(std::string_view data);

private:
    OperationType _type;
    OperationData _data;
    Timestamp _timestamp;
    SessionId _author;
};

using OperationPtr = std::shared_ptr<Operation>;

//=============================================================================
// OperationLog — ordered history of operations
//
// The log is the source of truth. Document state is derived by replaying
// operations from the log.
//
// For efficiency, we also maintain computed state (the Document) and only
// replay when needed (e.g., undo, conflict resolution).
//=============================================================================
class OperationLog {
public:
    // Append operation to log
    void append(OperationPtr op);

    // Get operations since timestamp (for sync)
    std::vector<OperationPtr> since(Timestamp ts) const;

    // Get all operations
    const std::vector<OperationPtr>& operations() const { return _ops; }

    // Get operation count
    size_t size() const { return _ops.size(); }

    // Clear log (careful - loses history)
    void clear() { _ops.clear(); }

    // Compact log (merge consecutive ops from same author)
    void compact();

    // Current timestamp (max of all operations)
    Timestamp currentTimestamp() const { return _currentTs; }

private:
    std::vector<OperationPtr> _ops;
    Timestamp _currentTs;
};

//=============================================================================
// Session — a collaborating user's state
//
// Each connected client has a Session. Sessions track:
// - Identity (session ID, user name/color)
// - Cursor position
// - Selection
// - Connection state
//=============================================================================
class Session {
public:
    explicit Session(SessionId id) : _id(id) {}

    SessionId id() const { return _id; }

    // User identity
    void setUserName(std::string_view name) { _userName = name; }
    std::string userName() const { return _userName; }

    void setUserColor(Color color) { _userColor = color; }
    Color userColor() const { return _userColor; }

    // Cursor/selection (updated by presence operations)
    void setCursor(ElementId element, int position) {
        _cursorElement = element;
        _cursorPosition = position;
    }
    ElementId cursorElement() const { return _cursorElement; }
    int cursorPosition() const { return _cursorPosition; }

    void setSelection(const Selection::State& sel) { _selection = sel; }
    const Selection::State& selection() const { return _selection; }

    // Timestamp tracking
    Timestamp lastSeen() const { return _lastSeen; }
    void updateLastSeen(Timestamp ts) {
        if (_lastSeen < ts) _lastSeen = ts;
    }

    // Clock management
    Timestamp nextTimestamp() {
        _clock = _clock.next();
        return _clock;
    }

    void mergeClock(Timestamp remote) {
        _clock = _clock.merge(remote);
    }

private:
    SessionId _id;
    std::string _userName = "Anonymous";
    Color _userColor = Color{100, 149, 237, 255};  // cornflower blue

    ElementId _cursorElement = INVALID_ELEMENT_ID;
    int _cursorPosition = 0;
    Selection::State _selection;

    Timestamp _lastSeen;
    Timestamp _clock{0, _id};
};

using SessionPtr = std::shared_ptr<Session>;

//=============================================================================
// SessionManager — tracks all active sessions
//=============================================================================
class SessionManager {
public:
    // Local session (this client)
    SessionPtr localSession() const { return _local; }
    void setLocalSession(SessionPtr session) { _local = session; }

    // Remote sessions
    void addSession(SessionPtr session);
    void removeSession(SessionId id);
    SessionPtr findSession(SessionId id) const;

    // Iterate all sessions (including local)
    template<typename F>
    void forEachSession(F&& fn) const {
        if (_local) fn(_local);
        for (const auto& [id, session] : _remote) {
            fn(session);
        }
    }

    // Get all remote sessions
    const std::map<SessionId, SessionPtr>& remoteSessions() const { return _remote; }

private:
    SessionPtr _local;
    std::map<SessionId, SessionPtr> _remote;
};

} // namespace yetty::yrich
