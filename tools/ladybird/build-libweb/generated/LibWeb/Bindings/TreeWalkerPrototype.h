
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TreeWalkerPrototype : public JS::Object {
    JS_OBJECT(TreeWalkerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TreeWalkerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TreeWalkerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TreeWalkerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(parent_node);
        
    JS_DECLARE_NATIVE_FUNCTION(first_child);
        
    JS_DECLARE_NATIVE_FUNCTION(last_child);
        
    JS_DECLARE_NATIVE_FUNCTION(previous_sibling);
        
    JS_DECLARE_NATIVE_FUNCTION(next_sibling);
        
    JS_DECLARE_NATIVE_FUNCTION(previous_node);
        
    JS_DECLARE_NATIVE_FUNCTION(next_node);
        
    JS_DECLARE_NATIVE_FUNCTION(root_getter);

    JS_DECLARE_NATIVE_FUNCTION(what_to_show_getter);

    JS_DECLARE_NATIVE_FUNCTION(filter_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_node_setter);


};


} // namespace Web::Bindings
    