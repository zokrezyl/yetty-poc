
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEImageElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEImageElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEImageElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEImageElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEImageElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    