
#pragma once

#include <AK/FlyString.h>
#include <AK/Optional.h>

namespace Web::CSS {
enum class DimensionType : u8 {
    Angle,
    Flex,
    Frequency,
    Length,
    Resolution,
    Time,

};

Optional<DimensionType> dimension_for_unit(StringView);

enum class AngleUnit : u8 {
    Deg,
    Grad,
    Rad,
    Turn,

};
constexpr AngleUnit canonical_angle_unit() { return AngleUnit::Deg; }
Optional<AngleUnit> string_to_angle_unit(StringView);
FlyString to_string(AngleUnit);
bool units_are_compatible(AngleUnit, AngleUnit);
double ratio_between_units(AngleUnit, AngleUnit);

enum class FlexUnit : u8 {
    Fr,

};
constexpr FlexUnit canonical_flex_unit() { return FlexUnit::Fr; }
Optional<FlexUnit> string_to_flex_unit(StringView);
FlyString to_string(FlexUnit);
bool units_are_compatible(FlexUnit, FlexUnit);
double ratio_between_units(FlexUnit, FlexUnit);

enum class FrequencyUnit : u8 {
    Hz,
    KHz,

};
constexpr FrequencyUnit canonical_frequency_unit() { return FrequencyUnit::Hz; }
Optional<FrequencyUnit> string_to_frequency_unit(StringView);
FlyString to_string(FrequencyUnit);
bool units_are_compatible(FrequencyUnit, FrequencyUnit);
double ratio_between_units(FrequencyUnit, FrequencyUnit);

enum class LengthUnit : u8 {
    Cap,
    Ch,
    Cm,
    Dvb,
    Dvh,
    Dvi,
    Dvmax,
    Dvmin,
    Dvw,
    Em,
    Ex,
    Ic,
    In,
    Lh,
    Lvb,
    Lvh,
    Lvi,
    Lvmax,
    Lvmin,
    Lvw,
    Mm,
    Pc,
    Pt,
    Px,
    Q,
    Rcap,
    Rch,
    Rem,
    Rex,
    Ric,
    Rlh,
    Svb,
    Svh,
    Svi,
    Svmax,
    Svmin,
    Svw,
    Vb,
    Vh,
    Vi,
    Vmax,
    Vmin,
    Vw,

};
constexpr LengthUnit canonical_length_unit() { return LengthUnit::Px; }
Optional<LengthUnit> string_to_length_unit(StringView);
FlyString to_string(LengthUnit);
bool units_are_compatible(LengthUnit, LengthUnit);
double ratio_between_units(LengthUnit, LengthUnit);

enum class ResolutionUnit : u8 {
    Dpcm,
    Dpi,
    Dppx,
    X,

};
constexpr ResolutionUnit canonical_resolution_unit() { return ResolutionUnit::Dppx; }
Optional<ResolutionUnit> string_to_resolution_unit(StringView);
FlyString to_string(ResolutionUnit);
bool units_are_compatible(ResolutionUnit, ResolutionUnit);
double ratio_between_units(ResolutionUnit, ResolutionUnit);

enum class TimeUnit : u8 {
    Ms,
    S,

};
constexpr TimeUnit canonical_time_unit() { return TimeUnit::S; }
Optional<TimeUnit> string_to_time_unit(StringView);
FlyString to_string(TimeUnit);
bool units_are_compatible(TimeUnit, TimeUnit);
double ratio_between_units(TimeUnit, TimeUnit);

bool is_absolute(LengthUnit);
bool is_font_relative(LengthUnit);
bool is_viewport_relative(LengthUnit);
inline bool is_relative(LengthUnit unit) { return !is_absolute(unit); }

}
