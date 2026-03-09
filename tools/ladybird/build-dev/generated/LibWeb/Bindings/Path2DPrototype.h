
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class Path2DPrototype : public JS::Object {
    JS_OBJECT(Path2DPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(Path2DPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit Path2DPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~Path2DPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add_path);
        
    JS_DECLARE_NATIVE_FUNCTION(close_path);
        
    JS_DECLARE_NATIVE_FUNCTION(move_to);
        
    JS_DECLARE_NATIVE_FUNCTION(line_to);
        
    JS_DECLARE_NATIVE_FUNCTION(quadratic_curve_to);
        
    JS_DECLARE_NATIVE_FUNCTION(bezier_curve_to);
        
    JS_DECLARE_NATIVE_FUNCTION(arc_to);
        
    JS_DECLARE_NATIVE_FUNCTION(rect);
        
    JS_DECLARE_NATIVE_FUNCTION(round_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(arc);
        
    JS_DECLARE_NATIVE_FUNCTION(ellipse);
        

};


} // namespace Web::Bindings
    