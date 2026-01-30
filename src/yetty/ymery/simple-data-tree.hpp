#pragma once

#include "types.hpp"
#include <yaml-cpp/yaml.h>
#include <map>

namespace yetty::ymery {

class SimpleDataTree : public TreeLike {
public:
    // Create empty tree (with empty map as root)
    static Result<TreeLikePtr> create();

    // Create from existing YAML::Node
    static Result<TreeLikePtr> create(YAML::Node node);

    Result<void> init() override;

    Result<std::vector<std::string>> getChildrenNames(const DataPath& path) override;
    Result<Dict> getMetadata(const DataPath& path) override;
    Result<std::vector<std::string>> getMetadataKeys(const DataPath& path) override;
    Result<Value> get(const DataPath& path) override;
    Result<void> set(const DataPath& path, const Value& value) override;
    Result<void> addChild(const DataPath& path, const std::string& name, const Dict& data) override;
    Result<std::string> asTree(const DataPath& path, int depth = -1) override;

    // Register a nested TreeLike at a path
    void registerNested(const DataPath& path, TreeLikePtr tree);

    // Get the underlying YAML::Node (for serialization)
    YAML::Node& root() { return _root; }
    const YAML::Node& root() const { return _root; }

private:
    // Navigate to a node, returns (node, remaining_path)
    Result<std::pair<YAML::Node, DataPath>> _navigate(const DataPath& path);
    DataPath _getPathBeforeRemaining(const DataPath& fullPath, const DataPath& remaining);

    static Dict _yamlToDict(const YAML::Node& node);
    static Value _yamlToValue(const YAML::Node& node);
    static YAML::Node _dictToYaml(const Dict& dict);
    static YAML::Node _valueToYaml(const Value& value);

    YAML::Node _root;
    std::map<std::string, TreeLikePtr> _nestedTrees;
};

} // namespace yetty::ymery
