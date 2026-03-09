
#pragma once

#include <AK/StringView.h>
#include <AK/Traits.h>
#include <LibWeb/Export.h>

namespace Web::CSS {

enum class Keyword : u16 {
    Invalid,

    NegativeInfinity,

    LibwebButtonfacedisabled,

    LibwebButtonfacehover,

    LibwebCenter,

    LibwebInheritOrCenter,

    LibwebLeft,

    LibwebLink,

    LibwebPaletteActiveLink,

    LibwebPaletteActiveWindowBorder1,

    LibwebPaletteActiveWindowBorder2,

    LibwebPaletteActiveWindowTitle,

    LibwebPaletteBase,

    LibwebPaletteBaseText,

    LibwebPaletteButton,

    LibwebPaletteButtonText,

    LibwebPaletteDesktopBackground,

    LibwebPaletteFocusOutline,

    LibwebPaletteHighlightWindowBorder1,

    LibwebPaletteHighlightWindowBorder2,

    LibwebPaletteHighlightWindowTitle,

    LibwebPaletteHoverHighlight,

    LibwebPaletteInactiveSelection,

    LibwebPaletteInactiveSelectionText,

    LibwebPaletteInactiveWindowBorder1,

    LibwebPaletteInactiveWindowBorder2,

    LibwebPaletteInactiveWindowTitle,

    LibwebPaletteLink,

    LibwebPaletteMenuBase,

    LibwebPaletteMenuBaseText,

    LibwebPaletteMenuSelection,

    LibwebPaletteMenuSelectionText,

    LibwebPaletteMenuStripe,

    LibwebPaletteMovingWindowBorder1,

    LibwebPaletteMovingWindowBorder2,

    LibwebPaletteMovingWindowTitle,

    LibwebPaletteRubberBandBorder,

    LibwebPaletteRubberBandFill,

    LibwebPaletteRuler,

    LibwebPaletteRulerActiveText,

    LibwebPaletteRulerBorder,

    LibwebPaletteRulerInactiveText,

    LibwebPaletteSelection,

    LibwebPaletteSelectionText,

    LibwebPaletteSyntaxComment,

    LibwebPaletteSyntaxControlKeyword,

    LibwebPaletteSyntaxIdentifier,

    LibwebPaletteSyntaxKeyword,

    LibwebPaletteSyntaxNumber,

    LibwebPaletteSyntaxOperator,

    LibwebPaletteSyntaxPreprocessorStatement,

    LibwebPaletteSyntaxPreprocessorValue,

    LibwebPaletteSyntaxPunctuation,

    LibwebPaletteSyntaxString,

    LibwebPaletteSyntaxType,

    LibwebPaletteTextCursor,

    LibwebPaletteThreedHighlight,

    LibwebPaletteThreedShadow1,

    LibwebPaletteThreedShadow2,

    LibwebPaletteVisitedLink,

    LibwebPaletteWindow,

    LibwebPaletteWindowText,

    LibwebRight,

    A3,

    A4,

    A5,

    Absolute,

    Accentcolor,

    Accentcolortext,

    Accumulate,

    Active,

    Activeborder,

    Activecaption,

    Activetext,

    Add,

    Additive,

    Adlam,

    Afar,

    Agaw,

    Alias,

    All,

    AllPetiteCaps,

    AllScroll,

    AllSmallCaps,

    AllowDiscrete,

    Alpha,

    Alphabetic,

    Alternate,

    AlternateReverse,

    Always,

    AnchorsValid,

    AnchorsVisible,

    AncientTamil,

    Anonymous,

    Anywhere,

    Appworkspace,

    ArabicAbjad,

    ArabicIndic,

    Ari,

    Armenian,

    Auto,

    AutoAdd,

    Avar2,

    B4,

    B5,

    Back,

    Background,

    Backwards,

    Balance,

    Balinese,

    Bamum,

    Bangla,

    Baseline,

    Bengali,

    Bevel,

    BidiOverride,

    Binary,

    Blin,

    Blink,

    Block,

    BlockEnd,

    BlockStart,

    Bodo,

    Bold,

    Bolder,

    BorderBox,

    Both,

    BothEdges,

    Bottom,

    BreakAll,

    BreakSpaces,

    BreakWord,

    Browser,

    Bullets,

    Butt,

    Button,

    Buttonborder,

    Buttonface,

    Buttonhighlight,

    Buttonshadow,

    Buttontext,

    Cambodian,

    Canvas,

    Canvastext,

    Capitalize,

    Captiontext,

    Cell,

    Center,

