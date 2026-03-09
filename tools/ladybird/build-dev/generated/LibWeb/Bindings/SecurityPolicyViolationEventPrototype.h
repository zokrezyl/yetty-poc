
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SecurityPolicyViolationEventPrototype : public JS::Object {
    JS_OBJECT(SecurityPolicyViolationEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SecurityPolicyViolationEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SecurityPolicyViolationEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SecurityPolicyViolationEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(document_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_getter);

    JS_DECLARE_NATIVE_FUNCTION(blocked_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(effective_directive_getter);

    JS_DECLARE_NATIVE_FUNCTION(violated_directive_getter);

    JS_DECLARE_NATIVE_FUNCTION(original_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(source_file_getter);

    JS_DECLARE_NATIVE_FUNCTION(sample_getter);

    JS_DECLARE_NATIVE_FUNCTION(disposition_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_code_getter);

    JS_DECLARE_NATIVE_FUNCTION(line_number_getter);

    JS_DECLARE_NATIVE_FUNCTION(column_number_getter);


};


enum class SecurityPolicyViolationEventDisposition : u8 {

    Enforce,

    Report,

};

inline String idl_enum_to_string(SecurityPolicyViolationEventDisposition value)
{
    switch (value) {

    case SecurityPolicyViolationEventDisposition::Enforce:
        return "enforce"_string;

    case SecurityPolicyViolationEventDisposition::Report:
        return "report"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    