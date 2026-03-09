
#pragma once

#include <AK/Optional.h>
#include <LibWeb/Forward.h>

namespace Web::CSS {

enum class AbsoluteSize : u8 {
    XxSmall,
    XSmall,
    Small,
    Medium,
    Large,
    XLarge,
    XxLarge,
    XxxLarge,
};
Optional<AbsoluteSize> keyword_to_absolute_size(Keyword);
Keyword to_keyword(AbsoluteSize);
StringView to_string(AbsoluteSize);

enum class AlignContent : u8 {
    Normal,
    Start,
    FlexStart,
    End,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly,
    Stretch,
};
Optional<AlignContent> keyword_to_align_content(Keyword);
Keyword to_keyword(AlignContent);
StringView to_string(AlignContent);

enum class AlignItems : u8 {
    Baseline,
    Center,
    End,
    FlexEnd,
    FlexStart,
    Normal,
    Safe,
    SelfEnd,
    SelfStart,
    Start,
    Stretch,
    Unsafe,
};
Optional<AlignItems> keyword_to_align_items(Keyword);
Keyword to_keyword(AlignItems);
StringView to_string(AlignItems);

enum class AlignSelf : u8 {
    Auto,
    Baseline,
    Center,
    End,
    FlexEnd,
    FlexStart,
    Normal,
    Safe,
    SelfEnd,
    SelfStart,
    Start,
    Stretch,
    Unsafe,
};
Optional<AlignSelf> keyword_to_align_self(Keyword);
Keyword to_keyword(AlignSelf);
StringView to_string(AlignSelf);

enum class AnchorSide : u8 {
    Inside,
    Outside,
    Top,
    Left,
    Right,
    Bottom,
    Start,
    End,
    SelfStart,
    SelfEnd,
    Center,
};
Optional<AnchorSide> keyword_to_anchor_side(Keyword);
Keyword to_keyword(AnchorSide);
StringView to_string(AnchorSide);

enum class AnchorSize : u8 {
    Block,
    Height,
    Inline,
    SelfBlock,
    SelfInline,
    Width,
};
Optional<AnchorSize> keyword_to_anchor_size(Keyword);
Keyword to_keyword(AnchorSize);
StringView to_string(AnchorSize);

enum class AnimationComposition : u8 {
    Replace,
    Add,
    Accumulate,
};
Optional<AnimationComposition> keyword_to_animation_composition(Keyword);
Keyword to_keyword(AnimationComposition);
StringView to_string(AnimationComposition);

enum class AnimationDirection : u8 {
    Alternate,
    AlternateReverse,
    Normal,
    Reverse,
};
Optional<AnimationDirection> keyword_to_animation_direction(Keyword);
Keyword to_keyword(AnimationDirection);
StringView to_string(AnimationDirection);

enum class AnimationFillMode : u8 {
    Backwards,
    Both,
    Forwards,
    None,
};
Optional<AnimationFillMode> keyword_to_animation_fill_mode(Keyword);
Keyword to_keyword(AnimationFillMode);
StringView to_string(AnimationFillMode);

enum class AnimationPlayState : u8 {
    Paused,
    Running,
};
Optional<AnimationPlayState> keyword_to_animation_play_state(Keyword);
Keyword to_keyword(AnimationPlayState);
StringView to_string(AnimationPlayState);

enum class Appearance : u8 {
    Auto,
    Button,
    Checkbox,
    Listbox,
    Menulist,
    Meter,
    MenulistButton,
    None,
    PushButton,
    ProgressBar,
    Radio,
    Searchfield,
    SliderHorizontal,
    SquareButton,
    Textarea,
    Textfield,
};
Optional<Appearance> keyword_to_appearance(Keyword);
Keyword to_keyword(Appearance);
StringView to_string(Appearance);

enum class Axis : u8 {
    Block,
    Inline,
    X,
    Y,
};
Optional<Axis> keyword_to_axis(Keyword);
Keyword to_keyword(Axis);
StringView to_string(Axis);

enum class BackgroundAttachment : u8 {
    Fixed,
    Local,
    Scroll,
};
Optional<BackgroundAttachment> keyword_to_background_attachment(Keyword);
Keyword to_keyword(BackgroundAttachment);
StringView to_string(BackgroundAttachment);

enum class BackgroundBox : u8 {
    BorderBox,
    ContentBox,
    PaddingBox,
    Text,
};
Optional<BackgroundBox> keyword_to_background_box(Keyword);
Keyword to_keyword(BackgroundBox);
StringView to_string(BackgroundBox);

enum class BaselineMetric : u8 {
    TextBottom,
    Alphabetic,
    Ideographic,
    Middle,
    Central,
    Mathematical,
    Hanging,
    TextTop,
};
Optional<BaselineMetric> keyword_to_baseline_metric(Keyword);
Keyword to_keyword(BaselineMetric);
StringView to_string(BaselineMetric);

enum class BorderCollapse : u8 {
    Separate,
    Collapse,
};
Optional<BorderCollapse> keyword_to_border_collapse(Keyword);
Keyword to_keyword(BorderCollapse);
StringView to_string(BorderCollapse);

enum class BorderImageRepeat : u8 {
    Stretch,
    Repeat,
    Round,
    Space,
};
Optional<BorderImageRepeat> keyword_to_border_image_repeat(Keyword);
Keyword to_keyword(BorderImageRepeat);
StringView to_string(BorderImageRepeat);

enum class BoxSizing : u8 {
    BorderBox,
    ContentBox,
};
Optional<BoxSizing> keyword_to_box_sizing(Keyword);
Keyword to_keyword(BoxSizing);
StringView to_string(BoxSizing);

enum class CaptionSide : u8 {
    Top,
    Bottom,
};
Optional<CaptionSide> keyword_to_caption_side(Keyword);
Keyword to_keyword(CaptionSide);
StringView to_string(CaptionSide);

enum class Clear : u8 {
    None,
    Left,
    Right,
    Both,
    InlineStart,
    InlineEnd,
};
Optional<Clear> keyword_to_clear(Keyword);
Keyword to_keyword(Clear);
StringView to_string(Clear);

enum class ColorInterpolation : u8 {
    Auto,
    Linearrgb,
    Srgb,
};
Optional<ColorInterpolation> keyword_to_color_interpolation(Keyword);
Keyword to_keyword(ColorInterpolation);
StringView to_string(ColorInterpolation);

enum class ColumnSpan : u8 {
    None,
    All,
};
Optional<ColumnSpan> keyword_to_column_span(Keyword);
Keyword to_keyword(ColumnSpan);
StringView to_string(ColumnSpan);

enum class CommonLigValue : u8 {
    CommonLigatures,
    NoCommonLigatures,
};
Optional<CommonLigValue> keyword_to_common_lig_value(Keyword);
Keyword to_keyword(CommonLigValue);
StringView to_string(CommonLigValue);

enum class CompositingOperator : u8 {
    Add,
    Subtract,
    Intersect,
    Exclude,
};
Optional<CompositingOperator> keyword_to_compositing_operator(Keyword);
Keyword to_keyword(CompositingOperator);
StringView to_string(CompositingOperator);

enum class Contain : u8 {
    None,
    Strict,
    Content,
    Size,
    InlineSize,
    Layout,
    Style,
    Paint,
};
Optional<Contain> keyword_to_contain(Keyword);
Keyword to_keyword(Contain);
StringView to_string(Contain);

enum class ContentVisibility : u8 {
    Visible,
    Auto,
    Hidden,
};
Optional<ContentVisibility> keyword_to_content_visibility(Keyword);
Keyword to_keyword(ContentVisibility);
StringView to_string(ContentVisibility);

enum class ContextualAltValue : u8 {
    Contextual,
    NoContextual,
};
Optional<ContextualAltValue> keyword_to_contextual_alt_value(Keyword);
Keyword to_keyword(ContextualAltValue);
StringView to_string(ContextualAltValue);

enum class CoordBox : u8 {
    ContentBox,
    PaddingBox,
    BorderBox,
    FillBox,
    StrokeBox,
    ViewBox,
};
Optional<CoordBox> keyword_to_coord_box(Keyword);
Keyword to_keyword(CoordBox);
StringView to_string(CoordBox);

enum class CounterStyleNameKeyword : u8 {
    Adlam,
    Afar,
    Agaw,
    AncientTamil,
    ArabicAbjad,
    ArabicIndic,
    Ari,
    Armenian,
    Balinese,
    Bamum,
    Bangla,
    Bengali,
    Binary,
    Blin,
    Bodo,
    Cambodian,
    Circle,
    CircledDecimal,
    CircledIdeograph,
    CircledKatakana,
    CircledKoreanConsonant,
    CircledKoreanSyllable,
    CircledLowerLatin,
    CircledUpperLatin,
    CjkDecimal,
    CjkEarthlyBranch,
    CjkHeavenlyStem,
    CjkIdeographic,
    CjkStemBranch,
    CjkTallyMark,
    Decimal,
    DecimalLeadingZero,
    Devanagari,
    Disc,
    DisclosureClosed,
    DisclosureOpen,
    Dizi,
    Dogri,
    DottedDecimal,
    DoubleCircledDecimal,
    EthiopicHalehameAm,
    EthiopicHalehameTiEr,
    EthiopicHalehameTiEt,
    EthiopicHalehame,
    EthiopicNumeric,
    FilledCircledDecimal,
    FullwidthDecimal,
    FullwidthLowerAlpha,
    FullwidthUpperAlpha,
    FullwidthUpperRoman,
    Gedeo,
    Georgian,
    GreekLowerAncient,
    GreekLowerModern,
    GreekUpperAncient,
    GreekUpperModern,
    GujaratiAlpha,
    Gujarati,
    Gumuz,
    Gurmukhi,
    Hadiyaa,
    HangulConsonant,
    Hangul,
    HanifiRohingya,
    Harari,
    Hebrew,
    Hindi,
    HiraganaIroha,
    Hiragana,
    JapaneseFormal,
    JapaneseInformal,
    Javanese,
    Kaffa,
    KannadaAlpha,
    Kannada,
    Kashmiri,
    KatakanaIroha,
    Katakana,
    KayahLi,
    Kebena,
    Kembata,
    KhmerConsonant,
    Khmer,
    Konkani,
    Konso,
    KoreanConsonant,
    KoreanHangulFormal,
    KoreanHanjaFormal,
    KoreanHanjaInformal,
    KoreanSyllable,
    Kunama,
    LannaHora,
    LannaTham,
    Lao,
    Lepcha,
    Limbu,
    LowerAlphaSymbolic,
    LowerAlpha,
    LowerArmenian,
    LowerBelorussian,
    LowerBulgarian,
    LowerGreek,
    LowerHexadecimal,
    LowerLatin,
    LowerMacedonian,
    LowerOromoQubee,
    LowerRoman,
    LowerRussianFull,
    LowerRussian,
    LowerSerbian,
    LowerUkrainianFull,
    LowerUkrainian,
    MaghrebiAbjad,
    Maithili,
    MalayalamAlpha,
    Malayalam,
    Manipuri,
    Marathi,
    Meen,
    Meetei,
    Mongolian,
    Mro,
    Myanmar,
    NagMundari,
    NewBase60,
    Newa,
    NkoCardinal,
    Octal,
    Odia,
    OlChiki,
    Oriya,
    Oromo,
    ParenthesizedDecimal,
    ParenthesizedHangulConsonant,
    ParenthesizedHangulSyllable,
    ParenthesizedIdeograph,
    ParenthesizedLowerLatin,
    PersianAbjad,
    PersianAlphabetic,
    Persian,
    Punjabi,
    Saho,
    Sanskrit,
    Santali,
    Shan,
    Sidama,
    Silti,
    SimpChineseFormal,
    SimpChineseInformal,
    SimpleLowerRoman,
    SimpleUpperRoman,
    Sindhi,
    Square,
    Sundanese,
    SuperDecimal,
    TaiLue,
    TallyMark,
    Tamil,
    TeluguAlpha,
    Telugu,
    ThaiAlpha,
    Thai,
    Tibetan,
    Tigre,
    TradChineseFormal,
    TradChineseInformal,
    UpperAlphaSymbolic,
    UpperAlpha,
    UpperArmenian,
    UpperBelorussian,
    UpperBulgarian,
    UpperHexadecimal,
    UpperLatin,
    UpperMacedonian,
    UpperOromoQubee,
    UpperRoman,
    UpperRussianFull,
    UpperRussian,
    UpperSerbian,
    UpperUkrainianFull,
    UpperUkrainian,
    UrduAbjad,
    UrduAlphabetic,
    Urdu,
    WarangCiti,
    Wolaita,
    Yemsa,
    Zhuyin,
};
Optional<CounterStyleNameKeyword> keyword_to_counter_style_name_keyword(Keyword);
Keyword to_keyword(CounterStyleNameKeyword);
StringView to_string(CounterStyleNameKeyword);

enum class CounterStyleSystem : u8 {
    Cyclic,
    Numeric,
    Alphabetic,
    Symbolic,
    Additive,
};
Optional<CounterStyleSystem> keyword_to_counter_style_system(Keyword);
Keyword to_keyword(CounterStyleSystem);
StringView to_string(CounterStyleSystem);

enum class CrossOriginModifierValue : u8 {
    Anonymous,
    UseCredentials,
};
Optional<CrossOriginModifierValue> keyword_to_cross_origin_modifier_value(Keyword);
Keyword to_keyword(CrossOriginModifierValue);
StringView to_string(CrossOriginModifierValue);

enum class CursorPredefined : u8 {
    Auto,
    Default,
    None,
    ContextMenu,
    Help,
    Pointer,
    Progress,
    Wait,
    Cell,
    Crosshair,
    Text,
    VerticalText,
    Alias,
    Copy,
    Move,
    NoDrop,
    NotAllowed,
    Grab,
    Grabbing,
    EResize,
    NResize,
    NeResize,
    NwResize,
    SResize,
    SeResize,
    SwResize,
    WResize,
    EwResize,
    NsResize,
    NeswResize,
    NwseResize,
    ColResize,
    RowResize,
    AllScroll,
    ZoomIn,
    ZoomOut,
};
Optional<CursorPredefined> keyword_to_cursor_predefined(Keyword);
Keyword to_keyword(CursorPredefined);
StringView to_string(CursorPredefined);

enum class Direction : u8 {
    Ltr,
    Rtl,
};
Optional<Direction> keyword_to_direction(Keyword);
Keyword to_keyword(Direction);
StringView to_string(Direction);

enum class DiscretionaryLigValue : u8 {
    DiscretionaryLigatures,
    NoDiscretionaryLigatures,
};
Optional<DiscretionaryLigValue> keyword_to_discretionary_lig_value(Keyword);
Keyword to_keyword(DiscretionaryLigValue);
StringView to_string(DiscretionaryLigValue);

enum class DisplayBox : u8 {
    Contents,
    None,
};
Optional<DisplayBox> keyword_to_display_box(Keyword);
Keyword to_keyword(DisplayBox);
StringView to_string(DisplayBox);

enum class DisplayInside : u8 {
    Flow,
    FlowRoot,
    Table,
    Flex,
    Grid,
    Ruby,
    Math,
};
Optional<DisplayInside> keyword_to_display_inside(Keyword);
Keyword to_keyword(DisplayInside);
StringView to_string(DisplayInside);

enum class DisplayInternal : u8 {
    TableRowGroup,
    TableHeaderGroup,
    TableFooterGroup,
    TableRow,
    TableCell,
    TableColumnGroup,
    TableColumn,
    TableCaption,
    RubyBase,
    RubyText,
    RubyBaseContainer,
    RubyTextContainer,
};
Optional<DisplayInternal> keyword_to_display_internal(Keyword);
Keyword to_keyword(DisplayInternal);
StringView to_string(DisplayInternal);

enum class DisplayLegacy : u8 {
    InlineBlock,
    InlineTable,
    InlineFlex,
    InlineGrid,
};
Optional<DisplayLegacy> keyword_to_display_legacy(Keyword);
Keyword to_keyword(DisplayLegacy);
StringView to_string(DisplayLegacy);

enum class DisplayOutside : u8 {
    Block,
    Inline,
    RunIn,
};
Optional<DisplayOutside> keyword_to_display_outside(Keyword);
Keyword to_keyword(DisplayOutside);
StringView to_string(DisplayOutside);

enum class EasingKeyword : u8 {
    Linear,
    Ease,
    EaseIn,
    EaseOut,
    EaseInOut,
};
Optional<EasingKeyword> keyword_to_easing_keyword(Keyword);
Keyword to_keyword(EasingKeyword);
StringView to_string(EasingKeyword);

enum class EastAsianVariant : u8 {
    Jis78,
    Jis83,
    Jis90,
    Jis04,
    Simplified,
    Traditional,
};
Optional<EastAsianVariant> keyword_to_east_asian_variant(Keyword);
Keyword to_keyword(EastAsianVariant);
StringView to_string(EastAsianVariant);

enum class EastAsianWidth : u8 {
    FullWidth,
    ProportionalWidth,
};
Optional<EastAsianWidth> keyword_to_east_asian_width(Keyword);
Keyword to_keyword(EastAsianWidth);
StringView to_string(EastAsianWidth);

enum class EmptyCells : u8 {
    Show,
    Hide,
};
Optional<EmptyCells> keyword_to_empty_cells(Keyword);
Keyword to_keyword(EmptyCells);
StringView to_string(EmptyCells);

enum class FillRule : u8 {
    Nonzero,
    Evenodd,
};
Optional<FillRule> keyword_to_fill_rule(Keyword);
Keyword to_keyword(FillRule);
StringView to_string(FillRule);

enum class FlexDirection : u8 {
    Row,
    RowReverse,
    Column,
    ColumnReverse,
};
Optional<FlexDirection> keyword_to_flex_direction(Keyword);
Keyword to_keyword(FlexDirection);
StringView to_string(FlexDirection);

enum class FlexWrap : u8 {
    Nowrap,
    Wrap,
    WrapReverse,
};
Optional<FlexWrap> keyword_to_flex_wrap(Keyword);
Keyword to_keyword(FlexWrap);
StringView to_string(FlexWrap);

enum class Float : u8 {
    None,
    Left,
    Right,
    InlineStart,
    InlineEnd,
};
Optional<Float> keyword_to_float(Keyword);
Keyword to_keyword(Float);
StringView to_string(Float);

enum class FontDisplay : u8 {
    Auto,
    Block,
    Swap,
    Fallback,
    Optional,
};
Optional<FontDisplay> keyword_to_font_display(Keyword);
Keyword to_keyword(FontDisplay);
StringView to_string(FontDisplay);

enum class FontKerning : u8 {
    Auto,
    Normal,
    None,
};
Optional<FontKerning> keyword_to_font_kerning(Keyword);
Keyword to_keyword(FontKerning);
StringView to_string(FontKerning);

enum class FontOpticalSizing : u8 {
    Auto,
    None,
};
Optional<FontOpticalSizing> keyword_to_font_optical_sizing(Keyword);
Keyword to_keyword(FontOpticalSizing);
StringView to_string(FontOpticalSizing);

enum class FontStyleKeyword : u8 {
    Normal,
    Italic,
    Left,
    Right,
    Oblique,
};
Optional<FontStyleKeyword> keyword_to_font_style_keyword(Keyword);
Keyword to_keyword(FontStyleKeyword);
StringView to_string(FontStyleKeyword);

enum class FontTech : u8 {
    Avar2,
    ColorCbdt,
    ColorColrv0,
    ColorColrv1,
    ColorSbix,
    ColorSvg,
    FeaturesAat,
    FeaturesGraphite,
    FeaturesOpentype,
    Incremental,
    Palettes,
    Variations,
};
Optional<FontTech> keyword_to_font_tech(Keyword);
Keyword to_keyword(FontTech);
StringView to_string(FontTech);

enum class FontVariantCaps : u8 {
    Normal,
    SmallCaps,
    AllSmallCaps,
    PetiteCaps,
    AllPetiteCaps,
    Unicase,
    TitlingCaps,
};
Optional<FontVariantCaps> keyword_to_font_variant_caps(Keyword);
Keyword to_keyword(FontVariantCaps);
StringView to_string(FontVariantCaps);

enum class FontVariantEmoji : u8 {
    Normal,
    Text,
    Emoji,
    Unicode,
};
Optional<FontVariantEmoji> keyword_to_font_variant_emoji(Keyword);
Keyword to_keyword(FontVariantEmoji);
StringView to_string(FontVariantEmoji);

enum class FontVariantPosition : u8 {
    Normal,
    Sub,
    Super,
};
Optional<FontVariantPosition> keyword_to_font_variant_position(Keyword);
Keyword to_keyword(FontVariantPosition);
StringView to_string(FontVariantPosition);

enum class FontWidth : u8 {
    UltraCondensed,
    ExtraCondensed,
    Condensed,
    SemiCondensed,
    Normal,
    SemiExpanded,
    Expanded,
    ExtraExpanded,
    UltraExpanded,
};
Optional<FontWidth> keyword_to_font_width(Keyword);
Keyword to_keyword(FontWidth);
StringView to_string(FontWidth);

enum class GenericFontFamily : u8 {
    Serif,
    SansSerif,
    Cursive,
    Fantasy,
    Monospace,
    Math,
    UiSerif,
    UiSansSerif,
    UiMonospace,
    UiRounded,
};
Optional<GenericFontFamily> keyword_to_generic_font_family(Keyword);
Keyword to_keyword(GenericFontFamily);
StringView to_string(GenericFontFamily);

enum class HistoricalLigValue : u8 {
    HistoricalLigatures,
    NoHistoricalLigatures,
};
Optional<HistoricalLigValue> keyword_to_historical_lig_value(Keyword);
Keyword to_keyword(HistoricalLigValue);
StringView to_string(HistoricalLigValue);

enum class HueInterpolationMethod : u8 {
    Shorter,
    Longer,
    Increasing,
    Decreasing,
};
Optional<HueInterpolationMethod> keyword_to_hue_interpolation_method(Keyword);
Keyword to_keyword(HueInterpolationMethod);
StringView to_string(HueInterpolationMethod);

enum class ImageRendering : u8 {
    Auto,
    CrispEdges,
    HighQuality,
    Pixelated,
    Smooth,
};
Optional<ImageRendering> keyword_to_image_rendering(Keyword);
Keyword to_keyword(ImageRendering);
StringView to_string(ImageRendering);

enum class Isolation : u8 {
    Auto,
    Isolate,
};
Optional<Isolation> keyword_to_isolation(Keyword);
Keyword to_keyword(Isolation);
StringView to_string(Isolation);

enum class JustifyContent : u8 {
    Normal,
    Start,
    End,
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly,
    Stretch,
    Left,
    Right,
};
Optional<JustifyContent> keyword_to_justify_content(Keyword);
Keyword to_keyword(JustifyContent);
StringView to_string(JustifyContent);

enum class JustifyItems : u8 {
    Baseline,
    Center,
    End,
    FlexEnd,
    FlexStart,
    Legacy,
    Normal,
    Safe,
    SelfEnd,
    SelfStart,
    Start,
    Stretch,
    Unsafe,
    Left,
    Right,
};
Optional<JustifyItems> keyword_to_justify_items(Keyword);
Keyword to_keyword(JustifyItems);
StringView to_string(JustifyItems);

enum class JustifySelf : u8 {
    Auto,
    Baseline,
    Center,
    End,
    FlexEnd,
    FlexStart,
    Normal,
    Safe,
    SelfEnd,
    SelfStart,
    Start,
    Stretch,
    Unsafe,
    Left,
    Right,
};
Optional<JustifySelf> keyword_to_justify_self(Keyword);
Keyword to_keyword(JustifySelf);
StringView to_string(JustifySelf);

enum class LineStyle : u8 {
    None,
    Hidden,
    Dotted,
    Dashed,
    Solid,
    Double,
    Groove,
    Ridge,
    Inset,
    Outset,
};
Optional<LineStyle> keyword_to_line_style(Keyword);
Keyword to_keyword(LineStyle);
StringView to_string(LineStyle);

enum class LineWidth : u8 {
    Thin,
    Medium,
    Thick,
};
Optional<LineWidth> keyword_to_line_width(Keyword);
Keyword to_keyword(LineWidth);
StringView to_string(LineWidth);

enum class ListStylePosition : u8 {
    Inside,
    Outside,
};
Optional<ListStylePosition> keyword_to_list_style_position(Keyword);
Keyword to_keyword(ListStylePosition);
StringView to_string(ListStylePosition);

enum class MaskType : u8 {
    Luminance,
    Alpha,
};
Optional<MaskType> keyword_to_mask_type(Keyword);
Keyword to_keyword(MaskType);
StringView to_string(MaskType);

enum class MaskingMode : u8 {
    Alpha,
    Luminance,
    MatchSource,
};
Optional<MaskingMode> keyword_to_masking_mode(Keyword);
Keyword to_keyword(MaskingMode);
StringView to_string(MaskingMode);

enum class MathShift : u8 {
    Normal,
    Compact,
};
Optional<MathShift> keyword_to_math_shift(Keyword);
Keyword to_keyword(MathShift);
StringView to_string(MathShift);

enum class MathStyle : u8 {
    Normal,
    Compact,
};
Optional<MathStyle> keyword_to_math_style(Keyword);
Keyword to_keyword(MathStyle);
StringView to_string(MathStyle);

enum class MixBlendMode : u8 {
    Normal,
    Multiply,
    Screen,
    Overlay,
    Darken,
    Lighten,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
    PlusDarker,
    PlusLighter,
};
Optional<MixBlendMode> keyword_to_mix_blend_mode(Keyword);
Keyword to_keyword(MixBlendMode);
StringView to_string(MixBlendMode);

enum class NumericFigureValue : u8 {
    LiningNums,
    OldstyleNums,
};
Optional<NumericFigureValue> keyword_to_numeric_figure_value(Keyword);
Keyword to_keyword(NumericFigureValue);
StringView to_string(NumericFigureValue);

enum class NumericFractionValue : u8 {
    DiagonalFractions,
    StackedFractions,
};
Optional<NumericFractionValue> keyword_to_numeric_fraction_value(Keyword);
Keyword to_keyword(NumericFractionValue);
StringView to_string(NumericFractionValue);

enum class NumericSpacingValue : u8 {
    ProportionalNums,
    TabularNums,
};
Optional<NumericSpacingValue> keyword_to_numeric_spacing_value(Keyword);
Keyword to_keyword(NumericSpacingValue);
StringView to_string(NumericSpacingValue);

enum class ObjectFit : u8 {
    Fill,
    Contain,
    Cover,
    None,
    ScaleDown,
};
Optional<ObjectFit> keyword_to_object_fit(Keyword);
Keyword to_keyword(ObjectFit);
StringView to_string(ObjectFit);

enum class OutlineStyle : u8 {
    Auto,
    None,
    Dotted,
    Dashed,
    Solid,
    Double,
    Groove,
    Ridge,
    Inset,
    Outset,
};
Optional<OutlineStyle> keyword_to_outline_style(Keyword);
Keyword to_keyword(OutlineStyle);
StringView to_string(OutlineStyle);

enum class Overflow : u8 {
    Auto,
    Clip,
    Hidden,
    Scroll,
    Visible,
};
Optional<Overflow> keyword_to_overflow(Keyword);
Keyword to_keyword(Overflow);
StringView to_string(Overflow);

enum class PageSize : u8 {
    A5,
    A4,
    A3,
    B5,
    B4,
    JisB5,
    JisB4,
    Letter,
    Legal,
    Ledger,
};
Optional<PageSize> keyword_to_page_size(Keyword);
Keyword to_keyword(PageSize);
StringView to_string(PageSize);

enum class PaintOrder : u8 {
    Fill,
    Stroke,
    Markers,
};
Optional<PaintOrder> keyword_to_paint_order(Keyword);
Keyword to_keyword(PaintOrder);
StringView to_string(PaintOrder);

enum class PointerEvents : u8 {
    Auto,
    All,
    None,
};
Optional<PointerEvents> keyword_to_pointer_events(Keyword);
Keyword to_keyword(PointerEvents);
StringView to_string(PointerEvents);

enum class PolarColorSpace : u8 {
    Hsl,
    Hwb,
    Lch,
    Oklch,
};
Optional<PolarColorSpace> keyword_to_polar_color_space(Keyword);
Keyword to_keyword(PolarColorSpace);
StringView to_string(PolarColorSpace);

enum class PositionArea : u8 {
    Left,
    Center,
    Right,
    SpanLeft,
    SpanRight,
    XStart,
    XEnd,
    SpanXStart,
    SpanXEnd,
    SelfXStart,
    SelfXEnd,
    SpanSelfXStart,
    SpanSelfXEnd,
    SpanAll,
    Top,
    Bottom,
    SpanTop,
    SpanBottom,
    YStart,
    YEnd,
    SpanYStart,
    SpanYEnd,
    SelfYStart,
    SelfYEnd,
    SpanSelfYStart,
    SpanSelfYEnd,
    BlockStart,
    BlockEnd,
    SpanBlockStart,
    SpanBlockEnd,
    InlineStart,
    InlineEnd,
    SpanInlineStart,
    SpanInlineEnd,
    SelfBlockStart,
    SelfBlockEnd,
    SpanSelfBlockStart,
    SpanSelfBlockEnd,
    SelfInlineStart,
    SelfInlineEnd,
    SpanSelfInlineStart,
    SpanSelfInlineEnd,
    Start,
    End,
    SpanStart,
    SpanEnd,
    SelfStart,
    SelfEnd,
    SpanSelfStart,
    SpanSelfEnd,
};
Optional<PositionArea> keyword_to_position_area(Keyword);
Keyword to_keyword(PositionArea);
StringView to_string(PositionArea);

enum class PositionEdge : u8 {
    Center,
    Left,
    Right,
    Top,
    Bottom,
};
Optional<PositionEdge> keyword_to_position_edge(Keyword);
Keyword to_keyword(PositionEdge);
StringView to_string(PositionEdge);

enum class Positioning : u8 {
    Absolute,
    Fixed,
    Relative,
    Static,
    Sticky,
};
Optional<Positioning> keyword_to_positioning(Keyword);
Keyword to_keyword(Positioning);
StringView to_string(Positioning);

enum class RadialExtent : u8 {
    ClosestCorner,
    ClosestSide,
    FarthestCorner,
    FarthestSide,
};
Optional<RadialExtent> keyword_to_radial_extent(Keyword);
Keyword to_keyword(RadialExtent);
StringView to_string(RadialExtent);

enum class ReferrerPolicyModifierValue : u8 {
    NoReferrer,
    NoReferrerWhenDowngrade,
    SameOrigin,
    Origin,
    StrictOrigin,
    OriginWhenCrossOrigin,
    StrictOriginWhenCrossOrigin,
    UnsafeUrl,
};
Optional<ReferrerPolicyModifierValue> keyword_to_referrer_policy_modifier_value(Keyword);
Keyword to_keyword(ReferrerPolicyModifierValue);
StringView to_string(ReferrerPolicyModifierValue);

enum class RelativeSize : u8 {
    Smaller,
    Larger,
};
Optional<RelativeSize> keyword_to_relative_size(Keyword);
Keyword to_keyword(RelativeSize);
StringView to_string(RelativeSize);

enum class Repetition : u8 {
    NoRepeat,
    Repeat,
    Round,
    Space,
};
Optional<Repetition> keyword_to_repetition(Keyword);
Keyword to_keyword(Repetition);
StringView to_string(Repetition);

enum class Resize : u8 {
    None,
    Both,
    Horizontal,
    Vertical,
    Inline,
    Block,
};
Optional<Resize> keyword_to_resize(Keyword);
Keyword to_keyword(Resize);
StringView to_string(Resize);

enum class RoundingStrategy : u8 {
    Down,
    Nearest,
    ToZero,
    Up,
};
Optional<RoundingStrategy> keyword_to_rounding_strategy(Keyword);
Keyword to_keyword(RoundingStrategy);
StringView to_string(RoundingStrategy);

enum class ScrollbarGutter : u8 {
    Auto,
    Stable,
    BothEdges,
};
Optional<ScrollbarGutter> keyword_to_scrollbar_gutter(Keyword);
Keyword to_keyword(ScrollbarGutter);
StringView to_string(ScrollbarGutter);

enum class ScrollbarWidth : u8 {
    Auto,
    Thin,
    None,
};
Optional<ScrollbarWidth> keyword_to_scrollbar_width(Keyword);
Keyword to_keyword(ScrollbarWidth);
StringView to_string(ScrollbarWidth);

enum class Scroller : u8 {
    Root,
    Nearest,
    Self,
};
Optional<Scroller> keyword_to_scroller(Keyword);
Keyword to_keyword(Scroller);
StringView to_string(Scroller);

enum class ShapeBox : u8 {
    ContentBox,
    PaddingBox,
    BorderBox,
    MarginBox,
};
Optional<ShapeBox> keyword_to_shape_box(Keyword);
Keyword to_keyword(ShapeBox);
StringView to_string(ShapeBox);

enum class ShapeRendering : u8 {
    Auto,
    Optimizespeed,
    Crispedges,
    Geometricprecision,
};
Optional<ShapeRendering> keyword_to_shape_rendering(Keyword);
Keyword to_keyword(ShapeRendering);
StringView to_string(ShapeRendering);

enum class StepPosition : u8 {
    JumpStart,
    JumpEnd,
    JumpNone,
    JumpBoth,
    Start,
    End,
};
Optional<StepPosition> keyword_to_step_position(Keyword);
Keyword to_keyword(StepPosition);
StringView to_string(StepPosition);

enum class StrokeLinecap : u8 {
    Butt,
    Square,
    Round,
};
Optional<StrokeLinecap> keyword_to_stroke_linecap(Keyword);
Keyword to_keyword(StrokeLinecap);
StringView to_string(StrokeLinecap);

enum class StrokeLinejoin : u8 {
    Miter,
    Round,
    Bevel,
};
Optional<StrokeLinejoin> keyword_to_stroke_linejoin(Keyword);
Keyword to_keyword(StrokeLinejoin);
StringView to_string(StrokeLinejoin);

enum class SymbolsType : u8 {
    Cyclic,
    Numeric,
    Alphabetic,
    Symbolic,
    Fixed,
};
Optional<SymbolsType> keyword_to_symbols_type(Keyword);
Keyword to_keyword(SymbolsType);
StringView to_string(SymbolsType);

enum class TableLayout : u8 {
    Auto,
    Fixed,
};
Optional<TableLayout> keyword_to_table_layout(Keyword);
Keyword to_keyword(TableLayout);
StringView to_string(TableLayout);

enum class TextAlign : u8 {
    Center,
    Justify,
    Start,
    End,
    Left,
    Right,
    MatchParent,
    LibwebCenter,
    LibwebInheritOrCenter,
    LibwebLeft,
    LibwebRight,
};
Optional<TextAlign> keyword_to_text_align(Keyword);
Keyword to_keyword(TextAlign);
StringView to_string(TextAlign);

enum class TextAnchor : u8 {
    Start,
    Middle,
    End,
};
Optional<TextAnchor> keyword_to_text_anchor(Keyword);
Keyword to_keyword(TextAnchor);
StringView to_string(TextAnchor);

enum class TextDecorationLine : u8 {
    None,
    Underline,
    Overline,
    LineThrough,
    Blink,
    SpellingError,
    GrammarError,
};
Optional<TextDecorationLine> keyword_to_text_decoration_line(Keyword);
Keyword to_keyword(TextDecorationLine);
StringView to_string(TextDecorationLine);

enum class TextDecorationStyle : u8 {
    Dashed,
    Dotted,
    Double,
    Solid,
    Wavy,
};
Optional<TextDecorationStyle> keyword_to_text_decoration_style(Keyword);
Keyword to_keyword(TextDecorationStyle);
StringView to_string(TextDecorationStyle);

enum class TextJustify : u8 {
    Auto,
    None,
    InterWord,
    InterCharacter,
};
Optional<TextJustify> keyword_to_text_justify(Keyword);
Keyword to_keyword(TextJustify);
StringView to_string(TextJustify);

enum class TextOverflow : u8 {
    Clip,
    Ellipsis,
};
Optional<TextOverflow> keyword_to_text_overflow(Keyword);
Keyword to_keyword(TextOverflow);
StringView to_string(TextOverflow);

enum class TextRendering : u8 {
    Auto,
    Optimizespeed,
    Optimizelegibility,
    Geometricprecision,
};
Optional<TextRendering> keyword_to_text_rendering(Keyword);
Keyword to_keyword(TextRendering);
StringView to_string(TextRendering);

enum class TextTransform : u8 {
    Capitalize,
    FullSizeKana,
    FullWidth,
    Lowercase,
    MathAuto,
    None,
    Uppercase,
};
Optional<TextTransform> keyword_to_text_transform(Keyword);
Keyword to_keyword(TextTransform);
StringView to_string(TextTransform);

enum class TextUnderlinePositionHorizontal : u8 {
    Auto,
    FromFont,
    Under,
};
Optional<TextUnderlinePositionHorizontal> keyword_to_text_underline_position_horizontal(Keyword);
Keyword to_keyword(TextUnderlinePositionHorizontal);
StringView to_string(TextUnderlinePositionHorizontal);

enum class TextUnderlinePositionVertical : u8 {
    Auto,
    Left,
    Right,
};
Optional<TextUnderlinePositionVertical> keyword_to_text_underline_position_vertical(Keyword);
Keyword to_keyword(TextUnderlinePositionVertical);
StringView to_string(TextUnderlinePositionVertical);

enum class TextWrapMode : u8 {
    Wrap,
    Nowrap,
};
Optional<TextWrapMode> keyword_to_text_wrap_mode(Keyword);
Keyword to_keyword(TextWrapMode);
StringView to_string(TextWrapMode);

enum class TextWrapStyle : u8 {
    Auto,
    Balance,
    Stable,
    Pretty,
};
Optional<TextWrapStyle> keyword_to_text_wrap_style(Keyword);
Keyword to_keyword(TextWrapStyle);
StringView to_string(TextWrapStyle);

enum class TouchAction : u8 {
    Auto,
    Manipulation,
    None,
    PanDown,
    PanLeft,
    PanRight,
    PanUp,
    PanX,
    PanY,
};
Optional<TouchAction> keyword_to_touch_action(Keyword);
Keyword to_keyword(TouchAction);
StringView to_string(TouchAction);

enum class TransformBox : u8 {
    ContentBox,
    BorderBox,
    FillBox,
    StrokeBox,
    ViewBox,
};
Optional<TransformBox> keyword_to_transform_box(Keyword);
Keyword to_keyword(TransformBox);
StringView to_string(TransformBox);

enum class TransformStyle : u8 {
    Flat,
    Preserve3d,
};
Optional<TransformStyle> keyword_to_transform_style(Keyword);
Keyword to_keyword(TransformStyle);
StringView to_string(TransformStyle);

enum class TransitionBehavior : u8 {
    Normal,
    AllowDiscrete,
};
Optional<TransitionBehavior> keyword_to_transition_behavior(Keyword);
Keyword to_keyword(TransitionBehavior);
StringView to_string(TransitionBehavior);

enum class TryOrder : u8 {
    MostWidth,
    MostHeight,
    MostBlockSize,
    MostInlineSize,
};
Optional<TryOrder> keyword_to_try_order(Keyword);
Keyword to_keyword(TryOrder);
StringView to_string(TryOrder);

enum class TryTactic : u8 {
    FlipBlock,
    FlipInline,
    FlipStart,
};
Optional<TryTactic> keyword_to_try_tactic(Keyword);
Keyword to_keyword(TryTactic);
StringView to_string(TryTactic);

enum class UnicodeBidi : u8 {
    BidiOverride,
    Embed,
    Isolate,
    IsolateOverride,
    Normal,
    Plaintext,
};
Optional<UnicodeBidi> keyword_to_unicode_bidi(Keyword);
Keyword to_keyword(UnicodeBidi);
StringView to_string(UnicodeBidi);

enum class UserSelect : u8 {
    All,
    Auto,
    Contain,
    None,
    Text,
};
Optional<UserSelect> keyword_to_user_select(Keyword);
Keyword to_keyword(UserSelect);
StringView to_string(UserSelect);

enum class VerticalAlign : u8 {
    Baseline,
    Bottom,
    Middle,
    Sub,
    Super,
    TextBottom,
    TextTop,
    Top,
};
Optional<VerticalAlign> keyword_to_vertical_align(Keyword);
Keyword to_keyword(VerticalAlign);
StringView to_string(VerticalAlign);

enum class Visibility : u8 {
    Collapse,
    Hidden,
    Visible,
};
Optional<Visibility> keyword_to_visibility(Keyword);
Keyword to_keyword(Visibility);
StringView to_string(Visibility);

enum class WhiteSpace : u8 {
    Normal,
    Pre,
    PreLine,
    PreWrap,
};
Optional<WhiteSpace> keyword_to_white_space(Keyword);
Keyword to_keyword(WhiteSpace);
StringView to_string(WhiteSpace);

enum class WhiteSpaceCollapse : u8 {
    Collapse,
    Discard,
    Preserve,
    PreserveBreaks,
    PreserveSpaces,
    BreakSpaces,
};
Optional<WhiteSpaceCollapse> keyword_to_white_space_collapse(Keyword);
Keyword to_keyword(WhiteSpaceCollapse);
StringView to_string(WhiteSpaceCollapse);

enum class WordBreak : u8 {
    Normal,
    KeepAll,
    BreakAll,
    BreakWord,
};
Optional<WordBreak> keyword_to_word_break(Keyword);
Keyword to_keyword(WordBreak);
StringView to_string(WordBreak);

enum class WritingMode : u8 {
    HorizontalTb,
    VerticalRl,
    VerticalLr,
    SidewaysRl,
    SidewaysLr,
};
Optional<WritingMode> keyword_to_writing_mode(Keyword);
Keyword to_keyword(WritingMode);
StringView to_string(WritingMode);

}