    Central,

    Checkbox,

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

    Clip,

    CloseQuote,

    ClosestCorner,

    ClosestSide,

    Coarse,

    ColResize,

    Collapse,

    Color,

    ColorBurn,

    ColorCbdt,

    ColorColrv0,

    ColorColrv1,

    ColorDodge,

    ColorSbix,

    ColorSvg,

    Column,

    ColumnReverse,

    CommonLigatures,

    Compact,

    Condensed,

    Contain,

    Content,

    ContentBox,

    Contents,

    ContextMenu,

    Contextual,

    Copy,

    Cover,

    CrispEdges,

    Crispedges,

    Crop,

    Cross,

    Crosshair,

    Currentcolor,

    Cursive,

    Custom,

    Cyclic,

    Dark,

    Darken,

    Dashed,

    Decimal,

    DecimalLeadingZero,

    Decreasing,

    Default,

    Devanagari,

    DiagonalFractions,

    Difference,

    Disc,

    Discard,

    DiscardBefore,

    DiscardAfter,

    DiscardInner,

    DisclosureClosed,

    DisclosureOpen,

    DiscretionaryLigatures,

    Distribute,

    Dizi,

    Dogri,

    DottedDecimal,

    Dotted,

    DoubleCircledDecimal,

    Double,

    Down,

    E,

    EResize,

    EachLine,

    Ease,

    EaseIn,

    EaseInOut,

    EaseOut,

    ElementShared,

    Ellipsis,

    Embed,

    Emoji,

    Enabled,

    End,

    EthiopicHalehameAm,

    EthiopicHalehameTiEr,

    EthiopicHalehameTiEt,

    EthiopicHalehame,

    EthiopicNumeric,

    Evenodd,

    EwResize,

    Exclude,

    Exclusion,

    Expanded,

    Extends,

    ExtraCondensed,

    ExtraExpanded,

    Fallback,

    False,

    Fantasy,

    FarthestCorner,

    FarthestSide,

    Fast,

    FeaturesAat,

    FeaturesGraphite,

    FeaturesOpentype,

    Field,

    Fieldtext,

    Fill,

    FillBox,

    FilledCircledDecimal,

    Fine,

    Fixed,

    Flat,

    Flex,

    FlexEnd,

    FlexStart,

    FlipBlock,

    FlipInline,

    FlipStart,

    Flow,

    FlowRoot,

    Forwards,

    FromFont,

    FullSizeKana,

    FullWidth,

    FullwidthDecimal,

    FullwidthLowerAlpha,

    FullwidthUpperAlpha,

    FullwidthUpperRoman,

    Fullscreen,

    Gedeo,

    Geometricprecision,

    Georgian,

    Grab,

    Grabbing,

    GrammarError,

    Graytext,

    GreekLowerAncient,

    GreekLowerModern,

    GreekUpperAncient,

    GreekUpperModern,

    Grid,

    Groove,

    GujaratiAlpha,

    Gujarati,

    Gumuz,

    Gurmukhi,

    Hadiyaa,

    Hanging,

    HangulConsonant,

    Hangul,

    HanifiRohingya,

    Harari,

    HardLight,

    Hebrew,

    Height,

    Help,

    Hide,

    Hidden,

    High,

    HighQuality,

    Highlight,

    Highlighttext,

    Hindi,

    HiraganaIroha,

    Hiragana,

    HistoricalForms,

    HistoricalLigatures,

    Horizontal,

    HorizontalTb,

    Hover,

    Hsl,

    Hue,

    Hwb,

    Inactiveborder,

    Inactivecaption,

    Inactivecaptiontext,

    Increasing,

    Ideographic,

    Incremental,

    Infinite,

    Infinity,

    Infobackground,

    Infotext,

    Inherit,

    Initial,

    InitialOnly,

    Inline,

    InlineBlock,

    InlineEnd,

    InlineFlex,

    InlineGrid,

    InlineSize,

    InlineStart,

    InlineTable,

    Inset,

    Inside,

    InterCharacter,

    InterWord,

    Interlace,

    Intersect,

    Invert,

    Inverted,

    Isolate,

    IsolateOverride,

    Italic,

    JapaneseFormal,

    JapaneseInformal,

    Javanese,

    JisB4,

    JisB5,

    Jis04,

    Jis78,

    Jis83,

    Jis90,

    JumpBoth,

    JumpEnd,

    JumpNone,

    JumpStart,

    Justify,

    Kaffa,

    KannadaAlpha,

    Kannada,

    Kashmiri,

    KatakanaIroha,

