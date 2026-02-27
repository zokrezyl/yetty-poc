#include <yetty/rpc/rpc-client.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <afunix.h>
#include <io.h>
#pragma comment(lib, "ws2_32.lib")
using SocketType = SOCKET;
#define INVALID_SOCK INVALID_SOCKET
#define CLOSE_SOCKET closesocket
using ssize_t = int;
static std::string getSocketError() {
    int err = WSAGetLastError();
    char buf[256];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err, 0, buf, sizeof(buf), nullptr);
    return std::string(buf);
}
#else
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
using SocketType = int;
#define INVALID_SOCK (-1)
#define CLOSE_SOCKET ::close
static std::string getSocketError() {
    return std::string(strerror(errno));
}
#endif

#include <cstring>
#include <atomic>
#include <mutex>
#include <uv.h>

namespace yetty {
namespace rpc {

// ─── Platform initialization ─────────────────────────────────────────────────

#ifdef _WIN32
struct WinsockInit {
    WinsockInit() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    ~WinsockInit() {
        WSACleanup();
    }
};
static WinsockInit s_winsockInit;
#endif

// ─── Helpers ─────────────────────────────────────────────────────────────────

static std::atomic<uint32_t> s_nextMsgId{1};

// Pack a notification: [2, channel, method, params]
static void packNotification(msgpack::sbuffer& out, Channel channel,
                             const std::string& method, const msgpack::sbuffer& params) {
    msgpack::packer<msgpack::sbuffer> pk(out);
    pk.pack_array(4);
    pk.pack(static_cast<uint32_t>(MessageType::Notification));
    pk.pack(static_cast<uint32_t>(channel));
    pk.pack(method);
    // Append raw params (already packed msgpack)
    out.write(params.data(), params.size());
}

// Pack a request: [0, msgid, channel, method, params]
static void packRequest(msgpack::sbuffer& out, uint32_t msgid, Channel channel,
                        const std::string& method, const msgpack::sbuffer& params) {
    msgpack::packer<msgpack::sbuffer> pk(out);
    pk.pack_array(5);
    pk.pack(static_cast<uint32_t>(MessageType::Request));
    pk.pack(msgid);
    pk.pack(static_cast<uint32_t>(channel));
    pk.pack(method);
    out.write(params.data(), params.size());
}

// ─── Sync Implementation ─────────────────────────────────────────────────────

class RpcClientSync : public RpcClient {
public:
    explicit RpcClientSync(std::string socketPath)
        : _socketPath(std::move(socketPath)) {}

    ~RpcClientSync() override {
        disconnect();
    }

    Result<void> connect() override {
        _fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
        if (_fd == INVALID_SOCK) {
            return Err<void>("RpcClient: socket() failed: " + getSocketError());
        }

        struct sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, _socketPath.c_str(), sizeof(addr.sun_path) - 1);

        if (::connect(_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
            CLOSE_SOCKET(_fd);
            _fd = INVALID_SOCK;
            return Err<void>("RpcClient: connect() failed: " + getSocketError());
        }

        _connected = true;
        return Ok();
    }

    void disconnect() override {
        if (_fd != INVALID_SOCK) {
            CLOSE_SOCKET(_fd);
            _fd = INVALID_SOCK;
            _connected = false;
        }
    }

    // ── Channel 0: EventLoop convenience methods ──

    Result<void> keyDown(int key, int mods, int scancode) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("key"); pk.pack(key);
        pk.pack("mods"); pk.pack(mods);
        pk.pack("scancode"); pk.pack(scancode);
        return notify(Channel::EventLoop, "key_down", params);
    }

    Result<void> keyUp(int key, int mods, int scancode) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("key"); pk.pack(key);
        pk.pack("mods"); pk.pack(mods);
        pk.pack("scancode"); pk.pack(scancode);
        return notify(Channel::EventLoop, "key_up", params);
    }

