/**
 * plot-client demo - Demonstrates streaming data to a yetty plot card
 *
 * Usage:
 *   1. Start yetty terminal (exports YETTY_SOCKET automatically)
 *   2. Run this client: ./plot-client [card-name]
 *   3. Watch the plot update with simulated audio waveform
 *
 * The client creates the plot card via OSC escape sequence, then
 * streams data directly to shared memory.
 */

#include <yetty/shared-memory.h>
#include <yetty/rpc/stream-types.h>
#include <msgpack.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <chrono>
#include <iostream>
#include <thread>
#include <optional>

using namespace yetty;
using namespace yetty::rpc;

// Simple Result type for the client
template<typename T>
struct ClientResult {
    std::optional<T> value;
    std::string error;

    bool ok() const { return value.has_value(); }
    operator bool() const { return ok(); }
    T& operator*() { return *value; }
    const T& operator*() const { return *value; }

    static ClientResult Ok(T v) { return {std::move(v), ""}; }
    static ClientResult Err(std::string msg) { return {{}, std::move(msg)}; }
};

template<>
struct ClientResult<void> {
    bool success;
    std::string error;

    bool ok() const { return success; }
    operator bool() const { return ok(); }

    static ClientResult Ok() { return {true, ""}; }
    static ClientResult Err(std::string msg) { return {false, std::move(msg)}; }
};

// Simple RPC client for msgpack-rpc
class RpcClient {
public:
    ClientResult<void> connect(const std::string& socketPath) {
        _fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (_fd < 0) {
            return ClientResult<void>::Err("Failed to create socket");
        }

        struct sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);

        if (::connect(_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(_fd);
            _fd = -1;
            return ClientResult<void>::Err("Failed to connect to " + socketPath);
        }
        return ClientResult<void>::Ok();
    }

    ~RpcClient() {
        if (_fd >= 0) close(_fd);
    }

    // Send request and get response
    ClientResult<msgpack::object_handle> call(uint32_t channel, const std::string& method,
                                 const std::map<std::string, msgpack::object>& params = {}) {
        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pk(&sbuf);

        // [type=0, msgid, channel, method, params]
        pk.pack_array(5);
        pk.pack(uint32_t(0));  // Request
        pk.pack(_msgid++);
        pk.pack(channel);
        pk.pack(method);
        pk.pack(params);

        // Send
        ssize_t sent = write(_fd, sbuf.data(), sbuf.size());
        if (sent != static_cast<ssize_t>(sbuf.size())) {
            return ClientResult<msgpack::object_handle>::Err("Failed to send RPC request");
        }

        // Receive response
        char buf[4096];
        ssize_t n = read(_fd, buf, sizeof(buf));
        if (n <= 0) {
            return ClientResult<msgpack::object_handle>::Err("Failed to read RPC response");
        }

        auto oh = msgpack::unpack(buf, n);
        auto& arr = oh.get().via.array;

        // [type=1, msgid, error, result]
        if (arr.size != 4 || arr.ptr[0].as<uint32_t>() != 1) {
            return ClientResult<msgpack::object_handle>::Err("Invalid RPC response format");
        }

        if (arr.ptr[2].type != msgpack::type::NIL) {
            return ClientResult<msgpack::object_handle>::Err("RPC error: " + arr.ptr[2].as<std::string>());
        }

        // Return result
        msgpack::sbuffer resbuf;
        msgpack::pack(resbuf, arr.ptr[3]);
        return ClientResult<msgpack::object_handle>::Ok(msgpack::unpack(resbuf.data(), resbuf.size()));
    }

private:
    int _fd = -1;
    uint32_t _msgid = 1;
};

