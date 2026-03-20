#include "yrich-sync.h"
#include "yrich-document.h"
#include <cstring>

namespace yetty::yrich {

//=============================================================================
// Message format: [4-byte length][payload]
//=============================================================================

static void writeLength(std::vector<uint8_t>& buf, uint32_t len) {
    buf.push_back((len >> 24) & 0xFF);
    buf.push_back((len >> 16) & 0xFF);
    buf.push_back((len >> 8) & 0xFF);
    buf.push_back(len & 0xFF);
}

static uint32_t readLength(const uint8_t* data) {
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8) |
           static_cast<uint32_t>(data[3]);
}

//=============================================================================
// SyncClient
//=============================================================================

SyncClient::SyncClient(uv_loop_t* loop) : _loop(loop) {
    uv_tcp_init(_loop, &_socket);
    _socket.data = this;
}

SyncClient::~SyncClient() {
    disconnect();
}

void SyncClient::connect(const std::string& host, int port, ConnectCallback cb) {
    _connectCb = std::move(cb);

    struct sockaddr_in addr;
    uv_ip4_addr(host.c_str(), port, &addr);

    _connectReq.data = this;
    int r = uv_tcp_connect(&_connectReq, &_socket,
                           reinterpret_cast<const sockaddr*>(&addr),
                           onConnect);
    if (r != 0 && _connectCb) {
        _connectCb(false);
    }
}

void SyncClient::disconnect() {
    if (_connected) {
        _connected = false;
        uv_close(reinterpret_cast<uv_handle_t*>(&_socket), nullptr);
        if (_onDisconnect) _onDisconnect();
    }
}

void SyncClient::onConnect(uv_connect_t* req, int status) {
    auto* self = static_cast<SyncClient*>(req->data);

    if (status < 0) {
        if (self->_connectCb) self->_connectCb(false);
        return;
    }

    self->_connected = true;
    uv_read_start(reinterpret_cast<uv_stream_t*>(&self->_socket),
                  allocBuffer, onRead);

    if (self->_connectCb) self->_connectCb(true);
}

void SyncClient::allocBuffer(uv_handle_t* handle, size_t suggested, uv_buf_t* buf) {
    buf->base = new char[suggested];
    buf->len = suggested;
}

void SyncClient::onRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    auto* self = static_cast<SyncClient*>(stream->data);

    if (nread < 0) {
        delete[] buf->base;
        self->disconnect();
        return;
    }

    if (nread > 0) {
        self->onData(reinterpret_cast<const uint8_t*>(buf->base), nread);
    }

    delete[] buf->base;
}

void SyncClient::onData(const uint8_t* data, size_t len) {
    _recvBuffer.insert(_recvBuffer.end(), data, data + len);
    processMessageQueue();
}

void SyncClient::processMessageQueue() {
    while (_recvBuffer.size() >= 4) {
        uint32_t msgLen = readLength(_recvBuffer.data());
        if (_recvBuffer.size() < 4 + msgLen) break;

        // Extract message
        auto ops = OperationSerializer::deserializeMany(
            _recvBuffer.data() + 4, msgLen);

        // Remove from buffer
        _recvBuffer.erase(_recvBuffer.begin(), _recvBuffer.begin() + 4 + msgLen);

        // Deliver
        if (_onOperations && !ops.empty()) {
            _onOperations(ops);
        }
    }
}

void SyncClient::sendOperations(const std::vector<OperationPtr>& ops) {
    if (!_connected || ops.empty()) return;

    auto payload = OperationSerializer::serializeMany(ops);

    std::vector<uint8_t> msg;
    writeLength(msg, static_cast<uint32_t>(payload.size()));
    msg.insert(msg.end(), payload.begin(), payload.end());

    _sendQueue.push(std::move(msg));

    if (!_sending) {
        _sending = true;
        auto& data = _sendQueue.front();

        uv_buf_t buf = uv_buf_init(reinterpret_cast<char*>(data.data()), data.size());
        auto* req = new uv_write_t;
        req->data = this;

        uv_write(req, reinterpret_cast<uv_stream_t*>(&_socket), &buf, 1, onWrite);
    }
}

void SyncClient::onWrite(uv_write_t* req, int status) {
    auto* self = static_cast<SyncClient*>(req->data);
    delete req;

    self->_sendQueue.pop();

    if (!self->_sendQueue.empty()) {
        auto& data = self->_sendQueue.front();
        uv_buf_t buf = uv_buf_init(reinterpret_cast<char*>(data.data()), data.size());
        auto* newReq = new uv_write_t;
        newReq->data = self;
        uv_write(newReq, reinterpret_cast<uv_stream_t*>(&self->_socket), &buf, 1, onWrite);
    } else {
        self->_sending = false;
    }
}

void SyncClient::onError(int status) {
    disconnect();
}

//=============================================================================
// SyncServer
//=============================================================================

SyncServer::SyncServer(uv_loop_t* loop) : _loop(loop) {
    uv_tcp_init(_loop, &_server);
    _server.data = this;
}

SyncServer::~SyncServer() {
    stop();
}

