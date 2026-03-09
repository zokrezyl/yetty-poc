
#pragma once

#include <AK/NonnullRefPtr.h>
#include <AK/StringView.h>
#include <AK/Traits.h>
#include <AK/Variant.h>
#include <LibJS/Forward.h>
#include <LibWeb/CSS/AcceptedTypeRange.h>
#include <LibWeb/CSS/ValueType.h>
#include <LibWeb/Export.h>
#include <LibWeb/Forward.h>

namespace Web::CSS {

enum class PropertyID : u16 {
    Custom,

        All,

        Animation,

        Background,

        BackgroundPosition,

        Border,

        BorderBlock,

        BorderBlockColor,

        BorderBlockEnd,

        BorderBlockStart,

        BorderBlockStyle,

        BorderBlockWidth,

        BorderBottom,

        BorderColor,

        BorderImage,

        BorderInline,

        BorderInlineColor,

        BorderInlineEnd,

        BorderInlineStart,

        BorderInlineStyle,

        BorderInlineWidth,

        BorderLeft,

        BorderRadius,

        BorderRight,

        BorderStyle,

        BorderTop,

        BorderWidth,

        Columns,

        CornerBlockEndShape,

        CornerBlockStartShape,

        CornerBottomShape,

        CornerInlineEndShape,

        CornerInlineStartShape,

        CornerLeftShape,

        CornerRightShape,

        CornerShape,

        CornerTopShape,

        Flex,

        FlexFlow,

        Font,

        FontVariant,

        Gap,

        Grid,

        GridArea,

        GridColumn,

        GridRow,

        GridTemplate,

        Inset,

        InsetBlock,

        InsetInline,

        ListStyle,

        Margin,

        MarginBlock,

        MarginInline,

        Mask,

        Outline,

        Overflow,

        OverflowClipMargin,

        OverflowClipMarginBlock,

        OverflowClipMarginInline,

        Padding,

        PaddingBlock,

        PaddingInline,

        PlaceContent,

        PlaceItems,

        PlaceSelf,

        ScrollTimeline,

        TextDecoration,

        TextWrap,

        Transition,

        ViewTimeline,

        WhiteSpace,

        WebkitTextFillColor,

        AccentColor,

        BorderCollapse,

        BorderSpacing,

        CaptionSide,

        CaretColor,

        ClipRule,

        Color,

        ColorInterpolation,

        ColorScheme,

        Cursor,

        Direction,

        DominantBaseline,

        EmptyCells,

        Fill,

        FillOpacity,

        FillRule,

        FontFamily,

        FontFeatureSettings,

        FontKerning,

        FontLanguageOverride,

        FontOpticalSizing,

        FontSize,

        FontStyle,

        FontVariantAlternates,

        FontVariantCaps,

        FontVariantEastAsian,

        FontVariantEmoji,

        FontVariantLigatures,

        FontVariantNumeric,

        FontVariantPosition,

        FontVariationSettings,

        FontWeight,

        FontWidth,

        ImageRendering,

        LetterSpacing,

        LineHeight,

        ListStyleImage,

        ListStylePosition,

        ListStyleType,

        MathDepth,

        MathShift,

        MathStyle,

        Orphans,

        OverflowWrap,

        PaintOrder,

        PointerEvents,

        Quotes,

        ScrollbarColor,

        ShapeRendering,

        Stroke,

        StrokeDasharray,

        StrokeDashoffset,

        StrokeLinecap,

        StrokeLinejoin,

        StrokeMiterlimit,

        StrokeOpacity,

        StrokeWidth,

        TabSize,

        TextAlign,

        TextAnchor,

        TextDecorationLine,

        TextIndent,

        TextJustify,

        TextRendering,

        TextShadow,

        TextTransform,

        TextUnderlineOffset,

        TextUnderlinePosition,

        TextWrapMode,

        TextWrapStyle,

        Visibility,

        WhiteSpaceCollapse,

        Widows,

        WordBreak,

        WordSpacing,

        WritingMode,

        AlignContent,

        AlignItems,

        AlignSelf,

        AnchorName,

        AnchorScope,

        AnimationComposition,

        AnimationDelay,

        AnimationDirection,

        AnimationDuration,

        AnimationFillMode,

        AnimationIterationCount,

        AnimationName,

        AnimationPlayState,

        AnimationTimeline,

        AnimationTimingFunction,

        Appearance,

        AspectRatio,

        BackdropFilter,

        BackgroundAttachment,

        BackgroundBlendMode,

        BackgroundClip,

        BackgroundColor,

        BackgroundImage,

        BackgroundOrigin,

        BackgroundPositionX,

        BackgroundPositionY,

        BackgroundRepeat,

        BackgroundSize,

        BlockSize,

        BorderBlockEndColor,

        BorderBlockEndStyle,

        BorderBlockEndWidth,

        BorderBlockStartColor,

        BorderBlockStartStyle,

        BorderBlockStartWidth,

        BorderBottomColor,

