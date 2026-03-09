
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGStyleElementPrototype : public JS::Object {
    JS_OBJECT(SVGStyleElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGStyleElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGStyleElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGStyleElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(title_getter);

    JS_DECLARE_NATIVE_FUNCTION(title_setter);

    JS_DECLARE_NATIVE_FUNCTION(sheet_getter);


};


} // namespace Web::Bindings
    