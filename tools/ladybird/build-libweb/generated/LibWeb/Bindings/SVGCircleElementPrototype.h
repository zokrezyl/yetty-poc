
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGCircleElementPrototype : public JS::Object {
    JS_OBJECT(SVGCircleElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGCircleElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGCircleElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGCircleElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cx_getter);

    JS_DECLARE_NATIVE_FUNCTION(cy_getter);

    JS_DECLARE_NATIVE_FUNCTION(r_getter);


};


} // namespace Web::Bindings
    