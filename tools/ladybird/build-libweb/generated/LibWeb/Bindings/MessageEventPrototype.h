
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MessageEventPrototype : public JS::Object {
    JS_OBJECT(MessageEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MessageEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MessageEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MessageEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(init_message_event);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_event_id_getter);

    JS_DECLARE_NATIVE_FUNCTION(source_getter);

    JS_DECLARE_NATIVE_FUNCTION(ports_getter);


};


} // namespace Web::Bindings
    