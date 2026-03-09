
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTableElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTableElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTableElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTableElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_caption);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_caption);
        
    JS_DECLARE_NATIVE_FUNCTION(create_t_head);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_t_head);
        
    JS_DECLARE_NATIVE_FUNCTION(create_t_foot);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_t_foot);
        
    JS_DECLARE_NATIVE_FUNCTION(create_t_body);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_row);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_row);
        
    JS_DECLARE_NATIVE_FUNCTION(caption_getter);

    JS_DECLARE_NATIVE_FUNCTION(caption_setter);

    JS_DECLARE_NATIVE_FUNCTION(t_head_getter);

    JS_DECLARE_NATIVE_FUNCTION(t_head_setter);

    JS_DECLARE_NATIVE_FUNCTION(t_foot_getter);

    JS_DECLARE_NATIVE_FUNCTION(t_foot_setter);

    JS_DECLARE_NATIVE_FUNCTION(t_bodies_getter);

    JS_DECLARE_NATIVE_FUNCTION(rows_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(border_getter);

    JS_DECLARE_NATIVE_FUNCTION(border_setter);

    JS_DECLARE_NATIVE_FUNCTION(frame_getter);

    JS_DECLARE_NATIVE_FUNCTION(frame_setter);

    JS_DECLARE_NATIVE_FUNCTION(rules_getter);

    JS_DECLARE_NATIVE_FUNCTION(rules_setter);

    JS_DECLARE_NATIVE_FUNCTION(summary_getter);

    JS_DECLARE_NATIVE_FUNCTION(summary_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);

    JS_DECLARE_NATIVE_FUNCTION(cell_padding_getter);

    JS_DECLARE_NATIVE_FUNCTION(cell_padding_setter);

    JS_DECLARE_NATIVE_FUNCTION(cell_spacing_getter);

    JS_DECLARE_NATIVE_FUNCTION(cell_spacing_setter);


};


} // namespace Web::Bindings
    