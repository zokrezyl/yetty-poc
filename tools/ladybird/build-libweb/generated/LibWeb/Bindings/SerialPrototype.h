
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SerialPrototype : public JS::Object {
    JS_OBJECT(SerialPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SerialPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SerialPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SerialPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_ports);
        
    JS_DECLARE_NATIVE_FUNCTION(request_port);
        
    JS_DECLARE_NATIVE_FUNCTION(onconnect_getter);

    JS_DECLARE_NATIVE_FUNCTION(onconnect_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondisconnect_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondisconnect_setter);


};


} // namespace Web::Bindings
    