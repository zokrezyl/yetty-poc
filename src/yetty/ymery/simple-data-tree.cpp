#include "simple-data-tree.hpp"

namespace yetty::ymery {

Result<TreeLikePtr> SimpleDataTree::create() {
    auto tree = std::make_shared<SimpleDataTree>();
    tree->_root = YAML::Node(YAML::NodeType::Map);
    if (auto res = tree->init(); !res) {
        return Err<TreeLikePtr>("SimpleDataTree::create failed", res);
    }
    return tree;
}

Result<TreeLikePtr> SimpleDataTree::create(YAML::Node node) {
    auto tree = std::make_shared<SimpleDataTree>();
    tree->_root = node;
    if (auto res = tree->init(); !res) {
        return Err<TreeLikePtr>("SimpleDataTree::create failed", res);
    }
    return tree;
}

Result<void> SimpleDataTree::init() {
    return Ok();
}

Result<std::vector<std::string>> SimpleDataTree::getChildrenNames(const DataPath& path) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Ok(std::vector<std::string>{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto nestedPath = _getPathBeforeRemaining(path, remaining);
        auto it = _nestedTrees.find(nestedPath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->getChildrenNames(remaining);
        }
    }

    std::vector<std::string> names;

    if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            names.push_back(it->first.as<std::string>());
        }
        return Ok(names);
    }

    if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); ++i) {
            names.push_back(std::to_string(i));
        }
        return Ok(names);
    }

    return Ok(names);
}

Result<Dict> SimpleDataTree::getMetadata(const DataPath& path) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Ok(Dict{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto nestedPath = _getPathBeforeRemaining(path, remaining);
        auto it = _nestedTrees.find(nestedPath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->getMetadata(remaining);
        }
    }

    std::string keyName = path.filename();
    Dict metadata;

    if (node.IsMap() || node.IsSequence()) {
        metadata["label"] = Value(keyName);
        return Ok(metadata);
    }

    std::string valueStr = node.IsDefined() ? node.as<std::string>("") : "";
    if (!keyName.empty()) {
        metadata["label"] = Value(keyName + ": " + valueStr);
    } else {
        metadata["label"] = Value(valueStr);
    }

    return Ok(metadata);
}

Result<std::vector<std::string>> SimpleDataTree::getMetadataKeys(const DataPath& path) {
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

Result<Value> SimpleDataTree::get(const DataPath& path) {
    DataPath nodePath = path.dirname();
    std::string key = path.filename();

    if (key.empty()) {
        return Err<Value>("SimpleDataTree::get: empty key");
    }

    auto navRes = _navigate(nodePath);
    if (!navRes) {
        return Ok(Value{});
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto nestedPath = _getPathBeforeRemaining(nodePath, remaining);
        auto it = _nestedTrees.find(nestedPath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->get(remaining / key);
        }
    }

    auto metaRes = getMetadata(nodePath);
    if (!metaRes) {
        return Ok(Value{});
    }

    auto it = (*metaRes).find(key);
    if (it != (*metaRes).end()) {
        return Ok(it->second);
    }

    return Ok(Value{});
}

Result<void> SimpleDataTree::set(const DataPath& path, const Value& value) {
    DataPath nodePath = path.dirname();
    std::string key = path.filename();

    if (key.empty()) {
        return Err<void>("SimpleDataTree::set: empty key");
    }

    auto navRes = _navigate(nodePath);
    if (!navRes) {
        return Err<void>("SimpleDataTree::set: path not found");
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto nestedPath = _getPathBeforeRemaining(nodePath, remaining);
        auto it = _nestedTrees.find(nestedPath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->set(remaining / key, value);
        }
    }

    if (key == "label" && node.IsScalar()) {
        if (auto s = getAs<std::string>(value)) {
            auto colonPos = s->find(": ");
            if (colonPos != std::string::npos) {
                node = s->substr(colonPos + 2);
            } else {
                node = *s;
            }
            return Ok();
        }
    }

    if (node.IsMap()) {
        node[key] = _valueToYaml(value);
        return Ok();
    }

    return Err<void>("SimpleDataTree::set: cannot set on this node type");
}

Result<void> SimpleDataTree::addChild(const DataPath& path, const std::string& name, const Dict& data) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Err<void>("SimpleDataTree::addChild: path not found");
    }

    auto& [node, remaining] = *navRes;

    if (!remaining.isRoot()) {
        auto nestedPath = _getPathBeforeRemaining(path, remaining);
        auto it = _nestedTrees.find(nestedPath.toString());
        if (it != _nestedTrees.end()) {
            return it->second->addChild(remaining, name, data);
        }
    }

    if (node.IsMap()) {
        auto labelIt = data.find("label");
        if (labelIt != data.end()) {
            node[name] = _valueToYaml(labelIt->second);
        } else {
            node[name] = _dictToYaml(data);
        }
        return Ok();
    }

    if (node.IsSequence()) {
        auto labelIt = data.find("label");
        if (labelIt != data.end()) {
            node.push_back(_valueToYaml(labelIt->second));
        } else {
            node.push_back(_dictToYaml(data));
        }
        return Ok();
    }

    return Err<void>("SimpleDataTree::addChild: cannot add child to this node type");
}

