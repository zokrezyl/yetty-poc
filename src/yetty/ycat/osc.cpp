#include "osc.h"

#include <cstdlib>
#include <sstream>

namespace ycat {

static constexpr char BASE64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const std::vector<uint8_t>& data) {
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);

    size_t i = 0;
    const size_t len = data.size();

    while (i + 2 < len) {
        uint32_t triple = (static_cast<uint32_t>(data[i]) << 16) |
                          (static_cast<uint32_t>(data[i + 1]) << 8) |
                          static_cast<uint32_t>(data[i + 2]);
        result.push_back(BASE64_CHARS[(triple >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 6) & 0x3F]);
        result.push_back(BASE64_CHARS[triple & 0x3F]);
        i += 3;
    }

    if (i + 1 == len) {
        uint32_t val = static_cast<uint32_t>(data[i]) << 16;
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back('=');
        result.push_back('=');
    } else if (i + 2 == len) {
        uint32_t val = (static_cast<uint32_t>(data[i]) << 16) |
                       (static_cast<uint32_t>(data[i + 1]) << 8);
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 6) & 0x3F]);
        result.push_back('=');
    }

    return result;
}

std::string base64Encode(std::string_view data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    return base64Encode(bytes);
}

static std::string buildCreateArgs(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative)
{
    std::ostringstream ss;
    ss << "create -p " << plugin
       << " -x " << x << " -y " << y
       << " -w " << w << " -h " << h;
    if (relative) ss << " -r";
    return ss.str();
}

std::string createSequence(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    std::string_view payload,
    std::string_view pluginArgs)
{
    auto args = buildCreateArgs(plugin, x, y, w, h, relative);
    auto encoded = base64Encode(payload);
    std::string seq;
    seq.reserve(10 + args.size() + pluginArgs.size() + encoded.size());
    seq += "\033]";
    seq += std::to_string(VENDOR_ID);
    seq += ';';
    seq += args;
    seq += ';';
    seq += pluginArgs;
    seq += ';';
    seq += encoded;
    seq += "\033\\";
    return seq;
}

std::string createSequenceBytes(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    const std::vector<uint8_t>& payloadBytes,
    std::string_view pluginArgs)
{
    auto args = buildCreateArgs(plugin, x, y, w, h, relative);
    auto encoded = base64Encode(payloadBytes);
    std::string seq;
    seq.reserve(10 + args.size() + pluginArgs.size() + encoded.size());
    seq += "\033]";
    seq += std::to_string(VENDOR_ID);
    seq += ';';
    seq += args;
    seq += ';';
    seq += pluginArgs;
    seq += ';';
    seq += encoded;
    seq += "\033\\";
    return seq;
}

static std::string wrapForTmux(const std::string& sequence) {
    std::string escaped;
    escaped.reserve(sequence.size() * 2);
    for (char c : sequence) {
        if (c == '\033') {
            escaped += "\033\033";
        } else {
            escaped += c;
        }
    }
    return std::string("\033Ptmux;") + escaped + "\033\\";
}

std::string maybeWrapForTmux(const std::string& sequence) {
    if (std::getenv("TMUX")) {
        return wrapForTmux(sequence);
    }
    return sequence;
}

} // namespace ycat
