
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnimationPlaybackEventPrototype : public JS::Object {
    JS_OBJECT(AnimationPlaybackEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnimationPlaybackEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnimationPlaybackEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnimationPlaybackEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(current_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_time_getter);


};


} // namespace Web::Bindings
    