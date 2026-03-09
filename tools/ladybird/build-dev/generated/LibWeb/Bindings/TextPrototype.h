
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextPrototype : public JS::Object {
    JS_OBJECT(TextPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(split_text);
        
    JS_DECLARE_NATIVE_FUNCTION(whole_text_getter);

    JS_DECLARE_NATIVE_FUNCTION(assigned_slot_getter);


};


} // namespace Web::Bindings
    