    Result<void> charInput(uint32_t codepoint, int mods) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("codepoint"); pk.pack(codepoint);
        pk.pack("mods"); pk.pack(mods);
        return notify(Channel::EventLoop, "char", params);
    }

    Result<void> mouseDown(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_down", params);
    }

    Result<void> mouseUp(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_up", params);
    }

    Result<void> mouseMove(float x, float y) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        return notify(Channel::EventLoop, "mouse_move", params);
    }

    Result<void> mouseDrag(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_drag", params);
    }

    Result<void> scroll(float x, float y, float dx, float dy, int mods) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(5);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("dx"); pk.pack(dx);
        pk.pack("dy"); pk.pack(dy);
        pk.pack("mods"); pk.pack(mods);
        return notify(Channel::EventLoop, "scroll", params);
    }

    Result<void> setFocus(uint64_t objectId) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(1);
        pk.pack("object_id"); pk.pack(objectId);
        return notify(Channel::EventLoop, "set_focus", params);
    }

    Result<void> resize(float width, float height) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("width"); pk.pack(width);
        pk.pack("height"); pk.pack(height);
        return notify(Channel::EventLoop, "resize", params);
    }

    Result<void> contextMenuAction(uint64_t objectId, const std::string& action, int row, int col) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("object_id"); pk.pack(objectId);
        pk.pack("action"); pk.pack(action);
        pk.pack("row"); pk.pack(row);
        pk.pack("col"); pk.pack(col);
        return notify(Channel::EventLoop, "context_menu_action", params);
    }

    Result<void> cardMouseDown(uint64_t targetId, float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "card_mouse_down", params);
    }

    Result<void> cardMouseUp(uint64_t targetId, float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "card_mouse_up", params);
    }

    Result<void> cardMouseMove(uint64_t targetId, float x, float y) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        return notify(Channel::EventLoop, "card_mouse_move", params);
    }

    Result<void> cardScroll(uint64_t targetId, float x, float y, float dx, float dy) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(5);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("dx"); pk.pack(dx);
        pk.pack("dy"); pk.pack(dy);
        return notify(Channel::EventLoop, "card_scroll", params);
    }

    Result<void> close(uint64_t objectId) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(1);
        pk.pack("object_id"); pk.pack(objectId);
        auto res = request(Channel::EventLoop, "close", params);
        if (!res) {
            return Err<void>("close failed", res);
        }
        return Ok();
    }

    Result<void> split(uint64_t objectId, int orientation) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("object_id"); pk.pack(objectId);
        pk.pack("orientation"); pk.pack(orientation);
        auto res = request(Channel::EventLoop, "split", params);
        if (!res) {
            return Err<void>("split failed", res);
        }
        return Ok();
    }

    Result<std::string> uiTree() override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(0);
        auto res = request(Channel::EventLoop, "ui_tree", params);
        if (!res) {
            return Err<std::string>("ui_tree failed", res);
        }
        return Ok(res->get().as<std::string>());
    }

    // ── Generic send methods ──

    Result<void> notify(Channel channel, const std::string& method, const msgpack::sbuffer& params) override {
        if (!_connected) {
            return Err<void>("RpcClient: not connected");
        }

        msgpack::sbuffer out;
        packNotification(out, channel, method, params);
        return sendAll(out);
    }

    Result<msgpack::object_handle> request(Channel channel, const std::string& method, const msgpack::sbuffer& params) override {
        if (!_connected) {
            return Err<msgpack::object_handle>("RpcClient: not connected");
        }

        uint32_t msgid = s_nextMsgId.fetch_add(1);
        msgpack::sbuffer out;
        packRequest(out, msgid, channel, method, params);

        auto res = sendAll(out);
        if (!res) {
            return Err<msgpack::object_handle>("RpcClient: send failed", res);
        }

        return recvResponse(msgid);
    }

private:
    Result<void> sendAll(const msgpack::sbuffer& buf) {
        const char* data = buf.data();
        size_t remaining = buf.size();
        while (remaining > 0) {
            ssize_t n = ::send(_fd, data, static_cast<int>(remaining), 0);
            if (n < 0) {
                return Err<void>("RpcClient: send() failed: " + getSocketError());
            }
            data += n;
            remaining -= static_cast<size_t>(n);
        }
        return Ok();
    }

    Result<msgpack::object_handle> recvResponse(uint32_t expectedMsgId) {
        msgpack::unpacker unpacker;
        unpacker.reserve_buffer(65536);

        while (true) {
            ssize_t n = ::recv(_fd, unpacker.buffer(), static_cast<int>(unpacker.buffer_capacity()), 0);
            if (n <= 0) {
                return Err<msgpack::object_handle>("RpcClient: recv() failed");
            }
            unpacker.buffer_consumed(static_cast<size_t>(n));

            msgpack::object_handle oh;
            while (unpacker.next(oh)) {
                auto& obj = oh.get();
                if (obj.type != msgpack::type::ARRAY || obj.via.array.size < 4) {
                    continue;
                }

                auto& arr = obj.via.array;
                uint32_t type = arr.ptr[0].as<uint32_t>();
                if (type != static_cast<uint32_t>(MessageType::Response)) {
                    continue;
                }

                uint32_t msgid = arr.ptr[1].as<uint32_t>();
                if (msgid != expectedMsgId) {
                    continue;
                }

                // [1, msgid, error, result]
                auto& error = arr.ptr[2];
                if (error.type != msgpack::type::NIL) {
                    std::string errMsg = error.as<std::string>();
                    return Err<msgpack::object_handle>("RpcClient: server error: " + errMsg);
                }

                return msgpack::clone(arr.ptr[3]);
            }
        }
    }

    std::string _socketPath;
    SocketType _fd = INVALID_SOCK;
    bool _connected = false;
};

