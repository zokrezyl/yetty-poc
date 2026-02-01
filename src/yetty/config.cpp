#include "yetty/config.h"
#include <ytrace/ytrace.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

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

using namespace ymery;

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

// Navigate a YAML node by path components (read-only, avoids operator[] side effects)
static YAML::Node navigateYaml(const YAML::Node& node, const std::vector<std::string>& parts) {
    YAML::Node current = YAML::Clone(node);
    for (const auto& p : parts) {
        if (!current.IsMap()) return YAML::Node();
        bool found = false;
        for (auto it = current.begin(); it != current.end(); ++it) {
            if (it->first.as<std::string>() == p) {
                current = it->second;
                found = true;
                break;
            }
        }
        if (!found) return YAML::Node();
    }
    return current;
}

// Convert a YAML scalar to ymery::Value
static Value yamlScalarToValue(const YAML::Node& node) {
    if (!node || node.IsNull() || !node.IsDefined()) {
        return Value{};
    }
    if (node.IsScalar()) {
        std::string str = node.as<std::string>();
        if (str == "true" || str == "True" || str == "TRUE") return Value(true);
        if (str == "false" || str == "False" || str == "FALSE") return Value(false);
        try {
            size_t pos;
            int i = std::stoi(str, &pos);
            if (pos == str.size()) return Value(i);
        } catch (...) {}
        try {
            size_t pos;
            double d = std::stod(str, &pos);
            if (pos == str.size()) return Value(d);
        } catch (...) {}
        return Value(str);
    }
    if (node.IsSequence()) {
        List list;
        for (const auto& item : node) {
            list.push_back(yamlScalarToValue(item));
        }
        return Value(list);
    }
    if (node.IsMap()) {
        Dict dict;
        for (auto it = node.begin(); it != node.end(); ++it) {
            dict[it->first.as<std::string>()] = yamlScalarToValue(it->second);
        }
        return Value(dict);
    }
    return Value{};
}

// Set a YAML node from a ymery::Value
static void setYamlFromValue(YAML::Node& node, const std::string& key, const Value& value) {
    if (auto s = getAs<std::string>(value)) node[key] = *s;
    else if (auto i = getAs<int>(value)) node[key] = *i;
    else if (auto d = getAs<double>(value)) node[key] = *d;
    else if (auto b = getAs<bool>(value)) node[key] = *b;
    else if (auto f = getAs<float>(value)) node[key] = *f;
    else if (auto u = getAs<uint64_t>(value)) node[key] = *u;
}

// Convert Dict to YAML::Node (for asTree)
static YAML::Node dictToYaml(const Dict& dict) {
    YAML::Node node(YAML::NodeType::Map);
    for (const auto& [k, v] : dict) {
        if (auto s = getAs<std::string>(v)) node[k] = *s;
        else if (auto i = getAs<int>(v)) node[k] = *i;
        else if (auto d = getAs<double>(v)) node[k] = *d;
        else if (auto b = getAs<bool>(v)) node[k] = *b;
        else if (auto f = getAs<float>(v)) node[k] = *f;
        else if (auto u = getAs<uint64_t>(v)) node[k] = *u;
    }
    return node;
}

// ─── ConfigImpl ──────────────────────────────────────────────────────────────

class ConfigImpl : public Config {
public:
    ConfigImpl(const std::string& configPath, const YAML::Node& cmdOverrides) noexcept
        : _configPath(configPath), _cmdOverrides(cmdOverrides),
          _config(YAML::Load(
            "plugins:\n"
            "  path: ''\n"
            "rendering:\n"
            "  damage-tracking: true\n"
            "  show-fps: true\n"
            "scrollback:\n"
            "  lines: 10000\n"
            "debug:\n"
            "  damage-rects: false\n"
            "shell:\n"
            "  env:\n"
            "    TERM: xterm-256color\n"
            "    COLORTERM: truecolor\n"
          )) {
    }

    ~ConfigImpl() override = default;

