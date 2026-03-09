
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTableColElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTableColElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTableColElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTableColElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableColElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(span_getter);

    JS_DECLARE_NATIVE_FUNCTION(span_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_setter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);


};


} // namespace Web::Bindings
    