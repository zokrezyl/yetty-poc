#pragma once

#include "types.h"
#include <yaml-cpp/yaml.h>
#include <map>

namespace yetty::ymery {

// Native tree node - replaces YAML::Node to avoid yaml-cpp mutation bugs
struct NativeNode {
    Dict metadata;
    std::map<std::string, std::shared_ptr<NativeNode>> children;
};
using NativeNodePtr = std::shared_ptr<NativeNode>;

// DataTree - tree with explicit children/metadata structure
// Each node has { children: {...}, metadata: {...} } format
// Internally uses native C++ structures (not YAML::Node) for safe mutation
class DataTree : public TreeLike {
public:
    static Result<TreeLikePtr> create();
    static Result<TreeLikePtr> create(YAML::Node node);
    static Result<TreeLikePtr> create(const Dict& data);

    Result<void> init() override;

    Result<std::vector<std::string>> getChildrenNames(const DataPath& path) override;
    Result<Dict> getMetadata(const DataPath& path) override;
    Result<std::vector<std::string>> getMetadataKeys(const DataPath& path) override;
    Result<Value> get(const DataPath& path) override;
    Result<void> set(const DataPath& path, const Value& value) override;
    Result<void> addChild(const DataPath& path, const std::string& name, const Dict& data) override;
    Result<std::string> asTree(const DataPath& path, int depth = -1) override;

    void registerNested(const DataPath& path, TreeLikePtr tree);

private:
    // Navigate to a node by path; returns (node, remaining path for nested trees)
    Result<std::pair<NativeNodePtr, DataPath>> _navigate(const DataPath& path);
    NativeNodePtr _ensurePath(const DataPath& path);

    // YAML conversion (for create(YAML::Node) and asTree)
    static NativeNodePtr _yamlToNative(const YAML::Node& node);
    static NativeNodePtr _dictArgToNative(const Dict& data);
    static YAML::Node _nativeToYaml(const NativeNodePtr& node);
    static Dict _yamlToDict(const YAML::Node& node);
    static Value _yamlToValue(const YAML::Node& node);
    static YAML::Node _dictToYaml(const Dict& dict);
    static YAML::Node _valueToYaml(const Value& value);

    NativeNodePtr _root;
    std::map<std::string, TreeLikePtr> _nestedTrees;
};

} // namespace yetty::ymery
