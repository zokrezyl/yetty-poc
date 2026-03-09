
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEFuncBElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEFuncBElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEFuncBElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEFuncBElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFuncBElementPrototype() override;
private:


};


} // namespace Web::Bindings
    