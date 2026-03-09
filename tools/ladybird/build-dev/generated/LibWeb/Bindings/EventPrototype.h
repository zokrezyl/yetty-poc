
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class EventPrototype : public JS::Object {
    JS_OBJECT(EventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(EventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit EventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~EventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(composed_path);
        
    JS_DECLARE_NATIVE_FUNCTION(stop_propagation);
        
    JS_DECLARE_NATIVE_FUNCTION(stop_immediate_propagation);
        
    JS_DECLARE_NATIVE_FUNCTION(prevent_default);
        
    JS_DECLARE_NATIVE_FUNCTION(init_event);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_target_getter);

    JS_DECLARE_NATIVE_FUNCTION(event_phase_getter);

    JS_DECLARE_NATIVE_FUNCTION(cancel_bubble_getter);

    JS_DECLARE_NATIVE_FUNCTION(cancel_bubble_setter);

    JS_DECLARE_NATIVE_FUNCTION(bubbles_getter);

    JS_DECLARE_NATIVE_FUNCTION(cancelable_getter);

    JS_DECLARE_NATIVE_FUNCTION(return_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(return_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_prevented_getter);

    JS_DECLARE_NATIVE_FUNCTION(composed_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_trusted_getter);

    JS_DECLARE_NATIVE_FUNCTION(time_stamp_getter);


};


} // namespace Web::Bindings
    