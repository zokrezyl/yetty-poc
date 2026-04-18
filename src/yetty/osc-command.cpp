#include <yetty/osc-command.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <chrono>
#include <cstring>

namespace yetty {

//-----------------------------------------------------------------------------
// OscCommandParser
//-----------------------------------------------------------------------------

OscCommandParser::OscCommandParser() {
    // Seed RNG with high-resolution clock
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng_.seed(static_cast<uint32_t>(seed));
}

std::string OscCommandParser::generateId() {
    std::string id;
    id.reserve(ID_LENGTH);

    std::uniform_int_distribution<size_t> dist(0, 35);  // 26 letters + 10 digits
    for (size_t i = 0; i < ID_LENGTH; ++i) {
        id += ID_CHARS[dist(rng_)];
    }

    return id;
}

std::vector<std::string> OscCommandParser::splitFields(const std::string& sequence) {
    // Split into exactly 4 fields: vendor-id;generic-args;plugin-args;payload
    // Only split on first 3 semicolons - payload may contain semicolons
    // Respect quoted strings and braces (semicolons inside don't count as separators)
    std::vector<std::string> fields;
    size_t start = 0;
    bool inQuotes = false;
    int braceDepth = 0;
    int fieldCount = 0;

    for (size_t i = 0; i < sequence.size() && fieldCount < 3; ++i) {
        char c = sequence[i];
        if (c == '"' || c == '\'') {
            inQuotes = !inQuotes;
        } else if (!inQuotes) {
            if (c == '{') {
                braceDepth++;
            } else if (c == '}') {
                braceDepth--;
            } else if (c == ';' && braceDepth == 0) {
                fields.push_back(sequence.substr(start, i - start));
                start = i + 1;
                fieldCount++;
            }
        }
    }

    // Remaining is payload (may contain semicolons)
    if (start < sequence.size()) {
        fields.push_back(sequence.substr(start));
    }
    return fields;
}

std::vector<std::string> OscCommandParser::tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];

        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

Result<OscCommand> OscCommandParser::parse(const std::string& sequence) {
    OscCommand cmd;

    // Split into fields: vendor-id;generic-args;plugin-args;payload
    auto fields = splitFields(sequence);

    if (fields.size() < 2) {
        cmd.error = "invalid sequence: expected at least vendor-id and command";
        return Ok(cmd);
    }

    // Check vendor ID
    int vendorId = 0;
    try {
        vendorId = std::stoi(fields[0]);
    } catch (...) {
        cmd.error = "invalid vendor ID";
        return Ok(cmd);
    }

    if (vendorId != YETTY_OSC_VENDOR_ID) {
        cmd.error = "unknown vendor ID: " + std::to_string(vendorId);
        return Ok(cmd);
    }

    // Parse generic args (field 1)
    auto tokens = tokenize(fields[1]);
    if (tokens.empty()) {
        cmd.error = "empty command";
        return Ok(cmd);
    }

    auto parseResult = parseGenericArgs(tokens);
    if (!parseResult) {
        cmd.error = error_msg(parseResult);
        return Ok(cmd);
    }
    cmd = *parseResult;

    // Card args (field 2, optional)
    if (fields.size() > 2) {
        cmd.cardArgs = fields[2];
    }

    // Payload (field 3, optional, base64 encoded)
    if (fields.size() > 3 && !fields[3].empty()) {
        cmd.payload = base64Decode(fields[3]);
    }

    return Ok(cmd);
}

