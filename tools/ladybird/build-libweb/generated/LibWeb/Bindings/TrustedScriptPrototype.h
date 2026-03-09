
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrustedScriptPrototype : public JS::Object {
    JS_OBJECT(TrustedScriptPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrustedScriptPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrustedScriptPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedScriptPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        

};


} // namespace Web::Bindings
    