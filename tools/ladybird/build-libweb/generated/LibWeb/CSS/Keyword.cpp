
#include <AK/Assertions.h>
#include <AK/HashMap.h>
#include <LibWeb/CSS/Keyword.h>

namespace Web::CSS {

HashMap<StringView, Keyword, AK::CaseInsensitiveASCIIStringViewTraits> g_stringview_to_keyword_map {

    {"-infinity"sv, Keyword::NegativeInfinity},

    {"-libweb-buttonfacedisabled"sv, Keyword::LibwebButtonfacedisabled},

    {"-libweb-buttonfacehover"sv, Keyword::LibwebButtonfacehover},

    {"-libweb-center"sv, Keyword::LibwebCenter},

    {"-libweb-inherit-or-center"sv, Keyword::LibwebInheritOrCenter},

    {"-libweb-left"sv, Keyword::LibwebLeft},

    {"-libweb-link"sv, Keyword::LibwebLink},

    {"-libweb-palette-active-link"sv, Keyword::LibwebPaletteActiveLink},

    {"-libweb-palette-active-window-border1"sv, Keyword::LibwebPaletteActiveWindowBorder1},

    {"-libweb-palette-active-window-border2"sv, Keyword::LibwebPaletteActiveWindowBorder2},

    {"-libweb-palette-active-window-title"sv, Keyword::LibwebPaletteActiveWindowTitle},

    {"-libweb-palette-base"sv, Keyword::LibwebPaletteBase},

    {"-libweb-palette-base-text"sv, Keyword::LibwebPaletteBaseText},

    {"-libweb-palette-button"sv, Keyword::LibwebPaletteButton},

    {"-libweb-palette-button-text"sv, Keyword::LibwebPaletteButtonText},

    {"-libweb-palette-desktop-background"sv, Keyword::LibwebPaletteDesktopBackground},

    {"-libweb-palette-focus-outline"sv, Keyword::LibwebPaletteFocusOutline},

    {"-libweb-palette-highlight-window-border1"sv, Keyword::LibwebPaletteHighlightWindowBorder1},

    {"-libweb-palette-highlight-window-border2"sv, Keyword::LibwebPaletteHighlightWindowBorder2},

    {"-libweb-palette-highlight-window-title"sv, Keyword::LibwebPaletteHighlightWindowTitle},

    {"-libweb-palette-hover-highlight"sv, Keyword::LibwebPaletteHoverHighlight},

    {"-libweb-palette-inactive-selection"sv, Keyword::LibwebPaletteInactiveSelection},

    {"-libweb-palette-inactive-selection-text"sv, Keyword::LibwebPaletteInactiveSelectionText},

    {"-libweb-palette-inactive-window-border1"sv, Keyword::LibwebPaletteInactiveWindowBorder1},

    {"-libweb-palette-inactive-window-border2"sv, Keyword::LibwebPaletteInactiveWindowBorder2},

    {"-libweb-palette-inactive-window-title"sv, Keyword::LibwebPaletteInactiveWindowTitle},

    {"-libweb-palette-link"sv, Keyword::LibwebPaletteLink},

    {"-libweb-palette-menu-base"sv, Keyword::LibwebPaletteMenuBase},

    {"-libweb-palette-menu-base-text"sv, Keyword::LibwebPaletteMenuBaseText},

    {"-libweb-palette-menu-selection"sv, Keyword::LibwebPaletteMenuSelection},

    {"-libweb-palette-menu-selection-text"sv, Keyword::LibwebPaletteMenuSelectionText},

    {"-libweb-palette-menu-stripe"sv, Keyword::LibwebPaletteMenuStripe},

    {"-libweb-palette-moving-window-border1"sv, Keyword::LibwebPaletteMovingWindowBorder1},

    {"-libweb-palette-moving-window-border2"sv, Keyword::LibwebPaletteMovingWindowBorder2},

    {"-libweb-palette-moving-window-title"sv, Keyword::LibwebPaletteMovingWindowTitle},

    {"-libweb-palette-rubber-band-border"sv, Keyword::LibwebPaletteRubberBandBorder},

    {"-libweb-palette-rubber-band-fill"sv, Keyword::LibwebPaletteRubberBandFill},

    {"-libweb-palette-ruler"sv, Keyword::LibwebPaletteRuler},

    {"-libweb-palette-ruler-active-text"sv, Keyword::LibwebPaletteRulerActiveText},

    {"-libweb-palette-ruler-border"sv, Keyword::LibwebPaletteRulerBorder},

    {"-libweb-palette-ruler-inactive-text"sv, Keyword::LibwebPaletteRulerInactiveText},

    {"-libweb-palette-selection"sv, Keyword::LibwebPaletteSelection},

    {"-libweb-palette-selection-text"sv, Keyword::LibwebPaletteSelectionText},

    {"-libweb-palette-syntax-comment"sv, Keyword::LibwebPaletteSyntaxComment},

    {"-libweb-palette-syntax-control-keyword"sv, Keyword::LibwebPaletteSyntaxControlKeyword},

    {"-libweb-palette-syntax-identifier"sv, Keyword::LibwebPaletteSyntaxIdentifier},

    {"-libweb-palette-syntax-keyword"sv, Keyword::LibwebPaletteSyntaxKeyword},

    {"-libweb-palette-syntax-number"sv, Keyword::LibwebPaletteSyntaxNumber},

    {"-libweb-palette-syntax-operator"sv, Keyword::LibwebPaletteSyntaxOperator},

    {"-libweb-palette-syntax-preprocessor-statement"sv, Keyword::LibwebPaletteSyntaxPreprocessorStatement},

    {"-libweb-palette-syntax-preprocessor-value"sv, Keyword::LibwebPaletteSyntaxPreprocessorValue},

    {"-libweb-palette-syntax-punctuation"sv, Keyword::LibwebPaletteSyntaxPunctuation},

    {"-libweb-palette-syntax-string"sv, Keyword::LibwebPaletteSyntaxString},

    {"-libweb-palette-syntax-type"sv, Keyword::LibwebPaletteSyntaxType},

    {"-libweb-palette-text-cursor"sv, Keyword::LibwebPaletteTextCursor},

    {"-libweb-palette-threed-highlight"sv, Keyword::LibwebPaletteThreedHighlight},

    {"-libweb-palette-threed-shadow1"sv, Keyword::LibwebPaletteThreedShadow1},

    {"-libweb-palette-threed-shadow2"sv, Keyword::LibwebPaletteThreedShadow2},

    {"-libweb-palette-visited-link"sv, Keyword::LibwebPaletteVisitedLink},

    {"-libweb-palette-window"sv, Keyword::LibwebPaletteWindow},

    {"-libweb-palette-window-text"sv, Keyword::LibwebPaletteWindowText},

    {"-libweb-right"sv, Keyword::LibwebRight},

    {"a3"sv, Keyword::A3},

    {"a4"sv, Keyword::A4},

    {"a5"sv, Keyword::A5},

    {"absolute"sv, Keyword::Absolute},

    {"accentcolor"sv, Keyword::Accentcolor},

    {"accentcolortext"sv, Keyword::Accentcolortext},

    {"accumulate"sv, Keyword::Accumulate},

    {"active"sv, Keyword::Active},

    {"activeborder"sv, Keyword::Activeborder},

    {"activecaption"sv, Keyword::Activecaption},

    {"activetext"sv, Keyword::Activetext},

    {"add"sv, Keyword::Add},

    {"additive"sv, Keyword::Additive},

    {"adlam"sv, Keyword::Adlam},

    {"afar"sv, Keyword::Afar},

    {"agaw"sv, Keyword::Agaw},

    {"alias"sv, Keyword::Alias},

    {"all"sv, Keyword::All},

    {"all-petite-caps"sv, Keyword::AllPetiteCaps},

    {"all-scroll"sv, Keyword::AllScroll},

    {"all-small-caps"sv, Keyword::AllSmallCaps},

    {"allow-discrete"sv, Keyword::AllowDiscrete},

    {"alpha"sv, Keyword::Alpha},

    {"alphabetic"sv, Keyword::Alphabetic},

    {"alternate"sv, Keyword::Alternate},

    {"alternate-reverse"sv, Keyword::AlternateReverse},

    {"always"sv, Keyword::Always},

    {"anchors-valid"sv, Keyword::AnchorsValid},

    {"anchors-visible"sv, Keyword::AnchorsVisible},

    {"ancient-tamil"sv, Keyword::AncientTamil},

    {"anonymous"sv, Keyword::Anonymous},

    {"anywhere"sv, Keyword::Anywhere},

    {"appworkspace"sv, Keyword::Appworkspace},

    {"arabic-abjad"sv, Keyword::ArabicAbjad},

    {"arabic-indic"sv, Keyword::ArabicIndic},

    {"ari"sv, Keyword::Ari},

    {"armenian"sv, Keyword::Armenian},

    {"auto"sv, Keyword::Auto},

    {"auto-add"sv, Keyword::AutoAdd},

    {"avar2"sv, Keyword::Avar2},

    {"b4"sv, Keyword::B4},

    {"b5"sv, Keyword::B5},

    {"back"sv, Keyword::Back},

    {"background"sv, Keyword::Background},

    {"backwards"sv, Keyword::Backwards},

    {"balance"sv, Keyword::Balance},

    {"balinese"sv, Keyword::Balinese},

    {"bamum"sv, Keyword::Bamum},

    {"bangla"sv, Keyword::Bangla},

    {"baseline"sv, Keyword::Baseline},

    {"bengali"sv, Keyword::Bengali},

    {"bevel"sv, Keyword::Bevel},

    {"bidi-override"sv, Keyword::BidiOverride},

    {"binary"sv, Keyword::Binary},

    {"blin"sv, Keyword::Blin},

    {"blink"sv, Keyword::Blink},

    {"block"sv, Keyword::Block},

    {"block-end"sv, Keyword::BlockEnd},

    {"block-start"sv, Keyword::BlockStart},

    {"bodo"sv, Keyword::Bodo},

    {"bold"sv, Keyword::Bold},

    {"bolder"sv, Keyword::Bolder},

    {"border-box"sv, Keyword::BorderBox},

    {"both"sv, Keyword::Both},

    {"both-edges"sv, Keyword::BothEdges},

    {"bottom"sv, Keyword::Bottom},

    {"break-all"sv, Keyword::BreakAll},

    {"break-spaces"sv, Keyword::BreakSpaces},

    {"break-word"sv, Keyword::BreakWord},

    {"browser"sv, Keyword::Browser},

    {"bullets"sv, Keyword::Bullets},

    {"butt"sv, Keyword::Butt},

    {"button"sv, Keyword::Button},

    {"buttonborder"sv, Keyword::Buttonborder},

    {"buttonface"sv, Keyword::Buttonface},

    {"buttonhighlight"sv, Keyword::Buttonhighlight},

    {"buttonshadow"sv, Keyword::Buttonshadow},

    {"buttontext"sv, Keyword::Buttontext},

    {"cambodian"sv, Keyword::Cambodian},

    {"canvas"sv, Keyword::Canvas},

    {"canvastext"sv, Keyword::Canvastext},

    {"capitalize"sv, Keyword::Capitalize},

    {"captiontext"sv, Keyword::Captiontext},

    {"cell"sv, Keyword::Cell},

    {"center"sv, Keyword::Center},

    {"central"sv, Keyword::Central},

    {"checkbox"sv, Keyword::Checkbox},

    {"circle"sv, Keyword::Circle},

    {"circled-decimal"sv, Keyword::CircledDecimal},

    {"circled-ideograph"sv, Keyword::CircledIdeograph},

    {"circled-katakana"sv, Keyword::CircledKatakana},

    {"circled-korean-consonant"sv, Keyword::CircledKoreanConsonant},

    {"circled-korean-syllable"sv, Keyword::CircledKoreanSyllable},

    {"circled-lower-latin"sv, Keyword::CircledLowerLatin},

    {"circled-upper-latin"sv, Keyword::CircledUpperLatin},

    {"cjk-decimal"sv, Keyword::CjkDecimal},

    {"cjk-earthly-branch"sv, Keyword::CjkEarthlyBranch},

    {"cjk-heavenly-stem"sv, Keyword::CjkHeavenlyStem},

    {"cjk-ideographic"sv, Keyword::CjkIdeographic},

    {"cjk-stem-branch"sv, Keyword::CjkStemBranch},

    {"cjk-tally-mark"sv, Keyword::CjkTallyMark},

    {"clip"sv, Keyword::Clip},

    {"close-quote"sv, Keyword::CloseQuote},

    {"closest-corner"sv, Keyword::ClosestCorner},

    {"closest-side"sv, Keyword::ClosestSide},

    {"coarse"sv, Keyword::Coarse},

    {"col-resize"sv, Keyword::ColResize},

    {"collapse"sv, Keyword::Collapse},

    {"color"sv, Keyword::Color},

    {"color-burn"sv, Keyword::ColorBurn},

    {"color-cbdt"sv, Keyword::ColorCbdt},

    {"color-colrv0"sv, Keyword::ColorColrv0},

    {"color-colrv1"sv, Keyword::ColorColrv1},

    {"color-dodge"sv, Keyword::ColorDodge},

    {"color-sbix"sv, Keyword::ColorSbix},

    {"color-svg"sv, Keyword::ColorSvg},

    {"column"sv, Keyword::Column},

    {"column-reverse"sv, Keyword::ColumnReverse},

    {"common-ligatures"sv, Keyword::CommonLigatures},

    {"compact"sv, Keyword::Compact},

    {"condensed"sv, Keyword::Condensed},

    {"contain"sv, Keyword::Contain},

    {"content"sv, Keyword::Content},

    {"content-box"sv, Keyword::ContentBox},

    {"contents"sv, Keyword::Contents},

    {"context-menu"sv, Keyword::ContextMenu},

    {"contextual"sv, Keyword::Contextual},

    {"copy"sv, Keyword::Copy},

    {"cover"sv, Keyword::Cover},

    {"crisp-edges"sv, Keyword::CrispEdges},

    {"crispedges"sv, Keyword::Crispedges},

    {"crop"sv, Keyword::Crop},

    {"cross"sv, Keyword::Cross},

    {"crosshair"sv, Keyword::Crosshair},

    {"currentcolor"sv, Keyword::Currentcolor},

    {"cursive"sv, Keyword::Cursive},

    {"custom"sv, Keyword::Custom},

    {"cyclic"sv, Keyword::Cyclic},

    {"dark"sv, Keyword::Dark},

    {"darken"sv, Keyword::Darken},

    {"dashed"sv, Keyword::Dashed},

    {"decimal"sv, Keyword::Decimal},

    {"decimal-leading-zero"sv, Keyword::DecimalLeadingZero},

    {"decreasing"sv, Keyword::Decreasing},

    {"default"sv, Keyword::Default},

    {"devanagari"sv, Keyword::Devanagari},

    {"diagonal-fractions"sv, Keyword::DiagonalFractions},

    {"difference"sv, Keyword::Difference},

    {"disc"sv, Keyword::Disc},

    {"discard"sv, Keyword::Discard},

    {"discard-before"sv, Keyword::DiscardBefore},

    {"discard-after"sv, Keyword::DiscardAfter},

    {"discard-inner"sv, Keyword::DiscardInner},

    {"disclosure-closed"sv, Keyword::DisclosureClosed},

    {"disclosure-open"sv, Keyword::DisclosureOpen},

    {"discretionary-ligatures"sv, Keyword::DiscretionaryLigatures},

    {"distribute"sv, Keyword::Distribute},

    {"dizi"sv, Keyword::Dizi},

    {"dogri"sv, Keyword::Dogri},

    {"dotted-decimal"sv, Keyword::DottedDecimal},

    {"dotted"sv, Keyword::Dotted},

    {"double-circled-decimal"sv, Keyword::DoubleCircledDecimal},

    {"double"sv, Keyword::Double},

    {"down"sv, Keyword::Down},

    {"e"sv, Keyword::E},

    {"e-resize"sv, Keyword::EResize},

    {"each-line"sv, Keyword::EachLine},

    {"ease"sv, Keyword::Ease},

    {"ease-in"sv, Keyword::EaseIn},

    {"ease-in-out"sv, Keyword::EaseInOut},

    {"ease-out"sv, Keyword::EaseOut},

    {"element-shared"sv, Keyword::ElementShared},

    {"ellipsis"sv, Keyword::Ellipsis},

    {"embed"sv, Keyword::Embed},

    {"emoji"sv, Keyword::Emoji},

    {"enabled"sv, Keyword::Enabled},

    {"end"sv, Keyword::End},

    {"ethiopic-halehame-am"sv, Keyword::EthiopicHalehameAm},

    {"ethiopic-halehame-ti-er"sv, Keyword::EthiopicHalehameTiEr},

    {"ethiopic-halehame-ti-et"sv, Keyword::EthiopicHalehameTiEt},

    {"ethiopic-halehame"sv, Keyword::EthiopicHalehame},

    {"ethiopic-numeric"sv, Keyword::EthiopicNumeric},

    {"evenodd"sv, Keyword::Evenodd},

    {"ew-resize"sv, Keyword::EwResize},

    {"exclude"sv, Keyword::Exclude},

    {"exclusion"sv, Keyword::Exclusion},

    {"expanded"sv, Keyword::Expanded},

    {"extends"sv, Keyword::Extends},

    {"extra-condensed"sv, Keyword::ExtraCondensed},

    {"extra-expanded"sv, Keyword::ExtraExpanded},

    {"fallback"sv, Keyword::Fallback},

    {"false"sv, Keyword::False},

    {"fantasy"sv, Keyword::Fantasy},

    {"farthest-corner"sv, Keyword::FarthestCorner},

    {"farthest-side"sv, Keyword::FarthestSide},

    {"fast"sv, Keyword::Fast},

    {"features-aat"sv, Keyword::FeaturesAat},

    {"features-graphite"sv, Keyword::FeaturesGraphite},

    {"features-opentype"sv, Keyword::FeaturesOpentype},

    {"field"sv, Keyword::Field},

    {"fieldtext"sv, Keyword::Fieldtext},

    {"fill"sv, Keyword::Fill},

    {"fill-box"sv, Keyword::FillBox},

    {"filled-circled-decimal"sv, Keyword::FilledCircledDecimal},

    {"fine"sv, Keyword::Fine},

    {"fixed"sv, Keyword::Fixed},

    {"flat"sv, Keyword::Flat},

    {"flex"sv, Keyword::Flex},

    {"flex-end"sv, Keyword::FlexEnd},

    {"flex-start"sv, Keyword::FlexStart},

    {"flip-block"sv, Keyword::FlipBlock},

    {"flip-inline"sv, Keyword::FlipInline},

    {"flip-start"sv, Keyword::FlipStart},

    {"flow"sv, Keyword::Flow},

    {"flow-root"sv, Keyword::FlowRoot},

    {"forwards"sv, Keyword::Forwards},

    {"from-font"sv, Keyword::FromFont},

    {"full-size-kana"sv, Keyword::FullSizeKana},

    {"full-width"sv, Keyword::FullWidth},

    {"fullwidth-decimal"sv, Keyword::FullwidthDecimal},

    {"fullwidth-lower-alpha"sv, Keyword::FullwidthLowerAlpha},

    {"fullwidth-upper-alpha"sv, Keyword::FullwidthUpperAlpha},

    {"fullwidth-upper-roman"sv, Keyword::FullwidthUpperRoman},

    {"fullscreen"sv, Keyword::Fullscreen},

    {"gedeo"sv, Keyword::Gedeo},

    {"geometricprecision"sv, Keyword::Geometricprecision},

    {"georgian"sv, Keyword::Georgian},

    {"grab"sv, Keyword::Grab},

    {"grabbing"sv, Keyword::Grabbing},

    {"grammar-error"sv, Keyword::GrammarError},

    {"graytext"sv, Keyword::Graytext},

    {"greek-lower-ancient"sv, Keyword::GreekLowerAncient},

    {"greek-lower-modern"sv, Keyword::GreekLowerModern},

    {"greek-upper-ancient"sv, Keyword::GreekUpperAncient},

    {"greek-upper-modern"sv, Keyword::GreekUpperModern},

    {"grid"sv, Keyword::Grid},

    {"groove"sv, Keyword::Groove},

    {"gujarati-alpha"sv, Keyword::GujaratiAlpha},

    {"gujarati"sv, Keyword::Gujarati},

    {"gumuz"sv, Keyword::Gumuz},

    {"gurmukhi"sv, Keyword::Gurmukhi},

    {"hadiyaa"sv, Keyword::Hadiyaa},

    {"hanging"sv, Keyword::Hanging},

    {"hangul-consonant"sv, Keyword::HangulConsonant},

    {"hangul"sv, Keyword::Hangul},

    {"hanifi-rohingya"sv, Keyword::HanifiRohingya},

    {"harari"sv, Keyword::Harari},

    {"hard-light"sv, Keyword::HardLight},

    {"hebrew"sv, Keyword::Hebrew},

    {"height"sv, Keyword::Height},

    {"help"sv, Keyword::Help},

    {"hide"sv, Keyword::Hide},

    {"hidden"sv, Keyword::Hidden},

    {"high"sv, Keyword::High},

    {"high-quality"sv, Keyword::HighQuality},

    {"highlight"sv, Keyword::Highlight},

    {"highlighttext"sv, Keyword::Highlighttext},

    {"hindi"sv, Keyword::Hindi},

    {"hiragana-iroha"sv, Keyword::HiraganaIroha},

    {"hiragana"sv, Keyword::Hiragana},

    {"historical-forms"sv, Keyword::HistoricalForms},

    {"historical-ligatures"sv, Keyword::HistoricalLigatures},

    {"horizontal"sv, Keyword::Horizontal},

    {"horizontal-tb"sv, Keyword::HorizontalTb},

    {"hover"sv, Keyword::Hover},

    {"hsl"sv, Keyword::Hsl},

    {"hue"sv, Keyword::Hue},

    {"hwb"sv, Keyword::Hwb},

    {"inactiveborder"sv, Keyword::Inactiveborder},

    {"inactivecaption"sv, Keyword::Inactivecaption},

    {"inactivecaptiontext"sv, Keyword::Inactivecaptiontext},

    {"increasing"sv, Keyword::Increasing},

    {"ideographic"sv, Keyword::Ideographic},

    {"incremental"sv, Keyword::Incremental},

    {"infinite"sv, Keyword::Infinite},

    {"infinity"sv, Keyword::Infinity},

    {"infobackground"sv, Keyword::Infobackground},

    {"infotext"sv, Keyword::Infotext},

    {"inherit"sv, Keyword::Inherit},

    {"initial"sv, Keyword::Initial},

    {"initial-only"sv, Keyword::InitialOnly},

    {"inline"sv, Keyword::Inline},

    {"inline-block"sv, Keyword::InlineBlock},

    {"inline-end"sv, Keyword::InlineEnd},

    {"inline-flex"sv, Keyword::InlineFlex},

    {"inline-grid"sv, Keyword::InlineGrid},

    {"inline-size"sv, Keyword::InlineSize},

    {"inline-start"sv, Keyword::InlineStart},

    {"inline-table"sv, Keyword::InlineTable},

    {"inset"sv, Keyword::Inset},

    {"inside"sv, Keyword::Inside},

    {"inter-character"sv, Keyword::InterCharacter},

    {"inter-word"sv, Keyword::InterWord},

    {"interlace"sv, Keyword::Interlace},

    {"intersect"sv, Keyword::Intersect},

    {"invert"sv, Keyword::Invert},

    {"inverted"sv, Keyword::Inverted},

    {"isolate"sv, Keyword::Isolate},

    {"isolate-override"sv, Keyword::IsolateOverride},

    {"italic"sv, Keyword::Italic},

    {"japanese-formal"sv, Keyword::JapaneseFormal},

    {"japanese-informal"sv, Keyword::JapaneseInformal},

    {"javanese"sv, Keyword::Javanese},

    {"jis-b4"sv, Keyword::JisB4},

    {"jis-b5"sv, Keyword::JisB5},

    {"jis04"sv, Keyword::Jis04},

    {"jis78"sv, Keyword::Jis78},

    {"jis83"sv, Keyword::Jis83},

    {"jis90"sv, Keyword::Jis90},

    {"jump-both"sv, Keyword::JumpBoth},

    {"jump-end"sv, Keyword::JumpEnd},

    {"jump-none"sv, Keyword::JumpNone},

    {"jump-start"sv, Keyword::JumpStart},

    {"justify"sv, Keyword::Justify},

    {"kaffa"sv, Keyword::Kaffa},

    {"kannada-alpha"sv, Keyword::KannadaAlpha},

    {"kannada"sv, Keyword::Kannada},

    {"kashmiri"sv, Keyword::Kashmiri},

    {"katakana-iroha"sv, Keyword::KatakanaIroha},

    {"katakana"sv, Keyword::Katakana},

    {"kayah-li"sv, Keyword::KayahLi},

    {"kebena"sv, Keyword::Kebena},

    {"keep-all"sv, Keyword::KeepAll},

    {"kembata"sv, Keyword::Kembata},

    {"khmer-consonant"sv, Keyword::KhmerConsonant},

    {"khmer"sv, Keyword::Khmer},

    {"konkani"sv, Keyword::Konkani},

    {"konso"sv, Keyword::Konso},

    {"korean-consonant"sv, Keyword::KoreanConsonant},

    {"korean-hangul-formal"sv, Keyword::KoreanHangulFormal},

    {"korean-hanja-formal"sv, Keyword::KoreanHanjaFormal},

    {"korean-hanja-informal"sv, Keyword::KoreanHanjaInformal},

    {"korean-syllable"sv, Keyword::KoreanSyllable},

    {"kunama"sv, Keyword::Kunama},

    {"landscape"sv, Keyword::Landscape},

    {"lanna-hora"sv, Keyword::LannaHora},

    {"lanna-tham"sv, Keyword::LannaTham},

    {"lao"sv, Keyword::Lao},

    {"large"sv, Keyword::Large},

    {"larger"sv, Keyword::Larger},

    {"layout"sv, Keyword::Layout},

    {"lch"sv, Keyword::Lch},

    {"ledger"sv, Keyword::Ledger},

    {"left"sv, Keyword::Left},

    {"legacy"sv, Keyword::Legacy},

    {"legal"sv, Keyword::Legal},

    {"lepcha"sv, Keyword::Lepcha},

    {"less"sv, Keyword::Less},

    {"letter"sv, Keyword::Letter},

    {"light"sv, Keyword::Light},

    {"lighten"sv, Keyword::Lighten},

    {"lighter"sv, Keyword::Lighter},

    {"limbu"sv, Keyword::Limbu},

    {"line-through"sv, Keyword::LineThrough},

    {"linear"sv, Keyword::Linear},

    {"linearrgb"sv, Keyword::Linearrgb},

    {"lining-nums"sv, Keyword::LiningNums},

    {"linktext"sv, Keyword::Linktext},

    {"list-item"sv, Keyword::ListItem},

    {"listbox"sv, Keyword::Listbox},

    {"local"sv, Keyword::Local},

    {"longer"sv, Keyword::Longer},

    {"lower-alpha-symbolic"sv, Keyword::LowerAlphaSymbolic},

    {"lower-alpha"sv, Keyword::LowerAlpha},

    {"lower-armenian"sv, Keyword::LowerArmenian},

    {"lower-belorussian"sv, Keyword::LowerBelorussian},

    {"lower-bulgarian"sv, Keyword::LowerBulgarian},

    {"lower-greek"sv, Keyword::LowerGreek},

    {"lower-hexadecimal"sv, Keyword::LowerHexadecimal},

    {"lower-latin"sv, Keyword::LowerLatin},

    {"lower-macedonian"sv, Keyword::LowerMacedonian},

    {"lower-oromo-qubee"sv, Keyword::LowerOromoQubee},

    {"lower-roman"sv, Keyword::LowerRoman},

    {"lower-russian-full"sv, Keyword::LowerRussianFull},

    {"lower-russian"sv, Keyword::LowerRussian},

    {"lower-serbian"sv, Keyword::LowerSerbian},

    {"lower-ukrainian-full"sv, Keyword::LowerUkrainianFull},

    {"lower-ukrainian"sv, Keyword::LowerUkrainian},

    {"lowercase"sv, Keyword::Lowercase},

    {"ltr"sv, Keyword::Ltr},

    {"luminance"sv, Keyword::Luminance},

    {"luminosity"sv, Keyword::Luminosity},

    {"maghrebi-abjad"sv, Keyword::MaghrebiAbjad},

    {"maithili"sv, Keyword::Maithili},

    {"malayalam-alpha"sv, Keyword::MalayalamAlpha},

    {"malayalam"sv, Keyword::Malayalam},

    {"manipulation"sv, Keyword::Manipulation},

    {"manipuri"sv, Keyword::Manipuri},

    {"marathi"sv, Keyword::Marathi},

    {"margin-box"sv, Keyword::MarginBox},

    {"mark"sv, Keyword::Mark},

    {"markers"sv, Keyword::Markers},

    {"marktext"sv, Keyword::Marktext},

    {"match-parent"sv, Keyword::MatchParent},

    {"match-source"sv, Keyword::MatchSource},

    {"math"sv, Keyword::Math},

    {"math-auto"sv, Keyword::MathAuto},

    {"mathematical"sv, Keyword::Mathematical},

    {"max-content"sv, Keyword::MaxContent},

    {"medium"sv, Keyword::Medium},

    {"meen"sv, Keyword::Meen},

    {"meetei"sv, Keyword::Meetei},

    {"menu"sv, Keyword::Menu},

    {"menulist"sv, Keyword::Menulist},

    {"menulist-button"sv, Keyword::MenulistButton},

    {"menutext"sv, Keyword::Menutext},

    {"meter"sv, Keyword::Meter},

    {"middle"sv, Keyword::Middle},

    {"min-content"sv, Keyword::MinContent},

    {"minimal-ui"sv, Keyword::MinimalUi},

    {"miter"sv, Keyword::Miter},

    {"mongolian"sv, Keyword::Mongolian},

    {"monospace"sv, Keyword::Monospace},

    {"more"sv, Keyword::More},

    {"most-block-size"sv, Keyword::MostBlockSize},

    {"most-height"sv, Keyword::MostHeight},

    {"most-inline-size"sv, Keyword::MostInlineSize},

    {"most-width"sv, Keyword::MostWidth},

    {"move"sv, Keyword::Move},

    {"mro"sv, Keyword::Mro},

    {"multiply"sv, Keyword::Multiply},

    {"myanmar"sv, Keyword::Myanmar},

    {"n-resize"sv, Keyword::NResize},

    {"nag-mundari"sv, Keyword::NagMundari},

    {"nan"sv, Keyword::Nan},

    {"ne-resize"sv, Keyword::NeResize},

    {"nearest"sv, Keyword::Nearest},

    {"nesw-resize"sv, Keyword::NeswResize},

    {"new-base-60"sv, Keyword::NewBase60},

    {"newa"sv, Keyword::Newa},

    {"nko-cardinal"sv, Keyword::NkoCardinal},

    {"no-clip"sv, Keyword::NoClip},

    {"no-close-quote"sv, Keyword::NoCloseQuote},

    {"no-common-ligatures"sv, Keyword::NoCommonLigatures},

    {"no-contextual"sv, Keyword::NoContextual},

    {"no-discretionary-ligatures"sv, Keyword::NoDiscretionaryLigatures},

    {"no-drop"sv, Keyword::NoDrop},

    {"no-historical-ligatures"sv, Keyword::NoHistoricalLigatures},

    {"no-open-quote"sv, Keyword::NoOpenQuote},

    {"no-overflow"sv, Keyword::NoOverflow},

    {"no-preference"sv, Keyword::NoPreference},

    {"no-referrer"sv, Keyword::NoReferrer},

    {"no-referrer-when-downgrade"sv, Keyword::NoReferrerWhenDowngrade},

    {"no-repeat"sv, Keyword::NoRepeat},

    {"none"sv, Keyword::None},

    {"nonzero"sv, Keyword::Nonzero},

    {"normal"sv, Keyword::Normal},

    {"not-allowed"sv, Keyword::NotAllowed},

    {"notch"sv, Keyword::Notch},

    {"nowrap"sv, Keyword::Nowrap},

    {"ns-resize"sv, Keyword::NsResize},

    {"numbers"sv, Keyword::Numbers},

    {"numeric"sv, Keyword::Numeric},

    {"nw-resize"sv, Keyword::NwResize},

    {"nwse-resize"sv, Keyword::NwseResize},

    {"oblique"sv, Keyword::Oblique},

    {"octal"sv, Keyword::Octal},

    {"odia"sv, Keyword::Odia},

    {"off"sv, Keyword::Off},

    {"oklch"sv, Keyword::Oklch},

    {"ol-chiki"sv, Keyword::OlChiki},

    {"oldstyle-nums"sv, Keyword::OldstyleNums},

    {"on"sv, Keyword::On},

    {"opaque"sv, Keyword::Opaque},

    {"open-quote"sv, Keyword::OpenQuote},

    {"optimizelegibility"sv, Keyword::Optimizelegibility},

    {"optimizequality"sv, Keyword::Optimizequality},

    {"optimizespeed"sv, Keyword::Optimizespeed},

    {"optional"sv, Keyword::Optional},

    {"ordinal"sv, Keyword::Ordinal},

    {"origin"sv, Keyword::Origin},

    {"origin-when-cross-origin"sv, Keyword::OriginWhenCrossOrigin},

    {"oriya"sv, Keyword::Oriya},

    {"oromo"sv, Keyword::Oromo},

    {"outset"sv, Keyword::Outset},

    {"outside"sv, Keyword::Outside},

    {"overlay"sv, Keyword::Overlay},

    {"overline"sv, Keyword::Overline},

    {"p3"sv, Keyword::P3},

    {"padding-box"sv, Keyword::PaddingBox},

    {"paged"sv, Keyword::Paged},

    {"paint"sv, Keyword::Paint},

    {"palettes"sv, Keyword::Palettes},

    {"pan-down"sv, Keyword::PanDown},

    {"pan-left"sv, Keyword::PanLeft},

    {"pan-right"sv, Keyword::PanRight},

    {"pan-up"sv, Keyword::PanUp},

    {"pan-x"sv, Keyword::PanX},

    {"pan-y"sv, Keyword::PanY},

    {"parenthesized-decimal"sv, Keyword::ParenthesizedDecimal},

    {"parenthesized-hangul-consonant"sv, Keyword::ParenthesizedHangulConsonant},

    {"parenthesized-hangul-syllable"sv, Keyword::ParenthesizedHangulSyllable},

    {"parenthesized-ideograph"sv, Keyword::ParenthesizedIdeograph},

    {"parenthesized-lower-latin"sv, Keyword::ParenthesizedLowerLatin},

    {"paused"sv, Keyword::Paused},

    {"persian-abjad"sv, Keyword::PersianAbjad},

    {"persian-alphabetic"sv, Keyword::PersianAlphabetic},

    {"persian"sv, Keyword::Persian},

    {"petite-caps"sv, Keyword::PetiteCaps},

    {"pi"sv, Keyword::Pi},

    {"picture-in-picture"sv, Keyword::PictureInPicture},

    {"pixelated"sv, Keyword::Pixelated},

    {"plaintext"sv, Keyword::Plaintext},

    {"plus-darker"sv, Keyword::PlusDarker},

    {"plus-lighter"sv, Keyword::PlusLighter},

    {"pointer"sv, Keyword::Pointer},

    {"portrait"sv, Keyword::Portrait},

    {"pre"sv, Keyword::Pre},

    {"pre-line"sv, Keyword::PreLine},

    {"pre-wrap"sv, Keyword::PreWrap},

    {"preserve"sv, Keyword::Preserve},

    {"preserve-3d"sv, Keyword::Preserve3d},

    {"preserve-breaks"sv, Keyword::PreserveBreaks},

    {"preserve-spaces"sv, Keyword::PreserveSpaces},

    {"pretty"sv, Keyword::Pretty},

    {"progress"sv, Keyword::Progress},

    {"progress-bar"sv, Keyword::ProgressBar},

    {"progressive"sv, Keyword::Progressive},

    {"proportional-nums"sv, Keyword::ProportionalNums},

    {"proportional-width"sv, Keyword::ProportionalWidth},

    {"punjabi"sv, Keyword::Punjabi},

    {"push-button"sv, Keyword::PushButton},

    {"radio"sv, Keyword::Radio},

    {"rec2020"sv, Keyword::Rec2020},

    {"reduce"sv, Keyword::Reduce},

    {"relative"sv, Keyword::Relative},

    {"repeat"sv, Keyword::Repeat},

    {"repeat-x"sv, Keyword::RepeatX},

    {"repeat-y"sv, Keyword::RepeatY},

    {"replace"sv, Keyword::Replace},

    {"reverse"sv, Keyword::Reverse},

    {"revert"sv, Keyword::Revert},

    {"revert-layer"sv, Keyword::RevertLayer},

    {"ridge"sv, Keyword::Ridge},

    {"right"sv, Keyword::Right},

    {"root"sv, Keyword::Root},

    {"rotate-left"sv, Keyword::RotateLeft},

    {"rotate-right"sv, Keyword::RotateRight},

    {"round"sv, Keyword::Round},

    {"row"sv, Keyword::Row},

    {"row-resize"sv, Keyword::RowResize},

    {"row-reverse"sv, Keyword::RowReverse},

    {"rtl"sv, Keyword::Rtl},

    {"ruby"sv, Keyword::Ruby},

    {"ruby-base"sv, Keyword::RubyBase},

    {"ruby-base-container"sv, Keyword::RubyBaseContainer},

    {"ruby-text"sv, Keyword::RubyText},

    {"ruby-text-container"sv, Keyword::RubyTextContainer},

    {"run-in"sv, Keyword::RunIn},

    {"running"sv, Keyword::Running},

    {"s-resize"sv, Keyword::SResize},

    {"safe"sv, Keyword::Safe},

    {"saho"sv, Keyword::Saho},

    {"same-origin"sv, Keyword::SameOrigin},

    {"sans-serif"sv, Keyword::SansSerif},

    {"sanskrit"sv, Keyword::Sanskrit},

    {"santali"sv, Keyword::Santali},

    {"saturation"sv, Keyword::Saturation},

    {"scale-down"sv, Keyword::ScaleDown},

    {"scoop"sv, Keyword::Scoop},

    {"screen"sv, Keyword::Screen},

    {"scroll"sv, Keyword::Scroll},

    {"scroll-position"sv, Keyword::ScrollPosition},

    {"scroll-state"sv, Keyword::ScrollState},

    {"scrollbar"sv, Keyword::Scrollbar},

    {"se-resize"sv, Keyword::SeResize},

    {"searchfield"sv, Keyword::Searchfield},

    {"selecteditem"sv, Keyword::Selecteditem},

    {"selecteditemtext"sv, Keyword::Selecteditemtext},

    {"self"sv, Keyword::Self},

    {"self-block"sv, Keyword::SelfBlock},

    {"self-block-end"sv, Keyword::SelfBlockEnd},

    {"self-block-start"sv, Keyword::SelfBlockStart},

    {"self-end"sv, Keyword::SelfEnd},

    {"self-inline"sv, Keyword::SelfInline},

    {"self-inline-end"sv, Keyword::SelfInlineEnd},

    {"self-inline-start"sv, Keyword::SelfInlineStart},

    {"self-x-end"sv, Keyword::SelfXEnd},

    {"self-x-start"sv, Keyword::SelfXStart},

    {"self-y-end"sv, Keyword::SelfYEnd},

    {"self-y-start"sv, Keyword::SelfYStart},

    {"self-start"sv, Keyword::SelfStart},

    {"semi-condensed"sv, Keyword::SemiCondensed},

    {"semi-expanded"sv, Keyword::SemiExpanded},

    {"separate"sv, Keyword::Separate},

    {"serif"sv, Keyword::Serif},

    {"shan"sv, Keyword::Shan},

    {"shorter"sv, Keyword::Shorter},

    {"show"sv, Keyword::Show},

    {"sidama"sv, Keyword::Sidama},

    {"sideways-lr"sv, Keyword::SidewaysLr},

    {"sideways-rl"sv, Keyword::SidewaysRl},

    {"silti"sv, Keyword::Silti},

    {"simp-chinese-formal"sv, Keyword::SimpChineseFormal},

    {"simp-chinese-informal"sv, Keyword::SimpChineseInformal},

    {"simple-lower-roman"sv, Keyword::SimpleLowerRoman},

    {"simple-upper-roman"sv, Keyword::SimpleUpperRoman},

    {"simplified"sv, Keyword::Simplified},

    {"sindhi"sv, Keyword::Sindhi},

    {"size"sv, Keyword::Size},

    {"slashed-zero"sv, Keyword::SlashedZero},

    {"slider-horizontal"sv, Keyword::SliderHorizontal},

    {"slow"sv, Keyword::Slow},

    {"small"sv, Keyword::Small},

    {"small-caps"sv, Keyword::SmallCaps},

    {"smaller"sv, Keyword::Smaller},

    {"smooth"sv, Keyword::Smooth},

    {"soft-light"sv, Keyword::SoftLight},

    {"solid"sv, Keyword::Solid},

    {"space"sv, Keyword::Space},

    {"space-around"sv, Keyword::SpaceAround},

    {"space-between"sv, Keyword::SpaceBetween},

    {"space-evenly"sv, Keyword::SpaceEvenly},

    {"span-all"sv, Keyword::SpanAll},

    {"span-block-end"sv, Keyword::SpanBlockEnd},

    {"span-block-start"sv, Keyword::SpanBlockStart},

    {"span-bottom"sv, Keyword::SpanBottom},

    {"span-end"sv, Keyword::SpanEnd},

    {"span-inline-end"sv, Keyword::SpanInlineEnd},

    {"span-inline-start"sv, Keyword::SpanInlineStart},

    {"span-left"sv, Keyword::SpanLeft},

    {"span-right"sv, Keyword::SpanRight},

    {"span-self-block-end"sv, Keyword::SpanSelfBlockEnd},

    {"span-self-block-start"sv, Keyword::SpanSelfBlockStart},

    {"span-self-end"sv, Keyword::SpanSelfEnd},

    {"span-self-inline-end"sv, Keyword::SpanSelfInlineEnd},

    {"span-self-inline-start"sv, Keyword::SpanSelfInlineStart},

    {"span-self-start"sv, Keyword::SpanSelfStart},

    {"span-self-x-end"sv, Keyword::SpanSelfXEnd},

    {"span-self-x-start"sv, Keyword::SpanSelfXStart},

    {"span-self-y-end"sv, Keyword::SpanSelfYEnd},

    {"span-self-y-start"sv, Keyword::SpanSelfYStart},

    {"span-start"sv, Keyword::SpanStart},

    {"span-top"sv, Keyword::SpanTop},

    {"span-x-end"sv, Keyword::SpanXEnd},

    {"span-x-start"sv, Keyword::SpanXStart},

    {"span-y-end"sv, Keyword::SpanYEnd},

    {"span-y-start"sv, Keyword::SpanYStart},

    {"spelling-error"sv, Keyword::SpellingError},

    {"spell-out"sv, Keyword::SpellOut},

    {"square"sv, Keyword::Square},

    {"square-button"sv, Keyword::SquareButton},

    {"squircle"sv, Keyword::Squircle},

    {"srgb"sv, Keyword::Srgb},

    {"stable"sv, Keyword::Stable},

    {"stacked-fractions"sv, Keyword::StackedFractions},

    {"standalone"sv, Keyword::Standalone},

    {"standard"sv, Keyword::Standard},

    {"start"sv, Keyword::Start},

    {"static"sv, Keyword::Static},

    {"sticky"sv, Keyword::Sticky},

    {"stretch"sv, Keyword::Stretch},

    {"strict"sv, Keyword::Strict},

    {"strict-origin"sv, Keyword::StrictOrigin},

    {"strict-origin-when-cross-origin"sv, Keyword::StrictOriginWhenCrossOrigin},

    {"stroke"sv, Keyword::Stroke},

    {"stroke-box"sv, Keyword::StrokeBox},

    {"style"sv, Keyword::Style},

    {"sub"sv, Keyword::Sub},

    {"subtract"sv, Keyword::Subtract},

    {"subtractive"sv, Keyword::Subtractive},

    {"sundanese"sv, Keyword::Sundanese},

    {"super-decimal"sv, Keyword::SuperDecimal},

    {"super"sv, Keyword::Super},

    {"sw-resize"sv, Keyword::SwResize},

    {"swap"sv, Keyword::Swap},

    {"symbolic"sv, Keyword::Symbolic},

    {"table"sv, Keyword::Table},

    {"table-caption"sv, Keyword::TableCaption},

    {"table-cell"sv, Keyword::TableCell},

    {"table-column"sv, Keyword::TableColumn},

    {"table-column-group"sv, Keyword::TableColumnGroup},

    {"table-footer-group"sv, Keyword::TableFooterGroup},

    {"table-header-group"sv, Keyword::TableHeaderGroup},

    {"table-row"sv, Keyword::TableRow},

    {"table-row-group"sv, Keyword::TableRowGroup},

    {"tabular-nums"sv, Keyword::TabularNums},

    {"tai-lue"sv, Keyword::TaiLue},

    {"tally-mark"sv, Keyword::TallyMark},

    {"tamil"sv, Keyword::Tamil},

    {"telugu-alpha"sv, Keyword::TeluguAlpha},

    {"telugu"sv, Keyword::Telugu},

    {"text"sv, Keyword::Text},

    {"text-bottom"sv, Keyword::TextBottom},

    {"text-top"sv, Keyword::TextTop},

    {"textarea"sv, Keyword::Textarea},

    {"textfield"sv, Keyword::Textfield},

    {"thai-alpha"sv, Keyword::ThaiAlpha},

    {"thai"sv, Keyword::Thai},

    {"thick"sv, Keyword::Thick},

    {"thin"sv, Keyword::Thin},

    {"threeddarkshadow"sv, Keyword::Threeddarkshadow},

    {"threedface"sv, Keyword::Threedface},

    {"threedhighlight"sv, Keyword::Threedhighlight},

    {"threedlightshadow"sv, Keyword::Threedlightshadow},

    {"threedshadow"sv, Keyword::Threedshadow},

    {"tibetan"sv, Keyword::Tibetan},

    {"tigre"sv, Keyword::Tigre},

    {"titling-caps"sv, Keyword::TitlingCaps},

    {"to-zero"sv, Keyword::ToZero},

    {"top"sv, Keyword::Top},

    {"trad-chinese-formal"sv, Keyword::TradChineseFormal},

    {"trad-chinese-informal"sv, Keyword::TradChineseInformal},

    {"traditional"sv, Keyword::Traditional},

    {"true"sv, Keyword::True},

    {"ui-monospace"sv, Keyword::UiMonospace},

    {"ui-rounded"sv, Keyword::UiRounded},

    {"ui-sans-serif"sv, Keyword::UiSansSerif},

    {"ui-serif"sv, Keyword::UiSerif},

    {"ultra-condensed"sv, Keyword::UltraCondensed},

    {"ultra-expanded"sv, Keyword::UltraExpanded},

    {"under"sv, Keyword::Under},

    {"underline"sv, Keyword::Underline},

    {"unicase"sv, Keyword::Unicase},

    {"unicode"sv, Keyword::Unicode},

    {"unsafe"sv, Keyword::Unsafe},

    {"unsafe-url"sv, Keyword::UnsafeUrl},

    {"unset"sv, Keyword::Unset},

    {"up"sv, Keyword::Up},

    {"upper-alpha-symbolic"sv, Keyword::UpperAlphaSymbolic},

    {"upper-alpha"sv, Keyword::UpperAlpha},

    {"upper-armenian"sv, Keyword::UpperArmenian},

    {"upper-belorussian"sv, Keyword::UpperBelorussian},

    {"upper-bulgarian"sv, Keyword::UpperBulgarian},

    {"upper-hexadecimal"sv, Keyword::UpperHexadecimal},

    {"upper-latin"sv, Keyword::UpperLatin},

    {"upper-macedonian"sv, Keyword::UpperMacedonian},

    {"upper-oromo-qubee"sv, Keyword::UpperOromoQubee},

    {"upper-roman"sv, Keyword::UpperRoman},

    {"upper-russian-full"sv, Keyword::UpperRussianFull},

    {"upper-russian"sv, Keyword::UpperRussian},

    {"upper-serbian"sv, Keyword::UpperSerbian},

    {"upper-ukrainian-full"sv, Keyword::UpperUkrainianFull},

    {"upper-ukrainian"sv, Keyword::UpperUkrainian},

    {"uppercase"sv, Keyword::Uppercase},

    {"upright"sv, Keyword::Upright},

    {"urdu-abjad"sv, Keyword::UrduAbjad},

    {"urdu-alphabetic"sv, Keyword::UrduAlphabetic},

    {"urdu"sv, Keyword::Urdu},

    {"use-credentials"sv, Keyword::UseCredentials},

    {"variations"sv, Keyword::Variations},

    {"vertical"sv, Keyword::Vertical},

    {"vertical-lr"sv, Keyword::VerticalLr},

    {"vertical-rl"sv, Keyword::VerticalRl},

    {"vertical-text"sv, Keyword::VerticalText},

    {"view-box"sv, Keyword::ViewBox},

    {"visible"sv, Keyword::Visible},

    {"visitedtext"sv, Keyword::Visitedtext},

    {"w-resize"sv, Keyword::WResize},

    {"wait"sv, Keyword::Wait},

    {"warang-citi"sv, Keyword::WarangCiti},

    {"wavy"sv, Keyword::Wavy},

    {"width"sv, Keyword::Width},

    {"window"sv, Keyword::Window},

    {"windowframe"sv, Keyword::Windowframe},

    {"windowtext"sv, Keyword::Windowtext},

    {"wolaita"sv, Keyword::Wolaita},

    {"words"sv, Keyword::Words},

    {"wrap"sv, Keyword::Wrap},

    {"wrap-reverse"sv, Keyword::WrapReverse},

    {"x"sv, Keyword::X},

    {"x-end"sv, Keyword::XEnd},

    {"x-large"sv, Keyword::XLarge},

    {"x-start"sv, Keyword::XStart},

    {"x-small"sv, Keyword::XSmall},

    {"xx-large"sv, Keyword::XxLarge},

    {"xx-small"sv, Keyword::XxSmall},

    {"xxx-large"sv, Keyword::XxxLarge},

    {"y"sv, Keyword::Y},

    {"y-end"sv, Keyword::YEnd},

    {"y-start"sv, Keyword::YStart},

    {"yemsa"sv, Keyword::Yemsa},

    {"zhuyin"sv, Keyword::Zhuyin},

    {"zoom-in"sv, Keyword::ZoomIn},

    {"zoom-out"sv, Keyword::ZoomOut},

};

Optional<Keyword> keyword_from_string(StringView string)
{
    return g_stringview_to_keyword_map.get(string);
}

StringView string_from_keyword(Keyword keyword) {
    switch (keyword) {

    case Keyword::NegativeInfinity:
        return "-infinity"sv;
        
    case Keyword::LibwebButtonfacedisabled:
        return "-libweb-buttonfacedisabled"sv;
        
    case Keyword::LibwebButtonfacehover:
        return "-libweb-buttonfacehover"sv;
        
    case Keyword::LibwebCenter:
        return "-libweb-center"sv;
        
    case Keyword::LibwebInheritOrCenter:
        return "-libweb-inherit-or-center"sv;
        
    case Keyword::LibwebLeft:
        return "-libweb-left"sv;
        
    case Keyword::LibwebLink:
        return "-libweb-link"sv;
        
    case Keyword::LibwebPaletteActiveLink:
        return "-libweb-palette-active-link"sv;
        
    case Keyword::LibwebPaletteActiveWindowBorder1:
        return "-libweb-palette-active-window-border1"sv;
        
    case Keyword::LibwebPaletteActiveWindowBorder2:
        return "-libweb-palette-active-window-border2"sv;
        
    case Keyword::LibwebPaletteActiveWindowTitle:
        return "-libweb-palette-active-window-title"sv;
        
    case Keyword::LibwebPaletteBase:
        return "-libweb-palette-base"sv;
        
    case Keyword::LibwebPaletteBaseText:
        return "-libweb-palette-base-text"sv;
        
    case Keyword::LibwebPaletteButton:
        return "-libweb-palette-button"sv;
        
    case Keyword::LibwebPaletteButtonText:
        return "-libweb-palette-button-text"sv;
        
    case Keyword::LibwebPaletteDesktopBackground:
        return "-libweb-palette-desktop-background"sv;
        
    case Keyword::LibwebPaletteFocusOutline:
        return "-libweb-palette-focus-outline"sv;
        
    case Keyword::LibwebPaletteHighlightWindowBorder1:
        return "-libweb-palette-highlight-window-border1"sv;
        
    case Keyword::LibwebPaletteHighlightWindowBorder2:
        return "-libweb-palette-highlight-window-border2"sv;
        
    case Keyword::LibwebPaletteHighlightWindowTitle:
        return "-libweb-palette-highlight-window-title"sv;
        
    case Keyword::LibwebPaletteHoverHighlight:
        return "-libweb-palette-hover-highlight"sv;
        
    case Keyword::LibwebPaletteInactiveSelection:
        return "-libweb-palette-inactive-selection"sv;
        
    case Keyword::LibwebPaletteInactiveSelectionText:
        return "-libweb-palette-inactive-selection-text"sv;
        
    case Keyword::LibwebPaletteInactiveWindowBorder1:
        return "-libweb-palette-inactive-window-border1"sv;
        
    case Keyword::LibwebPaletteInactiveWindowBorder2:
        return "-libweb-palette-inactive-window-border2"sv;
        
    case Keyword::LibwebPaletteInactiveWindowTitle:
        return "-libweb-palette-inactive-window-title"sv;
        
    case Keyword::LibwebPaletteLink:
        return "-libweb-palette-link"sv;
        
    case Keyword::LibwebPaletteMenuBase:
        return "-libweb-palette-menu-base"sv;
        
    case Keyword::LibwebPaletteMenuBaseText:
        return "-libweb-palette-menu-base-text"sv;
        
    case Keyword::LibwebPaletteMenuSelection:
        return "-libweb-palette-menu-selection"sv;
        
    case Keyword::LibwebPaletteMenuSelectionText:
        return "-libweb-palette-menu-selection-text"sv;
        
    case Keyword::LibwebPaletteMenuStripe:
        return "-libweb-palette-menu-stripe"sv;
        
    case Keyword::LibwebPaletteMovingWindowBorder1:
        return "-libweb-palette-moving-window-border1"sv;
        
    case Keyword::LibwebPaletteMovingWindowBorder2:
        return "-libweb-palette-moving-window-border2"sv;
        
    case Keyword::LibwebPaletteMovingWindowTitle:
        return "-libweb-palette-moving-window-title"sv;
        
    case Keyword::LibwebPaletteRubberBandBorder:
        return "-libweb-palette-rubber-band-border"sv;
        
    case Keyword::LibwebPaletteRubberBandFill:
        return "-libweb-palette-rubber-band-fill"sv;
        
    case Keyword::LibwebPaletteRuler:
        return "-libweb-palette-ruler"sv;
        
    case Keyword::LibwebPaletteRulerActiveText:
        return "-libweb-palette-ruler-active-text"sv;
        
    case Keyword::LibwebPaletteRulerBorder:
        return "-libweb-palette-ruler-border"sv;
        
    case Keyword::LibwebPaletteRulerInactiveText:
        return "-libweb-palette-ruler-inactive-text"sv;
        
    case Keyword::LibwebPaletteSelection:
        return "-libweb-palette-selection"sv;
        
    case Keyword::LibwebPaletteSelectionText:
        return "-libweb-palette-selection-text"sv;
        
    case Keyword::LibwebPaletteSyntaxComment:
        return "-libweb-palette-syntax-comment"sv;
        
    case Keyword::LibwebPaletteSyntaxControlKeyword:
        return "-libweb-palette-syntax-control-keyword"sv;
        
    case Keyword::LibwebPaletteSyntaxIdentifier:
        return "-libweb-palette-syntax-identifier"sv;
        
    case Keyword::LibwebPaletteSyntaxKeyword:
        return "-libweb-palette-syntax-keyword"sv;
        
    case Keyword::LibwebPaletteSyntaxNumber:
        return "-libweb-palette-syntax-number"sv;
        
    case Keyword::LibwebPaletteSyntaxOperator:
        return "-libweb-palette-syntax-operator"sv;
        
    case Keyword::LibwebPaletteSyntaxPreprocessorStatement:
        return "-libweb-palette-syntax-preprocessor-statement"sv;
        
    case Keyword::LibwebPaletteSyntaxPreprocessorValue:
        return "-libweb-palette-syntax-preprocessor-value"sv;
        
    case Keyword::LibwebPaletteSyntaxPunctuation:
        return "-libweb-palette-syntax-punctuation"sv;
        
    case Keyword::LibwebPaletteSyntaxString:
        return "-libweb-palette-syntax-string"sv;
        
    case Keyword::LibwebPaletteSyntaxType:
        return "-libweb-palette-syntax-type"sv;
        
    case Keyword::LibwebPaletteTextCursor:
        return "-libweb-palette-text-cursor"sv;
        
    case Keyword::LibwebPaletteThreedHighlight:
        return "-libweb-palette-threed-highlight"sv;
        
    case Keyword::LibwebPaletteThreedShadow1:
        return "-libweb-palette-threed-shadow1"sv;
        
    case Keyword::LibwebPaletteThreedShadow2:
        return "-libweb-palette-threed-shadow2"sv;
        
    case Keyword::LibwebPaletteVisitedLink:
        return "-libweb-palette-visited-link"sv;
        
    case Keyword::LibwebPaletteWindow:
        return "-libweb-palette-window"sv;
        
    case Keyword::LibwebPaletteWindowText:
        return "-libweb-palette-window-text"sv;
        
    case Keyword::LibwebRight:
        return "-libweb-right"sv;
        
    case Keyword::A3:
        return "a3"sv;
        
    case Keyword::A4:
        return "a4"sv;
        
    case Keyword::A5:
        return "a5"sv;
        
    case Keyword::Absolute:
        return "absolute"sv;
        
    case Keyword::Accentcolor:
        return "accentcolor"sv;
        
    case Keyword::Accentcolortext:
        return "accentcolortext"sv;
        
    case Keyword::Accumulate:
        return "accumulate"sv;
        
    case Keyword::Active:
        return "active"sv;
        
    case Keyword::Activeborder:
        return "activeborder"sv;
        
    case Keyword::Activecaption:
        return "activecaption"sv;
        
    case Keyword::Activetext:
        return "activetext"sv;
        
    case Keyword::Add:
        return "add"sv;
        
    case Keyword::Additive:
        return "additive"sv;
        
    case Keyword::Adlam:
        return "adlam"sv;
        
    case Keyword::Afar:
        return "afar"sv;
        
    case Keyword::Agaw:
        return "agaw"sv;
        
    case Keyword::Alias:
        return "alias"sv;
        
    case Keyword::All:
        return "all"sv;
        
    case Keyword::AllPetiteCaps:
        return "all-petite-caps"sv;
        
    case Keyword::AllScroll:
        return "all-scroll"sv;
        
    case Keyword::AllSmallCaps:
        return "all-small-caps"sv;
        
    case Keyword::AllowDiscrete:
        return "allow-discrete"sv;
        
    case Keyword::Alpha:
        return "alpha"sv;
        
    case Keyword::Alphabetic:
        return "alphabetic"sv;
        
    case Keyword::Alternate:
        return "alternate"sv;
        
    case Keyword::AlternateReverse:
        return "alternate-reverse"sv;
        
    case Keyword::Always:
        return "always"sv;
        
    case Keyword::AnchorsValid:
        return "anchors-valid"sv;
        
    case Keyword::AnchorsVisible:
        return "anchors-visible"sv;
        
    case Keyword::AncientTamil:
        return "ancient-tamil"sv;
        
    case Keyword::Anonymous:
        return "anonymous"sv;
        
    case Keyword::Anywhere:
        return "anywhere"sv;
        
    case Keyword::Appworkspace:
        return "appworkspace"sv;
        
    case Keyword::ArabicAbjad:
        return "arabic-abjad"sv;
        
    case Keyword::ArabicIndic:
        return "arabic-indic"sv;
        
    case Keyword::Ari:
        return "ari"sv;
        
    case Keyword::Armenian:
        return "armenian"sv;
        
    case Keyword::Auto:
        return "auto"sv;
        
    case Keyword::AutoAdd:
        return "auto-add"sv;
        
    case Keyword::Avar2:
        return "avar2"sv;
        
    case Keyword::B4:
        return "b4"sv;
        
    case Keyword::B5:
        return "b5"sv;
        
    case Keyword::Back:
        return "back"sv;
        
    case Keyword::Background:
        return "background"sv;
        
    case Keyword::Backwards:
        return "backwards"sv;
        
    case Keyword::Balance:
        return "balance"sv;
        
    case Keyword::Balinese:
        return "balinese"sv;
        
    case Keyword::Bamum:
        return "bamum"sv;
        
    case Keyword::Bangla:
        return "bangla"sv;
        
    case Keyword::Baseline:
        return "baseline"sv;
        
    case Keyword::Bengali:
        return "bengali"sv;
        
    case Keyword::Bevel:
        return "bevel"sv;
        
    case Keyword::BidiOverride:
        return "bidi-override"sv;
        
    case Keyword::Binary:
        return "binary"sv;
        
    case Keyword::Blin:
        return "blin"sv;
        
    case Keyword::Blink:
        return "blink"sv;
        
    case Keyword::Block:
        return "block"sv;
        
    case Keyword::BlockEnd:
        return "block-end"sv;
        
    case Keyword::BlockStart:
        return "block-start"sv;
        
    case Keyword::Bodo:
        return "bodo"sv;
        
    case Keyword::Bold:
        return "bold"sv;
        
    case Keyword::Bolder:
        return "bolder"sv;
        
    case Keyword::BorderBox:
        return "border-box"sv;
        
    case Keyword::Both:
        return "both"sv;
        
    case Keyword::BothEdges:
        return "both-edges"sv;
        
    case Keyword::Bottom:
        return "bottom"sv;
        
    case Keyword::BreakAll:
        return "break-all"sv;
        
    case Keyword::BreakSpaces:
        return "break-spaces"sv;
        
    case Keyword::BreakWord:
        return "break-word"sv;
        
    case Keyword::Browser:
        return "browser"sv;
        
    case Keyword::Bullets:
        return "bullets"sv;
        
    case Keyword::Butt:
        return "butt"sv;
        
    case Keyword::Button:
        return "button"sv;
        
    case Keyword::Buttonborder:
        return "buttonborder"sv;
        
    case Keyword::Buttonface:
        return "buttonface"sv;
        
    case Keyword::Buttonhighlight:
        return "buttonhighlight"sv;
        
    case Keyword::Buttonshadow:
        return "buttonshadow"sv;
        
    case Keyword::Buttontext:
        return "buttontext"sv;
        
    case Keyword::Cambodian:
        return "cambodian"sv;
        
    case Keyword::Canvas:
        return "canvas"sv;
        
    case Keyword::Canvastext:
        return "canvastext"sv;
        
    case Keyword::Capitalize:
        return "capitalize"sv;
        
    case Keyword::Captiontext:
        return "captiontext"sv;
        
    case Keyword::Cell:
        return "cell"sv;
        
    case Keyword::Center:
        return "center"sv;
        
    case Keyword::Central:
        return "central"sv;
        
    case Keyword::Checkbox:
        return "checkbox"sv;
        
    case Keyword::Circle:
        return "circle"sv;
        
    case Keyword::CircledDecimal:
        return "circled-decimal"sv;
        
    case Keyword::CircledIdeograph:
        return "circled-ideograph"sv;
        
    case Keyword::CircledKatakana:
        return "circled-katakana"sv;
        
    case Keyword::CircledKoreanConsonant:
        return "circled-korean-consonant"sv;
        
    case Keyword::CircledKoreanSyllable:
        return "circled-korean-syllable"sv;
        
    case Keyword::CircledLowerLatin:
        return "circled-lower-latin"sv;
        
    case Keyword::CircledUpperLatin:
        return "circled-upper-latin"sv;
        
    case Keyword::CjkDecimal:
        return "cjk-decimal"sv;
        
    case Keyword::CjkEarthlyBranch:
        return "cjk-earthly-branch"sv;
        
    case Keyword::CjkHeavenlyStem:
        return "cjk-heavenly-stem"sv;
        
    case Keyword::CjkIdeographic:
        return "cjk-ideographic"sv;
        
    case Keyword::CjkStemBranch:
        return "cjk-stem-branch"sv;
        
    case Keyword::CjkTallyMark:
        return "cjk-tally-mark"sv;
        
    case Keyword::Clip:
        return "clip"sv;
        
    case Keyword::CloseQuote:
        return "close-quote"sv;
        
    case Keyword::ClosestCorner:
        return "closest-corner"sv;
        
    case Keyword::ClosestSide:
        return "closest-side"sv;
        
    case Keyword::Coarse:
        return "coarse"sv;
        
    case Keyword::ColResize:
        return "col-resize"sv;
        
    case Keyword::Collapse:
        return "collapse"sv;
        
    case Keyword::Color:
        return "color"sv;
        
    case Keyword::ColorBurn:
        return "color-burn"sv;
        
    case Keyword::ColorCbdt:
        return "color-cbdt"sv;
        
    case Keyword::ColorColrv0:
        return "color-colrv0"sv;
        
    case Keyword::ColorColrv1:
        return "color-colrv1"sv;
        
    case Keyword::ColorDodge:
        return "color-dodge"sv;
        
    case Keyword::ColorSbix:
        return "color-sbix"sv;
        
    case Keyword::ColorSvg:
        return "color-svg"sv;
        
    case Keyword::Column:
        return "column"sv;
        
    case Keyword::ColumnReverse:
        return "column-reverse"sv;
        
    case Keyword::CommonLigatures:
        return "common-ligatures"sv;
        
    case Keyword::Compact:
        return "compact"sv;
        
    case Keyword::Condensed:
        return "condensed"sv;
        
    case Keyword::Contain:
        return "contain"sv;
        
    case Keyword::Content:
        return "content"sv;
        
    case Keyword::ContentBox:
        return "content-box"sv;
        
    case Keyword::Contents:
        return "contents"sv;
        
    case Keyword::ContextMenu:
        return "context-menu"sv;
        
    case Keyword::Contextual:
        return "contextual"sv;
        
    case Keyword::Copy:
        return "copy"sv;
        
    case Keyword::Cover:
        return "cover"sv;
        
    case Keyword::CrispEdges:
        return "crisp-edges"sv;
        
    case Keyword::Crispedges:
        return "crispedges"sv;
        
    case Keyword::Crop:
        return "crop"sv;
        
    case Keyword::Cross:
        return "cross"sv;
        
    case Keyword::Crosshair:
        return "crosshair"sv;
        
    case Keyword::Currentcolor:
        return "currentcolor"sv;
        
    case Keyword::Cursive:
        return "cursive"sv;
        
    case Keyword::Custom:
        return "custom"sv;
        
    case Keyword::Cyclic:
        return "cyclic"sv;
        
    case Keyword::Dark:
        return "dark"sv;
        
    case Keyword::Darken:
        return "darken"sv;
        
    case Keyword::Dashed:
        return "dashed"sv;
        
    case Keyword::Decimal:
        return "decimal"sv;
        
    case Keyword::DecimalLeadingZero:
        return "decimal-leading-zero"sv;
        
    case Keyword::Decreasing:
        return "decreasing"sv;
        
    case Keyword::Default:
        return "default"sv;
        
    case Keyword::Devanagari:
        return "devanagari"sv;
        
    case Keyword::DiagonalFractions:
        return "diagonal-fractions"sv;
        
    case Keyword::Difference:
        return "difference"sv;
        
    case Keyword::Disc:
        return "disc"sv;
        
    case Keyword::Discard:
        return "discard"sv;
        
    case Keyword::DiscardBefore:
        return "discard-before"sv;
        
    case Keyword::DiscardAfter:
        return "discard-after"sv;
        
    case Keyword::DiscardInner:
        return "discard-inner"sv;
        
    case Keyword::DisclosureClosed:
        return "disclosure-closed"sv;
        
    case Keyword::DisclosureOpen:
        return "disclosure-open"sv;
        
    case Keyword::DiscretionaryLigatures:
        return "discretionary-ligatures"sv;
        
    case Keyword::Distribute:
        return "distribute"sv;
        
    case Keyword::Dizi:
        return "dizi"sv;
        
    case Keyword::Dogri:
        return "dogri"sv;
        
    case Keyword::DottedDecimal:
        return "dotted-decimal"sv;
        
    case Keyword::Dotted:
        return "dotted"sv;
        
    case Keyword::DoubleCircledDecimal:
        return "double-circled-decimal"sv;
        
    case Keyword::Double:
        return "double"sv;
        
    case Keyword::Down:
        return "down"sv;
        
    case Keyword::E:
        return "e"sv;
        
    case Keyword::EResize:
        return "e-resize"sv;
        
    case Keyword::EachLine:
        return "each-line"sv;
        
    case Keyword::Ease:
        return "ease"sv;
        
    case Keyword::EaseIn:
        return "ease-in"sv;
        
    case Keyword::EaseInOut:
        return "ease-in-out"sv;
        
    case Keyword::EaseOut:
        return "ease-out"sv;
        
    case Keyword::ElementShared:
        return "element-shared"sv;
        
    case Keyword::Ellipsis:
        return "ellipsis"sv;
        
    case Keyword::Embed:
        return "embed"sv;
        
    case Keyword::Emoji:
        return "emoji"sv;
        
    case Keyword::Enabled:
        return "enabled"sv;
        
    case Keyword::End:
        return "end"sv;
        
    case Keyword::EthiopicHalehameAm:
        return "ethiopic-halehame-am"sv;
        
    case Keyword::EthiopicHalehameTiEr:
        return "ethiopic-halehame-ti-er"sv;
        
    case Keyword::EthiopicHalehameTiEt:
        return "ethiopic-halehame-ti-et"sv;
        
    case Keyword::EthiopicHalehame:
        return "ethiopic-halehame"sv;
        
    case Keyword::EthiopicNumeric:
        return "ethiopic-numeric"sv;
        
    case Keyword::Evenodd:
        return "evenodd"sv;
        
    case Keyword::EwResize:
        return "ew-resize"sv;
        
    case Keyword::Exclude:
        return "exclude"sv;
        
    case Keyword::Exclusion:
        return "exclusion"sv;
        
    case Keyword::Expanded:
        return "expanded"sv;
        
    case Keyword::Extends:
        return "extends"sv;
        
    case Keyword::ExtraCondensed:
        return "extra-condensed"sv;
        
    case Keyword::ExtraExpanded:
        return "extra-expanded"sv;
        
    case Keyword::Fallback:
        return "fallback"sv;
        
    case Keyword::False:
        return "false"sv;
        
    case Keyword::Fantasy:
        return "fantasy"sv;
        
    case Keyword::FarthestCorner:
        return "farthest-corner"sv;
        
    case Keyword::FarthestSide:
        return "farthest-side"sv;
        
    case Keyword::Fast:
        return "fast"sv;
        
    case Keyword::FeaturesAat:
        return "features-aat"sv;
        
    case Keyword::FeaturesGraphite:
        return "features-graphite"sv;
        
    case Keyword::FeaturesOpentype:
        return "features-opentype"sv;
        
    case Keyword::Field:
        return "field"sv;
        
    case Keyword::Fieldtext:
        return "fieldtext"sv;
        
    case Keyword::Fill:
        return "fill"sv;
        
    case Keyword::FillBox:
        return "fill-box"sv;
        
    case Keyword::FilledCircledDecimal:
        return "filled-circled-decimal"sv;
        
    case Keyword::Fine:
        return "fine"sv;
        
    case Keyword::Fixed:
        return "fixed"sv;
        
    case Keyword::Flat:
        return "flat"sv;
        
    case Keyword::Flex:
        return "flex"sv;
        
    case Keyword::FlexEnd:
        return "flex-end"sv;
        
    case Keyword::FlexStart:
        return "flex-start"sv;
        
    case Keyword::FlipBlock:
        return "flip-block"sv;
        
    case Keyword::FlipInline:
        return "flip-inline"sv;
        
    case Keyword::FlipStart:
        return "flip-start"sv;
        
    case Keyword::Flow:
        return "flow"sv;
        
    case Keyword::FlowRoot:
        return "flow-root"sv;
        
    case Keyword::Forwards:
        return "forwards"sv;
        
    case Keyword::FromFont:
        return "from-font"sv;
        
    case Keyword::FullSizeKana:
        return "full-size-kana"sv;
        
    case Keyword::FullWidth:
        return "full-width"sv;
        
    case Keyword::FullwidthDecimal:
        return "fullwidth-decimal"sv;
        
    case Keyword::FullwidthLowerAlpha:
        return "fullwidth-lower-alpha"sv;
        
    case Keyword::FullwidthUpperAlpha:
        return "fullwidth-upper-alpha"sv;
        
    case Keyword::FullwidthUpperRoman:
        return "fullwidth-upper-roman"sv;
        
    case Keyword::Fullscreen:
        return "fullscreen"sv;
        
    case Keyword::Gedeo:
        return "gedeo"sv;
        
    case Keyword::Geometricprecision:
        return "geometricprecision"sv;
        
    case Keyword::Georgian:
        return "georgian"sv;
        
    case Keyword::Grab:
        return "grab"sv;
        
    case Keyword::Grabbing:
        return "grabbing"sv;
        
    case Keyword::GrammarError:
        return "grammar-error"sv;
        
    case Keyword::Graytext:
        return "graytext"sv;
        
    case Keyword::GreekLowerAncient:
        return "greek-lower-ancient"sv;
        
    case Keyword::GreekLowerModern:
        return "greek-lower-modern"sv;
        
    case Keyword::GreekUpperAncient:
        return "greek-upper-ancient"sv;
        
    case Keyword::GreekUpperModern:
        return "greek-upper-modern"sv;
        
    case Keyword::Grid:
        return "grid"sv;
        
    case Keyword::Groove:
        return "groove"sv;
        
    case Keyword::GujaratiAlpha:
        return "gujarati-alpha"sv;
        
    case Keyword::Gujarati:
        return "gujarati"sv;
        
    case Keyword::Gumuz:
        return "gumuz"sv;
        
    case Keyword::Gurmukhi:
        return "gurmukhi"sv;
        
    case Keyword::Hadiyaa:
        return "hadiyaa"sv;
        
    case Keyword::Hanging:
        return "hanging"sv;
        
    case Keyword::HangulConsonant:
        return "hangul-consonant"sv;
        
    case Keyword::Hangul:
        return "hangul"sv;
        
    case Keyword::HanifiRohingya:
        return "hanifi-rohingya"sv;
        
    case Keyword::Harari:
        return "harari"sv;
        
    case Keyword::HardLight:
        return "hard-light"sv;
        
    case Keyword::Hebrew:
        return "hebrew"sv;
        
    case Keyword::Height:
        return "height"sv;
        
    case Keyword::Help:
        return "help"sv;
        
    case Keyword::Hide:
        return "hide"sv;
        
    case Keyword::Hidden:
        return "hidden"sv;
        
    case Keyword::High:
        return "high"sv;
        
    case Keyword::HighQuality:
        return "high-quality"sv;
        
    case Keyword::Highlight:
        return "highlight"sv;
        
    case Keyword::Highlighttext:
        return "highlighttext"sv;
        
    case Keyword::Hindi:
        return "hindi"sv;
        
    case Keyword::HiraganaIroha:
        return "hiragana-iroha"sv;
        
    case Keyword::Hiragana:
        return "hiragana"sv;
        
    case Keyword::HistoricalForms:
        return "historical-forms"sv;
        
    case Keyword::HistoricalLigatures:
        return "historical-ligatures"sv;
        
    case Keyword::Horizontal:
        return "horizontal"sv;
        
    case Keyword::HorizontalTb:
        return "horizontal-tb"sv;
        
    case Keyword::Hover:
        return "hover"sv;
        
    case Keyword::Hsl:
        return "hsl"sv;
        
    case Keyword::Hue:
        return "hue"sv;
        
    case Keyword::Hwb:
        return "hwb"sv;
        
    case Keyword::Inactiveborder:
        return "inactiveborder"sv;
        
    case Keyword::Inactivecaption:
        return "inactivecaption"sv;
        
    case Keyword::Inactivecaptiontext:
        return "inactivecaptiontext"sv;
        
    case Keyword::Increasing:
        return "increasing"sv;
        
    case Keyword::Ideographic:
        return "ideographic"sv;
        
    case Keyword::Incremental:
        return "incremental"sv;
        
    case Keyword::Infinite:
        return "infinite"sv;
        
    case Keyword::Infinity:
        return "infinity"sv;
        
    case Keyword::Infobackground:
        return "infobackground"sv;
        
    case Keyword::Infotext:
        return "infotext"sv;
        
    case Keyword::Inherit:
        return "inherit"sv;
        
    case Keyword::Initial:
        return "initial"sv;
        
    case Keyword::InitialOnly:
        return "initial-only"sv;
        
    case Keyword::Inline:
        return "inline"sv;
        
    case Keyword::InlineBlock:
        return "inline-block"sv;
        
    case Keyword::InlineEnd:
        return "inline-end"sv;
        
    case Keyword::InlineFlex:
        return "inline-flex"sv;
        
    case Keyword::InlineGrid:
        return "inline-grid"sv;
        
    case Keyword::InlineSize:
        return "inline-size"sv;
        
    case Keyword::InlineStart:
        return "inline-start"sv;
        
    case Keyword::InlineTable:
        return "inline-table"sv;
        
    case Keyword::Inset:
        return "inset"sv;
        
    case Keyword::Inside:
        return "inside"sv;
        
    case Keyword::InterCharacter:
        return "inter-character"sv;
        
    case Keyword::InterWord:
        return "inter-word"sv;
        
    case Keyword::Interlace:
        return "interlace"sv;
        
    case Keyword::Intersect:
        return "intersect"sv;
        
    case Keyword::Invert:
        return "invert"sv;
        
    case Keyword::Inverted:
        return "inverted"sv;
        
    case Keyword::Isolate:
        return "isolate"sv;
        
    case Keyword::IsolateOverride:
        return "isolate-override"sv;
        
    case Keyword::Italic:
        return "italic"sv;
        
    case Keyword::JapaneseFormal:
        return "japanese-formal"sv;
        
    case Keyword::JapaneseInformal:
        return "japanese-informal"sv;
        
    case Keyword::Javanese:
        return "javanese"sv;
        
    case Keyword::JisB4:
        return "jis-b4"sv;
        
    case Keyword::JisB5:
        return "jis-b5"sv;
        
    case Keyword::Jis04:
        return "jis04"sv;
        
    case Keyword::Jis78:
        return "jis78"sv;
        
    case Keyword::Jis83:
        return "jis83"sv;
        
    case Keyword::Jis90:
        return "jis90"sv;
        
    case Keyword::JumpBoth:
        return "jump-both"sv;
        
    case Keyword::JumpEnd:
        return "jump-end"sv;
        
    case Keyword::JumpNone:
        return "jump-none"sv;
        
    case Keyword::JumpStart:
        return "jump-start"sv;
        
    case Keyword::Justify:
        return "justify"sv;
        
    case Keyword::Kaffa:
        return "kaffa"sv;
        
    case Keyword::KannadaAlpha:
        return "kannada-alpha"sv;
        
    case Keyword::Kannada:
        return "kannada"sv;
        
    case Keyword::Kashmiri:
        return "kashmiri"sv;
        
    case Keyword::KatakanaIroha:
        return "katakana-iroha"sv;
        
    case Keyword::Katakana:
        return "katakana"sv;
        
    case Keyword::KayahLi:
        return "kayah-li"sv;
        
    case Keyword::Kebena:
        return "kebena"sv;
        
    case Keyword::KeepAll:
        return "keep-all"sv;
        
    case Keyword::Kembata:
        return "kembata"sv;
        
    case Keyword::KhmerConsonant:
        return "khmer-consonant"sv;
        
    case Keyword::Khmer:
        return "khmer"sv;
        
    case Keyword::Konkani:
        return "konkani"sv;
        
    case Keyword::Konso:
        return "konso"sv;
        
    case Keyword::KoreanConsonant:
        return "korean-consonant"sv;
        
    case Keyword::KoreanHangulFormal:
        return "korean-hangul-formal"sv;
        
    case Keyword::KoreanHanjaFormal:
        return "korean-hanja-formal"sv;
        
    case Keyword::KoreanHanjaInformal:
        return "korean-hanja-informal"sv;
        
    case Keyword::KoreanSyllable:
        return "korean-syllable"sv;
        
    case Keyword::Kunama:
        return "kunama"sv;
        
    case Keyword::Landscape:
        return "landscape"sv;
        
    case Keyword::LannaHora:
        return "lanna-hora"sv;
        
    case Keyword::LannaTham:
        return "lanna-tham"sv;
        
    case Keyword::Lao:
        return "lao"sv;
        
    case Keyword::Large:
        return "large"sv;
        
    case Keyword::Larger:
        return "larger"sv;
        
    case Keyword::Layout:
        return "layout"sv;
        
    case Keyword::Lch:
        return "lch"sv;
        
    case Keyword::Ledger:
        return "ledger"sv;
        
    case Keyword::Left:
        return "left"sv;
        
    case Keyword::Legacy:
        return "legacy"sv;
        
    case Keyword::Legal:
        return "legal"sv;
        
    case Keyword::Lepcha:
        return "lepcha"sv;
        
    case Keyword::Less:
        return "less"sv;
        
    case Keyword::Letter:
        return "letter"sv;
        
    case Keyword::Light:
        return "light"sv;
        
    case Keyword::Lighten:
        return "lighten"sv;
        
    case Keyword::Lighter:
        return "lighter"sv;
        
    case Keyword::Limbu:
        return "limbu"sv;
        
    case Keyword::LineThrough:
        return "line-through"sv;
        
    case Keyword::Linear:
        return "linear"sv;
        
    case Keyword::Linearrgb:
        return "linearrgb"sv;
        
    case Keyword::LiningNums:
        return "lining-nums"sv;
        
    case Keyword::Linktext:
        return "linktext"sv;
        
    case Keyword::ListItem:
        return "list-item"sv;
        
    case Keyword::Listbox:
        return "listbox"sv;
        
    case Keyword::Local:
        return "local"sv;
        
    case Keyword::Longer:
        return "longer"sv;
        
    case Keyword::LowerAlphaSymbolic:
        return "lower-alpha-symbolic"sv;
        
    case Keyword::LowerAlpha:
        return "lower-alpha"sv;
        
    case Keyword::LowerArmenian:
        return "lower-armenian"sv;
        
    case Keyword::LowerBelorussian:
        return "lower-belorussian"sv;
        
    case Keyword::LowerBulgarian:
        return "lower-bulgarian"sv;
        
    case Keyword::LowerGreek:
        return "lower-greek"sv;
        
    case Keyword::LowerHexadecimal:
        return "lower-hexadecimal"sv;
        
    case Keyword::LowerLatin:
        return "lower-latin"sv;
        
    case Keyword::LowerMacedonian:
        return "lower-macedonian"sv;
        
    case Keyword::LowerOromoQubee:
        return "lower-oromo-qubee"sv;
        
    case Keyword::LowerRoman:
        return "lower-roman"sv;
        
    case Keyword::LowerRussianFull:
        return "lower-russian-full"sv;
        
    case Keyword::LowerRussian:
        return "lower-russian"sv;
        
    case Keyword::LowerSerbian:
        return "lower-serbian"sv;
        
    case Keyword::LowerUkrainianFull:
        return "lower-ukrainian-full"sv;
        
    case Keyword::LowerUkrainian:
        return "lower-ukrainian"sv;
        
    case Keyword::Lowercase:
        return "lowercase"sv;
        
    case Keyword::Ltr:
        return "ltr"sv;
        
    case Keyword::Luminance:
        return "luminance"sv;
        
    case Keyword::Luminosity:
        return "luminosity"sv;
        
    case Keyword::MaghrebiAbjad:
        return "maghrebi-abjad"sv;
        
    case Keyword::Maithili:
        return "maithili"sv;
        
    case Keyword::MalayalamAlpha:
        return "malayalam-alpha"sv;
        
    case Keyword::Malayalam:
        return "malayalam"sv;
        
    case Keyword::Manipulation:
        return "manipulation"sv;
        
    case Keyword::Manipuri:
        return "manipuri"sv;
        
    case Keyword::Marathi:
        return "marathi"sv;
        
    case Keyword::MarginBox:
        return "margin-box"sv;
        
    case Keyword::Mark:
        return "mark"sv;
        
    case Keyword::Markers:
        return "markers"sv;
        
    case Keyword::Marktext:
        return "marktext"sv;
        
    case Keyword::MatchParent:
        return "match-parent"sv;
        
    case Keyword::MatchSource:
        return "match-source"sv;
        
    case Keyword::Math:
        return "math"sv;
        
    case Keyword::MathAuto:
        return "math-auto"sv;
        
    case Keyword::Mathematical:
        return "mathematical"sv;
        
    case Keyword::MaxContent:
        return "max-content"sv;
        
    case Keyword::Medium:
        return "medium"sv;
        
    case Keyword::Meen:
        return "meen"sv;
        
    case Keyword::Meetei:
        return "meetei"sv;
        
    case Keyword::Menu:
        return "menu"sv;
        
    case Keyword::Menulist:
        return "menulist"sv;
        
    case Keyword::MenulistButton:
        return "menulist-button"sv;
        
    case Keyword::Menutext:
        return "menutext"sv;
        
    case Keyword::Meter:
        return "meter"sv;
        
    case Keyword::Middle:
        return "middle"sv;
        
    case Keyword::MinContent:
        return "min-content"sv;
        
    case Keyword::MinimalUi:
        return "minimal-ui"sv;
        
    case Keyword::Miter:
        return "miter"sv;
        
    case Keyword::Mongolian:
        return "mongolian"sv;
        
    case Keyword::Monospace:
        return "monospace"sv;
        
    case Keyword::More:
        return "more"sv;
        
    case Keyword::MostBlockSize:
        return "most-block-size"sv;
        
    case Keyword::MostHeight:
        return "most-height"sv;
        
    case Keyword::MostInlineSize:
        return "most-inline-size"sv;
        
    case Keyword::MostWidth:
        return "most-width"sv;
        
    case Keyword::Move:
        return "move"sv;
        
    case Keyword::Mro:
        return "mro"sv;
        
    case Keyword::Multiply:
        return "multiply"sv;
        
    case Keyword::Myanmar:
        return "myanmar"sv;
        
    case Keyword::NResize:
        return "n-resize"sv;
        
    case Keyword::NagMundari:
        return "nag-mundari"sv;
        
    case Keyword::Nan:
        return "nan"sv;
        
    case Keyword::NeResize:
        return "ne-resize"sv;
        
    case Keyword::Nearest:
        return "nearest"sv;
        
    case Keyword::NeswResize:
        return "nesw-resize"sv;
        
    case Keyword::NewBase60:
        return "new-base-60"sv;
        
    case Keyword::Newa:
        return "newa"sv;
        
    case Keyword::NkoCardinal:
        return "nko-cardinal"sv;
        
    case Keyword::NoClip:
        return "no-clip"sv;
        
    case Keyword::NoCloseQuote:
        return "no-close-quote"sv;
        
    case Keyword::NoCommonLigatures:
        return "no-common-ligatures"sv;
        
    case Keyword::NoContextual:
        return "no-contextual"sv;
        
    case Keyword::NoDiscretionaryLigatures:
        return "no-discretionary-ligatures"sv;
        
    case Keyword::NoDrop:
        return "no-drop"sv;
        
    case Keyword::NoHistoricalLigatures:
        return "no-historical-ligatures"sv;
        
    case Keyword::NoOpenQuote:
        return "no-open-quote"sv;
        
    case Keyword::NoOverflow:
        return "no-overflow"sv;
        
    case Keyword::NoPreference:
        return "no-preference"sv;
        
    case Keyword::NoReferrer:
        return "no-referrer"sv;
        
    case Keyword::NoReferrerWhenDowngrade:
        return "no-referrer-when-downgrade"sv;
        
    case Keyword::NoRepeat:
        return "no-repeat"sv;
        
    case Keyword::None:
        return "none"sv;
        
    case Keyword::Nonzero:
        return "nonzero"sv;
        
    case Keyword::Normal:
        return "normal"sv;
        
    case Keyword::NotAllowed:
        return "not-allowed"sv;
        
    case Keyword::Notch:
        return "notch"sv;
        
    case Keyword::Nowrap:
        return "nowrap"sv;
        
    case Keyword::NsResize:
        return "ns-resize"sv;
        
    case Keyword::Numbers:
        return "numbers"sv;
        
    case Keyword::Numeric:
        return "numeric"sv;
        
    case Keyword::NwResize:
        return "nw-resize"sv;
        
    case Keyword::NwseResize:
        return "nwse-resize"sv;
        
    case Keyword::Oblique:
        return "oblique"sv;
        
    case Keyword::Octal:
        return "octal"sv;
        
    case Keyword::Odia:
        return "odia"sv;
        
    case Keyword::Off:
        return "off"sv;
        
    case Keyword::Oklch:
        return "oklch"sv;
        
    case Keyword::OlChiki:
        return "ol-chiki"sv;
        
    case Keyword::OldstyleNums:
        return "oldstyle-nums"sv;
        
    case Keyword::On:
        return "on"sv;
        
    case Keyword::Opaque:
        return "opaque"sv;
        
    case Keyword::OpenQuote:
        return "open-quote"sv;
        
    case Keyword::Optimizelegibility:
        return "optimizelegibility"sv;
        
    case Keyword::Optimizequality:
        return "optimizequality"sv;
        
    case Keyword::Optimizespeed:
        return "optimizespeed"sv;
        
    case Keyword::Optional:
        return "optional"sv;
        
    case Keyword::Ordinal:
        return "ordinal"sv;
        
    case Keyword::Origin:
        return "origin"sv;
        
    case Keyword::OriginWhenCrossOrigin:
        return "origin-when-cross-origin"sv;
        
    case Keyword::Oriya:
        return "oriya"sv;
        
    case Keyword::Oromo:
        return "oromo"sv;
        
    case Keyword::Outset:
        return "outset"sv;
        
    case Keyword::Outside:
        return "outside"sv;
        
    case Keyword::Overlay:
        return "overlay"sv;
        
    case Keyword::Overline:
        return "overline"sv;
        
    case Keyword::P3:
        return "p3"sv;
        
    case Keyword::PaddingBox:
        return "padding-box"sv;
        
    case Keyword::Paged:
        return "paged"sv;
        
    case Keyword::Paint:
        return "paint"sv;
        
    case Keyword::Palettes:
        return "palettes"sv;
        
    case Keyword::PanDown:
        return "pan-down"sv;
        
    case Keyword::PanLeft:
        return "pan-left"sv;
        
    case Keyword::PanRight:
        return "pan-right"sv;
        
    case Keyword::PanUp:
        return "pan-up"sv;
        
    case Keyword::PanX:
        return "pan-x"sv;
        
    case Keyword::PanY:
        return "pan-y"sv;
        
    case Keyword::ParenthesizedDecimal:
        return "parenthesized-decimal"sv;
        
    case Keyword::ParenthesizedHangulConsonant:
        return "parenthesized-hangul-consonant"sv;
        
    case Keyword::ParenthesizedHangulSyllable:
        return "parenthesized-hangul-syllable"sv;
        
    case Keyword::ParenthesizedIdeograph:
        return "parenthesized-ideograph"sv;
        
    case Keyword::ParenthesizedLowerLatin:
        return "parenthesized-lower-latin"sv;
        
    case Keyword::Paused:
        return "paused"sv;
        
    case Keyword::PersianAbjad:
        return "persian-abjad"sv;
        
    case Keyword::PersianAlphabetic:
        return "persian-alphabetic"sv;
        
    case Keyword::Persian:
        return "persian"sv;
        
    case Keyword::PetiteCaps:
        return "petite-caps"sv;
        
    case Keyword::Pi:
        return "pi"sv;
        
    case Keyword::PictureInPicture:
        return "picture-in-picture"sv;
        
    case Keyword::Pixelated:
        return "pixelated"sv;
        
    case Keyword::Plaintext:
        return "plaintext"sv;
        
    case Keyword::PlusDarker:
        return "plus-darker"sv;
        
    case Keyword::PlusLighter:
        return "plus-lighter"sv;
        
    case Keyword::Pointer:
        return "pointer"sv;
        
    case Keyword::Portrait:
        return "portrait"sv;
        
    case Keyword::Pre:
        return "pre"sv;
        
    case Keyword::PreLine:
        return "pre-line"sv;
        
    case Keyword::PreWrap:
        return "pre-wrap"sv;
        
    case Keyword::Preserve:
        return "preserve"sv;
        
    case Keyword::Preserve3d:
        return "preserve-3d"sv;
        
    case Keyword::PreserveBreaks:
        return "preserve-breaks"sv;
        
    case Keyword::PreserveSpaces:
        return "preserve-spaces"sv;
        
    case Keyword::Pretty:
        return "pretty"sv;
        
    case Keyword::Progress:
        return "progress"sv;
        
    case Keyword::ProgressBar:
        return "progress-bar"sv;
        
    case Keyword::Progressive:
        return "progressive"sv;
        
    case Keyword::ProportionalNums:
        return "proportional-nums"sv;
        
    case Keyword::ProportionalWidth:
        return "proportional-width"sv;
        
    case Keyword::Punjabi:
        return "punjabi"sv;
        
    case Keyword::PushButton:
        return "push-button"sv;
        
    case Keyword::Radio:
        return "radio"sv;
        
    case Keyword::Rec2020:
        return "rec2020"sv;
        
    case Keyword::Reduce:
        return "reduce"sv;
        
    case Keyword::Relative:
        return "relative"sv;
        
    case Keyword::Repeat:
        return "repeat"sv;
        
    case Keyword::RepeatX:
        return "repeat-x"sv;
        
    case Keyword::RepeatY:
        return "repeat-y"sv;
        
    case Keyword::Replace:
        return "replace"sv;
        
    case Keyword::Reverse:
        return "reverse"sv;
        
    case Keyword::Revert:
        return "revert"sv;
        
    case Keyword::RevertLayer:
        return "revert-layer"sv;
        
    case Keyword::Ridge:
        return "ridge"sv;
        
    case Keyword::Right:
        return "right"sv;
        
    case Keyword::Root:
        return "root"sv;
        
    case Keyword::RotateLeft:
        return "rotate-left"sv;
        
    case Keyword::RotateRight:
        return "rotate-right"sv;
        
    case Keyword::Round:
        return "round"sv;
        
    case Keyword::Row:
        return "row"sv;
        
    case Keyword::RowResize:
        return "row-resize"sv;
        
    case Keyword::RowReverse:
        return "row-reverse"sv;
        
    case Keyword::Rtl:
        return "rtl"sv;
        
    case Keyword::Ruby:
        return "ruby"sv;
        
    case Keyword::RubyBase:
        return "ruby-base"sv;
        
    case Keyword::RubyBaseContainer:
        return "ruby-base-container"sv;
        
    case Keyword::RubyText:
        return "ruby-text"sv;
        
    case Keyword::RubyTextContainer:
        return "ruby-text-container"sv;
        
    case Keyword::RunIn:
        return "run-in"sv;
        
    case Keyword::Running:
        return "running"sv;
        
    case Keyword::SResize:
        return "s-resize"sv;
        
    case Keyword::Safe:
        return "safe"sv;
        
    case Keyword::Saho:
        return "saho"sv;
        
    case Keyword::SameOrigin:
        return "same-origin"sv;
        
    case Keyword::SansSerif:
        return "sans-serif"sv;
        
    case Keyword::Sanskrit:
        return "sanskrit"sv;
        
    case Keyword::Santali:
        return "santali"sv;
        
    case Keyword::Saturation:
        return "saturation"sv;
        
    case Keyword::ScaleDown:
        return "scale-down"sv;
        
    case Keyword::Scoop:
        return "scoop"sv;
        
    case Keyword::Screen:
        return "screen"sv;
        
    case Keyword::Scroll:
        return "scroll"sv;
        
    case Keyword::ScrollPosition:
        return "scroll-position"sv;
        
    case Keyword::ScrollState:
        return "scroll-state"sv;
        
    case Keyword::Scrollbar:
        return "scrollbar"sv;
        
    case Keyword::SeResize:
        return "se-resize"sv;
        
    case Keyword::Searchfield:
        return "searchfield"sv;
        
    case Keyword::Selecteditem:
        return "selecteditem"sv;
        
    case Keyword::Selecteditemtext:
        return "selecteditemtext"sv;
        
    case Keyword::Self:
        return "self"sv;
        
    case Keyword::SelfBlock:
        return "self-block"sv;
        
    case Keyword::SelfBlockEnd:
        return "self-block-end"sv;
        
    case Keyword::SelfBlockStart:
        return "self-block-start"sv;
        
    case Keyword::SelfEnd:
        return "self-end"sv;
        
    case Keyword::SelfInline:
        return "self-inline"sv;
        
    case Keyword::SelfInlineEnd:
        return "self-inline-end"sv;
        
    case Keyword::SelfInlineStart:
        return "self-inline-start"sv;
        
    case Keyword::SelfXEnd:
        return "self-x-end"sv;
        
    case Keyword::SelfXStart:
        return "self-x-start"sv;
        
    case Keyword::SelfYEnd:
        return "self-y-end"sv;
        
    case Keyword::SelfYStart:
        return "self-y-start"sv;
        
    case Keyword::SelfStart:
        return "self-start"sv;
        
    case Keyword::SemiCondensed:
        return "semi-condensed"sv;
        
    case Keyword::SemiExpanded:
        return "semi-expanded"sv;
        
    case Keyword::Separate:
        return "separate"sv;
        
    case Keyword::Serif:
        return "serif"sv;
        
    case Keyword::Shan:
        return "shan"sv;
        
    case Keyword::Shorter:
        return "shorter"sv;
        
    case Keyword::Show:
        return "show"sv;
        
    case Keyword::Sidama:
        return "sidama"sv;
        
    case Keyword::SidewaysLr:
        return "sideways-lr"sv;
        
    case Keyword::SidewaysRl:
        return "sideways-rl"sv;
        
    case Keyword::Silti:
        return "silti"sv;
        
    case Keyword::SimpChineseFormal:
        return "simp-chinese-formal"sv;
        
    case Keyword::SimpChineseInformal:
        return "simp-chinese-informal"sv;
        
    case Keyword::SimpleLowerRoman:
        return "simple-lower-roman"sv;
        
    case Keyword::SimpleUpperRoman:
        return "simple-upper-roman"sv;
        
    case Keyword::Simplified:
        return "simplified"sv;
        
    case Keyword::Sindhi:
        return "sindhi"sv;
        
    case Keyword::Size:
        return "size"sv;
        
    case Keyword::SlashedZero:
        return "slashed-zero"sv;
        
    case Keyword::SliderHorizontal:
        return "slider-horizontal"sv;
        
    case Keyword::Slow:
        return "slow"sv;
        
    case Keyword::Small:
        return "small"sv;
        
    case Keyword::SmallCaps:
        return "small-caps"sv;
        
    case Keyword::Smaller:
        return "smaller"sv;
        
    case Keyword::Smooth:
        return "smooth"sv;
        
    case Keyword::SoftLight:
        return "soft-light"sv;
        
    case Keyword::Solid:
        return "solid"sv;
        
    case Keyword::Space:
        return "space"sv;
        
    case Keyword::SpaceAround:
        return "space-around"sv;
        
    case Keyword::SpaceBetween:
        return "space-between"sv;
        
    case Keyword::SpaceEvenly:
        return "space-evenly"sv;
        
    case Keyword::SpanAll:
        return "span-all"sv;
        
    case Keyword::SpanBlockEnd:
        return "span-block-end"sv;
        
    case Keyword::SpanBlockStart:
        return "span-block-start"sv;
        
    case Keyword::SpanBottom:
        return "span-bottom"sv;
        
    case Keyword::SpanEnd:
        return "span-end"sv;
        
    case Keyword::SpanInlineEnd:
        return "span-inline-end"sv;
        
    case Keyword::SpanInlineStart:
        return "span-inline-start"sv;
        
    case Keyword::SpanLeft:
        return "span-left"sv;
        
    case Keyword::SpanRight:
        return "span-right"sv;
        
    case Keyword::SpanSelfBlockEnd:
        return "span-self-block-end"sv;
        
    case Keyword::SpanSelfBlockStart:
        return "span-self-block-start"sv;
        
    case Keyword::SpanSelfEnd:
        return "span-self-end"sv;
        
    case Keyword::SpanSelfInlineEnd:
        return "span-self-inline-end"sv;
        
    case Keyword::SpanSelfInlineStart:
        return "span-self-inline-start"sv;
        
    case Keyword::SpanSelfStart:
        return "span-self-start"sv;
        
    case Keyword::SpanSelfXEnd:
        return "span-self-x-end"sv;
        
    case Keyword::SpanSelfXStart:
        return "span-self-x-start"sv;
        
    case Keyword::SpanSelfYEnd:
        return "span-self-y-end"sv;
        
    case Keyword::SpanSelfYStart:
        return "span-self-y-start"sv;
        
    case Keyword::SpanStart:
        return "span-start"sv;
        
    case Keyword::SpanTop:
        return "span-top"sv;
        
    case Keyword::SpanXEnd:
        return "span-x-end"sv;
        
    case Keyword::SpanXStart:
        return "span-x-start"sv;
        
    case Keyword::SpanYEnd:
        return "span-y-end"sv;
        
    case Keyword::SpanYStart:
        return "span-y-start"sv;
        
    case Keyword::SpellingError:
        return "spelling-error"sv;
        
    case Keyword::SpellOut:
        return "spell-out"sv;
        
    case Keyword::Square:
        return "square"sv;
        
    case Keyword::SquareButton:
        return "square-button"sv;
        
    case Keyword::Squircle:
        return "squircle"sv;
        
    case Keyword::Srgb:
        return "srgb"sv;
        
    case Keyword::Stable:
        return "stable"sv;
        
    case Keyword::StackedFractions:
        return "stacked-fractions"sv;
        
    case Keyword::Standalone:
        return "standalone"sv;
        
    case Keyword::Standard:
        return "standard"sv;
        
    case Keyword::Start:
        return "start"sv;
        
    case Keyword::Static:
        return "static"sv;
        
    case Keyword::Sticky:
        return "sticky"sv;
        
    case Keyword::Stretch:
        return "stretch"sv;
        
    case Keyword::Strict:
        return "strict"sv;
        
    case Keyword::StrictOrigin:
        return "strict-origin"sv;
        
    case Keyword::StrictOriginWhenCrossOrigin:
        return "strict-origin-when-cross-origin"sv;
        
    case Keyword::Stroke:
        return "stroke"sv;
        
    case Keyword::StrokeBox:
        return "stroke-box"sv;
        
    case Keyword::Style:
        return "style"sv;
        
    case Keyword::Sub:
        return "sub"sv;
        
    case Keyword::Subtract:
        return "subtract"sv;
        
    case Keyword::Subtractive:
        return "subtractive"sv;
        
    case Keyword::Sundanese:
        return "sundanese"sv;
        
    case Keyword::SuperDecimal:
        return "super-decimal"sv;
        
    case Keyword::Super:
        return "super"sv;
        
    case Keyword::SwResize:
        return "sw-resize"sv;
        
    case Keyword::Swap:
        return "swap"sv;
        
    case Keyword::Symbolic:
        return "symbolic"sv;
        
    case Keyword::Table:
        return "table"sv;
        
    case Keyword::TableCaption:
        return "table-caption"sv;
        
    case Keyword::TableCell:
        return "table-cell"sv;
        
    case Keyword::TableColumn:
        return "table-column"sv;
        
    case Keyword::TableColumnGroup:
        return "table-column-group"sv;
        
    case Keyword::TableFooterGroup:
        return "table-footer-group"sv;
        
    case Keyword::TableHeaderGroup:
        return "table-header-group"sv;
        
    case Keyword::TableRow:
        return "table-row"sv;
        
    case Keyword::TableRowGroup:
        return "table-row-group"sv;
        
    case Keyword::TabularNums:
        return "tabular-nums"sv;
        
    case Keyword::TaiLue:
        return "tai-lue"sv;
        
    case Keyword::TallyMark:
        return "tally-mark"sv;
        
    case Keyword::Tamil:
        return "tamil"sv;
        
    case Keyword::TeluguAlpha:
        return "telugu-alpha"sv;
        
    case Keyword::Telugu:
        return "telugu"sv;
        
    case Keyword::Text:
        return "text"sv;
        
    case Keyword::TextBottom:
        return "text-bottom"sv;
        
    case Keyword::TextTop:
        return "text-top"sv;
        
    case Keyword::Textarea:
        return "textarea"sv;
        
    case Keyword::Textfield:
        return "textfield"sv;
        
    case Keyword::ThaiAlpha:
        return "thai-alpha"sv;
        
    case Keyword::Thai:
        return "thai"sv;
        
    case Keyword::Thick:
        return "thick"sv;
        
    case Keyword::Thin:
        return "thin"sv;
        
    case Keyword::Threeddarkshadow:
        return "threeddarkshadow"sv;
        
    case Keyword::Threedface:
        return "threedface"sv;
        
    case Keyword::Threedhighlight:
        return "threedhighlight"sv;
        
    case Keyword::Threedlightshadow:
        return "threedlightshadow"sv;
        
    case Keyword::Threedshadow:
        return "threedshadow"sv;
        
    case Keyword::Tibetan:
        return "tibetan"sv;
        
    case Keyword::Tigre:
        return "tigre"sv;
        
    case Keyword::TitlingCaps:
        return "titling-caps"sv;
        
    case Keyword::ToZero:
        return "to-zero"sv;
        
    case Keyword::Top:
        return "top"sv;
        
    case Keyword::TradChineseFormal:
        return "trad-chinese-formal"sv;
        
    case Keyword::TradChineseInformal:
        return "trad-chinese-informal"sv;
        
    case Keyword::Traditional:
        return "traditional"sv;
        
    case Keyword::True:
        return "true"sv;
        
    case Keyword::UiMonospace:
        return "ui-monospace"sv;
        
    case Keyword::UiRounded:
        return "ui-rounded"sv;
        
    case Keyword::UiSansSerif:
        return "ui-sans-serif"sv;
        
    case Keyword::UiSerif:
        return "ui-serif"sv;
        
    case Keyword::UltraCondensed:
        return "ultra-condensed"sv;
        
    case Keyword::UltraExpanded:
        return "ultra-expanded"sv;
        
    case Keyword::Under:
        return "under"sv;
        
    case Keyword::Underline:
        return "underline"sv;
        
    case Keyword::Unicase:
        return "unicase"sv;
        
    case Keyword::Unicode:
        return "unicode"sv;
        
    case Keyword::Unsafe:
        return "unsafe"sv;
        
    case Keyword::UnsafeUrl:
        return "unsafe-url"sv;
        
    case Keyword::Unset:
        return "unset"sv;
        
    case Keyword::Up:
        return "up"sv;
        
    case Keyword::UpperAlphaSymbolic:
        return "upper-alpha-symbolic"sv;
        
    case Keyword::UpperAlpha:
        return "upper-alpha"sv;
        
    case Keyword::UpperArmenian:
        return "upper-armenian"sv;
        
    case Keyword::UpperBelorussian:
        return "upper-belorussian"sv;
        
    case Keyword::UpperBulgarian:
        return "upper-bulgarian"sv;
        
    case Keyword::UpperHexadecimal:
        return "upper-hexadecimal"sv;
        
    case Keyword::UpperLatin:
        return "upper-latin"sv;
        
    case Keyword::UpperMacedonian:
        return "upper-macedonian"sv;
        
    case Keyword::UpperOromoQubee:
        return "upper-oromo-qubee"sv;
        
    case Keyword::UpperRoman:
        return "upper-roman"sv;
        
    case Keyword::UpperRussianFull:
        return "upper-russian-full"sv;
        
    case Keyword::UpperRussian:
        return "upper-russian"sv;
        
    case Keyword::UpperSerbian:
        return "upper-serbian"sv;
        
    case Keyword::UpperUkrainianFull:
        return "upper-ukrainian-full"sv;
        
    case Keyword::UpperUkrainian:
        return "upper-ukrainian"sv;
        
    case Keyword::Uppercase:
        return "uppercase"sv;
        
    case Keyword::Upright:
        return "upright"sv;
        
    case Keyword::UrduAbjad:
        return "urdu-abjad"sv;
        
    case Keyword::UrduAlphabetic:
        return "urdu-alphabetic"sv;
        
    case Keyword::Urdu:
        return "urdu"sv;
        
    case Keyword::UseCredentials:
        return "use-credentials"sv;
        
    case Keyword::Variations:
        return "variations"sv;
        
    case Keyword::Vertical:
        return "vertical"sv;
        
    case Keyword::VerticalLr:
        return "vertical-lr"sv;
        
    case Keyword::VerticalRl:
        return "vertical-rl"sv;
        
    case Keyword::VerticalText:
        return "vertical-text"sv;
        
    case Keyword::ViewBox:
        return "view-box"sv;
        
    case Keyword::Visible:
        return "visible"sv;
        
    case Keyword::Visitedtext:
        return "visitedtext"sv;
        
    case Keyword::WResize:
        return "w-resize"sv;
        
    case Keyword::Wait:
        return "wait"sv;
        
    case Keyword::WarangCiti:
        return "warang-citi"sv;
        
    case Keyword::Wavy:
        return "wavy"sv;
        
    case Keyword::Width:
        return "width"sv;
        
    case Keyword::Window:
        return "window"sv;
        
    case Keyword::Windowframe:
        return "windowframe"sv;
        
    case Keyword::Windowtext:
        return "windowtext"sv;
        
    case Keyword::Wolaita:
        return "wolaita"sv;
        
    case Keyword::Words:
        return "words"sv;
        
    case Keyword::Wrap:
        return "wrap"sv;
        
    case Keyword::WrapReverse:
        return "wrap-reverse"sv;
        
    case Keyword::X:
        return "x"sv;
        
    case Keyword::XEnd:
        return "x-end"sv;
        
    case Keyword::XLarge:
        return "x-large"sv;
        
    case Keyword::XStart:
        return "x-start"sv;
        
    case Keyword::XSmall:
        return "x-small"sv;
        
    case Keyword::XxLarge:
        return "xx-large"sv;
        
    case Keyword::XxSmall:
        return "xx-small"sv;
        
    case Keyword::XxxLarge:
        return "xxx-large"sv;
        
    case Keyword::Y:
        return "y"sv;
        
    case Keyword::YEnd:
        return "y-end"sv;
        
    case Keyword::YStart:
        return "y-start"sv;
        
    case Keyword::Yemsa:
        return "yemsa"sv;
        
    case Keyword::Zhuyin:
        return "zhuyin"sv;
        
    case Keyword::ZoomIn:
        return "zoom-in"sv;
        
    case Keyword::ZoomOut:
        return "zoom-out"sv;
        
    default:
        return "(invalid CSS::Keyword)"sv;
    }
}

} // namespace Web::CSS
