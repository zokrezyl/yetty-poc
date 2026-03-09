
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class InputEventPrototype : public JS::Object {
    JS_OBJECT(InputEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(InputEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit InputEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~InputEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_target_ranges);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_composing_getter);

    JS_DECLARE_NATIVE_FUNCTION(input_type_getter);


};


} // namespace Web::Bindings
    