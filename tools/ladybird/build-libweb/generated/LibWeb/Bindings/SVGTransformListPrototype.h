
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTransformListPrototype : public JS::Object {
    JS_OBJECT(SVGTransformListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTransformListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTransformListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTransformListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(clear);
        
    JS_DECLARE_NATIVE_FUNCTION(initialize_);
        
    JS_DECLARE_NATIVE_FUNCTION(get_item);
        
    JS_DECLARE_NATIVE_FUNCTION(insert_item_before);
        
    JS_DECLARE_NATIVE_FUNCTION(replace_item);
        
    JS_DECLARE_NATIVE_FUNCTION(remove_item);
        
    JS_DECLARE_NATIVE_FUNCTION(append_item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(number_of_items_getter);


};


} // namespace Web::Bindings
    