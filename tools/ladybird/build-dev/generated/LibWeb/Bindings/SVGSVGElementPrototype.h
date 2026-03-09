
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGSVGElementPrototype : public JS::Object {
    JS_OBJECT(SVGSVGElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGSVGElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGSVGElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGSVGElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_intersection_list);
        
    JS_DECLARE_NATIVE_FUNCTION(get_enclosure_list);
        
    JS_DECLARE_NATIVE_FUNCTION(check_intersection);
        
    JS_DECLARE_NATIVE_FUNCTION(check_enclosure);
        
    JS_DECLARE_NATIVE_FUNCTION(deselect_all);
        
    JS_DECLARE_NATIVE_FUNCTION(create_svg_length);
        
    JS_DECLARE_NATIVE_FUNCTION(create_svg_point);
        
    JS_DECLARE_NATIVE_FUNCTION(create_svg_matrix);
        
    JS_DECLARE_NATIVE_FUNCTION(create_svg_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(create_svg_transform);
        
    JS_DECLARE_NATIVE_FUNCTION(get_element_by_id);
        
    JS_DECLARE_NATIVE_FUNCTION(suspend_redraw);
        
    JS_DECLARE_NATIVE_FUNCTION(unsuspend_redraw);
        
    JS_DECLARE_NATIVE_FUNCTION(unsuspend_redraw_all);
        
    JS_DECLARE_NATIVE_FUNCTION(force_redraw);
        
    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_scale_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_scale_setter);

    JS_DECLARE_NATIVE_FUNCTION(current_translate_getter);

    JS_DECLARE_NATIVE_FUNCTION(view_box_getter);


};


} // namespace Web::Bindings
    