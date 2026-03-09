
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLBRElementPrototype : public JS::Object {
    JS_OBJECT(HTMLBRElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLBRElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLBRElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLBRElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(clear_getter);

    JS_DECLARE_NATIVE_FUNCTION(clear_setter);


};


} // namespace Web::Bindings
    