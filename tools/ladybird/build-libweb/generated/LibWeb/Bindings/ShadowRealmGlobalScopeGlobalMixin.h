
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ShadowRealmGlobalScopeGlobalMixin {
public:
    void initialize(JS::Realm&, JS::Object&);
    void define_unforgeable_attributes(JS::Realm&, JS::Object&);
    ShadowRealmGlobalScopeGlobalMixin();
    virtual ~ShadowRealmGlobalScopeGlobalMixin();

private:

    JS_DECLARE_NATIVE_FUNCTION(btoa);
        
    JS_DECLARE_NATIVE_FUNCTION(atob);
        
    JS_DECLARE_NATIVE_FUNCTION(queue_microtask);
        
    JS_DECLARE_NATIVE_FUNCTION(structured_clone);
        
    JS_DECLARE_NATIVE_FUNCTION(self_getter);


};


} // namespace Web::Bindings
    