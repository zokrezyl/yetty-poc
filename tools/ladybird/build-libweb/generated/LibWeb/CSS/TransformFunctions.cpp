
#include <LibWeb/CSS/TransformFunctions.h>
#include <AK/Assertions.h>

namespace Web::CSS {

Optional<TransformFunction> transform_function_from_string(StringView name)
{

    if (name.equals_ignoring_ascii_case("matrix"sv))
        return TransformFunction::Matrix;

    if (name.equals_ignoring_ascii_case("matrix3d"sv))
        return TransformFunction::Matrix3d;

    if (name.equals_ignoring_ascii_case("perspective"sv))
        return TransformFunction::Perspective;

    if (name.equals_ignoring_ascii_case("translate"sv))
        return TransformFunction::Translate;

    if (name.equals_ignoring_ascii_case("translate3d"sv))
        return TransformFunction::Translate3d;

    if (name.equals_ignoring_ascii_case("translateX"sv))
        return TransformFunction::TranslateX;

    if (name.equals_ignoring_ascii_case("translateY"sv))
        return TransformFunction::TranslateY;

    if (name.equals_ignoring_ascii_case("translateZ"sv))
        return TransformFunction::TranslateZ;

    if (name.equals_ignoring_ascii_case("scale"sv))
        return TransformFunction::Scale;

    if (name.equals_ignoring_ascii_case("scale3d"sv))
        return TransformFunction::Scale3d;

    if (name.equals_ignoring_ascii_case("scaleX"sv))
        return TransformFunction::ScaleX;

    if (name.equals_ignoring_ascii_case("scaleY"sv))
        return TransformFunction::ScaleY;

    if (name.equals_ignoring_ascii_case("scaleZ"sv))
        return TransformFunction::ScaleZ;

    if (name.equals_ignoring_ascii_case("rotate"sv))
        return TransformFunction::Rotate;

    if (name.equals_ignoring_ascii_case("rotate3d"sv))
        return TransformFunction::Rotate3d;

    if (name.equals_ignoring_ascii_case("rotateX"sv))
        return TransformFunction::RotateX;

    if (name.equals_ignoring_ascii_case("rotateY"sv))
        return TransformFunction::RotateY;

    if (name.equals_ignoring_ascii_case("rotateZ"sv))
        return TransformFunction::RotateZ;

    if (name.equals_ignoring_ascii_case("skew"sv))
        return TransformFunction::Skew;

    if (name.equals_ignoring_ascii_case("skewX"sv))
        return TransformFunction::SkewX;

    if (name.equals_ignoring_ascii_case("skewY"sv))
        return TransformFunction::SkewY;

    return {};
}

StringView to_string(TransformFunction transform_function)
{
    switch (transform_function) {

    case TransformFunction::Matrix:
        return "matrix"sv;

    case TransformFunction::Matrix3d:
        return "matrix3d"sv;

    case TransformFunction::Perspective:
        return "perspective"sv;

    case TransformFunction::Translate:
        return "translate"sv;

    case TransformFunction::Translate3d:
        return "translate3d"sv;

    case TransformFunction::TranslateX:
        return "translateX"sv;

    case TransformFunction::TranslateY:
        return "translateY"sv;

    case TransformFunction::TranslateZ:
        return "translateZ"sv;

    case TransformFunction::Scale:
        return "scale"sv;

    case TransformFunction::Scale3d:
        return "scale3d"sv;

    case TransformFunction::ScaleX:
        return "scaleX"sv;

    case TransformFunction::ScaleY:
        return "scaleY"sv;

    case TransformFunction::ScaleZ:
        return "scaleZ"sv;

    case TransformFunction::Rotate:
        return "rotate"sv;

    case TransformFunction::Rotate3d:
        return "rotate3d"sv;

    case TransformFunction::RotateX:
        return "rotateX"sv;

    case TransformFunction::RotateY:
        return "rotateY"sv;

    case TransformFunction::RotateZ:
        return "rotateZ"sv;

    case TransformFunction::Skew:
        return "skew"sv;

    case TransformFunction::SkewX:
        return "skewX"sv;

    case TransformFunction::SkewY:
        return "skewY"sv;

    default:
        VERIFY_NOT_REACHED();
    }
}

TransformFunctionMetadata transform_function_metadata(TransformFunction transform_function)
{
    switch (transform_function) {

    case TransformFunction::Matrix:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true} }
    };

    case TransformFunction::Matrix3d:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true} }
    };

    case TransformFunction::Perspective:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::LengthNone, true} }
    };

    case TransformFunction::Translate:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::LengthPercentage, true}, { TransformFunctionParameterType::LengthPercentage, false} }
    };

    case TransformFunction::Translate3d:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::LengthPercentage, true}, { TransformFunctionParameterType::LengthPercentage, true}, { TransformFunctionParameterType::Length, true} }
    };

    case TransformFunction::TranslateX:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::LengthPercentage, true} }
    };

    case TransformFunction::TranslateY:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::LengthPercentage, true} }
    };

    case TransformFunction::TranslateZ:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Length, true} }
    };

    case TransformFunction::Scale:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::NumberPercentage, true}, { TransformFunctionParameterType::NumberPercentage, false} }
    };

    case TransformFunction::Scale3d:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::NumberPercentage, true}, { TransformFunctionParameterType::NumberPercentage, true}, { TransformFunctionParameterType::NumberPercentage, true} }
    };

    case TransformFunction::ScaleX:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::NumberPercentage, true} }
    };

    case TransformFunction::ScaleY:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::NumberPercentage, true} }
    };

    case TransformFunction::ScaleZ:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::NumberPercentage, true} }
    };

    case TransformFunction::Rotate:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::Rotate3d:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Number, true}, { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::RotateX:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::RotateY:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::RotateZ:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::Skew:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true}, { TransformFunctionParameterType::Angle, false} }
    };

    case TransformFunction::SkewX:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    case TransformFunction::SkewY:
        return TransformFunctionMetadata {
            .parameters = { { TransformFunctionParameterType::Angle, true} }
    };

    default:
        VERIFY_NOT_REACHED();
    }
}

}
