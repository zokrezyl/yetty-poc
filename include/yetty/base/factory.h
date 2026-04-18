#pragma once

#include <memory>
#include <type_traits>
#include <yetty/result.hpp>

namespace yetty {
namespace base {

// Context type for ObjectFactory - acts as compile-time marker to enforce
// intentional implementation of the factory pattern
struct ObjectFactoryContext {};

// ObjectFactory - enforces create protocol for shared_ptr objects
//
// The create protocol:
//   1. Header declares the interface type (e.g., Widget)
//   2. Cpp defines private subclass (e.g., WidgetImpl) with init() method
//   3. createImpl() creates WidgetImpl, calls init(), returns Result<Ptr>
//
// Subclass must implement one of (checked in order):
//   1. static Result<Ptr> createImpl(ContextType&, Args...)
//   2. static Result<Ptr> createImpl(ContextType&)
//   3. static Result<Ptr> createImpl(Args...)
//   4. static Result<Ptr> createImpl()
//
// Example:
//   // widget.h
//   class Widget : public Object, public ObjectFactory<Widget> {
//   public:
//       static Result<Ptr> createImpl(ContextType&, int x);
//       virtual void doSomething() = 0;
//   };
//
//   // widget.cpp
//   class WidgetImpl : public Widget {
//       WidgetImpl(int x) : _x(x) {}
//       Result<void> init() { if (_x < 0) return Err("bad x"); return Ok(); }
//       void doSomething() override { /* ... */ }
//       int _x;
//   };
//   Result<Widget::Ptr> Widget::createImpl(ContextType&, int x) {
//       auto impl = Ptr(new WidgetImpl(x));
//       if (auto res = impl->init(); !res) return Err<Ptr>("init failed", res);
//       return Ok(std::move(impl));
//   }
//
template<typename T, typename ContextT = ObjectFactoryContext>
class ObjectFactory {
public:
    using ContextType = ContextT;
    using Type = T;
    using Ptr = std::shared_ptr<T>;
    using FactoryType = ObjectFactory<Type, ContextType>;

private:
    // SFINAE: check for static Result<Ptr> createImpl(ContextType&, Args...)
    template<typename FType, typename... Args>
    struct HasCreateImplWithContext {
    private:
        template<typename F>
        static auto check(F*) -> decltype(
            F::Type::createImpl(std::declval<typename F::ContextType&>(),
                               std::declval<Args>()...),
            std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<FType>(nullptr)), std::true_type>;
    };

    // SFINAE: check for static Result<Ptr> createImpl(ContextType&)
    template<typename FType>
    struct HasCreateImplWithContextNoArgs {
    private:
        template<typename F>
        static auto check(F*) -> decltype(
            F::Type::createImpl(std::declval<typename F::ContextType&>()),
            std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<FType>(nullptr)), std::true_type>;
    };

    // SFINAE: check for static Result<Ptr> createImpl(Args...)
    template<typename FType, typename... Args>
    struct HasCreateImpl {
    private:
        template<typename F>
        static auto check(F*) -> decltype(
            F::Type::createImpl(std::declval<Args>()...),
            std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<FType>(nullptr)), std::true_type>;
    };

    // SFINAE: check for static Result<Ptr> createImpl()
    template<typename FType>
    struct HasCreateImplNoArgs {
    private:
        template<typename F>
        static auto check(F*) -> decltype(F::Type::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<FType>(nullptr)), std::true_type>;
    };

public:
    template<typename... Args>
    static Result<Ptr> create(Args&&... args) {
        ContextType context;

        if constexpr (HasCreateImplWithContext<FactoryType, Args...>::value) {
            return Type::createImpl(context, std::forward<Args>(args)...);

        } else if constexpr (HasCreateImplWithContextNoArgs<FactoryType>::value) {
            return Type::createImpl(context);

        } else if constexpr (HasCreateImpl<FactoryType, Args...>::value) {
            return Type::createImpl(std::forward<Args>(args)...);

        } else if constexpr (HasCreateImplNoArgs<FactoryType>::value) {
            return Type::createImpl();

        } else {
            // clang-format off
            static_assert(sizeof(T) == 0,
                "ObjectFactory: No createImpl found.\n"
                "Subclass must implement one of (checked in order):\n"
                "  1. static Result<Ptr> createImpl(ContextType&, Args...)\n"
                "  2. static Result<Ptr> createImpl(ContextType&)\n"
                "  3. static Result<Ptr> createImpl(Args...)\n"
                "  4. static Result<Ptr> createImpl()\n"
                "\n"
                "The createImpl should:\n"
                "  - Create an instance of the implementation subclass\n"
                "  - Call init() on it (which returns Result<void>)\n"
                "  - Return Result<Ptr> based on init() success\n");
            // clang-format on
            return Err<Ptr>("unreachable");
        }
    }
};

// Singleton context type
struct SingletonContext {};

// Singleton - global singleton (one instance per process)
//
// Subclass must implement one of:
//   1. static Result<Ptr> createImpl(ContextType&)
//   2. static Result<Ptr> createImpl()
//
// instance() returns Result<Ptr> - if creation fails, the error is cached
// and returned on all subsequent calls.
//
template<typename T, typename ContextT = SingletonContext>
class Singleton {
public:
    using ContextType = ContextT;
    using Type = T;
    using Ptr = std::shared_ptr<T>;
    using SingletonType = Singleton<Type, ContextType>;

private:
    template<typename SType>
    struct HasCreateImplWithContext {
    private:
        template<typename S>
        static auto check(S*) -> decltype(
            S::Type::createImpl(std::declval<typename S::ContextType&>()),
            std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<SType>(nullptr)), std::true_type>;
    };