    Katakana,

    KayahLi,

    Kebena,

    KeepAll,

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

    Landscape,

    LannaHora,

    LannaTham,

    Lao,

    Large,

    Larger,

    Layout,

    Lch,

    Ledger,

    Left,

    Legacy,

    Legal,

    Lepcha,

    Less,

    Letter,

    Light,

    Lighten,

    Lighter,

    Limbu,

    LineThrough,

    Linear,

    Linearrgb,

    LiningNums,

    Linktext,

    ListItem,

    Listbox,

    Local,

    Longer,

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

    Lowercase,

    Ltr,

    Luminance,

    Luminosity,

    MaghrebiAbjad,

    Maithili,

    MalayalamAlpha,

    Malayalam,

    Manipulation,

    Manipuri,

    Marathi,

    MarginBox,

    Mark,

    Markers,

    Marktext,

    MatchParent,

    MatchSource,

    Math,

    MathAuto,

    Mathematical,

    MaxContent,

    Medium,

    Meen,

    Meetei,

    Menu,

    Menulist,

    MenulistButton,

    Menutext,

    Meter,

    Middle,

    MinContent,

    MinimalUi,

    Miter,

    Mongolian,

    Monospace,

    More,

    MostBlockSize,

    MostHeight,

    MostInlineSize,

    MostWidth,

    Move,

    Mro,

    Multiply,

    Myanmar,

    NResize,

    NagMundari,

    Nan,

    NeResize,

    Nearest,

    NeswResize,

    NewBase60,

    Newa,

    NkoCardinal,

    NoClip,

    NoCloseQuote,

    NoCommonLigatures,

    NoContextual,

    NoDiscretionaryLigatures,

    NoDrop,

    NoHistoricalLigatures,

    NoOpenQuote,

    NoOverflow,

    NoPreference,

    NoReferrer,

    NoReferrerWhenDowngrade,

    NoRepeat,

    None,

    Nonzero,

    Normal,

    NotAllowed,

    Notch,

    Nowrap,

    NsResize,

    Numbers,

    Numeric,

    NwResize,

    NwseResize,

    Oblique,

    Octal,

    Odia,

    Off,

    Oklch,

    OlChiki,

    OldstyleNums,

    On,

    Opaque,

    OpenQuote,

    Optimizelegibility,

    Optimizequality,

    Optimizespeed,

    Optional,

    Ordinal,

    Origin,

    OriginWhenCrossOrigin,

    Oriya,

    Oromo,

    Outset,

    Outside,

    Overlay,

    Overline,

    P3,

    PaddingBox,

    Paged,

    Paint,

    Palettes,

    PanDown,

    PanLeft,

    PanRight,

    PanUp,

    PanX,

    PanY,

    ParenthesizedDecimal,

    ParenthesizedHangulConsonant,

    ParenthesizedHangulSyllable,

    ParenthesizedIdeograph,

    ParenthesizedLowerLatin,

    Paused,

    PersianAbjad,

    PersianAlphabetic,

    Persian,

    PetiteCaps,

    Pi,

    PictureInPicture,

    Pixelated,

    Plaintext,

    PlusDarker,

    PlusLighter,

    Pointer,

    Portrait,

    Pre,

    PreLine,

    PreWrap,

    Preserve,

    Preserve3d,

    PreserveBreaks,

    PreserveSpaces,

    Pretty,

    Progress,

    ProgressBar,

    Progressive,

    ProportionalNums,

    ProportionalWidth,

    Punjabi,

    PushButton,

    Radio,

    Rec2020,

    Reduce,

    Relative,

    Repeat,

    RepeatX,

    RepeatY,

    Replace,

    Reverse,

    Revert,

    RevertLayer,

    Ridge,

    Right,

    Root,

    RotateLeft,

    RotateRight,

    Round,

    Row,

    RowResize,

    RowReverse,

    Rtl,

    Ruby,

    RubyBase,

    RubyBaseContainer,

    RubyText,

    RubyTextContainer,

    RunIn,

    Running,

    SResize,

    Safe,

    Saho,

    SameOrigin,

    SansSerif,

    Sanskrit,

    Santali,

    Saturation,

    ScaleDown,

    Scoop,

    Screen,

    Scroll,

    ScrollPosition,

    ScrollState,

    Scrollbar,

    SeResize,

    Searchfield,

    Selecteditem,

    Selecteditemtext,

    Self,

    SelfBlock,

    SelfBlockEnd,

    SelfBlockStart,

    SelfEnd,

    SelfInline,

