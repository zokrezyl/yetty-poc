
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLOptionElementPrototype : public JS::Object {
    JS_OBJECT(HTMLOptionElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLOptionElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLOptionElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLOptionElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_selected_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_selected_setter);

    JS_DECLARE_NATIVE_FUNCTION(selected_getter);

    JS_DECLARE_NATIVE_FUNCTION(selected_setter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);

    JS_DECLARE_NATIVE_FUNCTION(index_getter);


};


} // namespace Web::Bindings
    