// ─── Async Implementation ────────────────────────────────────────────────────

class RpcClientAsync : public RpcClient {
public:
    RpcClientAsync(std::string socketPath, uv_loop_t* loop)
        : _socketPath(std::move(socketPath)), _loop(loop) {}

    ~RpcClientAsync() override {
        disconnect();
    }

    Result<void> connect() override {
        uv_pipe_init(_loop, &_pipe, 0);
        _pipe.data = this;

        auto* req = new uv_connect_t;
        req->data = this;

        uv_pipe_connect(req, &_pipe, _socketPath.c_str(), onConnect);
        _connecting = true;
        return Ok();
    }

    void disconnect() override {
        if (_connected || _connecting) {
            if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&_pipe))) {
                uv_close(reinterpret_cast<uv_handle_t*>(&_pipe), nullptr);
            }
            _connected = false;
            _connecting = false;
        }
    }

    // ── Channel 0: EventLoop convenience methods ──
    // These delegate to the same notify() as sync mode

    Result<void> keyDown(int key, int mods, int scancode) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("key"); pk.pack(key);
        pk.pack("mods"); pk.pack(mods);
        pk.pack("scancode"); pk.pack(scancode);
        return notify(Channel::EventLoop, "key_down", params);
    }

    Result<void> keyUp(int key, int mods, int scancode) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("key"); pk.pack(key);
        pk.pack("mods"); pk.pack(mods);
        pk.pack("scancode"); pk.pack(scancode);
        return notify(Channel::EventLoop, "key_up", params);
    }

    Result<void> charInput(uint32_t codepoint, int mods) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("codepoint"); pk.pack(codepoint);
        pk.pack("mods"); pk.pack(mods);
        return notify(Channel::EventLoop, "char", params);
    }

    Result<void> mouseDown(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_down", params);
    }

    Result<void> mouseUp(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_up", params);
    }

    Result<void> mouseMove(float x, float y) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        return notify(Channel::EventLoop, "mouse_move", params);
    }

    Result<void> mouseDrag(float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "mouse_drag", params);
    }

    Result<void> scroll(float x, float y, float dx, float dy, int mods) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(5);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("dx"); pk.pack(dx);
        pk.pack("dy"); pk.pack(dy);
        pk.pack("mods"); pk.pack(mods);
        return notify(Channel::EventLoop, "scroll", params);
    }

    Result<void> setFocus(uint64_t objectId) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(1);
        pk.pack("object_id"); pk.pack(objectId);
        return notify(Channel::EventLoop, "set_focus", params);
    }

    Result<void> resize(float width, float height) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("width"); pk.pack(width);
        pk.pack("height"); pk.pack(height);
        return notify(Channel::EventLoop, "resize", params);
    }

    Result<void> contextMenuAction(uint64_t objectId, const std::string& action, int row, int col) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("object_id"); pk.pack(objectId);
        pk.pack("action"); pk.pack(action);
        pk.pack("row"); pk.pack(row);
        pk.pack("col"); pk.pack(col);
        return notify(Channel::EventLoop, "context_menu_action", params);
    }

    Result<void> cardMouseDown(uint64_t targetId, float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "card_mouse_down", params);
    }

    Result<void> cardMouseUp(uint64_t targetId, float x, float y, int button) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(4);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("button"); pk.pack(button);
        return notify(Channel::EventLoop, "card_mouse_up", params);
    }

    Result<void> cardMouseMove(uint64_t targetId, float x, float y) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(3);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        return notify(Channel::EventLoop, "card_mouse_move", params);
    }

    Result<void> cardScroll(uint64_t targetId, float x, float y, float dx, float dy) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(5);
        pk.pack("target_id"); pk.pack(targetId);
        pk.pack("x"); pk.pack(x);
        pk.pack("y"); pk.pack(y);
        pk.pack("dx"); pk.pack(dx);
        pk.pack("dy"); pk.pack(dy);
        return notify(Channel::EventLoop, "card_scroll", params);
    }

    Result<void> close(uint64_t objectId) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(1);
        pk.pack("object_id"); pk.pack(objectId);
        auto res = request(Channel::EventLoop, "close", params);
        if (!res) {
            return Err<void>("close failed", res);
        }
        return Ok();
    }

    Result<void> split(uint64_t objectId, int orientation) override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(2);
        pk.pack("object_id"); pk.pack(objectId);
        pk.pack("orientation"); pk.pack(orientation);
        auto res = request(Channel::EventLoop, "split", params);
        if (!res) {
            return Err<void>("split failed", res);
        }
        return Ok();
    }

    Result<std::string> uiTree() override {
        msgpack::sbuffer params;
        msgpack::packer<msgpack::sbuffer> pk(params);
        pk.pack_map(0);
        auto res = request(Channel::EventLoop, "ui_tree", params);
        if (!res) {
            return Err<std::string>("ui_tree failed", res);
        }
        return Ok(res->get().as<std::string>());
    }

    // ── Generic send methods ──

    Result<void> notify(Channel channel, const std::string& method, const msgpack::sbuffer& params) override {
        if (!_connected) {
            return Err<void>("RpcClient: not connected");
        }

        msgpack::sbuffer out;
        packNotification(out, channel, method, params);
        return asyncWrite(out);
    }

    Result<msgpack::object_handle> request(Channel channel, const std::string& method, const msgpack::sbuffer& params) override {
        // Async request: queues the write; response handling requires the caller
        // to run the event loop and handle responses via a callback mechanism.
        // For now, we queue the request — full async response correlation will
        // be added when needed.
        if (!_connected) {
            return Err<msgpack::object_handle>("RpcClient: not connected");
        }

        uint32_t msgid = s_nextMsgId.fetch_add(1);
        msgpack::sbuffer out;
        packRequest(out, msgid, channel, method, params);

        auto res = asyncWrite(out);
        if (!res) {
            return Err<msgpack::object_handle>("RpcClient: async write failed", res);
        }

        // Return empty result — async callers should not block on response
        msgpack::sbuffer empty;
        msgpack::pack(empty, true);
        return msgpack::unpack(empty.data(), empty.size());
    }

