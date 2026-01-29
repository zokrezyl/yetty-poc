#include "types.hpp"
#include <sstream>
#include <algorithm>

namespace yetty::ymery {

DataPath::DataPath(const std::string& path) {
    *this = parse(path);
}

DataPath::DataPath(const std::vector<std::string>& components)
    : path_(components), isAbsolute_(false) {}

std::string DataPath::filename() const {
    return path_.empty() ? "" : path_.back();
}

std::vector<std::string> DataPath::namespace_() const {
    if (path_.size() <= 1) return {};
    return std::vector<std::string>(path_.begin(), path_.end() - 1);
}

DataPath DataPath::dirname() const {
    if (path_.empty()) return *this;
    DataPath result;
    result.path_ = std::vector<std::string>(path_.begin(), path_.end() - 1);
    result.isAbsolute_ = isAbsolute_;
    return result;
}

DataPath DataPath::operator/(const std::string& component) const {
    DataPath result = *this;
    if (!component.empty() && component != ".") {
        if (component == "..") {
            if (!result.path_.empty()) {
                result.path_.pop_back();
            }
        } else {
            result.path_.push_back(component);
        }
    }
    return result;
}

DataPath DataPath::operator/(const DataPath& other) const {
    if (other.isAbsolute_) {
        return other;
    }
    DataPath result = *this;
    for (const auto& comp : other.path_) {
        result = result / comp;
    }
    return result;
}

bool DataPath::startsWith(const DataPath& other) const {
    if (other.path_.size() > path_.size()) return false;
    for (size_t i = 0; i < other.path_.size(); ++i) {
        if (path_[i] != other.path_[i]) return false;
    }
    return true;
}

bool DataPath::operator==(const DataPath& other) const {
    return path_ == other.path_ && isAbsolute_ == other.isAbsolute_;
}

std::string DataPath::toString() const {
    std::ostringstream oss;
    if (isAbsolute_) oss << "/";
    for (size_t i = 0; i < path_.size(); ++i) {
        if (i > 0) oss << "/";
        oss << path_[i];
    }
    return path_.empty() && isAbsolute_ ? "/" : oss.str();
}

DataPath DataPath::parse(const std::string& pathStr) {
    DataPath result;

    if (pathStr.empty()) {
        return result;
    }

    std::string s = pathStr;
    if (s[0] == '/') {
        result.isAbsolute_ = true;
        s = s.substr(1);
    }

    if (s.empty()) {
        return result;
    }

    std::istringstream iss(s);
    std::string component;
    while (std::getline(iss, component, '/')) {
        if (component.empty() || component == ".") {
            continue;
        }
        if (component == "..") {
            if (!result.path_.empty()) {
                result.path_.pop_back();
            }
        } else {
            result.path_.push_back(component);
        }
    }

    return result;
}

} // namespace yetty::ymery
