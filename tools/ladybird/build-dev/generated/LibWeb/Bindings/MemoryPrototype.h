
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MemoryPrototype : public JS::Object {
    JS_OBJECT(MemoryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MemoryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MemoryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MemoryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(grow);
        
    JS_DECLARE_NATIVE_FUNCTION(to_fixed_length_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(to_resizable_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(buffer_getter);


};


} // namespace Web::Bindings
    