    Result<void> init() noexcept {
        std::string effectivePath = _configPath;
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
                yinfo("Loaded config from: {}", effectivePath);
            }
        }

        applyEnvOverrides();

        if (_cmdOverrides && !_cmdOverrides.IsNull()) {
            applyOverrides(_cmdOverrides);
        }

        auto paths = pluginPaths();
        if (paths.empty()) {
            auto defaults = getDefaultPluginPaths();
            std::string pathStr;
            for (size_t i = 0; i < defaults.size(); ++i) {
                if (i > 0) pathStr += ":";
                pathStr += defaults[i];
            }
            _config["plugins"]["path"] = pathStr;
        }

        _initialized = true;
        return Ok();
    }

    // ─── Existing Config API (now uses slash paths) ─────────────────────

    std::vector<std::string> getPathList(const std::string& path) const override {
        auto node = getNode(path);
        if (!node || node.IsNull()) return {};
        if (node.IsSequence()) {
            std::vector<std::string> result;
            for (const auto& item : node) {
                result.push_back(item.as<std::string>());
            }
            return result;
        }
        if (node.IsScalar()) {
            return parsePathList(node.as<std::string>());
        }
        return {};
    }

    bool has(const std::string& path) const override {
        YAML::Node node = getNode(path);
        return node && !node.IsNull();
    }

    const YAML::Node& root() const override { return _config; }

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
        YAML::Node node = navigateYaml(_config, path.asList());
        if (!node || !node.IsMap()) return Ok(std::vector<std::string>{});

        std::vector<std::string> children;
        for (auto it = node.begin(); it != node.end(); ++it) {
            if (it->second.IsMap()) {
                children.push_back(it->first.as<std::string>());
            }
        }
        return Ok(children);
    }

    Result<Dict> getMetadata(const DataPath& path) override {
        YAML::Node node = navigateYaml(_config, path.asList());
        if (!node || !node.IsMap()) return Ok(Dict{});

        Dict meta;
        for (auto it = node.begin(); it != node.end(); ++it) {
            if (!it->second.IsMap()) {
                meta[it->first.as<std::string>()] = yamlScalarToValue(it->second);
            }
        }
        return Ok(meta);
    }

    Result<std::vector<std::string>> getMetadataKeys(const DataPath& path) override {
        auto meta = getMetadata(path);
        if (!meta) return Err<std::vector<std::string>>("getMetadataKeys failed", meta);
        std::vector<std::string> keys;
        for (const auto& [k, _] : *meta) {
            keys.push_back(k);
        }
        return Ok(keys);
    }

    Result<Value> get(const DataPath& path) override {
        if (path.isRoot()) return Err<Value>("Config::get: need a key");
        auto parts = path.asList();
        std::string key = parts.back();
        parts.pop_back();
        YAML::Node node = navigateYaml(_config, parts);
        if (!node || !node[key]) return Err<Value>("Config: key not found: " + path.toString());
        return Ok(yamlScalarToValue(node[key]));
    }

    Result<void> set(const DataPath& path, const Value& value) override {
        if (path.isRoot()) return Err("Config::set: need a key");
        auto parts = path.asList();
        std::string key = parts.back();
        parts.pop_back();

        // Navigate/create intermediate YAML nodes
        // Must avoid operator[] on non-existent keys — yaml-cpp will corrupt the node
        YAML::Node current = _config;
        for (const auto& p : parts) {
            bool exists = false;
            for (auto it = current.begin(); it != current.end(); ++it) {
                if (it->first.as<std::string>() == p && it->second.IsMap()) {
                    current.reset(it->second);
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                current[p] = YAML::Node(YAML::NodeType::Map);
                current.reset(current[p]);
            }
        }
        setYamlFromValue(current, key, value);
        return Ok();
    }

    Result<void> addChild(const DataPath& path, const std::string& name, const Dict& data) override {
        YAML::Node current = _config;
        for (const auto& p : path.asList()) {
            bool exists = false;
            for (auto it = current.begin(); it != current.end(); ++it) {
                if (it->first.as<std::string>() == p && it->second.IsMap()) {
                    current.reset(it->second);
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                current[p] = YAML::Node(YAML::NodeType::Map);
                current.reset(current[p]);
            }
        }
        current[name] = YAML::Node(YAML::NodeType::Map);
        YAML::Node child = current[name];
        for (const auto& [k, v] : data) {
            setYamlFromValue(child, k, v);
        }
        return Ok();
    }

    Result<std::string> asTree(const DataPath& path, int depth) override {
        YAML::Node node = navigateYaml(_config, path.asList());
        if (!node || !node.IsMap()) {
            return Err<std::string>("Config::asTree: not a map at " + path.toString());
        }
        auto yamlTree = buildTreeYaml(node, depth);
        return Ok(YAML::Dump(yamlTree));
    }

protected:
    // Get YAML node by slash path
    YAML::Node getNode(const std::string& path) const override {
        return navigateYaml(_config, splitPath(path));
    }

private:
    // Build {metadata: {...}, children: {...}} YAML for asTree
    YAML::Node buildTreeYaml(const YAML::Node& node, int depth) {
        YAML::Node yaml(YAML::NodeType::Map);

        // Metadata: non-map entries
        YAML::Node meta(YAML::NodeType::Map);
        YAML::Node children(YAML::NodeType::Map);
        bool hasMeta = false, hasChildren = false;

        for (auto it = node.begin(); it != node.end(); ++it) {
            std::string key = it->first.as<std::string>();
            if (it->second.IsMap()) {
                hasChildren = true;
                if (depth != 0) {
                    int childDepth = (depth > 0) ? depth - 1 : -1;
                    children[key] = buildTreeYaml(it->second, childDepth);
                }
            } else {
                hasMeta = true;
                meta[key] = YAML::Clone(it->second);
            }
        }

        if (hasMeta) yaml["metadata"] = meta;
        if (hasChildren && depth != 0) yaml["children"] = children;
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
                mergeNodes(_config, fileConfig);
            }
            return Ok();
        } catch (const YAML::Exception& e) {
            return Err<void>("YAML parse error: " + std::string(e.what()));
        }
    }

    // Automatic env override: walk all leaf scalars, derive YETTY_* env var name
    void applyEnvOverrides() {
        walkAndApplyEnv(_config, "");
    }

    void walkAndApplyEnv(YAML::Node node, const std::string& prefix) {
        if (!node.IsMap()) return;
        for (auto it = node.begin(); it != node.end(); ++it) {
            std::string key = it->first.as<std::string>();
            std::string fullPath = prefix.empty() ? key : prefix + "/" + key;

            // Skip shell/env — those are env vars TO export, not FROM environment
            if (fullPath == "shell/env" || fullPath.starts_with("shell/env/")) {
                continue;
            }

            if (it->second.IsMap()) {
                walkAndApplyEnv(it->second, fullPath);
            } else {
                // Derive env var: YETTY_ + uppercase(path with / and - → _)
                std::string envVar = "YETTY_";
                for (char c : fullPath) {
                    if (c == '/' || c == '-') envVar += '_';
                    else envVar += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                }
                const char* val = std::getenv(envVar.c_str());
                if (val) {
                    applyEnvValue(it->second, val);
                    ydebug("Config override from env: {}={}", envVar, val);
                }
            }
        }
    }

    static void applyEnvValue(YAML::Node node, const char* val) {
        std::string s(val);
        if (s == "true" || s == "1") {
            node = true;
        } else if (s == "false" || s == "0") {
            node = false;
        } else {
            try {
                int intVal = std::stoi(s);
                node = intVal;
            } catch (...) {
                node = s;
            }
        }
    }

    void applyOverrides(const YAML::Node& overrides) {
        if (overrides && !overrides.IsNull()) {
            mergeNodes(_config, overrides);
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

    static void mergeNodes(YAML::Node& target, const YAML::Node& source) {
        if (!source || source.IsNull()) return;
        if (source.IsMap()) {
            for (const auto& pair : source) {
                std::string key = pair.first.as<std::string>();
                if (pair.second.IsMap() && target[key] && target[key].IsMap()) {
                    YAML::Node targetChild = target[key];
                    mergeNodes(targetChild, pair.second);
                } else {
                    target[key] = YAML::Clone(pair.second);
                }
            }
        } else {
            target = YAML::Clone(source);
        }
    }

    YAML::Node _config;
    std::string _configPath;
    YAML::Node _cmdOverrides;
    bool _initialized = false;
};

// Factory
Result<Config::Ptr>
Config::createImpl(ContextType&, const std::string& configPath, const YAML::Node& cmdOverrides) noexcept {
    auto impl = Ptr(new ConfigImpl(configPath, cmdOverrides));
    if (auto res = static_cast<ConfigImpl*>(impl.get())->init(); !res) {
        return Err<Ptr>("Failed to initialize Config", res);
    }
    return Ok(std::move(impl));
}

// Static helpers
std::filesystem::path Config::getExecutableDir() {
#ifdef __linux__
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
            configDir = "/tmp";
        }
    }
#endif

    return configDir / "yetty" / "config.yaml";
}

std::vector<std::string> Config::getDefaultPluginPaths() {
    auto exeDir = getExecutableDir();
    return {
        (exeDir / "plugins").string()
    };
}

} // namespace yetty
