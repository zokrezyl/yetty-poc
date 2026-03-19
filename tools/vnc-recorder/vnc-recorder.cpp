// VNC Recorder - Records VNC server output to MP4 file with audio
// Connects as a headless VNC client, requests H.264 encoding,
// captures audio from microphone, encodes to AAC,
// and muxes both streams into an MP4 container.

#define MINIMP4_IMPLEMENTATION
#include <minimp4.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <aacenc_lib.h>

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
#include <mutex>
#include <thread>

using namespace yetty::vnc;

namespace {

// Audio settings
constexpr uint32_t AUDIO_SAMPLE_RATE = 48000;
constexpr uint32_t AUDIO_CHANNELS = 2;
constexpr uint32_t AUDIO_BITRATE = 128000;
constexpr uint32_t AAC_FRAME_SIZE = 1024;  // samples per channel per AAC frame

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

// Audio capture state
struct AudioState {
    std::mutex mutex;
    std::vector<int16_t> buffer;
    bool enabled = false;
};

// miniaudio capture callback
void audioDataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
    (void)output;
    auto* state = static_cast<AudioState*>(device->pUserData);
    if (!state->enabled) return;

    const auto* samples = static_cast<const int16_t*>(input);
    std::lock_guard<std::mutex> lock(state->mutex);
    state->buffer.insert(state->buffer.end(), samples, samples + frameCount * AUDIO_CHANNELS);
}

// List available audio devices
void listAudioDevices() {
    ma_context context;
    if (ma_context_init(nullptr, 0, nullptr, &context) != MA_SUCCESS) {
        yerror("Failed to initialize audio context");
        return;
    }

    ma_device_info* captureDevices;
    ma_uint32 captureCount;
    ma_device_info* playbackDevices;
    ma_uint32 playbackCount;

    if (ma_context_get_devices(&context, &playbackDevices, &playbackCount, &captureDevices, &captureCount) != MA_SUCCESS) {
        yerror("Failed to enumerate audio devices");
        ma_context_uninit(&context);
        return;
    }

    std::cout << "Capture devices (" << captureCount << "):\n";
    for (ma_uint32 i = 0; i < captureCount; i++) {
        std::cout << "  [" << i << "] " << captureDevices[i].name;
        if (captureDevices[i].isDefault) {
            std::cout << " (default)";
        }
        std::cout << "\n";
    }

    ma_context_uninit(&context);
}

