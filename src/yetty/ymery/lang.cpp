#include "lang.h"
#include <array>
#include <fstream>
#include <queue>
#include <algorithm>
#include <sstream>

namespace yetty::ymery {

// Builtin filesystem browser layout - shown when no layout is specified
static const char* BUILTIN_YAML = R"(
widgets:
  # Recursive filesystem tree display
  fs-recursive:
    type: composite
    body:
      - foreach-child:
          - tree-node:
              body: builtin.fs-recursive

  # Main filesystem browser window
  filesystem-browser:
    type: imgui-main-window
    label: "Ymery - Filesystem Browser"
    window-size: [900, 600]
    body:
      - text:
          content: "Filesystem Browser"
      - separator
      - text:
          content: "Navigate to a layout file (.yaml) to open it"
      - separator
      - child:
          size: [0, 0]
          border: true
          body:
            - collapsing-header:
                label: "Available Locations"
                data-path: /available
                default-open: true
                body:
                  - builtin.fs-recursive:
            - collapsing-header:
                label: "Recent Files"
                data-path: /opened
                body:
                  - builtin.fs-recursive:

app:
  root-widget: builtin.filesystem-browser
  data-tree: filesystem
)";

Result<std::shared_ptr<Lang>> Lang::create(
    const std::vector<std::filesystem::path>& layoutPaths,
    const std::string& mainModule
) {
    auto lang = std::shared_ptr<Lang>(new Lang());
    lang->_layoutPaths = layoutPaths;
    lang->_mainModule = mainModule;

    if (auto res = lang->init(); !res) {
        return Err<std::shared_ptr<Lang>>("Lang::create: init failed", res);
    }

    return lang;
}

Result<std::shared_ptr<Lang>> Lang::createFromString(
    const std::string& yamlContent
) {
    // Pre-parse to extract module name from app.root-widget
    std::string moduleName = "app";
    try {
        YAML::Node root = YAML::Load(yamlContent);
        if (root["app"] && root["app"]["root-widget"]) {
            std::string rootWidget = root["app"]["root-widget"].as<std::string>();
            size_t dot = rootWidget.find('.');
            if (dot != std::string::npos) {
                moduleName = rootWidget.substr(0, dot);
            }
        }
    } catch (const YAML::Exception&) {
        return Err<std::shared_ptr<Lang>>("Lang::createFromString: YAML parse error");
    }

    auto lang = std::shared_ptr<Lang>(new Lang());
    lang->_mainModule = moduleName;

    std::queue<std::pair<std::string, std::string>> toLoad;

    // Load inline content first so its app config takes priority
    // (_loadModuleFromString only sets _appConfig when empty)
    if (auto res = lang->_loadModuleFromString(yamlContent, moduleName, toLoad); !res) {
        return Err<std::shared_ptr<Lang>>("Lang::createFromString: failed to load inline YAML", res);
    }
    lang->_loadedModules.insert(moduleName);

    // Load builtin (its app config won't overwrite since _appConfig is now set)
    if (auto res = lang->_loadModuleFromString(BUILTIN_YAML, "builtin", toLoad); res) {
        lang->_loadedModules.insert("builtin");
    }

    // Process remaining imports (file-based, need layout paths)
    while (!toLoad.empty()) {
        auto [mod, ns] = toLoad.front();
        toLoad.pop();
        if (lang->_loadedModules.count(mod)) continue;

        auto res = lang->_loadModule(mod, ns, toLoad);
        if (!res) {
            return Err<std::shared_ptr<Lang>>(
                "Lang::createFromString: cannot resolve import '" + mod + "'", res);
        }
        lang->_loadedModules.insert(mod);
    }

    return lang;
}

const char* Lang::_getBuiltinYaml() {
    return BUILTIN_YAML;
}

Result<void> Lang::init() {
    // Breadth-first loading starting from main module
    std::queue<std::pair<std::string, std::string>> toLoad; // (moduleName, namespace)

    // Always load builtin first (from embedded string)
    if (auto res = _loadModuleFromString(BUILTIN_YAML, "builtin", toLoad); !res) {
        // Builtin should always work, but don't fail if it doesn't
    } else {
        _loadedModules.insert("builtin");
    }

    // Then load main module
    toLoad.push({_mainModule, _mainModule});

    while (!toLoad.empty()) {
        auto [moduleName, ns] = toLoad.front();
        toLoad.pop();

        if (_loadedModules.count(moduleName)) {
            continue;
        }

        auto res = _loadModule(moduleName, ns, toLoad);
        if (!res) {
            return Err<void>("Lang::init: failed to load module '" + moduleName + "'", res);
        }

        _loadedModules.insert(moduleName);
    }

    return Ok();
}

