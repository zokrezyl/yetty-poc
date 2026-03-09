
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGLineElementPrototype : public JS::Object {
    JS_OBJECT(SVGLineElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGLineElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGLineElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGLineElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x1_getter);

    JS_DECLARE_NATIVE_FUNCTION(y1_getter);

    JS_DECLARE_NATIVE_FUNCTION(x2_getter);

    JS_DECLARE_NATIVE_FUNCTION(y2_getter);


};


} // namespace Web::Bindings
    