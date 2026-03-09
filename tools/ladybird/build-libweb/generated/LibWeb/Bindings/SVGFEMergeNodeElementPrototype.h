
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGFEMergeNodeElementPrototype : public JS::Object {
    JS_OBJECT(SVGFEMergeNodeElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGFEMergeNodeElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGFEMergeNodeElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGFEMergeNodeElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(in1_getter);


};


} // namespace Web::Bindings
    