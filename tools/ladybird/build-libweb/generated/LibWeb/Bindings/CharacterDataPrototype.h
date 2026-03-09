
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CharacterDataPrototype : public JS::Object {
    JS_OBJECT(CharacterDataPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CharacterDataPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CharacterDataPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CharacterDataPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(substring_data);
        
    JS_DECLARE_NATIVE_FUNCTION(append_data);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_data);
        
    JS_DECLARE_NATIVE_FUNCTION(delete_data);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_data);
        
    JS_DECLARE_NATIVE_FUNCTION(before);
        
    JS_DECLARE_NATIVE_FUNCTION(after);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_with);
        
    JS_DECLARE_NATIVE_FUNCTION(remove);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(data_setter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(previous_element_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(next_element_sibling_getter);


};


} // namespace Web::Bindings
    