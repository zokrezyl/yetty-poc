#pragma once

#include "types.h"
#include <yetty/result.hpp>
#include <memory>
#include <atomic>
#include <type_traits>

namespace yetty {
namespace base {

class Object : public std::enable_shared_from_this<Object> {
public:
    using Ptr = std::shared_ptr<Object>;

    virtual ~Object() = default;

    // Public entry point: guards against double-shutdown, then calls onShutdown().
    // Call this on children before releasing shared_ptrs.
    Result<void> shutdown() {
        if (_shutdownCalled) return Ok();
        _shutdownCalled = true;
        return onShutdown();
    }

    ObjectId id() const { return _id; }

    virtual const char* typeName() const { return "Object"; }

    // Cast shared_ptr to any derived type - all share same control block
    template<typename T>
    std::shared_ptr<T> sharedAs() {
        static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    template<typename T>
    std::shared_ptr<const T> sharedAs() const {
        static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
        return std::static_pointer_cast<const T>(shared_from_this());
    }

    // Prevent copying/moving - use shared_ptr
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

protected:
    Object() : _id(nextId()) {}

    // Override in subclasses to do cleanup while shared_ptr is still alive.
    virtual Result<void> onShutdown() { return Ok(); }

private:
    bool _shutdownCalled = false;
    static ObjectId nextId() {
        static std::atomic<ObjectId> _counter{1};
        return _counter++;
    }

    ObjectId _id;
};

} // namespace base
} // namespace yetty
