
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ServiceWorkerContainerPrototype : public JS::Object {
    JS_OBJECT(ServiceWorkerContainerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ServiceWorkerContainerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ServiceWorkerContainerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ServiceWorkerContainerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(register_);
        
    JS_DECLARE_NATIVE_FUNCTION(get_registration);
        
    JS_DECLARE_NATIVE_FUNCTION(ready_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontrollerchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontrollerchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);


};


} // namespace Web::Bindings
    