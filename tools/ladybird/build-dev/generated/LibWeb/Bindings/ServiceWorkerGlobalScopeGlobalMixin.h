
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ServiceWorkerGlobalScopeGlobalMixin {
public:
    void initialize(JS::Realm&, JS::Object&);
    void define_unforgeable_attributes(JS::Realm&, JS::Object&);
    ServiceWorkerGlobalScopeGlobalMixin();
    virtual ~ServiceWorkerGlobalScopeGlobalMixin();

private:

    JS_DECLARE_NATIVE_FUNCTION(oninstall_getter);

    JS_DECLARE_NATIVE_FUNCTION(oninstall_setter);

    JS_DECLARE_NATIVE_FUNCTION(onactivate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onactivate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfetch_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfetch_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(cookie_store_getter);


};


} // namespace Web::Bindings
    