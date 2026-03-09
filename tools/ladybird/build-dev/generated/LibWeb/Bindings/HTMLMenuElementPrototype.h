
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLMenuElementPrototype : public JS::Object {
    JS_OBJECT(HTMLMenuElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLMenuElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLMenuElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMenuElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(compact_getter);

    JS_DECLARE_NATIVE_FUNCTION(compact_setter);


};


} // namespace Web::Bindings
    