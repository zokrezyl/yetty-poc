
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGUseElementPrototype : public JS::Object {
    JS_OBJECT(SVGUseElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGUseElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGUseElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGUseElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(instance_root_getter);

    JS_DECLARE_NATIVE_FUNCTION(animated_instance_root_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    