
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLScriptElementPrototype : public JS::Object {
    JS_OBJECT(HTMLScriptElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLScriptElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLScriptElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLScriptElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_module_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_module_setter);

    JS_DECLARE_NATIVE_FUNCTION(async_getter);

    JS_DECLARE_NATIVE_FUNCTION(async_setter);

    JS_DECLARE_NATIVE_FUNCTION(defer_getter);

    JS_DECLARE_NATIVE_FUNCTION(defer_setter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_setter);

    JS_DECLARE_NATIVE_FUNCTION(integrity_getter);

    JS_DECLARE_NATIVE_FUNCTION(integrity_setter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_getter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_content_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_content_setter);

    JS_DECLARE_NATIVE_FUNCTION(inner_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_text_setter);

    JS_DECLARE_NATIVE_FUNCTION(charset_getter);

    JS_DECLARE_NATIVE_FUNCTION(charset_setter);

    JS_DECLARE_NATIVE_FUNCTION(event_getter);

    JS_DECLARE_NATIVE_FUNCTION(event_setter);

    JS_DECLARE_NATIVE_FUNCTION(html_for_getter);

    JS_DECLARE_NATIVE_FUNCTION(html_for_setter);


};


} // namespace Web::Bindings
    