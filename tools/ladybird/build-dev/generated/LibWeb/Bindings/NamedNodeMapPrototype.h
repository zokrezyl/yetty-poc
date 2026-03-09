
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NamedNodeMapPrototype : public JS::Object {
    JS_OBJECT(NamedNodeMapPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NamedNodeMapPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NamedNodeMapPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NamedNodeMapPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(get_named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(get_named_item_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(set_named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(set_named_item_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_named_item_ns);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    