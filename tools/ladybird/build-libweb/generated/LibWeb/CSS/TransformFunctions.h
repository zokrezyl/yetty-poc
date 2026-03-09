
#pragma once

#include <AK/Optional.h>
#include <AK/StringView.h>
#include <AK/Vector.h>

namespace Web::CSS {

enum class TransformFunction {
    Matrix,
    Matrix3d,
    Perspective,
    Translate,
    Translate3d,
    TranslateX,
    TranslateY,
    TranslateZ,
    Scale,
    Scale3d,
    ScaleX,
    ScaleY,
    ScaleZ,
    Rotate,
    Rotate3d,
    RotateX,
    RotateY,
    RotateZ,
    Skew,
    SkewX,
    SkewY,
};
Optional<TransformFunction> transform_function_from_string(StringView);
StringView to_string(TransformFunction);

enum class TransformFunctionParameterType {
    Angle,
    Length,
    LengthNone,
    LengthPercentage,
    Number,
    NumberPercentage
};

struct TransformFunctionParameter {
    TransformFunctionParameterType type;
    bool required;
};

struct TransformFunctionMetadata {
    Vector<TransformFunctionParameter> parameters;
};
TransformFunctionMetadata transform_function_metadata(TransformFunction);

}
