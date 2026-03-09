
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEFloodElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEFloodElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEFloodElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEFloodElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFloodElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);


};


} // namespace Web::Bindings
    