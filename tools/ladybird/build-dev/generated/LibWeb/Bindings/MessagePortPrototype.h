
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MessagePortPrototype : public JS::Object {
    JS_OBJECT(MessagePortPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MessagePortPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MessagePortPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MessagePortPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(post_message);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message0);

    JS_DECLARE_NATIVE_FUNCTION(post_message1);

    JS_DECLARE_NATIVE_FUNCTION(start);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);


};


} // namespace Web::Bindings
    