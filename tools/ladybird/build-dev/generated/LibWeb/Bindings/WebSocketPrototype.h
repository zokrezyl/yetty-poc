
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebSocketPrototype : public JS::Object {
    JS_OBJECT(WebSocketPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebSocketPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebSocketPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebSocketPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(send);
        
    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(onopen_getter);

    JS_DECLARE_NATIVE_FUNCTION(onopen_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_getter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_setter);

    JS_DECLARE_NATIVE_FUNCTION(extensions_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(binary_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(binary_type_setter);


};


} // namespace Web::Bindings
    