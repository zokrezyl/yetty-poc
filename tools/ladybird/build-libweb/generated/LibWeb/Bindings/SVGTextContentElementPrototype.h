
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGTextContentElementPrototype : public JS::Object {
    JS_OBJECT(SVGTextContentElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGTextContentElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGTextContentElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGTextContentElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_number_of_chars);
        
    JS_DECLARE_NATIVE_FUNCTION(get_start_position_of_char);
        

};


} // namespace Web::Bindings
    