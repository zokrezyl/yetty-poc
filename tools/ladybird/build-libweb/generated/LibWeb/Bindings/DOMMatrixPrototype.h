
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMMatrixPrototype : public JS::Object {
    JS_OBJECT(DOMMatrixPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMMatrixPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMMatrixPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMMatrixPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(multiply_self);
        
    JS_DECLARE_NATIVE_FUNCTION(pre_multiply_self);
        
    JS_DECLARE_NATIVE_FUNCTION(translate_self);
        
    JS_DECLARE_NATIVE_FUNCTION(scale_self);
        
    JS_DECLARE_NATIVE_FUNCTION(scale3d_self);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate_self);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate_from_vector_self);
        
    JS_DECLARE_NATIVE_FUNCTION(rotate_axis_angle_self);
        
    JS_DECLARE_NATIVE_FUNCTION(skew_x_self);
        
    JS_DECLARE_NATIVE_FUNCTION(skew_y_self);
        
    JS_DECLARE_NATIVE_FUNCTION(invert_self);
        
    JS_DECLARE_NATIVE_FUNCTION(set_matrix_value);
        
    JS_DECLARE_NATIVE_FUNCTION(a_getter);

    JS_DECLARE_NATIVE_FUNCTION(a_setter);

    JS_DECLARE_NATIVE_FUNCTION(b_getter);

    JS_DECLARE_NATIVE_FUNCTION(b_setter);

    JS_DECLARE_NATIVE_FUNCTION(c_getter);

    JS_DECLARE_NATIVE_FUNCTION(c_setter);

    JS_DECLARE_NATIVE_FUNCTION(d_getter);

    JS_DECLARE_NATIVE_FUNCTION(d_setter);

    JS_DECLARE_NATIVE_FUNCTION(e_getter);

    JS_DECLARE_NATIVE_FUNCTION(e_setter);

    JS_DECLARE_NATIVE_FUNCTION(f_getter);

    JS_DECLARE_NATIVE_FUNCTION(f_setter);

    JS_DECLARE_NATIVE_FUNCTION(m11_getter);

    JS_DECLARE_NATIVE_FUNCTION(m11_setter);

    JS_DECLARE_NATIVE_FUNCTION(m12_getter);

    JS_DECLARE_NATIVE_FUNCTION(m12_setter);

    JS_DECLARE_NATIVE_FUNCTION(m13_getter);

    JS_DECLARE_NATIVE_FUNCTION(m13_setter);

    JS_DECLARE_NATIVE_FUNCTION(m14_getter);

    JS_DECLARE_NATIVE_FUNCTION(m14_setter);

    JS_DECLARE_NATIVE_FUNCTION(m21_getter);

    JS_DECLARE_NATIVE_FUNCTION(m21_setter);

    JS_DECLARE_NATIVE_FUNCTION(m22_getter);

    JS_DECLARE_NATIVE_FUNCTION(m22_setter);

    JS_DECLARE_NATIVE_FUNCTION(m23_getter);

    JS_DECLARE_NATIVE_FUNCTION(m23_setter);

    JS_DECLARE_NATIVE_FUNCTION(m24_getter);

    JS_DECLARE_NATIVE_FUNCTION(m24_setter);

    JS_DECLARE_NATIVE_FUNCTION(m31_getter);

    JS_DECLARE_NATIVE_FUNCTION(m31_setter);

    JS_DECLARE_NATIVE_FUNCTION(m32_getter);

    JS_DECLARE_NATIVE_FUNCTION(m32_setter);

    JS_DECLARE_NATIVE_FUNCTION(m33_getter);

    JS_DECLARE_NATIVE_FUNCTION(m33_setter);

    JS_DECLARE_NATIVE_FUNCTION(m34_getter);

    JS_DECLARE_NATIVE_FUNCTION(m34_setter);

    JS_DECLARE_NATIVE_FUNCTION(m41_getter);

    JS_DECLARE_NATIVE_FUNCTION(m41_setter);

    JS_DECLARE_NATIVE_FUNCTION(m42_getter);

    JS_DECLARE_NATIVE_FUNCTION(m42_setter);

    JS_DECLARE_NATIVE_FUNCTION(m43_getter);

    JS_DECLARE_NATIVE_FUNCTION(m43_setter);

    JS_DECLARE_NATIVE_FUNCTION(m44_getter);

    JS_DECLARE_NATIVE_FUNCTION(m44_setter);


};


} // namespace Web::Bindings
    