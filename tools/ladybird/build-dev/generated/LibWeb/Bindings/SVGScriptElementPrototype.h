
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGScriptElementPrototype : public JS::Object {
    JS_OBJECT(SVGScriptElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGScriptElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGScriptElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGScriptElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_setter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);


};


} // namespace Web::Bindings
    