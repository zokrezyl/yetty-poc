
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class XPathResultPrototype : public JS::Object {
    JS_OBJECT(XPathResultPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(XPathResultPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit XPathResultPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~XPathResultPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(iterate_next);
        
    JS_DECLARE_NATIVE_FUNCTION(snapshot_item);
        
    JS_DECLARE_NATIVE_FUNCTION(result_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(number_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(string_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(boolean_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(single_node_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(invalid_iterator_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(snapshot_length_getter);


};


} // namespace Web::Bindings
    