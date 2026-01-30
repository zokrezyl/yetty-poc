#pragma once

#include "types.hpp"
#include <yaml-cpp/yaml.h>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <filesystem>

namespace yetty::ymery {

// Lang - YAML loader and module resolver
class Lang {
public:
    // Factory method
    static Result<std::shared_ptr<Lang>> create(
        const std::vector<std::filesystem::path>& layoutPaths,
        const std::string& mainModule = "app"
    );

    // Access loaded definitions
    const std::map<std::string, Dict>& widgetDefinitions() const { return _widgetDefinitions; }
    const std::map<std::string, Dict>& dataDefinitions() const { return _dataDefinitions; }
    const Dict& appConfig() const { return _appConfig; }

    // Lifecycle
    Result<void> init();

private:
    Lang() = default;

    // Module loading
    Result<void> _loadModule(
        const std::string& moduleName,
        const std::string& namespace_,
        std::queue<std::pair<std::string, std::string>>& toLoad
    );
    Result<void> _loadModuleFromString(
        const std::string& yamlContent,
        const std::string& namespace_,
        std::queue<std::pair<std::string, std::string>>& toLoad
    );
    Result<std::filesystem::path> _resolveModulePath(const std::string& moduleName);

    // Builtin layouts
    static const char* _getBuiltinYaml();

    // YAML to Dict conversion
    static Dict _yamlToDict(const YAML::Node& node);
    static Value _yamlToValue(const YAML::Node& node);

    std::vector<std::filesystem::path> _layoutPaths;
    std::string _mainModule;

    std::map<std::string, Dict> _widgetDefinitions;
    std::map<std::string, Dict> _dataDefinitions;
    Dict _appConfig;

    std::set<std::string> _loadedModules;
};

using LangPtr = std::shared_ptr<Lang>;

} // namespace yetty::ymery
