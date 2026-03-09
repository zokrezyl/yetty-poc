
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DocumentPrototype : public JS::Object {
    JS_OBJECT(DocumentPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DocumentPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DocumentPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DocumentPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(has_focus);
        
    JS_DECLARE_NATIVE_FUNCTION(open);
        
    JS_DECLARE_NATIVE_FUNCTION(open0);

    JS_DECLARE_NATIVE_FUNCTION(open1);

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(write);
        
    JS_DECLARE_NATIVE_FUNCTION(writeln);
        
    JS_DECLARE_NATIVE_FUNCTION(get_element_by_id);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_name);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_tag_name);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_tag_name_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(get_elements_by_class_name);
        
    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(capture_events);
        
    JS_DECLARE_NATIVE_FUNCTION(release_events);
        
    JS_DECLARE_NATIVE_FUNCTION(create_element);
        
    JS_DECLARE_NATIVE_FUNCTION(create_element_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(create_document_fragment);
        
    JS_DECLARE_NATIVE_FUNCTION(create_text_node);
        
    JS_DECLARE_NATIVE_FUNCTION(create_cdata_section);
        
    JS_DECLARE_NATIVE_FUNCTION(create_comment);
        
    JS_DECLARE_NATIVE_FUNCTION(create_processing_instruction);
        
    JS_DECLARE_NATIVE_FUNCTION(create_attribute);
        
    JS_DECLARE_NATIVE_FUNCTION(create_attribute_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(create_range);
        
    JS_DECLARE_NATIVE_FUNCTION(create_event);
        
    JS_DECLARE_NATIVE_FUNCTION(import_node);
        
    JS_DECLARE_NATIVE_FUNCTION(adopt_node);
        
    JS_DECLARE_NATIVE_FUNCTION(create_node_iterator);
        
    JS_DECLARE_NATIVE_FUNCTION(create_tree_walker);
        
    JS_DECLARE_NATIVE_FUNCTION(get_selection);
        
    JS_DECLARE_NATIVE_FUNCTION(element_from_point);
        
    JS_DECLARE_NATIVE_FUNCTION(elements_from_point);
        
    JS_DECLARE_NATIVE_FUNCTION(exec_command);
        
    JS_DECLARE_NATIVE_FUNCTION(query_command_enabled);
        
    JS_DECLARE_NATIVE_FUNCTION(query_command_indeterm);
        
    JS_DECLARE_NATIVE_FUNCTION(query_command_state);
        
    JS_DECLARE_NATIVE_FUNCTION(query_command_supported);
        
    JS_DECLARE_NATIVE_FUNCTION(query_command_value);
        
    JS_DECLARE_NATIVE_FUNCTION(create_expression);
        
    JS_DECLARE_NATIVE_FUNCTION(evaluate);
        
    JS_DECLARE_NATIVE_FUNCTION(create_ns_resolver);
        
    JS_DECLARE_NATIVE_FUNCTION(start_view_transition);
        
    JS_DECLARE_NATIVE_FUNCTION(exit_pointer_lock);
        
    JS_DECLARE_NATIVE_FUNCTION(exit_fullscreen);
        
    JS_DECLARE_NATIVE_FUNCTION(get_animations);
        
    JS_DECLARE_NATIVE_FUNCTION(prepend);
        
    JS_DECLARE_NATIVE_FUNCTION(append);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_children);
        
    JS_DECLARE_NATIVE_FUNCTION(move_before);
        
    JS_DECLARE_NATIVE_FUNCTION(query_selector);
        
    JS_DECLARE_NATIVE_FUNCTION(query_selector_all);
        
    JS_DECLARE_NATIVE_FUNCTION(location_getter);

    JS_DECLARE_NATIVE_FUNCTION(location_setter);

    JS_DECLARE_NATIVE_FUNCTION(domain_getter);

    JS_DECLARE_NATIVE_FUNCTION(domain_setter);

    JS_DECLARE_NATIVE_FUNCTION(implementation_getter);

    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(document_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(character_set_getter);

    JS_DECLARE_NATIVE_FUNCTION(charset_getter);

    JS_DECLARE_NATIVE_FUNCTION(input_encoding_getter);

    JS_DECLARE_NATIVE_FUNCTION(content_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_view_getter);

    JS_DECLARE_NATIVE_FUNCTION(cookie_getter);

    JS_DECLARE_NATIVE_FUNCTION(cookie_setter);

    JS_DECLARE_NATIVE_FUNCTION(fg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(fg_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(link_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(link_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(vlink_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(vlink_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(alink_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(alink_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_getter);

    JS_DECLARE_NATIVE_FUNCTION(applets_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchors_getter);

    JS_DECLARE_NATIVE_FUNCTION(images_getter);

    JS_DECLARE_NATIVE_FUNCTION(embeds_getter);

    JS_DECLARE_NATIVE_FUNCTION(plugins_getter);

    JS_DECLARE_NATIVE_FUNCTION(links_getter);

    JS_DECLARE_NATIVE_FUNCTION(forms_getter);

    JS_DECLARE_NATIVE_FUNCTION(scripts_getter);

    JS_DECLARE_NATIVE_FUNCTION(all_getter);

    JS_DECLARE_NATIVE_FUNCTION(compat_mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(doctype_getter);

    JS_DECLARE_NATIVE_FUNCTION(document_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(dir_getter);

    JS_DECLARE_NATIVE_FUNCTION(dir_setter);

    JS_DECLARE_NATIVE_FUNCTION(body_getter);

    JS_DECLARE_NATIVE_FUNCTION(body_setter);

    JS_DECLARE_NATIVE_FUNCTION(head_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_script_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_modified_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(title_getter);

    JS_DECLARE_NATIVE_FUNCTION(title_setter);

    JS_DECLARE_NATIVE_FUNCTION(hidden_getter);

    JS_DECLARE_NATIVE_FUNCTION(visibility_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(design_mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(design_mode_setter);

    JS_DECLARE_NATIVE_FUNCTION(timeline_getter);

    JS_DECLARE_NATIVE_FUNCTION(scrolling_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(onreadystatechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onreadystatechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onvisibilitychange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onvisibilitychange_setter);

    JS_DECLARE_NATIVE_FUNCTION(root_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_enabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_enabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_getter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onfullscreenerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(active_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_sheets_getter);

    JS_DECLARE_NATIVE_FUNCTION(adopted_style_sheets_getter);

    JS_DECLARE_NATIVE_FUNCTION(adopted_style_sheets_setter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(fullscreen_element_setter);

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

    JS_DECLARE_NATIVE_FUNCTION(fonts_getter);

    JS_DECLARE_NATIVE_FUNCTION(children_getter);

    JS_DECLARE_NATIVE_FUNCTION(first_element_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_element_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(child_element_count_getter);


};


} // namespace Web::Bindings
    