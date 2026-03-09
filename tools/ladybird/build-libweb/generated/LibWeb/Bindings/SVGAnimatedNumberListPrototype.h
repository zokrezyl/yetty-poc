
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAnimatedNumberListPrototype : public JS::Object {
    JS_OBJECT(SVGAnimatedNumberListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAnimatedNumberListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAnimatedNumberListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAnimatedNumberListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(base_val_getter);

    JS_DECLARE_NATIVE_FUNCTION(anim_val_getter);


};


} // namespace Web::Bindings
    