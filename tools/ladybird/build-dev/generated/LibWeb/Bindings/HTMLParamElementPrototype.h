
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLParamElementPrototype : public JS::Object {
    JS_OBJECT(HTMLParamElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLParamElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLParamElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLParamElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_type_setter);


};


} // namespace Web::Bindings
    