/**
 * yplayer - Video player for yetty terminal
 *
 * Decodes video files and sends frames as OSC image sequences to the terminal.
 * Uses the yvideo decoder for H.264/AV1 decoding.
 *
 * Usage: yplayer <video.mp4> [-w width] [-h height] [-l loop]
 */

#include "yetty/yvideo/yvideo-decoder.h"
#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <args.hxx>

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstring>
#include <cstdint>

// Base64 encoding table
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string ret;
    ret.reserve(((len + 2) / 3) * 4);
    
    for (size_t i = 0; i < len; i += 3) {
        uint32_t n = static_cast<uint32_t>(data[i]) << 16;
        if (i + 1 < len) n |= static_cast<uint32_t>(data[i + 1]) << 8;
        if (i + 2 < len) n |= static_cast<uint32_t>(data[i + 2]);
        
        ret += base64_chars[(n >> 18) & 0x3F];
        ret += base64_chars[(n >> 12) & 0x3F];
        ret += (i + 1 < len) ? base64_chars[(n >> 6) & 0x3F] : '=';
        ret += (i + 2 < len) ? base64_chars[n & 0x3F] : '=';
    }
    return ret;
}

// Simple TGA image encoder (uncompressed, easy to implement)
std::vector<uint8_t> encode_tga(const uint8_t* rgb, uint32_t width, uint32_t height) {
    std::vector<uint8_t> tga;
    tga.reserve(18 + width * height * 3);
    
    // TGA header (18 bytes)
    tga.push_back(0);           // ID length
    tga.push_back(0);           // Color map type
    tga.push_back(2);           // Image type: uncompressed RGB
    tga.push_back(0); tga.push_back(0);  // Color map origin
    tga.push_back(0); tga.push_back(0);  // Color map length
    tga.push_back(0);           // Color map depth
    tga.push_back(0); tga.push_back(0);  // X origin
    tga.push_back(0); tga.push_back(0);  // Y origin
    tga.push_back(width & 0xFF);
    tga.push_back((width >> 8) & 0xFF);
    tga.push_back(height & 0xFF);
    tga.push_back((height >> 8) & 0xFF);
    tga.push_back(24);          // Bits per pixel
    tga.push_back(0x20);        // Image descriptor (top-left origin)
    
    // Pixel data (BGR format for TGA)
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            size_t idx = (y * width + x) * 3;
            tga.push_back(rgb[idx + 2]);  // B
            tga.push_back(rgb[idx + 1]);  // G
            tga.push_back(rgb[idx + 0]);  // R
        }
    }
    
    return tga;
}

// YUV to RGB conversion (BT.709)
void yuv_to_rgb(const yetty::yvideo::YUVFrame& frame, std::vector<uint8_t>& rgb) {
    uint32_t w = frame.width;
    uint32_t h = frame.height;
    rgb.resize(w * h * 3);
    
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; ++x) {
            // Get Y value
            uint8_t Y = frame.yPlane[y * frame.yStride + x];
            
            // Get UV values (half resolution)
            uint32_t uvX = x / 2;
            uint32_t uvY = y / 2;
            uint8_t U = frame.uPlane[uvY * frame.uStride + uvX];
            uint8_t V = frame.vPlane[uvY * frame.vStride + uvX];
            
            // BT.709 conversion
            float fY = static_cast<float>(Y);
            float fU = static_cast<float>(U) - 128.0f;
            float fV = static_cast<float>(V) - 128.0f;
            
            float R = fY + 1.5748f * fV;
            float G = fY - 0.1873f * fU - 0.4681f * fV;
            float B = fY + 1.8556f * fU;
            
            // Clamp to [0, 255]
            size_t idx = (y * w + x) * 3;
            rgb[idx + 0] = static_cast<uint8_t>(std::clamp(R, 0.0f, 255.0f));
            rgb[idx + 1] = static_cast<uint8_t>(std::clamp(G, 0.0f, 255.0f));
            rgb[idx + 2] = static_cast<uint8_t>(std::clamp(B, 0.0f, 255.0f));
        }
    }
}

// Card name for updates
static const char* CARD_NAME = "yplayer-video";

void send_frame_osc(const std::vector<uint8_t>& tga, int x, int y, int w, int h) {
    std::string payload = base64_encode(tga.data(), tga.size());
    
    ytrace("Sending update frame, payload size: {}", payload.size());
    
    // OSC sequence: \033]666666;update --name NAME;-i -;BASE64\033\\
    std::cout << "\033]666666;update --name " << CARD_NAME << ";-i -;" << payload << "\033\\";
    std::cout.flush();
    
    ytrace("Update frame sent");
}

