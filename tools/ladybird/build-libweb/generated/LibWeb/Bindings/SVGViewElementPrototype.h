
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGViewElementPrototype : public JS::Object {
    JS_OBJECT(SVGViewElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGViewElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGViewElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGViewElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(view_box_getter);


};


} // namespace Web::Bindings
    