        BorderBottomLeftRadius,

        BorderBottomRightRadius,

        BorderBottomStyle,

        BorderBottomWidth,

        BorderEndEndRadius,

        BorderEndStartRadius,

        BorderImageOutset,

        BorderImageRepeat,

        BorderImageSlice,

        BorderImageSource,

        BorderImageWidth,

        BorderInlineEndColor,

        BorderInlineEndStyle,

        BorderInlineEndWidth,

        BorderInlineStartColor,

        BorderInlineStartStyle,

        BorderInlineStartWidth,

        BorderLeftColor,

        BorderLeftStyle,

        BorderLeftWidth,

        BorderRightColor,

        BorderRightStyle,

        BorderRightWidth,

        BorderStartEndRadius,

        BorderStartStartRadius,

        BorderTopColor,

        BorderTopLeftRadius,

        BorderTopRightRadius,

        BorderTopStyle,

        BorderTopWidth,

        Bottom,

        BoxShadow,

        BoxSizing,

        Clear,

        Clip,

        ClipPath,

        ColumnCount,

        ColumnGap,

        ColumnHeight,

        ColumnSpan,

        ColumnWidth,

        Contain,

        ContainerType,

        Content,

        ContentVisibility,

        CornerBottomLeftShape,

        CornerBottomRightShape,

        CornerEndEndShape,

        CornerEndStartShape,

        CornerStartEndShape,

        CornerStartStartShape,

        CornerTopLeftShape,

        CornerTopRightShape,

        CounterIncrement,

        CounterReset,

        CounterSet,

        Cx,

        Cy,

        Display,

        Filter,

        FlexBasis,

        FlexDirection,

        FlexGrow,

        FlexShrink,

        FlexWrap,

        Float,

        FloodColor,

        FloodOpacity,

        GridAutoColumns,

        GridAutoFlow,

        GridAutoRows,

        GridColumnEnd,

        GridColumnStart,

        GridRowEnd,

        GridRowStart,

        GridTemplateAreas,

        GridTemplateColumns,

        GridTemplateRows,

        Height,

        InlineSize,

        InsetBlockEnd,

        InsetBlockStart,

        InsetInlineEnd,

        InsetInlineStart,

        Isolation,

        JustifyContent,

        JustifyItems,

        JustifySelf,

        Left,

        MarginBlockEnd,

        MarginBlockStart,

        MarginBottom,

        MarginInlineEnd,

        MarginInlineStart,

        MarginLeft,

        MarginRight,

        MarginTop,

        MaskClip,

        MaskComposite,

        MaskImage,

        MaskMode,

        MaskOrigin,

        MaskPosition,

        MaskRepeat,

        MaskSize,

        MaskType,

        MaxBlockSize,

        MaxHeight,

        MaxInlineSize,

        MaxWidth,

        MinBlockSize,

        MinHeight,

        MinInlineSize,

        MinWidth,

        MixBlendMode,

        ObjectFit,

        ObjectPosition,

        Opacity,

        Order,

        OutlineColor,

        OutlineOffset,

        OutlineStyle,

        OutlineWidth,

        OverflowBlock,

        OverflowClipMarginBlockEnd,

        OverflowClipMarginBlockStart,

        OverflowClipMarginBottom,

        OverflowClipMarginInlineEnd,

        OverflowClipMarginInlineStart,

        OverflowClipMarginLeft,

        OverflowClipMarginRight,

        OverflowClipMarginTop,

        OverflowInline,

        OverflowX,

        OverflowY,

        PaddingBlockEnd,

        PaddingBlockStart,

        PaddingBottom,

        PaddingInlineEnd,

        PaddingInlineStart,

        PaddingLeft,

        PaddingRight,

        PaddingTop,

        Perspective,

        PerspectiveOrigin,

        Position,

        PositionAnchor,

        PositionArea,

        PositionTryFallbacks,

        PositionTryOrder,

        PositionVisibility,

        R,

        Resize,

        Right,

        Rotate,

        RowGap,

        Rx,

        Ry,

        Scale,

        ScrollTimelineAxis,

        ScrollTimelineName,

        ScrollbarGutter,

        ScrollbarWidth,

        ShapeImageThreshold,

        ShapeMargin,

        ShapeOutside,

        StopColor,

        StopOpacity,

        TableLayout,

        TextDecorationColor,

        TextDecorationStyle,

        TextDecorationThickness,

        TextOverflow,

        TimelineScope,

        Top,

        TouchAction,

        Transform,

        TransformBox,

        TransformOrigin,

        TransformStyle,

        TransitionBehavior,

        TransitionDelay,

        TransitionDuration,

        TransitionProperty,

        TransitionTimingFunction,

        Translate,

        UnicodeBidi,

        UserSelect,

        VerticalAlign,

        ViewTimelineAxis,

        ViewTimelineInset,

        ViewTimelineName,

        ViewTransitionName,

        WhiteSpaceTrim,

        Width,