private:
    static void onConnect(uv_connect_t* req, int status) {
        auto* self = static_cast<RpcClientAsync*>(req->data);
        delete req;
        self->_connecting = false;

        if (status < 0) {
            self->_connected = false;
            return;
        }

        self->_connected = true;
    }

    Result<void> asyncWrite(const msgpack::sbuffer& buf) {
        auto* data = new char[buf.size()];
        std::memcpy(data, buf.data(), buf.size());

        auto* req = new uv_write_t;
        auto* uvBuf = new uv_buf_t;
        uvBuf->base = data;
        uvBuf->len = buf.size();
        req->data = uvBuf;

        int r = uv_write(req, reinterpret_cast<uv_stream_t*>(&_pipe),
                         uvBuf, 1, onWriteDone);
        if (r != 0) {
            delete[] data;
            delete uvBuf;
            delete req;
            return Err<void>("RpcClient: uv_write failed: " + std::string(uv_strerror(r)));
        }

        return Ok();
    }

    static void onWriteDone(uv_write_t* req, int /*status*/) {
        auto* buf = static_cast<uv_buf_t*>(req->data);
        delete[] buf->base;
        delete buf;
        delete req;
    }

    std::string _socketPath;
    uv_loop_t* _loop;
    uv_pipe_t _pipe{};
    bool _connected = false;
    bool _connecting = false;
};

// ─── Factory implementations ─────────────────────────────────────────────────

Result<RpcClient::Ptr> RpcClient::createImpl(const std::string& socketPath) noexcept {
    return Ok(Ptr(new RpcClientSync(socketPath)));
}

Result<RpcClient::Ptr> RpcClient::createImpl(const std::string& socketPath, uv_loop_t* loop) noexcept {
    if (!loop) {
        return Err<Ptr>("RpcClient: null uv_loop_t*");
    }
    return Ok(Ptr(new RpcClientAsync(socketPath, loop)));
}

} // namespace rpc
} // namespace yetty
