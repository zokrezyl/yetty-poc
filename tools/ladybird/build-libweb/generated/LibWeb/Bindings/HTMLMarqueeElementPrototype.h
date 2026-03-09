
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLMarqueeElementPrototype : public JS::Object {
    JS_OBJECT(HTMLMarqueeElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLMarqueeElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLMarqueeElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMarqueeElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(behavior_getter);

    JS_DECLARE_NATIVE_FUNCTION(behavior_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(direction_getter);

    JS_DECLARE_NATIVE_FUNCTION(direction_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_amount_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_amount_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_delay_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_delay_setter);

    JS_DECLARE_NATIVE_FUNCTION(true_speed_getter);

    JS_DECLARE_NATIVE_FUNCTION(true_speed_setter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);


};


} // namespace Web::Bindings
    