Result<OscCommand> OscCommandParser::parseGenericArgs(const std::vector<std::string>& tokens) {
    OscCommand cmd;

    if (tokens.empty()) {
        return Err<OscCommand>("empty command");
    }

    const std::string& command = tokens[0];

    if (command == "run" || command == "create") {
        // "create" is accepted for backwards compatibility but "run" is preferred
        cmd.type = OscCommandType::Run;
        auto result = parseRunArgs(tokens);
        if (!result) {
            return Err<OscCommand>("run: " + error_msg(result));
        }
        cmd.run = *result;
    }
    else if (command == "ls" || command == "list") {
        cmd.type = OscCommandType::List;
        auto result = parseListArgs(tokens);
        if (!result) {
            return Err<OscCommand>("ls: " + error_msg(result));
        }
        cmd.list = *result;
    }
    else if (command == "cards" || command == "plugins") {
        // "plugins" accepted for backwards compatibility
        cmd.type = OscCommandType::Cards;
    }
    else if (command == "kill") {
        cmd.type = OscCommandType::Kill;
        auto result = parseTargetArgs(tokens);
        if (!result) {
            return Err<OscCommand>("kill: " + error_msg(result));
        }
        cmd.target = *result;
    }
    else if (command == "stop") {
        cmd.type = OscCommandType::Stop;
        auto result = parseTargetArgs(tokens);
        if (!result) {
            return Err<OscCommand>("stop: " + error_msg(result));
        }
        cmd.target = *result;
    }
    else if (command == "start") {
        cmd.type = OscCommandType::Start;
        auto result = parseTargetArgs(tokens);
        if (!result) {
            return Err<OscCommand>("start: " + error_msg(result));
        }
        cmd.target = *result;
    }
    else if (command == "update") {
        cmd.type = OscCommandType::Update;
        auto result = parseTargetArgs(tokens);
        if (!result) {
            return Err<OscCommand>("update: " + error_msg(result));
        }
        cmd.target = *result;
    }
    else if (command == "help") {
        cmd.type = OscCommandType::Help;
        auto result = parseHelpArgs(tokens);
        if (!result) {
            return Err<OscCommand>("help: " + error_msg(result));
        }
        cmd.help = *result;
    }
    else {
        return Err<OscCommand>("unknown command: " + command);
    }

    return Ok(cmd);
}

Result<RunArgs> OscCommandParser::parseRunArgs(const std::vector<std::string>& tokens) {
    RunArgs args;

    for (size_t i = 1; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token == "-x" || token == "--x-pos") {
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.x = std::stoi(tokens[i]);
        }
        else if (token == "-y" || token == "--y-pos") {
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.y = std::stoi(tokens[i]);
        }
        else if (token == "-w" || token == "--width") {
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.width = std::stoi(tokens[i]);
        }
        else if (token == "-h" || token == "--height") {
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.height = std::stoi(tokens[i]);
        }
        else if (token == "-c" || token == "--card" || token == "-p" || token == "--plugin") {
            // -p/--plugin accepted for backwards compatibility
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.card = tokens[i];
        }
        else if (token == "-r" || token == "--relative") {
            args.relative = true;
        }
        else if (token == "-n" || token == "--name") {
            if (++i >= tokens.size()) return Err<RunArgs>("missing value for " + token);
            args.name = tokens[i];
        }
        else {
            return Err<RunArgs>("unknown option: " + token);
        }
    }

    if (args.card.empty()) {
        return Err<RunArgs>("--card/-c is required");
    }

    return Ok(args);
}

Result<ListArgs> OscCommandParser::parseListArgs(const std::vector<std::string>& tokens) {
    ListArgs args;

    for (size_t i = 1; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token == "-a" || token == "--all") {
            args.all = true;
        }
        else {
            return Err<ListArgs>("unknown option: " + token);
        }
    }

    return Ok(args);
}

Result<TargetArgs> OscCommandParser::parseTargetArgs(const std::vector<std::string>& tokens) {
    TargetArgs args;

    for (size_t i = 1; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token == "--id") {
            if (++i >= tokens.size()) return Err<TargetArgs>("missing value for --id");
            args.id = tokens[i];
        }
        else if (token == "--name" || token == "-n") {
            // --name targets card by user-assigned name (stored in args.id for lookup)
            if (++i >= tokens.size()) return Err<TargetArgs>("missing value for --name");
            args.id = tokens[i];
        }
        else if (token == "--card" || token == "-c" || token == "--plugin" || token == "-p") {
            // --plugin/-p accepted for backwards compatibility
            if (++i >= tokens.size()) return Err<TargetArgs>("missing value for --card");
            args.card = tokens[i];
        }
        else {
            return Err<TargetArgs>("unknown option: " + token);
        }
    }

    if (args.id.empty() && args.card.empty()) {
        return Err<TargetArgs>("--id, --name, or --card is required");
    }

    return Ok(args);
}

