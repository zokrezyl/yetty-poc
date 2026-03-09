
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGDefsElementPrototype : public JS::Object {
    JS_OBJECT(SVGDefsElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGDefsElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGDefsElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGDefsElementPrototype() override;
private:


};


} // namespace Web::Bindings
    