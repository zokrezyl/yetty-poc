
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGForeignObjectElementPrototype : public JS::Object {
    JS_OBJECT(SVGForeignObjectElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGForeignObjectElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGForeignObjectElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGForeignObjectElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);


};


} // namespace Web::Bindings
    