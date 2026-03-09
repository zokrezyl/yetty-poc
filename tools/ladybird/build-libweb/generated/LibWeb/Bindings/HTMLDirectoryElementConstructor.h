
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class HTMLDirectoryElementConstructor : public JS::NativeFunction {
    JS_OBJECT(HTMLDirectoryElementConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(HTMLDirectoryElementConstructor);
public:
    explicit HTMLDirectoryElementConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDirectoryElementConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
