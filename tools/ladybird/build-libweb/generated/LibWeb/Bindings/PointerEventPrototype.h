
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PointerEventPrototype : public JS::Object {
    JS_OBJECT(PointerEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PointerEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PointerEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PointerEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_coalesced_events);
        
    JS_DECLARE_NATIVE_FUNCTION(get_predicted_events);
        
    JS_DECLARE_NATIVE_FUNCTION(pointer_id_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(pressure_getter);

    JS_DECLARE_NATIVE_FUNCTION(tangential_pressure_getter);

    JS_DECLARE_NATIVE_FUNCTION(tilt_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(tilt_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(twist_getter);

    JS_DECLARE_NATIVE_FUNCTION(altitude_angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(azimuth_angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(pointer_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_primary_getter);

    JS_DECLARE_NATIVE_FUNCTION(persistent_device_id_getter);


};


} // namespace Web::Bindings
    