    template<typename SType>
    struct HasCreateImplNoArgs {
    private:
        template<typename S>
        static auto check(S*) -> decltype(S::Type::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<SType>(nullptr)), std::true_type>;
    };

public:
    static Result<Ptr> instance() {
        static Result<Ptr> _instance = []() -> Result<Ptr> {
            static ContextType context;

            if constexpr (HasCreateImplWithContext<SingletonType>::value) {
                auto result = Type::createImpl(context);
                if (!result) {
                    return Err<Ptr>("Singleton creation failed", result);
                }
                return result;

            } else if constexpr (HasCreateImplNoArgs<SingletonType>::value) {
                auto result = Type::createImpl();
                if (!result) {
                    return Err<Ptr>("Singleton creation failed", result);
                }
                return result;

            } else {
                // clang-format off
                static_assert(sizeof(T) == 0,
                    "Singleton: No createImpl found.\n"
                    "Subclass must implement one of:\n"
                    "  1. static Result<Ptr> createImpl(ContextType&)\n"
                    "  2. static Result<Ptr> createImpl()\n");
                // clang-format on
                return Err<Ptr>("unreachable");
            }
        }();
        return _instance;
    }

protected:
    Singleton() = default;
};

// ThreadSingleton context type
struct ThreadSingletonContext {};

// ThreadSingleton - one instance per thread
//
// Subclass must implement one of:
//   1. static Result<Ptr> createImpl(ContextType&)
//   2. static Result<Ptr> createImpl()
//
// instance() returns Result<Ptr> - if creation fails, the error is cached
// per-thread and returned on all subsequent calls from that thread.
//
template<typename T, typename ContextT = ThreadSingletonContext>
class ThreadSingleton {
public:
    using ContextType = ContextT;
    using Type = T;
    using Ptr = std::shared_ptr<T>;
    using ThreadSingletonType = ThreadSingleton<Type, ContextType>;

private:
    template<typename SType>
    struct HasCreateImplWithContext {
    private:
        template<typename S>
        static auto check(S*) -> decltype(
            S::Type::createImpl(std::declval<typename S::ContextType&>()),
            std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<SType>(nullptr)), std::true_type>;
    };

    template<typename SType>
    struct HasCreateImplNoArgs {
    private:
        template<typename S>
        static auto check(S*) -> decltype(S::Type::createImpl(), std::true_type{});
        template<typename>
        static std::false_type check(...);
    public:
        static constexpr bool value =
            std::is_same_v<decltype(check<SType>(nullptr)), std::true_type>;
    };

public:
    static Result<Ptr> instance() {
        static thread_local Result<Ptr> _instance = []() -> Result<Ptr> {
            static thread_local ContextType context;

            if constexpr (HasCreateImplWithContext<ThreadSingletonType>::value) {
                auto result = Type::createImpl(context);
                if (!result) {
                    return Err<Ptr>("ThreadSingleton creation failed", result);
                }
                return result;

            } else if constexpr (HasCreateImplNoArgs<ThreadSingletonType>::value) {
                auto result = Type::createImpl();
                if (!result) {
                    return Err<Ptr>("ThreadSingleton creation failed", result);
                }
                return result;

            } else {
                // clang-format off
                static_assert(sizeof(T) == 0,
                    "ThreadSingleton: No createImpl found.\n"
                    "Subclass must implement one of:\n"
                    "  1. static Result<Ptr> createImpl(ContextType&)\n"
                    "  2. static Result<Ptr> createImpl()\n");
                // clang-format on
                return Err<Ptr>("unreachable");
            }
        }();
        return _instance;
    }

protected:
    ThreadSingleton() = default;
};

} // namespace base
} // namespace yetty
