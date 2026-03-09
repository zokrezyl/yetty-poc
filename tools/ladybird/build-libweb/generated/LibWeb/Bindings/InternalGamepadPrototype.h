
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class InternalGamepadPrototype : public JS::Object {
    JS_OBJECT(InternalGamepadPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(InternalGamepadPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit InternalGamepadPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~InternalGamepadPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_button);
        
    JS_DECLARE_NATIVE_FUNCTION(set_axis);
        
    JS_DECLARE_NATIVE_FUNCTION(get_received_rumble_effects);
        
    JS_DECLARE_NATIVE_FUNCTION(get_received_rumble_trigger_effects);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        
    JS_DECLARE_NATIVE_FUNCTION(buttons_getter);

    JS_DECLARE_NATIVE_FUNCTION(axes_getter);

    JS_DECLARE_NATIVE_FUNCTION(triggers_getter);


};


} // namespace Web::Bindings
    