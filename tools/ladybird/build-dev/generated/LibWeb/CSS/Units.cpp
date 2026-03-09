
#include <LibWeb/CSS/Units.h>

namespace Web::CSS {

Optional<DimensionType> dimension_for_unit(StringView unit_name)
{
    if (unit_name.equals_ignoring_ascii_case("deg"sv)
         || unit_name.equals_ignoring_ascii_case("grad"sv)
         || unit_name.equals_ignoring_ascii_case("rad"sv)
         || unit_name.equals_ignoring_ascii_case("turn"sv))
        return DimensionType::Angle;
    if (unit_name.equals_ignoring_ascii_case("fr"sv))
        return DimensionType::Flex;
    if (unit_name.equals_ignoring_ascii_case("Hz"sv)
         || unit_name.equals_ignoring_ascii_case("kHz"sv))
        return DimensionType::Frequency;
    if (unit_name.equals_ignoring_ascii_case("cap"sv)
         || unit_name.equals_ignoring_ascii_case("ch"sv)
         || unit_name.equals_ignoring_ascii_case("cm"sv)
         || unit_name.equals_ignoring_ascii_case("dvb"sv)
         || unit_name.equals_ignoring_ascii_case("dvh"sv)
         || unit_name.equals_ignoring_ascii_case("dvi"sv)
         || unit_name.equals_ignoring_ascii_case("dvmax"sv)
         || unit_name.equals_ignoring_ascii_case("dvmin"sv)
         || unit_name.equals_ignoring_ascii_case("dvw"sv)
         || unit_name.equals_ignoring_ascii_case("em"sv)
         || unit_name.equals_ignoring_ascii_case("ex"sv)
         || unit_name.equals_ignoring_ascii_case("ic"sv)
         || unit_name.equals_ignoring_ascii_case("in"sv)
         || unit_name.equals_ignoring_ascii_case("lh"sv)
         || unit_name.equals_ignoring_ascii_case("lvb"sv)
         || unit_name.equals_ignoring_ascii_case("lvh"sv)
         || unit_name.equals_ignoring_ascii_case("lvi"sv)
         || unit_name.equals_ignoring_ascii_case("lvmax"sv)
         || unit_name.equals_ignoring_ascii_case("lvmin"sv)
         || unit_name.equals_ignoring_ascii_case("lvw"sv)
         || unit_name.equals_ignoring_ascii_case("mm"sv)
         || unit_name.equals_ignoring_ascii_case("pc"sv)
         || unit_name.equals_ignoring_ascii_case("pt"sv)
         || unit_name.equals_ignoring_ascii_case("px"sv)
         || unit_name.equals_ignoring_ascii_case("Q"sv)
         || unit_name.equals_ignoring_ascii_case("rcap"sv)
         || unit_name.equals_ignoring_ascii_case("rch"sv)
         || unit_name.equals_ignoring_ascii_case("rem"sv)
         || unit_name.equals_ignoring_ascii_case("rex"sv)
         || unit_name.equals_ignoring_ascii_case("ric"sv)
         || unit_name.equals_ignoring_ascii_case("rlh"sv)
         || unit_name.equals_ignoring_ascii_case("svb"sv)
         || unit_name.equals_ignoring_ascii_case("svh"sv)
         || unit_name.equals_ignoring_ascii_case("svi"sv)
         || unit_name.equals_ignoring_ascii_case("svmax"sv)
         || unit_name.equals_ignoring_ascii_case("svmin"sv)
         || unit_name.equals_ignoring_ascii_case("svw"sv)
         || unit_name.equals_ignoring_ascii_case("vb"sv)
         || unit_name.equals_ignoring_ascii_case("vh"sv)
         || unit_name.equals_ignoring_ascii_case("vi"sv)
         || unit_name.equals_ignoring_ascii_case("vmax"sv)
         || unit_name.equals_ignoring_ascii_case("vmin"sv)
         || unit_name.equals_ignoring_ascii_case("vw"sv))
        return DimensionType::Length;
    if (unit_name.equals_ignoring_ascii_case("dpcm"sv)
         || unit_name.equals_ignoring_ascii_case("dpi"sv)
         || unit_name.equals_ignoring_ascii_case("dppx"sv)
         || unit_name.equals_ignoring_ascii_case("x"sv))
        return DimensionType::Resolution;
    if (unit_name.equals_ignoring_ascii_case("ms"sv)
         || unit_name.equals_ignoring_ascii_case("s"sv))
        return DimensionType::Time;

    return {};
}

Optional<AngleUnit> string_to_angle_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("deg"sv))
        return AngleUnit::Deg;
    if (unit_name.equals_ignoring_ascii_case("grad"sv))
        return AngleUnit::Grad;
    if (unit_name.equals_ignoring_ascii_case("rad"sv))
        return AngleUnit::Rad;
    if (unit_name.equals_ignoring_ascii_case("turn"sv))
        return AngleUnit::Turn;
    return {};
}

