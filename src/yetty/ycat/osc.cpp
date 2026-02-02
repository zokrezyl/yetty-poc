#include "osc.h"

#include <cstdlib>
#include <sstream>

namespace ycat {

std::string base94Encode(const std::vector<uint8_t>& data) {
    std::string result;
    result.reserve(data.size() * 2);
    for (uint8_t byte : data) {
        result.push_back('!' + (byte / 94));
        result.push_back('!' + (byte % 94));
    }
    return result;
}

std::string base94Encode(std::string_view data) {
    std::string result;
    result.reserve(data.size() * 2);
    for (unsigned char byte : data) {
        result.push_back('!' + (byte / 94));
        result.push_back('!' + (byte % 94));
    }
    return result;
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
    auto encoded = base94Encode(payload);
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
    auto encoded = base94Encode(payloadBytes);
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
