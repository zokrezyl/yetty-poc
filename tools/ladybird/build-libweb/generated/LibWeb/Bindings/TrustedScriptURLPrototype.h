
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrustedScriptURLPrototype : public JS::Object {
    JS_OBJECT(TrustedScriptURLPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrustedScriptURLPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrustedScriptURLPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedScriptURLPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        

};


} // namespace Web::Bindings
    