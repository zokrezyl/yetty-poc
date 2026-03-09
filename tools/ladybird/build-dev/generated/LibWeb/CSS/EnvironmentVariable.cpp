
#include <LibWeb/CSS/EnvironmentVariable.h>

namespace Web::CSS {

Optional<EnvironmentVariable> environment_variable_from_string(StringView string)
{

    if (string.equals_ignoring_ascii_case("preferred-text-scale"sv))
        return EnvironmentVariable::PreferredTextScale;

    if (string.equals_ignoring_ascii_case("safe-area-inset-bottom"sv))
        return EnvironmentVariable::SafeAreaInsetBottom;

    if (string.equals_ignoring_ascii_case("safe-area-inset-left"sv))
        return EnvironmentVariable::SafeAreaInsetLeft;

    if (string.equals_ignoring_ascii_case("safe-area-inset-right"sv))
        return EnvironmentVariable::SafeAreaInsetRight;

    if (string.equals_ignoring_ascii_case("safe-area-inset-top"sv))
        return EnvironmentVariable::SafeAreaInsetTop;

    if (string.equals_ignoring_ascii_case("safe-area-max-inset-bottom"sv))
        return EnvironmentVariable::SafeAreaMaxInsetBottom;

    if (string.equals_ignoring_ascii_case("safe-area-max-inset-left"sv))
        return EnvironmentVariable::SafeAreaMaxInsetLeft;

    if (string.equals_ignoring_ascii_case("safe-area-max-inset-right"sv))
        return EnvironmentVariable::SafeAreaMaxInsetRight;

    if (string.equals_ignoring_ascii_case("safe-area-max-inset-top"sv))
        return EnvironmentVariable::SafeAreaMaxInsetTop;

    if (string.equals_ignoring_ascii_case("viewport-segment-bottom"sv))
        return EnvironmentVariable::ViewportSegmentBottom;

    if (string.equals_ignoring_ascii_case("viewport-segment-height"sv))
        return EnvironmentVariable::ViewportSegmentHeight;

    if (string.equals_ignoring_ascii_case("viewport-segment-left"sv))
        return EnvironmentVariable::ViewportSegmentLeft;

    if (string.equals_ignoring_ascii_case("viewport-segment-right"sv))
        return EnvironmentVariable::ViewportSegmentRight;

    if (string.equals_ignoring_ascii_case("viewport-segment-top"sv))
        return EnvironmentVariable::ViewportSegmentTop;

    if (string.equals_ignoring_ascii_case("viewport-segment-width"sv))
        return EnvironmentVariable::ViewportSegmentWidth;


    return {};
}

StringView to_string(EnvironmentVariable environment_variable)
{
    switch (environment_variable) {

    case EnvironmentVariable::PreferredTextScale:
        return "preferred-text-scale"sv;

    case EnvironmentVariable::SafeAreaInsetBottom:
        return "safe-area-inset-bottom"sv;

    case EnvironmentVariable::SafeAreaInsetLeft:
        return "safe-area-inset-left"sv;

    case EnvironmentVariable::SafeAreaInsetRight:
        return "safe-area-inset-right"sv;

    case EnvironmentVariable::SafeAreaInsetTop:
        return "safe-area-inset-top"sv;

    case EnvironmentVariable::SafeAreaMaxInsetBottom:
        return "safe-area-max-inset-bottom"sv;

    case EnvironmentVariable::SafeAreaMaxInsetLeft:
        return "safe-area-max-inset-left"sv;

    case EnvironmentVariable::SafeAreaMaxInsetRight:
        return "safe-area-max-inset-right"sv;

    case EnvironmentVariable::SafeAreaMaxInsetTop:
        return "safe-area-max-inset-top"sv;

    case EnvironmentVariable::ViewportSegmentBottom:
        return "viewport-segment-bottom"sv;

    case EnvironmentVariable::ViewportSegmentHeight:
        return "viewport-segment-height"sv;

    case EnvironmentVariable::ViewportSegmentLeft:
        return "viewport-segment-left"sv;

    case EnvironmentVariable::ViewportSegmentRight:
        return "viewport-segment-right"sv;

    case EnvironmentVariable::ViewportSegmentTop:
        return "viewport-segment-top"sv;

    case EnvironmentVariable::ViewportSegmentWidth:
        return "viewport-segment-width"sv;

    }
    VERIFY_NOT_REACHED();
}

ValueType environment_variable_type(EnvironmentVariable environment_variable)
{
    switch (environment_variable) {

    case EnvironmentVariable::PreferredTextScale:
        return ValueType::Number;

    case EnvironmentVariable::SafeAreaInsetBottom:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaInsetLeft:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaInsetRight:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaInsetTop:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaMaxInsetBottom:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaMaxInsetLeft:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaMaxInsetRight:
        return ValueType::Length;

    case EnvironmentVariable::SafeAreaMaxInsetTop:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentBottom:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentHeight:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentLeft:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentRight:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentTop:
        return ValueType::Length;

    case EnvironmentVariable::ViewportSegmentWidth:
        return ValueType::Length;

    }
    VERIFY_NOT_REACHED();
}

u32 environment_variable_dimension_count(EnvironmentVariable environment_variable)
{
    switch (environment_variable) {

    case EnvironmentVariable::PreferredTextScale:
        return 0;

    case EnvironmentVariable::SafeAreaInsetBottom:
        return 0;

    case EnvironmentVariable::SafeAreaInsetLeft:
        return 0;

    case EnvironmentVariable::SafeAreaInsetRight:
        return 0;

    case EnvironmentVariable::SafeAreaInsetTop:
        return 0;

    case EnvironmentVariable::SafeAreaMaxInsetBottom:
        return 0;

    case EnvironmentVariable::SafeAreaMaxInsetLeft:
        return 0;

    case EnvironmentVariable::SafeAreaMaxInsetRight:
        return 0;

    case EnvironmentVariable::SafeAreaMaxInsetTop:
        return 0;

    case EnvironmentVariable::ViewportSegmentBottom:
        return 2;

    case EnvironmentVariable::ViewportSegmentHeight:
        return 2;

    case EnvironmentVariable::ViewportSegmentLeft:
        return 2;

    case EnvironmentVariable::ViewportSegmentRight:
        return 2;

    case EnvironmentVariable::ViewportSegmentTop:
        return 2;

    case EnvironmentVariable::ViewportSegmentWidth:
        return 2;

    }
    VERIFY_NOT_REACHED();
}

}
