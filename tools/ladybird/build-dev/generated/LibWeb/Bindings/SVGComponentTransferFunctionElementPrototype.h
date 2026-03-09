
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGComponentTransferFunctionElementPrototype : public JS::Object {
    JS_OBJECT(SVGComponentTransferFunctionElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGComponentTransferFunctionElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGComponentTransferFunctionElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGComponentTransferFunctionElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(table_values_getter);

    JS_DECLARE_NATIVE_FUNCTION(slope_getter);

    JS_DECLARE_NATIVE_FUNCTION(intercept_getter);

    JS_DECLARE_NATIVE_FUNCTION(amplitude_getter);

    JS_DECLARE_NATIVE_FUNCTION(exponent_getter);

    JS_DECLARE_NATIVE_FUNCTION(offset_getter);


};


} // namespace Web::Bindings
    