
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMPointPrototype : public JS::Object {
    JS_OBJECT(DOMPointPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMPointPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMPointPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMPointPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_setter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_setter);

    JS_DECLARE_NATIVE_FUNCTION(w_getter);

    JS_DECLARE_NATIVE_FUNCTION(w_setter);


};


} // namespace Web::Bindings
    