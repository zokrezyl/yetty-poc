// VNC Recorder - Records VNC server output to MP4 file
// Connects as a headless VNC client, requests H.264 encoding,
// and muxes the NAL stream into an MP4 container.

#define MINIMP4_IMPLEMENTATION
#include <minimp4.h>

#include <string>
#include "../../src/yetty/vnc/protocol.h"
#include "../../src/yetty/vnc/socket-compat.h"
#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>
#include <args.hxx>

#include <chrono>
#include <csignal>
#include <cstring>
#include <atomic>
#include <vector>

using namespace yetty::vnc;

namespace {

std::atomic<bool> g_running{true};

void signalHandler(int) {
    g_running = false;
}

// minimp4 file write callback
int mp4WriteCallback(int64_t offset, const void* buffer, size_t size, void* token) {
    FILE* f = static_cast<FILE*>(token);
    if (fseeko(f, offset, SEEK_SET) != 0)
        return -1;
    return fwrite(buffer, 1, size, f) != size ? -1 : 0;
}

// Recv that reads exactly 'needed' bytes, using poll() to allow SIGINT handling
bool recvExact(int fd, void* buf, size_t needed) {
    uint8_t* p = static_cast<uint8_t*>(buf);
    size_t got = 0;
    while (got < needed && g_running) {
        struct pollfd pfd = {fd, POLLIN, 0};
        int pr = poll(&pfd, 1, 500);  // 500ms timeout to check g_running
        if (pr < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        if (pr == 0) continue;  // timeout, check g_running
        if (pfd.revents & (POLLERR | POLLHUP)) return false;

        auto n = sock::recv(fd, p + got, needed - got, 0);
        if (n <= 0) return false;
        got += n;
    }
    return got == needed;
}

// Send a message to the server
bool sendMsg(int fd, const void* data, size_t size) {
    const uint8_t* p = static_cast<const uint8_t*>(data);
    size_t sent = 0;
    while (sent < size) {
        auto n = sock::send(fd, p + sent, size - sent, MSG_NOSIGNAL);
        if (n <= 0) return false;
        sent += n;
    }
    return true;
}

void sendResize(int fd, uint16_t width, uint16_t height) {
    InputHeader ih;
    ih.type = static_cast<uint8_t>(InputType::RESIZE);
    ih.reserved = 0;
    ih.data_size = sizeof(ResizeEvent);

    ResizeEvent ev;
    ev.width = width;
    ev.height = height;

    uint8_t buf[sizeof(ih) + sizeof(ev)];
    std::memcpy(buf, &ih, sizeof(ih));
    std::memcpy(buf + sizeof(ih), &ev, sizeof(ev));
    sendMsg(fd, buf, sizeof(buf));
}

void sendCompressionConfig(int fd, uint8_t codec) {
    InputHeader ih;
    ih.type = static_cast<uint8_t>(InputType::COMPRESSION_CONFIG);
    ih.reserved = 0;
    ih.data_size = sizeof(CompressionConfigEvent);

    CompressionConfigEvent ev;
    ev.forceRaw = 0;
    ev.quality = 0;
    ev.alwaysFull = 1;
    ev.codec = codec;

    uint8_t buf[sizeof(ih) + sizeof(ev)];
    std::memcpy(buf, &ih, sizeof(ih));
    std::memcpy(buf + sizeof(ih), &ev, sizeof(ev));
    sendMsg(fd, buf, sizeof(buf));
}

void sendFrameAck(int fd) {
    InputHeader ih;
    ih.type = static_cast<uint8_t>(InputType::FRAME_ACK);
    ih.reserved = 0;
    ih.data_size = 0;
    sendMsg(fd, &ih, sizeof(ih));
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);

    args::ArgumentParser parser("vnc-recorder", "Record VNC server output to MP4");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::ValueFlag<std::string> hostFlag(parser, "host", "Server host (default localhost)", {"host"}, "localhost");
    args::ValueFlag<uint16_t> portFlag(parser, "port", "Server port (default 5900)", {'p', "port"}, 5900);
    args::ValueFlag<uint16_t> widthFlag(parser, "width", "Override recording width (default: use server size)", {'W', "width"}, 0);
    args::ValueFlag<uint16_t> heightFlag(parser, "height", "Override recording height (default: use server size)", {'H', "height"}, 0);
    args::ValueFlag<std::string> outputFlag(parser, "output", "Output MP4 file (default recording.mp4)", {'o', "output"}, "recording.mp4");
    args::ValueFlag<uint32_t> durationFlag(parser, "seconds", "Recording duration in seconds (0 = unlimited)", {'d', "duration"}, 0);
    args::Flag verboseFlag(parser, "verbose", "Enable verbose logging", {'v', "verbose"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (verboseFlag) spdlog::set_level(spdlog::level::debug);

    std::string host = args::get(hostFlag);
    uint16_t port = args::get(portFlag);
    uint16_t recWidth = args::get(widthFlag);
    uint16_t recHeight = args::get(heightFlag);
    std::string outputPath = args::get(outputFlag);
    uint32_t maxDuration = args::get(durationFlag);

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    yinfo("VNC Recorder: connecting to {}:{}", host, port);
    if (recWidth > 0 && recHeight > 0)
        yinfo("Recording {}x{} -> {}", recWidth, recHeight, outputPath);
    else
        yinfo("Recording (server size) -> {}", outputPath);

    // Connect to server
    sock::init();

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        yerror("Failed to create socket");
        return 1;
    }

    // Disable Nagle for low latency
    int one = 1;
    sock::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, one);

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    struct addrinfo hints = {}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0 || !res) {
        yerror("Failed to resolve host: {}", host);
        sock::close(fd);
        return 1;
    }
    addr.sin_addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr)->sin_addr;
    freeaddrinfo(res);

    if (::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
        yerror("Failed to connect to {}:{}", host, port);
        sock::close(fd);
        return 1;
    }

    yinfo("Connected to {}:{}", host, port);

    // Only send resize if dimensions were explicitly specified
    if (recWidth > 0 && recHeight > 0) {
        sendResize(fd, recWidth, recHeight);
    }
    sendCompressionConfig(fd, CODEC_H264);

    // Open MP4 output
    FILE* mp4File = fopen(outputPath.c_str(), "wb");
    if (!mp4File) {
        yerror("Failed to open output file: {}", outputPath);
        sock::close(fd);
        return 1;
    }

    MP4E_mux_t* mux = MP4E_open(0, 0, mp4File, mp4WriteCallback);
    if (!mux) {
        yerror("Failed to create MP4 muxer");
        fclose(mp4File);
        sock::close(fd);
        return 1;
    }

    mp4_h26x_writer_t mp4wr = {};
    // mp4_h26x_write_init will be called when we know actual encoded dimensions

    bool mp4Initialized = false;
    uint32_t frameCount = 0;
    uint64_t totalBytes = 0;
    uint32_t prevTimestampMs = 0;
    auto startTime = std::chrono::steady_clock::now();

    std::vector<uint8_t> recvBuf;

    // Main receive loop
    while (g_running) {
        // Check duration limit
        if (maxDuration > 0) {
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - startTime).count();
            if (elapsed >= maxDuration) {
                yinfo("Duration limit reached ({}s)", maxDuration);
                break;
            }
        }

        // Read frame header
        FrameHeader fh;
        if (!recvExact(fd, &fh, sizeof(fh))) {
            if (g_running) ywarn("Server disconnected");
            break;
        }

        if (fh.magic != FRAME_MAGIC) {
            ywarn("Bad frame magic 0x{:08X}", fh.magic);
            break;
        }

        ydebug("Frame {}x{} tiles={}", fh.width, fh.height, fh.num_tiles);

        if (fh.num_tiles == 0) {
            sendFrameAck(fd);
            continue;
        }

        // We only handle tile-mode frames (tile_size != 0)
        if (fh.tile_size == 0) {
            ywarn("Unexpected rectangle mode frame, skipping");
            // Would need to drain rect headers+data - just bail
            break;
        }

        for (uint16_t t = 0; t < fh.num_tiles; t++) {
            // Read tile header
            TileHeader th;
            if (!recvExact(fd, &th, sizeof(th))) {
                if (g_running) ywarn("Server disconnected during tile header");
                g_running = false;
                break;
            }

            // Read tile data
            recvBuf.resize(th.data_size);
            if (!recvExact(fd, recvBuf.data(), th.data_size)) {
                if (g_running) ywarn("Server disconnected during tile data");
                g_running = false;
                break;
            }

            auto enc = static_cast<Encoding>(th.encoding);

            if (enc == Encoding::H264) {
                if (th.data_size < sizeof(VideoFrameHeader)) {
                    ywarn("H264 tile data too small");
                    continue;
                }

                const auto* vh = reinterpret_cast<const VideoFrameHeader*>(recvBuf.data());
                const uint8_t* nalData = recvBuf.data() + sizeof(VideoFrameHeader);
                uint32_t nalSize = vh->dataSize;

                // Initialize MP4 writer on first H.264 frame
                if (!mp4Initialized) {
                    if (mp4_h26x_write_init(&mp4wr, mux, fh.width, fh.height, 0) != MP4E_STATUS_OK) {
                        yerror("Failed to initialize MP4 H.264 writer");
                        g_running = false;
                        break;
                    }
                    mp4Initialized = true;
                    yinfo("MP4 writer initialized: {}x{}", fh.width, fh.height);
                }

                // mp4_h26x_write_nal's "timeStamp90kHz_next" is actually the DURATION
                // of this sample in 90kHz units (despite the misleading parameter name)
                uint32_t durationMs = (frameCount == 0) ? 33 : (vh->timestamp - prevTimestampMs);
                if (durationMs == 0) durationMs = 33;  // fallback to ~30fps
                uint32_t duration90k = durationMs * 90;
                prevTimestampMs = vh->timestamp;

                // Write NAL data to MP4
                int err = mp4_h26x_write_nal(&mp4wr, nalData, nalSize, duration90k);
                if (err != MP4E_STATUS_OK) {
                    ywarn("mp4_h26x_write_nal failed: {}", err);
                }

                frameCount++;
                totalBytes += nalSize;

                if (frameCount % 30 == 0) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::steady_clock::now() - startTime).count();
                    if (elapsed > 0) {
                        yinfo("Recorded {} frames ({:.1f}s, {:.0f} fps, {:.1f} KB/s)",
                              frameCount, static_cast<double>(elapsed),
                              static_cast<double>(frameCount) / elapsed,
                              static_cast<double>(totalBytes) / 1024.0 / elapsed);
                    }
                }
            } else {
                ydebug("Ignoring non-H264 tile encoding={}", th.encoding);
            }
        }

        sendFrameAck(fd);
    }

    yinfo("Recording stopped. Finalizing MP4...");

    // Finalize MP4
    if (mp4Initialized) {
        mp4_h26x_write_close(&mp4wr);
    }
    if (mux) {
        MP4E_close(mux);
    }
    fclose(mp4File);

    // Disconnect
    sock::close(fd);
    sock::cleanup();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime).count();

    yinfo("Saved {} ({} frames, {:.1f}s, {:.1f} KB)",
          outputPath, frameCount,
          static_cast<double>(elapsed) / 1000.0,
          static_cast<double>(totalBytes) / 1024.0);

    return 0;
}
