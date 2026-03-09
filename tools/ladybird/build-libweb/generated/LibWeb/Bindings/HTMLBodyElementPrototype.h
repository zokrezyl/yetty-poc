
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLBodyElementPrototype : public JS::Object {
    JS_OBJECT(HTMLBodyElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLBodyElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLBodyElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLBodyElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);

    JS_DECLARE_NATIVE_FUNCTION(link_getter);

    JS_DECLARE_NATIVE_FUNCTION(link_setter);

    JS_DECLARE_NATIVE_FUNCTION(v_link_getter);

    JS_DECLARE_NATIVE_FUNCTION(v_link_setter);

    JS_DECLARE_NATIVE_FUNCTION(a_link_getter);

    JS_DECLARE_NATIVE_FUNCTION(a_link_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(background_getter);

    JS_DECLARE_NATIVE_FUNCTION(background_setter);

    JS_DECLARE_NATIVE_FUNCTION(onafterprint_getter);

    JS_DECLARE_NATIVE_FUNCTION(onafterprint_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeprint_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeprint_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeunload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeunload_setter);

    JS_DECLARE_NATIVE_FUNCTION(onhashchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onhashchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_getter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_setter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_getter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpagehide_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpagehide_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpagereveal_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpagereveal_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpageshow_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpageshow_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpageswap_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpageswap_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpopstate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpopstate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_getter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_setter);

    JS_DECLARE_NATIVE_FUNCTION(onstorage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstorage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_getter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_setter);

    JS_DECLARE_NATIVE_FUNCTION(onunload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onunload_setter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepadconnected_getter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepadconnected_setter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepaddisconnected_getter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepaddisconnected_setter);


};


} // namespace Web::Bindings
    