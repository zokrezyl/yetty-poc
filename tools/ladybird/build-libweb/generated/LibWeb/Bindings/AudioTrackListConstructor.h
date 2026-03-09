
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class AudioTrackListConstructor : public JS::NativeFunction {
    JS_OBJECT(AudioTrackListConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(AudioTrackListConstructor);
public:
    explicit AudioTrackListConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioTrackListConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