        WillChange,

        X,

        Y,

        ZIndex,

};

enum class AnimationType {
    Discrete,
    ByComputedValue,
    RepeatableList,
    Custom,
    None,
};
AnimationType animation_type_from_longhand_property(PropertyID);
bool is_animatable_property(PropertyID);

Optional<PropertyID> property_id_from_camel_case_string(StringView);
WEB_API Optional<PropertyID> property_id_from_string(StringView);
[[nodiscard]] WEB_API FlyString const& string_from_property_id(PropertyID);
[[nodiscard]] FlyString const& camel_case_string_from_property_id(PropertyID);
WEB_API bool is_inherited_property(PropertyID);
NonnullRefPtr<StyleValue const> property_initial_value(PropertyID);

enum class PropertyMultiplicity {
    Single,
    List,
    CoordinatingList,
};
PropertyMultiplicity property_multiplicity(PropertyID);
bool property_is_single_valued(PropertyID);
bool property_is_list_valued(PropertyID);

bool property_accepts_type(PropertyID, ValueType);
AcceptedTypeRangeMap property_accepted_type_ranges(PropertyID);
bool property_accepts_keyword(PropertyID, Keyword);
Optional<Keyword> resolve_legacy_value_alias(PropertyID, Keyword);
Optional<ValueType> property_resolves_percentages_relative_to(PropertyID);
Vector<StringView> property_custom_ident_blacklist(PropertyID);

// These perform range-checking, but are also safe to call with properties that don't accept that type. (They'll just return false.)
bool property_accepts_angle(PropertyID, Angle const&);
bool property_accepts_flex(PropertyID, Flex const&);
bool property_accepts_frequency(PropertyID, Frequency const&);
bool property_accepts_integer(PropertyID, i64 const&);
bool property_accepts_length(PropertyID, Length const&);
bool property_accepts_number(PropertyID, double const&);
bool property_accepts_percentage(PropertyID, Percentage const&);
bool property_accepts_resolution(PropertyID, Resolution const&);
bool property_accepts_time(PropertyID, Time const&);

bool property_is_shorthand(PropertyID);
Vector<PropertyID> const& longhands_for_shorthand(PropertyID);
Vector<PropertyID> const& expanded_longhands_for_shorthand(PropertyID);
bool property_maps_to_shorthand(PropertyID);
Vector<PropertyID> const& shorthands_for_longhand(PropertyID);
Vector<PropertyID> const& property_computation_order();
bool property_is_positional_value_list_shorthand(PropertyID);

bool property_requires_computation_with_inherited_value(PropertyID);
bool property_requires_computation_with_initial_value(PropertyID);
bool property_requires_computation_with_cascaded_value(PropertyID);

size_t property_maximum_value_count(PropertyID);

bool property_affects_layout(PropertyID);
bool property_affects_stacking_context(PropertyID);
bool property_needs_layout_for_getcomputedstyle(PropertyID);
bool property_needs_layout_node_for_resolved_value(PropertyID);

constexpr PropertyID first_property_id = PropertyID::All;
constexpr PropertyID last_property_id = PropertyID::ZIndex;
constexpr PropertyID first_inherited_property_id = PropertyID::WebkitTextFillColor;
constexpr PropertyID last_inherited_property_id = PropertyID::WritingMode;
constexpr PropertyID first_longhand_property_id = PropertyID::WebkitTextFillColor;
constexpr PropertyID last_longhand_property_id = PropertyID::ZIndex;
constexpr size_t number_of_longhand_properties = to_underlying(last_longhand_property_id) - to_underlying(first_longhand_property_id) + 1;

enum class Quirk {
    // https://quirks.spec.whatwg.org/#the-hashless-hex-color-quirk
    HashlessHexColor,
    // https://quirks.spec.whatwg.org/#the-unitless-length-quirk
    UnitlessLength,
};
bool property_has_quirk(PropertyID, Quirk);

struct LogicalAliasMappingContext {
    WritingMode writing_mode;
    Direction direction;
    // TODO: text-orientation
};
bool property_is_logical_alias(PropertyID);
PropertyID map_logical_alias_to_physical_property(PropertyID logical_property_id, LogicalAliasMappingContext const&);

enum class LogicalPropertyGroup : u8 {

    BorderColor,

    BorderRadius,

    BorderStyle,

    BorderWidth,

    CornerShape,

    Inset,

    Margin,

    MaxSize,

    MinSize,

    Overflow,

    OverflowClipMargin,

    Padding,

    Size,

};

Optional<LogicalPropertyGroup> logical_property_group_for_property(PropertyID);

} // namespace Web::CSS

namespace AK {

template<>
struct Formatter<Web::CSS::PropertyID> : Formatter<StringView> {
    ErrorOr<void> format(FormatBuilder& builder, Web::CSS::PropertyID const& property_id)
    {
        return Formatter<StringView>::format(builder, Web::CSS::string_from_property_id(property_id));
    }
};
} // namespace AK
