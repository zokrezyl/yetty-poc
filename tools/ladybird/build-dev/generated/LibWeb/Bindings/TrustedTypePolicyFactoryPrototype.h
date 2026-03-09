
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrustedTypePolicyFactoryPrototype : public JS::Object {
    JS_OBJECT(TrustedTypePolicyFactoryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrustedTypePolicyFactoryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrustedTypePolicyFactoryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedTypePolicyFactoryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_policy);
        
    JS_DECLARE_NATIVE_FUNCTION(is_html);
        
    JS_DECLARE_NATIVE_FUNCTION(is_script);
        
    JS_DECLARE_NATIVE_FUNCTION(is_script_url);
        
    JS_DECLARE_NATIVE_FUNCTION(get_attribute_type);
        
    JS_DECLARE_NATIVE_FUNCTION(get_property_type);
        
    JS_DECLARE_NATIVE_FUNCTION(empty_html_getter);

    JS_DECLARE_NATIVE_FUNCTION(empty_script_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_policy_getter);


};


} // namespace Web::Bindings
    