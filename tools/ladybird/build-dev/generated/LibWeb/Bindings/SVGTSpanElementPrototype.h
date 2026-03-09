
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTSpanElementPrototype : public JS::Object {
    JS_OBJECT(SVGTSpanElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTSpanElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTSpanElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTSpanElementPrototype() override;
private:


};


} // namespace Web::Bindings
    