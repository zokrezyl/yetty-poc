
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLMeterElementPrototype : public JS::Object {
    JS_OBJECT(HTMLMeterElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLMeterElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLMeterElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMeterElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(min_getter);

    JS_DECLARE_NATIVE_FUNCTION(min_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_setter);

    JS_DECLARE_NATIVE_FUNCTION(low_getter);

    JS_DECLARE_NATIVE_FUNCTION(low_setter);

    JS_DECLARE_NATIVE_FUNCTION(high_getter);

    JS_DECLARE_NATIVE_FUNCTION(high_setter);

    JS_DECLARE_NATIVE_FUNCTION(optimum_getter);

    JS_DECLARE_NATIVE_FUNCTION(optimum_setter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);


};


} // namespace Web::Bindings
    