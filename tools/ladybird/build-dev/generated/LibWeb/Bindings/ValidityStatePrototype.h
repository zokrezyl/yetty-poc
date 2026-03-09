
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ValidityStatePrototype : public JS::Object {
    JS_OBJECT(ValidityStatePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ValidityStatePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ValidityStatePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ValidityStatePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_missing_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_mismatch_getter);

    JS_DECLARE_NATIVE_FUNCTION(pattern_mismatch_getter);

    JS_DECLARE_NATIVE_FUNCTION(too_long_getter);

    JS_DECLARE_NATIVE_FUNCTION(too_short_getter);

    JS_DECLARE_NATIVE_FUNCTION(range_underflow_getter);

    JS_DECLARE_NATIVE_FUNCTION(range_overflow_getter);

    JS_DECLARE_NATIVE_FUNCTION(step_mismatch_getter);

    JS_DECLARE_NATIVE_FUNCTION(bad_input_getter);

    JS_DECLARE_NATIVE_FUNCTION(custom_error_getter);

    JS_DECLARE_NATIVE_FUNCTION(valid_getter);


};


} // namespace Web::Bindings
    