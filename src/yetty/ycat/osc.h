#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace ycat {

// Yetty OSC vendor ID (matches yetty/osc-command.h)
constexpr int VENDOR_ID = 666666;

// Base64 encode binary data
std::string base64Encode(const std::vector<uint8_t>& data);
std::string base64Encode(std::string_view data);

// Build an OSC create sequence with a text payload (e.g., file path)
std::string createSequence(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    std::string_view payload,
    std::string_view pluginArgs = "");

// Build an OSC create sequence with a binary payload (base64-encoded)
std::string createSequenceBytes(
    std::string_view plugin,
    int x, int y, int w, int h,
    bool relative,
    const std::vector<uint8_t>& payloadBytes,
    std::string_view pluginArgs = "");

// Build an OSC help sequence for a card
std::string createHelpSequence(std::string_view card);

// Wrap an OSC sequence for tmux DCS passthrough if $TMUX is set
std::string maybeWrapForTmux(const std::string& sequence);

} // namespace ycat
