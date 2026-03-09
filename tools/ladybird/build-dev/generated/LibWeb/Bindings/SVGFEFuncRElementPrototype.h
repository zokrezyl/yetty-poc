
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEFuncRElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEFuncRElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEFuncRElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEFuncRElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFuncRElementPrototype() override;
private:


};


} // namespace Web::Bindings
    