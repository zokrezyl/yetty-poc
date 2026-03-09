
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGSymbolElementPrototype : public JS::Object {
    JS_OBJECT(SVGSymbolElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGSymbolElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGSymbolElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGSymbolElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(view_box_getter);


};


} // namespace Web::Bindings
    