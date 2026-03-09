
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimatedNumberPrototype : public JS::Object {
    JS_OBJECT(SVGAnimatedNumberPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimatedNumberPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimatedNumberPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedNumberPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_val_getter);

    JS_DECLARE_NATIVE_FUNCTION(base_val_setter);

    JS_DECLARE_NATIVE_FUNCTION(anim_val_getter);


};


} // namespace Web::Bindings
    