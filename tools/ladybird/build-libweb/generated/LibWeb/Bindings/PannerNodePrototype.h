
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PannerNodePrototype : public JS::Object {
    JS_OBJECT(PannerNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PannerNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PannerNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PannerNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_position);
        
    JS_DECLARE_NATIVE_FUNCTION(set_orientation);
        
    JS_DECLARE_NATIVE_FUNCTION(panning_model_getter);

    JS_DECLARE_NATIVE_FUNCTION(panning_model_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_z_getter);

    JS_DECLARE_NATIVE_FUNCTION(orientation_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(orientation_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(orientation_z_getter);

    JS_DECLARE_NATIVE_FUNCTION(distance_model_getter);

    JS_DECLARE_NATIVE_FUNCTION(distance_model_setter);

    JS_DECLARE_NATIVE_FUNCTION(ref_distance_getter);

    JS_DECLARE_NATIVE_FUNCTION(ref_distance_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_distance_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_distance_setter);

    JS_DECLARE_NATIVE_FUNCTION(rolloff_factor_getter);

    JS_DECLARE_NATIVE_FUNCTION(rolloff_factor_setter);

    JS_DECLARE_NATIVE_FUNCTION(cone_inner_angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(cone_inner_angle_setter);

    JS_DECLARE_NATIVE_FUNCTION(cone_outer_angle_getter);

    JS_DECLARE_NATIVE_FUNCTION(cone_outer_angle_setter);

    JS_DECLARE_NATIVE_FUNCTION(cone_outer_gain_getter);

    JS_DECLARE_NATIVE_FUNCTION(cone_outer_gain_setter);


};


enum class DistanceModelType : u8 {

    Linear,

    Inverse,

    Exponential,

};

inline String idl_enum_to_string(DistanceModelType value)
{
    switch (value) {

    case DistanceModelType::Linear:
        return "linear"_string;

    case DistanceModelType::Inverse:
        return "inverse"_string;

    case DistanceModelType::Exponential:
        return "exponential"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class PanningModelType : u8 {

    Equalpower,

    Hrtf,

};

inline String idl_enum_to_string(PanningModelType value)
{
    switch (value) {

    case PanningModelType::Equalpower:
        return "equalpower"_string;

    case PanningModelType::Hrtf:
        return "HRTF"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    