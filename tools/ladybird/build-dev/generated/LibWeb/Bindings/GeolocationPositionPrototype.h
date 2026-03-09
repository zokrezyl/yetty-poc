
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GeolocationPositionPrototype : public JS::Object {
    JS_OBJECT(GeolocationPositionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GeolocationPositionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GeolocationPositionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GeolocationPositionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(coords_getter);

    JS_DECLARE_NATIVE_FUNCTION(timestamp_getter);


};


} // namespace Web::Bindings
    