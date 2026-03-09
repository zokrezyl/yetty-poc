
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGAElementPrototype : public JS::Object {
    JS_OBJECT(SVGAElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGAElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGAElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGAElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(download_getter);

    JS_DECLARE_NATIVE_FUNCTION(download_setter);

    JS_DECLARE_NATIVE_FUNCTION(ping_getter);

    JS_DECLARE_NATIVE_FUNCTION(ping_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_setter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_getter);

    JS_DECLARE_NATIVE_FUNCTION(rel_list_setter);

    JS_DECLARE_NATIVE_FUNCTION(hreflang_getter);

    JS_DECLARE_NATIVE_FUNCTION(hreflang_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_setter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    