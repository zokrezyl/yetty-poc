#pragma once

#include <expected>
#include <string>
#include <memory>
#include <source_location>

namespace yetty {

// Error with chaining and source location
class Error {
public:
    explicit Error(std::string msg, std::source_location loc = std::source_location::current())
        : _msg(std::move(msg)), _loc(loc) {}

    Error(std::string msg, Error prev, std::source_location loc = std::source_location::current())
        : _msg(std::move(msg)), _prev(std::make_unique<Error>(std::move(prev))), _loc(loc) {}

    Error(const Error& other)
        : _msg(other._msg), _loc(other._loc) {
        if (other._prev) _prev = std::make_unique<Error>(*other._prev);
    }

    Error& operator=(const Error& other) {
        if (this != &other) {
            _msg = other._msg;
            _loc = other._loc;
            _prev = other._prev ? std::make_unique<Error>(*other._prev) : nullptr;
        }
        return *this;
    }

    Error(Error&&) = default;
    Error& operator=(Error&&) = default;

    [[nodiscard]] const std::string& message() const { return _msg; }

    [[nodiscard]] std::string to_string() const {
        std::string result = _msg;
        result += " [";
        result += _loc.file_name();
        result += ":";
        result += std::to_string(_loc.line());
        result += "]";
        if (_prev) {
            result += " <- ";
            result += _prev->to_string();
        }
        return result;
    }

private:
    std::string _msg;
    std::unique_ptr<Error> _prev;
    std::source_location _loc;
};

// Result type using std::expected
template<typename T>
using Result = std::expected<T, Error>;

// Helper functions for creating results
template<typename T>
[[nodiscard]] inline Result<T> Ok(T value) {
    return Result<T>(std::move(value));
}

[[nodiscard]] inline Result<void> Ok() {
    return Result<void>();
}

template<typename T = void>
[[nodiscard]] inline std::unexpected<Error> Err(std::string msg, std::source_location loc = std::source_location::current()) {
    return std::unexpected(Error(std::move(msg), loc));
}

template<typename T, typename U>
[[nodiscard]] inline std::unexpected<Error> Err(std::string msg, const Result<U>& prev, std::source_location loc = std::source_location::current()) {
    if (!prev.has_value()) {
        return std::unexpected(Error(std::move(msg), prev.error(), loc));
    }
    return std::unexpected(Error(std::move(msg), loc));
}

// Get error message from result
template<typename T>
[[nodiscard]] inline std::string error_msg(const Result<T>& res) {
    return res.has_value() ? "" : res.error().to_string();
}

} // namespace yetty
