
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSUnitValuePrototype : public JS::Object {
    JS_OBJECT(CSSUnitValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSUnitValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSUnitValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSUnitValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(unit_getter);


};


} // namespace Web::Bindings
    