# Coding Conventions

## Naming

- Private class member variables are prefixed with underscore: `_memberVar`

## Class Design

"Heavy/big" classes should follow this pattern:

1. Subclass `Object` and `ObjectFactory<T>`
2. Keep the header/public interface clean and minimal
3. Implementation class (`XXXImpl`) should be a private subclass defined in the `.cpp` file
4. Implement `createImpl()` which creates the impl, calls `init()`, and returns `Result<Ptr>`

Example (from `include/yetty/base/factory.h`):

```cpp
// widget.h
class Widget : public Object, public ObjectFactory<Widget> {
public:
    static Result<Ptr> createImpl(ContextType&, int x);
    virtual void doSomething() = 0;
};

// widget.cpp
class WidgetImpl : public Widget {
    WidgetImpl(int x) : _x(x) {}
    Result<void> init() { if (_x < 0) return Err("bad x"); return Ok(); }
    void doSomething() override { /* ... */ }
    int _x;
};

Result<Widget::Ptr> Widget::createImpl(ContextType&, int x) {
    auto impl = Ptr(new WidgetImpl(x));
    if (auto res = impl->init(); !res) return Err<Ptr>("init failed", res);
    return Ok(std::move(impl));
}
```

## Logging

Use the ytrace logging macros:

```cpp
ydebug("Debug message: %s", value);   // Debug level
yinfo("Info message: %d", count);     // Info level
ywarn("Warning: %s", msg);            // Warning level
yerror("Error: %s", error);           // Error level
ytrace("Trace: entering loop");       // Trace level (most verbose)
```

All logging is disabled by default. See [troubleshooting.md](troubleshooting.md) for how to enable.
