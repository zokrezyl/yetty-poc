
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMPointReadOnlyPrototype : public JS::Object {
    JS_OBJECT(DOMPointReadOnlyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMPointReadOnlyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMPointReadOnlyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMPointReadOnlyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(matrix_transform);
        
    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_getter);

    JS_DECLARE_NATIVE_FUNCTION(w_getter);


};


} // namespace Web::Bindings
    