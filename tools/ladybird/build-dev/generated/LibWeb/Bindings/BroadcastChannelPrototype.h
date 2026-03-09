
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BroadcastChannelPrototype : public JS::Object {
    JS_OBJECT(BroadcastChannelPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BroadcastChannelPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BroadcastChannelPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BroadcastChannelPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(post_message);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);


};


} // namespace Web::Bindings
    