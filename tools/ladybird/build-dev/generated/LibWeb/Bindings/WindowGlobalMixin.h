
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WindowGlobalMixin {
public:
    void initialize(JS::Realm&, JS::Object&);
    void define_unforgeable_attributes(JS::Realm&, JS::Object&);
    WindowGlobalMixin();
    virtual ~WindowGlobalMixin();

private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(stop);
        
    JS_DECLARE_NATIVE_FUNCTION(focus);
        
    JS_DECLARE_NATIVE_FUNCTION(blur);
        
    JS_DECLARE_NATIVE_FUNCTION(open);
        
    JS_DECLARE_NATIVE_FUNCTION(alert);
        
    JS_DECLARE_NATIVE_FUNCTION(alert0);

    JS_DECLARE_NATIVE_FUNCTION(alert1);

    JS_DECLARE_NATIVE_FUNCTION(confirm);
        
    JS_DECLARE_NATIVE_FUNCTION(prompt);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message0);

    JS_DECLARE_NATIVE_FUNCTION(post_message1);

    JS_DECLARE_NATIVE_FUNCTION(get_computed_style);
        
    JS_DECLARE_NATIVE_FUNCTION(match_media);
        
    JS_DECLARE_NATIVE_FUNCTION(move_to);
        
    JS_DECLARE_NATIVE_FUNCTION(move_by);
        
    JS_DECLARE_NATIVE_FUNCTION(resize_to);
        
    JS_DECLARE_NATIVE_FUNCTION(resize_by);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll0);

    JS_DECLARE_NATIVE_FUNCTION(scroll1);

    JS_DECLARE_NATIVE_FUNCTION(scroll_to);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll_to0);

    JS_DECLARE_NATIVE_FUNCTION(scroll_to1);

    JS_DECLARE_NATIVE_FUNCTION(scroll_by);
        
    JS_DECLARE_NATIVE_FUNCTION(scroll_by0);

    JS_DECLARE_NATIVE_FUNCTION(scroll_by1);

    JS_DECLARE_NATIVE_FUNCTION(request_idle_callback);
        
    JS_DECLARE_NATIVE_FUNCTION(cancel_idle_callback);
        
    JS_DECLARE_NATIVE_FUNCTION(get_selection);
        
    JS_DECLARE_NATIVE_FUNCTION(find);
        
    JS_DECLARE_NATIVE_FUNCTION(capture_events);
        
    JS_DECLARE_NATIVE_FUNCTION(release_events);
        
    JS_DECLARE_NATIVE_FUNCTION(request_animation_frame);
        
    JS_DECLARE_NATIVE_FUNCTION(cancel_animation_frame);
        
    JS_DECLARE_NATIVE_FUNCTION(report_error);
        
    JS_DECLARE_NATIVE_FUNCTION(set_timeout);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_timeout);
        
    JS_DECLARE_NATIVE_FUNCTION(set_interval);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_interval);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap0);

    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap1);

    JS_DECLARE_NATIVE_FUNCTION(fetch);
        
    JS_DECLARE_NATIVE_FUNCTION(btoa);
        
    JS_DECLARE_NATIVE_FUNCTION(atob);
        
    JS_DECLARE_NATIVE_FUNCTION(queue_microtask);
        
    JS_DECLARE_NATIVE_FUNCTION(structured_clone);
        
    JS_DECLARE_NATIVE_FUNCTION(window_getter);

    JS_DECLARE_NATIVE_FUNCTION(self_getter);

    JS_DECLARE_NATIVE_FUNCTION(self_setter);

    JS_DECLARE_NATIVE_FUNCTION(document_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(status_getter);

    JS_DECLARE_NATIVE_FUNCTION(status_setter);

    JS_DECLARE_NATIVE_FUNCTION(closed_getter);

    JS_DECLARE_NATIVE_FUNCTION(location_getter);

    JS_DECLARE_NATIVE_FUNCTION(location_setter);

    JS_DECLARE_NATIVE_FUNCTION(history_getter);

    JS_DECLARE_NATIVE_FUNCTION(navigation_getter);

    JS_DECLARE_NATIVE_FUNCTION(custom_elements_getter);

    JS_DECLARE_NATIVE_FUNCTION(locationbar_getter);

    JS_DECLARE_NATIVE_FUNCTION(locationbar_setter);

    JS_DECLARE_NATIVE_FUNCTION(menubar_getter);

    JS_DECLARE_NATIVE_FUNCTION(menubar_setter);

    JS_DECLARE_NATIVE_FUNCTION(personalbar_getter);

    JS_DECLARE_NATIVE_FUNCTION(personalbar_setter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbars_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrollbars_setter);

    JS_DECLARE_NATIVE_FUNCTION(statusbar_getter);

    JS_DECLARE_NATIVE_FUNCTION(statusbar_setter);

    JS_DECLARE_NATIVE_FUNCTION(toolbar_getter);

    JS_DECLARE_NATIVE_FUNCTION(toolbar_setter);

    JS_DECLARE_NATIVE_FUNCTION(frames_getter);

    JS_DECLARE_NATIVE_FUNCTION(frames_setter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_setter);

    JS_DECLARE_NATIVE_FUNCTION(top_getter);

    JS_DECLARE_NATIVE_FUNCTION(opener_getter);

    JS_DECLARE_NATIVE_FUNCTION(opener_setter);

    JS_DECLARE_NATIVE_FUNCTION(parent_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_setter);

    JS_DECLARE_NATIVE_FUNCTION(frame_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(navigator_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_information_getter);

    JS_DECLARE_NATIVE_FUNCTION(client_information_setter);

    JS_DECLARE_NATIVE_FUNCTION(event_getter);

    JS_DECLARE_NATIVE_FUNCTION(event_setter);

    JS_DECLARE_NATIVE_FUNCTION(screen_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_setter);

    JS_DECLARE_NATIVE_FUNCTION(visual_viewport_getter);

    JS_DECLARE_NATIVE_FUNCTION(visual_viewport_setter);

    JS_DECLARE_NATIVE_FUNCTION(inner_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_width_setter);

    JS_DECLARE_NATIVE_FUNCTION(inner_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_height_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_x_setter);

    JS_DECLARE_NATIVE_FUNCTION(page_x_offset_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_x_offset_setter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_y_setter);

    JS_DECLARE_NATIVE_FUNCTION(page_y_offset_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_y_offset_setter);

    JS_DECLARE_NATIVE_FUNCTION(screen_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_x_setter);

    JS_DECLARE_NATIVE_FUNCTION(screen_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_left_setter);

    JS_DECLARE_NATIVE_FUNCTION(screen_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_y_setter);

    JS_DECLARE_NATIVE_FUNCTION(screen_top_getter);

    JS_DECLARE_NATIVE_FUNCTION(screen_top_setter);

    JS_DECLARE_NATIVE_FUNCTION(outer_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(outer_width_setter);

    JS_DECLARE_NATIVE_FUNCTION(outer_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(outer_height_setter);

    JS_DECLARE_NATIVE_FUNCTION(device_pixel_ratio_getter);

    JS_DECLARE_NATIVE_FUNCTION(device_pixel_ratio_setter);

    JS_DECLARE_NATIVE_FUNCTION(cookie_store_getter);

    JS_DECLARE_NATIVE_FUNCTION(external_getter);

    JS_DECLARE_NATIVE_FUNCTION(external_setter);

    JS_DECLARE_NATIVE_FUNCTION(speech_synthesis_getter);

    JS_DECLARE_NATIVE_FUNCTION(onafterprint_getter);

    JS_DECLARE_NATIVE_FUNCTION(onafterprint_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeprint_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeprint_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeunload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeunload_setter);

    JS_DECLARE_NATIVE_FUNCTION(onhashchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onhashchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_getter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_setter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_getter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpagehide_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpagehide_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpagereveal_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpagereveal_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpageshow_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpageshow_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpageswap_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpageswap_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpopstate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpopstate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_getter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_setter);

    JS_DECLARE_NATIVE_FUNCTION(onstorage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstorage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_getter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_setter);

    JS_DECLARE_NATIVE_FUNCTION(onunload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onunload_setter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepadconnected_getter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepadconnected_setter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepaddisconnected_getter);

    JS_DECLARE_NATIVE_FUNCTION(ongamepaddisconnected_setter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(is_secure_context_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_isolated_getter);

    JS_DECLARE_NATIVE_FUNCTION(performance_getter);

    JS_DECLARE_NATIVE_FUNCTION(performance_setter);

    JS_DECLARE_NATIVE_FUNCTION(indexed_db_getter);

    JS_DECLARE_NATIVE_FUNCTION(crypto_getter);

    JS_DECLARE_NATIVE_FUNCTION(caches_getter);

    JS_DECLARE_NATIVE_FUNCTION(trusted_types_getter);

    JS_DECLARE_NATIVE_FUNCTION(local_storage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);

    JS_DECLARE_NATIVE_FUNCTION(onauxclick_getter);

    JS_DECLARE_NATIVE_FUNCTION(onauxclick_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeinput_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforeinput_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforematch_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforematch_setter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforetoggle_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbeforetoggle_setter);

    JS_DECLARE_NATIVE_FUNCTION(onblur_getter);

    JS_DECLARE_NATIVE_FUNCTION(onblur_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncancel_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncancel_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncanplay_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncanplay_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncanplaythrough_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncanplaythrough_setter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onclick_getter);

    JS_DECLARE_NATIVE_FUNCTION(onclick_setter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_getter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncommand_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncommand_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextlost_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextlost_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextmenu_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextmenu_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextrestored_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncontextrestored_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncopy_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncopy_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncuechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncuechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(oncut_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncut_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondblclick_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondblclick_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondrag_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondrag_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondragend_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondragend_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondragenter_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondragenter_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondragleave_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondragleave_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondragover_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondragover_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondragstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondragstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondrop_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondrop_setter);

    JS_DECLARE_NATIVE_FUNCTION(ondurationchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondurationchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onemptied_getter);

    JS_DECLARE_NATIVE_FUNCTION(onemptied_setter);

    JS_DECLARE_NATIVE_FUNCTION(onended_getter);

    JS_DECLARE_NATIVE_FUNCTION(onended_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfocus_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfocus_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfocusin_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfocusin_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfocusout_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfocusout_setter);

    JS_DECLARE_NATIVE_FUNCTION(onformdata_getter);

    JS_DECLARE_NATIVE_FUNCTION(onformdata_setter);

    JS_DECLARE_NATIVE_FUNCTION(oninput_getter);

    JS_DECLARE_NATIVE_FUNCTION(oninput_setter);

    JS_DECLARE_NATIVE_FUNCTION(oninvalid_getter);

    JS_DECLARE_NATIVE_FUNCTION(oninvalid_setter);

    JS_DECLARE_NATIVE_FUNCTION(onkeydown_getter);

    JS_DECLARE_NATIVE_FUNCTION(onkeydown_setter);

    JS_DECLARE_NATIVE_FUNCTION(onkeypress_getter);

    JS_DECLARE_NATIVE_FUNCTION(onkeypress_setter);

    JS_DECLARE_NATIVE_FUNCTION(onkeyup_getter);

    JS_DECLARE_NATIVE_FUNCTION(onkeyup_setter);

    JS_DECLARE_NATIVE_FUNCTION(onload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onload_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadeddata_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadeddata_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadedmetadata_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadedmetadata_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmousedown_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmousedown_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseenter_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseenter_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseleave_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseleave_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmousemove_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmousemove_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseout_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseout_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseover_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseover_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseup_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmouseup_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpaste_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpaste_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpause_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpause_setter);

    JS_DECLARE_NATIVE_FUNCTION(onplay_getter);

    JS_DECLARE_NATIVE_FUNCTION(onplay_setter);

    JS_DECLARE_NATIVE_FUNCTION(onplaying_getter);

    JS_DECLARE_NATIVE_FUNCTION(onplaying_setter);

    JS_DECLARE_NATIVE_FUNCTION(onprogress_getter);

    JS_DECLARE_NATIVE_FUNCTION(onprogress_setter);

    JS_DECLARE_NATIVE_FUNCTION(onratechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onratechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onreset_getter);

    JS_DECLARE_NATIVE_FUNCTION(onreset_setter);

    JS_DECLARE_NATIVE_FUNCTION(onresize_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresize_setter);

    JS_DECLARE_NATIVE_FUNCTION(onscroll_getter);

    JS_DECLARE_NATIVE_FUNCTION(onscroll_setter);

    JS_DECLARE_NATIVE_FUNCTION(onscrollend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onscrollend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsecuritypolicyviolation_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsecuritypolicyviolation_setter);

    JS_DECLARE_NATIVE_FUNCTION(onseeked_getter);

    JS_DECLARE_NATIVE_FUNCTION(onseeked_setter);

    JS_DECLARE_NATIVE_FUNCTION(onseeking_getter);

    JS_DECLARE_NATIVE_FUNCTION(onseeking_setter);

    JS_DECLARE_NATIVE_FUNCTION(onselect_getter);

    JS_DECLARE_NATIVE_FUNCTION(onselect_setter);

    JS_DECLARE_NATIVE_FUNCTION(onselectionchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onselectionchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onslotchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onslotchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onstalled_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstalled_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsubmit_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsubmit_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsuspend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsuspend_setter);

    JS_DECLARE_NATIVE_FUNCTION(ontimeupdate_getter);

    JS_DECLARE_NATIVE_FUNCTION(ontimeupdate_setter);

    JS_DECLARE_NATIVE_FUNCTION(ontoggle_getter);

    JS_DECLARE_NATIVE_FUNCTION(ontoggle_setter);

    JS_DECLARE_NATIVE_FUNCTION(onvolumechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onvolumechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwaiting_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwaiting_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationiteration_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationiteration_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkitanimationstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkittransitionend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwebkittransitionend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onwheel_getter);

    JS_DECLARE_NATIVE_FUNCTION(onwheel_setter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationcancel_getter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationcancel_setter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationiteration_getter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationiteration_setter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onanimationstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerover_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerover_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerenter_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerenter_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerdown_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerdown_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointermove_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointermove_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerrawupdate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerrawupdate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerup_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerup_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointercancel_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointercancel_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerout_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerout_setter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerleave_getter);

    JS_DECLARE_NATIVE_FUNCTION(onpointerleave_setter);

    JS_DECLARE_NATIVE_FUNCTION(ongotpointercapture_getter);

    JS_DECLARE_NATIVE_FUNCTION(ongotpointercapture_setter);

    JS_DECLARE_NATIVE_FUNCTION(onlostpointercapture_getter);

    JS_DECLARE_NATIVE_FUNCTION(onlostpointercapture_setter);

    JS_DECLARE_NATIVE_FUNCTION(session_storage_getter);


};


enum class ScrollBehavior : u8 {

    Auto,

    Instant,

    Smooth,

};

inline String idl_enum_to_string(ScrollBehavior value)
{
    switch (value) {

    case ScrollBehavior::Auto:
        return "auto"_string;

    case ScrollBehavior::Instant:
        return "instant"_string;

    case ScrollBehavior::Smooth:
        return "smooth"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    