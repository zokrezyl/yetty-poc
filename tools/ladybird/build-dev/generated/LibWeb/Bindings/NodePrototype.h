
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NodePrototype : public JS::Object {
    JS_OBJECT(NodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_root_node);
        
    JS_DECLARE_NATIVE_FUNCTION(has_child_nodes);
        
    JS_DECLARE_NATIVE_FUNCTION(normalize);
        
    JS_DECLARE_NATIVE_FUNCTION(clone_node);
        
    JS_DECLARE_NATIVE_FUNCTION(is_equal_node);
        
    JS_DECLARE_NATIVE_FUNCTION(is_same_node);
        
    JS_DECLARE_NATIVE_FUNCTION(compare_document_position);
        
    JS_DECLARE_NATIVE_FUNCTION(contains);
        
    JS_DECLARE_NATIVE_FUNCTION(lookup_prefix);
        
    JS_DECLARE_NATIVE_FUNCTION(lookup_namespace_uri);
        
    JS_DECLARE_NATIVE_FUNCTION(is_default_namespace);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_before);
        
    JS_DECLARE_NATIVE_FUNCTION(append_child);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_child);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_child);
        
    JS_DECLARE_NATIVE_FUNCTION(node_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(node_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(base_uri_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_connected_getter);

    JS_DECLARE_NATIVE_FUNCTION(owner_document_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(child_nodes_getter);

    JS_DECLARE_NATIVE_FUNCTION(first_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(last_child_getter);

    JS_DECLARE_NATIVE_FUNCTION(previous_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(next_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(node_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(node_value_setter);

    JS_DECLARE_NATIVE_FUNCTION(text_content_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_content_setter);


};


} // namespace Web::Bindings
    