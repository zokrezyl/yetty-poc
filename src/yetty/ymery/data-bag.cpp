#include "data-bag.h"
#include "data-tree.h"
#include "simple-data-tree.h"
#include <regex>
#include <ytrace/ytrace.hpp>

namespace yetty::ymery {

Result<DataBag::Ptr> DataBag::create(
    std::shared_ptr<Dispatcher> dispatcher,
    std::map<std::string, TreeLikePtr> dataTrees,
    const std::string& mainDataKey,
    const DataPath& mainDataPath,
    const Dict& statics
) {
    auto bag = Ptr(new DataBag());
    bag->_dispatcher = dispatcher;
    bag->_dataTrees = std::move(dataTrees);
    bag->_mainDataKey = mainDataKey;
    bag->_mainDataPath = mainDataPath;
    bag->_statics = statics;

    // Set main data tree
    if (!mainDataKey.empty()) {
        auto it = bag->_dataTrees.find(mainDataKey);
        if (it != bag->_dataTrees.end()) {
            bag->_mainDataTree = it->second;
        }
    }

    if (auto res = bag->_init(); !res) {
        return Err<Ptr>("DataBag::create: init failed", res);
    }

    return bag;
}

Result<void> DataBag::_init() {
    // Process 'data' section: create local data trees from definitions
    auto dataIt = _statics.find("data");
    if (dataIt != _statics.end()) {
        if (auto dataDict = getAs<Dict>(dataIt->second)) {
            for (const auto& [treeName, treeDef] : *dataDict) {
                auto defDict = getAs<Dict>(treeDef);
                if (!defDict) continue;

                // Get type (default: "data-tree")
                std::string treeType = "data-tree";
                auto typeIt = defDict->find("type");
                if (typeIt != defDict->end()) {
                    if (auto t = getAs<std::string>(typeIt->second)) {
                        treeType = *t;
                    }
                }

                // Get arg (initial data)
                Dict arg;
                auto argIt = defDict->find("arg");
                if (argIt != defDict->end()) {
                    if (auto a = getAs<Dict>(argIt->second)) {
                        arg = *a;
                    }
                }

                // Create the tree
                TreeLikePtr tree;
                if (treeType == "data-tree") {
                    auto treeRes = DataTree::create(arg);
                    if (!treeRes) {
                        ydebug("DataBag::_init: failed to create data-tree '{}'", treeName);
                        continue;
                    }
                    tree = *treeRes;
                } else if (treeType == "simple-data-tree") {
                    auto treeRes = SimpleDataTree::create();
                    if (!treeRes) {
                        ydebug("DataBag::_init: failed to create simple-data-tree '{}'", treeName);
                        continue;
                    }
                    tree = *treeRes;
                } else {
                    ydebug("DataBag::_init: unknown tree type '{}'", treeType);
                    continue;
                }

                _dataTrees[treeName] = tree;
                ydebug("DataBag::_init: created data tree '{}' (type={})", treeName, treeType);
            }
        }
    }

    // Process 'main-data' to override main tree
    auto mainDataIt = _statics.find("main-data");
    if (mainDataIt != _statics.end()) {
        if (auto key = getAs<std::string>(mainDataIt->second)) {
            _mainDataKey = *key;
            auto it = _dataTrees.find(*key);
            if (it != _dataTrees.end()) {
                _mainDataTree = it->second;
                ydebug("DataBag::_init: set main data tree to '{}'", *key);
            } else {
                ydebug("DataBag::_init: main-data '{}' not found in trees", *key);
            }
        }
    }

    return Ok();
}

Result<DataBag::Ptr> DataBag::inherit(
    const std::string& dataPathSpec,
    const Dict& statics
) {
    DataPath newPath = _mainDataPath;
    TreeLikePtr newTree = _mainDataTree;
    std::string newKey = _mainDataKey;

    // Parse data-path spec if provided
    if (!dataPathSpec.empty()) {
        auto parsed = _parseDataPathSpec(dataPathSpec);
        if (!parsed) {
            return Err<Ptr>(
                "DataBag::inherit: failed to parse data-path", parsed);
        }
        newTree = (*parsed).first;
        newPath = (*parsed).second;

        // Find key for the new tree
        for (const auto& [k, v] : _dataTrees) {
            if (v == newTree) {
                newKey = k;
                break;
            }
        }
    }

    // Copy dataTrees (sibling isolation)
    auto treesCopy = _dataTrees;

    return create(
        _dispatcher,
        std::move(treesCopy),
        newKey,
        newPath,
        statics
    );
}

Result<Value> DataBag::get(const std::string& key, const Value& defaultValue) {
    // First check statics
    auto it = _statics.find(key);
    if (it != _statics.end()) {
        Value val = it->second;

        // Check if it's a reference
        if (auto str = getAs<std::string>(val)) {
            if (_isReference(*str)) {
                return _resolveReference(*str);
            }
            if (_hasInterpolation(*str)) {
                auto interp = _interpolate(*str);
                if (!interp) {
                    return Err<Value>("DataBag::get: interpolation failed", interp);
                }
                return Ok(Value(*interp));
            }
        }
        return Ok(val);
    }

    // Then try main data tree
    if (_mainDataTree) {
        DataPath path = _mainDataPath / key;
        auto res = _mainDataTree->get(path);
        if (res) {
            return res;
        }
    }

    // Return default
    if (defaultValue.has_value()) {
        return Ok(defaultValue);
    }

    return Err<Value>("DataBag::get: key '" + key + "' not found");
}

Result<void> DataBag::set(const std::string& key, const Value& value) {
    // Check if statics has a reference for this key
    auto it = _statics.find(key);
    if (it != _statics.end()) {
        if (auto str = getAs<std::string>(it->second)) {
            if (_isReference(*str)) {
                auto parsed = _parseDataPathSpec(*str);
                if (!parsed) {
                    return Err<void>("DataBag::set: failed to parse reference", parsed);
                }
                auto& [tree, path] = *parsed;
                return tree->set(path, value);
            }
        }
    }

    // Default: set in main data tree
    if (_mainDataTree) {
        DataPath path = _mainDataPath / key;
        return _mainDataTree->set(path, value);
    }

    return Err<void>("DataBag::set: no data tree available");
}

Result<Value> DataBag::getStatic(const std::string& key, const Value& defaultValue) {
    auto it = _statics.find(key);
    if (it != _statics.end()) {
        return Ok(it->second);
    }
    if (defaultValue.has_value()) {
        return Ok(defaultValue);
    }
    return Err<Value>("DataBag::getStatic: key '" + key + "' not found");
}

Result<Dict> DataBag::getMetadata() {
    if (!_mainDataTree) {
        return Err<Dict>("DataBag::getMetadata: no main data tree");
    }
    return _mainDataTree->getMetadata(_mainDataPath);
}

Result<std::vector<std::string>> DataBag::getMetadataKeys() {
    if (!_mainDataTree) {
        return Err<std::vector<std::string>>(
            "DataBag::getMetadataKeys: no main data tree");
    }
    return _mainDataTree->getMetadataKeys(_mainDataPath);
}

Result<std::vector<std::string>> DataBag::getChildrenNames() {
    if (!_mainDataTree) {
        return Err<std::vector<std::string>>(
            "DataBag::getChildrenNames: no main data tree");
    }
    return _mainDataTree->getChildrenNames(_mainDataPath);
}

Result<DataPath> DataBag::getDataPath() {
    return Ok(_mainDataPath);
}

Result<std::string> DataBag::getDataPathStr() {
    return Ok(_mainDataPath.toString());
}

Result<void> DataBag::addChild(const Dict& childSpec) {
    if (!_mainDataTree) {
        return Err<void>("DataBag::addChild: no main data tree");
    }

    auto nameIt = childSpec.find("name");
    if (nameIt == childSpec.end()) {
        return Err<void>("DataBag::addChild: 'name' required");
    }

    auto name = getAs<std::string>(nameIt->second);
    if (!name) {
        return Err<void>("DataBag::addChild: 'name' must be string");
    }

    Dict data;
    auto metadataIt = childSpec.find("metadata");
    if (metadataIt != childSpec.end()) {
        if (auto md = getAs<Dict>(metadataIt->second)) {
            data = *md;
        }
    }

    return _mainDataTree->addChild(_mainDataPath, *name, data);
}

bool DataBag::_isReference(const std::string& str) {
    return !str.empty() && (str[0] == '@' || str[0] == '$');
}

bool DataBag::_hasInterpolation(const std::string& str) {
    return str.find("$") != std::string::npos || str.find("@") != std::string::npos;
}

Result<Value> DataBag::_resolveReference(const std::string& refStr) {
    auto parsed = _parseDataPathSpec(refStr);
    if (!parsed) {
        return Err<Value>("DataBag::_resolveReference: parse failed", parsed);
    }

    auto& [tree, path] = *parsed;
    return tree->get(path);
}

Result<std::pair<TreeLikePtr, DataPath>> DataBag::_parseDataPathSpec(const std::string& spec) {
    TreeLikePtr tree = _mainDataTree;
    std::string pathStr = spec;

    // Handle $tree@path or $tree format
    if (!spec.empty() && spec[0] == '$') {
        size_t atPos = spec.find('@');
        std::string treeName;

        if (atPos != std::string::npos) {
            treeName = spec.substr(1, atPos - 1);
            pathStr = spec.substr(atPos + 1);
        } else {
            treeName = spec.substr(1);
            pathStr = "/";
        }

        auto it = _dataTrees.find(treeName);
        if (it == _dataTrees.end()) {
            return Err<std::pair<TreeLikePtr, DataPath>>(
                "DataBag: tree '" + treeName + "' not found");
        }
        tree = it->second;

        DataPath path = DataPath::parse(pathStr);
        return Ok(std::make_pair(tree, path));
    }

    // Handle @path (relative to main tree)
    if (!spec.empty() && spec[0] == '@') {
        pathStr = spec.substr(1);
        DataPath relPath = DataPath::parse(pathStr);

        DataPath fullPath = relPath.isAbsolute() ? relPath : (_mainDataPath / relPath);
        return Ok(std::make_pair(tree, fullPath));
    }

    // Plain path (relative to current position)
    DataPath path = _mainDataPath / DataPath::parse(spec);
    return Ok(std::make_pair(tree, path));
}

Result<std::string> DataBag::_interpolate(const std::string& str) {
    std::regex refPattern(R"([@$][a-zA-Z_][a-zA-Z0-9_-]*)");
    std::smatch match;
    std::string::const_iterator searchStart = str.cbegin();

    std::string output;
    while (std::regex_search(searchStart, str.cend(), match, refPattern)) {
        output += std::string(searchStart, searchStart + match.position());

        std::string ref = match[0];
        auto resolved = _resolveReference(ref);
        if (resolved) {
            if (auto s = getAs<std::string>(*resolved)) {
                output += *s;
            } else {
                output += ref;
            }
        } else {
            output += ref;
        }

        searchStart = match.suffix().first;
    }
    output += std::string(searchStart, str.cend());

    return Ok(output);
}

std::vector<std::string> DataBag::getTreeNames() const {
    std::vector<std::string> names;
    for (const auto& [name, tree] : _dataTrees) {
        names.push_back(name);
    }
    return names;
}

Result<std::vector<std::string>> DataBag::getTreeChildren(const std::string& treeName, const DataPath& path) {
    auto it = _dataTrees.find(treeName);
    if (it == _dataTrees.end()) {
        return Err<std::vector<std::string>>("DataBag::getTreeChildren: tree '" + treeName + "' not found");
    }
    return it->second->getChildrenNames(path);
}

} // namespace yetty::ymery
