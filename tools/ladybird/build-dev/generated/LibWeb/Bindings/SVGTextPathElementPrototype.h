
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTextPathElementPrototype : public JS::Object {
    JS_OBJECT(SVGTextPathElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTextPathElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTextPathElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTextPathElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    