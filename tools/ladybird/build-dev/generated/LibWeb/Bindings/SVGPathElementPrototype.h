
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGPathElementPrototype : public JS::Object {
    JS_OBJECT(SVGPathElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGPathElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGPathElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGPathElementPrototype() override;
private:


};


} // namespace Web::Bindings
    