    SelfInlineEnd,

    SelfInlineStart,

    SelfXEnd,

    SelfXStart,

    SelfYEnd,

    SelfYStart,

    SelfStart,

    SemiCondensed,

    SemiExpanded,

    Separate,

    Serif,

    Shan,

    Shorter,

    Show,

    Sidama,

    SidewaysLr,

    SidewaysRl,

    Silti,

    SimpChineseFormal,

    SimpChineseInformal,

    SimpleLowerRoman,

    SimpleUpperRoman,

    Simplified,

    Sindhi,

    Size,

    SlashedZero,

    SliderHorizontal,

    Slow,

    Small,

    SmallCaps,

    Smaller,

    Smooth,

    SoftLight,

    Solid,

    Space,

    SpaceAround,

    SpaceBetween,

    SpaceEvenly,

    SpanAll,

    SpanBlockEnd,

    SpanBlockStart,

    SpanBottom,

    SpanEnd,

    SpanInlineEnd,

    SpanInlineStart,

    SpanLeft,

    SpanRight,

    SpanSelfBlockEnd,

    SpanSelfBlockStart,

    SpanSelfEnd,

    SpanSelfInlineEnd,

    SpanSelfInlineStart,

    SpanSelfStart,

    SpanSelfXEnd,

    SpanSelfXStart,

    SpanSelfYEnd,

    SpanSelfYStart,

    SpanStart,

    SpanTop,

    SpanXEnd,

    SpanXStart,

    SpanYEnd,

    SpanYStart,

    SpellingError,

    SpellOut,

    Square,

    SquareButton,

    Squircle,

    Srgb,

    Stable,

    StackedFractions,

    Standalone,

    Standard,

    Start,

    Static,

    Sticky,

    Stretch,

    Strict,

    StrictOrigin,

    StrictOriginWhenCrossOrigin,

    Stroke,

    StrokeBox,

    Style,

    Sub,

    Subtract,

    Subtractive,

    Sundanese,

    SuperDecimal,

    Super,

    SwResize,

    Swap,

    Symbolic,

    Table,

    TableCaption,

    TableCell,

    TableColumn,

    TableColumnGroup,

    TableFooterGroup,

    TableHeaderGroup,

    TableRow,

    TableRowGroup,

    TabularNums,

    TaiLue,

    TallyMark,

    Tamil,

    TeluguAlpha,

    Telugu,

    Text,

    TextBottom,

    TextTop,

    Textarea,

    Textfield,

    ThaiAlpha,

    Thai,

    Thick,

    Thin,

    Threeddarkshadow,

    Threedface,

    Threedhighlight,

    Threedlightshadow,

    Threedshadow,

    Tibetan,

    Tigre,

    TitlingCaps,

    ToZero,

    Top,

    TradChineseFormal,

    TradChineseInformal,

    Traditional,

    True,

    UiMonospace,

    UiRounded,

    UiSansSerif,

    UiSerif,

    UltraCondensed,

    UltraExpanded,

    Under,

    Underline,

    Unicase,

    Unicode,

    Unsafe,

    UnsafeUrl,

    Unset,

    Up,

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

    Uppercase,

    Upright,

    UrduAbjad,

    UrduAlphabetic,

    Urdu,

    UseCredentials,

    Variations,

    Vertical,

    VerticalLr,

    VerticalRl,

    VerticalText,

    ViewBox,

    Visible,

    Visitedtext,

    WResize,

    Wait,

    WarangCiti,

    Wavy,

    Width,

    Window,

    Windowframe,

    Windowtext,

    Wolaita,

    Words,

    Wrap,

    WrapReverse,

    X,

    XEnd,

    XLarge,

    XStart,

    XSmall,

    XxLarge,

    XxSmall,

    XxxLarge,

    Y,

    YEnd,

    YStart,

    Yemsa,

    Zhuyin,

    ZoomIn,

    ZoomOut,

};

WEB_API Optional<Keyword> keyword_from_string(StringView);
StringView string_from_keyword(Keyword);

// https://www.w3.org/TR/css-values-4/#common-keywords
// https://drafts.csswg.org/css-cascade-4/#valdef-all-revert
inline bool is_css_wide_keyword(StringView name)
{
    return name.equals_ignoring_ascii_case("inherit"sv)
        || name.equals_ignoring_ascii_case("initial"sv)
        || name.equals_ignoring_ascii_case("revert"sv)
        || name.equals_ignoring_ascii_case("revert-layer"sv)
        || name.equals_ignoring_ascii_case("unset"sv);
}

}

