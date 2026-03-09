
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimatedTransformListPrototype : public JS::Object {
    JS_OBJECT(SVGAnimatedTransformListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimatedTransformListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimatedTransformListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedTransformListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_val_getter);

    JS_DECLARE_NATIVE_FUNCTION(anim_val_getter);


};


} // namespace Web::Bindings
    