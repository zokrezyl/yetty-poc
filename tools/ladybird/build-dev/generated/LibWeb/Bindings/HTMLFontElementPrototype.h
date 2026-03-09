
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLFontElementPrototype : public JS::Object {
    JS_OBJECT(HTMLFontElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLFontElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLFontElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFontElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(color_getter);

    JS_DECLARE_NATIVE_FUNCTION(color_setter);

    JS_DECLARE_NATIVE_FUNCTION(face_getter);

    JS_DECLARE_NATIVE_FUNCTION(face_setter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_setter);


};


} // namespace Web::Bindings
    