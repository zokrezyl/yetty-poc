// data-tree - tree with explicit children/metadata structure
// Each node has { "children": {...}, "metadata": {...} } format
// Uses native C++ structures internally (not YAML::Node) to avoid yaml-cpp mutation bugs
#include "data-tree.h"
#include <ytrace/ytrace.hpp>
#include <sstream>

namespace yetty::ymery {

Result<TreeLikePtr> DataTree::create() {
    auto tree = std::make_shared<DataTree>();
    tree->_root = std::make_shared<NativeNode>();
    if (auto res = tree->init(); !res) {
        return Err<TreeLikePtr>("DataTree::create failed", res);
    }
    return tree;
}

Result<TreeLikePtr> DataTree::create(YAML::Node node) {
    auto tree = std::make_shared<DataTree>();
    tree->_root = _yamlToNative(node);
    if (auto res = tree->init(); !res) {
        return Err<TreeLikePtr>("DataTree::create failed", res);
    }
    return tree;
}

Result<TreeLikePtr> DataTree::create(const Dict& data) {
    auto tree = std::make_shared<DataTree>();
    tree->_root = _dictArgToNative(data);
    ydebug("DataTree::create(Dict) root children={}", tree->_root->children.size());
    if (auto res = tree->init(); !res) {
        return Err<TreeLikePtr>("DataTree::create from Dict failed", res);
    }
    return tree;
}

Result<void> DataTree::init() {
    if (!_root) {
        _root = std::make_shared<NativeNode>();
    }
    return Ok();
}

Result<std::vector<std::string>> DataTree::getChildrenNames(const DataPath& path) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Ok(std::vector<std::string>{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto it = _nestedTrees.find(path.toString());
        if (it != _nestedTrees.end()) {
            return it->second->getChildrenNames(remaining);
        }
    }

    std::vector<std::string> names;
    for (const auto& [name, _] : node->children) {
        names.push_back(name);
    }
    return Ok(names);
}

Result<Dict> DataTree::getMetadata(const DataPath& path) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Ok(Dict{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto it = _nestedTrees.find(path.toString());
        if (it != _nestedTrees.end()) {
            return it->second->getMetadata(remaining);
        }
    }

    return Ok(node->metadata);
}

Result<std::vector<std::string>> DataTree::getMetadataKeys(const DataPath& path) {
    auto res = getMetadata(path);
    if (!res) {
        return Err<std::vector<std::string>>("getMetadata failed", res);
    }
    std::vector<std::string> keys;
    for (const auto& [k, _] : *res) {
        keys.push_back(k);
    }
    return Ok(keys);
}

Result<Value> DataTree::get(const DataPath& path) {
    DataPath nodePath = path.dirname();
    std::string key = path.filename();

    if (key.empty()) {
        return Err<Value>("DataTree::get: empty key");
    }

    auto navRes = _navigate(nodePath);
    if (!navRes) {
        return Ok(Value{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto it = _nestedTrees.find(nodePath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->get(remaining / key);
        }
    }

    auto metaIt = node->metadata.find(key);
    if (metaIt == node->metadata.end()) {
        return Ok(Value{});
    }

    return Ok(metaIt->second);
}

Result<void> DataTree::set(const DataPath& path, const Value& value) {
    DataPath nodePath = path.dirname();
    std::string key = path.filename();

    if (key.empty()) {
        return Err<void>("DataTree::set: empty key");
    }

    auto navRes = _navigate(nodePath);
    if (!navRes) {
        auto ensured = _ensurePath(nodePath);
        if (!ensured) {
            return Err<void>("DataTree::set: failed to create path");
        }
        navRes = _navigate(nodePath);
        if (!navRes) {
            return Err<void>("DataTree::set: failed to navigate after ensure");
        }
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto it = _nestedTrees.find(nodePath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->set(remaining / key, value);
        }
    }

    node->metadata[key] = value;
    return Ok();
}

Result<void> DataTree::addChild(const DataPath& path, const std::string& name, const Dict& data) {
    auto navRes = _navigate(path);
    if (!navRes) {
        auto ensured = _ensurePath(path);
        if (!ensured) {
            return Err<void>("DataTree::addChild: failed to create path");
        }
        navRes = _navigate(path);
        if (!navRes) {
            return Err<void>("DataTree::addChild: failed to navigate after ensure");
        }
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto it = _nestedTrees.find(path.toString());
        if (it != _nestedTrees.end()) {
            return it->second->addChild(remaining, name, data);
        }
    }

    auto child = std::make_shared<NativeNode>();
    child->metadata = data;
    node->children[name] = child;

    return Ok();
}

Result<std::string> DataTree::asTree(const DataPath& /*path*/, int /*depth*/) {
    YAML::Node yaml = _nativeToYaml(_root);
    return Ok(YAML::Dump(yaml));
}

void DataTree::registerNested(const DataPath& path, TreeLikePtr tree) {
    _nestedTrees[path.toString()] = tree;
}

Result<std::pair<NativeNodePtr, DataPath>> DataTree::_navigate(const DataPath& path) {
    if (path.isRoot() || path.asList().empty()) {
        return Ok(std::make_pair(_root, DataPath::root()));
    }

    NativeNodePtr current = _root;
    const auto& parts = path.asList();

    for (size_t i = 0; i < parts.size(); ++i) {
        const auto& part = parts[i];

        DataPath currentPath = DataPath(std::vector<std::string>(parts.begin(), parts.begin() + i + 1));
        auto nestedIt = _nestedTrees.find(currentPath.toString());
        if (nestedIt != _nestedTrees.end()) {
            DataPath remaining(std::vector<std::string>(parts.begin() + i + 1, parts.end()));
            return Ok(std::make_pair(current, remaining));
        }

        auto childIt = current->children.find(part);
        if (childIt == current->children.end()) {
            return Err<std::pair<NativeNodePtr, DataPath>>(
                "DataTree::_navigate: child '" + part + "' not found");
        }

        current = childIt->second;
    }

    return Ok(std::make_pair(current, DataPath::root()));
}

NativeNodePtr DataTree::_ensurePath(const DataPath& path) {
    if (path.isRoot() || path.asList().empty()) {
        return _root;
    }

    NativeNodePtr current = _root;
    for (const auto& part : path.asList()) {
        auto childIt = current->children.find(part);
        if (childIt == current->children.end()) {
            auto child = std::make_shared<NativeNode>();
            current->children[part] = child;
            current = child;
        } else {
            current = childIt->second;
        }
    }
    return current;
}

// Convert YAML node (with children/metadata structure) to native tree
NativeNodePtr DataTree::_yamlToNative(const YAML::Node& node) {
    auto native = std::make_shared<NativeNode>();
    if (!node.IsMap()) return native;

    // Extract metadata
    for (auto it = node.begin(); it != node.end(); ++it) {
        std::string key = it->first.as<std::string>();
        if (key == "children") {
            // Process children recursively
            if (it->second.IsMap()) {
                for (auto childIt = it->second.begin(); childIt != it->second.end(); ++childIt) {
                    std::string childName = childIt->first.as<std::string>();
                    native->children[childName] = _yamlToNative(childIt->second);
                }
            }
        } else if (key == "metadata") {
            if (it->second.IsMap()) {
                native->metadata = _yamlToDict(it->second);
            }
        }
        // Ignore other top-level keys
    }

    return native;
}

// Convert Dict arg (from data: section) to native tree
// The Dict arg uses the same children/metadata structure
NativeNodePtr DataTree::_dictArgToNative(const Dict& data) {
    auto native = std::make_shared<NativeNode>();

    auto childrenIt = data.find("children");
    if (childrenIt != data.end()) {
        if (auto childrenDict = getAs<Dict>(childrenIt->second)) {
            for (const auto& [name, childVal] : *childrenDict) {
                if (auto childDict = getAs<Dict>(childVal)) {
                    native->children[name] = _dictArgToNative(*childDict);
                } else {
                    // Leaf child with no structure
                    native->children[name] = std::make_shared<NativeNode>();
                }
            }
        }
    }

    auto metaIt = data.find("metadata");
    if (metaIt != data.end()) {
        if (auto metaDict = getAs<Dict>(metaIt->second)) {
            native->metadata = *metaDict;
        }
    }

    return native;
}

// Convert native tree to YAML (for asTree debug output)
YAML::Node DataTree::_nativeToYaml(const NativeNodePtr& node) {
    YAML::Node yaml(YAML::NodeType::Map);

    if (!node->metadata.empty()) {
        yaml["metadata"] = _dictToYaml(node->metadata);
    }

    if (!node->children.empty()) {
        YAML::Node childrenYaml(YAML::NodeType::Map);
        for (const auto& [name, child] : node->children) {
            childrenYaml[name] = _nativeToYaml(child);
        }
        yaml["children"] = childrenYaml;
    }

    return yaml;
}

Dict DataTree::_yamlToDict(const YAML::Node& node) {
    Dict result;
    if (!node.IsMap()) return result;

    for (auto it = node.begin(); it != node.end(); ++it) {
        std::string key = it->first.as<std::string>();
        result[key] = _yamlToValue(it->second);
    }
    return result;
}

Value DataTree::_yamlToValue(const YAML::Node& node) {
    if (node.IsNull() || !node.IsDefined()) {
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
            list.push_back(_yamlToValue(item));
        }
        return Value(list);
    }
    if (node.IsMap()) {
        return Value(_yamlToDict(node));
    }
    return Value{};
}

YAML::Node DataTree::_dictToYaml(const Dict& dict) {
    YAML::Node node(YAML::NodeType::Map);
    for (const auto& [k, v] : dict) {
        node[k] = _valueToYaml(v);
    }
    return node;
}

YAML::Node DataTree::_valueToYaml(const Value& value) {
    if (!value.has_value()) {
        return YAML::Node();
    }
    if (auto s = getAs<std::string>(value)) {
        return YAML::Node(*s);
    }
    if (auto i = getAs<int>(value)) {
        return YAML::Node(*i);
    }
    if (auto d = getAs<double>(value)) {
        return YAML::Node(*d);
    }
    if (auto b = getAs<bool>(value)) {
        return YAML::Node(*b);
    }
    if (auto list = getAs<List>(value)) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (const auto& item : *list) {
            node.push_back(_valueToYaml(item));
        }
        return node;
    }
    if (auto dict = getAs<Dict>(value)) {
        return _dictToYaml(*dict);
    }
    return YAML::Node();
}

} // namespace yetty::ymery
