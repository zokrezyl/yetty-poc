
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimatedIntegerPrototype : public JS::Object {
    JS_OBJECT(SVGAnimatedIntegerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimatedIntegerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimatedIntegerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedIntegerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_val_getter);

    JS_DECLARE_NATIVE_FUNCTION(base_val_setter);

    JS_DECLARE_NATIVE_FUNCTION(anim_val_getter);


};


} // namespace Web::Bindings
    