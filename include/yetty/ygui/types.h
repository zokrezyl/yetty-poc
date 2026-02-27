#pragma once

#include <yetty/result.hpp>
#include <string>
#include <vector>
#include <map>
#include <any>
#include <optional>
#include <memory>

namespace yetty::ygui {

// Forward declarations
class TreeLike;

// Value type for dynamic data
using Value = std::any;
using Dict = std::map<std::string, Value>;
using List = std::vector<Value>;

// Helper to get value from std::any
template<typename T>
std::optional<T> getAs(const Value& v) {
    try {
        return std::any_cast<T>(v);
    } catch (const std::bad_any_cast&) {
        return std::nullopt;
    }
}

// DataPath - hierarchical path for navigating data
class DataPath {
public:
    DataPath() = default;
    explicit DataPath(const std::string& path);
    explicit DataPath(const std::vector<std::string>& components);

    // Status
    bool isRoot() const { return path_.empty(); }
    bool isAbsolute() const { return isAbsolute_; }

    // Components
    const std::vector<std::string>& asList() const { return path_; }
    std::string filename() const;
    std::vector<std::string> namespace_() const;
    DataPath dirname() const;

    // Operations
    DataPath operator/(const std::string& component) const;
    DataPath operator/(const DataPath& other) const;
    bool startsWith(const DataPath& other) const;

    // Comparison
    bool operator==(const DataPath& other) const;
    bool operator!=(const DataPath& other) const { return !(*this == other); }

    // String conversion
    std::string toString() const;

    // Parse from string (handles /, /abs, rel, ../parent)
    static DataPath parse(const std::string& pathStr);

    // Create root path
    static DataPath root() { return DataPath(); }

private:
    std::vector<std::string> path_;
    bool isAbsolute_ = false;
};

// TreeLike - abstract interface for hierarchical data access
class TreeLike {
public:
    virtual ~TreeLike() = default;

    // Child navigation
    virtual Result<std::vector<std::string>> getChildrenNames(const DataPath& path) = 0;

    // Metadata access
    virtual Result<Dict> getMetadata(const DataPath& path) = 0;
    virtual Result<std::vector<std::string>> getMetadataKeys(const DataPath& path) = 0;

    // Value access (path's last component is the key)
    virtual Result<Value> get(const DataPath& path) = 0;
    virtual Result<void> set(const DataPath& path, const Value& value) = 0;

    // Child management
    virtual Result<void> addChild(const DataPath& path, const std::string& name, const Dict& data) = 0;

    // Debug
    virtual Result<std::string> asTree(const DataPath& path, int depth = -1) = 0;

    // Lifecycle
    virtual Result<void> init() { return Ok(); }
    virtual Result<void> dispose() { return Ok(); }
};

// Shared pointer for TreeLike
using TreeLikePtr = std::shared_ptr<TreeLike>;

} // namespace yetty::ygui
