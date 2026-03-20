#pragma once

#include "yrich-operation.h"
#include "yrich-serialize.h"
#include <uv.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <queue>

namespace yetty::yrich {

class Document;

//=============================================================================
// SyncClient — connects to a collaboration server
//
// Handles:
// - Connection management (WebSocket-style over TCP)
// - Operation serialization/deserialization
// - Sending local operations
// - Receiving remote operations
// - Reconnection with operation replay
//=============================================================================
class SyncClient {
public:
    using Ptr = std::shared_ptr<SyncClient>;
    using ConnectCallback = std::function<void(bool success)>;
    using DisconnectCallback = std::function<void()>;
    using OperationsCallback = std::function<void(const std::vector<OperationPtr>&)>;

    SyncClient(uv_loop_t* loop);
    ~SyncClient();

    // Connection
    void connect(const std::string& host, int port, ConnectCallback cb);
    void disconnect();
    bool isConnected() const { return _connected; }

    // Callbacks
    void setOnDisconnect(DisconnectCallback cb) { _onDisconnect = std::move(cb); }
    void setOnOperations(OperationsCallback cb) { _onOperations = std::move(cb); }

    // Send operations
    void sendOperations(const std::vector<OperationPtr>& ops);

    // Session info
    void setSessionId(SessionId id) { _sessionId = id; }
    SessionId sessionId() const { return _sessionId; }

    void setDocumentId(const std::string& id) { _documentId = id; }
    const std::string& documentId() const { return _documentId; }

private:
    void onConnected();
    void onData(const uint8_t* data, size_t len);
    void onError(int status);
    void processMessageQueue();

    static void onConnect(uv_connect_t* req, int status);
    static void onRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    static void allocBuffer(uv_handle_t* handle, size_t suggested, uv_buf_t* buf);
    static void onWrite(uv_write_t* req, int status);

    uv_loop_t* _loop;
    uv_tcp_t _socket;
    uv_connect_t _connectReq;

    bool _connected = false;
    SessionId _sessionId = 0;
    std::string _documentId;

    ConnectCallback _connectCb;
    DisconnectCallback _onDisconnect;
    OperationsCallback _onOperations;

    // Receive buffer
    std::vector<uint8_t> _recvBuffer;

    // Send queue
    std::queue<std::vector<uint8_t>> _sendQueue;
    bool _sending = false;
};

//=============================================================================
// SyncServer — simple collaboration server
//
// Accepts connections and broadcasts operations to all clients.
// For production, you'd use a proper server with persistence.
//=============================================================================
class SyncServer {
public:
    using Ptr = std::shared_ptr<SyncServer>;
    using ClientConnectedCallback = std::function<void(SessionId)>;
    using ClientDisconnectedCallback = std::function<void(SessionId)>;

    SyncServer(uv_loop_t* loop);
    ~SyncServer();

    // Start listening
    bool listen(const std::string& host, int port);
    void stop();

    // Callbacks
    void setOnClientConnected(ClientConnectedCallback cb) { _onClientConnected = std::move(cb); }
    void setOnClientDisconnected(ClientDisconnectedCallback cb) { _onClientDisconnected = std::move(cb); }

    // Broadcast operations to all clients (except sender)
    void broadcast(const std::vector<OperationPtr>& ops, SessionId excludeSession = 0);

    // Get connected client count
    size_t clientCount() const { return _clients.size(); }

private:
    struct Client {
        uv_tcp_t socket;
        SessionId sessionId;
        std::vector<uint8_t> recvBuffer;
    };

    void onNewConnection();
    void onClientData(Client* client, const uint8_t* data, size_t len);
    void removeClient(Client* client);

    static void onConnection(uv_stream_t* server, int status);
    static void onClientRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    static void allocBuffer(uv_handle_t* handle, size_t suggested, uv_buf_t* buf);

    uv_loop_t* _loop;
    uv_tcp_t _server;
    bool _listening = false;

    std::vector<std::unique_ptr<Client>> _clients;
    SessionId _nextSessionId = 1;

    ClientConnectedCallback _onClientConnected;
    ClientDisconnectedCallback _onClientDisconnected;
};

//=============================================================================
// DocumentSync — attaches sync to a document
//
// Convenience class that connects a Document to a SyncClient.
//=============================================================================
class DocumentSync {
public:
    DocumentSync(Document* doc, SyncClient::Ptr client);
    ~DocumentSync();

    // Start syncing
    void start();
    void stop();

    bool isSyncing() const { return _syncing; }

private:
    void onLocalOperations(const std::vector<OperationPtr>& ops);
    void onRemoteOperations(const std::vector<OperationPtr>& ops);

    Document* _doc;
    SyncClient::Ptr _client;
    bool _syncing = false;
};

} // namespace yetty::yrich
