
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGMaskElementPrototype : public JS::Object {
    JS_OBJECT(SVGMaskElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGMaskElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGMaskElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGMaskElementPrototype() override;
private:


};


} // namespace Web::Bindings
    