
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGGeometryElementPrototype : public JS::Object {
    JS_OBJECT(SVGGeometryElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGGeometryElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGGeometryElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGGeometryElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_total_length);
        
    JS_DECLARE_NATIVE_FUNCTION(get_point_at_length);
        
    JS_DECLARE_NATIVE_FUNCTION(path_length_getter);


};


} // namespace Web::Bindings
    