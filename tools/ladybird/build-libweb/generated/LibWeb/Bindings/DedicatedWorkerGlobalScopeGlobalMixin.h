
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DedicatedWorkerGlobalScopeGlobalMixin {
public:
    void initialize(JS::Realm&, JS::Object&);
    void define_unforgeable_attributes(JS::Realm&, JS::Object&);
    DedicatedWorkerGlobalScopeGlobalMixin();
    virtual ~DedicatedWorkerGlobalScopeGlobalMixin();

private:

    JS_DECLARE_NATIVE_FUNCTION(post_message);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message0);

    JS_DECLARE_NATIVE_FUNCTION(post_message1);

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);


};


} // namespace Web::Bindings
    