int main(int argc, char* argv[]) {
    // Default card name
    std::string cardName = argc > 1 ? argv[1] : "audio";

    // Get socket path from environment
    const char* socketPath = getenv("YETTY_SOCKET");
    if (!socketPath) {
        std::cerr << "Error: YETTY_SOCKET environment variable not set.\n";
        std::cerr << "Run this from within a yetty terminal.\n";
        return 1;
    }

    // Create the plot card via OSC escape sequence
    // OSC format: 666666;generic-args;plugin-args;payload
    // --buffer waveform=400 declares a buffer named "waveform" with 400 floats
    constexpr uint32_t kBufferSize = 400;
    const std::string kBufferName = "waveform";
    std::string osc = "\033]666666;run -c plot -x 0 -y 0 -w 80 -h 20 -r --name " + cardName +
                      ";--buffer " + kBufferName + "=" + std::to_string(kBufferSize) + ";\033\\";
    write(STDOUT_FILENO, osc.c_str(), osc.size());
    write(STDOUT_FILENO, "\n", 1);

    // Give the terminal time to create the card
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cerr << "Connecting to " << socketPath << "...\n";

    RpcClient client;
    if (auto res = client.connect(socketPath); !res) {
        std::cerr << "Error: " << res.error << "\n";
        return 1;
    }

    std::cerr << "Connected. Calling stream_connect...\n";

    // Call stream_connect to get shared memory info
    auto connectResult = client.call(1, "stream_connect");
    if (!connectResult) {
        std::cerr << "Error: " << connectResult.error << "\n";
        return 1;
    }

    auto& connectMap = (*connectResult).get().via.map;

    std::string shmName;
    uint64_t shmSize = 0;

    for (uint32_t i = 0; i < connectMap.size; ++i) {
        std::string key(connectMap.ptr[i].key.via.str.ptr, connectMap.ptr[i].key.via.str.size);
        if (key == "data_shm") {
            shmName = std::string(connectMap.ptr[i].val.via.str.ptr,
                                   connectMap.ptr[i].val.via.str.size);
        } else if (key == "data_size") {
            shmSize = connectMap.ptr[i].val.as<uint64_t>();
        }
    }

    if (shmName.empty() || shmSize == 0) {
        std::cerr << "Error: Shared memory not available (streaming disabled on server)\n";
        return 1;
    }

    std::cerr << "Shared memory: " << shmName << " (" << shmSize << " bytes)\n";

    // Open shared memory
    auto shmResult = SharedMemoryRegion::open(shmName);
    if (!shmResult) {
        std::cerr << "Error: Failed to open shared memory: " << shmResult.error().message() << "\n";
        return 1;
    }
    auto shm = *shmResult;
    std::cerr << "Mapped shared memory at " << shm->data() << "\n";

    // Get buffer offset for the named card and buffer
    std::cerr << "Querying buffer '" << kBufferName << "' for card '" << cardName << "'...\n";

    msgpack::zone z;
    std::map<std::string, msgpack::object> bufParams;
    bufParams["name"] = msgpack::object(cardName, z);
    bufParams["scope"] = msgpack::object(kBufferName, z);

    auto bufResult = client.call(1, "stream_get_buffer", bufParams);
    if (!bufResult) {
        std::cerr << "Error: " << bufResult.error << "\n";
        return 1;
    }

    auto& bufMap = (*bufResult).get().via.map;
    uint32_t offset = 0, size = 0;

    for (uint32_t i = 0; i < bufMap.size; ++i) {
        std::string key(bufMap.ptr[i].key.via.str.ptr, bufMap.ptr[i].key.via.str.size);
        if (key == "offset") {
            offset = bufMap.ptr[i].val.as<uint32_t>();
        } else if (key == "size") {
            size = bufMap.ptr[i].val.as<uint32_t>();
        }
    }

    std::cerr << "Buffer offset=" << offset << " size=" << size << "\n";

    if (size == 0) {
        std::cerr << "Error: Buffer not allocated. Plot card may not have data yet.\n";
        return 1;
    }

    // Get pointer to data in shared memory
    float* data = reinterpret_cast<float*>(static_cast<uint8_t*>(shm->data()) + offset);
    uint32_t numFloats = size / sizeof(float);

    std::cerr << "Streaming " << numFloats << " floats. Press Ctrl+C to stop.\n";

    // Simulate audio waveform - continuously update
    double phase = 0.0;
    const double freq1 = 2.0;   // Hz
    const double freq2 = 5.0;
    const double sampleRate = 60.0;  // updates per second

    while (true) {
        // Generate waveform: sum of two sine waves
        for (uint32_t i = 0; i < numFloats; ++i) {
            double t = phase + i * 0.05;
            data[i] = static_cast<float>(
                0.5 * std::sin(freq1 * 2.0 * M_PI * t) +
                0.3 * std::sin(freq2 * 2.0 * M_PI * t) +
                0.1 * std::sin(freq1 * freq2 * 2.0 * M_PI * t)
            );
        }

        // Notify yetty that buffer data changed
        auto dirtyResult = client.call(1, "stream_mark_dirty", bufParams);
        if (!dirtyResult) {
            std::cerr << "Warning: stream_mark_dirty failed: " << dirtyResult.error << "\n";
        }

        phase += 1.0 / sampleRate;

        // Sleep ~16ms (60fps)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
