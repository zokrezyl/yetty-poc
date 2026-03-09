
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GeolocationCoordinatesPrototype : public JS::Object {
    JS_OBJECT(GeolocationCoordinatesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GeolocationCoordinatesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GeolocationCoordinatesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GeolocationCoordinatesPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(accuracy_getter);

    JS_DECLARE_NATIVE_FUNCTION(latitude_getter);

    JS_DECLARE_NATIVE_FUNCTION(longitude_getter);

    JS_DECLARE_NATIVE_FUNCTION(altitude_getter);

    JS_DECLARE_NATIVE_FUNCTION(altitude_accuracy_getter);

    JS_DECLARE_NATIVE_FUNCTION(heading_getter);

    JS_DECLARE_NATIVE_FUNCTION(speed_getter);


};


} // namespace Web::Bindings
    