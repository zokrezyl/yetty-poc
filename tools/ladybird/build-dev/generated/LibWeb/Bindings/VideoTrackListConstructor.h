
#pragma once

#include <LibJS/Runtime/NativeFunction.h>

namespace Web::Bindings {

class VideoTrackListConstructor : public JS::NativeFunction {
    JS_OBJECT(VideoTrackListConstructor, JS::NativeFunction);
    GC_DECLARE_ALLOCATOR(VideoTrackListConstructor);
public:
    explicit VideoTrackListConstructor(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VideoTrackListConstructor() override;

    virtual JS::ThrowCompletionOr<JS::Value> call() override;
    virtual JS::ThrowCompletionOr<GC::Ref<JS::Object>> construct(JS::FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

};

} // namespace Web::Bindings
