
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class RangePrototype : public JS::Object {
    JS_OBJECT(RangePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(RangePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit RangePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~RangePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_start);
        
    JS_DECLARE_NATIVE_FUNCTION(set_end);
        
    JS_DECLARE_NATIVE_FUNCTION(set_start_before);
        
    JS_DECLARE_NATIVE_FUNCTION(set_start_after);
        
    JS_DECLARE_NATIVE_FUNCTION(set_end_before);
        
    JS_DECLARE_NATIVE_FUNCTION(set_end_after);
        
    JS_DECLARE_NATIVE_FUNCTION(collapse);
        
    JS_DECLARE_NATIVE_FUNCTION(select_node);
        
    JS_DECLARE_NATIVE_FUNCTION(select_node_contents);
        
    JS_DECLARE_NATIVE_FUNCTION(compare_boundary_points);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_contents);
        
    JS_DECLARE_NATIVE_FUNCTION(extract_contents);
        
    JS_DECLARE_NATIVE_FUNCTION(clone_contents);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_node);
        
    JS_DECLARE_NATIVE_FUNCTION(surround_contents);
        
    JS_DECLARE_NATIVE_FUNCTION(clone_range);
        
    JS_DECLARE_NATIVE_FUNCTION(detach);
        
    JS_DECLARE_NATIVE_FUNCTION(is_point_in_range);
        
    JS_DECLARE_NATIVE_FUNCTION(compare_point);
        
    JS_DECLARE_NATIVE_FUNCTION(intersects_node);
        
    JS_DECLARE_NATIVE_FUNCTION(get_client_rects);
        
    JS_DECLARE_NATIVE_FUNCTION(get_bounding_client_rect);
        
    JS_DECLARE_NATIVE_FUNCTION(create_contextual_fragment);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(common_ancestor_container_getter);


};


} // namespace Web::Bindings
    