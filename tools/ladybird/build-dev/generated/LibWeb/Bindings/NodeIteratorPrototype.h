
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NodeIteratorPrototype : public JS::Object {
    JS_OBJECT(NodeIteratorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NodeIteratorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NodeIteratorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NodeIteratorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(next_node);
        
    JS_DECLARE_NATIVE_FUNCTION(previous_node);
        
    JS_DECLARE_NATIVE_FUNCTION(detach);
        
    JS_DECLARE_NATIVE_FUNCTION(root_getter);

    JS_DECLARE_NATIVE_FUNCTION(reference_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_before_reference_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(what_to_show_getter);

    JS_DECLARE_NATIVE_FUNCTION(filter_getter);


};


} // namespace Web::Bindings
    