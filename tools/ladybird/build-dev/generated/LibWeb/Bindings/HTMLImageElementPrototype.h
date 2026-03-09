
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLImageElementPrototype : public JS::Object {
    JS_OBJECT(HTMLImageElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLImageElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLImageElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLImageElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(decode);
        
    JS_DECLARE_NATIVE_FUNCTION(alt_getter);

    JS_DECLARE_NATIVE_FUNCTION(alt_setter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(srcset_getter);

    JS_DECLARE_NATIVE_FUNCTION(srcset_setter);

    JS_DECLARE_NATIVE_FUNCTION(sizes_getter);

    JS_DECLARE_NATIVE_FUNCTION(sizes_setter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_getter);

    JS_DECLARE_NATIVE_FUNCTION(use_map_setter);

    JS_DECLARE_NATIVE_FUNCTION(is_map_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_map_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(natural_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(natural_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(complete_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_src_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_getter);

    JS_DECLARE_NATIVE_FUNCTION(referrer_policy_setter);

    JS_DECLARE_NATIVE_FUNCTION(decoding_getter);

    JS_DECLARE_NATIVE_FUNCTION(decoding_setter);

    JS_DECLARE_NATIVE_FUNCTION(loading_getter);

    JS_DECLARE_NATIVE_FUNCTION(loading_setter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_getter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_priority_setter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(lowsrc_getter);

    JS_DECLARE_NATIVE_FUNCTION(lowsrc_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(hspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_getter);

    JS_DECLARE_NATIVE_FUNCTION(vspace_setter);

    JS_DECLARE_NATIVE_FUNCTION(long_desc_getter);

    JS_DECLARE_NATIVE_FUNCTION(long_desc_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_setter);

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);


};


enum class Decoding : u8 {

    Sync,

    Async,

    Auto,

};

inline String idl_enum_to_string(Decoding value)
{
    switch (value) {

    case Decoding::Sync:
        return "sync"_string;

    case Decoding::Async:
        return "async"_string;

    case Decoding::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    