
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AnimationEventPrototype : public JS::Object {
    JS_OBJECT(AnimationEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AnimationEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AnimationEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AnimationEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(animation_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(elapsed_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(pseudo_element_getter);


};


} // namespace Web::Bindings
    