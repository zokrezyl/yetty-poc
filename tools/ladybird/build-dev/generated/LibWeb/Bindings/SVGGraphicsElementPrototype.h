
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGGraphicsElementPrototype : public JS::Object {
    JS_OBJECT(SVGGraphicsElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGGraphicsElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGGraphicsElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGGraphicsElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_b_box);
        
    JS_DECLARE_NATIVE_FUNCTION(get_ctm);
        
    JS_DECLARE_NATIVE_FUNCTION(get_screen_ctm);
        
    JS_DECLARE_NATIVE_FUNCTION(transform_getter);


};


} // namespace Web::Bindings
    