Result<std::string> SimpleDataTree::asTree(const DataPath& path, int /*depth*/) {
    auto navRes = _navigate(path);
    if (!navRes) {
        return Ok(path.toString() + " (not found)");
    }
    return Ok(YAML::Dump((*navRes).first));
}

void SimpleDataTree::registerNested(const DataPath& path, TreeLikePtr tree) {
    _nestedTrees[path.toString()] = tree;
}

// Safe key lookup that never mutates the node (unlike operator[])
static YAML::Node _findKeySimple(const YAML::Node& map, const std::string& key) {
    if (!map.IsMap()) return YAML::Node();
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->first.as<std::string>() == key) {
            return it->second;
        }
    }
    return YAML::Node();
}

Result<std::pair<YAML::Node, DataPath>> SimpleDataTree::_navigate(const DataPath& path) {
    if (path.isRoot() || path.asList().empty()) {
        return Ok(std::make_pair(_root, DataPath::root()));
    }

    YAML::Node current = _root;
    const auto& parts = path.asList();

    for (size_t i = 0; i < parts.size(); ++i) {
        const auto& part = parts[i];

        DataPath currentPath = DataPath(std::vector<std::string>(parts.begin(), parts.begin() + i + 1));
        auto nestedIt = _nestedTrees.find(currentPath.toString());
        if (nestedIt != _nestedTrees.end()) {
            DataPath remaining(std::vector<std::string>(parts.begin() + i + 1, parts.end()));
            return Ok(std::make_pair(current, remaining));
        }

        if (current.IsMap()) {
            YAML::Node child = _findKeySimple(current, part);
            if (!child.IsDefined()) {
                return Err<std::pair<YAML::Node, DataPath>>(
                    "SimpleDataTree::_navigate: key '" + part + "' not found");
            }
            current = child;
        } else if (current.IsSequence()) {
            try {
                size_t index = std::stoul(part);
                if (index >= current.size()) {
                    return Err<std::pair<YAML::Node, DataPath>>(
                        "SimpleDataTree::_navigate: index " + part + " out of range");
                }
                current = current[index];
            } catch (...) {
                return Err<std::pair<YAML::Node, DataPath>>(
                    "SimpleDataTree::_navigate: '" + part + "' is not a valid index");
            }
        } else {
            return Err<std::pair<YAML::Node, DataPath>>(
                "SimpleDataTree::_navigate: cannot navigate through primitive at '" + part + "'");
        }
    }

    return Ok(std::make_pair(current, DataPath::root()));
}

DataPath SimpleDataTree::_getPathBeforeRemaining(const DataPath& fullPath, const DataPath& remaining) {
    const auto& fullParts = fullPath.asList();
    const auto& remParts = remaining.asList();

    if (fullParts.size() <= remParts.size()) {
        return DataPath::root();
    }

    size_t prefixLen = fullParts.size() - remParts.size();
    return DataPath(std::vector<std::string>(fullParts.begin(), fullParts.begin() + prefixLen));
}

Dict SimpleDataTree::_yamlToDict(const YAML::Node& node) {
    Dict result;
    if (!node.IsMap()) return result;

    for (auto it = node.begin(); it != node.end(); ++it) {
        std::string key = it->first.as<std::string>();
        result[key] = _yamlToValue(it->second);
    }
    return result;
}

Value SimpleDataTree::_yamlToValue(const YAML::Node& node) {
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

YAML::Node SimpleDataTree::_dictToYaml(const Dict& dict) {
    YAML::Node node(YAML::NodeType::Map);
    for (const auto& [k, v] : dict) {
        node[k] = _valueToYaml(v);
    }
    return node;
}

YAML::Node SimpleDataTree::_valueToYaml(const Value& value) {
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
