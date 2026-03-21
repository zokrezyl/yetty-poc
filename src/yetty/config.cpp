#include "yetty/config.h"
#include <ytrace/ytrace.hpp>
#include <args.hxx>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cstdlib>

#ifdef __linux__
#include <unistd.h>
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace yetty {

using namespace ygui;

// ─── ConfigNode: Custom tree structure ───────────────────────────────────────

struct ConfigNode {
    Dict values;                                  // leaf values (scalars, lists)
    std::map<std::string, ConfigNode> children;   // child nodes

    bool empty() const { return values.empty() && children.empty(); }
};

// ─── Helpers ─────────────────────────────────────────────────────────────────

// Split a slash-separated path into components
static std::vector<std::string> splitPath(const std::string& path) {
    std::vector<std::string> parts;
    std::istringstream ss(path);
    std::string part;
    while (std::getline(ss, part, '/')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    return parts;
}

// Navigate to a node by path (returns nullptr if not found)
static ConfigNode* navigateNode(ConfigNode& root, const std::vector<std::string>& parts) {
    ConfigNode* current = &root;
    for (const auto& p : parts) {
        auto it = current->children.find(p);
        if (it == current->children.end()) return nullptr;
        current = &it->second;
    }
    return current;
}

static const ConfigNode* navigateNode(const ConfigNode& root, const std::vector<std::string>& parts) {
    const ConfigNode* current = &root;
    for (const auto& p : parts) {
        auto it = current->children.find(p);
        if (it == current->children.end()) return nullptr;
        current = &it->second;
    }
    return current;
}

// Navigate to a node, creating intermediate nodes as needed
static ConfigNode* navigateOrCreate(ConfigNode& root, const std::vector<std::string>& parts) {
    ConfigNode* current = &root;
    for (const auto& p : parts) {
        current = &current->children[p];  // creates if doesn't exist
    }
    return current;
}

// Convert a YAML node to ConfigNode (recursive)
static void yamlToConfigNode(const YAML::Node& yaml, ConfigNode& node) {
    if (!yaml || !yaml.IsMap()) return;

    for (auto it = yaml.begin(); it != yaml.end(); ++it) {
        std::string key = it->first.as<std::string>();
        YAML::Node val = it->second;  // Copy - yaml-cpp iterator returns temporary

        if (val.IsMap()) {
            // Recurse into child
            yamlToConfigNode(val, node.children[key]);
        } else if (val.IsScalar()) {
            // Parse scalar to typed Value
            std::string str = val.as<std::string>();
            if (str == "true" || str == "True" || str == "TRUE") {
                node.values[key] = Value(true);
            } else if (str == "false" || str == "False" || str == "FALSE") {
                node.values[key] = Value(false);
            } else {
                // Try int
                try {
                    size_t pos;
                    int i = std::stoi(str, &pos);
                    if (pos == str.size()) {
                        node.values[key] = Value(i);
                        continue;
                    }
                } catch (...) {}
                // Try double
                try {
                    size_t pos;
                    double d = std::stod(str, &pos);
                    if (pos == str.size()) {
                        node.values[key] = Value(d);
                        continue;
                    }
                } catch (...) {}
                // String
                node.values[key] = Value(str);
            }
        } else if (val.IsSequence()) {
            List list;
            for (const auto& item : val) {
                if (item.IsScalar()) {
                    list.push_back(Value(item.as<std::string>()));
                }
            }
            node.values[key] = Value(list);
        }
    }
}

// Convert Value to YAML node
static YAML::Node valueToYaml(const Value& value);

// Convert ConfigNode to YAML (recursive)
static YAML::Node configNodeToYaml(const ConfigNode& node) {
    YAML::Node yaml(YAML::NodeType::Map);

    // Add values
    for (const auto& [key, val] : node.values) {
        yaml[key] = valueToYaml(val);
    }

    // Add children
    for (const auto& [key, child] : node.children) {
        yaml[key] = configNodeToYaml(child);
    }

    return yaml;
}

// Convert Value to YAML node
static YAML::Node valueToYaml(const Value& value) {
    if (auto s = getAs<std::string>(value)) return YAML::Node(*s);
    if (auto i = getAs<int>(value)) return YAML::Node(*i);
    if (auto d = getAs<double>(value)) return YAML::Node(*d);
    if (auto b = getAs<bool>(value)) return YAML::Node(*b);
    if (auto f = getAs<float>(value)) return YAML::Node(*f);
    if (auto u = getAs<uint64_t>(value)) return YAML::Node(*u);
    if (auto list = getAs<List>(value)) {
        YAML::Node seq(YAML::NodeType::Sequence);
        for (const auto& item : *list) {
            seq.push_back(valueToYaml(item));
        }
        return seq;
    }
    if (auto dict = getAs<Dict>(value)) {
        YAML::Node map(YAML::NodeType::Map);
        for (const auto& [k, v] : *dict) {
            map[k] = valueToYaml(v);
        }
        return map;
    }
    return YAML::Node();
}

// ─── ConfigImpl ──────────────────────────────────────────────────────────────

class ConfigImpl : public Config {
public:
    ConfigImpl(int argc, char* argv[]) noexcept
        : _argc(argc), _argv(argv) {
        loadDefaults();
    }

    ~ConfigImpl() override = default;

    Result<void> init() noexcept {
        ydebug("Config::init starting...");

        // Step 1: Quick scan for -c/--config path
        std::string configFilePath = scanForConfigPath();

        // Step 2: Load config file (lowest priority)
#if YETTY_WEB
        ydebug("Config::init - web build, skipping config file");
#else
        std::string effectivePath = configFilePath;
        if (effectivePath.empty()) {
            auto xdgPath = getXDGConfigPath();
            if (std::filesystem::exists(xdgPath)) {
                effectivePath = xdgPath.string();
            }
        }
        if (!effectivePath.empty()) {
            if (auto res = loadFile(effectivePath); !res) {
                ywarn("Failed to load config file {}: {}", effectivePath, error_msg(res));
            } else {
                ydebug("Loaded config from: {}", effectivePath);
            }
        }
#endif

        // Step 3: Apply env overrides (medium priority)
        applyEnvOverrides(_root, "");

        // Step 4: Parse and apply CLI args (highest priority)
        if (auto res = parseAndApplyArgs(); !res) {
            return res;
        }

        // Ensure plugin paths
#if YETTY_WEB
        std::vector<std::string> paths;
#else
        auto paths = pluginPaths();
#endif
        if (paths.empty()) {
            auto defaults = getDefaultPluginPaths();
            std::string pathStr;
            for (size_t i = 0; i < defaults.size(); ++i) {
                if (i > 0) pathStr += ":";
                pathStr += defaults[i];
            }
            set(DataPath("plugins/path"), Value(pathStr));
        }

        _initialized = true;
        ydebug("Config::init done");
        return Ok();
    }

    std::string scanForConfigPath() {
        if (_argc <= 0 || _argv == nullptr) return "";
        for (int i = 1; i < _argc; ++i) {
            std::string arg(_argv[i]);
            if ((arg == "-c" || arg == "--config") && i + 1 < _argc) {
                return _argv[i + 1];
            }
            if (arg.starts_with("-c=")) return arg.substr(3);
            if (arg.starts_with("--config=")) return arg.substr(9);
        }
        return "";
    }

    Result<void> parseAndApplyArgs() {
        if (_argc <= 0 || _argv == nullptr) return Ok();

#if YETTY_WEB
        // Web: read mode from JS-set env vars
        if (const char* mode = getenv("YETTY_MODE")) {
            if (std::string(mode) == "vnc") {
                set(DataPath("vnc/client-mode"), Value(true));
                if (const char* url = getenv("YETTY_VNC_CLIENT"); url && url[0]) {
                    set(DataPath("vnc/host"), Value(std::string(url)));
                }
            } else if (std::string(mode) == "telnet") {
                if (const char* url = getenv("YETTY_TELNET"); url && url[0]) {
                    set(DataPath("shell/telnet"), Value(std::string(url)));
                }
            }
        }
#endif

        args::ArgumentParser parser("yetty", "Terminal emulator with GPU rendering");
        args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
        args::ValueFlag<std::string> configFlag(parser, "FILE", "Config file path", {'c', "config"});
        args::ValueFlag<std::string> executeFlag(parser, "COMMAND", "Execute command", {'e'});
        args::ValueFlag<std::string> telnetFlag(parser, "HOST:PORT", "Connect via telnet", {"telnet"});
        args::ValueFlag<std::string> msdfProviderFlag(parser, "PROVIDER", "MSDF provider (cpu/gpu)", {"msdf-provider"});
        args::ValueFlag<std::string> vncClientFlag(parser, "HOST:PORT", "VNC client", {"vnc-client"});
        args::Flag vncServerFlag(parser, "vnc-server", "Start VNC server", {"vnc-server"});
        args::ValueFlag<uint16_t> vncPortFlag(parser, "PORT", "VNC port", {"vnc-port"}, 5900);
        args::Flag vncHeadlessFlag(parser, "vnc-headless", "VNC headless", {"vnc-headless"});
        args::Flag vncMergeRectsFlag(parser, "vnc-merge-rects", "Merge rects", {"vnc-merge-rects"});
        args::Flag vncRawFlag(parser, "vnc-raw", "Raw encoding", {"vnc-raw"});
        args::ValueFlag<int> vncQualityFlag(parser, "Q", "JPEG quality", {"vnc-compression-quality"}, 0);
        args::Flag vncAlwaysFullFlag(parser, "vnc-always-full", "Always full", {"vnc-always-full"});
        args::Flag vncUseH264Flag(parser, "vnc-use-h264", "H.264", {"vnc-use-h264"});
        args::ValueFlag<std::string> vncTestFlag(parser, "PATTERN", "VNC test", {"vnc-test"});
        args::Flag captureBenchmarkFlag(parser, "capture-benchmark", "Capture benchmark", {"capture-benchmark"});
        args::Flag ytraceDefaultOnFlag(parser, "ytrace-default-on", "Enable all ytrace points", {"ytrace-default-on"});
        args::ValueFlag<std::string> ytraceOutFlag(parser, "FILE", "ytrace output file", {"ytrace-out"});
        args::ValueFlag<std::string> ytraceCtrlSocketFlag(parser, "PATH", "ytrace control socket", {"ytrace-ctrl-socket"});

        try {
            parser.ParseCLI(_argc, _argv);
        } catch (const args::Help&) {
            std::cout << parser;
            std::exit(0);
        } catch (const args::Error& e) {
            yerror("Argument error: {}", e.what());
            std::cerr << parser;
            return Err<void>(e.what());
        }

        // Apply to tree
        if (executeFlag) set(DataPath("shell/command"), Value(args::get(executeFlag)));
        if (telnetFlag) {
            std::string addr = args::get(telnetFlag);
            set(DataPath("shell/telnet"), Value(addr.empty() ? "127.0.0.1:8023" : addr));
        }
        if (msdfProviderFlag) set(DataPath("rendering/msdf-provider"), Value(args::get(msdfProviderFlag)));

        if (vncClientFlag) {
            set(DataPath("vnc/client-mode"), Value(true));
            std::string hp = args::get(vncClientFlag);
            auto pos = hp.rfind(':');
            if (pos != std::string::npos) {
                set(DataPath("vnc/host"), Value(hp.substr(0, pos)));
                set(DataPath("vnc/port"), Value(std::stoi(hp.substr(pos + 1))));
            } else {
                set(DataPath("vnc/host"), Value(hp));
            }
        }

        if (vncServerFlag && vncHeadlessFlag) {
            return Err<void>("--vnc-server and --vnc-headless are mutually exclusive");
        }
        if (vncServerFlag) {
            set(DataPath("vnc/server-mode"), Value(true));
            set(DataPath("vnc/server-port"), Value(static_cast<int>(args::get(vncPortFlag))));
        }
        if (vncHeadlessFlag) {
            set(DataPath("vnc/server-mode"), Value(true));
            set(DataPath("vnc/headless"), Value(true));
            set(DataPath("vnc/server-port"), Value(static_cast<int>(args::get(vncPortFlag))));
        }
        if (vncMergeRectsFlag) set(DataPath("vnc/merge-rects"), Value(true));
        if (vncRawFlag) set(DataPath("vnc/force-raw"), Value(true));
        if (vncQualityFlag && args::get(vncQualityFlag) > 0) {
            set(DataPath("vnc/compression-quality"), Value(std::min(args::get(vncQualityFlag), 100)));
        }
        if (vncAlwaysFullFlag) set(DataPath("vnc/always-full"), Value(true));
        if (vncUseH264Flag) {
            set(DataPath("vnc/use-h264"), Value(true));
            set(DataPath("vnc/always-full"), Value(true));
        }
        if (vncTestFlag) {
            set(DataPath("vnc/test-mode"), Value(true));
            set(DataPath("vnc/server-mode"), Value(true));
            std::string p = args::get(vncTestFlag);
            set(DataPath("vnc/test-pattern"), Value(p));
            std::string cmd;
            if (p == "text") cmd = "bash -c 'frame=0; while true; do clear; echo \"=== VNC TEST ===\"; echo \"Frame: $((++frame))\"; seq 1 20 | xargs -I{} echo \"Line {}: ABCDEFGHIJKLMNOPQRSTUVWXYZ\"; sleep 0.1; done'";
            else if (p == "color") cmd = "bash -c 'while true; do clear; for fg in 30 31 32 33 34 35 36 37; do for bg in 40 41 42 43 44 45 46 47; do printf \"\\033[%d;%dm X \\033[0m\" $fg $bg; done; echo; done; sleep 1; done'";
            else if (p == "scroll") cmd = "bash -c 'i=0; while true; do echo \"Line $((++i)): $(date)\"; sleep 0.05; done'";
            else if (p == "stress") cmd = "bash -c 'while true; do head -c 1000 /dev/urandom | tr -dc A-Za-z0-9; echo; done'";
            if (!cmd.empty()) set(DataPath("shell/command"), Value(cmd));
        }
        if (captureBenchmarkFlag) set(DataPath("debug/capture-benchmark"), Value(true));

        // ytrace options - apply immediately
        if (ytraceDefaultOnFlag) {
            set(DataPath("ytrace/default-on"), Value(true));
        }
        if (ytraceOutFlag) {
            set(DataPath("ytrace/output"), Value(args::get(ytraceOutFlag)));
        }
#ifndef YTRACE_NO_CONTROL_SOCKET
        if (ytraceCtrlSocketFlag) {
            std::string ctrlSocket = args::get(ytraceCtrlSocketFlag);
            set(DataPath("ytrace/ctrl-socket"), Value(ctrlSocket));
            ytrace::TraceManager::instance().open_ctrl_socket(ctrlSocket.c_str());
        }
#endif

        return Ok();
    }

    // ─── Existing Config API ─────────────────────────────────────────────

    std::vector<std::string> getPathList(const std::string& path) const override {
        auto parts = splitPath(path);
        if (parts.empty()) return {};

        std::string key = parts.back();
        parts.pop_back();

        const ConfigNode* node = navigateNode(_root, parts);
        if (!node) return {};

        auto it = node->values.find(key);
        if (it == node->values.end()) return {};

        // Check if it's a List
        if (auto list = getAs<List>(it->second)) {
            std::vector<std::string> result;
            for (const auto& item : *list) {
                if (auto s = getAs<std::string>(item)) {
                    result.push_back(*s);
                }
            }
            return result;
        }

        // Check if it's a colon-separated string
        if (auto s = getAs<std::string>(it->second)) {
            return parsePathList(*s);
        }

        return {};
    }

    bool has(const std::string& path) const override {
        auto parts = splitPath(path);
        if (parts.empty()) return false;

        std::string key = parts.back();
        parts.pop_back();

        const ConfigNode* node = navigateNode(_root, parts);
        if (!node) return false;

        return node->values.count(key) > 0 || node->children.count(key) > 0;
    }

    const YAML::Node& root() const override {
        // Lazily rebuild YAML representation when requested
        _yamlCache = configNodeToYaml(_root);
        return _yamlCache;
    }

    Result<void> setString(const std::string& path, const std::string& value) override {
        return set(DataPath(path), Value(value));
    }

    // Legacy accessors
    bool useDamageTracking() const override {
        return Config::get<bool>(KEY_RENDERING_DAMAGE_TRACKING, true);
    }
    bool showFps() const override {
        return Config::get<bool>(KEY_RENDERING_SHOW_FPS, true);
    }
    bool debugDamageRects() const override {
        return Config::get<bool>(KEY_DEBUG_DAMAGE_RECTS, false);
    }
    uint32_t scrollbackLines() const override {
        return Config::get<uint32_t>(KEY_SCROLLBACK_LINES, 10000);
    }
    std::vector<std::string> pluginPaths() const override {
        return getPathList(KEY_PLUGINS_PATH);
    }
    std::string fontFamily() const override {
        return Config::get<std::string>(KEY_FONT_FAMILY, "default");
    }

    // ─── TreeLike interface ─────────────────────────────────────────────

    Result<std::vector<std::string>> getChildrenNames(const DataPath& path) override {
        const ConfigNode* node = navigateNode(_root, path.asList());
        if (!node) return Ok(std::vector<std::string>{});

        std::vector<std::string> names;
        names.reserve(node->children.size());
        for (const auto& [name, _] : node->children) {
            names.push_back(name);
        }
        return Ok(names);
    }

    Result<Dict> getMetadata(const DataPath& path) override {
        const ConfigNode* node = navigateNode(_root, path.asList());
        if (!node) return Ok(Dict{});
        return Ok(node->values);
    }

    Result<std::vector<std::string>> getMetadataKeys(const DataPath& path) override {
        const ConfigNode* node = navigateNode(_root, path.asList());
        if (!node) return Ok(std::vector<std::string>{});

        std::vector<std::string> keys;
        keys.reserve(node->values.size());
        for (const auto& [k, _] : node->values) {
            keys.push_back(k);
        }
        return Ok(keys);
    }

    Result<Value> get(const DataPath& path) override {
        if (path.isRoot()) return Err<Value>("Config::get: need a key");

        auto parts = path.asList();
        std::string key = parts.back();
        parts.pop_back();

        const ConfigNode* node = navigateNode(_root, parts);
        if (!node) return Err<Value>("Config: path not found: " + path.toString());

        auto it = node->values.find(key);
        if (it != node->values.end()) {
            return Ok(it->second);
        }

        // Maybe it's a child node - return it as a Dict
        auto childIt = node->children.find(key);
        if (childIt != node->children.end()) {
            return Ok(Value(childIt->second.values));
        }

        return Err<Value>("Config: key not found: " + path.toString());
    }

    Result<void> set(const DataPath& path, const Value& value) override {
        if (path.isRoot()) return Err("Config::set: need a key");

        auto parts = path.asList();
        std::string key = parts.back();
        parts.pop_back();

        ConfigNode* node = navigateOrCreate(_root, parts);
        node->values[key] = value;
        return Ok();
    }

    Result<void> addChild(const DataPath& path, const std::string& name, const Dict& data) override {
        ConfigNode* parent = navigateOrCreate(_root, path.asList());
        ConfigNode& child = parent->children[name];
        for (const auto& [k, v] : data) {
            child.values[k] = v;
        }
        return Ok();
    }

    Result<std::string> asTree(const DataPath& path, int depth) override {
        const ConfigNode* node = navigateNode(_root, path.asList());
        if (!node) {
            return Err<std::string>("Config::asTree: path not found: " + path.toString());
        }
        YAML::Node yaml = buildTreeYaml(*node, depth);
        return Ok(YAML::Dump(yaml));
    }

    // ─── File I/O ───────────────────────────────────────────────────────

    Result<void> saveFile(const std::string& path) {
        try {
            std::ofstream file(path);
            if (!file.is_open()) {
                return Err<void>("Cannot open config file for writing: " + path);
            }
            YAML::Node yaml = configNodeToYaml(_root);
            file << yaml;
            return Ok();
        } catch (const std::exception& e) {
            return Err<void>("Failed to save config: " + std::string(e.what()));
        }
    }

protected:
    // Get YAML node by slash path (for template get<T> in base class)
    YAML::Node getNode(const std::string& path) const override {
        auto parts = splitPath(path);
        if (parts.empty()) {
            return configNodeToYaml(_root);
        }

        std::string key = parts.back();
        parts.pop_back();

        const ConfigNode* node = navigateNode(_root, parts);
        if (!node) return YAML::Node();

        // Check values first
        auto it = node->values.find(key);
        if (it != node->values.end()) {
            return valueToYaml(it->second);
        }

        // Check children
        auto childIt = node->children.find(key);
        if (childIt != node->children.end()) {
            return configNodeToYaml(childIt->second);
        }

        return YAML::Node();
    }

private:
    void loadDefaults() {
        // plugins
        _root.children["plugins"].values["path"] = Value(std::string(""));

        // rendering
        _root.children["rendering"].values["damage-tracking"] = Value(true);
        _root.children["rendering"].values["show-fps"] = Value(true);

        // scrollback
        _root.children["scrollback"].values["lines"] = Value(10000);

        // debug
        _root.children["debug"].values["damage-rects"] = Value(false);

        // shell/env
        auto& shellEnv = _root.children["shell"].children["env"];
        shellEnv.values["TERM"] = Value(std::string("xterm-256color"));
        shellEnv.values["COLORTERM"] = Value(std::string("truecolor"));
    }

    // Build {metadata: {...}, children: {...}} YAML for asTree
    YAML::Node buildTreeYaml(const ConfigNode& node, int depth) {
        YAML::Node yaml(YAML::NodeType::Map);

        if (!node.values.empty()) {
            YAML::Node meta(YAML::NodeType::Map);
            for (const auto& [k, v] : node.values) {
                meta[k] = valueToYaml(v);
            }
            yaml["metadata"] = meta;
        }

        if (!node.children.empty() && depth != 0) {
            YAML::Node children(YAML::NodeType::Map);
            int childDepth = (depth > 0) ? depth - 1 : -1;
            for (const auto& [name, child] : node.children) {
                children[name] = buildTreeYaml(child, childDepth);
            }
            yaml["children"] = children;
        }

        return yaml;
    }

    Result<void> loadFile(const std::string& path) {
        try {
            std::ifstream file(path);
            if (!file.is_open()) {
                return Err<void>("Cannot open config file: " + path);
            }
            YAML::Node fileConfig = YAML::Load(file);
            if (fileConfig && !fileConfig.IsNull()) {
                yamlToConfigNode(fileConfig, _root);
            }
            return Ok();
        } catch (const YAML::Exception& e) {
            return Err<void>("YAML parse error: " + std::string(e.what()));
        }
    }

    // Apply environment variable overrides
    void applyEnvOverrides(ConfigNode& node, const std::string& prefix) {
        // Apply to values at this level
        for (auto& [key, value] : node.values) {
            std::string fullPath = prefix.empty() ? key : prefix + "/" + key;

            // Skip shell/env — those are env vars TO export, not FROM environment
            if (fullPath == "shell/env" || fullPath.starts_with("shell/env/")) {
                continue;
            }

            std::string envVar = "YETTY_";
            for (char c : fullPath) {
                if (c == '/' || c == '-') envVar += '_';
                else envVar += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }

            const char* val = std::getenv(envVar.c_str());
            if (val) {
                std::string s(val);
                if (s == "true" || s == "1") {
                    value = Value(true);
                } else if (s == "false" || s == "0") {
                    value = Value(false);
                } else {
                    try {
                        int intVal = std::stoi(s);
                        value = Value(intVal);
                    } catch (...) {
                        value = Value(s);
                    }
                }
                ydebug("Config override from env: {}={}", envVar, val);
            }
        }

        // Recurse into children
        for (auto& [name, child] : node.children) {
            std::string childPrefix = prefix.empty() ? name : prefix + "/" + name;
            applyEnvOverrides(child, childPrefix);
        }
    }

    static std::vector<std::string> parsePathList(const std::string& pathStr) {
        std::vector<std::string> result;
        if (pathStr.empty()) return result;
        std::istringstream ss(pathStr);
        std::string item;
        while (std::getline(ss, item, ':')) {
            if (!item.empty()) result.push_back(item);
        }
        return result;
    }

    ConfigNode _root;
    int _argc = 0;
    char** _argv = nullptr;
    mutable YAML::Node _yamlCache;
    bool _initialized = false;
};

// Factory
Result<Config::Ptr>
Config::createImpl(ContextType&, int argc, char* argv[]) noexcept {
    auto impl = Ptr(new ConfigImpl(argc, argv));
    if (auto res = static_cast<ConfigImpl*>(impl.get())->init(); !res) {
        yerror("Config creation failed: {}", error_msg(res));
        return Err<Ptr>("Failed to initialize Config", res);
    }
    ytest("config-created", "Config created successfully");
    return Ok(std::move(impl));
}

// Static helpers
std::filesystem::path Config::getExecutableDir() {
#if YETTY_WEB
    // On web, there's no executable directory
    return "/";
#elif defined(__linux__)
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::filesystem::path(path).parent_path();
    }
#elif defined(__APPLE__)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        char realPath[PATH_MAX];
        if (realpath(path, realPath)) {
            return std::filesystem::path(realPath).parent_path();
        }
    }
#elif defined(_WIN32)
    char path[MAX_PATH];
    if (GetModuleFileNameA(nullptr, path, MAX_PATH) > 0) {
        return std::filesystem::path(path).parent_path();
    }
#endif
    return std::filesystem::current_path();
}

std::filesystem::path Config::getXDGConfigPath() {
#if YETTY_WEB
    // On web, no config file
    return "";
#else
    std::filesystem::path configDir;

#ifdef _WIN32
    const char* appData = std::getenv("APPDATA");
    if (appData && appData[0] != '\0') {
        configDir = appData;
    } else {
        const char* userProfile = std::getenv("USERPROFILE");
        if (userProfile) {
            configDir = std::filesystem::path(userProfile) / "AppData" / "Roaming";
        } else {
            configDir = "C:\\";
        }
    }
#else
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfig && xdgConfig[0] != '\0') {
        configDir = xdgConfig;
    } else {
        const char* home = std::getenv("HOME");
        if (home) {
            configDir = std::filesystem::path(home) / ".config";
        } else {
#if defined(__ANDROID__)
            // Android: skip config file (no HOME, can't use /tmp)
            return "";
#else
            configDir = "/tmp";
#endif
        }
    }
#endif

    return configDir / "yetty" / "config.yaml";
#endif
}

std::vector<std::string> Config::getDefaultPluginPaths() {
    auto exeDir = getExecutableDir();
    return {
        (exeDir / "plugins").string()
    };
}

} // namespace yetty
