
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTransformPrototype : public JS::Object {
    JS_OBJECT(SVGTransformPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTransformPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTransformPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTransformPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_translate);
        
    JS_DECLARE_NATIVE_FUNCTION(set_scale);
        
    JS_DECLARE_NATIVE_FUNCTION(set_rotate);
        
    JS_DECLARE_NATIVE_FUNCTION(set_skew_x);
        
    JS_DECLARE_NATIVE_FUNCTION(set_skew_y);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(angle_getter);


};


} // namespace Web::Bindings
    