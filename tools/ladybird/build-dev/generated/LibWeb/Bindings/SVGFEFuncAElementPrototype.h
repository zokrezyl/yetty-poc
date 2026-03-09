
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEFuncAElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEFuncAElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEFuncAElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEFuncAElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEFuncAElementPrototype() override;
private:


};


} // namespace Web::Bindings
    