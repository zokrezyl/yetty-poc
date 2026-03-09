
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLOListElementPrototype : public JS::Object {
    JS_OBJECT(HTMLOListElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLOListElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLOListElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLOListElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(reversed_getter);

    JS_DECLARE_NATIVE_FUNCTION(reversed_setter);

    JS_DECLARE_NATIVE_FUNCTION(start_getter);

    JS_DECLARE_NATIVE_FUNCTION(start_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(compact_getter);

    JS_DECLARE_NATIVE_FUNCTION(compact_setter);


};


} // namespace Web::Bindings
    