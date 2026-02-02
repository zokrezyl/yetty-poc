#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace ycat {

// Yetty OSC vendor ID (matches yetty/osc-command.h)
constexpr int VENDOR_ID = 666666;

// Base94 encode binary data (2 chars per byte, printable ASCII 33-126)
std::string base94Encode(const std::vector<uint8_t>& data);
std::string base94Encode(std::string_view data);

// Build an OSC create sequence with a text payload (e.g., file path)
std::string createSequence(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    std::string_view payload,
    std::string_view pluginArgs = "");

// Build an OSC create sequence with a binary payload (base94-encoded)
std::string createSequenceBytes(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    const std::vector<uint8_t>& payloadBytes,
    std::string_view pluginArgs = "");

// Wrap an OSC sequence for tmux DCS passthrough if $TMUX is set
std::string maybeWrapForTmux(const std::string& sequence);

} // namespace ycat
