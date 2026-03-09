
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLLabelElementPrototype : public JS::Object {
    JS_OBJECT(HTMLLabelElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLLabelElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLLabelElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLLabelElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(html_for_getter);

    JS_DECLARE_NATIVE_FUNCTION(html_for_setter);

    JS_DECLARE_NATIVE_FUNCTION(control_getter);


};


} // namespace Web::Bindings
    