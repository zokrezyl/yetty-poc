
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnimationTimelinePrototype : public JS::Object {
    JS_OBJECT(AnimationTimelinePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnimationTimelinePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnimationTimelinePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnimationTimelinePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(current_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(duration_getter);


};


} // namespace Web::Bindings
    