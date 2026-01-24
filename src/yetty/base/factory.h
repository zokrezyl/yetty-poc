#pragma once

#include <memory>
#include <type_traits>

namespace yetty {
namespace base {

// Context passed to factory create methods
struct FactoryContext {};

// ObjectFactory - enforces create pattern for shared_ptr objects
// Subclass must implement: static Ptr createImpl(Args...)
template<typename T>
class ObjectFactory {
public:
    using Type = T;
    using Ptr = std::shared_ptr<T>;

private:
    // Check for static createImpl with args
    template<typename FType, typename... Args>
    struct HasCreateImpl {
    private:
        template<typename U>
        static auto check(int) -> decltype(U::createImpl(std::declval<Args>()...), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value = std::is_same_v<decltype(check<FType>(0)), std::true_type>;
    };

    template<typename FType>
    struct HasCreateImplNoArgs {
    private:
        template<typename U>
        static auto check(int) -> decltype(U::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value = std::is_same_v<decltype(check<FType>(0)), std::true_type>;
    };

public:
    template<typename... Args>
    static Ptr create(Args&&... args) {
        if constexpr (HasCreateImpl<T, Args...>::value) {
            return T::createImpl(std::forward<Args>(args)...);
        } else if constexpr (HasCreateImplNoArgs<T>::value) {
            return T::createImpl();
        } else {
            static_assert(sizeof(T) == 0,
                "ObjectFactory requires static Ptr createImpl(...) method in subclass");
            return nullptr;
        }
    }
};

// Singleton - global singleton (one instance per process)
template<typename T>
class Singleton {
public:
    using Type = T;
    using Ptr = std::shared_ptr<T>;

private:
    template<typename FType>
    struct HasCreateImpl {
    private:
        template<typename U>
        static auto check(int) -> decltype(U::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value = std::is_same_v<decltype(check<FType>(0)), std::true_type>;
    };

public:
    static Ptr instance() {
        static Ptr _instance = []() {
            if constexpr (HasCreateImpl<T>::value) {
                return T::createImpl();
            } else {
                return Ptr(new T());
            }
        }();
        return _instance;
    }

protected:
    Singleton() = default;
};

// ThreadSingleton - one instance per thread
template<typename T>
class ThreadSingleton {
public:
    using Type = T;
    using Ptr = std::shared_ptr<T>;

private:
    template<typename FType>
    struct HasCreateImpl {
    private:
        template<typename U>
        static auto check(int) -> decltype(U::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value = std::is_same_v<decltype(check<FType>(0)), std::true_type>;
    };

public:
    static Ptr instance() {
        static thread_local Ptr _instance = []() {
            if constexpr (HasCreateImpl<T>::value) {
                return T::createImpl();
            } else {
                return Ptr(new T());
            }
        }();
        return _instance;
    }

protected:
    ThreadSingleton() = default;
};

} // namespace base
} // namespace yetty
