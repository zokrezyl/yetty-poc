
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGPolylineElementPrototype : public JS::Object {
    JS_OBJECT(SVGPolylineElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGPolylineElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGPolylineElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGPolylineElementPrototype() override;
private:


};


} // namespace Web::Bindings
    