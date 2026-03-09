
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGGElementPrototype : public JS::Object {
    JS_OBJECT(SVGGElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGGElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGGElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGGElementPrototype() override;
private:


};


} // namespace Web::Bindings
    