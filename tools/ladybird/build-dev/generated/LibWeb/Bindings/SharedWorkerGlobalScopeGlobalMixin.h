
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SharedWorkerGlobalScopeGlobalMixin {
public:
    void initialize(JS::Realm&, JS::Object&);
    void define_unforgeable_attributes(JS::Realm&, JS::Object&);
    SharedWorkerGlobalScopeGlobalMixin();
    virtual ~SharedWorkerGlobalScopeGlobalMixin();

private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(onconnect_getter);

    JS_DECLARE_NATIVE_FUNCTION(onconnect_setter);


};


} // namespace Web::Bindings
    