bool SyncServer::listen(const std::string& host, int port) {
    struct sockaddr_in addr;
    uv_ip4_addr(host.c_str(), port, &addr);

    int r = uv_tcp_bind(&_server, reinterpret_cast<const sockaddr*>(&addr), 0);
    if (r != 0) return false;

    r = uv_listen(reinterpret_cast<uv_stream_t*>(&_server), 128, onConnection);
    if (r != 0) return false;

    _listening = true;
    return true;
}

void SyncServer::stop() {
    if (_listening) {
        _listening = false;
        uv_close(reinterpret_cast<uv_handle_t*>(&_server), nullptr);

        for (auto& client : _clients) {
            uv_close(reinterpret_cast<uv_handle_t*>(&client->socket), nullptr);
        }
        _clients.clear();
    }
}

void SyncServer::onConnection(uv_stream_t* server, int status) {
    if (status < 0) return;

    auto* self = static_cast<SyncServer*>(server->data);
    self->onNewConnection();
}

void SyncServer::onNewConnection() {
    auto client = std::make_unique<Client>();
    uv_tcp_init(_loop, &client->socket);
    client->socket.data = client.get();
    client->sessionId = _nextSessionId++;

    if (uv_accept(reinterpret_cast<uv_stream_t*>(&_server),
                  reinterpret_cast<uv_stream_t*>(&client->socket)) == 0) {
        uv_read_start(reinterpret_cast<uv_stream_t*>(&client->socket),
                      allocBuffer, onClientRead);

        if (_onClientConnected) {
            _onClientConnected(client->sessionId);
        }

        _clients.push_back(std::move(client));
    }
}

void SyncServer::allocBuffer(uv_handle_t* handle, size_t suggested, uv_buf_t* buf) {
    buf->base = new char[suggested];
    buf->len = suggested;
}

void SyncServer::onClientRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    auto* client = static_cast<Client*>(stream->data);

    if (nread < 0) {
        delete[] buf->base;

        // Find server from client
        // This is a bit awkward - in production you'd store server pointer
        return;
    }

    if (nread > 0) {
        client->recvBuffer.insert(client->recvBuffer.end(),
                                   buf->base, buf->base + nread);

        // Process messages
        while (client->recvBuffer.size() >= 4) {
            uint32_t msgLen = readLength(client->recvBuffer.data());
            if (client->recvBuffer.size() < 4 + msgLen) break;

            // Deserialize and broadcast
            auto ops = OperationSerializer::deserializeMany(
                client->recvBuffer.data() + 4, msgLen);

            client->recvBuffer.erase(client->recvBuffer.begin(),
                                      client->recvBuffer.begin() + 4 + msgLen);

            // TODO: Broadcast to other clients
            // This requires access to the server instance
        }
    }

    delete[] buf->base;
}

void SyncServer::broadcast(const std::vector<OperationPtr>& ops, SessionId excludeSession) {
    if (ops.empty()) return;

    auto payload = OperationSerializer::serializeMany(ops);

    std::vector<uint8_t> msg;
    writeLength(msg, static_cast<uint32_t>(payload.size()));
    msg.insert(msg.end(), payload.begin(), payload.end());

    for (auto& client : _clients) {
        if (client->sessionId == excludeSession) continue;

        uv_buf_t buf = uv_buf_init(reinterpret_cast<char*>(msg.data()), msg.size());
        auto* req = new uv_write_t;
        req->data = nullptr;

        uv_write(req, reinterpret_cast<uv_stream_t*>(&client->socket), &buf, 1,
                 [](uv_write_t* r, int) { delete r; });
    }
}

void SyncServer::removeClient(Client* client) {
    SessionId sid = client->sessionId;

    _clients.erase(
        std::remove_if(_clients.begin(), _clients.end(),
                       [client](const auto& c) { return c.get() == client; }),
        _clients.end());

    if (_onClientDisconnected) {
        _onClientDisconnected(sid);
    }
}

//=============================================================================
// DocumentSync
//=============================================================================

DocumentSync::DocumentSync(Document* doc, SyncClient::Ptr client)
    : _doc(doc), _client(std::move(client)) {}

DocumentSync::~DocumentSync() {
    stop();
}

void DocumentSync::start() {
    if (_syncing) return;
    _syncing = true;

    // Set up document sync callback
    _doc->setSyncCallback([this](const std::vector<OperationPtr>& ops) {
        onLocalOperations(ops);
    });

    // Set up client receive callback
    _client->setOnOperations([this](const std::vector<OperationPtr>& ops) {
        onRemoteOperations(ops);
    });
}

void DocumentSync::stop() {
    if (!_syncing) return;
    _syncing = false;

    _doc->setSyncCallback(nullptr);
    _client->setOnOperations(nullptr);
}

void DocumentSync::onLocalOperations(const std::vector<OperationPtr>& ops) {
    // Filter out presence-only operations for non-connected state
    if (!_client->isConnected()) return;

    _client->sendOperations(ops);
}

void DocumentSync::onRemoteOperations(const std::vector<OperationPtr>& ops) {
    _doc->receiveOperations(ops);
}

} // namespace yetty::yrich
