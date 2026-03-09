
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EventTargetPrototype : public JS::Object {
    JS_OBJECT(EventTargetPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EventTargetPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EventTargetPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EventTargetPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add_event_listener);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_event_listener);
        
    JS_DECLARE_NATIVE_FUNCTION(dispatch_event);
        

};


} // namespace Web::Bindings
    