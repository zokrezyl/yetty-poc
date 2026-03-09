
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLStyleElementPrototype : public JS::Object {
    JS_OBJECT(HTMLStyleElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLStyleElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLStyleElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLStyleElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(sheet_getter);


};


} // namespace Web::Bindings
    