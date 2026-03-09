
#include <LibWeb/CSS/DescriptorID.h>
#include <LibWeb/CSS/StyleValues/StyleValue.h>
#include <LibWeb/CSS/Parser/Parser.h>
#include <LibWeb/CSS/PropertyID.h>

namespace Web::CSS {

FlyString to_string(AtRuleID at_rule_id)
{
    switch (at_rule_id) {

    case AtRuleID::FontFace:
        return "@font-face"_fly_string;

    case AtRuleID::Page:
        return "@page"_fly_string;

    case AtRuleID::Property:
        return "@property"_fly_string;

    case AtRuleID::CounterStyle:
        return "@counter-style"_fly_string;

    }
    VERIFY_NOT_REACHED();
}

Optional<DescriptorID> descriptor_id_from_string(AtRuleID at_rule_id, StringView string)
{
    switch (at_rule_id) {

    case AtRuleID::FontFace:

        if (string.equals_ignoring_ascii_case("ascent-override"sv))
            return DescriptorID::AscentOverride;

        if (string.equals_ignoring_ascii_case("descent-override"sv))
            return DescriptorID::DescentOverride;

        if (string.equals_ignoring_ascii_case("font-display"sv))
            return DescriptorID::FontDisplay;

        if (string.equals_ignoring_ascii_case("font-family"sv))
            return DescriptorID::FontFamily;

        if (string.equals_ignoring_ascii_case("font-feature-settings"sv))
            return DescriptorID::FontFeatureSettings;

        if (string.equals_ignoring_ascii_case("font-language-override"sv))
            return DescriptorID::FontLanguageOverride;

        if (string.equals_ignoring_ascii_case("font-named-instance"sv))
            return DescriptorID::FontNamedInstance;

        if (string.equals_ignoring_ascii_case("font-stretch"sv))
            return DescriptorID::FontWidth;

        if (string.equals_ignoring_ascii_case("font-style"sv))
            return DescriptorID::FontStyle;

        if (string.equals_ignoring_ascii_case("font-variation-settings"sv))
            return DescriptorID::FontVariationSettings;

        if (string.equals_ignoring_ascii_case("font-weight"sv))
            return DescriptorID::FontWeight;

        if (string.equals_ignoring_ascii_case("font-width"sv))
            return DescriptorID::FontWidth;

        if (string.equals_ignoring_ascii_case("line-gap-override"sv))
            return DescriptorID::LineGapOverride;

        if (string.equals_ignoring_ascii_case("src"sv))
            return DescriptorID::Src;

        if (string.equals_ignoring_ascii_case("unicode-range"sv))
            return DescriptorID::UnicodeRange;

        break;

    case AtRuleID::Page:

        if (string.equals_ignoring_ascii_case("bleed"sv))
            return DescriptorID::Bleed;

        if (string.equals_ignoring_ascii_case("margin"sv))
            return DescriptorID::Margin;

        if (string.equals_ignoring_ascii_case("margin-bottom"sv))
            return DescriptorID::MarginBottom;

        if (string.equals_ignoring_ascii_case("margin-left"sv))
            return DescriptorID::MarginLeft;

        if (string.equals_ignoring_ascii_case("margin-right"sv))
            return DescriptorID::MarginRight;

        if (string.equals_ignoring_ascii_case("margin-top"sv))
            return DescriptorID::MarginTop;

        if (string.equals_ignoring_ascii_case("marks"sv))
            return DescriptorID::Marks;

        if (string.equals_ignoring_ascii_case("page-orientation"sv))
            return DescriptorID::PageOrientation;

        if (string.equals_ignoring_ascii_case("size"sv))
            return DescriptorID::Size;

        break;

    case AtRuleID::Property:

        if (string.equals_ignoring_ascii_case("inherits"sv))
            return DescriptorID::Inherits;

        if (string.equals_ignoring_ascii_case("initial-value"sv))
            return DescriptorID::InitialValue;

        if (string.equals_ignoring_ascii_case("syntax"sv))
            return DescriptorID::Syntax;

        break;

    case AtRuleID::CounterStyle:

        if (string.equals_ignoring_ascii_case("system"sv))
            return DescriptorID::System;

        if (string.equals_ignoring_ascii_case("negative"sv))
            return DescriptorID::Negative;

        if (string.equals_ignoring_ascii_case("prefix"sv))
            return DescriptorID::Prefix;

        if (string.equals_ignoring_ascii_case("suffix"sv))
            return DescriptorID::Suffix;

        if (string.equals_ignoring_ascii_case("range"sv))
            return DescriptorID::Range;

        if (string.equals_ignoring_ascii_case("pad"sv))
            return DescriptorID::Pad;

        if (string.equals_ignoring_ascii_case("fallback"sv))
            return DescriptorID::Fallback;

        if (string.equals_ignoring_ascii_case("symbols"sv))
            return DescriptorID::Symbols;

        if (string.equals_ignoring_ascii_case("additive-symbols"sv))
            return DescriptorID::AdditiveSymbols;

        if (string.equals_ignoring_ascii_case("speak-as"sv))
            return DescriptorID::SpeakAs;

        break;

    }
    return {};
}

FlyString to_string(DescriptorID descriptor_id)
{
    switch (descriptor_id) {

    case DescriptorID::AdditiveSymbols:
        return "additive-symbols"_fly_string;

    case DescriptorID::AscentOverride:
        return "ascent-override"_fly_string;

    case DescriptorID::Bleed:
        return "bleed"_fly_string;

    case DescriptorID::DescentOverride:
        return "descent-override"_fly_string;

    case DescriptorID::Fallback:
        return "fallback"_fly_string;

    case DescriptorID::FontDisplay:
        return "font-display"_fly_string;

    case DescriptorID::FontFamily:
        return "font-family"_fly_string;

    case DescriptorID::FontFeatureSettings:
        return "font-feature-settings"_fly_string;

    case DescriptorID::FontLanguageOverride:
        return "font-language-override"_fly_string;

    case DescriptorID::FontNamedInstance:
        return "font-named-instance"_fly_string;

    case DescriptorID::FontStyle:
        return "font-style"_fly_string;

    case DescriptorID::FontVariationSettings:
        return "font-variation-settings"_fly_string;

    case DescriptorID::FontWeight:
        return "font-weight"_fly_string;

    case DescriptorID::FontWidth:
        return "font-width"_fly_string;

    case DescriptorID::Inherits:
        return "inherits"_fly_string;

    case DescriptorID::InitialValue:
        return "initial-value"_fly_string;

    case DescriptorID::LineGapOverride:
        return "line-gap-override"_fly_string;

    case DescriptorID::Margin:
        return "margin"_fly_string;

    case DescriptorID::MarginBottom:
        return "margin-bottom"_fly_string;

    case DescriptorID::MarginLeft:
        return "margin-left"_fly_string;

    case DescriptorID::MarginRight:
        return "margin-right"_fly_string;

    case DescriptorID::MarginTop:
        return "margin-top"_fly_string;

    case DescriptorID::Marks:
        return "marks"_fly_string;

    case DescriptorID::Negative:
        return "negative"_fly_string;

    case DescriptorID::Pad:
        return "pad"_fly_string;

    case DescriptorID::PageOrientation:
        return "page-orientation"_fly_string;

    case DescriptorID::Prefix:
        return "prefix"_fly_string;

    case DescriptorID::Range:
        return "range"_fly_string;

    case DescriptorID::Size:
        return "size"_fly_string;

    case DescriptorID::SpeakAs:
        return "speak-as"_fly_string;

    case DescriptorID::Src:
        return "src"_fly_string;

    case DescriptorID::Suffix:
        return "suffix"_fly_string;

    case DescriptorID::Symbols:
        return "symbols"_fly_string;

    case DescriptorID::Syntax:
        return "syntax"_fly_string;

    case DescriptorID::System:
        return "system"_fly_string;

    case DescriptorID::UnicodeRange:
        return "unicode-range"_fly_string;

    }
    VERIFY_NOT_REACHED();
}

bool at_rule_supports_descriptor(AtRuleID at_rule_id, DescriptorID descriptor_id)
{
    switch (at_rule_id) {

    case AtRuleID::FontFace:
        switch (descriptor_id) {
        case DescriptorID::AscentOverride:
        case DescriptorID::DescentOverride:
        case DescriptorID::FontDisplay:
        case DescriptorID::FontFamily:
        case DescriptorID::FontFeatureSettings:
        case DescriptorID::FontLanguageOverride:
        case DescriptorID::FontNamedInstance:
        case DescriptorID::FontStyle:
        case DescriptorID::FontVariationSettings:
        case DescriptorID::FontWeight:
        case DescriptorID::FontWidth:
        case DescriptorID::LineGapOverride:
        case DescriptorID::Src:
        case DescriptorID::UnicodeRange:

            return true;
        default:
            return false;
        }

    case AtRuleID::Page:
        switch (descriptor_id) {
        case DescriptorID::Bleed:
        case DescriptorID::Margin:
        case DescriptorID::MarginBottom:
        case DescriptorID::MarginLeft:
        case DescriptorID::MarginRight:
        case DescriptorID::MarginTop:
        case DescriptorID::Marks:
        case DescriptorID::PageOrientation:
        case DescriptorID::Size:

            return true;
        default:
            return false;
        }

    case AtRuleID::Property:
        switch (descriptor_id) {
        case DescriptorID::Inherits:
        case DescriptorID::InitialValue:
        case DescriptorID::Syntax:

            return true;
        default:
            return false;
        }

    case AtRuleID::CounterStyle:
        switch (descriptor_id) {
        case DescriptorID::System:
        case DescriptorID::Negative:
        case DescriptorID::Prefix:
        case DescriptorID::Suffix:
        case DescriptorID::Range:
        case DescriptorID::Pad:
        case DescriptorID::Fallback:
        case DescriptorID::Symbols:
        case DescriptorID::AdditiveSymbols:
        case DescriptorID::SpeakAs:

            return true;
        default:
            return false;
        }

    }
    VERIFY_NOT_REACHED();
}


RefPtr<StyleValue const> descriptor_initial_value(AtRuleID at_rule_id, DescriptorID descriptor_id)
{
    if (!at_rule_supports_descriptor(at_rule_id, descriptor_id))
        return nullptr;

    static Array<Array<RefPtr<StyleValue const>, 36>, 4> initial_values;
    if (auto initial_value = initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)])
        return initial_value.release_nonnull();

    // Lazily parse initial values as needed.

    Parser::ParsingParams parsing_params;
    switch (at_rule_id) {

    case AtRuleID::FontFace:
        switch (descriptor_id) {

        case DescriptorID::AscentOverride: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::AscentOverride, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::DescentOverride: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::DescentOverride, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontDisplay: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontDisplay, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontFamily:
            return nullptr;

        case DescriptorID::FontFeatureSettings: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontFeatureSettings, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontLanguageOverride: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontLanguageOverride, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontNamedInstance: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontNamedInstance, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontStyle: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontStyle, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontVariationSettings: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontVariationSettings, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontWeight: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontWeight, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::FontWidth: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::FontWidth, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::LineGapOverride: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::LineGapOverride, "normal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Src:
            return nullptr;

        case DescriptorID::UnicodeRange: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::FontFace, DescriptorID::UnicodeRange, "U+0-10FFFF"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::Page:
        switch (descriptor_id) {

        case DescriptorID::Bleed: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::Bleed, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Margin: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::Margin, "0"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::MarginBottom: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::MarginBottom, "0"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::MarginLeft: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::MarginLeft, "0"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::MarginRight: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::MarginRight, "0"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::MarginTop: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::MarginTop, "0"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Marks: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::Marks, "none"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::PageOrientation: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::PageOrientation, "upright"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Size: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::Page, DescriptorID::Size, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::Property:
        switch (descriptor_id) {

        case DescriptorID::Inherits:
            return nullptr;

        case DescriptorID::InitialValue:
            return nullptr;

        case DescriptorID::Syntax:
            return nullptr;

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::CounterStyle:
        switch (descriptor_id) {

        case DescriptorID::System: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::System, "symbolic"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Negative: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Negative, "-"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Prefix: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Prefix, ""sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Suffix: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Suffix, ". "sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Range: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Range, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Pad: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Pad, "0 """sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Fallback: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::Fallback, "decimal"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        case DescriptorID::Symbols:
            return nullptr;

        case DescriptorID::AdditiveSymbols:
            return nullptr;

        case DescriptorID::SpeakAs: {
            auto parsed_value = parse_css_descriptor(parsing_params, AtRuleID::CounterStyle, DescriptorID::SpeakAs, "auto"sv);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(at_rule_id)][to_underlying(descriptor_id)] = initial_value;
            return initial_value;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    }
    VERIFY_NOT_REACHED();
}

DescriptorMetadata get_descriptor_metadata(AtRuleID at_rule_id, DescriptorID descriptor_id)
{
    switch (at_rule_id) {

    case AtRuleID::FontFace:
        switch (descriptor_id) {

        case DescriptorID::AscentOverride: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Normal);

            metadata.syntax.empend(DescriptorMetadata::ValueType::PositivePercentage);

            return metadata;
        }

        case DescriptorID::DescentOverride: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Normal);

            metadata.syntax.empend(DescriptorMetadata::ValueType::PositivePercentage);

            return metadata;
        }

        case DescriptorID::FontDisplay: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(Keyword::Block);

            metadata.syntax.empend(Keyword::Swap);

            metadata.syntax.empend(Keyword::Fallback);

            metadata.syntax.empend(Keyword::Optional);

            return metadata;
        }

        case DescriptorID::FontFamily: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::FamilyName);

            return metadata;
        }

        case DescriptorID::FontFeatureSettings: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::FontFeatureSettings);

            return metadata;
        }

        case DescriptorID::FontLanguageOverride: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Normal);

            metadata.syntax.empend(DescriptorMetadata::ValueType::String);

            return metadata;
        }

        case DescriptorID::FontNamedInstance: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(DescriptorMetadata::ValueType::String);

            return metadata;
        }

        case DescriptorID::FontStyle: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(PropertyID::FontStyle);

            return metadata;
        }

        case DescriptorID::FontVariationSettings: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::FontVariationSettings);

            return metadata;
        }

        case DescriptorID::FontWeight: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(DescriptorMetadata::ValueType::FontWeightAbsolutePair);

            return metadata;
        }

        case DescriptorID::FontWidth: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(PropertyID::FontWidth);

            return metadata;
        }

        case DescriptorID::LineGapOverride: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Normal);

            metadata.syntax.empend(DescriptorMetadata::ValueType::PositivePercentage);

            return metadata;
        }

        case DescriptorID::Src: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::FontSrcList);

            return metadata;
        }

        case DescriptorID::UnicodeRange: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::UnicodeRangeTokens);

            return metadata;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::Page:
        switch (descriptor_id) {

        case DescriptorID::Bleed: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(DescriptorMetadata::ValueType::Length);

            return metadata;
        }

        case DescriptorID::Margin: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::Margin);

            return metadata;
        }

        case DescriptorID::MarginBottom: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::MarginBottom);

            return metadata;
        }

        case DescriptorID::MarginLeft: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::MarginLeft);

            return metadata;
        }

        case DescriptorID::MarginRight: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::MarginRight);

            return metadata;
        }

        case DescriptorID::MarginTop: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(PropertyID::MarginTop);

            return metadata;
        }

        case DescriptorID::Marks: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::None);

            metadata.syntax.empend(DescriptorMetadata::ValueType::CropOrCross);

            return metadata;
        }

        case DescriptorID::PageOrientation: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Upright);

            metadata.syntax.empend(Keyword::RotateLeft);

            metadata.syntax.empend(Keyword::RotateRight);

            return metadata;
        }

        case DescriptorID::Size: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::PageSize);

            return metadata;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::Property:
        switch (descriptor_id) {

        case DescriptorID::Inherits: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::True);

            metadata.syntax.empend(Keyword::False);

            return metadata;
        }

        case DescriptorID::InitialValue: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::OptionalDeclarationValue);

            return metadata;
        }

        case DescriptorID::Syntax: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::String);

            return metadata;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    case AtRuleID::CounterStyle:
        switch (descriptor_id) {

        case DescriptorID::System: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleSystem);

            return metadata;
        }

        case DescriptorID::Negative: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleNegative);

            return metadata;
        }

        case DescriptorID::Prefix: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::Symbol);

            return metadata;
        }

        case DescriptorID::Suffix: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::Symbol);

            return metadata;
        }

        case DescriptorID::Range: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleRange);

            return metadata;
        }

        case DescriptorID::Pad: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStylePad);

            return metadata;
        }

        case DescriptorID::Fallback: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleName);

            return metadata;
        }

        case DescriptorID::Symbols: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::Symbols);

            return metadata;
        }

        case DescriptorID::AdditiveSymbols: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleAdditiveSymbols);

            return metadata;
        }

        case DescriptorID::SpeakAs: {
            DescriptorMetadata metadata;

            metadata.syntax.empend(Keyword::Auto);

            metadata.syntax.empend(Keyword::Bullets);

            metadata.syntax.empend(Keyword::Numbers);

            metadata.syntax.empend(Keyword::Words);

            metadata.syntax.empend(Keyword::SpellOut);

            metadata.syntax.empend(DescriptorMetadata::ValueType::CounterStyleName);

            return metadata;
        }

        default:
            VERIFY_NOT_REACHED();
        }

    }
    VERIFY_NOT_REACHED();
}

}
