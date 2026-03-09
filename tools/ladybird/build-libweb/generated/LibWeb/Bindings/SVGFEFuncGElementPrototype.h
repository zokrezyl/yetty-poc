
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEFuncGElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEFuncGElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEFuncGElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEFuncGElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFuncGElementPrototype() override;
private:


};


} // namespace Web::Bindings
    