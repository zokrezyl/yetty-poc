
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLSlotElementPrototype : public JS::Object {
    JS_OBJECT(HTMLSlotElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLSlotElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLSlotElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLSlotElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(assigned_nodes);
        
    JS_DECLARE_NATIVE_FUNCTION(assigned_elements);
        
    JS_DECLARE_NATIVE_FUNCTION(assign);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);


};


} // namespace Web::Bindings
    