// Find device by index or name
bool findAudioDevice(const std::string& selector, ma_device_id* outId) {
    ma_context context;
    if (ma_context_init(nullptr, 0, nullptr, &context) != MA_SUCCESS) {
        return false;
    }

    ma_device_info* captureDevices;
    ma_uint32 captureCount;
    ma_device_info* playbackDevices;
    ma_uint32 playbackCount;

    if (ma_context_get_devices(&context, &playbackDevices, &playbackCount, &captureDevices, &captureCount) != MA_SUCCESS) {
        ma_context_uninit(&context);
        return false;
    }

    bool found = false;

    // Try as index first
    try {
        size_t idx = std::stoul(selector);
        if (idx < captureCount) {
            *outId = captureDevices[idx].id;
            found = true;
        }
    } catch (...) {
        // Try as name substring match
        for (ma_uint32 i = 0; i < captureCount; i++) {
            if (std::string(captureDevices[i].name).find(selector) != std::string::npos) {
                *outId = captureDevices[i].id;
                found = true;
                break;
            }
        }
    }

    ma_context_uninit(&context);
    return found;
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

// AAC encoder wrapper
class AacEncoder {
public:
    AacEncoder() = default;
    ~AacEncoder() { close(); }

    bool init(uint32_t sampleRate, uint32_t channels, uint32_t bitrate) {
        if (aacEncOpen(&_encoder, 0, channels) != AACENC_OK) {
            yerror("aacEncOpen failed");
            return false;
        }

        if (aacEncoder_SetParam(_encoder, AACENC_AOT, 2) != AACENC_OK ||  // AAC-LC
            aacEncoder_SetParam(_encoder, AACENC_SAMPLERATE, sampleRate) != AACENC_OK ||
            aacEncoder_SetParam(_encoder, AACENC_CHANNELMODE, channels == 2 ? MODE_2 : MODE_1) != AACENC_OK ||
            aacEncoder_SetParam(_encoder, AACENC_BITRATE, bitrate) != AACENC_OK ||
            aacEncoder_SetParam(_encoder, AACENC_TRANSMUX, TT_MP4_RAW) != AACENC_OK) {
            yerror("aacEncoder_SetParam failed");
            close();
            return false;
        }

        if (aacEncEncode(_encoder, nullptr, nullptr, nullptr, nullptr) != AACENC_OK) {
            yerror("aacEncEncode init failed");
            close();
            return false;
        }

        AACENC_InfoStruct info = {};
        if (aacEncInfo(_encoder, &info) != AACENC_OK) {
            yerror("aacEncInfo failed");
            close();
            return false;
        }

        _frameSize = info.frameLength;
        _dsi.assign(info.confBuf, info.confBuf + info.confSize);
        _channels = channels;

        yinfo("AAC encoder: {} Hz, {} ch, {} kbps, frame={}", sampleRate, channels, bitrate / 1000, _frameSize);
        return true;
    }

    void close() {
        if (_encoder) {
            aacEncClose(&_encoder);
            _encoder = nullptr;
        }
    }

    // Encode PCM samples to AAC. Returns encoded bytes or empty on error.
    std::vector<uint8_t> encode(const int16_t* pcm, size_t sampleCount) {
        std::vector<uint8_t> output(8192);

        AACENC_BufDesc inBuf = {}, outBuf = {};
        AACENC_InArgs inArgs = {};
        AACENC_OutArgs outArgs = {};

        void* inPtr = const_cast<int16_t*>(pcm);
        int inSize = static_cast<int>(sampleCount * sizeof(int16_t));
        int inElemSize = sizeof(int16_t);
        int inId = IN_AUDIO_DATA;

        inBuf.numBufs = 1;
        inBuf.bufs = &inPtr;
        inBuf.bufSizes = &inSize;
        inBuf.bufElSizes = &inElemSize;
        inBuf.bufferIdentifiers = &inId;

        void* outPtr = output.data();
        int outSize = static_cast<int>(output.size());
        int outElemSize = 1;
        int outId = OUT_BITSTREAM_DATA;

        outBuf.numBufs = 1;
        outBuf.bufs = &outPtr;
        outBuf.bufSizes = &outSize;
        outBuf.bufElSizes = &outElemSize;
        outBuf.bufferIdentifiers = &outId;

        inArgs.numInSamples = static_cast<int>(sampleCount);

        auto err = aacEncEncode(_encoder, &inBuf, &outBuf, &inArgs, &outArgs);
        if (err != AACENC_OK) {
            ywarn("aacEncEncode failed: {}", static_cast<int>(err));
            return {};
        }

        output.resize(outArgs.numOutBytes);
        return output;
    }

    uint32_t frameSize() const { return _frameSize; }
    const std::vector<uint8_t>& dsi() const { return _dsi; }

private:
    HANDLE_AACENCODER _encoder = nullptr;
    uint32_t _frameSize = 0;
    uint32_t _channels = 0;
    std::vector<uint8_t> _dsi;
};

} // anonymous namespace

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);

    args::ArgumentParser parser("vnc-recorder", "Record VNC server output to MP4 with audio");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::ValueFlag<std::string> hostFlag(parser, "host", "Server host (default localhost)", {"host"}, "localhost");
    args::ValueFlag<uint16_t> portFlag(parser, "port", "Server port (default 5900)", {'p', "port"}, 5900);
    args::ValueFlag<uint16_t> widthFlag(parser, "width", "Override recording width (default: use server size)", {'W', "width"}, 0);
    args::ValueFlag<uint16_t> heightFlag(parser, "height", "Override recording height (default: use server size)", {'H', "height"}, 0);
    args::ValueFlag<std::string> outputFlag(parser, "output", "Output MP4 file (default recording.mp4)", {'o', "output"}, "recording.mp4");
    args::ValueFlag<uint32_t> durationFlag(parser, "seconds", "Recording duration in seconds (0 = unlimited)", {'d', "duration"}, 0);
    args::Flag verboseFlag(parser, "verbose", "Enable verbose logging", {'v', "verbose"});

    // Audio flags
    args::Flag listDevicesFlag(parser, "list-devices", "List available audio capture devices and exit", {"list-audio-devices"});
    args::ValueFlag<std::string> audioDeviceFlag(parser, "device", "Audio device index or name substring (default: system default)", {'a', "audio-device"}, "");
    args::Flag noAudioFlag(parser, "no-audio", "Disable audio recording", {"no-audio"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (listDevicesFlag) {
        listAudioDevices();
        return 0;
    }

    if (verboseFlag) spdlog::set_level(spdlog::level::debug);

    std::string host = args::get(hostFlag);
    uint16_t port = args::get(portFlag);
    uint16_t recWidth = args::get(widthFlag);
    uint16_t recHeight = args::get(heightFlag);
    std::string outputPath = args::get(outputFlag);
    uint32_t maxDuration = args::get(durationFlag);
    std::string audioDeviceSelector = args::get(audioDeviceFlag);
    bool enableAudio = !noAudioFlag;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    yinfo("VNC Recorder: connecting to {}:{}", host, port);
    if (recWidth > 0 && recHeight > 0)
        yinfo("Recording {}x{} -> {}", recWidth, recHeight, outputPath);
    else
        yinfo("Recording (server size) -> {}", outputPath);

    // Initialize audio capture
    ma_device audioDevice = {};
    AudioState audioState;
    AacEncoder aacEncoder;

    if (enableAudio) {
        ma_device_config deviceConfig = ma_device_config_init(ma_device_type_capture);
        deviceConfig.capture.format = ma_format_s16;
        deviceConfig.capture.channels = AUDIO_CHANNELS;
        deviceConfig.sampleRate = AUDIO_SAMPLE_RATE;
        deviceConfig.dataCallback = audioDataCallback;
        deviceConfig.pUserData = &audioState;

        // Select specific device if requested
        ma_device_id selectedDeviceId;
        if (!audioDeviceSelector.empty()) {
            if (findAudioDevice(audioDeviceSelector, &selectedDeviceId)) {
                deviceConfig.capture.pDeviceID = &selectedDeviceId;
            } else {
                yerror("Audio device not found: {}", audioDeviceSelector);
                return 1;
            }
        }

        if (ma_device_init(nullptr, &deviceConfig, &audioDevice) != MA_SUCCESS) {
            yerror("Failed to initialize audio device");
            return 1;
        }

        if (!aacEncoder.init(AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_BITRATE)) {
            ma_device_uninit(&audioDevice);
            return 1;
        }

        yinfo("Audio: {} @ {} Hz, {} channels", audioDevice.capture.name, AUDIO_SAMPLE_RATE, AUDIO_CHANNELS);
    }

    // Connect to server
    sock::init();

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        yerror("Failed to create socket");
        if (enableAudio) ma_device_uninit(&audioDevice);
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
        if (enableAudio) ma_device_uninit(&audioDevice);
        return 1;
    }
    addr.sin_addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr)->sin_addr;
    freeaddrinfo(res);

    if (::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
        yerror("Failed to connect to {}:{}", host, port);
        sock::close(fd);
        if (enableAudio) ma_device_uninit(&audioDevice);
        return 1;
    }

    yinfo("Connected to {}:{}", host, port);

    // Send resize: explicit dimensions or 0xFFFF = "use current server size"
    if (recWidth > 0 && recHeight > 0) {
        sendResize(fd, recWidth, recHeight);
    } else {
        sendResize(fd, 0xFFFF, 0xFFFF);
    }
    sendCompressionConfig(fd, CODEC_H264);

    // Open MP4 output
    FILE* mp4File = fopen(outputPath.c_str(), "wb");
    if (!mp4File) {
        yerror("Failed to open output file: {}", outputPath);
        sock::close(fd);
        if (enableAudio) ma_device_uninit(&audioDevice);
        return 1;
    }

    MP4E_mux_t* mux = MP4E_open(0, 0, mp4File, mp4WriteCallback);
    if (!mux) {
        yerror("Failed to create MP4 muxer");
        fclose(mp4File);
        sock::close(fd);
        if (enableAudio) ma_device_uninit(&audioDevice);
        return 1;
    }

    mp4_h26x_writer_t mp4wr = {};
    int audioTrackId = -1;

    // Add audio track if enabled
    if (enableAudio) {
        MP4E_track_t audioTrack = {};
        audioTrack.track_media_kind = e_audio;
        audioTrack.language[0] = 'u';
        audioTrack.language[1] = 'n';
        audioTrack.language[2] = 'd';
        audioTrack.object_type_indication = MP4_OBJECT_TYPE_AUDIO_ISO_IEC_14496_3;  // AAC
        audioTrack.time_scale = AUDIO_SAMPLE_RATE;
        audioTrack.default_duration = AAC_FRAME_SIZE;
        audioTrack.u.a.channelcount = AUDIO_CHANNELS;

        audioTrackId = MP4E_add_track(mux, &audioTrack);
        if (audioTrackId < 0) {
            yerror("Failed to add audio track");
        } else {
            // Set decoder specific info (AudioSpecificConfig)
            const auto& dsi = aacEncoder.dsi();
            MP4E_set_dsi(mux, audioTrackId, dsi.data(), static_cast<int>(dsi.size()));
        }
    }

    bool mp4Initialized = false;
    uint32_t frameCount = 0;
    uint64_t totalVideoBytes = 0;
    uint64_t totalAudioBytes = 0;
    uint32_t audioFrameCount = 0;
    uint32_t prevTimestampMs = 0;
    auto startTime = std::chrono::steady_clock::now();

    std::vector<uint8_t> recvBuf;
    std::vector<int16_t> audioEncodeBuffer;

    // Start audio capture
    if (enableAudio && audioTrackId >= 0) {
        audioState.enabled = true;
        if (ma_device_start(&audioDevice) != MA_SUCCESS) {
            yerror("Failed to start audio capture");
            audioState.enabled = false;
        }
    }

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

        // Process accumulated audio
        if (audioState.enabled && audioTrackId >= 0) {
            std::vector<int16_t> samples;
            {
                std::lock_guard<std::mutex> lock(audioState.mutex);
                samples.swap(audioState.buffer);
            }

            audioEncodeBuffer.insert(audioEncodeBuffer.end(), samples.begin(), samples.end());

            // Encode complete AAC frames
            const size_t samplesPerFrame = AAC_FRAME_SIZE * AUDIO_CHANNELS;
            while (audioEncodeBuffer.size() >= samplesPerFrame) {
                auto aacData = aacEncoder.encode(audioEncodeBuffer.data(), samplesPerFrame);
                if (!aacData.empty()) {
                    // Duration in timescale units (sample rate)
                    MP4E_put_sample(mux, audioTrackId, aacData.data(), static_cast<int>(aacData.size()),
                                    AAC_FRAME_SIZE, MP4E_SAMPLE_DEFAULT);
                    totalAudioBytes += aacData.size();
                    audioFrameCount++;
                }
                audioEncodeBuffer.erase(audioEncodeBuffer.begin(), audioEncodeBuffer.begin() + samplesPerFrame);
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
                totalVideoBytes += nalSize;

                if (frameCount % 30 == 0) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::steady_clock::now() - startTime).count();
                    if (elapsed > 0) {
                        yinfo("Recorded {} video frames, {} audio frames ({:.1f}s, {:.0f} fps, V:{:.1f} KB/s, A:{:.1f} KB/s)",
                              frameCount, audioFrameCount, static_cast<double>(elapsed),
                              static_cast<double>(frameCount) / elapsed,
                              static_cast<double>(totalVideoBytes) / 1024.0 / elapsed,
                              static_cast<double>(totalAudioBytes) / 1024.0 / elapsed);
                    }
                }
            } else {
                ydebug("Ignoring non-H264 tile encoding={}", th.encoding);
            }
        }

        sendFrameAck(fd);
    }

    yinfo("Recording stopped. Finalizing MP4...");

    // Stop audio capture
    if (audioState.enabled) {
        audioState.enabled = false;
        ma_device_stop(&audioDevice);
    }

    // Finalize MP4
    if (mp4Initialized) {
        mp4_h26x_write_close(&mp4wr);
    }
    if (mux) {
        MP4E_close(mux);
    }
    fclose(mp4File);

    // Cleanup
    if (enableAudio) {
        ma_device_uninit(&audioDevice);
    }
    sock::close(fd);
    sock::cleanup();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime).count();

    yinfo("Saved {} ({} video frames, {} audio frames, {:.1f}s, {:.1f} KB video, {:.1f} KB audio)",
          outputPath, frameCount, audioFrameCount,
          static_cast<double>(elapsed) / 1000.0,
          static_cast<double>(totalVideoBytes) / 1024.0,
          static_cast<double>(totalAudioBytes) / 1024.0);

    return 0;
}
