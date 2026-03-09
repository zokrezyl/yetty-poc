
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimatedRectPrototype : public JS::Object {
    JS_OBJECT(SVGAnimatedRectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimatedRectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimatedRectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedRectPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_val_getter);

    JS_DECLARE_NATIVE_FUNCTION(anim_val_getter);


};


} // namespace Web::Bindings
    