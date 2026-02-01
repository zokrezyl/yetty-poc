#include <yetty/rpc/rpc-server.h>
#include <ytrace/ytrace.hpp>

#include <unordered_map>
#include <uv.h>

namespace yetty {
namespace rpc {

// Key for (channel, method) handler lookup
struct HandlerKey {
    Channel     channel;
    std::string method;

    bool operator==(const HandlerKey& o) const {
        return channel == o.channel && method == o.method;
    }
};

struct HandlerKeyHash {
    std::size_t operator()(const HandlerKey& k) const noexcept {
        auto h1 = std::hash<uint32_t>{}(static_cast<uint32_t>(k.channel));
        auto h2 = std::hash<std::string>{}(k.method);
        return h1 ^ (h2 << 1);
    }
};

// Per-client connection state
struct ClientConnection {
    uv_pipe_t             handle;
    msgpack::unpacker     unpacker;
    RpcServer*            server = nullptr;  // back-pointer for callbacks

    ClientConnection() {
        unpacker.reserve_buffer(65536);
    }
};

class RpcServerImpl : public RpcServer {
public:
    explicit RpcServerImpl(std::string socketPath)
        : _socketPath(std::move(socketPath)) {}

    ~RpcServerImpl() override {
        stop();
    }

    Result<void> init() noexcept {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("RpcServer: no EventLoop", loopResult);
        }
        _eventLoop = *loopResult;
        return Ok();
    }

    Result<void> start() override {
        // Get the libuv loop from the EventLoop (it uses uv_default_loop)
        _uvLoop = uv_default_loop();

        // Initialize the pipe (Unix domain socket) server
        uv_pipe_init(_uvLoop, &_serverPipe, 0);
        _serverPipe.data = this;

        // Remove stale socket file if it exists
        unlink(_socketPath.c_str());

        int r = uv_pipe_bind(&_serverPipe, _socketPath.c_str());
        if (r != 0) {
            return Err<void>("RpcServer: bind failed: " + std::string(uv_strerror(r)));
        }

        r = uv_listen(reinterpret_cast<uv_stream_t*>(&_serverPipe), 8, onNewConnection);
        if (r != 0) {
            return Err<void>("RpcServer: listen failed: " + std::string(uv_strerror(r)));
        }

        _running = true;
        yinfo("RpcServer: listening on {}", _socketPath);
        return Ok();
    }

    Result<void> stop() override {
        if (!_running) return Ok();
        _running = false;

        // Close all client connections
        for (auto& [ptr, conn] : _clients) {
            if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&conn->handle))) {
                uv_close(reinterpret_cast<uv_handle_t*>(&conn->handle), onClientClosed);
            }
        }
        _clients.clear();

        // Close server pipe
        if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&_serverPipe))) {
            uv_close(reinterpret_cast<uv_handle_t*>(&_serverPipe), nullptr);
        }

        unlink(_socketPath.c_str());
        yinfo("RpcServer: stopped");
        return Ok();
    }

    void registerHandler(Channel channel, const std::string& method, RpcHandler handler) override {
        _handlers[{channel, method}] = std::move(handler);
        yinfo("RpcServer: registered handler for channel={} method={}",
              static_cast<uint32_t>(channel), method);
    }

    const std::string& socketPath() const override {
        return _socketPath;
    }

    // EventListener interface (unused - we use libuv callbacks directly)
    Result<bool> onEvent(const base::Event& /*event*/) override {
        return Ok(false);
    }

