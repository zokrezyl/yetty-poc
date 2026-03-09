
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MouseEventPrototype : public JS::Object {
    JS_OBJECT(MouseEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MouseEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MouseEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MouseEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_modifier_state);
        
    JS_DECLARE_NATIVE_FUNCTION(init_mouse_event);
        
    JS_DECLARE_NATIVE_FUNCTION(screen_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(offset_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(offset_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(ctrl_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(shift_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(alt_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(meta_key_getter);

    JS_DECLARE_NATIVE_FUNCTION(movement_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(movement_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(button_getter);

    JS_DECLARE_NATIVE_FUNCTION(buttons_getter);

    JS_DECLARE_NATIVE_FUNCTION(related_target_getter);


};


} // namespace Web::Bindings
    