void run_card_osc(int x, int y, int w, int h, const std::vector<uint8_t>& tga) {
    std::string payload = base64_encode(tga.data(), tga.size());
    
    // OSC sequence to create image card with initial image
    // Format: run -c image --name NAME -x X -y Y -w W -h H -r;-i -;PAYLOAD
    std::cout << "\033]666666;run -c image --name " << CARD_NAME 
              << " -x " << x << " -y " << y 
              << " -w " << w << " -h " << h << " -r;-i -;" << payload << "\033\\";
    std::cout.flush();
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("yplayer - Video player for yetty terminal");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
    args::Positional<std::string> videoFile(parser, "video", "Video file to play");
    args::ValueFlag<int> widthArg(parser, "width", "Display width in cells", {'w', "width"}, 40);
    args::ValueFlag<int> heightArg(parser, "height", "Display height in cells", {'H', "height"}, 20);
    args::ValueFlag<int> xArg(parser, "x", "X position", {'x'}, 0);
    args::ValueFlag<int> yArg(parser, "y", "Y position", {'y'}, 0);
    args::Flag loopArg(parser, "loop", "Loop playback", {'l', "loop"});
    args::ValueFlag<std::string> ytraceFileArg(parser, "file", "Write ytrace output to file", {"ytrace-file"});
    
    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    
    if (!videoFile) {
        std::cerr << "Error: Video file required\n";
        std::cerr << parser;
        return 1;
    }
    
    std::string path = args::get(videoFile);
    int width = args::get(widthArg);
    int height = args::get(heightArg);
    int x = args::get(xArg);
    int y = args::get(yArg);
    bool loop = args::get(loopArg);
    
    // Setup spdlog file output if specified
    if (ytraceFileArg) {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(args::get(ytraceFileArg), true);
        auto logger = std::make_shared<spdlog::logger>("yplayer", file_sink);
        logger->set_level(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    }
    
    // Read video file
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file: " << path << std::endl;
        return 1;
    }
    
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
    file.close();
    
    ytrace("Loaded {} bytes from {}", data.size(), path);
    
    // Create video source
    auto sourceResult = yetty::yvideo::VideoSource::create();
    if (!sourceResult) {
        std::cerr << "Error: Failed to create video source" << std::endl;
        return 1;
    }
    auto source = *sourceResult;
    
    // Open video
    if (auto res = source->open(std::span<const uint8_t>(data)); !res) {
        std::cerr << "Error: Failed to open video" << std::endl;
        return 1;
    }
    
    ytrace("Video: {}x{} @ {} fps, {} frames", 
           source->width(), source->height(), source->frameRate(), source->frameCount());
    
    // Calculate frame interval
    auto frameInterval = std::chrono::microseconds(
        static_cast<int64_t>(1000000.0 / source->frameRate())
    );
    
    std::vector<uint8_t> rgb;
    uint32_t frameNum = 0;
    bool decoderWarmedUp = false;
    bool cardCreated = false;
    
    do {
        auto frameStart = std::chrono::steady_clock::now();
        
        // Get next frame
        auto frameResult = source->nextFrame();
        if (!frameResult) {
            std::cerr << "Error: Failed to get frame" << std::endl;
            break;
        }
        
        auto frameOpt = *frameResult;
        if (!frameOpt) {
            if (!decoderWarmedUp) {
                // H.264 decoder may need multiple samples before first frame
                // Keep trying until we get a frame or exhaust samples
                continue;
            }
            // End of video
            if (loop) {
                source->seekFrame(0);
                frameNum = 0;
                decoderWarmedUp = false;
                continue;
            }
            break;
        }
        
        decoderWarmedUp = true;
        auto& frame = *frameOpt;
        if (!frame.isValid()) {
            source->releaseFrame();
            continue;
        }
        
        // Convert YUV to RGB
        yuv_to_rgb(frame, rgb);
        source->releaseFrame();
        
        // Encode as TGA
        auto tga = encode_tga(rgb.data(), frame.width, frame.height);
        
        // Send OSC sequence - create card with first frame, update for rest
        if (!cardCreated) {
            run_card_osc(x, y, width, height, tga);
            cardCreated = true;
        } else {
            send_frame_osc(tga, x, y, width, height);
        }
        
        ++frameNum;
        if (frameNum % 30 == 0) {
            ytrace("Frame {}", frameNum);
        }
        
        // Wait for next frame time
        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        if (elapsed < frameInterval) {
            std::this_thread::sleep_for(frameInterval - elapsed);
        }
        
    } while (true);
    
    ytrace("Playback complete: {} frames", frameNum);
    
    return 0;
}
