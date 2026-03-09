
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ClipboardPrototype : public JS::Object {
    JS_OBJECT(ClipboardPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ClipboardPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ClipboardPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ClipboardPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(read);
        
    JS_DECLARE_NATIVE_FUNCTION(read_text);
        
    JS_DECLARE_NATIVE_FUNCTION(write);
        
    JS_DECLARE_NATIVE_FUNCTION(write_text);
        

};


} // namespace Web::Bindings
    