Result<HelpArgs> OscCommandParser::parseHelpArgs(const std::vector<std::string>& tokens) {
    HelpArgs args;

    for (size_t i = 1; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        if (token == "--card" || token == "-c" || token == "--plugin" || token == "-p") {
            // --plugin/-p accepted for backwards compatibility
            if (++i >= tokens.size()) return Err<HelpArgs>("missing value for --card");
            args.card = tokens[i];
        }
        else {
            return Err<HelpArgs>("unknown option: " + token);
        }
    }

    if (args.card.empty()) {
        return Err<HelpArgs>("--card/-c is required");
    }

    return Ok(args);
}

//-----------------------------------------------------------------------------
// Base64 encoding/decoding
//-----------------------------------------------------------------------------

static constexpr char BASE64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static constexpr unsigned char BASE64_DECODE_TABLE[128] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
     52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255, 255, 255, 255,
    255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255
};

std::string OscCommandParser::base64Encode(const std::string& data) {
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);

    size_t i = 0;
    const size_t len = data.size();

    while (i + 2 < len) {
        uint32_t triple = (static_cast<unsigned char>(data[i]) << 16) |
                          (static_cast<unsigned char>(data[i + 1]) << 8) |
                          static_cast<unsigned char>(data[i + 2]);
        result.push_back(BASE64_CHARS[(triple >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 6) & 0x3F]);
        result.push_back(BASE64_CHARS[triple & 0x3F]);
        i += 3;
    }

    if (i + 1 == len) {
        uint32_t val = static_cast<unsigned char>(data[i]) << 16;
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back('=');
        result.push_back('=');
    } else if (i + 2 == len) {
        uint32_t val = (static_cast<unsigned char>(data[i]) << 16) |
                       (static_cast<unsigned char>(data[i + 1]) << 8);
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 6) & 0x3F]);
        result.push_back('=');
    }

    return result;
}

std::string OscCommandParser::base64Decode(const std::string& encoded) {
    if (encoded.empty()) return "";

    // Find actual length (excluding padding)
    size_t len = encoded.size();
    while (len > 0 && encoded[len - 1] == '=') {
        --len;
    }

    std::string result;
    result.reserve((len * 3) / 4);

    uint32_t buffer = 0;
    int bits = 0;

    for (size_t i = 0; i < len; ++i) {
        unsigned char c = encoded[i];
        if (c >= 128 || BASE64_DECODE_TABLE[c] == 255) {
            continue;  // Skip invalid characters
        }
        buffer = (buffer << 6) | BASE64_DECODE_TABLE[c];
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            result.push_back(static_cast<char>((buffer >> bits) & 0xFF));
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// OscResponse
//-----------------------------------------------------------------------------

std::string OscResponse::success(const std::string& message) {
    if (message.empty()) {
        return "";  // Silent success
    }
    return message + "\n";
}

std::string OscResponse::error(const std::string& message) {
    return "error: " + message + "\n";
}

std::string OscResponse::cardList(const std::vector<std::tuple<std::string, std::string, int, int, int, int, bool>>& cards) {
    if (cards.empty()) {
        return "no cards\n";
    }

    std::string result;
    result += "ID        PLUGIN          X     Y     W     H  STATE\n";
    result += "--------  --------------  ----  ----  ----  ----  -------\n";

    for (const auto& [id, plugin, x, y, w, h, running] : cards) {
        char line[128];
        snprintf(line, sizeof(line), "%-8s  %-14s  %4d  %4d  %4d  %4d  %s\n",
                 id.c_str(), plugin.c_str(), x, y, w, h,
                 running ? "running" : "stopped");
        result += line;
    }

    return result;
}

std::string OscResponse::pluginList(const std::vector<std::string>& plugins) {
    if (plugins.empty()) {
        return "no plugins available\n";
    }

    std::string result = "available plugins:\n";
    for (const auto& name : plugins) {
        result += "  " + name + "\n";
    }
    return result;
}

} // namespace yetty
