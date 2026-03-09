
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMMatrixReadOnlyPrototype : public JS::Object {
    JS_OBJECT(DOMMatrixReadOnlyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMMatrixReadOnlyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMMatrixReadOnlyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMMatrixReadOnlyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(translate);
        
    JS_DECLARE_NATIVE_FUNCTION(scale);
        
    JS_DECLARE_NATIVE_FUNCTION(scale_non_uniform);
        
    JS_DECLARE_NATIVE_FUNCTION(scale3d);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate_from_vector);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate_axis_angle);
        
    JS_DECLARE_NATIVE_FUNCTION(skew_x);
        
    JS_DECLARE_NATIVE_FUNCTION(skew_y);
        
    JS_DECLARE_NATIVE_FUNCTION(multiply);
        
    JS_DECLARE_NATIVE_FUNCTION(flip_x);
        
    JS_DECLARE_NATIVE_FUNCTION(flip_y);
        
    JS_DECLARE_NATIVE_FUNCTION(inverse);
        
    JS_DECLARE_NATIVE_FUNCTION(transform_point);
        
    JS_DECLARE_NATIVE_FUNCTION(to_float32_array);
        
    JS_DECLARE_NATIVE_FUNCTION(to_float64_array);
        
    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(a_getter);

    JS_DECLARE_NATIVE_FUNCTION(b_getter);

    JS_DECLARE_NATIVE_FUNCTION(c_getter);

    JS_DECLARE_NATIVE_FUNCTION(d_getter);

    JS_DECLARE_NATIVE_FUNCTION(e_getter);

    JS_DECLARE_NATIVE_FUNCTION(f_getter);

    JS_DECLARE_NATIVE_FUNCTION(m11_getter);

    JS_DECLARE_NATIVE_FUNCTION(m12_getter);

    JS_DECLARE_NATIVE_FUNCTION(m13_getter);

    JS_DECLARE_NATIVE_FUNCTION(m14_getter);

    JS_DECLARE_NATIVE_FUNCTION(m21_getter);

    JS_DECLARE_NATIVE_FUNCTION(m22_getter);

    JS_DECLARE_NATIVE_FUNCTION(m23_getter);

    JS_DECLARE_NATIVE_FUNCTION(m24_getter);

    JS_DECLARE_NATIVE_FUNCTION(m31_getter);

    JS_DECLARE_NATIVE_FUNCTION(m32_getter);

    JS_DECLARE_NATIVE_FUNCTION(m33_getter);

    JS_DECLARE_NATIVE_FUNCTION(m34_getter);

    JS_DECLARE_NATIVE_FUNCTION(m41_getter);

    JS_DECLARE_NATIVE_FUNCTION(m42_getter);

    JS_DECLARE_NATIVE_FUNCTION(m43_getter);

    JS_DECLARE_NATIVE_FUNCTION(m44_getter);

    JS_DECLARE_NATIVE_FUNCTION(is2d_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_identity_getter);


};


} // namespace Web::Bindings
    