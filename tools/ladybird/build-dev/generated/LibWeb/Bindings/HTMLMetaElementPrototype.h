
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLMetaElementPrototype : public JS::Object {
    JS_OBJECT(HTMLMetaElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLMetaElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLMetaElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMetaElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(http_equiv_getter);

    JS_DECLARE_NATIVE_FUNCTION(http_equiv_setter);

    JS_DECLARE_NATIVE_FUNCTION(content_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_setter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(scheme_getter);

    JS_DECLARE_NATIVE_FUNCTION(scheme_setter);


};


} // namespace Web::Bindings
    