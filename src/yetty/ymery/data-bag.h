#pragma once

#include "types.h"
#include <yetty/base/object.h>
#include <map>
#include <memory>
#include <regex>

namespace yetty::ymery {

class Dispatcher;

// DataBag - bridges widgets to data trees with reference resolution
class DataBag : public base::Object {
public:
    using Ptr = std::shared_ptr<DataBag>;

    // Factory method
    static Result<Ptr> create(
        std::shared_ptr<Dispatcher> dispatcher,
        std::map<std::string, TreeLikePtr> dataTrees,
        const std::string& mainDataKey,
        const DataPath& mainDataPath,
        const Dict& statics
    );

    // Create child DataBag with inherited context
    Result<Ptr> inherit(
        const std::string& dataPathSpec,
        const Dict& statics
    );

    // Data access
    Result<Value> get(const std::string& key, const Value& defaultValue = {});
    Result<void> set(const std::string& key, const Value& value);

    // Static config access (no reference resolution)
    Result<Value> getStatic(const std::string& key, const Value& defaultValue = {});

    // Metadata access
    Result<Dict> getMetadata();
    Result<std::vector<std::string>> getMetadataKeys();
    Result<std::vector<std::string>> getChildrenNames();

    // Path info
    Result<DataPath> getDataPath();
    Result<std::string> getDataPathStr();
    std::string mainDataKey() const { return _mainDataKey; }

    // Tree browsing
    std::vector<std::string> getTreeNames() const;
    Result<std::vector<std::string>> getTreeChildren(const std::string& treeName, const DataPath& path);

    // Child management
    Result<void> addChild(const Dict& childSpec);

    // Unique string ID for ImGui
    std::string uidStr() const { return std::to_string(id()); }

    const char* typeName() const override { return "DataBag"; }

private:
    DataBag() = default;

    Result<void> _init();

    // Reference resolution
    Result<Value> _resolveReference(const std::string& refStr);
    Result<std::pair<TreeLikePtr, DataPath>> _parseDataPathSpec(const std::string& spec);
    bool _isReference(const std::string& str);
    bool _hasInterpolation(const std::string& str);
    Result<std::string> _interpolate(const std::string& str);

    std::shared_ptr<Dispatcher> _dispatcher;
    std::map<std::string, TreeLikePtr> _dataTrees;
    TreeLikePtr _mainDataTree;
    std::string _mainDataKey;
    DataPath _mainDataPath;
    Dict _statics;
};

using DataBagPtr = std::shared_ptr<DataBag>;

} // namespace yetty::ymery
