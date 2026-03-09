
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGNumberPrototype : public JS::Object {
    JS_OBJECT(SVGNumberPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGNumberPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGNumberPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGNumberPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);


};


} // namespace Web::Bindings
    