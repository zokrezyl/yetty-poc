
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SelectionPrototype : public JS::Object {
    JS_OBJECT(SelectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SelectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SelectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SelectionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_range_at);
        
    JS_DECLARE_NATIVE_FUNCTION(add_range);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_range);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_all_ranges);
        
    JS_DECLARE_NATIVE_FUNCTION(empty);
        
    JS_DECLARE_NATIVE_FUNCTION(collapse);
        
    JS_DECLARE_NATIVE_FUNCTION(set_position);
        
    JS_DECLARE_NATIVE_FUNCTION(collapse_to_start);
        
    JS_DECLARE_NATIVE_FUNCTION(collapse_to_end);
        
    JS_DECLARE_NATIVE_FUNCTION(extend);
        
    JS_DECLARE_NATIVE_FUNCTION(set_base_and_extent);
        
    JS_DECLARE_NATIVE_FUNCTION(select_all_children);
        
    JS_DECLARE_NATIVE_FUNCTION(modify);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_from_document);
        
    JS_DECLARE_NATIVE_FUNCTION(contains_node);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        
    JS_DECLARE_NATIVE_FUNCTION(anchor_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(anchor_offset_getter);

    JS_DECLARE_NATIVE_FUNCTION(focus_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(focus_offset_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_collapsed_getter);

    JS_DECLARE_NATIVE_FUNCTION(range_count_getter);

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(direction_getter);


};


} // namespace Web::Bindings
    