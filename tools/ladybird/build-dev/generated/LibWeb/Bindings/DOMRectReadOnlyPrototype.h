
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMRectReadOnlyPrototype : public JS::Object {
    JS_OBJECT(DOMRectReadOnlyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMRectReadOnlyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMRectReadOnlyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMRectReadOnlyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(top_getter);

    JS_DECLARE_NATIVE_FUNCTION(right_getter);

    JS_DECLARE_NATIVE_FUNCTION(bottom_getter);

    JS_DECLARE_NATIVE_FUNCTION(left_getter);


};


} // namespace Web::Bindings
    