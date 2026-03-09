
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class OfflineAudioContextConstructor : public JS::NativeFunction {
    JS_OBJECT(OfflineAudioContextConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(OfflineAudioContextConstructor);
public:
    explicit OfflineAudioContextConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OfflineAudioContextConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct0(JS::FunctionObject& new_target);

    JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct1(JS::FunctionObject& new_target);

};

} // namespace Web::Bindings
