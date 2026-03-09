
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGLengthPrototype : public JS::Object {
    JS_OBJECT(SVGLengthPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGLengthPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGLengthPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGLengthPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(unit_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);


};


} // namespace Web::Bindings
    