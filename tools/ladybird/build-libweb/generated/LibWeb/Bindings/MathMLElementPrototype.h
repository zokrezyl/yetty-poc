
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MathMLElementPrototype : public JS::Object {
    JS_OBJECT(MathMLElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MathMLElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MathMLElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MathMLElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(focus);
        
    JS_DECLARE_NATIVE_FUNCTION(blur);
        
    JS_DECLARE_NATIVE_FUNCTION(dataset_getter);

    JS_DECLARE_NATIVE_FUNCTION(nonce_getter);

    JS_DECLARE_NATIVE_FUNCTION(nonce_setter);

    JS_DECLARE_NATIVE_FUNCTION(autofocus_getter);

    JS_DECLARE_NATIVE_FUNCTION(autofocus_setter);

    JS_DECLARE_NATIVE_FUNCTION(tab_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(tab_index_setter);

    JS_DECLARE_NATIVE_FUNCTION(style_getter);

    JS_DECLARE_NATIVE_FUNCTION(style_setter);

    JS_DECLARE_NATIVE_FUNCTION(attribute_style_map_getter);

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


};


} // namespace Web::Bindings
    