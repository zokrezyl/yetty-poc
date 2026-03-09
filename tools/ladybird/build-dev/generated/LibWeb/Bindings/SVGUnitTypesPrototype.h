
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGUnitTypesPrototype : public JS::Object {
    JS_OBJECT(SVGUnitTypesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGUnitTypesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGUnitTypesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGUnitTypesPrototype() override;
private:


};


} // namespace Web::Bindings
    