FlyString to_string(AngleUnit value)
{
    switch (value) {
    case AngleUnit::Deg:
        return "deg"_fly_string;
    case AngleUnit::Grad:
        return "grad"_fly_string;
    case AngleUnit::Rad:
        return "rad"_fly_string;
    case AngleUnit::Turn:
        return "turn"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(AngleUnit a, AngleUnit b)
{
    auto is_absolute = [](AngleUnit unit) -> bool {
        switch (unit) {
        case AngleUnit::Deg:
        case AngleUnit::Grad:
        case AngleUnit::Rad:
        case AngleUnit::Turn:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(AngleUnit from, AngleUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](AngleUnit unit) -> double {
        switch (unit) {

        case AngleUnit::Deg:
            return 1;

        case AngleUnit::Grad:
            return 0.9;

        case AngleUnit::Rad:
            return 57.29577951308232;

        case AngleUnit::Turn:
            return 360;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == AngleUnit::Deg)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

Optional<FlexUnit> string_to_flex_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("fr"sv))
        return FlexUnit::Fr;
    return {};
}

FlyString to_string(FlexUnit value)
{
    switch (value) {
    case FlexUnit::Fr:
        return "fr"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(FlexUnit a, FlexUnit b)
{
    auto is_absolute = [](FlexUnit unit) -> bool {
        switch (unit) {
        case FlexUnit::Fr:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(FlexUnit from, FlexUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](FlexUnit unit) -> double {
        switch (unit) {

        case FlexUnit::Fr:
            return 1;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == FlexUnit::Fr)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

Optional<FrequencyUnit> string_to_frequency_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("Hz"sv))
        return FrequencyUnit::Hz;
    if (unit_name.equals_ignoring_ascii_case("kHz"sv))
        return FrequencyUnit::KHz;
    return {};
}

FlyString to_string(FrequencyUnit value)
{
    switch (value) {
    case FrequencyUnit::Hz:
        return "Hz"_fly_string;
    case FrequencyUnit::KHz:
        return "kHz"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(FrequencyUnit a, FrequencyUnit b)
{
    auto is_absolute = [](FrequencyUnit unit) -> bool {
        switch (unit) {
        case FrequencyUnit::Hz:
        case FrequencyUnit::KHz:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(FrequencyUnit from, FrequencyUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](FrequencyUnit unit) -> double {
        switch (unit) {

        case FrequencyUnit::Hz:
            return 1;

        case FrequencyUnit::KHz:
            return 1000;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == FrequencyUnit::Hz)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

Optional<LengthUnit> string_to_length_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("cap"sv))
        return LengthUnit::Cap;
    if (unit_name.equals_ignoring_ascii_case("ch"sv))
        return LengthUnit::Ch;
    if (unit_name.equals_ignoring_ascii_case("cm"sv))
        return LengthUnit::Cm;
    if (unit_name.equals_ignoring_ascii_case("dvb"sv))
        return LengthUnit::Dvb;
    if (unit_name.equals_ignoring_ascii_case("dvh"sv))
        return LengthUnit::Dvh;
    if (unit_name.equals_ignoring_ascii_case("dvi"sv))
        return LengthUnit::Dvi;
    if (unit_name.equals_ignoring_ascii_case("dvmax"sv))
        return LengthUnit::Dvmax;
    if (unit_name.equals_ignoring_ascii_case("dvmin"sv))
        return LengthUnit::Dvmin;
    if (unit_name.equals_ignoring_ascii_case("dvw"sv))
        return LengthUnit::Dvw;
    if (unit_name.equals_ignoring_ascii_case("em"sv))
        return LengthUnit::Em;
    if (unit_name.equals_ignoring_ascii_case("ex"sv))
        return LengthUnit::Ex;
    if (unit_name.equals_ignoring_ascii_case("ic"sv))
        return LengthUnit::Ic;
    if (unit_name.equals_ignoring_ascii_case("in"sv))
        return LengthUnit::In;
    if (unit_name.equals_ignoring_ascii_case("lh"sv))
        return LengthUnit::Lh;
    if (unit_name.equals_ignoring_ascii_case("lvb"sv))
        return LengthUnit::Lvb;
    if (unit_name.equals_ignoring_ascii_case("lvh"sv))
        return LengthUnit::Lvh;
    if (unit_name.equals_ignoring_ascii_case("lvi"sv))
        return LengthUnit::Lvi;
    if (unit_name.equals_ignoring_ascii_case("lvmax"sv))
        return LengthUnit::Lvmax;
    if (unit_name.equals_ignoring_ascii_case("lvmin"sv))
        return LengthUnit::Lvmin;
    if (unit_name.equals_ignoring_ascii_case("lvw"sv))
        return LengthUnit::Lvw;
    if (unit_name.equals_ignoring_ascii_case("mm"sv))
        return LengthUnit::Mm;
    if (unit_name.equals_ignoring_ascii_case("pc"sv))
        return LengthUnit::Pc;
    if (unit_name.equals_ignoring_ascii_case("pt"sv))
        return LengthUnit::Pt;
    if (unit_name.equals_ignoring_ascii_case("px"sv))
        return LengthUnit::Px;
    if (unit_name.equals_ignoring_ascii_case("Q"sv))
        return LengthUnit::Q;
    if (unit_name.equals_ignoring_ascii_case("rcap"sv))
        return LengthUnit::Rcap;
    if (unit_name.equals_ignoring_ascii_case("rch"sv))
        return LengthUnit::Rch;
    if (unit_name.equals_ignoring_ascii_case("rem"sv))
        return LengthUnit::Rem;
    if (unit_name.equals_ignoring_ascii_case("rex"sv))
        return LengthUnit::Rex;
    if (unit_name.equals_ignoring_ascii_case("ric"sv))
        return LengthUnit::Ric;
    if (unit_name.equals_ignoring_ascii_case("rlh"sv))
        return LengthUnit::Rlh;
    if (unit_name.equals_ignoring_ascii_case("svb"sv))
        return LengthUnit::Svb;
    if (unit_name.equals_ignoring_ascii_case("svh"sv))
        return LengthUnit::Svh;
    if (unit_name.equals_ignoring_ascii_case("svi"sv))
        return LengthUnit::Svi;
    if (unit_name.equals_ignoring_ascii_case("svmax"sv))
        return LengthUnit::Svmax;
    if (unit_name.equals_ignoring_ascii_case("svmin"sv))
        return LengthUnit::Svmin;
    if (unit_name.equals_ignoring_ascii_case("svw"sv))
        return LengthUnit::Svw;
    if (unit_name.equals_ignoring_ascii_case("vb"sv))
        return LengthUnit::Vb;
    if (unit_name.equals_ignoring_ascii_case("vh"sv))
        return LengthUnit::Vh;
    if (unit_name.equals_ignoring_ascii_case("vi"sv))
        return LengthUnit::Vi;
    if (unit_name.equals_ignoring_ascii_case("vmax"sv))
        return LengthUnit::Vmax;
    if (unit_name.equals_ignoring_ascii_case("vmin"sv))
        return LengthUnit::Vmin;
    if (unit_name.equals_ignoring_ascii_case("vw"sv))
        return LengthUnit::Vw;
    return {};
}

FlyString to_string(LengthUnit value)
{
    switch (value) {
    case LengthUnit::Cap:
        return "cap"_fly_string;
    case LengthUnit::Ch:
        return "ch"_fly_string;
    case LengthUnit::Cm:
        return "cm"_fly_string;
    case LengthUnit::Dvb:
        return "dvb"_fly_string;
    case LengthUnit::Dvh:
        return "dvh"_fly_string;
    case LengthUnit::Dvi:
        return "dvi"_fly_string;
    case LengthUnit::Dvmax:
        return "dvmax"_fly_string;
    case LengthUnit::Dvmin:
        return "dvmin"_fly_string;
    case LengthUnit::Dvw:
        return "dvw"_fly_string;
    case LengthUnit::Em:
        return "em"_fly_string;
    case LengthUnit::Ex:
        return "ex"_fly_string;
    case LengthUnit::Ic:
        return "ic"_fly_string;
    case LengthUnit::In:
        return "in"_fly_string;
    case LengthUnit::Lh:
        return "lh"_fly_string;
    case LengthUnit::Lvb:
        return "lvb"_fly_string;
    case LengthUnit::Lvh:
        return "lvh"_fly_string;
    case LengthUnit::Lvi:
        return "lvi"_fly_string;
    case LengthUnit::Lvmax:
        return "lvmax"_fly_string;
    case LengthUnit::Lvmin:
        return "lvmin"_fly_string;
    case LengthUnit::Lvw:
        return "lvw"_fly_string;
    case LengthUnit::Mm:
        return "mm"_fly_string;
    case LengthUnit::Pc:
        return "pc"_fly_string;
    case LengthUnit::Pt:
        return "pt"_fly_string;
    case LengthUnit::Px:
        return "px"_fly_string;
    case LengthUnit::Q:
        return "Q"_fly_string;
    case LengthUnit::Rcap:
        return "rcap"_fly_string;
    case LengthUnit::Rch:
        return "rch"_fly_string;
    case LengthUnit::Rem:
        return "rem"_fly_string;
    case LengthUnit::Rex:
        return "rex"_fly_string;
    case LengthUnit::Ric:
        return "ric"_fly_string;
    case LengthUnit::Rlh:
        return "rlh"_fly_string;
    case LengthUnit::Svb:
        return "svb"_fly_string;
    case LengthUnit::Svh:
        return "svh"_fly_string;
    case LengthUnit::Svi:
        return "svi"_fly_string;
    case LengthUnit::Svmax:
        return "svmax"_fly_string;
    case LengthUnit::Svmin:
        return "svmin"_fly_string;
    case LengthUnit::Svw:
        return "svw"_fly_string;
    case LengthUnit::Vb:
        return "vb"_fly_string;
    case LengthUnit::Vh:
        return "vh"_fly_string;
    case LengthUnit::Vi:
        return "vi"_fly_string;
    case LengthUnit::Vmax:
        return "vmax"_fly_string;
    case LengthUnit::Vmin:
        return "vmin"_fly_string;
    case LengthUnit::Vw:
        return "vw"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(LengthUnit a, LengthUnit b)
{
    auto is_absolute = [](LengthUnit unit) -> bool {
        switch (unit) {
        case LengthUnit::Cm:
        case LengthUnit::In:
        case LengthUnit::Mm:
        case LengthUnit::Pc:
        case LengthUnit::Pt:
        case LengthUnit::Px:
        case LengthUnit::Q:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(LengthUnit from, LengthUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](LengthUnit unit) -> double {
        switch (unit) {

        case LengthUnit::Cm:
            return 37.79527559055118;

        case LengthUnit::In:
            return 96;

        case LengthUnit::Mm:
            return 3.779527559055118;

        case LengthUnit::Pc:
            return 16;

        case LengthUnit::Pt:
            return 1.3333333333333333;

        case LengthUnit::Px:
            return 1;

        case LengthUnit::Q:
            return 0.9448818897637795;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == LengthUnit::Px)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

Optional<ResolutionUnit> string_to_resolution_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("dpcm"sv))
        return ResolutionUnit::Dpcm;
    if (unit_name.equals_ignoring_ascii_case("dpi"sv))
        return ResolutionUnit::Dpi;
    if (unit_name.equals_ignoring_ascii_case("dppx"sv))
        return ResolutionUnit::Dppx;
    if (unit_name.equals_ignoring_ascii_case("x"sv))
        return ResolutionUnit::X;
    return {};
}

FlyString to_string(ResolutionUnit value)
{
    switch (value) {
    case ResolutionUnit::Dpcm:
        return "dpcm"_fly_string;
    case ResolutionUnit::Dpi:
        return "dpi"_fly_string;
    case ResolutionUnit::Dppx:
        return "dppx"_fly_string;
    case ResolutionUnit::X:
        return "x"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(ResolutionUnit a, ResolutionUnit b)
{
    auto is_absolute = [](ResolutionUnit unit) -> bool {
        switch (unit) {
        case ResolutionUnit::Dpcm:
        case ResolutionUnit::Dpi:
        case ResolutionUnit::Dppx:
        case ResolutionUnit::X:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(ResolutionUnit from, ResolutionUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](ResolutionUnit unit) -> double {
        switch (unit) {

        case ResolutionUnit::Dpcm:
            return 0.026458333333333334;

        case ResolutionUnit::Dpi:
            return 0.010416666666666666;

        case ResolutionUnit::Dppx:
            return 1;

        case ResolutionUnit::X:
            return 1;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == ResolutionUnit::Dppx)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

Optional<TimeUnit> string_to_time_unit(StringView unit_name)
{

    if (unit_name.equals_ignoring_ascii_case("ms"sv))
        return TimeUnit::Ms;
    if (unit_name.equals_ignoring_ascii_case("s"sv))
        return TimeUnit::S;
    return {};
}

FlyString to_string(TimeUnit value)
{
    switch (value) {
    case TimeUnit::Ms:
        return "ms"_fly_string;
    case TimeUnit::S:
        return "s"_fly_string;
    default:
        VERIFY_NOT_REACHED();
    }
}

bool units_are_compatible(TimeUnit a, TimeUnit b)
{
    auto is_absolute = [](TimeUnit unit) -> bool {
        switch (unit) {
        case TimeUnit::Ms:
        case TimeUnit::S:

            return true;
        default:
            return false;
        }
    };

    return is_absolute(a) && is_absolute(b);
}

double ratio_between_units(TimeUnit from, TimeUnit to)
{
    if (from == to)
        return 1;

    auto ratio_to_canonical_unit = [](TimeUnit unit) -> double {
        switch (unit) {

        case TimeUnit::Ms:
            return 0.001;

        case TimeUnit::S:
            return 1;

        default:
            // `from` is a relative unit, so this isn't valid.
            VERIFY_NOT_REACHED();
        }
    };

    if (to == TimeUnit::S)
        return ratio_to_canonical_unit(from);
    return ratio_to_canonical_unit(from) / ratio_to_canonical_unit(to);
}

bool is_absolute(LengthUnit unit)
{
    switch (unit) {
    case LengthUnit::Cm:
    case LengthUnit::In:
    case LengthUnit::Mm:
    case LengthUnit::Pc:
    case LengthUnit::Pt:
    case LengthUnit::Px:
    case LengthUnit::Q:

        return true;
    default:
        return false;
    }
}

bool is_font_relative(LengthUnit unit)
{
    switch (unit) {
    case LengthUnit::Cap:
    case LengthUnit::Ch:
    case LengthUnit::Em:
    case LengthUnit::Ex:
    case LengthUnit::Ic:
    case LengthUnit::Lh:
    case LengthUnit::Rcap:
    case LengthUnit::Rch:
    case LengthUnit::Rem:
    case LengthUnit::Rex:
    case LengthUnit::Ric:
    case LengthUnit::Rlh:

        return true;
    default:
        return false;
    }
}

bool is_viewport_relative(LengthUnit unit)
{
    switch (unit) {
    case LengthUnit::Dvb:
    case LengthUnit::Dvh:
    case LengthUnit::Dvi:
    case LengthUnit::Dvmax:
    case LengthUnit::Dvmin:
    case LengthUnit::Dvw:
    case LengthUnit::Lvb:
    case LengthUnit::Lvh:
    case LengthUnit::Lvi:
    case LengthUnit::Lvmax:
    case LengthUnit::Lvmin:
    case LengthUnit::Lvw:
    case LengthUnit::Svb:
    case LengthUnit::Svh:
    case LengthUnit::Svi:
    case LengthUnit::Svmax:
    case LengthUnit::Svmin:
    case LengthUnit::Svw:
    case LengthUnit::Vb:
    case LengthUnit::Vh:
    case LengthUnit::Vi:
    case LengthUnit::Vmax:
    case LengthUnit::Vmin:
    case LengthUnit::Vw:

        return true;
    default:
        return false;
    }
}

}
