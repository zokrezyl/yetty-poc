
#pragma once

#include <AK/Optional.h>
#include <AK/StringView.h>
#include <LibWeb/CSS/ValueType.h>

namespace Web::CSS {

enum class EnvironmentVariable : u8 {
    PreferredTextScale,
    SafeAreaInsetBottom,
    SafeAreaInsetLeft,
    SafeAreaInsetRight,
    SafeAreaInsetTop,
    SafeAreaMaxInsetBottom,
    SafeAreaMaxInsetLeft,
    SafeAreaMaxInsetRight,
    SafeAreaMaxInsetTop,
    ViewportSegmentBottom,
    ViewportSegmentHeight,
    ViewportSegmentLeft,
    ViewportSegmentRight,
    ViewportSegmentTop,
    ViewportSegmentWidth,

};

Optional<EnvironmentVariable> environment_variable_from_string(StringView);
StringView to_string(EnvironmentVariable);

ValueType environment_variable_type(EnvironmentVariable);
u32 environment_variable_dimension_count(EnvironmentVariable);
}
