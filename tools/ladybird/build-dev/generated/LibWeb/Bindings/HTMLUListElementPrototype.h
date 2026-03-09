
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLUListElementPrototype : public JS::Object {
    JS_OBJECT(HTMLUListElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLUListElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLUListElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLUListElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(compact_getter);

    JS_DECLARE_NATIVE_FUNCTION(compact_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);


};


} // namespace Web::Bindings
    