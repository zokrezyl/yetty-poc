
#include <LibWeb/CSS/PseudoElement.h>
#include <LibWeb/CSS/PropertyID.h>

namespace Web::CSS {

Optional<PseudoElement> pseudo_element_from_string(StringView string)
{

    if (string.equals_ignoring_ascii_case("after"sv))
        return PseudoElement::After;

    if (string.equals_ignoring_ascii_case("backdrop"sv))
        return PseudoElement::Backdrop;

    if (string.equals_ignoring_ascii_case("before"sv))
        return PseudoElement::Before;

    if (string.equals_ignoring_ascii_case("details-content"sv))
        return PseudoElement::DetailsContent;

    if (string.equals_ignoring_ascii_case("file-selector-button"sv))
        return PseudoElement::FileSelectorButton;

    if (string.equals_ignoring_ascii_case("first-letter"sv))
        return PseudoElement::FirstLetter;

    if (string.equals_ignoring_ascii_case("first-line"sv))
        return PseudoElement::FirstLine;

    if (string.equals_ignoring_ascii_case("marker"sv))
        return PseudoElement::Marker;

    if (string.equals_ignoring_ascii_case("part"sv))
        return PseudoElement::Part;

    if (string.equals_ignoring_ascii_case("placeholder"sv))
        return PseudoElement::Placeholder;

    if (string.equals_ignoring_ascii_case("selection"sv))
        return PseudoElement::Selection;

    if (string.equals_ignoring_ascii_case("slider-fill"sv))
        return PseudoElement::SliderFill;

    if (string.equals_ignoring_ascii_case("slider-thumb"sv))
        return PseudoElement::SliderThumb;

    if (string.equals_ignoring_ascii_case("slider-track"sv))
        return PseudoElement::SliderTrack;

    if (string.equals_ignoring_ascii_case("slotted"sv))
        return PseudoElement::Slotted;

    if (string.equals_ignoring_ascii_case("view-transition"sv))
        return PseudoElement::ViewTransition;

    if (string.equals_ignoring_ascii_case("view-transition-group"sv))
        return PseudoElement::ViewTransitionGroup;

    if (string.equals_ignoring_ascii_case("view-transition-image-pair"sv))
        return PseudoElement::ViewTransitionImagePair;

    if (string.equals_ignoring_ascii_case("view-transition-new"sv))
        return PseudoElement::ViewTransitionNew;

    if (string.equals_ignoring_ascii_case("view-transition-old"sv))
        return PseudoElement::ViewTransitionOld;


    return {};
}

Optional<PseudoElement> aliased_pseudo_element_from_string(StringView string)
{

    if (string.equals_ignoring_ascii_case("-moz-meter-bar"sv))
        return PseudoElement::SliderFill;

    if (string.equals_ignoring_ascii_case("-moz-placeholder"sv))
        return PseudoElement::Placeholder;

    if (string.equals_ignoring_ascii_case("-moz-progress-bar"sv))
        return PseudoElement::SliderFill;

    if (string.equals_ignoring_ascii_case("-moz-range-progress"sv))
        return PseudoElement::SliderFill;

    if (string.equals_ignoring_ascii_case("-moz-range-track"sv))
        return PseudoElement::SliderTrack;

    if (string.equals_ignoring_ascii_case("-moz-range-thumb"sv))
        return PseudoElement::SliderThumb;

    if (string.equals_ignoring_ascii_case("-webkit-input-placeholder"sv))
        return PseudoElement::Placeholder;

    if (string.equals_ignoring_ascii_case("-webkit-meter-bar"sv))
        return PseudoElement::SliderTrack;

    if (string.equals_ignoring_ascii_case("-webkit-progress-bar"sv))
        return PseudoElement::SliderTrack;

    if (string.equals_ignoring_ascii_case("-webkit-progress-value"sv))
        return PseudoElement::SliderFill;

    if (string.equals_ignoring_ascii_case("-webkit-slider-runnable-track"sv))
        return PseudoElement::SliderTrack;

    if (string.equals_ignoring_ascii_case("-webkit-slider-thumb"sv))
        return PseudoElement::SliderThumb;


    return {};
}

StringView pseudo_element_name(PseudoElement pseudo_element)
{
    switch (pseudo_element) {

    case PseudoElement::After:
        return "after"sv;

    case PseudoElement::Backdrop:
        return "backdrop"sv;

    case PseudoElement::Before:
        return "before"sv;

    case PseudoElement::DetailsContent:
        return "details-content"sv;

    case PseudoElement::FileSelectorButton:
        return "file-selector-button"sv;

    case PseudoElement::FirstLetter:
        return "first-letter"sv;

    case PseudoElement::FirstLine:
        return "first-line"sv;

    case PseudoElement::Marker:
        return "marker"sv;

    case PseudoElement::Part:
        return "part"sv;

    case PseudoElement::Placeholder:
        return "placeholder"sv;

    case PseudoElement::Selection:
        return "selection"sv;

    case PseudoElement::SliderFill:
        return "slider-fill"sv;

    case PseudoElement::SliderThumb:
        return "slider-thumb"sv;

    case PseudoElement::SliderTrack:
        return "slider-track"sv;

    case PseudoElement::Slotted:
        return "slotted"sv;

    case PseudoElement::ViewTransition:
        return "view-transition"sv;

    case PseudoElement::ViewTransitionGroup:
        return "view-transition-group"sv;

    case PseudoElement::ViewTransitionImagePair:
        return "view-transition-image-pair"sv;

    case PseudoElement::ViewTransitionNew:
        return "view-transition-new"sv;

    case PseudoElement::ViewTransitionOld:
        return "view-transition-old"sv;

    case PseudoElement::KnownPseudoElementCount:
    case PseudoElement::UnknownWebKit:
        VERIFY_NOT_REACHED();
    }
    VERIFY_NOT_REACHED();
}

bool is_has_allowed_pseudo_element(PseudoElement pseudo_element)
{
    switch (pseudo_element) {

    default:
        return false;
    }
}

bool is_pseudo_element_root(PseudoElement pseudo_element)
{
    switch (pseudo_element) {

    case PseudoElement::ViewTransition:
        return true;

    default:
        return false;
    }
}

bool pseudo_element_supports_property(PseudoElement pseudo_element, PropertyID property_id)
{
    switch (pseudo_element) {

    case PseudoElement::FirstLetter:
        switch (property_id) {
        case PropertyID::Font:
        case PropertyID::FontFamily:
        case PropertyID::FontFeatureSettings:
        case PropertyID::FontLanguageOverride:
        case PropertyID::FontSize:
        case PropertyID::FontStyle:
        case PropertyID::FontVariant:
        case PropertyID::FontVariantAlternates:
        case PropertyID::FontVariantCaps:
        case PropertyID::FontVariantEastAsian:
        case PropertyID::FontVariantEmoji:
        case PropertyID::FontVariantLigatures:
        case PropertyID::FontVariantNumeric:
        case PropertyID::FontVariantPosition:
        case PropertyID::FontVariationSettings:
        case PropertyID::FontWeight:
        case PropertyID::FontWidth:
        case PropertyID::Color:
        case PropertyID::Opacity:
        case PropertyID::Background:
        case PropertyID::BackgroundAttachment:
        case PropertyID::BackgroundClip:
        case PropertyID::BackgroundColor:
        case PropertyID::BackgroundImage:
        case PropertyID::BackgroundOrigin:
        case PropertyID::BackgroundPosition:
        case PropertyID::BackgroundPositionX:
        case PropertyID::BackgroundPositionY:
        case PropertyID::BackgroundRepeat:
        case PropertyID::BackgroundSize:
        case PropertyID::LetterSpacing:
        case PropertyID::OverflowWrap:
        case PropertyID::TabSize:
        case PropertyID::TextAlign:
        case PropertyID::TextIndent:
        case PropertyID::TextJustify:
        case PropertyID::TextTransform:
        case PropertyID::TextWrap:
        case PropertyID::TextWrapMode:
        case PropertyID::TextWrapStyle:
        case PropertyID::WhiteSpace:
        case PropertyID::WhiteSpaceCollapse:
        case PropertyID::WhiteSpaceTrim:
        case PropertyID::WordBreak:
        case PropertyID::WordSpacing:
        case PropertyID::TextDecoration:
        case PropertyID::TextDecorationColor:
        case PropertyID::TextDecorationLine:
        case PropertyID::TextDecorationStyle:
        case PropertyID::TextDecorationThickness:
        case PropertyID::LineHeight:
        case PropertyID::VerticalAlign:
        case PropertyID::Margin:
        case PropertyID::MarginBlock:
        case PropertyID::MarginBlockEnd:
        case PropertyID::MarginBlockStart:
        case PropertyID::MarginBottom:
        case PropertyID::MarginInline:
        case PropertyID::MarginInlineEnd:
        case PropertyID::MarginInlineStart:
        case PropertyID::MarginLeft:
        case PropertyID::MarginRight:
        case PropertyID::MarginTop:
        case PropertyID::Padding:
        case PropertyID::PaddingBlock:
        case PropertyID::PaddingBlockEnd:
        case PropertyID::PaddingBlockStart:
        case PropertyID::PaddingBottom:
        case PropertyID::PaddingInline:
        case PropertyID::PaddingInlineEnd:
        case PropertyID::PaddingInlineStart:
        case PropertyID::PaddingLeft:
        case PropertyID::PaddingRight:
        case PropertyID::PaddingTop:
        case PropertyID::Border:
        case PropertyID::BorderBlockEnd:
        case PropertyID::BorderBlockEndColor:
        case PropertyID::BorderBlockEndStyle:
        case PropertyID::BorderBlockEndWidth:
        case PropertyID::BorderBlockStart:
        case PropertyID::BorderBlockStartColor:
        case PropertyID::BorderBlockStartStyle:
        case PropertyID::BorderBlockStartWidth:
        case PropertyID::BorderBottom:
        case PropertyID::BorderBottomColor:
        case PropertyID::BorderBottomLeftRadius:
        case PropertyID::BorderBottomRightRadius:
        case PropertyID::BorderBottomStyle:
        case PropertyID::BorderBottomWidth:
        case PropertyID::BorderColor:
        case PropertyID::BorderImageOutset:
        case PropertyID::BorderImageRepeat:
        case PropertyID::BorderImageSlice:
        case PropertyID::BorderImageSource:
        case PropertyID::BorderImageWidth:
        case PropertyID::BorderInlineEnd:
        case PropertyID::BorderInlineEndColor:
        case PropertyID::BorderInlineEndStyle:
        case PropertyID::BorderInlineEndWidth:
        case PropertyID::BorderInlineStart:
        case PropertyID::BorderInlineStartColor:
        case PropertyID::BorderInlineStartStyle:
        case PropertyID::BorderInlineStartWidth:
        case PropertyID::BorderLeft:
        case PropertyID::BorderLeftColor:
        case PropertyID::BorderLeftStyle:
        case PropertyID::BorderLeftWidth:
        case PropertyID::BorderRadius:
        case PropertyID::BorderRight:
        case PropertyID::BorderRightColor:
        case PropertyID::BorderRightStyle:
        case PropertyID::BorderRightWidth:
        case PropertyID::BorderStyle:
        case PropertyID::BorderTop:
        case PropertyID::BorderTopColor:
        case PropertyID::BorderTopLeftRadius:
        case PropertyID::BorderTopRightRadius:
        case PropertyID::BorderTopStyle:
        case PropertyID::BorderTopWidth:
        case PropertyID::BorderWidth:
        case PropertyID::BoxShadow:
        case PropertyID::Custom:

            return true;
        default:
            return false;
        }

    case PseudoElement::FirstLine:
        switch (property_id) {
        case PropertyID::Font:
        case PropertyID::FontFamily:
        case PropertyID::FontFeatureSettings:
        case PropertyID::FontLanguageOverride:
        case PropertyID::FontSize:
        case PropertyID::FontStyle:
        case PropertyID::FontVariant:
        case PropertyID::FontVariantAlternates:
        case PropertyID::FontVariantCaps:
        case PropertyID::FontVariantEastAsian:
        case PropertyID::FontVariantEmoji:
        case PropertyID::FontVariantLigatures:
        case PropertyID::FontVariantNumeric:
        case PropertyID::FontVariantPosition:
        case PropertyID::FontVariationSettings:
        case PropertyID::FontWeight:
        case PropertyID::FontWidth:
        case PropertyID::Color:
        case PropertyID::Opacity:
        case PropertyID::Background:
        case PropertyID::BackgroundAttachment:
        case PropertyID::BackgroundClip:
        case PropertyID::BackgroundColor:
        case PropertyID::BackgroundImage:
        case PropertyID::BackgroundOrigin:
        case PropertyID::BackgroundPosition:
        case PropertyID::BackgroundPositionX:
        case PropertyID::BackgroundPositionY:
        case PropertyID::BackgroundRepeat:
        case PropertyID::BackgroundSize:
        case PropertyID::LetterSpacing:
        case PropertyID::OverflowWrap:
        case PropertyID::TabSize:
        case PropertyID::TextAlign:
        case PropertyID::TextIndent:
        case PropertyID::TextJustify:
        case PropertyID::TextTransform:
        case PropertyID::TextWrap:
        case PropertyID::TextWrapMode:
        case PropertyID::TextWrapStyle:
        case PropertyID::WhiteSpace:
        case PropertyID::WhiteSpaceCollapse:
        case PropertyID::WhiteSpaceTrim:
        case PropertyID::WordBreak:
        case PropertyID::WordSpacing:
        case PropertyID::TextDecoration:
        case PropertyID::TextDecorationColor:
        case PropertyID::TextDecorationLine:
        case PropertyID::TextDecorationStyle:
        case PropertyID::TextDecorationThickness:
        case PropertyID::Custom:
        case PropertyID::LineHeight:
        case PropertyID::VerticalAlign:

            return true;
        default:
            return false;
        }

    case PseudoElement::Placeholder:
        switch (property_id) {
        case PropertyID::Font:
        case PropertyID::FontFamily:
        case PropertyID::FontFeatureSettings:
        case PropertyID::FontLanguageOverride:
        case PropertyID::FontSize:
        case PropertyID::FontStyle:
        case PropertyID::FontVariant:
        case PropertyID::FontVariantAlternates:
        case PropertyID::FontVariantCaps:
        case PropertyID::FontVariantEastAsian:
        case PropertyID::FontVariantEmoji:
        case PropertyID::FontVariantLigatures:
        case PropertyID::FontVariantNumeric:
        case PropertyID::FontVariantPosition:
        case PropertyID::FontVariationSettings:
        case PropertyID::FontWeight:
        case PropertyID::FontWidth:
        case PropertyID::Color:
        case PropertyID::Opacity:
        case PropertyID::Background:
        case PropertyID::BackgroundAttachment:
        case PropertyID::BackgroundClip:
        case PropertyID::BackgroundColor:
        case PropertyID::BackgroundImage:
        case PropertyID::BackgroundOrigin:
        case PropertyID::BackgroundPosition:
        case PropertyID::BackgroundPositionX:
        case PropertyID::BackgroundPositionY:
        case PropertyID::BackgroundRepeat:
        case PropertyID::BackgroundSize:
        case PropertyID::LetterSpacing:
        case PropertyID::OverflowWrap:
        case PropertyID::TabSize:
        case PropertyID::TextAlign:
        case PropertyID::TextIndent:
        case PropertyID::TextJustify:
        case PropertyID::TextTransform:
        case PropertyID::TextWrap:
        case PropertyID::TextWrapMode:
        case PropertyID::TextWrapStyle:
        case PropertyID::WhiteSpace:
        case PropertyID::WhiteSpaceCollapse:
        case PropertyID::WhiteSpaceTrim:
        case PropertyID::WordBreak:
        case PropertyID::WordSpacing:
        case PropertyID::TextDecoration:
        case PropertyID::TextDecorationColor:
        case PropertyID::TextDecorationLine:
        case PropertyID::TextDecorationStyle:
        case PropertyID::TextDecorationThickness:
        case PropertyID::Custom:

            return true;
        default:
            return false;
        }

    case PseudoElement::Selection:
        switch (property_id) {
        case PropertyID::Color:
        case PropertyID::BackgroundColor:
        case PropertyID::TextDecoration:
        case PropertyID::TextDecorationColor:
        case PropertyID::TextDecorationLine:
        case PropertyID::TextDecorationStyle:
        case PropertyID::TextDecorationThickness:
        case PropertyID::TextShadow:
        case PropertyID::StrokeWidth:
        case PropertyID::Custom:

            return true;
        default:
            return false;
        }

    default:
        return true;
    }
}

PseudoElementMetadata pseudo_element_metadata(PseudoElement pseudo_element)
{
    switch (pseudo_element) {

    case PseudoElement::After:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Backdrop:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Before:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::DetailsContent:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::FileSelectorButton:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::FirstLetter:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::FirstLine:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Marker:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Part:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::IdentList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::Placeholder:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Selection:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::SliderFill:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::SliderThumb:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::SliderTrack:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::Slotted:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::CompoundSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::ViewTransition:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoElement::ViewTransitionGroup:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::PTNameSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::ViewTransitionImagePair:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::PTNameSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::ViewTransitionNew:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::PTNameSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::ViewTransitionOld:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::PTNameSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoElement::UnknownWebKit:
        return {
            .parameter_type = PseudoElementMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };
    case PseudoElement::KnownPseudoElementCount:
        break;
    }
    VERIFY_NOT_REACHED();
}

}
