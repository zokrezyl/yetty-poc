
#include <LibWeb/CSS/MediaFeatureID.h>
#include <LibWeb/Infra/Strings.h>

namespace Web::CSS {

Optional<MediaFeatureID> media_feature_id_from_string(StringView string)
{
    if (string.equals_ignoring_ascii_case("any-hover"sv))
        return MediaFeatureID::AnyHover;

    if (string.equals_ignoring_ascii_case("any-pointer"sv))
        return MediaFeatureID::AnyPointer;

    if (string.equals_ignoring_ascii_case("aspect-ratio"sv))
        return MediaFeatureID::AspectRatio;

    if (string.equals_ignoring_ascii_case("color"sv))
        return MediaFeatureID::Color;

    if (string.equals_ignoring_ascii_case("color-gamut"sv))
        return MediaFeatureID::ColorGamut;

    if (string.equals_ignoring_ascii_case("color-index"sv))
        return MediaFeatureID::ColorIndex;

    if (string.equals_ignoring_ascii_case("device-aspect-ratio"sv))
        return MediaFeatureID::DeviceAspectRatio;

    if (string.equals_ignoring_ascii_case("device-height"sv))
        return MediaFeatureID::DeviceHeight;

    if (string.equals_ignoring_ascii_case("device-width"sv))
        return MediaFeatureID::DeviceWidth;

    if (string.equals_ignoring_ascii_case("display-mode"sv))
        return MediaFeatureID::DisplayMode;

    if (string.equals_ignoring_ascii_case("dynamic-range"sv))
        return MediaFeatureID::DynamicRange;

    if (string.equals_ignoring_ascii_case("environment-blending"sv))
        return MediaFeatureID::EnvironmentBlending;

    if (string.equals_ignoring_ascii_case("forced-colors"sv))
        return MediaFeatureID::ForcedColors;

    if (string.equals_ignoring_ascii_case("grid"sv))
        return MediaFeatureID::Grid;

    if (string.equals_ignoring_ascii_case("height"sv))
        return MediaFeatureID::Height;

    if (string.equals_ignoring_ascii_case("horizontal-viewport-segments"sv))
        return MediaFeatureID::HorizontalViewportSegments;

    if (string.equals_ignoring_ascii_case("hover"sv))
        return MediaFeatureID::Hover;

    if (string.equals_ignoring_ascii_case("inverted-colors"sv))
        return MediaFeatureID::InvertedColors;

    if (string.equals_ignoring_ascii_case("monochrome"sv))
        return MediaFeatureID::Monochrome;

    if (string.equals_ignoring_ascii_case("nav-controls"sv))
        return MediaFeatureID::NavControls;

    if (string.equals_ignoring_ascii_case("orientation"sv))
        return MediaFeatureID::Orientation;

    if (string.equals_ignoring_ascii_case("overflow-block"sv))
        return MediaFeatureID::OverflowBlock;

    if (string.equals_ignoring_ascii_case("overflow-inline"sv))
        return MediaFeatureID::OverflowInline;

    if (string.equals_ignoring_ascii_case("pointer"sv))
        return MediaFeatureID::Pointer;

    if (string.equals_ignoring_ascii_case("prefers-color-scheme"sv))
        return MediaFeatureID::PrefersColorScheme;

    if (string.equals_ignoring_ascii_case("prefers-contrast"sv))
        return MediaFeatureID::PrefersContrast;

    if (string.equals_ignoring_ascii_case("prefers-reduced-data"sv))
        return MediaFeatureID::PrefersReducedData;

    if (string.equals_ignoring_ascii_case("prefers-reduced-motion"sv))
        return MediaFeatureID::PrefersReducedMotion;

    if (string.equals_ignoring_ascii_case("prefers-reduced-transparency"sv))
        return MediaFeatureID::PrefersReducedTransparency;

    if (string.equals_ignoring_ascii_case("resolution"sv))
        return MediaFeatureID::Resolution;

    if (string.equals_ignoring_ascii_case("scan"sv))
        return MediaFeatureID::Scan;

    if (string.equals_ignoring_ascii_case("scripting"sv))
        return MediaFeatureID::Scripting;

    if (string.equals_ignoring_ascii_case("update"sv))
        return MediaFeatureID::Update;

    if (string.equals_ignoring_ascii_case("vertical-viewport-segments"sv))
        return MediaFeatureID::VerticalViewportSegments;

    if (string.equals_ignoring_ascii_case("video-color-gamut"sv))
        return MediaFeatureID::VideoColorGamut;

    if (string.equals_ignoring_ascii_case("video-dynamic-range"sv))
        return MediaFeatureID::VideoDynamicRange;

    if (string.equals_ignoring_ascii_case("width"sv))
        return MediaFeatureID::Width;

    return {};
}

StringView string_from_media_feature_id(MediaFeatureID media_feature_id)
{
    switch (media_feature_id) {
    case MediaFeatureID::AnyHover:
        return "any-hover"sv;
    case MediaFeatureID::AnyPointer:
        return "any-pointer"sv;
    case MediaFeatureID::AspectRatio:
        return "aspect-ratio"sv;
    case MediaFeatureID::Color:
        return "color"sv;
    case MediaFeatureID::ColorGamut:
        return "color-gamut"sv;
    case MediaFeatureID::ColorIndex:
        return "color-index"sv;
    case MediaFeatureID::DeviceAspectRatio:
        return "device-aspect-ratio"sv;
    case MediaFeatureID::DeviceHeight:
        return "device-height"sv;
    case MediaFeatureID::DeviceWidth:
        return "device-width"sv;
    case MediaFeatureID::DisplayMode:
        return "display-mode"sv;
    case MediaFeatureID::DynamicRange:
        return "dynamic-range"sv;
    case MediaFeatureID::EnvironmentBlending:
        return "environment-blending"sv;
    case MediaFeatureID::ForcedColors:
        return "forced-colors"sv;
    case MediaFeatureID::Grid:
        return "grid"sv;
    case MediaFeatureID::Height:
        return "height"sv;
    case MediaFeatureID::HorizontalViewportSegments:
        return "horizontal-viewport-segments"sv;
    case MediaFeatureID::Hover:
        return "hover"sv;
    case MediaFeatureID::InvertedColors:
        return "inverted-colors"sv;
    case MediaFeatureID::Monochrome:
        return "monochrome"sv;
    case MediaFeatureID::NavControls:
        return "nav-controls"sv;
    case MediaFeatureID::Orientation:
        return "orientation"sv;
    case MediaFeatureID::OverflowBlock:
        return "overflow-block"sv;
    case MediaFeatureID::OverflowInline:
        return "overflow-inline"sv;
    case MediaFeatureID::Pointer:
        return "pointer"sv;
    case MediaFeatureID::PrefersColorScheme:
        return "prefers-color-scheme"sv;
    case MediaFeatureID::PrefersContrast:
        return "prefers-contrast"sv;
    case MediaFeatureID::PrefersReducedData:
        return "prefers-reduced-data"sv;
    case MediaFeatureID::PrefersReducedMotion:
        return "prefers-reduced-motion"sv;
    case MediaFeatureID::PrefersReducedTransparency:
        return "prefers-reduced-transparency"sv;
    case MediaFeatureID::Resolution:
        return "resolution"sv;
    case MediaFeatureID::Scan:
        return "scan"sv;
    case MediaFeatureID::Scripting:
        return "scripting"sv;
    case MediaFeatureID::Update:
        return "update"sv;
    case MediaFeatureID::VerticalViewportSegments:
        return "vertical-viewport-segments"sv;
    case MediaFeatureID::VideoColorGamut:
        return "video-color-gamut"sv;
    case MediaFeatureID::VideoDynamicRange:
        return "video-dynamic-range"sv;
    case MediaFeatureID::Width:
        return "width"sv;
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_type_is_range(MediaFeatureID media_feature_id)
{
    switch (media_feature_id) {
    case MediaFeatureID::AnyHover:
        return false;
    case MediaFeatureID::AnyPointer:
        return false;
    case MediaFeatureID::AspectRatio:
        return true;
    case MediaFeatureID::Color:
        return true;
    case MediaFeatureID::ColorGamut:
        return false;
    case MediaFeatureID::ColorIndex:
        return true;
    case MediaFeatureID::DeviceAspectRatio:
        return true;
    case MediaFeatureID::DeviceHeight:
        return true;
    case MediaFeatureID::DeviceWidth:
        return true;
    case MediaFeatureID::DisplayMode:
        return false;
    case MediaFeatureID::DynamicRange:
        return false;
    case MediaFeatureID::EnvironmentBlending:
        return false;
    case MediaFeatureID::ForcedColors:
        return false;
    case MediaFeatureID::Grid:
        return false;
    case MediaFeatureID::Height:
        return true;
    case MediaFeatureID::HorizontalViewportSegments:
        return true;
    case MediaFeatureID::Hover:
        return false;
    case MediaFeatureID::InvertedColors:
        return false;
    case MediaFeatureID::Monochrome:
        return true;
    case MediaFeatureID::NavControls:
        return false;
    case MediaFeatureID::Orientation:
        return false;
    case MediaFeatureID::OverflowBlock:
        return false;
    case MediaFeatureID::OverflowInline:
        return false;
    case MediaFeatureID::Pointer:
        return false;
    case MediaFeatureID::PrefersColorScheme:
        return false;
    case MediaFeatureID::PrefersContrast:
        return false;
    case MediaFeatureID::PrefersReducedData:
        return false;
    case MediaFeatureID::PrefersReducedMotion:
        return false;
    case MediaFeatureID::PrefersReducedTransparency:
        return false;
    case MediaFeatureID::Resolution:
        return true;
    case MediaFeatureID::Scan:
        return false;
    case MediaFeatureID::Scripting:
        return false;
    case MediaFeatureID::Update:
        return false;
    case MediaFeatureID::VerticalViewportSegments:
        return true;
    case MediaFeatureID::VideoColorGamut:
        return false;
    case MediaFeatureID::VideoDynamicRange:
        return false;
    case MediaFeatureID::Width:
        return true;
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_accepts_type(MediaFeatureID media_feature_id, MediaFeatureValueType value_type)
{
    switch (media_feature_id) {
    case MediaFeatureID::AnyHover:
        return false;
    case MediaFeatureID::AnyPointer:
        return false;
    case MediaFeatureID::AspectRatio:
        switch (value_type) {
        case MediaFeatureValueType::Ratio:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Color:
        switch (value_type) {
        case MediaFeatureValueType::Integer:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::ColorGamut:
        return false;
    case MediaFeatureID::ColorIndex:
        switch (value_type) {
        case MediaFeatureValueType::Integer:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DeviceAspectRatio:
        switch (value_type) {
        case MediaFeatureValueType::Ratio:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DeviceHeight:
        switch (value_type) {
        case MediaFeatureValueType::Length:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DeviceWidth:
        switch (value_type) {
        case MediaFeatureValueType::Length:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DisplayMode:
        return false;
    case MediaFeatureID::DynamicRange:
        return false;
    case MediaFeatureID::EnvironmentBlending:
        return false;
    case MediaFeatureID::ForcedColors:
        return false;
    case MediaFeatureID::Grid:
        switch (value_type) {
        case MediaFeatureValueType::Boolean:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Height:
        switch (value_type) {
        case MediaFeatureValueType::Length:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::HorizontalViewportSegments:
        switch (value_type) {
        case MediaFeatureValueType::Integer:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Hover:
        return false;
    case MediaFeatureID::InvertedColors:
        return false;
    case MediaFeatureID::Monochrome:
        switch (value_type) {
        case MediaFeatureValueType::Integer:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::NavControls:
        return false;
    case MediaFeatureID::Orientation:
        return false;
    case MediaFeatureID::OverflowBlock:
        return false;
    case MediaFeatureID::OverflowInline:
        return false;
    case MediaFeatureID::Pointer:
        return false;
    case MediaFeatureID::PrefersColorScheme:
        return false;
    case MediaFeatureID::PrefersContrast:
        return false;
    case MediaFeatureID::PrefersReducedData:
        return false;
    case MediaFeatureID::PrefersReducedMotion:
        return false;
    case MediaFeatureID::PrefersReducedTransparency:
        return false;
    case MediaFeatureID::Resolution:
        switch (value_type) {
        case MediaFeatureValueType::Resolution:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Scan:
        return false;
    case MediaFeatureID::Scripting:
        return false;
    case MediaFeatureID::Update:
        return false;
    case MediaFeatureID::VerticalViewportSegments:
        switch (value_type) {
        case MediaFeatureValueType::Integer:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::VideoColorGamut:
        return false;
    case MediaFeatureID::VideoDynamicRange:
        return false;
    case MediaFeatureID::Width:
        switch (value_type) {
        case MediaFeatureValueType::Length:
            return true;
        default:
            return false;
        }
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_accepts_keyword(MediaFeatureID media_feature_id, Keyword keyword)
{
    switch (media_feature_id) {
    case MediaFeatureID::AnyHover:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Hover:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::AnyPointer:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Coarse:
            return true;
        case Keyword::Fine:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::AspectRatio:
        return false;
    case MediaFeatureID::Color:
        return false;
    case MediaFeatureID::ColorGamut:
        switch (keyword) {
        case Keyword::Srgb:
            return true;
        case Keyword::P3:
            return true;
        case Keyword::Rec2020:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::ColorIndex:
        return false;
    case MediaFeatureID::DeviceAspectRatio:
        return false;
    case MediaFeatureID::DeviceHeight:
        return false;
    case MediaFeatureID::DeviceWidth:
        return false;
    case MediaFeatureID::DisplayMode:
        switch (keyword) {
        case Keyword::Fullscreen:
            return true;
        case Keyword::Standalone:
            return true;
        case Keyword::MinimalUi:
            return true;
        case Keyword::Browser:
            return true;
        case Keyword::PictureInPicture:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DynamicRange:
        switch (keyword) {
        case Keyword::Standard:
            return true;
        case Keyword::High:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::EnvironmentBlending:
        switch (keyword) {
        case Keyword::Opaque:
            return true;
        case Keyword::Additive:
            return true;
        case Keyword::Subtractive:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::ForcedColors:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Active:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Grid:
        return false;
    case MediaFeatureID::Height:
        return false;
    case MediaFeatureID::HorizontalViewportSegments:
        return false;
    case MediaFeatureID::Hover:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Hover:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::InvertedColors:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Inverted:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Monochrome:
        return false;
    case MediaFeatureID::NavControls:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Back:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Orientation:
        switch (keyword) {
        case Keyword::Portrait:
            return true;
        case Keyword::Landscape:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::OverflowBlock:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Scroll:
            return true;
        case Keyword::Paged:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::OverflowInline:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Scroll:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Pointer:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Coarse:
            return true;
        case Keyword::Fine:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersColorScheme:
        switch (keyword) {
        case Keyword::Light:
            return true;
        case Keyword::Dark:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersContrast:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        case Keyword::Less:
            return true;
        case Keyword::More:
            return true;
        case Keyword::Custom:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedData:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        case Keyword::Reduce:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedMotion:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        case Keyword::Reduce:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedTransparency:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        case Keyword::Reduce:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Resolution:
        switch (keyword) {
        case Keyword::Infinite:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Scan:
        switch (keyword) {
        case Keyword::Interlace:
            return true;
        case Keyword::Progressive:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Scripting:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::InitialOnly:
            return true;
        case Keyword::Enabled:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Update:
        switch (keyword) {
        case Keyword::None:
            return true;
        case Keyword::Slow:
            return true;
        case Keyword::Fast:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::VerticalViewportSegments:
        return false;
    case MediaFeatureID::VideoColorGamut:
        switch (keyword) {
        case Keyword::Srgb:
            return true;
        case Keyword::P3:
            return true;
        case Keyword::Rec2020:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::VideoDynamicRange:
        switch (keyword) {
        case Keyword::Standard:
            return true;
        case Keyword::High:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Width:
        return false;
    }
    VERIFY_NOT_REACHED();
}

bool media_feature_keyword_is_falsey(MediaFeatureID media_feature_id, Keyword keyword)
{
    switch (media_feature_id) {
    case MediaFeatureID::AnyHover:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::AnyPointer:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::DynamicRange:
        switch (keyword) {
        case Keyword::Standard:
        case Keyword::High:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::ForcedColors:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Hover:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::InvertedColors:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::NavControls:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::OverflowBlock:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::OverflowInline:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Pointer:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersContrast:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedData:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedMotion:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::PrefersReducedTransparency:
        switch (keyword) {
        case Keyword::NoPreference:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Scan:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Scripting:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::Update:
        switch (keyword) {
        case Keyword::None:
            return true;
        default:
            return false;
        }
    case MediaFeatureID::VideoDynamicRange:
        switch (keyword) {
        case Keyword::Standard:
        case Keyword::High:
            return true;
        default:
            return false;
        }
    default:
        return false;
    }
}

}
