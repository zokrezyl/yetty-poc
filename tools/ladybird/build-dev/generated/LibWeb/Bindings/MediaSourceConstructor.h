
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class MediaSourceConstructor : public JS::NativeFunction {
    JS_OBJECT(MediaSourceConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(MediaSourceConstructor);
public:
    explicit MediaSourceConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaSourceConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(can_construct_in_dedicated_worker_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_type_supported);

};

} // namespace Web::Bindings
