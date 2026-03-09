
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLLegendElementPrototype : public JS::Object {
    JS_OBJECT(HTMLLegendElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLLegendElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLLegendElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLLegendElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(form_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);


};


} // namespace Web::Bindings
    