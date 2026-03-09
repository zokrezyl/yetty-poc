
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGLinearGradientElementPrototype : public JS::Object {
    JS_OBJECT(SVGLinearGradientElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGLinearGradientElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGLinearGradientElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGLinearGradientElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x1_getter);

    JS_DECLARE_NATIVE_FUNCTION(y1_getter);

    JS_DECLARE_NATIVE_FUNCTION(x2_getter);

    JS_DECLARE_NATIVE_FUNCTION(y2_getter);


};


} // namespace Web::Bindings
    