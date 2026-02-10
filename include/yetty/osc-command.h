#pragma once

#include <yetty/result.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <optional>

namespace yetty {

// Vendor ID for yetty card OSC sequences
constexpr int YETTY_OSC_VENDOR_ID = 666666;

//-----------------------------------------------------------------------------
// OscCommand - parsed OSC command
//-----------------------------------------------------------------------------
enum class OscCommandType {
    Run,        // run -x 0 -y 0 -w 100 -h 100 -c image (creates + renders card)
    List,       // ls [--all]
    Cards,      // cards (list available card types)
    Kill,       // kill --id <id> | --card <name>
    Stop,       // stop --id <id> | --card <name>
    Start,      // start --id <id> | --card <name>
    Update,     // update --id <id>
    Help,       // help -c <card>
    Unknown
};

struct RunArgs {
    int32_t x = 0;
    int32_t y = 0;
    int32_t width = 0;      // 0 = stretch to edge
    int32_t height = 0;     // 0 = stretch to edge
    std::string card;
    std::string name;       // User-assigned card name for RPC lookup
    bool relative = false;  // Position relative to cursor
};

struct ListArgs {
    bool all = false;       // Include stopped cards
};

struct TargetArgs {
    std::string id;         // Target specific card by ID
    std::string card;       // Target all cards of a type
};

struct HelpArgs {
    std::string card;       // Card type to get help for
};

struct OscCommand {
    OscCommandType type = OscCommandType::Unknown;

    // Command-specific args (use appropriate one based on type)
    RunArgs run;
    ListArgs list;
    TargetArgs target;      // For kill, stop, start, update
    HelpArgs help;

    // Card-specific args (raw string, passed to card)
    std::string cardArgs;

    // Payload (base64 decoded)
    std::string payload;

    // Error message if parsing failed
    std::string error;

    bool isValid() const { return type != OscCommandType::Unknown && error.empty(); }
};

//-----------------------------------------------------------------------------
// OscCommandParser - parses OSC sequences into commands
//-----------------------------------------------------------------------------
class OscCommandParser {
public:
    OscCommandParser();
    ~OscCommandParser() = default;

    // Parse an OSC sequence (without the OSC prefix and ST suffix)
    // Format: <vendor-id>;<generic-args>;<plugin-args>;<payload>
    Result<OscCommand> parse(const std::string& sequence);

    // Generate a unique 8-character hash ID
    std::string generateId();

    // Base64 encoding/decoding
    static std::string base64Encode(const std::string& data);
    static std::string base64Decode(const std::string& encoded);

private:
    // Tokenize a string respecting quoted strings
    // "hello world" --foo bar -> ["hello world", "--foo", "bar"]
    std::vector<std::string> tokenize(const std::string& str);

    // Parse generic args into command
    Result<OscCommand> parseGenericArgs(const std::vector<std::string>& tokens);

    // Parse run command args
    Result<RunArgs> parseRunArgs(const std::vector<std::string>& tokens);

    // Parse list command args
    Result<ListArgs> parseListArgs(const std::vector<std::string>& tokens);

    // Parse target args (for kill, stop, start, update)
    Result<TargetArgs> parseTargetArgs(const std::vector<std::string>& tokens);

    // Parse help command args
    Result<HelpArgs> parseHelpArgs(const std::vector<std::string>& tokens);

    // Split sequence by semicolon into fields
    std::vector<std::string> splitFields(const std::string& sequence);

    // ID generation
    std::mt19937 rng_;
    static constexpr const char* ID_CHARS = "abcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr size_t ID_LENGTH = 8;
};

//-----------------------------------------------------------------------------
// OscResponse - helper for building OSC responses
//-----------------------------------------------------------------------------
class OscResponse {
public:
    // Build success response
    static std::string success(const std::string& message = "");

    // Build error response
    static std::string error(const std::string& message);

    // Build list response (cards)
    static std::string cardList(const std::vector<std::tuple<std::string, std::string, int, int, int, int, bool>>& cards);

    // Build plugins list response
    static std::string pluginList(const std::vector<std::string>& plugins);
};

} // namespace yetty
