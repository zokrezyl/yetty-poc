
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XRTestPrototype : public JS::Object {
    JS_OBJECT(XRTestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XRTestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XRTestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XRTestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(simulate_device_connection);
        
    JS_DECLARE_NATIVE_FUNCTION(simulate_user_activation);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect_all_devices);
        

};


} // namespace Web::Bindings
    