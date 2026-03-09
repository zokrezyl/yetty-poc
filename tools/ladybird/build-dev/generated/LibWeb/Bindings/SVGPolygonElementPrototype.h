
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGPolygonElementPrototype : public JS::Object {
    JS_OBJECT(SVGPolygonElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGPolygonElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGPolygonElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGPolygonElementPrototype() override;
private:


};


} // namespace Web::Bindings
    