
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrustedTypePolicyPrototype : public JS::Object {
    JS_OBJECT(TrustedTypePolicyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrustedTypePolicyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrustedTypePolicyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedTypePolicyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_html);
        
    JS_DECLARE_NATIVE_FUNCTION(create_script);
        
    JS_DECLARE_NATIVE_FUNCTION(create_script_url);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);


};


} // namespace Web::Bindings
    