
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EventSourcePrototype : public JS::Object {
    JS_OBJECT(EventSourcePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EventSourcePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EventSourcePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EventSourcePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(with_credentials_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(onopen_getter);

    JS_DECLARE_NATIVE_FUNCTION(onopen_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


} // namespace Web::Bindings
    