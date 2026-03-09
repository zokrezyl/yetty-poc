
#pragma once

#include <AK/FlyString.h>
#include <AK/Optional.h>
#include <AK/Types.h>
#include <LibWeb/Forward.h>

namespace Web::CSS {

enum class AtRuleID : u8 {
    FontFace,
    Page,
    Property,
    CounterStyle,

};

FlyString to_string(AtRuleID);

enum class DescriptorID : u8 {
    AdditiveSymbols,
    AscentOverride,
    Bleed,
    DescentOverride,
    Fallback,
    FontDisplay,
    FontFamily,
    FontFeatureSettings,
    FontLanguageOverride,
    FontNamedInstance,
    FontStyle,
    FontVariationSettings,
    FontWeight,
    FontWidth,
    Inherits,
    InitialValue,
    LineGapOverride,
    Margin,
    MarginBottom,
    MarginLeft,
    MarginRight,
    MarginTop,
    Marks,
    Negative,
    Pad,
    PageOrientation,
    Prefix,
    Range,
    Size,
    SpeakAs,
    Src,
    Suffix,
    Symbols,
    Syntax,
    System,
    UnicodeRange,

};

Optional<DescriptorID> descriptor_id_from_string(AtRuleID, StringView);
FlyString to_string(DescriptorID);

bool at_rule_supports_descriptor(AtRuleID, DescriptorID);
RefPtr<StyleValue const> descriptor_initial_value(AtRuleID, DescriptorID);

struct DescriptorMetadata {
    enum class ValueType {
        // FIXME: Parse the grammar instead of hard-coding all the options!
        CounterStyleSystem,
        CounterStyleAdditiveSymbols,
        CounterStyleName,
        CounterStyleNegative,
        CounterStylePad,
        CounterStyleRange,
        CropOrCross,
        FamilyName,
        FontSrcList,
        FontWeightAbsolutePair,
        Length,
        OptionalDeclarationValue,
        PageSize,
        PositivePercentage,
        String,
        Symbol,
        Symbols,
        UnicodeRangeTokens,
    };
    Vector<Variant<Keyword, PropertyID, ValueType>> syntax;
};

DescriptorMetadata get_descriptor_metadata(AtRuleID, DescriptorID);

}
