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

class ConfigImpl : public Config {
public:
    ConfigImpl(const std::string& configPath, const YAML::Node& cmdOverrides) noexcept
        : _configPath(configPath), _cmdOverrides(cmdOverrides) {
        // Set defaults
        _config["plugins"]["path"] = "";
        _config["rendering"]["damage-tracking"] = true;
        _config["rendering"]["show-fps"] = true;
        _config["scrollback"]["lines"] = 10000;
        _config["debug"]["damage-rects"] = false;
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

    // Virtual interface implementations
    std::vector<std::string> getPathList(const std::string& path) const override {
        auto node = getNode(path);
        if (!node || node.IsNull()) {
            return {};
        }

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

    bool useDamageTracking() const override {
        return get<bool>(KEY_RENDERING_DAMAGE_TRACKING, true);
    }

    bool showFps() const override {
        return get<bool>(KEY_RENDERING_SHOW_FPS, true);
    }

    bool debugDamageRects() const override {
        return get<bool>(KEY_DEBUG_DAMAGE_RECTS, false);
    }

    uint32_t scrollbackLines() const override {
        return get<uint32_t>(KEY_SCROLLBACK_LINES, 10000);
    }

    std::vector<std::string> pluginPaths() const override {
        return getPathList(KEY_PLUGINS_PATH);
    }

    std::string fontFamily() const override {
        return get<std::string>(KEY_FONT_FAMILY, "default");
    }

protected:
    YAML::Node getNode(const std::string& path) const override {
        std::vector<std::string> parts;
        std::stringstream ss(path);
        std::string part;
        while (std::getline(ss, part, '.')) {
            parts.push_back(part);
        }

        YAML::Node current = YAML::Clone(_config);
        for (const auto& p : parts) {
            if (!current || !current[p]) {
                return YAML::Node();
            }
            current = current[p];
        }
        return current;
    }

private:
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

    void applyEnvOverrides() {
        const std::vector<std::pair<std::string, std::string>> envMappings = {
            {"plugins.path", "YETTY_PLUGINS_PATH"},
            {"rendering.damage-tracking", "YETTY_RENDERING_DAMAGE_TRACKING"},
            {"rendering.show-fps", "YETTY_RENDERING_SHOW_FPS"},
            {"scrollback.lines", "YETTY_SCROLLBACK_LINES"},
            {"debug.damage-rects", "YETTY_DEBUG_DAMAGE_RECTS"},
        };

        for (const auto& [path, envVar] : envMappings) {
            const char* value = std::getenv(envVar.c_str());
            if (value) {
                std::vector<std::string> parts;
                std::stringstream ss(path);
                std::string part;
                while (std::getline(ss, part, '.')) {
                    parts.push_back(part);
                }

                YAML::Node current = _config;
                for (size_t i = 0; i < parts.size() - 1; ++i) {
                    current = current[parts[i]];
                }

                YAML::Node existing = current[parts.back()];
                if (existing && existing.IsScalar()) {
                    try {
                        if (std::string(value) == "true" || std::string(value) == "1") {
                            current[parts.back()] = true;
                        } else if (std::string(value) == "false" || std::string(value) == "0") {
                            current[parts.back()] = false;
                        } else {
                            try {
                                int intVal = std::stoi(value);
                                current[parts.back()] = intVal;
                            } catch (...) {
                                current[parts.back()] = std::string(value);
                            }
                        }
                    } catch (...) {
                        current[parts.back()] = std::string(value);
                    }
                } else {
                    current[parts.back()] = std::string(value);
                }

                ydebug("Config override from env: {}={}", envVar, value);
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
        if (pathStr.empty()) {
            return result;
        }

        std::stringstream ss(pathStr);
        std::string item;
        while (std::getline(ss, item, ':')) {
            if (!item.empty()) {
                result.push_back(item);
            }
        }
        return result;
    }

    static void mergeNodes(YAML::Node& target, const YAML::Node& source) {
        if (!source || source.IsNull()) {
            return;
        }

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

// Static helpers (not part of impl - they're utility methods on the base class)
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
