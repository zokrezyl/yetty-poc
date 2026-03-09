
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTextElementPrototype : public JS::Object {
    JS_OBJECT(SVGTextElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTextElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTextElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTextElementPrototype() override;
private:


};


} // namespace Web::Bindings
    