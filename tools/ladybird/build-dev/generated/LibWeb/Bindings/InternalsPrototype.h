
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class InternalsPrototype : public JS::Object {
    JS_OBJECT(InternalsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(InternalsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit InternalsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~InternalsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(signal_test_is_done);
        
    JS_DECLARE_NATIVE_FUNCTION(set_test_timeout);
        
    JS_DECLARE_NATIVE_FUNCTION(load_reference_test_metadata);
        
    JS_DECLARE_NATIVE_FUNCTION(load_test_variants);
        
    JS_DECLARE_NATIVE_FUNCTION(set_time_zone);
        
    JS_DECLARE_NATIVE_FUNCTION(gc);
        
    JS_DECLARE_NATIVE_FUNCTION(hit_test);
        
    JS_DECLARE_NATIVE_FUNCTION(send_text);
        
    JS_DECLARE_NATIVE_FUNCTION(send_key);
        
    JS_DECLARE_NATIVE_FUNCTION(paste);
        
    JS_DECLARE_NATIVE_FUNCTION(commit_text);
        
    JS_DECLARE_NATIVE_FUNCTION(mouse_down);
        
    JS_DECLARE_NATIVE_FUNCTION(mouse_up);
        
    JS_DECLARE_NATIVE_FUNCTION(mouse_move);
        
    JS_DECLARE_NATIVE_FUNCTION(click);
        
    JS_DECLARE_NATIVE_FUNCTION(click_and_hold);
        
    JS_DECLARE_NATIVE_FUNCTION(wheel);
        
    JS_DECLARE_NATIVE_FUNCTION(pinch);
        
    JS_DECLARE_NATIVE_FUNCTION(current_cursor);
        
    JS_DECLARE_NATIVE_FUNCTION(dispatch_user_activated_event);
        
    JS_DECLARE_NATIVE_FUNCTION(spoof_current_url);
        
    JS_DECLARE_NATIVE_FUNCTION(create_internal_animation_timeline);
        
    JS_DECLARE_NATIVE_FUNCTION(simulate_drag_start);
        
    JS_DECLARE_NATIVE_FUNCTION(simulate_drag_move);
        
    JS_DECLARE_NATIVE_FUNCTION(simulate_drop);
        
    JS_DECLARE_NATIVE_FUNCTION(expire_cookies_with_time_offset);
        
    JS_DECLARE_NATIVE_FUNCTION(set_http_memory_cache_enabled);
        
    JS_DECLARE_NATIVE_FUNCTION(get_computed_role);
        
    JS_DECLARE_NATIVE_FUNCTION(get_computed_label);
        
    JS_DECLARE_NATIVE_FUNCTION(get_computed_aria_level);
        
    JS_DECLARE_NATIVE_FUNCTION(get_echo_server_port);
        
    JS_DECLARE_NATIVE_FUNCTION(set_browser_zoom);
        
    JS_DECLARE_NATIVE_FUNCTION(set_device_pixel_ratio);
        
    JS_DECLARE_NATIVE_FUNCTION(dump_display_list);
        
    JS_DECLARE_NATIVE_FUNCTION(dump_layout_tree);
        
    JS_DECLARE_NATIVE_FUNCTION(dump_paintable_tree);
        
    JS_DECLARE_NATIVE_FUNCTION(dump_stacking_context_tree);
        
    JS_DECLARE_NATIVE_FUNCTION(dump_gc_graph);
        
    JS_DECLARE_NATIVE_FUNCTION(get_shadow_root);
        
    JS_DECLARE_NATIVE_FUNCTION(handle_sdl_input_events);
        
    JS_DECLARE_NATIVE_FUNCTION(set_environments_top_level_url);
        
    JS_DECLARE_NATIVE_FUNCTION(connect_virtual_gamepad);
        
    JS_DECLARE_NATIVE_FUNCTION(set_highlighted_node);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_element);
        
    JS_DECLARE_NATIVE_FUNCTION(headless_getter);


};


} // namespace Web::Bindings
    