Result<void> Lang::_loadModule(
    const std::string& moduleName,
    const std::string& namespace_,
    std::queue<std::pair<std::string, std::string>>& toLoad
) {
    auto pathRes = _resolveModulePath(moduleName);
    if (!pathRes) {
        return Err<void>("Lang::_loadModule: could not resolve path", pathRes);
    }

    auto path = *pathRes;

    // Load YAML file
    YAML::Node root;
    try {
        root = YAML::LoadFile(path.string());
    } catch (const YAML::Exception& e) {
        return Err<void>("Lang::_loadModule: YAML parse error: " + std::string(e.what()));
    }

    // Process 'import' section - queue imported modules for loading
    if (root["import"]) {
        for (const auto& importNode : root["import"]) {
            std::string importName = importNode.as<std::string>();
            // Queue with import name as both module name and namespace
            toLoad.push({importName, importName});
        }
    }

    // Process 'widgets' section
    if (root["widgets"]) {
        for (const auto& kv : root["widgets"]) {
            std::string widgetName = kv.first.as<std::string>();
            std::string fullName = namespace_ + "." + widgetName;
            _widgetDefinitions[fullName] = _yamlToDict(kv.second);
        }
    }

    // Process 'data' section
    if (root["data"]) {
        for (const auto& kv : root["data"]) {
            std::string dataName = kv.first.as<std::string>();
            _dataDefinitions[dataName] = _yamlToDict(kv.second);
        }
    }

    // Process 'app' section
    if (root["app"]) {
        _appConfig = _yamlToDict(root["app"]);
    }

    return Ok();
}

Result<void> Lang::_loadModuleFromString(
    const std::string& yamlContent,
    const std::string& namespace_,
    std::queue<std::pair<std::string, std::string>>& toLoad
) {
    // Parse YAML from string
    YAML::Node root;
    try {
        root = YAML::Load(yamlContent);
    } catch (const YAML::Exception& e) {
        return Err<void>("Lang::_loadModuleFromString: YAML parse error: " + std::string(e.what()));
    }

    // Process 'import' section
    if (root["import"]) {
        for (const auto& importNode : root["import"]) {
            std::string importName = importNode.as<std::string>();
            toLoad.push({importName, importName});
        }
    }

    // Process 'widgets' section
    if (root["widgets"]) {
        for (const auto& kv : root["widgets"]) {
            std::string widgetName = kv.first.as<std::string>();
            std::string fullName = namespace_ + "." + widgetName;
            _widgetDefinitions[fullName] = _yamlToDict(kv.second);
        }
    }

    // Process 'data' section
    if (root["data"]) {
        for (const auto& kv : root["data"]) {
            std::string dataName = kv.first.as<std::string>();
            _dataDefinitions[dataName] = _yamlToDict(kv.second);
        }
    }

    // Process 'app' section - only if not already set by main module
    if (root["app"] && _appConfig.empty()) {
        _appConfig = _yamlToDict(root["app"]);
    }

    return Ok();
}

Result<std::filesystem::path> Lang::_resolveModulePath(const std::string& moduleName) {
    // Convert dots to path separators
    std::string pathStr = moduleName;
    std::replace(pathStr.begin(), pathStr.end(), '.', '/');

    // Try both .ymery and .yaml extensions
    static const std::array<const char*, 2> extensions = {".ymery", ".yaml"};

    for (const auto& basePath : _layoutPaths) {
        for (const auto& ext : extensions) {
            auto fullPath = basePath / (pathStr + ext);
            std::error_code ec;
            if (std::filesystem::exists(fullPath, ec) && !ec) {
                return Ok(fullPath);
            }
        }
    }

    return Err<std::filesystem::path>(
        "Lang::_resolveModulePath: module '" + moduleName + "' not found");
}

Dict Lang::_yamlToDict(const YAML::Node& node) {
    Dict result;
    if (!node.IsMap()) {
        return result;
    }

    for (const auto& kv : node) {
        std::string key = kv.first.as<std::string>();
        result[key] = _yamlToValue(kv.second);
    }

    return result;
}

Value Lang::_yamlToValue(const YAML::Node& node) {
    if (node.IsNull()) {
        return Value{};
    }

    if (node.IsScalar()) {
        // Try to determine type
        std::string str = node.as<std::string>();

        // Try bool
        if (str == "true" || str == "True" || str == "TRUE") {
            return Value(true);
        }
        if (str == "false" || str == "False" || str == "FALSE") {
            return Value(false);
        }

        // Try int
        try {
            size_t pos;
            int i = std::stoi(str, &pos);
            if (pos == str.size()) {
                return Value(i);
            }
        } catch (...) {}

        // Try double
        try {
            size_t pos;
            double d = std::stod(str, &pos);
            if (pos == str.size()) {
                return Value(d);
            }
        } catch (...) {}

        // Default to string
        return Value(str);
    }

    if (node.IsSequence()) {
        List list;
        for (const auto& item : node) {
            list.push_back(_yamlToValue(item));
        }
        return Value(list);
    }

    if (node.IsMap()) {
        return Value(_yamlToDict(node));
    }

    return Value{};
}

} // namespace yetty::ymery
