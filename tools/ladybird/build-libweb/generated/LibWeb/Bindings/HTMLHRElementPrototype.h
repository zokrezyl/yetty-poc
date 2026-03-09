
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLHRElementPrototype : public JS::Object {
    JS_OBJECT(HTMLHRElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLHRElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLHRElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHRElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(color_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_shade_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_shade_setter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);


};


} // namespace Web::Bindings
    