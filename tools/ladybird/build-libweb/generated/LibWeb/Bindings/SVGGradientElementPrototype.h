
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGGradientElementPrototype : public JS::Object {
    JS_OBJECT(SVGGradientElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGGradientElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGGradientElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGGradientElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    