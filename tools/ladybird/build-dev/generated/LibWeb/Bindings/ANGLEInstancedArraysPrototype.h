
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ANGLEInstancedArraysPrototype : public JS::Object {
    JS_OBJECT(ANGLEInstancedArraysPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ANGLEInstancedArraysPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ANGLEInstancedArraysPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ANGLEInstancedArraysPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(draw_arrays_instanced_angle);
        
    JS_DECLARE_NATIVE_FUNCTION(draw_elements_instanced_angle);
        
    JS_DECLARE_NATIVE_FUNCTION(vertex_attrib_divisor_angle);
        

};


} // namespace Web::Bindings
    