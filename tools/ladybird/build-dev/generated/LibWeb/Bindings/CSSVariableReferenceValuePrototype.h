
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSVariableReferenceValuePrototype : public JS::Object {
    JS_OBJECT(CSSVariableReferenceValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSVariableReferenceValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSVariableReferenceValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSVariableReferenceValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(variable_getter);

    JS_DECLARE_NATIVE_FUNCTION(variable_setter);

    JS_DECLARE_NATIVE_FUNCTION(fallback_getter);


};


} // namespace Web::Bindings
    