private:
    // --- libuv callbacks (static, C-style) ---

    static void onNewConnection(uv_stream_t* server, int status) {
        auto* self = static_cast<RpcServerImpl*>(server->data);
        if (status < 0) {
            yerror("RpcServer: accept error: {}", uv_strerror(status));
            return;
        }

        auto conn = std::make_unique<ClientConnection>();
        conn->server = self;
        uv_pipe_init(self->_uvLoop, &conn->handle, 0);
        conn->handle.data = conn.get();

        int r = uv_accept(server, reinterpret_cast<uv_stream_t*>(&conn->handle));
        if (r != 0) {
            yerror("RpcServer: accept failed: {}", uv_strerror(r));
            return;
        }

        auto* raw = conn.get();
        self->_clients[raw] = std::move(conn);

        uv_read_start(
            reinterpret_cast<uv_stream_t*>(&raw->handle),
            onAllocBuffer,
            onRead
        );

        yinfo("RpcServer: client connected (total={})", self->_clients.size());
    }

    // libuv asks us where to read into — hand it the unpacker's
    // internal buffer directly so data lands there with no copy.
    static void onAllocBuffer(uv_handle_t* handle, size_t suggested, uv_buf_t* buf) {
        auto* conn = static_cast<ClientConnection*>(handle->data);
        conn->unpacker.reserve_buffer(std::max(suggested, size_t(65536)));
        buf->base = conn->unpacker.buffer();
        buf->len  = conn->unpacker.buffer_capacity();
    }

    static void onRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* /*buf*/) {
        auto* conn = static_cast<ClientConnection*>(stream->data);
        auto* self = static_cast<RpcServerImpl*>(conn->server);

        if (nread < 0) {
            if (nread != UV_EOF) {
                yerror("RpcServer: read error: {}", uv_strerror(static_cast<int>(nread)));
            }
            yinfo("RpcServer: client disconnected");
            uv_close(reinterpret_cast<uv_handle_t*>(stream), onClientClosed);
            return;
        }

        if (nread > 0) {
            // Data already sits in the unpacker buffer (from onAllocBuffer),
            // just tell it how many bytes libuv actually wrote.
            conn->unpacker.buffer_consumed(static_cast<size_t>(nread));

            // Process all complete messages
            msgpack::object_handle oh;
            while (conn->unpacker.next(oh)) {
                self->processMessage(conn, oh);
            }
        }
    }

    static void onClientClosed(uv_handle_t* handle) {
        auto* conn = static_cast<ClientConnection*>(handle->data);
        auto* self = static_cast<RpcServerImpl*>(conn->server);
        self->_clients.erase(conn);
    }

    // --- Message processing ---

    void processMessage(ClientConnection* conn, msgpack::object_handle& oh) {
        auto& obj = oh.get();

        if (obj.type != msgpack::type::ARRAY) {
            yerror("RpcServer: expected array, got type={}", static_cast<int>(obj.type));
            return;
        }

        auto& arr = obj.via.array;
        if (arr.size < 3) {
            yerror("RpcServer: message too short (size={})", arr.size);
            return;
        }

        uint32_t type = arr.ptr[0].as<uint32_t>();

        if (type == static_cast<uint32_t>(MessageType::Request)) {
            // [0, msgid, channel, method, params]
            if (arr.size < 5) {
                yerror("RpcServer: request too short");
                return;
            }
            RpcMessage msg;
            msg.type    = MessageType::Request;
            msg.msgid   = arr.ptr[1].as<uint32_t>();
            msg.channel = arr.ptr[2].as<uint32_t>();
            msg.method  = arr.ptr[3].as<std::string>();
            // Keep params as object_handle for lazy decode
            msg.params  = msgpack::clone(arr.ptr[4]);

            ydebug("RpcServer: request msgid={} ch={} method={}",
                   msg.msgid, msg.channel, msg.method);

            handleRequest(conn, msg);

        } else if (type == static_cast<uint32_t>(MessageType::Notification)) {
            // [2, channel, method, params]
            if (arr.size < 4) {
                yerror("RpcServer: notification too short");
                return;
            }
            RpcMessage msg;
            msg.type    = MessageType::Notification;
            msg.msgid   = 0;
            msg.channel = arr.ptr[1].as<uint32_t>();
            msg.method  = arr.ptr[2].as<std::string>();
            msg.params  = msgpack::clone(arr.ptr[3]);

            ydebug("RpcServer: notification ch={} method={}", msg.channel, msg.method);

            handleNotification(msg);
        } else {
            yerror("RpcServer: unknown message type={}", type);
        }
    }

    void handleRequest(ClientConnection* conn, const RpcMessage& msg) {
        auto channel = static_cast<Channel>(msg.channel);
        HandlerKey key{channel, msg.method};

        auto it = _handlers.find(key);
        if (it == _handlers.end()) {
            sendError(conn, msg.msgid, "unknown method: " + msg.method);
            return;
        }

        try {
            auto result = it->second(msg);
            sendResult(conn, msg.msgid, result.get());
        } catch (const std::exception& e) {
            sendError(conn, msg.msgid, e.what());
        }
    }

    void handleNotification(const RpcMessage& msg) {
        auto channel = static_cast<Channel>(msg.channel);
        HandlerKey key{channel, msg.method};

        auto it = _handlers.find(key);
        if (it == _handlers.end()) {
            ywarn("RpcServer: no handler for ch={} method={}",
                  msg.channel, msg.method);
            return;
        }

        try {
            it->second(msg);
        } catch (const std::exception& e) {
            yerror("RpcServer: notification handler threw: {}", e.what());
        }
    }

    // --- Response serialization ---

    void sendResult(ClientConnection* conn, uint32_t msgid, const msgpack::object& result) {
        // [1, msgid, nil, result]
        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pk(sbuf);
        pk.pack_array(4);
        pk.pack(static_cast<uint32_t>(MessageType::Response));
        pk.pack(msgid);
        pk.pack_nil();  // no error
        pk.pack(result);

        writeToClient(conn, sbuf);
    }

    void sendError(ClientConnection* conn, uint32_t msgid, const std::string& error) {
        // [1, msgid, error, nil]
        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pk(sbuf);
        pk.pack_array(4);
        pk.pack(static_cast<uint32_t>(MessageType::Response));
        pk.pack(msgid);
        pk.pack(error);
        pk.pack_nil();  // no result

        writeToClient(conn, sbuf);
    }

    void writeToClient(ClientConnection* conn, const msgpack::sbuffer& sbuf) {
        // Copy buffer for async write
        auto* data = new char[sbuf.size()];
        std::memcpy(data, sbuf.data(), sbuf.size());

        auto* req = new uv_write_t;
        auto* bufPtr = new uv_buf_t;
        bufPtr->base = data;
        bufPtr->len = sbuf.size();
        req->data = bufPtr;

        uv_write(req, reinterpret_cast<uv_stream_t*>(&conn->handle),
                 bufPtr, 1, onWriteDone);
    }

    static void onWriteDone(uv_write_t* req, int status) {
        if (status < 0) {
            yerror("RpcServer: write failed: {}", uv_strerror(status));
        }
        auto* buf = static_cast<uv_buf_t*>(req->data);
        delete[] buf->base;
        delete buf;
        delete req;
    }

    // --- State ---
    std::string _socketPath;
    base::EventLoop::Ptr _eventLoop;
    uv_loop_t* _uvLoop = nullptr;
    uv_pipe_t _serverPipe{};
    bool _running = false;

    std::unordered_map<ClientConnection*, std::unique_ptr<ClientConnection>> _clients;
    std::unordered_map<HandlerKey, RpcHandler, HandlerKeyHash> _handlers;
};

// Factory
Result<RpcServer::Ptr> RpcServer::createImpl(const std::string& socketPath) noexcept {
    auto impl = Ptr(new RpcServerImpl(socketPath));
    auto* implPtr = static_cast<RpcServerImpl*>(impl.get());
    if (auto res = implPtr->init(); !res) {
        return Err<Ptr>("RpcServer init failed", res);
    }
    return Ok(std::move(impl));
}

} // namespace rpc
} // namespace yetty
