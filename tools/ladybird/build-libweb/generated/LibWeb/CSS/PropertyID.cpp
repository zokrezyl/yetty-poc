
#include <AK/Assertions.h>
#include <LibWeb/CSS/Enums.h>
#include <LibWeb/CSS/Parser/Parser.h>
#include <LibWeb/CSS/PropertyID.h>
#include <LibWeb/CSS/PropertyName.h>
#include <LibWeb/CSS/StyleValues/PercentageStyleValue.h>
#include <LibWeb/CSS/StyleValues/StyleValue.h>
#include <LibWeb/CSS/StyleValues/TimeStyleValue.h>
#include <LibWeb/Infra/Strings.h>

namespace Web::CSS {

static auto generate_camel_case_property_table()
{
    HashMap<StringView, PropertyID, CaseInsensitiveASCIIStringViewTraits> table;

    table.set("mozAppearance"sv, PropertyID::Appearance);

    table.set("webkitAlignContent"sv, PropertyID::AlignContent);

    table.set("webkitAlignItems"sv, PropertyID::AlignItems);

    table.set("webkitAlignSelf"sv, PropertyID::AlignSelf);

    table.set("webkitAnimation"sv, PropertyID::Animation);

    table.set("webkitAnimationDelay"sv, PropertyID::AnimationDelay);

    table.set("webkitAnimationDirection"sv, PropertyID::AnimationDirection);

    table.set("webkitAnimationDuration"sv, PropertyID::AnimationDuration);

    table.set("webkitAnimationFillMode"sv, PropertyID::AnimationFillMode);

    table.set("webkitAnimationIterationCount"sv, PropertyID::AnimationIterationCount);

    table.set("webkitAnimationName"sv, PropertyID::AnimationName);

    table.set("webkitAnimationPlayState"sv, PropertyID::AnimationPlayState);

    table.set("webkitAnimationTimingFunction"sv, PropertyID::AnimationTimingFunction);

    table.set("webkitAppearance"sv, PropertyID::Appearance);

    table.set("webkitBackgroundClip"sv, PropertyID::BackgroundClip);

    table.set("webkitBackgroundOrigin"sv, PropertyID::BackgroundOrigin);

    table.set("webkitBackgroundSize"sv, PropertyID::BackgroundSize);

    table.set("webkitBorderBottomLeftRadius"sv, PropertyID::BorderBottomLeftRadius);

    table.set("webkitBorderBottomRightRadius"sv, PropertyID::BorderBottomRightRadius);

    table.set("webkitBorderRadius"sv, PropertyID::BorderRadius);

    table.set("webkitBorderTopLeftRadius"sv, PropertyID::BorderTopLeftRadius);

    table.set("webkitBorderTopRightRadius"sv, PropertyID::BorderTopRightRadius);

    table.set("webkitBoxAlign"sv, PropertyID::AlignItems);

    table.set("webkitBoxFlex"sv, PropertyID::FlexGrow);

    table.set("webkitBoxOrdinalGroup"sv, PropertyID::Order);

    table.set("webkitBoxOrient"sv, PropertyID::FlexDirection);

    table.set("webkitBoxPack"sv, PropertyID::JustifyContent);

    table.set("webkitBoxShadow"sv, PropertyID::BoxShadow);

    table.set("webkitBoxSizing"sv, PropertyID::BoxSizing);

    table.set("webkitFilter"sv, PropertyID::Filter);

    table.set("webkitFlex"sv, PropertyID::Flex);

    table.set("webkitFlexBasis"sv, PropertyID::FlexBasis);

    table.set("webkitFlexDirection"sv, PropertyID::FlexDirection);

    table.set("webkitFlexFlow"sv, PropertyID::FlexFlow);

    table.set("webkitFlexGrow"sv, PropertyID::FlexGrow);

    table.set("webkitFlexShrink"sv, PropertyID::FlexShrink);

    table.set("webkitFlexWrap"sv, PropertyID::FlexWrap);

    table.set("webkitJustifyContent"sv, PropertyID::JustifyContent);

    table.set("webkitMask"sv, PropertyID::Mask);

    table.set("webkitMaskClip"sv, PropertyID::MaskClip);

    table.set("webkitMaskComposite"sv, PropertyID::MaskComposite);

    table.set("webkitMaskImage"sv, PropertyID::MaskImage);

    table.set("webkitMaskOrigin"sv, PropertyID::MaskOrigin);

    table.set("webkitMaskPosition"sv, PropertyID::MaskPosition);

    table.set("webkitMaskRepeat"sv, PropertyID::MaskRepeat);

    table.set("webkitMaskSize"sv, PropertyID::MaskSize);

    table.set("webkitOrder"sv, PropertyID::Order);

    table.set("webkitPerspective"sv, PropertyID::Perspective);

    table.set("webkitPerspectiveOrigin"sv, PropertyID::PerspectiveOrigin);

    table.set("webkitTextFillColor"sv, PropertyID::WebkitTextFillColor);

    table.set("webkitTransform"sv, PropertyID::Transform);

    table.set("webkitTransformOrigin"sv, PropertyID::TransformOrigin);

    table.set("webkitTransformStyle"sv, PropertyID::TransformStyle);

    table.set("webkitTransition"sv, PropertyID::Transition);

    table.set("webkitTransitionDelay"sv, PropertyID::TransitionDelay);

    table.set("webkitTransitionDuration"sv, PropertyID::TransitionDuration);

    table.set("webkitTransitionProperty"sv, PropertyID::TransitionProperty);

    table.set("webkitTransitionTimingFunction"sv, PropertyID::TransitionTimingFunction);

    table.set("webkitUserSelect"sv, PropertyID::UserSelect);

    table.set("accentColor"sv, PropertyID::AccentColor);

    table.set("alignContent"sv, PropertyID::AlignContent);

    table.set("alignItems"sv, PropertyID::AlignItems);

    table.set("alignSelf"sv, PropertyID::AlignSelf);

    table.set("all"sv, PropertyID::All);

    table.set("anchorName"sv, PropertyID::AnchorName);

    table.set("anchorScope"sv, PropertyID::AnchorScope);

    table.set("animation"sv, PropertyID::Animation);

    table.set("animationComposition"sv, PropertyID::AnimationComposition);

    table.set("animationDelay"sv, PropertyID::AnimationDelay);

    table.set("animationDirection"sv, PropertyID::AnimationDirection);

    table.set("animationDuration"sv, PropertyID::AnimationDuration);

    table.set("animationFillMode"sv, PropertyID::AnimationFillMode);

    table.set("animationIterationCount"sv, PropertyID::AnimationIterationCount);

    table.set("animationName"sv, PropertyID::AnimationName);

    table.set("animationPlayState"sv, PropertyID::AnimationPlayState);

    table.set("animationTimeline"sv, PropertyID::AnimationTimeline);

    table.set("animationTimingFunction"sv, PropertyID::AnimationTimingFunction);

    table.set("appearance"sv, PropertyID::Appearance);

    table.set("aspectRatio"sv, PropertyID::AspectRatio);

    table.set("backdropFilter"sv, PropertyID::BackdropFilter);

    table.set("background"sv, PropertyID::Background);

    table.set("backgroundAttachment"sv, PropertyID::BackgroundAttachment);

    table.set("backgroundBlendMode"sv, PropertyID::BackgroundBlendMode);

    table.set("backgroundClip"sv, PropertyID::BackgroundClip);

    table.set("backgroundColor"sv, PropertyID::BackgroundColor);

    table.set("backgroundImage"sv, PropertyID::BackgroundImage);

    table.set("backgroundOrigin"sv, PropertyID::BackgroundOrigin);

    table.set("backgroundPosition"sv, PropertyID::BackgroundPosition);

    table.set("backgroundPositionX"sv, PropertyID::BackgroundPositionX);

    table.set("backgroundPositionY"sv, PropertyID::BackgroundPositionY);

    table.set("backgroundRepeat"sv, PropertyID::BackgroundRepeat);

    table.set("backgroundSize"sv, PropertyID::BackgroundSize);

    table.set("blockSize"sv, PropertyID::BlockSize);

    table.set("border"sv, PropertyID::Border);

    table.set("borderBlock"sv, PropertyID::BorderBlock);

    table.set("borderBlockColor"sv, PropertyID::BorderBlockColor);

    table.set("borderBlockEnd"sv, PropertyID::BorderBlockEnd);

    table.set("borderBlockEndColor"sv, PropertyID::BorderBlockEndColor);

    table.set("borderBlockEndStyle"sv, PropertyID::BorderBlockEndStyle);

    table.set("borderBlockEndWidth"sv, PropertyID::BorderBlockEndWidth);

    table.set("borderBlockStart"sv, PropertyID::BorderBlockStart);

    table.set("borderBlockStartColor"sv, PropertyID::BorderBlockStartColor);

    table.set("borderBlockStartStyle"sv, PropertyID::BorderBlockStartStyle);

    table.set("borderBlockStartWidth"sv, PropertyID::BorderBlockStartWidth);

    table.set("borderBlockStyle"sv, PropertyID::BorderBlockStyle);

    table.set("borderBlockWidth"sv, PropertyID::BorderBlockWidth);

    table.set("borderBottom"sv, PropertyID::BorderBottom);

    table.set("borderBottomColor"sv, PropertyID::BorderBottomColor);

    table.set("borderBottomLeftRadius"sv, PropertyID::BorderBottomLeftRadius);

    table.set("borderBottomRightRadius"sv, PropertyID::BorderBottomRightRadius);

    table.set("borderBottomStyle"sv, PropertyID::BorderBottomStyle);

    table.set("borderBottomWidth"sv, PropertyID::BorderBottomWidth);

    table.set("borderCollapse"sv, PropertyID::BorderCollapse);

    table.set("borderColor"sv, PropertyID::BorderColor);

    table.set("borderEndEndRadius"sv, PropertyID::BorderEndEndRadius);

    table.set("borderEndStartRadius"sv, PropertyID::BorderEndStartRadius);

    table.set("borderImage"sv, PropertyID::BorderImage);

    table.set("borderImageOutset"sv, PropertyID::BorderImageOutset);

    table.set("borderImageRepeat"sv, PropertyID::BorderImageRepeat);

    table.set("borderImageSlice"sv, PropertyID::BorderImageSlice);

    table.set("borderImageSource"sv, PropertyID::BorderImageSource);

    table.set("borderImageWidth"sv, PropertyID::BorderImageWidth);

    table.set("borderInline"sv, PropertyID::BorderInline);

    table.set("borderInlineColor"sv, PropertyID::BorderInlineColor);

    table.set("borderInlineEnd"sv, PropertyID::BorderInlineEnd);

    table.set("borderInlineEndColor"sv, PropertyID::BorderInlineEndColor);

    table.set("borderInlineEndStyle"sv, PropertyID::BorderInlineEndStyle);

    table.set("borderInlineEndWidth"sv, PropertyID::BorderInlineEndWidth);

    table.set("borderInlineStart"sv, PropertyID::BorderInlineStart);

    table.set("borderInlineStartColor"sv, PropertyID::BorderInlineStartColor);

    table.set("borderInlineStartStyle"sv, PropertyID::BorderInlineStartStyle);

    table.set("borderInlineStartWidth"sv, PropertyID::BorderInlineStartWidth);

    table.set("borderInlineStyle"sv, PropertyID::BorderInlineStyle);

    table.set("borderInlineWidth"sv, PropertyID::BorderInlineWidth);

    table.set("borderLeft"sv, PropertyID::BorderLeft);

    table.set("borderLeftColor"sv, PropertyID::BorderLeftColor);

    table.set("borderLeftStyle"sv, PropertyID::BorderLeftStyle);

    table.set("borderLeftWidth"sv, PropertyID::BorderLeftWidth);

    table.set("borderRadius"sv, PropertyID::BorderRadius);

    table.set("borderRight"sv, PropertyID::BorderRight);

    table.set("borderRightColor"sv, PropertyID::BorderRightColor);

    table.set("borderRightStyle"sv, PropertyID::BorderRightStyle);

    table.set("borderRightWidth"sv, PropertyID::BorderRightWidth);

    table.set("borderSpacing"sv, PropertyID::BorderSpacing);

    table.set("borderStartEndRadius"sv, PropertyID::BorderStartEndRadius);

    table.set("borderStartStartRadius"sv, PropertyID::BorderStartStartRadius);

    table.set("borderStyle"sv, PropertyID::BorderStyle);

    table.set("borderTop"sv, PropertyID::BorderTop);

    table.set("borderTopColor"sv, PropertyID::BorderTopColor);

    table.set("borderTopLeftRadius"sv, PropertyID::BorderTopLeftRadius);

    table.set("borderTopRightRadius"sv, PropertyID::BorderTopRightRadius);

    table.set("borderTopStyle"sv, PropertyID::BorderTopStyle);

    table.set("borderTopWidth"sv, PropertyID::BorderTopWidth);

    table.set("borderWidth"sv, PropertyID::BorderWidth);

    table.set("bottom"sv, PropertyID::Bottom);

    table.set("boxShadow"sv, PropertyID::BoxShadow);

    table.set("boxSizing"sv, PropertyID::BoxSizing);

    table.set("captionSide"sv, PropertyID::CaptionSide);

    table.set("caretColor"sv, PropertyID::CaretColor);

    table.set("clear"sv, PropertyID::Clear);

    table.set("clip"sv, PropertyID::Clip);

    table.set("clipPath"sv, PropertyID::ClipPath);

    table.set("clipRule"sv, PropertyID::ClipRule);

    table.set("color"sv, PropertyID::Color);

    table.set("colorInterpolation"sv, PropertyID::ColorInterpolation);

    table.set("colorScheme"sv, PropertyID::ColorScheme);

    table.set("columnCount"sv, PropertyID::ColumnCount);

    table.set("columnGap"sv, PropertyID::ColumnGap);

    table.set("columnHeight"sv, PropertyID::ColumnHeight);

    table.set("columnSpan"sv, PropertyID::ColumnSpan);

    table.set("columnWidth"sv, PropertyID::ColumnWidth);

    table.set("columns"sv, PropertyID::Columns);

    table.set("contain"sv, PropertyID::Contain);

    table.set("containerType"sv, PropertyID::ContainerType);

    table.set("content"sv, PropertyID::Content);

    table.set("contentVisibility"sv, PropertyID::ContentVisibility);

    table.set("cornerBlockEndShape"sv, PropertyID::CornerBlockEndShape);

    table.set("cornerBlockStartShape"sv, PropertyID::CornerBlockStartShape);

    table.set("cornerBottomLeftShape"sv, PropertyID::CornerBottomLeftShape);

    table.set("cornerBottomRightShape"sv, PropertyID::CornerBottomRightShape);

    table.set("cornerBottomShape"sv, PropertyID::CornerBottomShape);

    table.set("cornerEndEndShape"sv, PropertyID::CornerEndEndShape);

    table.set("cornerEndStartShape"sv, PropertyID::CornerEndStartShape);

    table.set("cornerInlineEndShape"sv, PropertyID::CornerInlineEndShape);

    table.set("cornerInlineStartShape"sv, PropertyID::CornerInlineStartShape);

    table.set("cornerLeftShape"sv, PropertyID::CornerLeftShape);

    table.set("cornerRightShape"sv, PropertyID::CornerRightShape);

    table.set("cornerShape"sv, PropertyID::CornerShape);

    table.set("cornerStartEndShape"sv, PropertyID::CornerStartEndShape);

    table.set("cornerStartStartShape"sv, PropertyID::CornerStartStartShape);

    table.set("cornerTopLeftShape"sv, PropertyID::CornerTopLeftShape);

    table.set("cornerTopRightShape"sv, PropertyID::CornerTopRightShape);

    table.set("cornerTopShape"sv, PropertyID::CornerTopShape);

    table.set("counterIncrement"sv, PropertyID::CounterIncrement);

    table.set("counterReset"sv, PropertyID::CounterReset);

    table.set("counterSet"sv, PropertyID::CounterSet);

    table.set("cursor"sv, PropertyID::Cursor);

    table.set("cx"sv, PropertyID::Cx);

    table.set("cy"sv, PropertyID::Cy);

    table.set("direction"sv, PropertyID::Direction);

    table.set("display"sv, PropertyID::Display);

    table.set("dominantBaseline"sv, PropertyID::DominantBaseline);

    table.set("emptyCells"sv, PropertyID::EmptyCells);

    table.set("fill"sv, PropertyID::Fill);

    table.set("fillOpacity"sv, PropertyID::FillOpacity);

    table.set("fillRule"sv, PropertyID::FillRule);

    table.set("filter"sv, PropertyID::Filter);

    table.set("flex"sv, PropertyID::Flex);

    table.set("flexBasis"sv, PropertyID::FlexBasis);

    table.set("flexDirection"sv, PropertyID::FlexDirection);

    table.set("flexFlow"sv, PropertyID::FlexFlow);

    table.set("flexGrow"sv, PropertyID::FlexGrow);

    table.set("flexShrink"sv, PropertyID::FlexShrink);

    table.set("flexWrap"sv, PropertyID::FlexWrap);

    table.set("float"sv, PropertyID::Float);

    table.set("floodColor"sv, PropertyID::FloodColor);

    table.set("floodOpacity"sv, PropertyID::FloodOpacity);

    table.set("font"sv, PropertyID::Font);

    table.set("fontFamily"sv, PropertyID::FontFamily);

    table.set("fontFeatureSettings"sv, PropertyID::FontFeatureSettings);

    table.set("fontKerning"sv, PropertyID::FontKerning);

    table.set("fontLanguageOverride"sv, PropertyID::FontLanguageOverride);

    table.set("fontOpticalSizing"sv, PropertyID::FontOpticalSizing);

    table.set("fontSize"sv, PropertyID::FontSize);

    table.set("fontStretch"sv, PropertyID::FontWidth);

    table.set("fontStyle"sv, PropertyID::FontStyle);

    table.set("fontVariant"sv, PropertyID::FontVariant);

    table.set("fontVariantAlternates"sv, PropertyID::FontVariantAlternates);

    table.set("fontVariantCaps"sv, PropertyID::FontVariantCaps);

    table.set("fontVariantEastAsian"sv, PropertyID::FontVariantEastAsian);

    table.set("fontVariantEmoji"sv, PropertyID::FontVariantEmoji);

    table.set("fontVariantLigatures"sv, PropertyID::FontVariantLigatures);

    table.set("fontVariantNumeric"sv, PropertyID::FontVariantNumeric);

    table.set("fontVariantPosition"sv, PropertyID::FontVariantPosition);

    table.set("fontVariationSettings"sv, PropertyID::FontVariationSettings);

    table.set("fontWeight"sv, PropertyID::FontWeight);

    table.set("fontWidth"sv, PropertyID::FontWidth);

    table.set("gap"sv, PropertyID::Gap);

    table.set("grid"sv, PropertyID::Grid);

    table.set("gridArea"sv, PropertyID::GridArea);

    table.set("gridAutoColumns"sv, PropertyID::GridAutoColumns);

    table.set("gridAutoFlow"sv, PropertyID::GridAutoFlow);

    table.set("gridAutoRows"sv, PropertyID::GridAutoRows);

    table.set("gridColumn"sv, PropertyID::GridColumn);

    table.set("gridColumnEnd"sv, PropertyID::GridColumnEnd);

    table.set("gridColumnGap"sv, PropertyID::ColumnGap);

    table.set("gridColumnStart"sv, PropertyID::GridColumnStart);

    table.set("gridGap"sv, PropertyID::Gap);

    table.set("gridRow"sv, PropertyID::GridRow);

    table.set("gridRowEnd"sv, PropertyID::GridRowEnd);

    table.set("gridRowGap"sv, PropertyID::RowGap);

    table.set("gridRowStart"sv, PropertyID::GridRowStart);

    table.set("gridTemplate"sv, PropertyID::GridTemplate);

    table.set("gridTemplateAreas"sv, PropertyID::GridTemplateAreas);

    table.set("gridTemplateColumns"sv, PropertyID::GridTemplateColumns);

    table.set("gridTemplateRows"sv, PropertyID::GridTemplateRows);

    table.set("height"sv, PropertyID::Height);

    table.set("imageRendering"sv, PropertyID::ImageRendering);

    table.set("inlineSize"sv, PropertyID::InlineSize);

    table.set("inset"sv, PropertyID::Inset);

    table.set("insetBlock"sv, PropertyID::InsetBlock);

    table.set("insetBlockEnd"sv, PropertyID::InsetBlockEnd);

    table.set("insetBlockStart"sv, PropertyID::InsetBlockStart);

    table.set("insetInline"sv, PropertyID::InsetInline);

    table.set("insetInlineEnd"sv, PropertyID::InsetInlineEnd);

    table.set("insetInlineStart"sv, PropertyID::InsetInlineStart);

    table.set("isolation"sv, PropertyID::Isolation);

    table.set("justifyContent"sv, PropertyID::JustifyContent);

    table.set("justifyItems"sv, PropertyID::JustifyItems);

    table.set("justifySelf"sv, PropertyID::JustifySelf);

    table.set("left"sv, PropertyID::Left);

    table.set("letterSpacing"sv, PropertyID::LetterSpacing);

    table.set("lineHeight"sv, PropertyID::LineHeight);

    table.set("listStyle"sv, PropertyID::ListStyle);

    table.set("listStyleImage"sv, PropertyID::ListStyleImage);

    table.set("listStylePosition"sv, PropertyID::ListStylePosition);

    table.set("listStyleType"sv, PropertyID::ListStyleType);

    table.set("margin"sv, PropertyID::Margin);

    table.set("marginBlock"sv, PropertyID::MarginBlock);

    table.set("marginBlockEnd"sv, PropertyID::MarginBlockEnd);

    table.set("marginBlockStart"sv, PropertyID::MarginBlockStart);

    table.set("marginBottom"sv, PropertyID::MarginBottom);

    table.set("marginInline"sv, PropertyID::MarginInline);

    table.set("marginInlineEnd"sv, PropertyID::MarginInlineEnd);

    table.set("marginInlineStart"sv, PropertyID::MarginInlineStart);

    table.set("marginLeft"sv, PropertyID::MarginLeft);

    table.set("marginRight"sv, PropertyID::MarginRight);

    table.set("marginTop"sv, PropertyID::MarginTop);

    table.set("mask"sv, PropertyID::Mask);

    table.set("maskClip"sv, PropertyID::MaskClip);

    table.set("maskComposite"sv, PropertyID::MaskComposite);

    table.set("maskImage"sv, PropertyID::MaskImage);

    table.set("maskMode"sv, PropertyID::MaskMode);

    table.set("maskOrigin"sv, PropertyID::MaskOrigin);

    table.set("maskPosition"sv, PropertyID::MaskPosition);

    table.set("maskRepeat"sv, PropertyID::MaskRepeat);

    table.set("maskSize"sv, PropertyID::MaskSize);

    table.set("maskType"sv, PropertyID::MaskType);

    table.set("mathDepth"sv, PropertyID::MathDepth);

    table.set("mathShift"sv, PropertyID::MathShift);

    table.set("mathStyle"sv, PropertyID::MathStyle);

    table.set("maxBlockSize"sv, PropertyID::MaxBlockSize);

    table.set("maxHeight"sv, PropertyID::MaxHeight);

    table.set("maxInlineSize"sv, PropertyID::MaxInlineSize);

    table.set("maxWidth"sv, PropertyID::MaxWidth);

    table.set("minBlockSize"sv, PropertyID::MinBlockSize);

    table.set("minHeight"sv, PropertyID::MinHeight);

    table.set("minInlineSize"sv, PropertyID::MinInlineSize);

    table.set("minWidth"sv, PropertyID::MinWidth);

    table.set("mixBlendMode"sv, PropertyID::MixBlendMode);

    table.set("objectFit"sv, PropertyID::ObjectFit);

    table.set("objectPosition"sv, PropertyID::ObjectPosition);

    table.set("opacity"sv, PropertyID::Opacity);

    table.set("order"sv, PropertyID::Order);

    table.set("orphans"sv, PropertyID::Orphans);

    table.set("outline"sv, PropertyID::Outline);

    table.set("outlineColor"sv, PropertyID::OutlineColor);

    table.set("outlineOffset"sv, PropertyID::OutlineOffset);

    table.set("outlineStyle"sv, PropertyID::OutlineStyle);

    table.set("outlineWidth"sv, PropertyID::OutlineWidth);

    table.set("overflow"sv, PropertyID::Overflow);

    table.set("overflowBlock"sv, PropertyID::OverflowBlock);

    table.set("overflowClipMargin"sv, PropertyID::OverflowClipMargin);

    table.set("overflowClipMarginBlock"sv, PropertyID::OverflowClipMarginBlock);

    table.set("overflowClipMarginBlockEnd"sv, PropertyID::OverflowClipMarginBlockEnd);

    table.set("overflowClipMarginBlockStart"sv, PropertyID::OverflowClipMarginBlockStart);

    table.set("overflowClipMarginBottom"sv, PropertyID::OverflowClipMarginBottom);

    table.set("overflowClipMarginInline"sv, PropertyID::OverflowClipMarginInline);

    table.set("overflowClipMarginInlineEnd"sv, PropertyID::OverflowClipMarginInlineEnd);

    table.set("overflowClipMarginInlineStart"sv, PropertyID::OverflowClipMarginInlineStart);

    table.set("overflowClipMarginLeft"sv, PropertyID::OverflowClipMarginLeft);

    table.set("overflowClipMarginRight"sv, PropertyID::OverflowClipMarginRight);

    table.set("overflowClipMarginTop"sv, PropertyID::OverflowClipMarginTop);

    table.set("overflowInline"sv, PropertyID::OverflowInline);

    table.set("overflowWrap"sv, PropertyID::OverflowWrap);

    table.set("overflowX"sv, PropertyID::OverflowX);

    table.set("overflowY"sv, PropertyID::OverflowY);

    table.set("padding"sv, PropertyID::Padding);

    table.set("paddingBlock"sv, PropertyID::PaddingBlock);

    table.set("paddingBlockEnd"sv, PropertyID::PaddingBlockEnd);

    table.set("paddingBlockStart"sv, PropertyID::PaddingBlockStart);

    table.set("paddingBottom"sv, PropertyID::PaddingBottom);

    table.set("paddingInline"sv, PropertyID::PaddingInline);

    table.set("paddingInlineEnd"sv, PropertyID::PaddingInlineEnd);

    table.set("paddingInlineStart"sv, PropertyID::PaddingInlineStart);

    table.set("paddingLeft"sv, PropertyID::PaddingLeft);

    table.set("paddingRight"sv, PropertyID::PaddingRight);

    table.set("paddingTop"sv, PropertyID::PaddingTop);

    table.set("paintOrder"sv, PropertyID::PaintOrder);

    table.set("perspective"sv, PropertyID::Perspective);

    table.set("perspectiveOrigin"sv, PropertyID::PerspectiveOrigin);

    table.set("placeContent"sv, PropertyID::PlaceContent);

    table.set("placeItems"sv, PropertyID::PlaceItems);

    table.set("placeSelf"sv, PropertyID::PlaceSelf);

    table.set("pointerEvents"sv, PropertyID::PointerEvents);

    table.set("position"sv, PropertyID::Position);

    table.set("positionAnchor"sv, PropertyID::PositionAnchor);

    table.set("positionArea"sv, PropertyID::PositionArea);

    table.set("positionTryFallbacks"sv, PropertyID::PositionTryFallbacks);

    table.set("positionTryOrder"sv, PropertyID::PositionTryOrder);

    table.set("positionVisibility"sv, PropertyID::PositionVisibility);

    table.set("quotes"sv, PropertyID::Quotes);

    table.set("r"sv, PropertyID::R);

    table.set("resize"sv, PropertyID::Resize);

    table.set("right"sv, PropertyID::Right);

    table.set("rotate"sv, PropertyID::Rotate);

    table.set("rowGap"sv, PropertyID::RowGap);

    table.set("rx"sv, PropertyID::Rx);

    table.set("ry"sv, PropertyID::Ry);

    table.set("scale"sv, PropertyID::Scale);

    table.set("scrollTimeline"sv, PropertyID::ScrollTimeline);

    table.set("scrollTimelineAxis"sv, PropertyID::ScrollTimelineAxis);

    table.set("scrollTimelineName"sv, PropertyID::ScrollTimelineName);

    table.set("scrollbarColor"sv, PropertyID::ScrollbarColor);

    table.set("scrollbarGutter"sv, PropertyID::ScrollbarGutter);

    table.set("scrollbarWidth"sv, PropertyID::ScrollbarWidth);

    table.set("shapeImageThreshold"sv, PropertyID::ShapeImageThreshold);

    table.set("shapeMargin"sv, PropertyID::ShapeMargin);

    table.set("shapeOutside"sv, PropertyID::ShapeOutside);

    table.set("shapeRendering"sv, PropertyID::ShapeRendering);

    table.set("stopColor"sv, PropertyID::StopColor);

    table.set("stopOpacity"sv, PropertyID::StopOpacity);

    table.set("stroke"sv, PropertyID::Stroke);

    table.set("strokeDasharray"sv, PropertyID::StrokeDasharray);

    table.set("strokeDashoffset"sv, PropertyID::StrokeDashoffset);

    table.set("strokeLinecap"sv, PropertyID::StrokeLinecap);

    table.set("strokeLinejoin"sv, PropertyID::StrokeLinejoin);

    table.set("strokeMiterlimit"sv, PropertyID::StrokeMiterlimit);

    table.set("strokeOpacity"sv, PropertyID::StrokeOpacity);

    table.set("strokeWidth"sv, PropertyID::StrokeWidth);

    table.set("tabSize"sv, PropertyID::TabSize);

    table.set("tableLayout"sv, PropertyID::TableLayout);

    table.set("textAlign"sv, PropertyID::TextAlign);

    table.set("textAnchor"sv, PropertyID::TextAnchor);

    table.set("textDecoration"sv, PropertyID::TextDecoration);

    table.set("textDecorationColor"sv, PropertyID::TextDecorationColor);

    table.set("textDecorationLine"sv, PropertyID::TextDecorationLine);

    table.set("textDecorationStyle"sv, PropertyID::TextDecorationStyle);

    table.set("textDecorationThickness"sv, PropertyID::TextDecorationThickness);

    table.set("textIndent"sv, PropertyID::TextIndent);

    table.set("textJustify"sv, PropertyID::TextJustify);

    table.set("textOverflow"sv, PropertyID::TextOverflow);

    table.set("textRendering"sv, PropertyID::TextRendering);

    table.set("textShadow"sv, PropertyID::TextShadow);

    table.set("textTransform"sv, PropertyID::TextTransform);

    table.set("textUnderlineOffset"sv, PropertyID::TextUnderlineOffset);

    table.set("textUnderlinePosition"sv, PropertyID::TextUnderlinePosition);

    table.set("textWrap"sv, PropertyID::TextWrap);

    table.set("textWrapMode"sv, PropertyID::TextWrapMode);

    table.set("textWrapStyle"sv, PropertyID::TextWrapStyle);

    table.set("timelineScope"sv, PropertyID::TimelineScope);

    table.set("top"sv, PropertyID::Top);

    table.set("touchAction"sv, PropertyID::TouchAction);

    table.set("transform"sv, PropertyID::Transform);

    table.set("transformBox"sv, PropertyID::TransformBox);

    table.set("transformOrigin"sv, PropertyID::TransformOrigin);

    table.set("transformStyle"sv, PropertyID::TransformStyle);

    table.set("transition"sv, PropertyID::Transition);

    table.set("transitionBehavior"sv, PropertyID::TransitionBehavior);

    table.set("transitionDelay"sv, PropertyID::TransitionDelay);

    table.set("transitionDuration"sv, PropertyID::TransitionDuration);

    table.set("transitionProperty"sv, PropertyID::TransitionProperty);

    table.set("transitionTimingFunction"sv, PropertyID::TransitionTimingFunction);

    table.set("translate"sv, PropertyID::Translate);

    table.set("unicodeBidi"sv, PropertyID::UnicodeBidi);

    table.set("userSelect"sv, PropertyID::UserSelect);

    table.set("verticalAlign"sv, PropertyID::VerticalAlign);

    table.set("viewTimeline"sv, PropertyID::ViewTimeline);

    table.set("viewTimelineAxis"sv, PropertyID::ViewTimelineAxis);

    table.set("viewTimelineInset"sv, PropertyID::ViewTimelineInset);

    table.set("viewTimelineName"sv, PropertyID::ViewTimelineName);

    table.set("viewTransitionName"sv, PropertyID::ViewTransitionName);

    table.set("visibility"sv, PropertyID::Visibility);

    table.set("whiteSpace"sv, PropertyID::WhiteSpace);

    table.set("whiteSpaceCollapse"sv, PropertyID::WhiteSpaceCollapse);

    table.set("whiteSpaceTrim"sv, PropertyID::WhiteSpaceTrim);

    table.set("widows"sv, PropertyID::Widows);

    table.set("width"sv, PropertyID::Width);

    table.set("willChange"sv, PropertyID::WillChange);

    table.set("wordBreak"sv, PropertyID::WordBreak);

    table.set("wordSpacing"sv, PropertyID::WordSpacing);

    table.set("wordWrap"sv, PropertyID::OverflowWrap);

    table.set("writingMode"sv, PropertyID::WritingMode);

    table.set("x"sv, PropertyID::X);

    table.set("y"sv, PropertyID::Y);

    table.set("zIndex"sv, PropertyID::ZIndex);

    return table;
}

static HashMap<StringView, PropertyID, CaseInsensitiveASCIIStringViewTraits> const camel_case_properties_table = generate_camel_case_property_table();

Optional<PropertyID> property_id_from_camel_case_string(StringView string)
{
    return camel_case_properties_table.get(string);
}

static auto generate_properties_table()
{
    HashMap<StringView, PropertyID, CaseInsensitiveASCIIStringViewTraits> table;

    table.set("-moz-appearance"sv, PropertyID::Appearance);

    table.set("-webkit-align-content"sv, PropertyID::AlignContent);

    table.set("-webkit-align-items"sv, PropertyID::AlignItems);

    table.set("-webkit-align-self"sv, PropertyID::AlignSelf);

    table.set("-webkit-animation"sv, PropertyID::Animation);

    table.set("-webkit-animation-delay"sv, PropertyID::AnimationDelay);

    table.set("-webkit-animation-direction"sv, PropertyID::AnimationDirection);

    table.set("-webkit-animation-duration"sv, PropertyID::AnimationDuration);

    table.set("-webkit-animation-fill-mode"sv, PropertyID::AnimationFillMode);

    table.set("-webkit-animation-iteration-count"sv, PropertyID::AnimationIterationCount);

    table.set("-webkit-animation-name"sv, PropertyID::AnimationName);

    table.set("-webkit-animation-play-state"sv, PropertyID::AnimationPlayState);

    table.set("-webkit-animation-timing-function"sv, PropertyID::AnimationTimingFunction);

    table.set("-webkit-appearance"sv, PropertyID::Appearance);

    table.set("-webkit-background-clip"sv, PropertyID::BackgroundClip);

    table.set("-webkit-background-origin"sv, PropertyID::BackgroundOrigin);

    table.set("-webkit-background-size"sv, PropertyID::BackgroundSize);

    table.set("-webkit-border-bottom-left-radius"sv, PropertyID::BorderBottomLeftRadius);

    table.set("-webkit-border-bottom-right-radius"sv, PropertyID::BorderBottomRightRadius);

    table.set("-webkit-border-radius"sv, PropertyID::BorderRadius);

    table.set("-webkit-border-top-left-radius"sv, PropertyID::BorderTopLeftRadius);

    table.set("-webkit-border-top-right-radius"sv, PropertyID::BorderTopRightRadius);

    table.set("-webkit-box-align"sv, PropertyID::AlignItems);

    table.set("-webkit-box-flex"sv, PropertyID::FlexGrow);

    table.set("-webkit-box-ordinal-group"sv, PropertyID::Order);

    table.set("-webkit-box-orient"sv, PropertyID::FlexDirection);

    table.set("-webkit-box-pack"sv, PropertyID::JustifyContent);

    table.set("-webkit-box-shadow"sv, PropertyID::BoxShadow);

    table.set("-webkit-box-sizing"sv, PropertyID::BoxSizing);

    table.set("-webkit-filter"sv, PropertyID::Filter);

    table.set("-webkit-flex"sv, PropertyID::Flex);

    table.set("-webkit-flex-basis"sv, PropertyID::FlexBasis);

    table.set("-webkit-flex-direction"sv, PropertyID::FlexDirection);

    table.set("-webkit-flex-flow"sv, PropertyID::FlexFlow);

    table.set("-webkit-flex-grow"sv, PropertyID::FlexGrow);

    table.set("-webkit-flex-shrink"sv, PropertyID::FlexShrink);

    table.set("-webkit-flex-wrap"sv, PropertyID::FlexWrap);

    table.set("-webkit-justify-content"sv, PropertyID::JustifyContent);

    table.set("-webkit-mask"sv, PropertyID::Mask);

    table.set("-webkit-mask-clip"sv, PropertyID::MaskClip);

    table.set("-webkit-mask-composite"sv, PropertyID::MaskComposite);

    table.set("-webkit-mask-image"sv, PropertyID::MaskImage);

    table.set("-webkit-mask-origin"sv, PropertyID::MaskOrigin);

    table.set("-webkit-mask-position"sv, PropertyID::MaskPosition);

    table.set("-webkit-mask-repeat"sv, PropertyID::MaskRepeat);

    table.set("-webkit-mask-size"sv, PropertyID::MaskSize);

    table.set("-webkit-order"sv, PropertyID::Order);

    table.set("-webkit-perspective"sv, PropertyID::Perspective);

    table.set("-webkit-perspective-origin"sv, PropertyID::PerspectiveOrigin);

    table.set("-webkit-text-fill-color"sv, PropertyID::WebkitTextFillColor);

    table.set("-webkit-transform"sv, PropertyID::Transform);

    table.set("-webkit-transform-origin"sv, PropertyID::TransformOrigin);

    table.set("-webkit-transform-style"sv, PropertyID::TransformStyle);

    table.set("-webkit-transition"sv, PropertyID::Transition);

    table.set("-webkit-transition-delay"sv, PropertyID::TransitionDelay);

    table.set("-webkit-transition-duration"sv, PropertyID::TransitionDuration);

    table.set("-webkit-transition-property"sv, PropertyID::TransitionProperty);

    table.set("-webkit-transition-timing-function"sv, PropertyID::TransitionTimingFunction);

    table.set("-webkit-user-select"sv, PropertyID::UserSelect);

    table.set("accent-color"sv, PropertyID::AccentColor);

    table.set("align-content"sv, PropertyID::AlignContent);

    table.set("align-items"sv, PropertyID::AlignItems);

    table.set("align-self"sv, PropertyID::AlignSelf);

    table.set("all"sv, PropertyID::All);

    table.set("anchor-name"sv, PropertyID::AnchorName);

    table.set("anchor-scope"sv, PropertyID::AnchorScope);

    table.set("animation"sv, PropertyID::Animation);

    table.set("animation-composition"sv, PropertyID::AnimationComposition);

    table.set("animation-delay"sv, PropertyID::AnimationDelay);

    table.set("animation-direction"sv, PropertyID::AnimationDirection);

    table.set("animation-duration"sv, PropertyID::AnimationDuration);

    table.set("animation-fill-mode"sv, PropertyID::AnimationFillMode);

    table.set("animation-iteration-count"sv, PropertyID::AnimationIterationCount);

    table.set("animation-name"sv, PropertyID::AnimationName);

    table.set("animation-play-state"sv, PropertyID::AnimationPlayState);

    table.set("animation-timeline"sv, PropertyID::AnimationTimeline);

    table.set("animation-timing-function"sv, PropertyID::AnimationTimingFunction);

    table.set("appearance"sv, PropertyID::Appearance);

    table.set("aspect-ratio"sv, PropertyID::AspectRatio);

    table.set("backdrop-filter"sv, PropertyID::BackdropFilter);

    table.set("background"sv, PropertyID::Background);

    table.set("background-attachment"sv, PropertyID::BackgroundAttachment);

    table.set("background-blend-mode"sv, PropertyID::BackgroundBlendMode);

    table.set("background-clip"sv, PropertyID::BackgroundClip);

    table.set("background-color"sv, PropertyID::BackgroundColor);

    table.set("background-image"sv, PropertyID::BackgroundImage);

    table.set("background-origin"sv, PropertyID::BackgroundOrigin);

    table.set("background-position"sv, PropertyID::BackgroundPosition);

    table.set("background-position-x"sv, PropertyID::BackgroundPositionX);

    table.set("background-position-y"sv, PropertyID::BackgroundPositionY);

    table.set("background-repeat"sv, PropertyID::BackgroundRepeat);

    table.set("background-size"sv, PropertyID::BackgroundSize);

    table.set("block-size"sv, PropertyID::BlockSize);

    table.set("border"sv, PropertyID::Border);

    table.set("border-block"sv, PropertyID::BorderBlock);

    table.set("border-block-color"sv, PropertyID::BorderBlockColor);

    table.set("border-block-end"sv, PropertyID::BorderBlockEnd);

    table.set("border-block-end-color"sv, PropertyID::BorderBlockEndColor);

    table.set("border-block-end-style"sv, PropertyID::BorderBlockEndStyle);

    table.set("border-block-end-width"sv, PropertyID::BorderBlockEndWidth);

    table.set("border-block-start"sv, PropertyID::BorderBlockStart);

    table.set("border-block-start-color"sv, PropertyID::BorderBlockStartColor);

    table.set("border-block-start-style"sv, PropertyID::BorderBlockStartStyle);

    table.set("border-block-start-width"sv, PropertyID::BorderBlockStartWidth);

    table.set("border-block-style"sv, PropertyID::BorderBlockStyle);

    table.set("border-block-width"sv, PropertyID::BorderBlockWidth);

    table.set("border-bottom"sv, PropertyID::BorderBottom);

    table.set("border-bottom-color"sv, PropertyID::BorderBottomColor);

    table.set("border-bottom-left-radius"sv, PropertyID::BorderBottomLeftRadius);

    table.set("border-bottom-right-radius"sv, PropertyID::BorderBottomRightRadius);

    table.set("border-bottom-style"sv, PropertyID::BorderBottomStyle);

    table.set("border-bottom-width"sv, PropertyID::BorderBottomWidth);

    table.set("border-collapse"sv, PropertyID::BorderCollapse);

    table.set("border-color"sv, PropertyID::BorderColor);

    table.set("border-end-end-radius"sv, PropertyID::BorderEndEndRadius);

    table.set("border-end-start-radius"sv, PropertyID::BorderEndStartRadius);

    table.set("border-image"sv, PropertyID::BorderImage);

    table.set("border-image-outset"sv, PropertyID::BorderImageOutset);

    table.set("border-image-repeat"sv, PropertyID::BorderImageRepeat);

    table.set("border-image-slice"sv, PropertyID::BorderImageSlice);

    table.set("border-image-source"sv, PropertyID::BorderImageSource);

    table.set("border-image-width"sv, PropertyID::BorderImageWidth);

    table.set("border-inline"sv, PropertyID::BorderInline);

    table.set("border-inline-color"sv, PropertyID::BorderInlineColor);

    table.set("border-inline-end"sv, PropertyID::BorderInlineEnd);

    table.set("border-inline-end-color"sv, PropertyID::BorderInlineEndColor);

    table.set("border-inline-end-style"sv, PropertyID::BorderInlineEndStyle);

    table.set("border-inline-end-width"sv, PropertyID::BorderInlineEndWidth);

    table.set("border-inline-start"sv, PropertyID::BorderInlineStart);

    table.set("border-inline-start-color"sv, PropertyID::BorderInlineStartColor);

    table.set("border-inline-start-style"sv, PropertyID::BorderInlineStartStyle);

    table.set("border-inline-start-width"sv, PropertyID::BorderInlineStartWidth);

    table.set("border-inline-style"sv, PropertyID::BorderInlineStyle);

    table.set("border-inline-width"sv, PropertyID::BorderInlineWidth);

    table.set("border-left"sv, PropertyID::BorderLeft);

    table.set("border-left-color"sv, PropertyID::BorderLeftColor);

    table.set("border-left-style"sv, PropertyID::BorderLeftStyle);

    table.set("border-left-width"sv, PropertyID::BorderLeftWidth);

    table.set("border-radius"sv, PropertyID::BorderRadius);

    table.set("border-right"sv, PropertyID::BorderRight);

    table.set("border-right-color"sv, PropertyID::BorderRightColor);

    table.set("border-right-style"sv, PropertyID::BorderRightStyle);

    table.set("border-right-width"sv, PropertyID::BorderRightWidth);

    table.set("border-spacing"sv, PropertyID::BorderSpacing);

    table.set("border-start-end-radius"sv, PropertyID::BorderStartEndRadius);

    table.set("border-start-start-radius"sv, PropertyID::BorderStartStartRadius);

    table.set("border-style"sv, PropertyID::BorderStyle);

    table.set("border-top"sv, PropertyID::BorderTop);

    table.set("border-top-color"sv, PropertyID::BorderTopColor);

    table.set("border-top-left-radius"sv, PropertyID::BorderTopLeftRadius);

    table.set("border-top-right-radius"sv, PropertyID::BorderTopRightRadius);

    table.set("border-top-style"sv, PropertyID::BorderTopStyle);

    table.set("border-top-width"sv, PropertyID::BorderTopWidth);

    table.set("border-width"sv, PropertyID::BorderWidth);

    table.set("bottom"sv, PropertyID::Bottom);

    table.set("box-shadow"sv, PropertyID::BoxShadow);

    table.set("box-sizing"sv, PropertyID::BoxSizing);

    table.set("caption-side"sv, PropertyID::CaptionSide);

    table.set("caret-color"sv, PropertyID::CaretColor);

    table.set("clear"sv, PropertyID::Clear);

    table.set("clip"sv, PropertyID::Clip);

    table.set("clip-path"sv, PropertyID::ClipPath);

    table.set("clip-rule"sv, PropertyID::ClipRule);

    table.set("color"sv, PropertyID::Color);

    table.set("color-interpolation"sv, PropertyID::ColorInterpolation);

    table.set("color-scheme"sv, PropertyID::ColorScheme);

    table.set("column-count"sv, PropertyID::ColumnCount);

    table.set("column-gap"sv, PropertyID::ColumnGap);

    table.set("column-height"sv, PropertyID::ColumnHeight);

    table.set("column-span"sv, PropertyID::ColumnSpan);

    table.set("column-width"sv, PropertyID::ColumnWidth);

    table.set("columns"sv, PropertyID::Columns);

    table.set("contain"sv, PropertyID::Contain);

    table.set("container-type"sv, PropertyID::ContainerType);

    table.set("content"sv, PropertyID::Content);

    table.set("content-visibility"sv, PropertyID::ContentVisibility);

    table.set("corner-block-end-shape"sv, PropertyID::CornerBlockEndShape);

    table.set("corner-block-start-shape"sv, PropertyID::CornerBlockStartShape);

    table.set("corner-bottom-left-shape"sv, PropertyID::CornerBottomLeftShape);

    table.set("corner-bottom-right-shape"sv, PropertyID::CornerBottomRightShape);

    table.set("corner-bottom-shape"sv, PropertyID::CornerBottomShape);

    table.set("corner-end-end-shape"sv, PropertyID::CornerEndEndShape);

    table.set("corner-end-start-shape"sv, PropertyID::CornerEndStartShape);

    table.set("corner-inline-end-shape"sv, PropertyID::CornerInlineEndShape);

    table.set("corner-inline-start-shape"sv, PropertyID::CornerInlineStartShape);

    table.set("corner-left-shape"sv, PropertyID::CornerLeftShape);

    table.set("corner-right-shape"sv, PropertyID::CornerRightShape);

    table.set("corner-shape"sv, PropertyID::CornerShape);

    table.set("corner-start-end-shape"sv, PropertyID::CornerStartEndShape);

    table.set("corner-start-start-shape"sv, PropertyID::CornerStartStartShape);

    table.set("corner-top-left-shape"sv, PropertyID::CornerTopLeftShape);

    table.set("corner-top-right-shape"sv, PropertyID::CornerTopRightShape);

    table.set("corner-top-shape"sv, PropertyID::CornerTopShape);

    table.set("counter-increment"sv, PropertyID::CounterIncrement);

    table.set("counter-reset"sv, PropertyID::CounterReset);

    table.set("counter-set"sv, PropertyID::CounterSet);

    table.set("cursor"sv, PropertyID::Cursor);

    table.set("cx"sv, PropertyID::Cx);

    table.set("cy"sv, PropertyID::Cy);

    table.set("direction"sv, PropertyID::Direction);

    table.set("display"sv, PropertyID::Display);

    table.set("dominant-baseline"sv, PropertyID::DominantBaseline);

    table.set("empty-cells"sv, PropertyID::EmptyCells);

    table.set("fill"sv, PropertyID::Fill);

    table.set("fill-opacity"sv, PropertyID::FillOpacity);

    table.set("fill-rule"sv, PropertyID::FillRule);

    table.set("filter"sv, PropertyID::Filter);

    table.set("flex"sv, PropertyID::Flex);

    table.set("flex-basis"sv, PropertyID::FlexBasis);

    table.set("flex-direction"sv, PropertyID::FlexDirection);

    table.set("flex-flow"sv, PropertyID::FlexFlow);

    table.set("flex-grow"sv, PropertyID::FlexGrow);

    table.set("flex-shrink"sv, PropertyID::FlexShrink);

    table.set("flex-wrap"sv, PropertyID::FlexWrap);

    table.set("float"sv, PropertyID::Float);

    table.set("flood-color"sv, PropertyID::FloodColor);

    table.set("flood-opacity"sv, PropertyID::FloodOpacity);

    table.set("font"sv, PropertyID::Font);

    table.set("font-family"sv, PropertyID::FontFamily);

    table.set("font-feature-settings"sv, PropertyID::FontFeatureSettings);

    table.set("font-kerning"sv, PropertyID::FontKerning);

    table.set("font-language-override"sv, PropertyID::FontLanguageOverride);

    table.set("font-optical-sizing"sv, PropertyID::FontOpticalSizing);

    table.set("font-size"sv, PropertyID::FontSize);

    table.set("font-stretch"sv, PropertyID::FontWidth);

    table.set("font-style"sv, PropertyID::FontStyle);

    table.set("font-variant"sv, PropertyID::FontVariant);

    table.set("font-variant-alternates"sv, PropertyID::FontVariantAlternates);

    table.set("font-variant-caps"sv, PropertyID::FontVariantCaps);

    table.set("font-variant-east-asian"sv, PropertyID::FontVariantEastAsian);

    table.set("font-variant-emoji"sv, PropertyID::FontVariantEmoji);

    table.set("font-variant-ligatures"sv, PropertyID::FontVariantLigatures);

    table.set("font-variant-numeric"sv, PropertyID::FontVariantNumeric);

    table.set("font-variant-position"sv, PropertyID::FontVariantPosition);

    table.set("font-variation-settings"sv, PropertyID::FontVariationSettings);

    table.set("font-weight"sv, PropertyID::FontWeight);

    table.set("font-width"sv, PropertyID::FontWidth);

    table.set("gap"sv, PropertyID::Gap);

    table.set("grid"sv, PropertyID::Grid);

    table.set("grid-area"sv, PropertyID::GridArea);

    table.set("grid-auto-columns"sv, PropertyID::GridAutoColumns);

    table.set("grid-auto-flow"sv, PropertyID::GridAutoFlow);

    table.set("grid-auto-rows"sv, PropertyID::GridAutoRows);

    table.set("grid-column"sv, PropertyID::GridColumn);

    table.set("grid-column-end"sv, PropertyID::GridColumnEnd);

    table.set("grid-column-gap"sv, PropertyID::ColumnGap);

    table.set("grid-column-start"sv, PropertyID::GridColumnStart);

    table.set("grid-gap"sv, PropertyID::Gap);

    table.set("grid-row"sv, PropertyID::GridRow);

    table.set("grid-row-end"sv, PropertyID::GridRowEnd);

    table.set("grid-row-gap"sv, PropertyID::RowGap);

    table.set("grid-row-start"sv, PropertyID::GridRowStart);

    table.set("grid-template"sv, PropertyID::GridTemplate);

    table.set("grid-template-areas"sv, PropertyID::GridTemplateAreas);

    table.set("grid-template-columns"sv, PropertyID::GridTemplateColumns);

    table.set("grid-template-rows"sv, PropertyID::GridTemplateRows);

    table.set("height"sv, PropertyID::Height);

    table.set("image-rendering"sv, PropertyID::ImageRendering);

    table.set("inline-size"sv, PropertyID::InlineSize);

    table.set("inset"sv, PropertyID::Inset);

    table.set("inset-block"sv, PropertyID::InsetBlock);

    table.set("inset-block-end"sv, PropertyID::InsetBlockEnd);

    table.set("inset-block-start"sv, PropertyID::InsetBlockStart);

    table.set("inset-inline"sv, PropertyID::InsetInline);

    table.set("inset-inline-end"sv, PropertyID::InsetInlineEnd);

    table.set("inset-inline-start"sv, PropertyID::InsetInlineStart);

    table.set("isolation"sv, PropertyID::Isolation);

    table.set("justify-content"sv, PropertyID::JustifyContent);

    table.set("justify-items"sv, PropertyID::JustifyItems);

    table.set("justify-self"sv, PropertyID::JustifySelf);

    table.set("left"sv, PropertyID::Left);

    table.set("letter-spacing"sv, PropertyID::LetterSpacing);

    table.set("line-height"sv, PropertyID::LineHeight);

    table.set("list-style"sv, PropertyID::ListStyle);

    table.set("list-style-image"sv, PropertyID::ListStyleImage);

    table.set("list-style-position"sv, PropertyID::ListStylePosition);

    table.set("list-style-type"sv, PropertyID::ListStyleType);

    table.set("margin"sv, PropertyID::Margin);

    table.set("margin-block"sv, PropertyID::MarginBlock);

    table.set("margin-block-end"sv, PropertyID::MarginBlockEnd);

    table.set("margin-block-start"sv, PropertyID::MarginBlockStart);

    table.set("margin-bottom"sv, PropertyID::MarginBottom);

    table.set("margin-inline"sv, PropertyID::MarginInline);

    table.set("margin-inline-end"sv, PropertyID::MarginInlineEnd);

    table.set("margin-inline-start"sv, PropertyID::MarginInlineStart);

    table.set("margin-left"sv, PropertyID::MarginLeft);

    table.set("margin-right"sv, PropertyID::MarginRight);

    table.set("margin-top"sv, PropertyID::MarginTop);

    table.set("mask"sv, PropertyID::Mask);

    table.set("mask-clip"sv, PropertyID::MaskClip);

    table.set("mask-composite"sv, PropertyID::MaskComposite);

    table.set("mask-image"sv, PropertyID::MaskImage);

    table.set("mask-mode"sv, PropertyID::MaskMode);

    table.set("mask-origin"sv, PropertyID::MaskOrigin);

    table.set("mask-position"sv, PropertyID::MaskPosition);

    table.set("mask-repeat"sv, PropertyID::MaskRepeat);

    table.set("mask-size"sv, PropertyID::MaskSize);

    table.set("mask-type"sv, PropertyID::MaskType);

    table.set("math-depth"sv, PropertyID::MathDepth);

    table.set("math-shift"sv, PropertyID::MathShift);

    table.set("math-style"sv, PropertyID::MathStyle);

    table.set("max-block-size"sv, PropertyID::MaxBlockSize);

    table.set("max-height"sv, PropertyID::MaxHeight);

    table.set("max-inline-size"sv, PropertyID::MaxInlineSize);

    table.set("max-width"sv, PropertyID::MaxWidth);

    table.set("min-block-size"sv, PropertyID::MinBlockSize);

    table.set("min-height"sv, PropertyID::MinHeight);

    table.set("min-inline-size"sv, PropertyID::MinInlineSize);

    table.set("min-width"sv, PropertyID::MinWidth);

    table.set("mix-blend-mode"sv, PropertyID::MixBlendMode);

    table.set("object-fit"sv, PropertyID::ObjectFit);

    table.set("object-position"sv, PropertyID::ObjectPosition);

    table.set("opacity"sv, PropertyID::Opacity);

    table.set("order"sv, PropertyID::Order);

    table.set("orphans"sv, PropertyID::Orphans);

    table.set("outline"sv, PropertyID::Outline);

    table.set("outline-color"sv, PropertyID::OutlineColor);

    table.set("outline-offset"sv, PropertyID::OutlineOffset);

    table.set("outline-style"sv, PropertyID::OutlineStyle);

    table.set("outline-width"sv, PropertyID::OutlineWidth);

    table.set("overflow"sv, PropertyID::Overflow);

    table.set("overflow-block"sv, PropertyID::OverflowBlock);

    table.set("overflow-clip-margin"sv, PropertyID::OverflowClipMargin);

    table.set("overflow-clip-margin-block"sv, PropertyID::OverflowClipMarginBlock);

    table.set("overflow-clip-margin-block-end"sv, PropertyID::OverflowClipMarginBlockEnd);

    table.set("overflow-clip-margin-block-start"sv, PropertyID::OverflowClipMarginBlockStart);

    table.set("overflow-clip-margin-bottom"sv, PropertyID::OverflowClipMarginBottom);

    table.set("overflow-clip-margin-inline"sv, PropertyID::OverflowClipMarginInline);

    table.set("overflow-clip-margin-inline-end"sv, PropertyID::OverflowClipMarginInlineEnd);

    table.set("overflow-clip-margin-inline-start"sv, PropertyID::OverflowClipMarginInlineStart);

    table.set("overflow-clip-margin-left"sv, PropertyID::OverflowClipMarginLeft);

    table.set("overflow-clip-margin-right"sv, PropertyID::OverflowClipMarginRight);

    table.set("overflow-clip-margin-top"sv, PropertyID::OverflowClipMarginTop);

    table.set("overflow-inline"sv, PropertyID::OverflowInline);

    table.set("overflow-wrap"sv, PropertyID::OverflowWrap);

    table.set("overflow-x"sv, PropertyID::OverflowX);

    table.set("overflow-y"sv, PropertyID::OverflowY);

    table.set("padding"sv, PropertyID::Padding);

    table.set("padding-block"sv, PropertyID::PaddingBlock);

    table.set("padding-block-end"sv, PropertyID::PaddingBlockEnd);

    table.set("padding-block-start"sv, PropertyID::PaddingBlockStart);

    table.set("padding-bottom"sv, PropertyID::PaddingBottom);

    table.set("padding-inline"sv, PropertyID::PaddingInline);

    table.set("padding-inline-end"sv, PropertyID::PaddingInlineEnd);

    table.set("padding-inline-start"sv, PropertyID::PaddingInlineStart);

    table.set("padding-left"sv, PropertyID::PaddingLeft);

    table.set("padding-right"sv, PropertyID::PaddingRight);

    table.set("padding-top"sv, PropertyID::PaddingTop);

    table.set("paint-order"sv, PropertyID::PaintOrder);

    table.set("perspective"sv, PropertyID::Perspective);

    table.set("perspective-origin"sv, PropertyID::PerspectiveOrigin);

    table.set("place-content"sv, PropertyID::PlaceContent);

    table.set("place-items"sv, PropertyID::PlaceItems);

    table.set("place-self"sv, PropertyID::PlaceSelf);

    table.set("pointer-events"sv, PropertyID::PointerEvents);

    table.set("position"sv, PropertyID::Position);

    table.set("position-anchor"sv, PropertyID::PositionAnchor);

    table.set("position-area"sv, PropertyID::PositionArea);

    table.set("position-try-fallbacks"sv, PropertyID::PositionTryFallbacks);

    table.set("position-try-order"sv, PropertyID::PositionTryOrder);

    table.set("position-visibility"sv, PropertyID::PositionVisibility);

    table.set("quotes"sv, PropertyID::Quotes);

    table.set("r"sv, PropertyID::R);

    table.set("resize"sv, PropertyID::Resize);

    table.set("right"sv, PropertyID::Right);

    table.set("rotate"sv, PropertyID::Rotate);

    table.set("row-gap"sv, PropertyID::RowGap);

    table.set("rx"sv, PropertyID::Rx);

    table.set("ry"sv, PropertyID::Ry);

    table.set("scale"sv, PropertyID::Scale);

    table.set("scroll-timeline"sv, PropertyID::ScrollTimeline);

    table.set("scroll-timeline-axis"sv, PropertyID::ScrollTimelineAxis);

    table.set("scroll-timeline-name"sv, PropertyID::ScrollTimelineName);

    table.set("scrollbar-color"sv, PropertyID::ScrollbarColor);

    table.set("scrollbar-gutter"sv, PropertyID::ScrollbarGutter);

    table.set("scrollbar-width"sv, PropertyID::ScrollbarWidth);

    table.set("shape-image-threshold"sv, PropertyID::ShapeImageThreshold);

    table.set("shape-margin"sv, PropertyID::ShapeMargin);

    table.set("shape-outside"sv, PropertyID::ShapeOutside);

    table.set("shape-rendering"sv, PropertyID::ShapeRendering);

    table.set("stop-color"sv, PropertyID::StopColor);

    table.set("stop-opacity"sv, PropertyID::StopOpacity);

    table.set("stroke"sv, PropertyID::Stroke);

    table.set("stroke-dasharray"sv, PropertyID::StrokeDasharray);

    table.set("stroke-dashoffset"sv, PropertyID::StrokeDashoffset);

    table.set("stroke-linecap"sv, PropertyID::StrokeLinecap);

    table.set("stroke-linejoin"sv, PropertyID::StrokeLinejoin);

    table.set("stroke-miterlimit"sv, PropertyID::StrokeMiterlimit);

    table.set("stroke-opacity"sv, PropertyID::StrokeOpacity);

    table.set("stroke-width"sv, PropertyID::StrokeWidth);

    table.set("tab-size"sv, PropertyID::TabSize);

    table.set("table-layout"sv, PropertyID::TableLayout);

    table.set("text-align"sv, PropertyID::TextAlign);

    table.set("text-anchor"sv, PropertyID::TextAnchor);

    table.set("text-decoration"sv, PropertyID::TextDecoration);

    table.set("text-decoration-color"sv, PropertyID::TextDecorationColor);

    table.set("text-decoration-line"sv, PropertyID::TextDecorationLine);

    table.set("text-decoration-style"sv, PropertyID::TextDecorationStyle);

    table.set("text-decoration-thickness"sv, PropertyID::TextDecorationThickness);

    table.set("text-indent"sv, PropertyID::TextIndent);

    table.set("text-justify"sv, PropertyID::TextJustify);

    table.set("text-overflow"sv, PropertyID::TextOverflow);

    table.set("text-rendering"sv, PropertyID::TextRendering);

    table.set("text-shadow"sv, PropertyID::TextShadow);

    table.set("text-transform"sv, PropertyID::TextTransform);

    table.set("text-underline-offset"sv, PropertyID::TextUnderlineOffset);

    table.set("text-underline-position"sv, PropertyID::TextUnderlinePosition);

    table.set("text-wrap"sv, PropertyID::TextWrap);

    table.set("text-wrap-mode"sv, PropertyID::TextWrapMode);

    table.set("text-wrap-style"sv, PropertyID::TextWrapStyle);

    table.set("timeline-scope"sv, PropertyID::TimelineScope);

    table.set("top"sv, PropertyID::Top);

    table.set("touch-action"sv, PropertyID::TouchAction);

    table.set("transform"sv, PropertyID::Transform);

    table.set("transform-box"sv, PropertyID::TransformBox);

    table.set("transform-origin"sv, PropertyID::TransformOrigin);

    table.set("transform-style"sv, PropertyID::TransformStyle);

    table.set("transition"sv, PropertyID::Transition);

    table.set("transition-behavior"sv, PropertyID::TransitionBehavior);

    table.set("transition-delay"sv, PropertyID::TransitionDelay);

    table.set("transition-duration"sv, PropertyID::TransitionDuration);

    table.set("transition-property"sv, PropertyID::TransitionProperty);

    table.set("transition-timing-function"sv, PropertyID::TransitionTimingFunction);

    table.set("translate"sv, PropertyID::Translate);

    table.set("unicode-bidi"sv, PropertyID::UnicodeBidi);

    table.set("user-select"sv, PropertyID::UserSelect);

    table.set("vertical-align"sv, PropertyID::VerticalAlign);

    table.set("view-timeline"sv, PropertyID::ViewTimeline);

    table.set("view-timeline-axis"sv, PropertyID::ViewTimelineAxis);

    table.set("view-timeline-inset"sv, PropertyID::ViewTimelineInset);

    table.set("view-timeline-name"sv, PropertyID::ViewTimelineName);

    table.set("view-transition-name"sv, PropertyID::ViewTransitionName);

    table.set("visibility"sv, PropertyID::Visibility);

    table.set("white-space"sv, PropertyID::WhiteSpace);

    table.set("white-space-collapse"sv, PropertyID::WhiteSpaceCollapse);

    table.set("white-space-trim"sv, PropertyID::WhiteSpaceTrim);

    table.set("widows"sv, PropertyID::Widows);

    table.set("width"sv, PropertyID::Width);

    table.set("will-change"sv, PropertyID::WillChange);

    table.set("word-break"sv, PropertyID::WordBreak);

    table.set("word-spacing"sv, PropertyID::WordSpacing);

    table.set("word-wrap"sv, PropertyID::OverflowWrap);

    table.set("writing-mode"sv, PropertyID::WritingMode);

    table.set("x"sv, PropertyID::X);

    table.set("y"sv, PropertyID::Y);

    table.set("z-index"sv, PropertyID::ZIndex);

    return table;
}

static HashMap<StringView, PropertyID, CaseInsensitiveASCIIStringViewTraits> const properties_table = generate_properties_table();

Optional<PropertyID> property_id_from_string(StringView string)
{
    if (is_a_custom_property_name_string(string))
        return PropertyID::Custom;

    return properties_table.get(string);
}

FlyString const& string_from_property_id(PropertyID property_id) {
    switch (property_id) {

    case PropertyID::WebkitTextFillColor: {
        static FlyString name = "-webkit-text-fill-color"_fly_string;
        return name;
    }

    case PropertyID::AccentColor: {
        static FlyString name = "accent-color"_fly_string;
        return name;
    }

    case PropertyID::AlignContent: {
        static FlyString name = "align-content"_fly_string;
        return name;
    }

    case PropertyID::AlignItems: {
        static FlyString name = "align-items"_fly_string;
        return name;
    }

    case PropertyID::AlignSelf: {
        static FlyString name = "align-self"_fly_string;
        return name;
    }

    case PropertyID::All: {
        static FlyString name = "all"_fly_string;
        return name;
    }

    case PropertyID::AnchorName: {
        static FlyString name = "anchor-name"_fly_string;
        return name;
    }

    case PropertyID::AnchorScope: {
        static FlyString name = "anchor-scope"_fly_string;
        return name;
    }

    case PropertyID::Animation: {
        static FlyString name = "animation"_fly_string;
        return name;
    }

    case PropertyID::AnimationComposition: {
        static FlyString name = "animation-composition"_fly_string;
        return name;
    }

    case PropertyID::AnimationDelay: {
        static FlyString name = "animation-delay"_fly_string;
        return name;
    }

    case PropertyID::AnimationDirection: {
        static FlyString name = "animation-direction"_fly_string;
        return name;
    }

    case PropertyID::AnimationDuration: {
        static FlyString name = "animation-duration"_fly_string;
        return name;
    }

    case PropertyID::AnimationFillMode: {
        static FlyString name = "animation-fill-mode"_fly_string;
        return name;
    }

    case PropertyID::AnimationIterationCount: {
        static FlyString name = "animation-iteration-count"_fly_string;
        return name;
    }

    case PropertyID::AnimationName: {
        static FlyString name = "animation-name"_fly_string;
        return name;
    }

    case PropertyID::AnimationPlayState: {
        static FlyString name = "animation-play-state"_fly_string;
        return name;
    }

    case PropertyID::AnimationTimeline: {
        static FlyString name = "animation-timeline"_fly_string;
        return name;
    }

    case PropertyID::AnimationTimingFunction: {
        static FlyString name = "animation-timing-function"_fly_string;
        return name;
    }

    case PropertyID::Appearance: {
        static FlyString name = "appearance"_fly_string;
        return name;
    }

    case PropertyID::AspectRatio: {
        static FlyString name = "aspect-ratio"_fly_string;
        return name;
    }

    case PropertyID::BackdropFilter: {
        static FlyString name = "backdrop-filter"_fly_string;
        return name;
    }

    case PropertyID::Background: {
        static FlyString name = "background"_fly_string;
        return name;
    }

    case PropertyID::BackgroundAttachment: {
        static FlyString name = "background-attachment"_fly_string;
        return name;
    }

    case PropertyID::BackgroundBlendMode: {
        static FlyString name = "background-blend-mode"_fly_string;
        return name;
    }

    case PropertyID::BackgroundClip: {
        static FlyString name = "background-clip"_fly_string;
        return name;
    }

    case PropertyID::BackgroundColor: {
        static FlyString name = "background-color"_fly_string;
        return name;
    }

    case PropertyID::BackgroundImage: {
        static FlyString name = "background-image"_fly_string;
        return name;
    }

    case PropertyID::BackgroundOrigin: {
        static FlyString name = "background-origin"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPosition: {
        static FlyString name = "background-position"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPositionX: {
        static FlyString name = "background-position-x"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPositionY: {
        static FlyString name = "background-position-y"_fly_string;
        return name;
    }

    case PropertyID::BackgroundRepeat: {
        static FlyString name = "background-repeat"_fly_string;
        return name;
    }

    case PropertyID::BackgroundSize: {
        static FlyString name = "background-size"_fly_string;
        return name;
    }

    case PropertyID::BlockSize: {
        static FlyString name = "block-size"_fly_string;
        return name;
    }

    case PropertyID::Border: {
        static FlyString name = "border"_fly_string;
        return name;
    }

    case PropertyID::BorderBlock: {
        static FlyString name = "border-block"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockColor: {
        static FlyString name = "border-block-color"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEnd: {
        static FlyString name = "border-block-end"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndColor: {
        static FlyString name = "border-block-end-color"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndStyle: {
        static FlyString name = "border-block-end-style"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndWidth: {
        static FlyString name = "border-block-end-width"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStart: {
        static FlyString name = "border-block-start"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartColor: {
        static FlyString name = "border-block-start-color"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartStyle: {
        static FlyString name = "border-block-start-style"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartWidth: {
        static FlyString name = "border-block-start-width"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStyle: {
        static FlyString name = "border-block-style"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockWidth: {
        static FlyString name = "border-block-width"_fly_string;
        return name;
    }

    case PropertyID::BorderBottom: {
        static FlyString name = "border-bottom"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomColor: {
        static FlyString name = "border-bottom-color"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomLeftRadius: {
        static FlyString name = "border-bottom-left-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomRightRadius: {
        static FlyString name = "border-bottom-right-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomStyle: {
        static FlyString name = "border-bottom-style"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomWidth: {
        static FlyString name = "border-bottom-width"_fly_string;
        return name;
    }

    case PropertyID::BorderCollapse: {
        static FlyString name = "border-collapse"_fly_string;
        return name;
    }

    case PropertyID::BorderColor: {
        static FlyString name = "border-color"_fly_string;
        return name;
    }

    case PropertyID::BorderEndEndRadius: {
        static FlyString name = "border-end-end-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderEndStartRadius: {
        static FlyString name = "border-end-start-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderImage: {
        static FlyString name = "border-image"_fly_string;
        return name;
    }

    case PropertyID::BorderImageOutset: {
        static FlyString name = "border-image-outset"_fly_string;
        return name;
    }

    case PropertyID::BorderImageRepeat: {
        static FlyString name = "border-image-repeat"_fly_string;
        return name;
    }

    case PropertyID::BorderImageSlice: {
        static FlyString name = "border-image-slice"_fly_string;
        return name;
    }

    case PropertyID::BorderImageSource: {
        static FlyString name = "border-image-source"_fly_string;
        return name;
    }

    case PropertyID::BorderImageWidth: {
        static FlyString name = "border-image-width"_fly_string;
        return name;
    }

    case PropertyID::BorderInline: {
        static FlyString name = "border-inline"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineColor: {
        static FlyString name = "border-inline-color"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEnd: {
        static FlyString name = "border-inline-end"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndColor: {
        static FlyString name = "border-inline-end-color"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndStyle: {
        static FlyString name = "border-inline-end-style"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndWidth: {
        static FlyString name = "border-inline-end-width"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStart: {
        static FlyString name = "border-inline-start"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartColor: {
        static FlyString name = "border-inline-start-color"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartStyle: {
        static FlyString name = "border-inline-start-style"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartWidth: {
        static FlyString name = "border-inline-start-width"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStyle: {
        static FlyString name = "border-inline-style"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineWidth: {
        static FlyString name = "border-inline-width"_fly_string;
        return name;
    }

    case PropertyID::BorderLeft: {
        static FlyString name = "border-left"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftColor: {
        static FlyString name = "border-left-color"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftStyle: {
        static FlyString name = "border-left-style"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftWidth: {
        static FlyString name = "border-left-width"_fly_string;
        return name;
    }

    case PropertyID::BorderRadius: {
        static FlyString name = "border-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderRight: {
        static FlyString name = "border-right"_fly_string;
        return name;
    }

    case PropertyID::BorderRightColor: {
        static FlyString name = "border-right-color"_fly_string;
        return name;
    }

    case PropertyID::BorderRightStyle: {
        static FlyString name = "border-right-style"_fly_string;
        return name;
    }

    case PropertyID::BorderRightWidth: {
        static FlyString name = "border-right-width"_fly_string;
        return name;
    }

    case PropertyID::BorderSpacing: {
        static FlyString name = "border-spacing"_fly_string;
        return name;
    }

    case PropertyID::BorderStartEndRadius: {
        static FlyString name = "border-start-end-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderStartStartRadius: {
        static FlyString name = "border-start-start-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderStyle: {
        static FlyString name = "border-style"_fly_string;
        return name;
    }

    case PropertyID::BorderTop: {
        static FlyString name = "border-top"_fly_string;
        return name;
    }

    case PropertyID::BorderTopColor: {
        static FlyString name = "border-top-color"_fly_string;
        return name;
    }

    case PropertyID::BorderTopLeftRadius: {
        static FlyString name = "border-top-left-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderTopRightRadius: {
        static FlyString name = "border-top-right-radius"_fly_string;
        return name;
    }

    case PropertyID::BorderTopStyle: {
        static FlyString name = "border-top-style"_fly_string;
        return name;
    }

    case PropertyID::BorderTopWidth: {
        static FlyString name = "border-top-width"_fly_string;
        return name;
    }

    case PropertyID::BorderWidth: {
        static FlyString name = "border-width"_fly_string;
        return name;
    }

    case PropertyID::Bottom: {
        static FlyString name = "bottom"_fly_string;
        return name;
    }

    case PropertyID::BoxShadow: {
        static FlyString name = "box-shadow"_fly_string;
        return name;
    }

    case PropertyID::BoxSizing: {
        static FlyString name = "box-sizing"_fly_string;
        return name;
    }

    case PropertyID::CaptionSide: {
        static FlyString name = "caption-side"_fly_string;
        return name;
    }

    case PropertyID::CaretColor: {
        static FlyString name = "caret-color"_fly_string;
        return name;
    }

    case PropertyID::Clear: {
        static FlyString name = "clear"_fly_string;
        return name;
    }

    case PropertyID::Clip: {
        static FlyString name = "clip"_fly_string;
        return name;
    }

    case PropertyID::ClipPath: {
        static FlyString name = "clip-path"_fly_string;
        return name;
    }

    case PropertyID::ClipRule: {
        static FlyString name = "clip-rule"_fly_string;
        return name;
    }

    case PropertyID::Color: {
        static FlyString name = "color"_fly_string;
        return name;
    }

    case PropertyID::ColorInterpolation: {
        static FlyString name = "color-interpolation"_fly_string;
        return name;
    }

    case PropertyID::ColorScheme: {
        static FlyString name = "color-scheme"_fly_string;
        return name;
    }

    case PropertyID::ColumnCount: {
        static FlyString name = "column-count"_fly_string;
        return name;
    }

    case PropertyID::ColumnGap: {
        static FlyString name = "column-gap"_fly_string;
        return name;
    }

    case PropertyID::ColumnHeight: {
        static FlyString name = "column-height"_fly_string;
        return name;
    }

    case PropertyID::ColumnSpan: {
        static FlyString name = "column-span"_fly_string;
        return name;
    }

    case PropertyID::ColumnWidth: {
        static FlyString name = "column-width"_fly_string;
        return name;
    }

    case PropertyID::Columns: {
        static FlyString name = "columns"_fly_string;
        return name;
    }

    case PropertyID::Contain: {
        static FlyString name = "contain"_fly_string;
        return name;
    }

    case PropertyID::ContainerType: {
        static FlyString name = "container-type"_fly_string;
        return name;
    }

    case PropertyID::Content: {
        static FlyString name = "content"_fly_string;
        return name;
    }

    case PropertyID::ContentVisibility: {
        static FlyString name = "content-visibility"_fly_string;
        return name;
    }

    case PropertyID::CornerBlockEndShape: {
        static FlyString name = "corner-block-end-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerBlockStartShape: {
        static FlyString name = "corner-block-start-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomLeftShape: {
        static FlyString name = "corner-bottom-left-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomRightShape: {
        static FlyString name = "corner-bottom-right-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomShape: {
        static FlyString name = "corner-bottom-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerEndEndShape: {
        static FlyString name = "corner-end-end-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerEndStartShape: {
        static FlyString name = "corner-end-start-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerInlineEndShape: {
        static FlyString name = "corner-inline-end-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerInlineStartShape: {
        static FlyString name = "corner-inline-start-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerLeftShape: {
        static FlyString name = "corner-left-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerRightShape: {
        static FlyString name = "corner-right-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerShape: {
        static FlyString name = "corner-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerStartEndShape: {
        static FlyString name = "corner-start-end-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerStartStartShape: {
        static FlyString name = "corner-start-start-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopLeftShape: {
        static FlyString name = "corner-top-left-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopRightShape: {
        static FlyString name = "corner-top-right-shape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopShape: {
        static FlyString name = "corner-top-shape"_fly_string;
        return name;
    }

    case PropertyID::CounterIncrement: {
        static FlyString name = "counter-increment"_fly_string;
        return name;
    }

    case PropertyID::CounterReset: {
        static FlyString name = "counter-reset"_fly_string;
        return name;
    }

    case PropertyID::CounterSet: {
        static FlyString name = "counter-set"_fly_string;
        return name;
    }

    case PropertyID::Cursor: {
        static FlyString name = "cursor"_fly_string;
        return name;
    }

    case PropertyID::Cx: {
        static FlyString name = "cx"_fly_string;
        return name;
    }

    case PropertyID::Cy: {
        static FlyString name = "cy"_fly_string;
        return name;
    }

    case PropertyID::Direction: {
        static FlyString name = "direction"_fly_string;
        return name;
    }

    case PropertyID::Display: {
        static FlyString name = "display"_fly_string;
        return name;
    }

    case PropertyID::DominantBaseline: {
        static FlyString name = "dominant-baseline"_fly_string;
        return name;
    }

    case PropertyID::EmptyCells: {
        static FlyString name = "empty-cells"_fly_string;
        return name;
    }

    case PropertyID::Fill: {
        static FlyString name = "fill"_fly_string;
        return name;
    }

    case PropertyID::FillOpacity: {
        static FlyString name = "fill-opacity"_fly_string;
        return name;
    }

    case PropertyID::FillRule: {
        static FlyString name = "fill-rule"_fly_string;
        return name;
    }

    case PropertyID::Filter: {
        static FlyString name = "filter"_fly_string;
        return name;
    }

    case PropertyID::Flex: {
        static FlyString name = "flex"_fly_string;
        return name;
    }

    case PropertyID::FlexBasis: {
        static FlyString name = "flex-basis"_fly_string;
        return name;
    }

    case PropertyID::FlexDirection: {
        static FlyString name = "flex-direction"_fly_string;
        return name;
    }

    case PropertyID::FlexFlow: {
        static FlyString name = "flex-flow"_fly_string;
        return name;
    }

    case PropertyID::FlexGrow: {
        static FlyString name = "flex-grow"_fly_string;
        return name;
    }

    case PropertyID::FlexShrink: {
        static FlyString name = "flex-shrink"_fly_string;
        return name;
    }

    case PropertyID::FlexWrap: {
        static FlyString name = "flex-wrap"_fly_string;
        return name;
    }

    case PropertyID::Float: {
        static FlyString name = "float"_fly_string;
        return name;
    }

    case PropertyID::FloodColor: {
        static FlyString name = "flood-color"_fly_string;
        return name;
    }

    case PropertyID::FloodOpacity: {
        static FlyString name = "flood-opacity"_fly_string;
        return name;
    }

    case PropertyID::Font: {
        static FlyString name = "font"_fly_string;
        return name;
    }

    case PropertyID::FontFamily: {
        static FlyString name = "font-family"_fly_string;
        return name;
    }

    case PropertyID::FontFeatureSettings: {
        static FlyString name = "font-feature-settings"_fly_string;
        return name;
    }

    case PropertyID::FontKerning: {
        static FlyString name = "font-kerning"_fly_string;
        return name;
    }

    case PropertyID::FontLanguageOverride: {
        static FlyString name = "font-language-override"_fly_string;
        return name;
    }

    case PropertyID::FontOpticalSizing: {
        static FlyString name = "font-optical-sizing"_fly_string;
        return name;
    }

    case PropertyID::FontSize: {
        static FlyString name = "font-size"_fly_string;
        return name;
    }

    case PropertyID::FontStyle: {
        static FlyString name = "font-style"_fly_string;
        return name;
    }

    case PropertyID::FontVariant: {
        static FlyString name = "font-variant"_fly_string;
        return name;
    }

    case PropertyID::FontVariantAlternates: {
        static FlyString name = "font-variant-alternates"_fly_string;
        return name;
    }

    case PropertyID::FontVariantCaps: {
        static FlyString name = "font-variant-caps"_fly_string;
        return name;
    }

    case PropertyID::FontVariantEastAsian: {
        static FlyString name = "font-variant-east-asian"_fly_string;
        return name;
    }

    case PropertyID::FontVariantEmoji: {
        static FlyString name = "font-variant-emoji"_fly_string;
        return name;
    }

    case PropertyID::FontVariantLigatures: {
        static FlyString name = "font-variant-ligatures"_fly_string;
        return name;
    }

    case PropertyID::FontVariantNumeric: {
        static FlyString name = "font-variant-numeric"_fly_string;
        return name;
    }

    case PropertyID::FontVariantPosition: {
        static FlyString name = "font-variant-position"_fly_string;
        return name;
    }

    case PropertyID::FontVariationSettings: {
        static FlyString name = "font-variation-settings"_fly_string;
        return name;
    }

    case PropertyID::FontWeight: {
        static FlyString name = "font-weight"_fly_string;
        return name;
    }

    case PropertyID::FontWidth: {
        static FlyString name = "font-width"_fly_string;
        return name;
    }

    case PropertyID::Gap: {
        static FlyString name = "gap"_fly_string;
        return name;
    }

    case PropertyID::Grid: {
        static FlyString name = "grid"_fly_string;
        return name;
    }

    case PropertyID::GridArea: {
        static FlyString name = "grid-area"_fly_string;
        return name;
    }

    case PropertyID::GridAutoColumns: {
        static FlyString name = "grid-auto-columns"_fly_string;
        return name;
    }

    case PropertyID::GridAutoFlow: {
        static FlyString name = "grid-auto-flow"_fly_string;
        return name;
    }

    case PropertyID::GridAutoRows: {
        static FlyString name = "grid-auto-rows"_fly_string;
        return name;
    }

    case PropertyID::GridColumn: {
        static FlyString name = "grid-column"_fly_string;
        return name;
    }

    case PropertyID::GridColumnEnd: {
        static FlyString name = "grid-column-end"_fly_string;
        return name;
    }

    case PropertyID::GridColumnStart: {
        static FlyString name = "grid-column-start"_fly_string;
        return name;
    }

    case PropertyID::GridRow: {
        static FlyString name = "grid-row"_fly_string;
        return name;
    }

    case PropertyID::GridRowEnd: {
        static FlyString name = "grid-row-end"_fly_string;
        return name;
    }

    case PropertyID::GridRowStart: {
        static FlyString name = "grid-row-start"_fly_string;
        return name;
    }

    case PropertyID::GridTemplate: {
        static FlyString name = "grid-template"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateAreas: {
        static FlyString name = "grid-template-areas"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateColumns: {
        static FlyString name = "grid-template-columns"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateRows: {
        static FlyString name = "grid-template-rows"_fly_string;
        return name;
    }

    case PropertyID::Height: {
        static FlyString name = "height"_fly_string;
        return name;
    }

    case PropertyID::ImageRendering: {
        static FlyString name = "image-rendering"_fly_string;
        return name;
    }

    case PropertyID::InlineSize: {
        static FlyString name = "inline-size"_fly_string;
        return name;
    }

    case PropertyID::Inset: {
        static FlyString name = "inset"_fly_string;
        return name;
    }

    case PropertyID::InsetBlock: {
        static FlyString name = "inset-block"_fly_string;
        return name;
    }

    case PropertyID::InsetBlockEnd: {
        static FlyString name = "inset-block-end"_fly_string;
        return name;
    }

    case PropertyID::InsetBlockStart: {
        static FlyString name = "inset-block-start"_fly_string;
        return name;
    }

    case PropertyID::InsetInline: {
        static FlyString name = "inset-inline"_fly_string;
        return name;
    }

    case PropertyID::InsetInlineEnd: {
        static FlyString name = "inset-inline-end"_fly_string;
        return name;
    }

    case PropertyID::InsetInlineStart: {
        static FlyString name = "inset-inline-start"_fly_string;
        return name;
    }

    case PropertyID::Isolation: {
        static FlyString name = "isolation"_fly_string;
        return name;
    }

    case PropertyID::JustifyContent: {
        static FlyString name = "justify-content"_fly_string;
        return name;
    }

    case PropertyID::JustifyItems: {
        static FlyString name = "justify-items"_fly_string;
        return name;
    }

    case PropertyID::JustifySelf: {
        static FlyString name = "justify-self"_fly_string;
        return name;
    }

    case PropertyID::Left: {
        static FlyString name = "left"_fly_string;
        return name;
    }

    case PropertyID::LetterSpacing: {
        static FlyString name = "letter-spacing"_fly_string;
        return name;
    }

    case PropertyID::LineHeight: {
        static FlyString name = "line-height"_fly_string;
        return name;
    }

    case PropertyID::ListStyle: {
        static FlyString name = "list-style"_fly_string;
        return name;
    }

    case PropertyID::ListStyleImage: {
        static FlyString name = "list-style-image"_fly_string;
        return name;
    }

    case PropertyID::ListStylePosition: {
        static FlyString name = "list-style-position"_fly_string;
        return name;
    }

    case PropertyID::ListStyleType: {
        static FlyString name = "list-style-type"_fly_string;
        return name;
    }

    case PropertyID::Margin: {
        static FlyString name = "margin"_fly_string;
        return name;
    }

    case PropertyID::MarginBlock: {
        static FlyString name = "margin-block"_fly_string;
        return name;
    }

    case PropertyID::MarginBlockEnd: {
        static FlyString name = "margin-block-end"_fly_string;
        return name;
    }

    case PropertyID::MarginBlockStart: {
        static FlyString name = "margin-block-start"_fly_string;
        return name;
    }

    case PropertyID::MarginBottom: {
        static FlyString name = "margin-bottom"_fly_string;
        return name;
    }

    case PropertyID::MarginInline: {
        static FlyString name = "margin-inline"_fly_string;
        return name;
    }

    case PropertyID::MarginInlineEnd: {
        static FlyString name = "margin-inline-end"_fly_string;
        return name;
    }

    case PropertyID::MarginInlineStart: {
        static FlyString name = "margin-inline-start"_fly_string;
        return name;
    }

    case PropertyID::MarginLeft: {
        static FlyString name = "margin-left"_fly_string;
        return name;
    }

    case PropertyID::MarginRight: {
        static FlyString name = "margin-right"_fly_string;
        return name;
    }

    case PropertyID::MarginTop: {
        static FlyString name = "margin-top"_fly_string;
        return name;
    }

    case PropertyID::Mask: {
        static FlyString name = "mask"_fly_string;
        return name;
    }

    case PropertyID::MaskClip: {
        static FlyString name = "mask-clip"_fly_string;
        return name;
    }

    case PropertyID::MaskComposite: {
        static FlyString name = "mask-composite"_fly_string;
        return name;
    }

    case PropertyID::MaskImage: {
        static FlyString name = "mask-image"_fly_string;
        return name;
    }

    case PropertyID::MaskMode: {
        static FlyString name = "mask-mode"_fly_string;
        return name;
    }

    case PropertyID::MaskOrigin: {
        static FlyString name = "mask-origin"_fly_string;
        return name;
    }

    case PropertyID::MaskPosition: {
        static FlyString name = "mask-position"_fly_string;
        return name;
    }

    case PropertyID::MaskRepeat: {
        static FlyString name = "mask-repeat"_fly_string;
        return name;
    }

    case PropertyID::MaskSize: {
        static FlyString name = "mask-size"_fly_string;
        return name;
    }

    case PropertyID::MaskType: {
        static FlyString name = "mask-type"_fly_string;
        return name;
    }

    case PropertyID::MathDepth: {
        static FlyString name = "math-depth"_fly_string;
        return name;
    }

    case PropertyID::MathShift: {
        static FlyString name = "math-shift"_fly_string;
        return name;
    }

    case PropertyID::MathStyle: {
        static FlyString name = "math-style"_fly_string;
        return name;
    }

    case PropertyID::MaxBlockSize: {
        static FlyString name = "max-block-size"_fly_string;
        return name;
    }

    case PropertyID::MaxHeight: {
        static FlyString name = "max-height"_fly_string;
        return name;
    }

    case PropertyID::MaxInlineSize: {
        static FlyString name = "max-inline-size"_fly_string;
        return name;
    }

    case PropertyID::MaxWidth: {
        static FlyString name = "max-width"_fly_string;
        return name;
    }

    case PropertyID::MinBlockSize: {
        static FlyString name = "min-block-size"_fly_string;
        return name;
    }

    case PropertyID::MinHeight: {
        static FlyString name = "min-height"_fly_string;
        return name;
    }

    case PropertyID::MinInlineSize: {
        static FlyString name = "min-inline-size"_fly_string;
        return name;
    }

    case PropertyID::MinWidth: {
        static FlyString name = "min-width"_fly_string;
        return name;
    }

    case PropertyID::MixBlendMode: {
        static FlyString name = "mix-blend-mode"_fly_string;
        return name;
    }

    case PropertyID::ObjectFit: {
        static FlyString name = "object-fit"_fly_string;
        return name;
    }

    case PropertyID::ObjectPosition: {
        static FlyString name = "object-position"_fly_string;
        return name;
    }

    case PropertyID::Opacity: {
        static FlyString name = "opacity"_fly_string;
        return name;
    }

    case PropertyID::Order: {
        static FlyString name = "order"_fly_string;
        return name;
    }

    case PropertyID::Orphans: {
        static FlyString name = "orphans"_fly_string;
        return name;
    }

    case PropertyID::Outline: {
        static FlyString name = "outline"_fly_string;
        return name;
    }

    case PropertyID::OutlineColor: {
        static FlyString name = "outline-color"_fly_string;
        return name;
    }

    case PropertyID::OutlineOffset: {
        static FlyString name = "outline-offset"_fly_string;
        return name;
    }

    case PropertyID::OutlineStyle: {
        static FlyString name = "outline-style"_fly_string;
        return name;
    }

    case PropertyID::OutlineWidth: {
        static FlyString name = "outline-width"_fly_string;
        return name;
    }

    case PropertyID::Overflow: {
        static FlyString name = "overflow"_fly_string;
        return name;
    }

    case PropertyID::OverflowBlock: {
        static FlyString name = "overflow-block"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMargin: {
        static FlyString name = "overflow-clip-margin"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlock: {
        static FlyString name = "overflow-clip-margin-block"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlockEnd: {
        static FlyString name = "overflow-clip-margin-block-end"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlockStart: {
        static FlyString name = "overflow-clip-margin-block-start"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBottom: {
        static FlyString name = "overflow-clip-margin-bottom"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInline: {
        static FlyString name = "overflow-clip-margin-inline"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInlineEnd: {
        static FlyString name = "overflow-clip-margin-inline-end"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInlineStart: {
        static FlyString name = "overflow-clip-margin-inline-start"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginLeft: {
        static FlyString name = "overflow-clip-margin-left"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginRight: {
        static FlyString name = "overflow-clip-margin-right"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginTop: {
        static FlyString name = "overflow-clip-margin-top"_fly_string;
        return name;
    }

    case PropertyID::OverflowInline: {
        static FlyString name = "overflow-inline"_fly_string;
        return name;
    }

    case PropertyID::OverflowWrap: {
        static FlyString name = "overflow-wrap"_fly_string;
        return name;
    }

    case PropertyID::OverflowX: {
        static FlyString name = "overflow-x"_fly_string;
        return name;
    }

    case PropertyID::OverflowY: {
        static FlyString name = "overflow-y"_fly_string;
        return name;
    }

    case PropertyID::Padding: {
        static FlyString name = "padding"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlock: {
        static FlyString name = "padding-block"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlockEnd: {
        static FlyString name = "padding-block-end"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlockStart: {
        static FlyString name = "padding-block-start"_fly_string;
        return name;
    }

    case PropertyID::PaddingBottom: {
        static FlyString name = "padding-bottom"_fly_string;
        return name;
    }

    case PropertyID::PaddingInline: {
        static FlyString name = "padding-inline"_fly_string;
        return name;
    }

    case PropertyID::PaddingInlineEnd: {
        static FlyString name = "padding-inline-end"_fly_string;
        return name;
    }

    case PropertyID::PaddingInlineStart: {
        static FlyString name = "padding-inline-start"_fly_string;
        return name;
    }

    case PropertyID::PaddingLeft: {
        static FlyString name = "padding-left"_fly_string;
        return name;
    }

    case PropertyID::PaddingRight: {
        static FlyString name = "padding-right"_fly_string;
        return name;
    }

    case PropertyID::PaddingTop: {
        static FlyString name = "padding-top"_fly_string;
        return name;
    }

    case PropertyID::PaintOrder: {
        static FlyString name = "paint-order"_fly_string;
        return name;
    }

    case PropertyID::Perspective: {
        static FlyString name = "perspective"_fly_string;
        return name;
    }

    case PropertyID::PerspectiveOrigin: {
        static FlyString name = "perspective-origin"_fly_string;
        return name;
    }

    case PropertyID::PlaceContent: {
        static FlyString name = "place-content"_fly_string;
        return name;
    }

    case PropertyID::PlaceItems: {
        static FlyString name = "place-items"_fly_string;
        return name;
    }

    case PropertyID::PlaceSelf: {
        static FlyString name = "place-self"_fly_string;
        return name;
    }

    case PropertyID::PointerEvents: {
        static FlyString name = "pointer-events"_fly_string;
        return name;
    }

    case PropertyID::Position: {
        static FlyString name = "position"_fly_string;
        return name;
    }

    case PropertyID::PositionAnchor: {
        static FlyString name = "position-anchor"_fly_string;
        return name;
    }

    case PropertyID::PositionArea: {
        static FlyString name = "position-area"_fly_string;
        return name;
    }

    case PropertyID::PositionTryFallbacks: {
        static FlyString name = "position-try-fallbacks"_fly_string;
        return name;
    }

    case PropertyID::PositionTryOrder: {
        static FlyString name = "position-try-order"_fly_string;
        return name;
    }

    case PropertyID::PositionVisibility: {
        static FlyString name = "position-visibility"_fly_string;
        return name;
    }

    case PropertyID::Quotes: {
        static FlyString name = "quotes"_fly_string;
        return name;
    }

    case PropertyID::R: {
        static FlyString name = "r"_fly_string;
        return name;
    }

    case PropertyID::Resize: {
        static FlyString name = "resize"_fly_string;
        return name;
    }

    case PropertyID::Right: {
        static FlyString name = "right"_fly_string;
        return name;
    }

    case PropertyID::Rotate: {
        static FlyString name = "rotate"_fly_string;
        return name;
    }

    case PropertyID::RowGap: {
        static FlyString name = "row-gap"_fly_string;
        return name;
    }

    case PropertyID::Rx: {
        static FlyString name = "rx"_fly_string;
        return name;
    }

    case PropertyID::Ry: {
        static FlyString name = "ry"_fly_string;
        return name;
    }

    case PropertyID::Scale: {
        static FlyString name = "scale"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimeline: {
        static FlyString name = "scroll-timeline"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimelineAxis: {
        static FlyString name = "scroll-timeline-axis"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimelineName: {
        static FlyString name = "scroll-timeline-name"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarColor: {
        static FlyString name = "scrollbar-color"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarGutter: {
        static FlyString name = "scrollbar-gutter"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarWidth: {
        static FlyString name = "scrollbar-width"_fly_string;
        return name;
    }

    case PropertyID::ShapeImageThreshold: {
        static FlyString name = "shape-image-threshold"_fly_string;
        return name;
    }

    case PropertyID::ShapeMargin: {
        static FlyString name = "shape-margin"_fly_string;
        return name;
    }

    case PropertyID::ShapeOutside: {
        static FlyString name = "shape-outside"_fly_string;
        return name;
    }

    case PropertyID::ShapeRendering: {
        static FlyString name = "shape-rendering"_fly_string;
        return name;
    }

    case PropertyID::StopColor: {
        static FlyString name = "stop-color"_fly_string;
        return name;
    }

    case PropertyID::StopOpacity: {
        static FlyString name = "stop-opacity"_fly_string;
        return name;
    }

    case PropertyID::Stroke: {
        static FlyString name = "stroke"_fly_string;
        return name;
    }

    case PropertyID::StrokeDasharray: {
        static FlyString name = "stroke-dasharray"_fly_string;
        return name;
    }

    case PropertyID::StrokeDashoffset: {
        static FlyString name = "stroke-dashoffset"_fly_string;
        return name;
    }

    case PropertyID::StrokeLinecap: {
        static FlyString name = "stroke-linecap"_fly_string;
        return name;
    }

    case PropertyID::StrokeLinejoin: {
        static FlyString name = "stroke-linejoin"_fly_string;
        return name;
    }

    case PropertyID::StrokeMiterlimit: {
        static FlyString name = "stroke-miterlimit"_fly_string;
        return name;
    }

    case PropertyID::StrokeOpacity: {
        static FlyString name = "stroke-opacity"_fly_string;
        return name;
    }

    case PropertyID::StrokeWidth: {
        static FlyString name = "stroke-width"_fly_string;
        return name;
    }

    case PropertyID::TabSize: {
        static FlyString name = "tab-size"_fly_string;
        return name;
    }

    case PropertyID::TableLayout: {
        static FlyString name = "table-layout"_fly_string;
        return name;
    }

    case PropertyID::TextAlign: {
        static FlyString name = "text-align"_fly_string;
        return name;
    }

    case PropertyID::TextAnchor: {
        static FlyString name = "text-anchor"_fly_string;
        return name;
    }

    case PropertyID::TextDecoration: {
        static FlyString name = "text-decoration"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationColor: {
        static FlyString name = "text-decoration-color"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationLine: {
        static FlyString name = "text-decoration-line"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationStyle: {
        static FlyString name = "text-decoration-style"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationThickness: {
        static FlyString name = "text-decoration-thickness"_fly_string;
        return name;
    }

    case PropertyID::TextIndent: {
        static FlyString name = "text-indent"_fly_string;
        return name;
    }

    case PropertyID::TextJustify: {
        static FlyString name = "text-justify"_fly_string;
        return name;
    }

    case PropertyID::TextOverflow: {
        static FlyString name = "text-overflow"_fly_string;
        return name;
    }

    case PropertyID::TextRendering: {
        static FlyString name = "text-rendering"_fly_string;
        return name;
    }

    case PropertyID::TextShadow: {
        static FlyString name = "text-shadow"_fly_string;
        return name;
    }

    case PropertyID::TextTransform: {
        static FlyString name = "text-transform"_fly_string;
        return name;
    }

    case PropertyID::TextUnderlineOffset: {
        static FlyString name = "text-underline-offset"_fly_string;
        return name;
    }

    case PropertyID::TextUnderlinePosition: {
        static FlyString name = "text-underline-position"_fly_string;
        return name;
    }

    case PropertyID::TextWrap: {
        static FlyString name = "text-wrap"_fly_string;
        return name;
    }

    case PropertyID::TextWrapMode: {
        static FlyString name = "text-wrap-mode"_fly_string;
        return name;
    }

    case PropertyID::TextWrapStyle: {
        static FlyString name = "text-wrap-style"_fly_string;
        return name;
    }

    case PropertyID::TimelineScope: {
        static FlyString name = "timeline-scope"_fly_string;
        return name;
    }

    case PropertyID::Top: {
        static FlyString name = "top"_fly_string;
        return name;
    }

    case PropertyID::TouchAction: {
        static FlyString name = "touch-action"_fly_string;
        return name;
    }

    case PropertyID::Transform: {
        static FlyString name = "transform"_fly_string;
        return name;
    }

    case PropertyID::TransformBox: {
        static FlyString name = "transform-box"_fly_string;
        return name;
    }

    case PropertyID::TransformOrigin: {
        static FlyString name = "transform-origin"_fly_string;
        return name;
    }

    case PropertyID::TransformStyle: {
        static FlyString name = "transform-style"_fly_string;
        return name;
    }

    case PropertyID::Transition: {
        static FlyString name = "transition"_fly_string;
        return name;
    }

    case PropertyID::TransitionBehavior: {
        static FlyString name = "transition-behavior"_fly_string;
        return name;
    }

    case PropertyID::TransitionDelay: {
        static FlyString name = "transition-delay"_fly_string;
        return name;
    }

    case PropertyID::TransitionDuration: {
        static FlyString name = "transition-duration"_fly_string;
        return name;
    }

    case PropertyID::TransitionProperty: {
        static FlyString name = "transition-property"_fly_string;
        return name;
    }

    case PropertyID::TransitionTimingFunction: {
        static FlyString name = "transition-timing-function"_fly_string;
        return name;
    }

    case PropertyID::Translate: {
        static FlyString name = "translate"_fly_string;
        return name;
    }

    case PropertyID::UnicodeBidi: {
        static FlyString name = "unicode-bidi"_fly_string;
        return name;
    }

    case PropertyID::UserSelect: {
        static FlyString name = "user-select"_fly_string;
        return name;
    }

    case PropertyID::VerticalAlign: {
        static FlyString name = "vertical-align"_fly_string;
        return name;
    }

    case PropertyID::ViewTimeline: {
        static FlyString name = "view-timeline"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineAxis: {
        static FlyString name = "view-timeline-axis"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineInset: {
        static FlyString name = "view-timeline-inset"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineName: {
        static FlyString name = "view-timeline-name"_fly_string;
        return name;
    }

    case PropertyID::ViewTransitionName: {
        static FlyString name = "view-transition-name"_fly_string;
        return name;
    }

    case PropertyID::Visibility: {
        static FlyString name = "visibility"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpace: {
        static FlyString name = "white-space"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpaceCollapse: {
        static FlyString name = "white-space-collapse"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpaceTrim: {
        static FlyString name = "white-space-trim"_fly_string;
        return name;
    }

    case PropertyID::Widows: {
        static FlyString name = "widows"_fly_string;
        return name;
    }

    case PropertyID::Width: {
        static FlyString name = "width"_fly_string;
        return name;
    }

    case PropertyID::WillChange: {
        static FlyString name = "will-change"_fly_string;
        return name;
    }

    case PropertyID::WordBreak: {
        static FlyString name = "word-break"_fly_string;
        return name;
    }

    case PropertyID::WordSpacing: {
        static FlyString name = "word-spacing"_fly_string;
        return name;
    }

    case PropertyID::WritingMode: {
        static FlyString name = "writing-mode"_fly_string;
        return name;
    }

    case PropertyID::X: {
        static FlyString name = "x"_fly_string;
        return name;
    }

    case PropertyID::Y: {
        static FlyString name = "y"_fly_string;
        return name;
    }

    case PropertyID::ZIndex: {
        static FlyString name = "z-index"_fly_string;
        return name;
    }

    default: {
        static FlyString invalid_property_id_string = "(invalid CSS::PropertyID)"_fly_string;
        return invalid_property_id_string;
    }
    }
}

FlyString const& camel_case_string_from_property_id(PropertyID property_id) {
    switch (property_id) {

    case PropertyID::WebkitTextFillColor: {
        static FlyString name = "webkitTextFillColor"_fly_string;
        return name;
    }

    case PropertyID::AccentColor: {
        static FlyString name = "accentColor"_fly_string;
        return name;
    }

    case PropertyID::AlignContent: {
        static FlyString name = "alignContent"_fly_string;
        return name;
    }

    case PropertyID::AlignItems: {
        static FlyString name = "alignItems"_fly_string;
        return name;
    }

    case PropertyID::AlignSelf: {
        static FlyString name = "alignSelf"_fly_string;
        return name;
    }

    case PropertyID::All: {
        static FlyString name = "all"_fly_string;
        return name;
    }

    case PropertyID::AnchorName: {
        static FlyString name = "anchorName"_fly_string;
        return name;
    }

    case PropertyID::AnchorScope: {
        static FlyString name = "anchorScope"_fly_string;
        return name;
    }

    case PropertyID::Animation: {
        static FlyString name = "animation"_fly_string;
        return name;
    }

    case PropertyID::AnimationComposition: {
        static FlyString name = "animationComposition"_fly_string;
        return name;
    }

    case PropertyID::AnimationDelay: {
        static FlyString name = "animationDelay"_fly_string;
        return name;
    }

    case PropertyID::AnimationDirection: {
        static FlyString name = "animationDirection"_fly_string;
        return name;
    }

    case PropertyID::AnimationDuration: {
        static FlyString name = "animationDuration"_fly_string;
        return name;
    }

    case PropertyID::AnimationFillMode: {
        static FlyString name = "animationFillMode"_fly_string;
        return name;
    }

    case PropertyID::AnimationIterationCount: {
        static FlyString name = "animationIterationCount"_fly_string;
        return name;
    }

    case PropertyID::AnimationName: {
        static FlyString name = "animationName"_fly_string;
        return name;
    }

    case PropertyID::AnimationPlayState: {
        static FlyString name = "animationPlayState"_fly_string;
        return name;
    }

    case PropertyID::AnimationTimeline: {
        static FlyString name = "animationTimeline"_fly_string;
        return name;
    }

    case PropertyID::AnimationTimingFunction: {
        static FlyString name = "animationTimingFunction"_fly_string;
        return name;
    }

    case PropertyID::Appearance: {
        static FlyString name = "appearance"_fly_string;
        return name;
    }

    case PropertyID::AspectRatio: {
        static FlyString name = "aspectRatio"_fly_string;
        return name;
    }

    case PropertyID::BackdropFilter: {
        static FlyString name = "backdropFilter"_fly_string;
        return name;
    }

    case PropertyID::Background: {
        static FlyString name = "background"_fly_string;
        return name;
    }

    case PropertyID::BackgroundAttachment: {
        static FlyString name = "backgroundAttachment"_fly_string;
        return name;
    }

    case PropertyID::BackgroundBlendMode: {
        static FlyString name = "backgroundBlendMode"_fly_string;
        return name;
    }

    case PropertyID::BackgroundClip: {
        static FlyString name = "backgroundClip"_fly_string;
        return name;
    }

    case PropertyID::BackgroundColor: {
        static FlyString name = "backgroundColor"_fly_string;
        return name;
    }

    case PropertyID::BackgroundImage: {
        static FlyString name = "backgroundImage"_fly_string;
        return name;
    }

    case PropertyID::BackgroundOrigin: {
        static FlyString name = "backgroundOrigin"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPosition: {
        static FlyString name = "backgroundPosition"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPositionX: {
        static FlyString name = "backgroundPositionX"_fly_string;
        return name;
    }

    case PropertyID::BackgroundPositionY: {
        static FlyString name = "backgroundPositionY"_fly_string;
        return name;
    }

    case PropertyID::BackgroundRepeat: {
        static FlyString name = "backgroundRepeat"_fly_string;
        return name;
    }

    case PropertyID::BackgroundSize: {
        static FlyString name = "backgroundSize"_fly_string;
        return name;
    }

    case PropertyID::BlockSize: {
        static FlyString name = "blockSize"_fly_string;
        return name;
    }

    case PropertyID::Border: {
        static FlyString name = "border"_fly_string;
        return name;
    }

    case PropertyID::BorderBlock: {
        static FlyString name = "borderBlock"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockColor: {
        static FlyString name = "borderBlockColor"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEnd: {
        static FlyString name = "borderBlockEnd"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndColor: {
        static FlyString name = "borderBlockEndColor"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndStyle: {
        static FlyString name = "borderBlockEndStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockEndWidth: {
        static FlyString name = "borderBlockEndWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStart: {
        static FlyString name = "borderBlockStart"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartColor: {
        static FlyString name = "borderBlockStartColor"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartStyle: {
        static FlyString name = "borderBlockStartStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStartWidth: {
        static FlyString name = "borderBlockStartWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockStyle: {
        static FlyString name = "borderBlockStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderBlockWidth: {
        static FlyString name = "borderBlockWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderBottom: {
        static FlyString name = "borderBottom"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomColor: {
        static FlyString name = "borderBottomColor"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomLeftRadius: {
        static FlyString name = "borderBottomLeftRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomRightRadius: {
        static FlyString name = "borderBottomRightRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomStyle: {
        static FlyString name = "borderBottomStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderBottomWidth: {
        static FlyString name = "borderBottomWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderCollapse: {
        static FlyString name = "borderCollapse"_fly_string;
        return name;
    }

    case PropertyID::BorderColor: {
        static FlyString name = "borderColor"_fly_string;
        return name;
    }

    case PropertyID::BorderEndEndRadius: {
        static FlyString name = "borderEndEndRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderEndStartRadius: {
        static FlyString name = "borderEndStartRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderImage: {
        static FlyString name = "borderImage"_fly_string;
        return name;
    }

    case PropertyID::BorderImageOutset: {
        static FlyString name = "borderImageOutset"_fly_string;
        return name;
    }

    case PropertyID::BorderImageRepeat: {
        static FlyString name = "borderImageRepeat"_fly_string;
        return name;
    }

    case PropertyID::BorderImageSlice: {
        static FlyString name = "borderImageSlice"_fly_string;
        return name;
    }

    case PropertyID::BorderImageSource: {
        static FlyString name = "borderImageSource"_fly_string;
        return name;
    }

    case PropertyID::BorderImageWidth: {
        static FlyString name = "borderImageWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderInline: {
        static FlyString name = "borderInline"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineColor: {
        static FlyString name = "borderInlineColor"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEnd: {
        static FlyString name = "borderInlineEnd"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndColor: {
        static FlyString name = "borderInlineEndColor"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndStyle: {
        static FlyString name = "borderInlineEndStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineEndWidth: {
        static FlyString name = "borderInlineEndWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStart: {
        static FlyString name = "borderInlineStart"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartColor: {
        static FlyString name = "borderInlineStartColor"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartStyle: {
        static FlyString name = "borderInlineStartStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStartWidth: {
        static FlyString name = "borderInlineStartWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineStyle: {
        static FlyString name = "borderInlineStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderInlineWidth: {
        static FlyString name = "borderInlineWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderLeft: {
        static FlyString name = "borderLeft"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftColor: {
        static FlyString name = "borderLeftColor"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftStyle: {
        static FlyString name = "borderLeftStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderLeftWidth: {
        static FlyString name = "borderLeftWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderRadius: {
        static FlyString name = "borderRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderRight: {
        static FlyString name = "borderRight"_fly_string;
        return name;
    }

    case PropertyID::BorderRightColor: {
        static FlyString name = "borderRightColor"_fly_string;
        return name;
    }

    case PropertyID::BorderRightStyle: {
        static FlyString name = "borderRightStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderRightWidth: {
        static FlyString name = "borderRightWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderSpacing: {
        static FlyString name = "borderSpacing"_fly_string;
        return name;
    }

    case PropertyID::BorderStartEndRadius: {
        static FlyString name = "borderStartEndRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderStartStartRadius: {
        static FlyString name = "borderStartStartRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderStyle: {
        static FlyString name = "borderStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderTop: {
        static FlyString name = "borderTop"_fly_string;
        return name;
    }

    case PropertyID::BorderTopColor: {
        static FlyString name = "borderTopColor"_fly_string;
        return name;
    }

    case PropertyID::BorderTopLeftRadius: {
        static FlyString name = "borderTopLeftRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderTopRightRadius: {
        static FlyString name = "borderTopRightRadius"_fly_string;
        return name;
    }

    case PropertyID::BorderTopStyle: {
        static FlyString name = "borderTopStyle"_fly_string;
        return name;
    }

    case PropertyID::BorderTopWidth: {
        static FlyString name = "borderTopWidth"_fly_string;
        return name;
    }

    case PropertyID::BorderWidth: {
        static FlyString name = "borderWidth"_fly_string;
        return name;
    }

    case PropertyID::Bottom: {
        static FlyString name = "bottom"_fly_string;
        return name;
    }

    case PropertyID::BoxShadow: {
        static FlyString name = "boxShadow"_fly_string;
        return name;
    }

    case PropertyID::BoxSizing: {
        static FlyString name = "boxSizing"_fly_string;
        return name;
    }

    case PropertyID::CaptionSide: {
        static FlyString name = "captionSide"_fly_string;
        return name;
    }

    case PropertyID::CaretColor: {
        static FlyString name = "caretColor"_fly_string;
        return name;
    }

    case PropertyID::Clear: {
        static FlyString name = "clear"_fly_string;
        return name;
    }

    case PropertyID::Clip: {
        static FlyString name = "clip"_fly_string;
        return name;
    }

    case PropertyID::ClipPath: {
        static FlyString name = "clipPath"_fly_string;
        return name;
    }

    case PropertyID::ClipRule: {
        static FlyString name = "clipRule"_fly_string;
        return name;
    }

    case PropertyID::Color: {
        static FlyString name = "color"_fly_string;
        return name;
    }

    case PropertyID::ColorInterpolation: {
        static FlyString name = "colorInterpolation"_fly_string;
        return name;
    }

    case PropertyID::ColorScheme: {
        static FlyString name = "colorScheme"_fly_string;
        return name;
    }

    case PropertyID::ColumnCount: {
        static FlyString name = "columnCount"_fly_string;
        return name;
    }

    case PropertyID::ColumnGap: {
        static FlyString name = "columnGap"_fly_string;
        return name;
    }

    case PropertyID::ColumnHeight: {
        static FlyString name = "columnHeight"_fly_string;
        return name;
    }

    case PropertyID::ColumnSpan: {
        static FlyString name = "columnSpan"_fly_string;
        return name;
    }

    case PropertyID::ColumnWidth: {
        static FlyString name = "columnWidth"_fly_string;
        return name;
    }

    case PropertyID::Columns: {
        static FlyString name = "columns"_fly_string;
        return name;
    }

    case PropertyID::Contain: {
        static FlyString name = "contain"_fly_string;
        return name;
    }

    case PropertyID::ContainerType: {
        static FlyString name = "containerType"_fly_string;
        return name;
    }

    case PropertyID::Content: {
        static FlyString name = "content"_fly_string;
        return name;
    }

    case PropertyID::ContentVisibility: {
        static FlyString name = "contentVisibility"_fly_string;
        return name;
    }

    case PropertyID::CornerBlockEndShape: {
        static FlyString name = "cornerBlockEndShape"_fly_string;
        return name;
    }

    case PropertyID::CornerBlockStartShape: {
        static FlyString name = "cornerBlockStartShape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomLeftShape: {
        static FlyString name = "cornerBottomLeftShape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomRightShape: {
        static FlyString name = "cornerBottomRightShape"_fly_string;
        return name;
    }

    case PropertyID::CornerBottomShape: {
        static FlyString name = "cornerBottomShape"_fly_string;
        return name;
    }

    case PropertyID::CornerEndEndShape: {
        static FlyString name = "cornerEndEndShape"_fly_string;
        return name;
    }

    case PropertyID::CornerEndStartShape: {
        static FlyString name = "cornerEndStartShape"_fly_string;
        return name;
    }

    case PropertyID::CornerInlineEndShape: {
        static FlyString name = "cornerInlineEndShape"_fly_string;
        return name;
    }

    case PropertyID::CornerInlineStartShape: {
        static FlyString name = "cornerInlineStartShape"_fly_string;
        return name;
    }

    case PropertyID::CornerLeftShape: {
        static FlyString name = "cornerLeftShape"_fly_string;
        return name;
    }

    case PropertyID::CornerRightShape: {
        static FlyString name = "cornerRightShape"_fly_string;
        return name;
    }

    case PropertyID::CornerShape: {
        static FlyString name = "cornerShape"_fly_string;
        return name;
    }

    case PropertyID::CornerStartEndShape: {
        static FlyString name = "cornerStartEndShape"_fly_string;
        return name;
    }

    case PropertyID::CornerStartStartShape: {
        static FlyString name = "cornerStartStartShape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopLeftShape: {
        static FlyString name = "cornerTopLeftShape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopRightShape: {
        static FlyString name = "cornerTopRightShape"_fly_string;
        return name;
    }

    case PropertyID::CornerTopShape: {
        static FlyString name = "cornerTopShape"_fly_string;
        return name;
    }

    case PropertyID::CounterIncrement: {
        static FlyString name = "counterIncrement"_fly_string;
        return name;
    }

    case PropertyID::CounterReset: {
        static FlyString name = "counterReset"_fly_string;
        return name;
    }

    case PropertyID::CounterSet: {
        static FlyString name = "counterSet"_fly_string;
        return name;
    }

    case PropertyID::Cursor: {
        static FlyString name = "cursor"_fly_string;
        return name;
    }

    case PropertyID::Cx: {
        static FlyString name = "cx"_fly_string;
        return name;
    }

    case PropertyID::Cy: {
        static FlyString name = "cy"_fly_string;
        return name;
    }

    case PropertyID::Direction: {
        static FlyString name = "direction"_fly_string;
        return name;
    }

    case PropertyID::Display: {
        static FlyString name = "display"_fly_string;
        return name;
    }

    case PropertyID::DominantBaseline: {
        static FlyString name = "dominantBaseline"_fly_string;
        return name;
    }

    case PropertyID::EmptyCells: {
        static FlyString name = "emptyCells"_fly_string;
        return name;
    }

    case PropertyID::Fill: {
        static FlyString name = "fill"_fly_string;
        return name;
    }

    case PropertyID::FillOpacity: {
        static FlyString name = "fillOpacity"_fly_string;
        return name;
    }

    case PropertyID::FillRule: {
        static FlyString name = "fillRule"_fly_string;
        return name;
    }

    case PropertyID::Filter: {
        static FlyString name = "filter"_fly_string;
        return name;
    }

    case PropertyID::Flex: {
        static FlyString name = "flex"_fly_string;
        return name;
    }

    case PropertyID::FlexBasis: {
        static FlyString name = "flexBasis"_fly_string;
        return name;
    }

    case PropertyID::FlexDirection: {
        static FlyString name = "flexDirection"_fly_string;
        return name;
    }

    case PropertyID::FlexFlow: {
        static FlyString name = "flexFlow"_fly_string;
        return name;
    }

    case PropertyID::FlexGrow: {
        static FlyString name = "flexGrow"_fly_string;
        return name;
    }

    case PropertyID::FlexShrink: {
        static FlyString name = "flexShrink"_fly_string;
        return name;
    }

    case PropertyID::FlexWrap: {
        static FlyString name = "flexWrap"_fly_string;
        return name;
    }

    case PropertyID::Float: {
        static FlyString name = "float"_fly_string;
        return name;
    }

    case PropertyID::FloodColor: {
        static FlyString name = "floodColor"_fly_string;
        return name;
    }

    case PropertyID::FloodOpacity: {
        static FlyString name = "floodOpacity"_fly_string;
        return name;
    }

    case PropertyID::Font: {
        static FlyString name = "font"_fly_string;
        return name;
    }

    case PropertyID::FontFamily: {
        static FlyString name = "fontFamily"_fly_string;
        return name;
    }

    case PropertyID::FontFeatureSettings: {
        static FlyString name = "fontFeatureSettings"_fly_string;
        return name;
    }

    case PropertyID::FontKerning: {
        static FlyString name = "fontKerning"_fly_string;
        return name;
    }

    case PropertyID::FontLanguageOverride: {
        static FlyString name = "fontLanguageOverride"_fly_string;
        return name;
    }

    case PropertyID::FontOpticalSizing: {
        static FlyString name = "fontOpticalSizing"_fly_string;
        return name;
    }

    case PropertyID::FontSize: {
        static FlyString name = "fontSize"_fly_string;
        return name;
    }

    case PropertyID::FontStyle: {
        static FlyString name = "fontStyle"_fly_string;
        return name;
    }

    case PropertyID::FontVariant: {
        static FlyString name = "fontVariant"_fly_string;
        return name;
    }

    case PropertyID::FontVariantAlternates: {
        static FlyString name = "fontVariantAlternates"_fly_string;
        return name;
    }

    case PropertyID::FontVariantCaps: {
        static FlyString name = "fontVariantCaps"_fly_string;
        return name;
    }

    case PropertyID::FontVariantEastAsian: {
        static FlyString name = "fontVariantEastAsian"_fly_string;
        return name;
    }

    case PropertyID::FontVariantEmoji: {
        static FlyString name = "fontVariantEmoji"_fly_string;
        return name;
    }

    case PropertyID::FontVariantLigatures: {
        static FlyString name = "fontVariantLigatures"_fly_string;
        return name;
    }

    case PropertyID::FontVariantNumeric: {
        static FlyString name = "fontVariantNumeric"_fly_string;
        return name;
    }

    case PropertyID::FontVariantPosition: {
        static FlyString name = "fontVariantPosition"_fly_string;
        return name;
    }

    case PropertyID::FontVariationSettings: {
        static FlyString name = "fontVariationSettings"_fly_string;
        return name;
    }

    case PropertyID::FontWeight: {
        static FlyString name = "fontWeight"_fly_string;
        return name;
    }

    case PropertyID::FontWidth: {
        static FlyString name = "fontWidth"_fly_string;
        return name;
    }

    case PropertyID::Gap: {
        static FlyString name = "gap"_fly_string;
        return name;
    }

    case PropertyID::Grid: {
        static FlyString name = "grid"_fly_string;
        return name;
    }

    case PropertyID::GridArea: {
        static FlyString name = "gridArea"_fly_string;
        return name;
    }

    case PropertyID::GridAutoColumns: {
        static FlyString name = "gridAutoColumns"_fly_string;
        return name;
    }

    case PropertyID::GridAutoFlow: {
        static FlyString name = "gridAutoFlow"_fly_string;
        return name;
    }

    case PropertyID::GridAutoRows: {
        static FlyString name = "gridAutoRows"_fly_string;
        return name;
    }

    case PropertyID::GridColumn: {
        static FlyString name = "gridColumn"_fly_string;
        return name;
    }

    case PropertyID::GridColumnEnd: {
        static FlyString name = "gridColumnEnd"_fly_string;
        return name;
    }

    case PropertyID::GridColumnStart: {
        static FlyString name = "gridColumnStart"_fly_string;
        return name;
    }

    case PropertyID::GridRow: {
        static FlyString name = "gridRow"_fly_string;
        return name;
    }

    case PropertyID::GridRowEnd: {
        static FlyString name = "gridRowEnd"_fly_string;
        return name;
    }

    case PropertyID::GridRowStart: {
        static FlyString name = "gridRowStart"_fly_string;
        return name;
    }

    case PropertyID::GridTemplate: {
        static FlyString name = "gridTemplate"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateAreas: {
        static FlyString name = "gridTemplateAreas"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateColumns: {
        static FlyString name = "gridTemplateColumns"_fly_string;
        return name;
    }

    case PropertyID::GridTemplateRows: {
        static FlyString name = "gridTemplateRows"_fly_string;
        return name;
    }

    case PropertyID::Height: {
        static FlyString name = "height"_fly_string;
        return name;
    }

    case PropertyID::ImageRendering: {
        static FlyString name = "imageRendering"_fly_string;
        return name;
    }

    case PropertyID::InlineSize: {
        static FlyString name = "inlineSize"_fly_string;
        return name;
    }

    case PropertyID::Inset: {
        static FlyString name = "inset"_fly_string;
        return name;
    }

    case PropertyID::InsetBlock: {
        static FlyString name = "insetBlock"_fly_string;
        return name;
    }

    case PropertyID::InsetBlockEnd: {
        static FlyString name = "insetBlockEnd"_fly_string;
        return name;
    }

    case PropertyID::InsetBlockStart: {
        static FlyString name = "insetBlockStart"_fly_string;
        return name;
    }

    case PropertyID::InsetInline: {
        static FlyString name = "insetInline"_fly_string;
        return name;
    }

    case PropertyID::InsetInlineEnd: {
        static FlyString name = "insetInlineEnd"_fly_string;
        return name;
    }

    case PropertyID::InsetInlineStart: {
        static FlyString name = "insetInlineStart"_fly_string;
        return name;
    }

    case PropertyID::Isolation: {
        static FlyString name = "isolation"_fly_string;
        return name;
    }

    case PropertyID::JustifyContent: {
        static FlyString name = "justifyContent"_fly_string;
        return name;
    }

    case PropertyID::JustifyItems: {
        static FlyString name = "justifyItems"_fly_string;
        return name;
    }

    case PropertyID::JustifySelf: {
        static FlyString name = "justifySelf"_fly_string;
        return name;
    }

    case PropertyID::Left: {
        static FlyString name = "left"_fly_string;
        return name;
    }

    case PropertyID::LetterSpacing: {
        static FlyString name = "letterSpacing"_fly_string;
        return name;
    }

    case PropertyID::LineHeight: {
        static FlyString name = "lineHeight"_fly_string;
        return name;
    }

    case PropertyID::ListStyle: {
        static FlyString name = "listStyle"_fly_string;
        return name;
    }

    case PropertyID::ListStyleImage: {
        static FlyString name = "listStyleImage"_fly_string;
        return name;
    }

    case PropertyID::ListStylePosition: {
        static FlyString name = "listStylePosition"_fly_string;
        return name;
    }

    case PropertyID::ListStyleType: {
        static FlyString name = "listStyleType"_fly_string;
        return name;
    }

    case PropertyID::Margin: {
        static FlyString name = "margin"_fly_string;
        return name;
    }

    case PropertyID::MarginBlock: {
        static FlyString name = "marginBlock"_fly_string;
        return name;
    }

    case PropertyID::MarginBlockEnd: {
        static FlyString name = "marginBlockEnd"_fly_string;
        return name;
    }

    case PropertyID::MarginBlockStart: {
        static FlyString name = "marginBlockStart"_fly_string;
        return name;
    }

    case PropertyID::MarginBottom: {
        static FlyString name = "marginBottom"_fly_string;
        return name;
    }

    case PropertyID::MarginInline: {
        static FlyString name = "marginInline"_fly_string;
        return name;
    }

    case PropertyID::MarginInlineEnd: {
        static FlyString name = "marginInlineEnd"_fly_string;
        return name;
    }

    case PropertyID::MarginInlineStart: {
        static FlyString name = "marginInlineStart"_fly_string;
        return name;
    }

    case PropertyID::MarginLeft: {
        static FlyString name = "marginLeft"_fly_string;
        return name;
    }

    case PropertyID::MarginRight: {
        static FlyString name = "marginRight"_fly_string;
        return name;
    }

    case PropertyID::MarginTop: {
        static FlyString name = "marginTop"_fly_string;
        return name;
    }

    case PropertyID::Mask: {
        static FlyString name = "mask"_fly_string;
        return name;
    }

    case PropertyID::MaskClip: {
        static FlyString name = "maskClip"_fly_string;
        return name;
    }

    case PropertyID::MaskComposite: {
        static FlyString name = "maskComposite"_fly_string;
        return name;
    }

    case PropertyID::MaskImage: {
        static FlyString name = "maskImage"_fly_string;
        return name;
    }

    case PropertyID::MaskMode: {
        static FlyString name = "maskMode"_fly_string;
        return name;
    }

    case PropertyID::MaskOrigin: {
        static FlyString name = "maskOrigin"_fly_string;
        return name;
    }

    case PropertyID::MaskPosition: {
        static FlyString name = "maskPosition"_fly_string;
        return name;
    }

    case PropertyID::MaskRepeat: {
        static FlyString name = "maskRepeat"_fly_string;
        return name;
    }

    case PropertyID::MaskSize: {
        static FlyString name = "maskSize"_fly_string;
        return name;
    }

    case PropertyID::MaskType: {
        static FlyString name = "maskType"_fly_string;
        return name;
    }

    case PropertyID::MathDepth: {
        static FlyString name = "mathDepth"_fly_string;
        return name;
    }

    case PropertyID::MathShift: {
        static FlyString name = "mathShift"_fly_string;
        return name;
    }

    case PropertyID::MathStyle: {
        static FlyString name = "mathStyle"_fly_string;
        return name;
    }

    case PropertyID::MaxBlockSize: {
        static FlyString name = "maxBlockSize"_fly_string;
        return name;
    }

    case PropertyID::MaxHeight: {
        static FlyString name = "maxHeight"_fly_string;
        return name;
    }

    case PropertyID::MaxInlineSize: {
        static FlyString name = "maxInlineSize"_fly_string;
        return name;
    }

    case PropertyID::MaxWidth: {
        static FlyString name = "maxWidth"_fly_string;
        return name;
    }

    case PropertyID::MinBlockSize: {
        static FlyString name = "minBlockSize"_fly_string;
        return name;
    }

    case PropertyID::MinHeight: {
        static FlyString name = "minHeight"_fly_string;
        return name;
    }

    case PropertyID::MinInlineSize: {
        static FlyString name = "minInlineSize"_fly_string;
        return name;
    }

    case PropertyID::MinWidth: {
        static FlyString name = "minWidth"_fly_string;
        return name;
    }

    case PropertyID::MixBlendMode: {
        static FlyString name = "mixBlendMode"_fly_string;
        return name;
    }

    case PropertyID::ObjectFit: {
        static FlyString name = "objectFit"_fly_string;
        return name;
    }

    case PropertyID::ObjectPosition: {
        static FlyString name = "objectPosition"_fly_string;
        return name;
    }

    case PropertyID::Opacity: {
        static FlyString name = "opacity"_fly_string;
        return name;
    }

    case PropertyID::Order: {
        static FlyString name = "order"_fly_string;
        return name;
    }

    case PropertyID::Orphans: {
        static FlyString name = "orphans"_fly_string;
        return name;
    }

    case PropertyID::Outline: {
        static FlyString name = "outline"_fly_string;
        return name;
    }

    case PropertyID::OutlineColor: {
        static FlyString name = "outlineColor"_fly_string;
        return name;
    }

    case PropertyID::OutlineOffset: {
        static FlyString name = "outlineOffset"_fly_string;
        return name;
    }

    case PropertyID::OutlineStyle: {
        static FlyString name = "outlineStyle"_fly_string;
        return name;
    }

    case PropertyID::OutlineWidth: {
        static FlyString name = "outlineWidth"_fly_string;
        return name;
    }

    case PropertyID::Overflow: {
        static FlyString name = "overflow"_fly_string;
        return name;
    }

    case PropertyID::OverflowBlock: {
        static FlyString name = "overflowBlock"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMargin: {
        static FlyString name = "overflowClipMargin"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlock: {
        static FlyString name = "overflowClipMarginBlock"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlockEnd: {
        static FlyString name = "overflowClipMarginBlockEnd"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBlockStart: {
        static FlyString name = "overflowClipMarginBlockStart"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginBottom: {
        static FlyString name = "overflowClipMarginBottom"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInline: {
        static FlyString name = "overflowClipMarginInline"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInlineEnd: {
        static FlyString name = "overflowClipMarginInlineEnd"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginInlineStart: {
        static FlyString name = "overflowClipMarginInlineStart"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginLeft: {
        static FlyString name = "overflowClipMarginLeft"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginRight: {
        static FlyString name = "overflowClipMarginRight"_fly_string;
        return name;
    }

    case PropertyID::OverflowClipMarginTop: {
        static FlyString name = "overflowClipMarginTop"_fly_string;
        return name;
    }

    case PropertyID::OverflowInline: {
        static FlyString name = "overflowInline"_fly_string;
        return name;
    }

    case PropertyID::OverflowWrap: {
        static FlyString name = "overflowWrap"_fly_string;
        return name;
    }

    case PropertyID::OverflowX: {
        static FlyString name = "overflowX"_fly_string;
        return name;
    }

    case PropertyID::OverflowY: {
        static FlyString name = "overflowY"_fly_string;
        return name;
    }

    case PropertyID::Padding: {
        static FlyString name = "padding"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlock: {
        static FlyString name = "paddingBlock"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlockEnd: {
        static FlyString name = "paddingBlockEnd"_fly_string;
        return name;
    }

    case PropertyID::PaddingBlockStart: {
        static FlyString name = "paddingBlockStart"_fly_string;
        return name;
    }

    case PropertyID::PaddingBottom: {
        static FlyString name = "paddingBottom"_fly_string;
        return name;
    }

    case PropertyID::PaddingInline: {
        static FlyString name = "paddingInline"_fly_string;
        return name;
    }

    case PropertyID::PaddingInlineEnd: {
        static FlyString name = "paddingInlineEnd"_fly_string;
        return name;
    }

    case PropertyID::PaddingInlineStart: {
        static FlyString name = "paddingInlineStart"_fly_string;
        return name;
    }

    case PropertyID::PaddingLeft: {
        static FlyString name = "paddingLeft"_fly_string;
        return name;
    }

    case PropertyID::PaddingRight: {
        static FlyString name = "paddingRight"_fly_string;
        return name;
    }

    case PropertyID::PaddingTop: {
        static FlyString name = "paddingTop"_fly_string;
        return name;
    }

    case PropertyID::PaintOrder: {
        static FlyString name = "paintOrder"_fly_string;
        return name;
    }

    case PropertyID::Perspective: {
        static FlyString name = "perspective"_fly_string;
        return name;
    }

    case PropertyID::PerspectiveOrigin: {
        static FlyString name = "perspectiveOrigin"_fly_string;
        return name;
    }

    case PropertyID::PlaceContent: {
        static FlyString name = "placeContent"_fly_string;
        return name;
    }

    case PropertyID::PlaceItems: {
        static FlyString name = "placeItems"_fly_string;
        return name;
    }

    case PropertyID::PlaceSelf: {
        static FlyString name = "placeSelf"_fly_string;
        return name;
    }

    case PropertyID::PointerEvents: {
        static FlyString name = "pointerEvents"_fly_string;
        return name;
    }

    case PropertyID::Position: {
        static FlyString name = "position"_fly_string;
        return name;
    }

    case PropertyID::PositionAnchor: {
        static FlyString name = "positionAnchor"_fly_string;
        return name;
    }

    case PropertyID::PositionArea: {
        static FlyString name = "positionArea"_fly_string;
        return name;
    }

    case PropertyID::PositionTryFallbacks: {
        static FlyString name = "positionTryFallbacks"_fly_string;
        return name;
    }

    case PropertyID::PositionTryOrder: {
        static FlyString name = "positionTryOrder"_fly_string;
        return name;
    }

    case PropertyID::PositionVisibility: {
        static FlyString name = "positionVisibility"_fly_string;
        return name;
    }

    case PropertyID::Quotes: {
        static FlyString name = "quotes"_fly_string;
        return name;
    }

    case PropertyID::R: {
        static FlyString name = "r"_fly_string;
        return name;
    }

    case PropertyID::Resize: {
        static FlyString name = "resize"_fly_string;
        return name;
    }

    case PropertyID::Right: {
        static FlyString name = "right"_fly_string;
        return name;
    }

    case PropertyID::Rotate: {
        static FlyString name = "rotate"_fly_string;
        return name;
    }

    case PropertyID::RowGap: {
        static FlyString name = "rowGap"_fly_string;
        return name;
    }

    case PropertyID::Rx: {
        static FlyString name = "rx"_fly_string;
        return name;
    }

    case PropertyID::Ry: {
        static FlyString name = "ry"_fly_string;
        return name;
    }

    case PropertyID::Scale: {
        static FlyString name = "scale"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimeline: {
        static FlyString name = "scrollTimeline"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimelineAxis: {
        static FlyString name = "scrollTimelineAxis"_fly_string;
        return name;
    }

    case PropertyID::ScrollTimelineName: {
        static FlyString name = "scrollTimelineName"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarColor: {
        static FlyString name = "scrollbarColor"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarGutter: {
        static FlyString name = "scrollbarGutter"_fly_string;
        return name;
    }

    case PropertyID::ScrollbarWidth: {
        static FlyString name = "scrollbarWidth"_fly_string;
        return name;
    }

    case PropertyID::ShapeImageThreshold: {
        static FlyString name = "shapeImageThreshold"_fly_string;
        return name;
    }

    case PropertyID::ShapeMargin: {
        static FlyString name = "shapeMargin"_fly_string;
        return name;
    }

    case PropertyID::ShapeOutside: {
        static FlyString name = "shapeOutside"_fly_string;
        return name;
    }

    case PropertyID::ShapeRendering: {
        static FlyString name = "shapeRendering"_fly_string;
        return name;
    }

    case PropertyID::StopColor: {
        static FlyString name = "stopColor"_fly_string;
        return name;
    }

    case PropertyID::StopOpacity: {
        static FlyString name = "stopOpacity"_fly_string;
        return name;
    }

    case PropertyID::Stroke: {
        static FlyString name = "stroke"_fly_string;
        return name;
    }

    case PropertyID::StrokeDasharray: {
        static FlyString name = "strokeDasharray"_fly_string;
        return name;
    }

    case PropertyID::StrokeDashoffset: {
        static FlyString name = "strokeDashoffset"_fly_string;
        return name;
    }

    case PropertyID::StrokeLinecap: {
        static FlyString name = "strokeLinecap"_fly_string;
        return name;
    }

    case PropertyID::StrokeLinejoin: {
        static FlyString name = "strokeLinejoin"_fly_string;
        return name;
    }

    case PropertyID::StrokeMiterlimit: {
        static FlyString name = "strokeMiterlimit"_fly_string;
        return name;
    }

    case PropertyID::StrokeOpacity: {
        static FlyString name = "strokeOpacity"_fly_string;
        return name;
    }

    case PropertyID::StrokeWidth: {
        static FlyString name = "strokeWidth"_fly_string;
        return name;
    }

    case PropertyID::TabSize: {
        static FlyString name = "tabSize"_fly_string;
        return name;
    }

    case PropertyID::TableLayout: {
        static FlyString name = "tableLayout"_fly_string;
        return name;
    }

    case PropertyID::TextAlign: {
        static FlyString name = "textAlign"_fly_string;
        return name;
    }

    case PropertyID::TextAnchor: {
        static FlyString name = "textAnchor"_fly_string;
        return name;
    }

    case PropertyID::TextDecoration: {
        static FlyString name = "textDecoration"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationColor: {
        static FlyString name = "textDecorationColor"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationLine: {
        static FlyString name = "textDecorationLine"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationStyle: {
        static FlyString name = "textDecorationStyle"_fly_string;
        return name;
    }

    case PropertyID::TextDecorationThickness: {
        static FlyString name = "textDecorationThickness"_fly_string;
        return name;
    }

    case PropertyID::TextIndent: {
        static FlyString name = "textIndent"_fly_string;
        return name;
    }

    case PropertyID::TextJustify: {
        static FlyString name = "textJustify"_fly_string;
        return name;
    }

    case PropertyID::TextOverflow: {
        static FlyString name = "textOverflow"_fly_string;
        return name;
    }

    case PropertyID::TextRendering: {
        static FlyString name = "textRendering"_fly_string;
        return name;
    }

    case PropertyID::TextShadow: {
        static FlyString name = "textShadow"_fly_string;
        return name;
    }

    case PropertyID::TextTransform: {
        static FlyString name = "textTransform"_fly_string;
        return name;
    }

    case PropertyID::TextUnderlineOffset: {
        static FlyString name = "textUnderlineOffset"_fly_string;
        return name;
    }

    case PropertyID::TextUnderlinePosition: {
        static FlyString name = "textUnderlinePosition"_fly_string;
        return name;
    }

    case PropertyID::TextWrap: {
        static FlyString name = "textWrap"_fly_string;
        return name;
    }

    case PropertyID::TextWrapMode: {
        static FlyString name = "textWrapMode"_fly_string;
        return name;
    }

    case PropertyID::TextWrapStyle: {
        static FlyString name = "textWrapStyle"_fly_string;
        return name;
    }

    case PropertyID::TimelineScope: {
        static FlyString name = "timelineScope"_fly_string;
        return name;
    }

    case PropertyID::Top: {
        static FlyString name = "top"_fly_string;
        return name;
    }

    case PropertyID::TouchAction: {
        static FlyString name = "touchAction"_fly_string;
        return name;
    }

    case PropertyID::Transform: {
        static FlyString name = "transform"_fly_string;
        return name;
    }

    case PropertyID::TransformBox: {
        static FlyString name = "transformBox"_fly_string;
        return name;
    }

    case PropertyID::TransformOrigin: {
        static FlyString name = "transformOrigin"_fly_string;
        return name;
    }

    case PropertyID::TransformStyle: {
        static FlyString name = "transformStyle"_fly_string;
        return name;
    }

    case PropertyID::Transition: {
        static FlyString name = "transition"_fly_string;
        return name;
    }

    case PropertyID::TransitionBehavior: {
        static FlyString name = "transitionBehavior"_fly_string;
        return name;
    }

    case PropertyID::TransitionDelay: {
        static FlyString name = "transitionDelay"_fly_string;
        return name;
    }

    case PropertyID::TransitionDuration: {
        static FlyString name = "transitionDuration"_fly_string;
        return name;
    }

    case PropertyID::TransitionProperty: {
        static FlyString name = "transitionProperty"_fly_string;
        return name;
    }

    case PropertyID::TransitionTimingFunction: {
        static FlyString name = "transitionTimingFunction"_fly_string;
        return name;
    }

    case PropertyID::Translate: {
        static FlyString name = "translate"_fly_string;
        return name;
    }

    case PropertyID::UnicodeBidi: {
        static FlyString name = "unicodeBidi"_fly_string;
        return name;
    }

    case PropertyID::UserSelect: {
        static FlyString name = "userSelect"_fly_string;
        return name;
    }

    case PropertyID::VerticalAlign: {
        static FlyString name = "verticalAlign"_fly_string;
        return name;
    }

    case PropertyID::ViewTimeline: {
        static FlyString name = "viewTimeline"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineAxis: {
        static FlyString name = "viewTimelineAxis"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineInset: {
        static FlyString name = "viewTimelineInset"_fly_string;
        return name;
    }

    case PropertyID::ViewTimelineName: {
        static FlyString name = "viewTimelineName"_fly_string;
        return name;
    }

    case PropertyID::ViewTransitionName: {
        static FlyString name = "viewTransitionName"_fly_string;
        return name;
    }

    case PropertyID::Visibility: {
        static FlyString name = "visibility"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpace: {
        static FlyString name = "whiteSpace"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpaceCollapse: {
        static FlyString name = "whiteSpaceCollapse"_fly_string;
        return name;
    }

    case PropertyID::WhiteSpaceTrim: {
        static FlyString name = "whiteSpaceTrim"_fly_string;
        return name;
    }

    case PropertyID::Widows: {
        static FlyString name = "widows"_fly_string;
        return name;
    }

    case PropertyID::Width: {
        static FlyString name = "width"_fly_string;
        return name;
    }

    case PropertyID::WillChange: {
        static FlyString name = "willChange"_fly_string;
        return name;
    }

    case PropertyID::WordBreak: {
        static FlyString name = "wordBreak"_fly_string;
        return name;
    }

    case PropertyID::WordSpacing: {
        static FlyString name = "wordSpacing"_fly_string;
        return name;
    }

    case PropertyID::WritingMode: {
        static FlyString name = "writingMode"_fly_string;
        return name;
    }

    case PropertyID::X: {
        static FlyString name = "x"_fly_string;
        return name;
    }

    case PropertyID::Y: {
        static FlyString name = "y"_fly_string;
        return name;
    }

    case PropertyID::ZIndex: {
        static FlyString name = "zIndex"_fly_string;
        return name;
    }

    default: {
        static FlyString invalid_property_id_string = "(invalid CSS::PropertyID)"_fly_string;
        return invalid_property_id_string;
    }
    }
}

AnimationType animation_type_from_longhand_property(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::WebkitTextFillColor:
        return AnimationType::ByComputedValue;

    case PropertyID::AccentColor:
        return AnimationType::ByComputedValue;

    case PropertyID::AlignContent:
        return AnimationType::Discrete;

    case PropertyID::AlignItems:
        return AnimationType::Discrete;

    case PropertyID::AlignSelf:
        return AnimationType::Discrete;

    case PropertyID::All:
        VERIFY_NOT_REACHED();

    case PropertyID::AnchorName:
        return AnimationType::Discrete;

    case PropertyID::AnchorScope:
        return AnimationType::Discrete;

    case PropertyID::Animation:
        VERIFY_NOT_REACHED();

    case PropertyID::AnimationComposition:
        return AnimationType::None;

    case PropertyID::AnimationDelay:
        return AnimationType::None;

    case PropertyID::AnimationDirection:
        return AnimationType::None;

    case PropertyID::AnimationDuration:
        return AnimationType::None;

    case PropertyID::AnimationFillMode:
        return AnimationType::None;

    case PropertyID::AnimationIterationCount:
        return AnimationType::None;

    case PropertyID::AnimationName:
        return AnimationType::None;

    case PropertyID::AnimationPlayState:
        return AnimationType::None;

    case PropertyID::AnimationTimeline:
        return AnimationType::None;

    case PropertyID::AnimationTimingFunction:
        return AnimationType::None;

    case PropertyID::Appearance:
        return AnimationType::Discrete;

    case PropertyID::AspectRatio:
        return AnimationType::ByComputedValue;

    case PropertyID::BackdropFilter:
        return AnimationType::Custom;

    case PropertyID::Background:
        VERIFY_NOT_REACHED();

    case PropertyID::BackgroundAttachment:
        return AnimationType::Discrete;

    case PropertyID::BackgroundBlendMode:
        return AnimationType::Discrete;

    case PropertyID::BackgroundClip:
        return AnimationType::RepeatableList;

    case PropertyID::BackgroundColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BackgroundImage:
        return AnimationType::Discrete;

    case PropertyID::BackgroundOrigin:
        return AnimationType::RepeatableList;

    case PropertyID::BackgroundPosition:
        VERIFY_NOT_REACHED();

    case PropertyID::BackgroundPositionX:
        return AnimationType::RepeatableList;

    case PropertyID::BackgroundPositionY:
        return AnimationType::RepeatableList;

    case PropertyID::BackgroundRepeat:
        return AnimationType::Discrete;

    case PropertyID::BackgroundSize:
        return AnimationType::RepeatableList;

    case PropertyID::BlockSize:
        return AnimationType::ByComputedValue;

    case PropertyID::Border:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlock:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlockColor:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlockEnd:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlockEndColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBlockEndStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderBlockEndWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBlockStart:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlockStartColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBlockStartStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderBlockStartWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBlockStyle:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBlockWidth:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBottom:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderBottomColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBottomLeftRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBottomRightRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderBottomStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderBottomWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderCollapse:
        return AnimationType::Discrete;

    case PropertyID::BorderColor:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderEndEndRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderEndStartRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderImage:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderImageOutset:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderImageRepeat:
        return AnimationType::Discrete;

    case PropertyID::BorderImageSlice:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderImageSource:
        return AnimationType::Discrete;

    case PropertyID::BorderImageWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderInline:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderInlineColor:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderInlineEnd:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderInlineEndColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderInlineEndStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderInlineEndWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderInlineStart:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderInlineStartColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderInlineStartStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderInlineStartWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderInlineStyle:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderInlineWidth:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderLeft:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderLeftColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderLeftStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderLeftWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderRadius:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderRight:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderRightColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderRightStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderRightWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderSpacing:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderStartEndRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderStartStartRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderStyle:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderTop:
        VERIFY_NOT_REACHED();

    case PropertyID::BorderTopColor:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderTopLeftRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderTopRightRadius:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderTopStyle:
        return AnimationType::Discrete;

    case PropertyID::BorderTopWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::BorderWidth:
        VERIFY_NOT_REACHED();

    case PropertyID::Bottom:
        return AnimationType::ByComputedValue;

    case PropertyID::BoxShadow:
        return AnimationType::Custom;

    case PropertyID::BoxSizing:
        return AnimationType::Discrete;

    case PropertyID::CaptionSide:
        return AnimationType::Discrete;

    case PropertyID::CaretColor:
        return AnimationType::ByComputedValue;

    case PropertyID::Clear:
        return AnimationType::Discrete;

    case PropertyID::Clip:
        return AnimationType::ByComputedValue;

    case PropertyID::ClipPath:
        return AnimationType::ByComputedValue;

    case PropertyID::ClipRule:
        return AnimationType::Discrete;

    case PropertyID::Color:
        return AnimationType::ByComputedValue;

    case PropertyID::ColorInterpolation:
        return AnimationType::Discrete;

    case PropertyID::ColorScheme:
        return AnimationType::Discrete;

    case PropertyID::ColumnCount:
        return AnimationType::ByComputedValue;

    case PropertyID::ColumnGap:
        return AnimationType::ByComputedValue;

    case PropertyID::ColumnHeight:
        return AnimationType::ByComputedValue;

    case PropertyID::ColumnSpan:
        return AnimationType::Discrete;

    case PropertyID::ColumnWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::Columns:
        VERIFY_NOT_REACHED();

    case PropertyID::Contain:
        return AnimationType::None;

    case PropertyID::ContainerType:
        return AnimationType::None;

    case PropertyID::Content:
        return AnimationType::Discrete;

    case PropertyID::ContentVisibility:
        return AnimationType::Custom;

    case PropertyID::CornerBlockEndShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerBlockStartShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerBottomLeftShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerBottomRightShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerBottomShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerEndEndShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerEndStartShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerInlineEndShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerInlineStartShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerLeftShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerRightShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CornerStartEndShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerStartStartShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerTopLeftShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerTopRightShape:
        return AnimationType::ByComputedValue;

    case PropertyID::CornerTopShape:
        VERIFY_NOT_REACHED();

    case PropertyID::CounterIncrement:
        return AnimationType::ByComputedValue;

    case PropertyID::CounterReset:
        return AnimationType::ByComputedValue;

    case PropertyID::CounterSet:
        return AnimationType::ByComputedValue;

    case PropertyID::Cursor:
        return AnimationType::Discrete;

    case PropertyID::Cx:
        return AnimationType::ByComputedValue;

    case PropertyID::Cy:
        return AnimationType::ByComputedValue;

    case PropertyID::Direction:
        return AnimationType::None;

    case PropertyID::Display:
        return AnimationType::Custom;

    case PropertyID::DominantBaseline:
        return AnimationType::Discrete;

    case PropertyID::EmptyCells:
        return AnimationType::Discrete;

    case PropertyID::Fill:
        return AnimationType::ByComputedValue;

    case PropertyID::FillOpacity:
        return AnimationType::ByComputedValue;

    case PropertyID::FillRule:
        return AnimationType::Discrete;

    case PropertyID::Filter:
        return AnimationType::Custom;

    case PropertyID::Flex:
        VERIFY_NOT_REACHED();

    case PropertyID::FlexBasis:
        return AnimationType::ByComputedValue;

    case PropertyID::FlexDirection:
        return AnimationType::Discrete;

    case PropertyID::FlexFlow:
        VERIFY_NOT_REACHED();

    case PropertyID::FlexGrow:
        return AnimationType::ByComputedValue;

    case PropertyID::FlexShrink:
        return AnimationType::ByComputedValue;

    case PropertyID::FlexWrap:
        return AnimationType::Discrete;

    case PropertyID::Float:
        return AnimationType::Discrete;

    case PropertyID::FloodColor:
        return AnimationType::ByComputedValue;

    case PropertyID::FloodOpacity:
        return AnimationType::ByComputedValue;

    case PropertyID::Font:
        VERIFY_NOT_REACHED();

    case PropertyID::FontFamily:
        return AnimationType::Discrete;

    case PropertyID::FontFeatureSettings:
        return AnimationType::Discrete;

    case PropertyID::FontKerning:
        return AnimationType::Discrete;

    case PropertyID::FontLanguageOverride:
        return AnimationType::Discrete;

    case PropertyID::FontOpticalSizing:
        return AnimationType::Discrete;

    case PropertyID::FontSize:
        return AnimationType::ByComputedValue;

    case PropertyID::FontStyle:
        return AnimationType::Custom;

    case PropertyID::FontVariant:
        VERIFY_NOT_REACHED();

    case PropertyID::FontVariantAlternates:
        return AnimationType::Discrete;

    case PropertyID::FontVariantCaps:
        return AnimationType::Discrete;

    case PropertyID::FontVariantEastAsian:
        return AnimationType::Discrete;

    case PropertyID::FontVariantEmoji:
        return AnimationType::Discrete;

    case PropertyID::FontVariantLigatures:
        return AnimationType::Discrete;

    case PropertyID::FontVariantNumeric:
        return AnimationType::Discrete;

    case PropertyID::FontVariantPosition:
        return AnimationType::Discrete;

    case PropertyID::FontVariationSettings:
        return AnimationType::Custom;

    case PropertyID::FontWeight:
        return AnimationType::ByComputedValue;

    case PropertyID::FontWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::Gap:
        VERIFY_NOT_REACHED();

    case PropertyID::Grid:
        VERIFY_NOT_REACHED();

    case PropertyID::GridArea:
        VERIFY_NOT_REACHED();

    case PropertyID::GridAutoColumns:
        return AnimationType::ByComputedValue;

    case PropertyID::GridAutoFlow:
        return AnimationType::Discrete;

    case PropertyID::GridAutoRows:
        return AnimationType::ByComputedValue;

    case PropertyID::GridColumn:
        VERIFY_NOT_REACHED();

    case PropertyID::GridColumnEnd:
        return AnimationType::Discrete;

    case PropertyID::GridColumnStart:
        return AnimationType::Discrete;

    case PropertyID::GridRow:
        VERIFY_NOT_REACHED();

    case PropertyID::GridRowEnd:
        return AnimationType::Discrete;

    case PropertyID::GridRowStart:
        return AnimationType::Discrete;

    case PropertyID::GridTemplate:
        VERIFY_NOT_REACHED();

    case PropertyID::GridTemplateAreas:
        return AnimationType::Discrete;

    case PropertyID::GridTemplateColumns:
        return AnimationType::Custom;

    case PropertyID::GridTemplateRows:
        return AnimationType::Custom;

    case PropertyID::Height:
        return AnimationType::ByComputedValue;

    case PropertyID::ImageRendering:
        return AnimationType::Discrete;

    case PropertyID::InlineSize:
        return AnimationType::ByComputedValue;

    case PropertyID::Inset:
        VERIFY_NOT_REACHED();

    case PropertyID::InsetBlock:
        VERIFY_NOT_REACHED();

    case PropertyID::InsetBlockEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::InsetBlockStart:
        return AnimationType::ByComputedValue;

    case PropertyID::InsetInline:
        VERIFY_NOT_REACHED();

    case PropertyID::InsetInlineEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::InsetInlineStart:
        return AnimationType::ByComputedValue;

    case PropertyID::Isolation:
        return AnimationType::Discrete;

    case PropertyID::JustifyContent:
        return AnimationType::Discrete;

    case PropertyID::JustifyItems:
        return AnimationType::Discrete;

    case PropertyID::JustifySelf:
        return AnimationType::Discrete;

    case PropertyID::Left:
        return AnimationType::ByComputedValue;

    case PropertyID::LetterSpacing:
        return AnimationType::ByComputedValue;

    case PropertyID::LineHeight:
        return AnimationType::ByComputedValue;

    case PropertyID::ListStyle:
        VERIFY_NOT_REACHED();

    case PropertyID::ListStyleImage:
        return AnimationType::Discrete;

    case PropertyID::ListStylePosition:
        return AnimationType::Discrete;

    case PropertyID::ListStyleType:
        return AnimationType::Discrete;

    case PropertyID::Margin:
        VERIFY_NOT_REACHED();

    case PropertyID::MarginBlock:
        VERIFY_NOT_REACHED();

    case PropertyID::MarginBlockEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginBlockStart:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginBottom:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginInline:
        VERIFY_NOT_REACHED();

    case PropertyID::MarginInlineEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginInlineStart:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginLeft:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginRight:
        return AnimationType::ByComputedValue;

    case PropertyID::MarginTop:
        return AnimationType::ByComputedValue;

    case PropertyID::Mask:
        VERIFY_NOT_REACHED();

    case PropertyID::MaskClip:
        return AnimationType::Discrete;

    case PropertyID::MaskComposite:
        return AnimationType::Discrete;

    case PropertyID::MaskImage:
        return AnimationType::Discrete;

    case PropertyID::MaskMode:
        return AnimationType::Discrete;

    case PropertyID::MaskOrigin:
        return AnimationType::Discrete;

    case PropertyID::MaskPosition:
        return AnimationType::RepeatableList;

    case PropertyID::MaskRepeat:
        return AnimationType::Discrete;

    case PropertyID::MaskSize:
        return AnimationType::RepeatableList;

    case PropertyID::MaskType:
        return AnimationType::Discrete;

    case PropertyID::MathDepth:
        return AnimationType::None;

    case PropertyID::MathShift:
        return AnimationType::None;

    case PropertyID::MathStyle:
        return AnimationType::None;

    case PropertyID::MaxBlockSize:
        return AnimationType::ByComputedValue;

    case PropertyID::MaxHeight:
        return AnimationType::ByComputedValue;

    case PropertyID::MaxInlineSize:
        return AnimationType::ByComputedValue;

    case PropertyID::MaxWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::MinBlockSize:
        return AnimationType::ByComputedValue;

    case PropertyID::MinHeight:
        return AnimationType::ByComputedValue;

    case PropertyID::MinInlineSize:
        return AnimationType::ByComputedValue;

    case PropertyID::MinWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::MixBlendMode:
        return AnimationType::Discrete;

    case PropertyID::ObjectFit:
        return AnimationType::Discrete;

    case PropertyID::ObjectPosition:
        return AnimationType::RepeatableList;

    case PropertyID::Opacity:
        return AnimationType::ByComputedValue;

    case PropertyID::Order:
        return AnimationType::ByComputedValue;

    case PropertyID::Orphans:
        return AnimationType::ByComputedValue;

    case PropertyID::Outline:
        VERIFY_NOT_REACHED();

    case PropertyID::OutlineColor:
        return AnimationType::ByComputedValue;

    case PropertyID::OutlineOffset:
        return AnimationType::ByComputedValue;

    case PropertyID::OutlineStyle:
        return AnimationType::ByComputedValue;

    case PropertyID::OutlineWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::Overflow:
        VERIFY_NOT_REACHED();

    case PropertyID::OverflowBlock:
        return AnimationType::Discrete;

    case PropertyID::OverflowClipMargin:
        VERIFY_NOT_REACHED();

    case PropertyID::OverflowClipMarginBlock:
        VERIFY_NOT_REACHED();

    case PropertyID::OverflowClipMarginBlockEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginBlockStart:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginBottom:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginInline:
        VERIFY_NOT_REACHED();

    case PropertyID::OverflowClipMarginInlineEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginInlineStart:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginLeft:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginRight:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowClipMarginTop:
        return AnimationType::ByComputedValue;

    case PropertyID::OverflowInline:
        return AnimationType::Discrete;

    case PropertyID::OverflowWrap:
        return AnimationType::Discrete;

    case PropertyID::OverflowX:
        return AnimationType::Discrete;

    case PropertyID::OverflowY:
        return AnimationType::Discrete;

    case PropertyID::Padding:
        VERIFY_NOT_REACHED();

    case PropertyID::PaddingBlock:
        VERIFY_NOT_REACHED();

    case PropertyID::PaddingBlockEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingBlockStart:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingBottom:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingInline:
        VERIFY_NOT_REACHED();

    case PropertyID::PaddingInlineEnd:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingInlineStart:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingLeft:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingRight:
        return AnimationType::ByComputedValue;

    case PropertyID::PaddingTop:
        return AnimationType::ByComputedValue;

    case PropertyID::PaintOrder:
        return AnimationType::Discrete;

    case PropertyID::Perspective:
        return AnimationType::ByComputedValue;

    case PropertyID::PerspectiveOrigin:
        return AnimationType::ByComputedValue;

    case PropertyID::PlaceContent:
        VERIFY_NOT_REACHED();

    case PropertyID::PlaceItems:
        VERIFY_NOT_REACHED();

    case PropertyID::PlaceSelf:
        VERIFY_NOT_REACHED();

    case PropertyID::PointerEvents:
        return AnimationType::Discrete;

    case PropertyID::Position:
        return AnimationType::Discrete;

    case PropertyID::PositionAnchor:
        return AnimationType::Discrete;

    case PropertyID::PositionArea:
        return AnimationType::Discrete;

    case PropertyID::PositionTryFallbacks:
        return AnimationType::Discrete;

    case PropertyID::PositionTryOrder:
        return AnimationType::Discrete;

    case PropertyID::PositionVisibility:
        return AnimationType::Discrete;

    case PropertyID::Quotes:
        return AnimationType::Discrete;

    case PropertyID::R:
        return AnimationType::ByComputedValue;

    case PropertyID::Resize:
        return AnimationType::None;

    case PropertyID::Right:
        return AnimationType::ByComputedValue;

    case PropertyID::Rotate:
        return AnimationType::Custom;

    case PropertyID::RowGap:
        return AnimationType::ByComputedValue;

    case PropertyID::Rx:
        return AnimationType::ByComputedValue;

    case PropertyID::Ry:
        return AnimationType::ByComputedValue;

    case PropertyID::Scale:
        return AnimationType::Custom;

    case PropertyID::ScrollTimeline:
        VERIFY_NOT_REACHED();

    case PropertyID::ScrollTimelineAxis:
        return AnimationType::None;

    case PropertyID::ScrollTimelineName:
        return AnimationType::None;

    case PropertyID::ScrollbarColor:
        return AnimationType::ByComputedValue;

    case PropertyID::ScrollbarGutter:
        return AnimationType::Discrete;

    case PropertyID::ScrollbarWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::ShapeImageThreshold:
        return AnimationType::ByComputedValue;

    case PropertyID::ShapeMargin:
        return AnimationType::ByComputedValue;

    case PropertyID::ShapeOutside:
        return AnimationType::ByComputedValue;

    case PropertyID::ShapeRendering:
        return AnimationType::Discrete;

    case PropertyID::StopColor:
        return AnimationType::ByComputedValue;

    case PropertyID::StopOpacity:
        return AnimationType::ByComputedValue;

    case PropertyID::Stroke:
        return AnimationType::ByComputedValue;

    case PropertyID::StrokeDasharray:
        return AnimationType::Custom;

    case PropertyID::StrokeDashoffset:
        return AnimationType::ByComputedValue;

    case PropertyID::StrokeLinecap:
        return AnimationType::Discrete;

    case PropertyID::StrokeLinejoin:
        return AnimationType::Discrete;

    case PropertyID::StrokeMiterlimit:
        return AnimationType::ByComputedValue;

    case PropertyID::StrokeOpacity:
        return AnimationType::ByComputedValue;

    case PropertyID::StrokeWidth:
        return AnimationType::ByComputedValue;

    case PropertyID::TabSize:
        return AnimationType::ByComputedValue;

    case PropertyID::TableLayout:
        return AnimationType::Discrete;

    case PropertyID::TextAlign:
        return AnimationType::Discrete;

    case PropertyID::TextAnchor:
        return AnimationType::Discrete;

    case PropertyID::TextDecoration:
        VERIFY_NOT_REACHED();

    case PropertyID::TextDecorationColor:
        return AnimationType::ByComputedValue;

    case PropertyID::TextDecorationLine:
        return AnimationType::Discrete;

    case PropertyID::TextDecorationStyle:
        return AnimationType::Discrete;

    case PropertyID::TextDecorationThickness:
        return AnimationType::ByComputedValue;

    case PropertyID::TextIndent:
        return AnimationType::ByComputedValue;

    case PropertyID::TextJustify:
        return AnimationType::Discrete;

    case PropertyID::TextOverflow:
        return AnimationType::Discrete;

    case PropertyID::TextRendering:
        return AnimationType::Discrete;

    case PropertyID::TextShadow:
        return AnimationType::Custom;

    case PropertyID::TextTransform:
        return AnimationType::Discrete;

    case PropertyID::TextUnderlineOffset:
        return AnimationType::ByComputedValue;

    case PropertyID::TextUnderlinePosition:
        return AnimationType::Discrete;

    case PropertyID::TextWrap:
        VERIFY_NOT_REACHED();

    case PropertyID::TextWrapMode:
        return AnimationType::Discrete;

    case PropertyID::TextWrapStyle:
        return AnimationType::Discrete;

    case PropertyID::TimelineScope:
        return AnimationType::None;

    case PropertyID::Top:
        return AnimationType::ByComputedValue;

    case PropertyID::TouchAction:
        return AnimationType::Discrete;

    case PropertyID::Transform:
        return AnimationType::Custom;

    case PropertyID::TransformBox:
        return AnimationType::Discrete;

    case PropertyID::TransformOrigin:
        return AnimationType::ByComputedValue;

    case PropertyID::TransformStyle:
        return AnimationType::Discrete;

    case PropertyID::Transition:
        VERIFY_NOT_REACHED();

    case PropertyID::TransitionBehavior:
        return AnimationType::None;

    case PropertyID::TransitionDelay:
        return AnimationType::None;

    case PropertyID::TransitionDuration:
        return AnimationType::None;

    case PropertyID::TransitionProperty:
        return AnimationType::None;

    case PropertyID::TransitionTimingFunction:
        return AnimationType::None;

    case PropertyID::Translate:
        return AnimationType::Custom;

    case PropertyID::UnicodeBidi:
        return AnimationType::None;

    case PropertyID::UserSelect:
        return AnimationType::Discrete;

    case PropertyID::VerticalAlign:
        return AnimationType::ByComputedValue;

    case PropertyID::ViewTimeline:
        VERIFY_NOT_REACHED();

    case PropertyID::ViewTimelineAxis:
        return AnimationType::None;

    case PropertyID::ViewTimelineInset:
        return AnimationType::None;

    case PropertyID::ViewTimelineName:
        return AnimationType::None;

    case PropertyID::ViewTransitionName:
        return AnimationType::Discrete;

    case PropertyID::Visibility:
        return AnimationType::Custom;

    case PropertyID::WhiteSpace:
        VERIFY_NOT_REACHED();

    case PropertyID::WhiteSpaceCollapse:
        return AnimationType::Discrete;

    case PropertyID::WhiteSpaceTrim:
        return AnimationType::Discrete;

    case PropertyID::Widows:
        return AnimationType::ByComputedValue;

    case PropertyID::Width:
        return AnimationType::ByComputedValue;

    case PropertyID::WillChange:
        return AnimationType::None;

    case PropertyID::WordBreak:
        return AnimationType::Discrete;

    case PropertyID::WordSpacing:
        return AnimationType::ByComputedValue;

    case PropertyID::WritingMode:
        return AnimationType::None;

    case PropertyID::X:
        return AnimationType::ByComputedValue;

    case PropertyID::Y:
        return AnimationType::ByComputedValue;

    case PropertyID::ZIndex:
        return AnimationType::ByComputedValue;

    default:
        return AnimationType::None;
    }
}

bool is_animatable_property(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::WebkitTextFillColor:

    case PropertyID::AccentColor:

    case PropertyID::AlignContent:

    case PropertyID::AlignItems:

    case PropertyID::AlignSelf:

    case PropertyID::All:

    case PropertyID::AnchorName:

    case PropertyID::AnchorScope:

    case PropertyID::Appearance:

    case PropertyID::AspectRatio:

    case PropertyID::BackdropFilter:

    case PropertyID::Background:

    case PropertyID::BackgroundAttachment:

    case PropertyID::BackgroundBlendMode:

    case PropertyID::BackgroundClip:

    case PropertyID::BackgroundColor:

    case PropertyID::BackgroundImage:

    case PropertyID::BackgroundOrigin:

    case PropertyID::BackgroundPosition:

    case PropertyID::BackgroundPositionX:

    case PropertyID::BackgroundPositionY:

    case PropertyID::BackgroundRepeat:

    case PropertyID::BackgroundSize:

    case PropertyID::BlockSize:

    case PropertyID::Border:

    case PropertyID::BorderBlock:

    case PropertyID::BorderBlockColor:

    case PropertyID::BorderBlockEnd:

    case PropertyID::BorderBlockEndColor:

    case PropertyID::BorderBlockEndStyle:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStart:

    case PropertyID::BorderBlockStartColor:

    case PropertyID::BorderBlockStartStyle:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderBlockStyle:

    case PropertyID::BorderBlockWidth:

    case PropertyID::BorderBottom:

    case PropertyID::BorderBottomColor:

    case PropertyID::BorderBottomLeftRadius:

    case PropertyID::BorderBottomRightRadius:

    case PropertyID::BorderBottomStyle:

    case PropertyID::BorderBottomWidth:

    case PropertyID::BorderCollapse:

    case PropertyID::BorderColor:

    case PropertyID::BorderEndEndRadius:

    case PropertyID::BorderEndStartRadius:

    case PropertyID::BorderImage:

    case PropertyID::BorderImageOutset:

    case PropertyID::BorderImageRepeat:

    case PropertyID::BorderImageSlice:

    case PropertyID::BorderImageSource:

    case PropertyID::BorderImageWidth:

    case PropertyID::BorderInline:

    case PropertyID::BorderInlineColor:

    case PropertyID::BorderInlineEnd:

    case PropertyID::BorderInlineEndColor:

    case PropertyID::BorderInlineEndStyle:

    case PropertyID::BorderInlineEndWidth:

    case PropertyID::BorderInlineStart:

    case PropertyID::BorderInlineStartColor:

    case PropertyID::BorderInlineStartStyle:

    case PropertyID::BorderInlineStartWidth:

    case PropertyID::BorderInlineStyle:

    case PropertyID::BorderInlineWidth:

    case PropertyID::BorderLeft:

    case PropertyID::BorderLeftColor:

    case PropertyID::BorderLeftStyle:

    case PropertyID::BorderLeftWidth:

    case PropertyID::BorderRadius:

    case PropertyID::BorderRight:

    case PropertyID::BorderRightColor:

    case PropertyID::BorderRightStyle:

    case PropertyID::BorderRightWidth:

    case PropertyID::BorderSpacing:

    case PropertyID::BorderStartEndRadius:

    case PropertyID::BorderStartStartRadius:

    case PropertyID::BorderStyle:

    case PropertyID::BorderTop:

    case PropertyID::BorderTopColor:

    case PropertyID::BorderTopLeftRadius:

    case PropertyID::BorderTopRightRadius:

    case PropertyID::BorderTopStyle:

    case PropertyID::BorderTopWidth:

    case PropertyID::BorderWidth:

    case PropertyID::Bottom:

    case PropertyID::BoxShadow:

    case PropertyID::BoxSizing:

    case PropertyID::CaptionSide:

    case PropertyID::CaretColor:

    case PropertyID::Clear:

    case PropertyID::Clip:

    case PropertyID::ClipPath:

    case PropertyID::ClipRule:

    case PropertyID::Color:

    case PropertyID::ColorInterpolation:

    case PropertyID::ColorScheme:

    case PropertyID::ColumnCount:

    case PropertyID::ColumnGap:

    case PropertyID::ColumnHeight:

    case PropertyID::ColumnSpan:

    case PropertyID::ColumnWidth:

    case PropertyID::Columns:

    case PropertyID::Content:

    case PropertyID::ContentVisibility:

    case PropertyID::CornerBlockEndShape:

    case PropertyID::CornerBlockStartShape:

    case PropertyID::CornerBottomLeftShape:

    case PropertyID::CornerBottomRightShape:

    case PropertyID::CornerBottomShape:

    case PropertyID::CornerEndEndShape:

    case PropertyID::CornerEndStartShape:

    case PropertyID::CornerInlineEndShape:

    case PropertyID::CornerInlineStartShape:

    case PropertyID::CornerLeftShape:

    case PropertyID::CornerRightShape:

    case PropertyID::CornerShape:

    case PropertyID::CornerStartEndShape:

    case PropertyID::CornerStartStartShape:

    case PropertyID::CornerTopLeftShape:

    case PropertyID::CornerTopRightShape:

    case PropertyID::CornerTopShape:

    case PropertyID::CounterIncrement:

    case PropertyID::CounterReset:

    case PropertyID::CounterSet:

    case PropertyID::Cursor:

    case PropertyID::Cx:

    case PropertyID::Cy:

    case PropertyID::Display:

    case PropertyID::DominantBaseline:

    case PropertyID::EmptyCells:

    case PropertyID::Fill:

    case PropertyID::FillOpacity:

    case PropertyID::FillRule:

    case PropertyID::Filter:

    case PropertyID::Flex:

    case PropertyID::FlexBasis:

    case PropertyID::FlexDirection:

    case PropertyID::FlexFlow:

    case PropertyID::FlexGrow:

    case PropertyID::FlexShrink:

    case PropertyID::FlexWrap:

    case PropertyID::Float:

    case PropertyID::FloodColor:

    case PropertyID::FloodOpacity:

    case PropertyID::Font:

    case PropertyID::FontFamily:

    case PropertyID::FontFeatureSettings:

    case PropertyID::FontKerning:

    case PropertyID::FontLanguageOverride:

    case PropertyID::FontOpticalSizing:

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

    case PropertyID::Gap:

    case PropertyID::Grid:

    case PropertyID::GridArea:

    case PropertyID::GridAutoColumns:

    case PropertyID::GridAutoFlow:

    case PropertyID::GridAutoRows:

    case PropertyID::GridColumn:

    case PropertyID::GridColumnEnd:

    case PropertyID::GridColumnStart:

    case PropertyID::GridRow:

    case PropertyID::GridRowEnd:

    case PropertyID::GridRowStart:

    case PropertyID::GridTemplate:

    case PropertyID::GridTemplateAreas:

    case PropertyID::GridTemplateColumns:

    case PropertyID::GridTemplateRows:

    case PropertyID::Height:

    case PropertyID::ImageRendering:

    case PropertyID::InlineSize:

    case PropertyID::Inset:

    case PropertyID::InsetBlock:

    case PropertyID::InsetBlockEnd:

    case PropertyID::InsetBlockStart:

    case PropertyID::InsetInline:

    case PropertyID::InsetInlineEnd:

    case PropertyID::InsetInlineStart:

    case PropertyID::Isolation:

    case PropertyID::JustifyContent:

    case PropertyID::JustifyItems:

    case PropertyID::JustifySelf:

    case PropertyID::Left:

    case PropertyID::LetterSpacing:

    case PropertyID::LineHeight:

    case PropertyID::ListStyle:

    case PropertyID::ListStyleImage:

    case PropertyID::ListStylePosition:

    case PropertyID::ListStyleType:

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

    case PropertyID::Mask:

    case PropertyID::MaskClip:

    case PropertyID::MaskComposite:

    case PropertyID::MaskImage:

    case PropertyID::MaskMode:

    case PropertyID::MaskOrigin:

    case PropertyID::MaskPosition:

    case PropertyID::MaskRepeat:

    case PropertyID::MaskSize:

    case PropertyID::MaskType:

    case PropertyID::MaxBlockSize:

    case PropertyID::MaxHeight:

    case PropertyID::MaxInlineSize:

    case PropertyID::MaxWidth:

    case PropertyID::MinBlockSize:

    case PropertyID::MinHeight:

    case PropertyID::MinInlineSize:

    case PropertyID::MinWidth:

    case PropertyID::MixBlendMode:

    case PropertyID::ObjectFit:

    case PropertyID::ObjectPosition:

    case PropertyID::Opacity:

    case PropertyID::Order:

    case PropertyID::Orphans:

    case PropertyID::Outline:

    case PropertyID::OutlineColor:

    case PropertyID::OutlineOffset:

    case PropertyID::OutlineStyle:

    case PropertyID::OutlineWidth:

    case PropertyID::Overflow:

    case PropertyID::OverflowBlock:

    case PropertyID::OverflowClipMargin:

    case PropertyID::OverflowClipMarginBlock:

    case PropertyID::OverflowClipMarginBlockEnd:

    case PropertyID::OverflowClipMarginBlockStart:

    case PropertyID::OverflowClipMarginBottom:

    case PropertyID::OverflowClipMarginInline:

    case PropertyID::OverflowClipMarginInlineEnd:

    case PropertyID::OverflowClipMarginInlineStart:

    case PropertyID::OverflowClipMarginLeft:

    case PropertyID::OverflowClipMarginRight:

    case PropertyID::OverflowClipMarginTop:

    case PropertyID::OverflowInline:

    case PropertyID::OverflowWrap:

    case PropertyID::OverflowX:

    case PropertyID::OverflowY:

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

    case PropertyID::PaintOrder:

    case PropertyID::Perspective:

    case PropertyID::PerspectiveOrigin:

    case PropertyID::PlaceContent:

    case PropertyID::PlaceItems:

    case PropertyID::PlaceSelf:

    case PropertyID::PointerEvents:

    case PropertyID::Position:

    case PropertyID::PositionAnchor:

    case PropertyID::PositionArea:

    case PropertyID::PositionTryFallbacks:

    case PropertyID::PositionTryOrder:

    case PropertyID::PositionVisibility:

    case PropertyID::Quotes:

    case PropertyID::R:

    case PropertyID::Right:

    case PropertyID::Rotate:

    case PropertyID::RowGap:

    case PropertyID::Rx:

    case PropertyID::Ry:

    case PropertyID::Scale:

    case PropertyID::ScrollbarColor:

    case PropertyID::ScrollbarGutter:

    case PropertyID::ScrollbarWidth:

    case PropertyID::ShapeImageThreshold:

    case PropertyID::ShapeMargin:

    case PropertyID::ShapeOutside:

    case PropertyID::ShapeRendering:

    case PropertyID::StopColor:

    case PropertyID::StopOpacity:

    case PropertyID::Stroke:

    case PropertyID::StrokeDasharray:

    case PropertyID::StrokeDashoffset:

    case PropertyID::StrokeLinecap:

    case PropertyID::StrokeLinejoin:

    case PropertyID::StrokeMiterlimit:

    case PropertyID::StrokeOpacity:

    case PropertyID::StrokeWidth:

    case PropertyID::TabSize:

    case PropertyID::TableLayout:

    case PropertyID::TextAlign:

    case PropertyID::TextAnchor:

    case PropertyID::TextDecoration:

    case PropertyID::TextDecorationColor:

    case PropertyID::TextDecorationLine:

    case PropertyID::TextDecorationStyle:

    case PropertyID::TextDecorationThickness:

    case PropertyID::TextIndent:

    case PropertyID::TextJustify:

    case PropertyID::TextOverflow:

    case PropertyID::TextRendering:

    case PropertyID::TextShadow:

    case PropertyID::TextTransform:

    case PropertyID::TextUnderlineOffset:

    case PropertyID::TextUnderlinePosition:

    case PropertyID::TextWrap:

    case PropertyID::TextWrapMode:

    case PropertyID::TextWrapStyle:

    case PropertyID::Top:

    case PropertyID::TouchAction:

    case PropertyID::Transform:

    case PropertyID::TransformBox:

    case PropertyID::TransformOrigin:

    case PropertyID::TransformStyle:

    case PropertyID::Translate:

    case PropertyID::UserSelect:

    case PropertyID::VerticalAlign:

    case PropertyID::ViewTransitionName:

    case PropertyID::Visibility:

    case PropertyID::WhiteSpace:

    case PropertyID::WhiteSpaceCollapse:

    case PropertyID::WhiteSpaceTrim:

    case PropertyID::Widows:

    case PropertyID::Width:

    case PropertyID::WordBreak:

    case PropertyID::WordSpacing:

    case PropertyID::X:

    case PropertyID::Y:

    case PropertyID::ZIndex:

        return true;
    default:
        return false;
    }
}

bool is_inherited_property(PropertyID property_id)
{
    if (property_id >= first_inherited_property_id && property_id <= last_inherited_property_id)
        return true;
    return false;
}

bool property_affects_layout(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::AlignContent:

    case PropertyID::AlignItems:

    case PropertyID::AlignSelf:

    case PropertyID::All:

    case PropertyID::AnchorName:

    case PropertyID::Appearance:

    case PropertyID::AspectRatio:

    case PropertyID::BlockSize:

    case PropertyID::Border:

    case PropertyID::BorderBlock:

    case PropertyID::BorderBlockEnd:

    case PropertyID::BorderBlockEndStyle:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStart:

    case PropertyID::BorderBlockStartStyle:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderBlockStyle:

    case PropertyID::BorderBlockWidth:

    case PropertyID::BorderBottom:

    case PropertyID::BorderBottomStyle:

    case PropertyID::BorderBottomWidth:

    case PropertyID::BorderCollapse:

    case PropertyID::BorderImage:

    case PropertyID::BorderInline:

    case PropertyID::BorderInlineEnd:

    case PropertyID::BorderInlineEndStyle:

    case PropertyID::BorderInlineEndWidth:

    case PropertyID::BorderInlineStart:

    case PropertyID::BorderInlineStartStyle:

    case PropertyID::BorderInlineStartWidth:

    case PropertyID::BorderInlineStyle:

    case PropertyID::BorderInlineWidth:

    case PropertyID::BorderLeft:

    case PropertyID::BorderLeftStyle:

    case PropertyID::BorderLeftWidth:

    case PropertyID::BorderRight:

    case PropertyID::BorderRightStyle:

    case PropertyID::BorderRightWidth:

    case PropertyID::BorderSpacing:

    case PropertyID::BorderStyle:

    case PropertyID::BorderTop:

    case PropertyID::BorderTopStyle:

    case PropertyID::BorderTopWidth:

    case PropertyID::BorderWidth:

    case PropertyID::Bottom:

    case PropertyID::BoxSizing:

    case PropertyID::CaptionSide:

    case PropertyID::Clear:

    case PropertyID::ColumnCount:

    case PropertyID::ColumnGap:

    case PropertyID::ColumnHeight:

    case PropertyID::ColumnSpan:

    case PropertyID::ColumnWidth:

    case PropertyID::Columns:

    case PropertyID::Contain:

    case PropertyID::ContainerType:

    case PropertyID::Content:

    case PropertyID::ContentVisibility:

    case PropertyID::CornerBlockEndShape:

    case PropertyID::CornerBlockStartShape:

    case PropertyID::CornerBottomLeftShape:

    case PropertyID::CornerBottomRightShape:

    case PropertyID::CornerBottomShape:

    case PropertyID::CornerEndEndShape:

    case PropertyID::CornerEndStartShape:

    case PropertyID::CornerInlineEndShape:

    case PropertyID::CornerInlineStartShape:

    case PropertyID::CornerLeftShape:

    case PropertyID::CornerRightShape:

    case PropertyID::CornerShape:

    case PropertyID::CornerStartEndShape:

    case PropertyID::CornerStartStartShape:

    case PropertyID::CornerTopLeftShape:

    case PropertyID::CornerTopRightShape:

    case PropertyID::CornerTopShape:

    case PropertyID::CounterIncrement:

    case PropertyID::CounterReset:

    case PropertyID::CounterSet:

    case PropertyID::Cx:

    case PropertyID::Cy:

    case PropertyID::Direction:

    case PropertyID::Display:

    case PropertyID::DominantBaseline:

    case PropertyID::EmptyCells:

    case PropertyID::Flex:

    case PropertyID::FlexBasis:

    case PropertyID::FlexDirection:

    case PropertyID::FlexFlow:

    case PropertyID::FlexGrow:

    case PropertyID::FlexShrink:

    case PropertyID::FlexWrap:

    case PropertyID::Float:

    case PropertyID::Font:

    case PropertyID::FontFamily:

    case PropertyID::FontFeatureSettings:

    case PropertyID::FontKerning:

    case PropertyID::FontLanguageOverride:

    case PropertyID::FontOpticalSizing:

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

    case PropertyID::Gap:

    case PropertyID::Grid:

    case PropertyID::GridArea:

    case PropertyID::GridAutoColumns:

    case PropertyID::GridAutoFlow:

    case PropertyID::GridAutoRows:

    case PropertyID::GridColumn:

    case PropertyID::GridColumnEnd:

    case PropertyID::GridColumnStart:

    case PropertyID::GridRow:

    case PropertyID::GridRowEnd:

    case PropertyID::GridRowStart:

    case PropertyID::GridTemplate:

    case PropertyID::GridTemplateAreas:

    case PropertyID::GridTemplateColumns:

    case PropertyID::GridTemplateRows:

    case PropertyID::Height:

    case PropertyID::InlineSize:

    case PropertyID::Inset:

    case PropertyID::InsetBlock:

    case PropertyID::InsetBlockEnd:

    case PropertyID::InsetBlockStart:

    case PropertyID::InsetInline:

    case PropertyID::InsetInlineEnd:

    case PropertyID::InsetInlineStart:

    case PropertyID::Isolation:

    case PropertyID::JustifyContent:

    case PropertyID::JustifyItems:

    case PropertyID::JustifySelf:

    case PropertyID::Left:

    case PropertyID::LetterSpacing:

    case PropertyID::LineHeight:

    case PropertyID::ListStyle:

    case PropertyID::ListStyleImage:

    case PropertyID::ListStylePosition:

    case PropertyID::ListStyleType:

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

    case PropertyID::MathDepth:

    case PropertyID::MathShift:

    case PropertyID::MathStyle:

    case PropertyID::MaxBlockSize:

    case PropertyID::MaxHeight:

    case PropertyID::MaxInlineSize:

    case PropertyID::MaxWidth:

    case PropertyID::MinBlockSize:

    case PropertyID::MinHeight:

    case PropertyID::MinInlineSize:

    case PropertyID::MinWidth:

    case PropertyID::Order:

    case PropertyID::Orphans:

    case PropertyID::Overflow:

    case PropertyID::OverflowBlock:

    case PropertyID::OverflowClipMargin:

    case PropertyID::OverflowClipMarginBlock:

    case PropertyID::OverflowClipMarginBlockEnd:

    case PropertyID::OverflowClipMarginBlockStart:

    case PropertyID::OverflowClipMarginBottom:

    case PropertyID::OverflowClipMarginInline:

    case PropertyID::OverflowClipMarginInlineEnd:

    case PropertyID::OverflowClipMarginInlineStart:

    case PropertyID::OverflowClipMarginLeft:

    case PropertyID::OverflowClipMarginRight:

    case PropertyID::OverflowClipMarginTop:

    case PropertyID::OverflowInline:

    case PropertyID::OverflowWrap:

    case PropertyID::OverflowX:

    case PropertyID::OverflowY:

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

    case PropertyID::PaintOrder:

    case PropertyID::Perspective:

    case PropertyID::PlaceContent:

    case PropertyID::PlaceItems:

    case PropertyID::PlaceSelf:

    case PropertyID::Position:

    case PropertyID::PositionAnchor:

    case PropertyID::PositionArea:

    case PropertyID::PositionTryFallbacks:

    case PropertyID::PositionTryOrder:

    case PropertyID::PositionVisibility:

    case PropertyID::Quotes:

    case PropertyID::R:

    case PropertyID::Right:

    case PropertyID::RowGap:

    case PropertyID::Rx:

    case PropertyID::Ry:

    case PropertyID::ShapeMargin:

    case PropertyID::ShapeOutside:

    case PropertyID::ShapeRendering:

    case PropertyID::TabSize:

    case PropertyID::TableLayout:

    case PropertyID::TextAlign:

    case PropertyID::TextAnchor:

    case PropertyID::TextIndent:

    case PropertyID::TextJustify:

    case PropertyID::TextTransform:

    case PropertyID::TextUnderlineOffset:

    case PropertyID::TextUnderlinePosition:

    case PropertyID::TextWrap:

    case PropertyID::TextWrapMode:

    case PropertyID::TextWrapStyle:

    case PropertyID::Top:

    case PropertyID::TransformStyle:

    case PropertyID::UnicodeBidi:

    case PropertyID::VerticalAlign:

    case PropertyID::Visibility:

    case PropertyID::WhiteSpace:

    case PropertyID::WhiteSpaceCollapse:

    case PropertyID::WhiteSpaceTrim:

    case PropertyID::Widows:

    case PropertyID::Width:

    case PropertyID::WordBreak:

    case PropertyID::WordSpacing:

    case PropertyID::WritingMode:

    case PropertyID::X:

    case PropertyID::Y:

        return true;
    default:
        return false;
    }
}

bool property_affects_stacking_context(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::All:

    case PropertyID::BackdropFilter:

    case PropertyID::ClipPath:

    case PropertyID::Contain:

    case PropertyID::ContainerType:

    case PropertyID::Filter:

    case PropertyID::MaskImage:

    case PropertyID::Opacity:

    case PropertyID::Rotate:

    case PropertyID::Scale:

    case PropertyID::Transform:

    case PropertyID::Translate:

    case PropertyID::ViewTransitionName:

    case PropertyID::WillChange:

    case PropertyID::ZIndex:

        return true;
    default:
        return false;
    }
}

bool property_needs_layout_for_getcomputedstyle(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::BlockSize:

    case PropertyID::Border:

    case PropertyID::BorderBlock:

    case PropertyID::BorderBlockEnd:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStart:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderBlockWidth:

    case PropertyID::BorderBottom:

    case PropertyID::BorderBottomWidth:

    case PropertyID::BorderImageSlice:

    case PropertyID::BorderImageWidth:

    case PropertyID::BorderInline:

    case PropertyID::BorderInlineEnd:

    case PropertyID::Bottom:

    case PropertyID::GridTemplate:

    case PropertyID::GridTemplateColumns:

    case PropertyID::GridTemplateRows:

    case PropertyID::Height:

    case PropertyID::InlineSize:

    case PropertyID::Inset:

    case PropertyID::InsetBlock:

    case PropertyID::InsetBlockEnd:

    case PropertyID::InsetBlockStart:

    case PropertyID::InsetInline:

    case PropertyID::InsetInlineEnd:

    case PropertyID::InsetInlineStart:

    case PropertyID::Left:

    case PropertyID::LineHeight:

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

    case PropertyID::Right:

    case PropertyID::Top:

    case PropertyID::Width:

        return true;
    default:
        return false;
    }
}

bool property_needs_layout_node_for_resolved_value(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::WebkitTextFillColor:

    case PropertyID::AnimationDuration:

    case PropertyID::BackdropFilter:

    case PropertyID::BackgroundColor:

    case PropertyID::BorderBlockEndColor:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStartColor:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderBottomColor:

    case PropertyID::BorderBottomWidth:

    case PropertyID::BorderInlineEndColor:

    case PropertyID::BorderInlineEndWidth:

    case PropertyID::BorderInlineStartColor:

    case PropertyID::BorderInlineStartWidth:

    case PropertyID::BorderLeftColor:

    case PropertyID::BorderLeftWidth:

    case PropertyID::BorderRightColor:

    case PropertyID::BorderRightWidth:

    case PropertyID::BorderTopColor:

    case PropertyID::BorderTopWidth:

    case PropertyID::BoxShadow:

    case PropertyID::CaretColor:

    case PropertyID::Color:

    case PropertyID::Contain:

    case PropertyID::Filter:

    case PropertyID::FontWeight:

    case PropertyID::FontWidth:

    case PropertyID::LetterSpacing:

    case PropertyID::LineHeight:

    case PropertyID::MathDepth:

    case PropertyID::OutlineColor:

    case PropertyID::TextDecorationColor:

    case PropertyID::TextShadow:

    case PropertyID::Transform:

    case PropertyID::ZIndex:

        return true;
    default:
        return false;
    }
}

NonnullRefPtr<StyleValue const> property_initial_value(PropertyID property_id)
{
    static Array<RefPtr<StyleValue const>, to_underlying(last_property_id) + 1> initial_values;
    if (auto initial_value = initial_values[to_underlying(property_id)])
        return initial_value.release_nonnull();

    // Lazily parse initial values as needed.
    // This ensures the shorthands will always be able to get the initial values of their longhands.
    // This also now allows a longhand have its own longhand (like background-position-x).

    Parser::ParsingParams parsing_params;
    switch (property_id) {
        case PropertyID::WebkitTextFillColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentColor"sv, PropertyID::WebkitTextFillColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WebkitTextFillColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::AccentColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::AccentColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AccentColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::AlignContent:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::AlignContent);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AlignContent)] = initial_value;
            return initial_value;
        }
        case PropertyID::AlignItems:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::AlignItems);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AlignItems)] = initial_value;
            return initial_value;
        }
        case PropertyID::AlignSelf:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::AlignSelf);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AlignSelf)] = initial_value;
            return initial_value;
        }
        case PropertyID::All:
        {
            auto parsed_value = parse_css_value(parsing_params, "initial"sv, PropertyID::All);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::All)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnchorName:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::AnchorName);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnchorName)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnchorScope:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::AnchorScope);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnchorScope)] = initial_value;
            return initial_value;
        }
        case PropertyID::Animation:
        {
            auto parsed_value = parse_css_value(parsing_params, "none 0s ease 1 normal running 0s none"sv, PropertyID::Animation);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Animation)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationComposition:
        {
            auto parsed_value = parse_css_value(parsing_params, "replace"sv, PropertyID::AnimationComposition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationComposition)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationDelay:
        {
            auto parsed_value = parse_css_value(parsing_params, "0s"sv, PropertyID::AnimationDelay);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationDelay)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationDirection:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::AnimationDirection);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationDirection)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationDuration:
        {
            auto parsed_value = parse_css_value(parsing_params, "0s"sv, PropertyID::AnimationDuration);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationDuration)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationFillMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::AnimationFillMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationFillMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationIterationCount:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::AnimationIterationCount);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationIterationCount)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationName:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::AnimationName);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationName)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationPlayState:
        {
            auto parsed_value = parse_css_value(parsing_params, "running"sv, PropertyID::AnimationPlayState);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationPlayState)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationTimeline:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::AnimationTimeline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationTimeline)] = initial_value;
            return initial_value;
        }
        case PropertyID::AnimationTimingFunction:
        {
            auto parsed_value = parse_css_value(parsing_params, "ease"sv, PropertyID::AnimationTimingFunction);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AnimationTimingFunction)] = initial_value;
            return initial_value;
        }
        case PropertyID::Appearance:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Appearance);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Appearance)] = initial_value;
            return initial_value;
        }
        case PropertyID::AspectRatio:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::AspectRatio);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::AspectRatio)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackdropFilter:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BackdropFilter);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackdropFilter)] = initial_value;
            return initial_value;
        }
        case PropertyID::Background:
        {
            auto parsed_value = parse_css_value(parsing_params, "transparent"sv, PropertyID::Background);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Background)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundAttachment:
        {
            auto parsed_value = parse_css_value(parsing_params, "scroll"sv, PropertyID::BackgroundAttachment);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundAttachment)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundBlendMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::BackgroundBlendMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundBlendMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundClip:
        {
            auto parsed_value = parse_css_value(parsing_params, "border-box"sv, PropertyID::BackgroundClip);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundClip)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "transparent"sv, PropertyID::BackgroundColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundImage:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BackgroundImage);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundImage)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundOrigin:
        {
            auto parsed_value = parse_css_value(parsing_params, "padding-box"sv, PropertyID::BackgroundOrigin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundOrigin)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundPosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "0% 0%"sv, PropertyID::BackgroundPosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundPosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundPositionX:
        {
            auto parsed_value = parse_css_value(parsing_params, "0%"sv, PropertyID::BackgroundPositionX);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundPositionX)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundPositionY:
        {
            auto parsed_value = parse_css_value(parsing_params, "0%"sv, PropertyID::BackgroundPositionY);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundPositionY)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundRepeat:
        {
            auto parsed_value = parse_css_value(parsing_params, "repeat"sv, PropertyID::BackgroundRepeat);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundRepeat)] = initial_value;
            return initial_value;
        }
        case PropertyID::BackgroundSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::BackgroundSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BackgroundSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::BlockSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::BlockSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BlockSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::Border:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::Border);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Border)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderBlockColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderBlockEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockEndColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderBlockEndColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockEndColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockEndStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderBlockEndStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockEndStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockEndWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderBlockEndWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockEndWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderBlockStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockStartColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderBlockStartColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockStartColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockStartStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderBlockStartStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockStartStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockStartWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderBlockStartWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockStartWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderBlockStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBlockWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderBlockWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBlockWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottom:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderBottom);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottom)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottomColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderBottomColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottomColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottomLeftRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderBottomLeftRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottomLeftRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottomRightRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderBottomRightRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottomRightRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottomStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderBottomStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottomStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderBottomWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderBottomWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderBottomWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderCollapse:
        {
            auto parsed_value = parse_css_value(parsing_params, "separate"sv, PropertyID::BorderCollapse);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderCollapse)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderEndEndRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderEndEndRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderEndEndRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderEndStartRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderEndStartRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderEndStartRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImage:
        {
            auto parsed_value = parse_css_value(parsing_params, "none 100% / 1 / 0 stretch"sv, PropertyID::BorderImage);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImage)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImageOutset:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderImageOutset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImageOutset)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImageRepeat:
        {
            auto parsed_value = parse_css_value(parsing_params, "stretch"sv, PropertyID::BorderImageRepeat);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImageRepeat)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImageSlice:
        {
            auto parsed_value = parse_css_value(parsing_params, "100%"sv, PropertyID::BorderImageSlice);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImageSlice)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImageSource:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderImageSource);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImageSource)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderImageWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::BorderImageWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderImageWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderInlineColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderInlineEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineEndColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderInlineEndColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineEndColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineEndStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderInlineEndStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineEndStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineEndWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderInlineEndWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineEndWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderInlineStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineStartColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderInlineStartColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineStartColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineStartStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderInlineStartStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineStartStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineStartWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderInlineStartWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineStartWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderInlineStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderInlineWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderInlineWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderInlineWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderLeft:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderLeft);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderLeft)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderLeftColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderLeftColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderLeftColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderLeftStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderLeftStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderLeftStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderLeftWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderLeftWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderLeftWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderRight:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderRight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderRight)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderRightColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderRightColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderRightColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderRightStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderRightStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderRightStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderRightWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderRightWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderRightWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderSpacing:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderSpacing);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderSpacing)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderStartEndRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderStartEndRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderStartEndRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderStartStartRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderStartStartRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderStartStartRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTop:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentcolor none"sv, PropertyID::BorderTop);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTop)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTopColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::BorderTopColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTopColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTopLeftRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderTopLeftRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTopLeftRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTopRightRadius:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::BorderTopRightRadius);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTopRightRadius)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTopStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BorderTopStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTopStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderTopWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderTopWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderTopWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::BorderWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::BorderWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BorderWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::Bottom:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Bottom);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Bottom)] = initial_value;
            return initial_value;
        }
        case PropertyID::BoxShadow:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::BoxShadow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BoxShadow)] = initial_value;
            return initial_value;
        }
        case PropertyID::BoxSizing:
        {
            auto parsed_value = parse_css_value(parsing_params, "content-box"sv, PropertyID::BoxSizing);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::BoxSizing)] = initial_value;
            return initial_value;
        }
        case PropertyID::CaptionSide:
        {
            auto parsed_value = parse_css_value(parsing_params, "top"sv, PropertyID::CaptionSide);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CaptionSide)] = initial_value;
            return initial_value;
        }
        case PropertyID::CaretColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::CaretColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CaretColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::Clear:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Clear);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Clear)] = initial_value;
            return initial_value;
        }
        case PropertyID::Clip:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Clip);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Clip)] = initial_value;
            return initial_value;
        }
        case PropertyID::ClipPath:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ClipPath);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ClipPath)] = initial_value;
            return initial_value;
        }
        case PropertyID::ClipRule:
        {
            auto parsed_value = parse_css_value(parsing_params, "nonzero"sv, PropertyID::ClipRule);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ClipRule)] = initial_value;
            return initial_value;
        }
        case PropertyID::Color:
        {
            auto parsed_value = parse_css_value(parsing_params, "canvastext"sv, PropertyID::Color);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Color)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColorInterpolation:
        {
            auto parsed_value = parse_css_value(parsing_params, "srgb"sv, PropertyID::ColorInterpolation);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColorInterpolation)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColorScheme:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::ColorScheme);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColorScheme)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColumnCount:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ColumnCount);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColumnCount)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColumnGap:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::ColumnGap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColumnGap)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColumnHeight:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ColumnHeight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColumnHeight)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColumnSpan:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ColumnSpan);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColumnSpan)] = initial_value;
            return initial_value;
        }
        case PropertyID::ColumnWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ColumnWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ColumnWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::Columns:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto auto / auto"sv, PropertyID::Columns);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Columns)] = initial_value;
            return initial_value;
        }
        case PropertyID::Contain:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Contain);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Contain)] = initial_value;
            return initial_value;
        }
        case PropertyID::ContainerType:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::ContainerType);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ContainerType)] = initial_value;
            return initial_value;
        }
        case PropertyID::Content:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::Content);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Content)] = initial_value;
            return initial_value;
        }
        case PropertyID::ContentVisibility:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::ContentVisibility);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ContentVisibility)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerBlockEndShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerBlockEndShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerBlockEndShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerBlockStartShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerBlockStartShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerBlockStartShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerBottomLeftShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerBottomLeftShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerBottomLeftShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerBottomRightShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerBottomRightShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerBottomRightShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerBottomShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerBottomShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerBottomShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerEndEndShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerEndEndShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerEndEndShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerEndStartShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerEndStartShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerEndStartShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerInlineEndShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerInlineEndShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerInlineEndShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerInlineStartShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerInlineStartShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerInlineStartShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerLeftShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerLeftShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerLeftShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerRightShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerRightShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerRightShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round round round"sv, PropertyID::CornerShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerStartEndShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerStartEndShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerStartEndShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerStartStartShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerStartStartShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerStartStartShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerTopLeftShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerTopLeftShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerTopLeftShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerTopRightShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round"sv, PropertyID::CornerTopRightShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerTopRightShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CornerTopShape:
        {
            auto parsed_value = parse_css_value(parsing_params, "round round"sv, PropertyID::CornerTopShape);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CornerTopShape)] = initial_value;
            return initial_value;
        }
        case PropertyID::CounterIncrement:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::CounterIncrement);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CounterIncrement)] = initial_value;
            return initial_value;
        }
        case PropertyID::CounterReset:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::CounterReset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CounterReset)] = initial_value;
            return initial_value;
        }
        case PropertyID::CounterSet:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::CounterSet);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::CounterSet)] = initial_value;
            return initial_value;
        }
        case PropertyID::Cursor:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Cursor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Cursor)] = initial_value;
            return initial_value;
        }
        case PropertyID::Cx:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Cx);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Cx)] = initial_value;
            return initial_value;
        }
        case PropertyID::Cy:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Cy);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Cy)] = initial_value;
            return initial_value;
        }
        case PropertyID::Direction:
        {
            auto parsed_value = parse_css_value(parsing_params, "ltr"sv, PropertyID::Direction);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Direction)] = initial_value;
            return initial_value;
        }
        case PropertyID::Display:
        {
            auto parsed_value = parse_css_value(parsing_params, "inline"sv, PropertyID::Display);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Display)] = initial_value;
            return initial_value;
        }
        case PropertyID::DominantBaseline:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::DominantBaseline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::DominantBaseline)] = initial_value;
            return initial_value;
        }
        case PropertyID::EmptyCells:
        {
            auto parsed_value = parse_css_value(parsing_params, "show"sv, PropertyID::EmptyCells);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::EmptyCells)] = initial_value;
            return initial_value;
        }
        case PropertyID::Fill:
        {
            auto parsed_value = parse_css_value(parsing_params, "black"sv, PropertyID::Fill);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Fill)] = initial_value;
            return initial_value;
        }
        case PropertyID::FillOpacity:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::FillOpacity);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FillOpacity)] = initial_value;
            return initial_value;
        }
        case PropertyID::FillRule:
        {
            auto parsed_value = parse_css_value(parsing_params, "nonzero"sv, PropertyID::FillRule);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FillRule)] = initial_value;
            return initial_value;
        }
        case PropertyID::Filter:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Filter);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Filter)] = initial_value;
            return initial_value;
        }
        case PropertyID::Flex:
        {
            auto parsed_value = parse_css_value(parsing_params, "0 1 auto"sv, PropertyID::Flex);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Flex)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexBasis:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::FlexBasis);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexBasis)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexDirection:
        {
            auto parsed_value = parse_css_value(parsing_params, "row"sv, PropertyID::FlexDirection);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexDirection)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexFlow:
        {
            auto parsed_value = parse_css_value(parsing_params, "row nowrap"sv, PropertyID::FlexFlow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexFlow)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexGrow:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::FlexGrow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexGrow)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexShrink:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::FlexShrink);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexShrink)] = initial_value;
            return initial_value;
        }
        case PropertyID::FlexWrap:
        {
            auto parsed_value = parse_css_value(parsing_params, "nowrap"sv, PropertyID::FlexWrap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FlexWrap)] = initial_value;
            return initial_value;
        }
        case PropertyID::Float:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Float);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Float)] = initial_value;
            return initial_value;
        }
        case PropertyID::FloodColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "black"sv, PropertyID::FloodColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FloodColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::FloodOpacity:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::FloodOpacity);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FloodOpacity)] = initial_value;
            return initial_value;
        }
        case PropertyID::Font:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal medium serif"sv, PropertyID::Font);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Font)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontFamily:
        {
            auto parsed_value = parse_css_value(parsing_params, "serif"sv, PropertyID::FontFamily);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontFamily)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontFeatureSettings:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontFeatureSettings);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontFeatureSettings)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontKerning:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::FontKerning);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontKerning)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontLanguageOverride:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontLanguageOverride);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontLanguageOverride)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontOpticalSizing:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::FontOpticalSizing);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontOpticalSizing)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::FontSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariant:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariant);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariant)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantAlternates:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantAlternates);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantAlternates)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantCaps:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantCaps);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantCaps)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantEastAsian:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantEastAsian);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantEastAsian)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantEmoji:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantEmoji);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantEmoji)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantLigatures:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantLigatures);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantLigatures)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantNumeric:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantNumeric);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantNumeric)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariantPosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariantPosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariantPosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontVariationSettings:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontVariationSettings);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontVariationSettings)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontWeight:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontWeight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontWeight)] = initial_value;
            return initial_value;
        }
        case PropertyID::FontWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::FontWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::FontWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::Gap:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::Gap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Gap)] = initial_value;
            return initial_value;
        }
        case PropertyID::Grid:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Grid);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Grid)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridArea:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridArea);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridArea)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridAutoColumns:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridAutoColumns);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridAutoColumns)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridAutoFlow:
        {
            auto parsed_value = parse_css_value(parsing_params, "row"sv, PropertyID::GridAutoFlow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridAutoFlow)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridAutoRows:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridAutoRows);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridAutoRows)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridColumn:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridColumn);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridColumn)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridColumnEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridColumnEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridColumnEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridColumnStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridColumnStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridColumnStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridRow:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridRow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridRow)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridRowEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridRowEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridRowEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridRowStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridRowStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridRowStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridTemplate:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::GridTemplate);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridTemplate)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridTemplateAreas:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::GridTemplateAreas);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridTemplateAreas)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridTemplateColumns:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::GridTemplateColumns);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridTemplateColumns)] = initial_value;
            return initial_value;
        }
        case PropertyID::GridTemplateRows:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::GridTemplateRows);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::GridTemplateRows)] = initial_value;
            return initial_value;
        }
        case PropertyID::Height:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Height);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Height)] = initial_value;
            return initial_value;
        }
        case PropertyID::ImageRendering:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ImageRendering);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ImageRendering)] = initial_value;
            return initial_value;
        }
        case PropertyID::InlineSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InlineSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InlineSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::Inset:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Inset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Inset)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetBlockEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetBlockEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetBlockEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetBlockStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetBlockStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetBlockStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetInlineEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetInlineEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetInlineEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::InsetInlineStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::InsetInlineStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::InsetInlineStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::Isolation:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Isolation);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Isolation)] = initial_value;
            return initial_value;
        }
        case PropertyID::JustifyContent:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::JustifyContent);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::JustifyContent)] = initial_value;
            return initial_value;
        }
        case PropertyID::JustifyItems:
        {
            auto parsed_value = parse_css_value(parsing_params, "legacy"sv, PropertyID::JustifyItems);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::JustifyItems)] = initial_value;
            return initial_value;
        }
        case PropertyID::JustifySelf:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::JustifySelf);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::JustifySelf)] = initial_value;
            return initial_value;
        }
        case PropertyID::Left:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Left);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Left)] = initial_value;
            return initial_value;
        }
        case PropertyID::LetterSpacing:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::LetterSpacing);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::LetterSpacing)] = initial_value;
            return initial_value;
        }
        case PropertyID::LineHeight:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::LineHeight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::LineHeight)] = initial_value;
            return initial_value;
        }
        case PropertyID::ListStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "outside none disc"sv, PropertyID::ListStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ListStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::ListStyleImage:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ListStyleImage);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ListStyleImage)] = initial_value;
            return initial_value;
        }
        case PropertyID::ListStylePosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "outside"sv, PropertyID::ListStylePosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ListStylePosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::ListStyleType:
        {
            auto parsed_value = parse_css_value(parsing_params, "disc"sv, PropertyID::ListStyleType);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ListStyleType)] = initial_value;
            return initial_value;
        }
        case PropertyID::Margin:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Margin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Margin)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginBlockEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginBlockEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginBlockEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginBlockStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginBlockStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginBlockStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginBottom:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginBottom);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginBottom)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginInlineEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginInlineEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginInlineEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginInlineStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginInlineStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginInlineStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginLeft:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginLeft);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginLeft)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginRight:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginRight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginRight)] = initial_value;
            return initial_value;
        }
        case PropertyID::MarginTop:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MarginTop);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MarginTop)] = initial_value;
            return initial_value;
        }
        case PropertyID::Mask:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Mask);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Mask)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskClip:
        {
            auto parsed_value = parse_css_value(parsing_params, "border-box"sv, PropertyID::MaskClip);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskClip)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskComposite:
        {
            auto parsed_value = parse_css_value(parsing_params, "add"sv, PropertyID::MaskComposite);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskComposite)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskImage:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::MaskImage);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskImage)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "match-source"sv, PropertyID::MaskMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskOrigin:
        {
            auto parsed_value = parse_css_value(parsing_params, "border-box"sv, PropertyID::MaskOrigin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskOrigin)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskPosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "0% 0%"sv, PropertyID::MaskPosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskPosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskRepeat:
        {
            auto parsed_value = parse_css_value(parsing_params, "repeat"sv, PropertyID::MaskRepeat);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskRepeat)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::MaskSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaskType:
        {
            auto parsed_value = parse_css_value(parsing_params, "luminance"sv, PropertyID::MaskType);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaskType)] = initial_value;
            return initial_value;
        }
        case PropertyID::MathDepth:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MathDepth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MathDepth)] = initial_value;
            return initial_value;
        }
        case PropertyID::MathShift:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::MathShift);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MathShift)] = initial_value;
            return initial_value;
        }
        case PropertyID::MathStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::MathStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MathStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaxBlockSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::MaxBlockSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaxBlockSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaxHeight:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::MaxHeight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaxHeight)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaxInlineSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::MaxInlineSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaxInlineSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::MaxWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::MaxWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MaxWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::MinBlockSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MinBlockSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MinBlockSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::MinHeight:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::MinHeight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MinHeight)] = initial_value;
            return initial_value;
        }
        case PropertyID::MinInlineSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::MinInlineSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MinInlineSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::MinWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::MinWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MinWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::MixBlendMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::MixBlendMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::MixBlendMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::ObjectFit:
        {
            auto parsed_value = parse_css_value(parsing_params, "fill"sv, PropertyID::ObjectFit);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ObjectFit)] = initial_value;
            return initial_value;
        }
        case PropertyID::ObjectPosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "50% 50%"sv, PropertyID::ObjectPosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ObjectPosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::Opacity:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::Opacity);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Opacity)] = initial_value;
            return initial_value;
        }
        case PropertyID::Order:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Order);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Order)] = initial_value;
            return initial_value;
        }
        case PropertyID::Orphans:
        {
            auto parsed_value = parse_css_value(parsing_params, "2"sv, PropertyID::Orphans);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Orphans)] = initial_value;
            return initial_value;
        }
        case PropertyID::Outline:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium currentColor none"sv, PropertyID::Outline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Outline)] = initial_value;
            return initial_value;
        }
        case PropertyID::OutlineColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentColor"sv, PropertyID::OutlineColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OutlineColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::OutlineOffset:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::OutlineOffset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OutlineOffset)] = initial_value;
            return initial_value;
        }
        case PropertyID::OutlineStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::OutlineStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OutlineStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::OutlineWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "medium"sv, PropertyID::OutlineWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OutlineWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::Overflow:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::Overflow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Overflow)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::OverflowBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMargin:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMargin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMargin)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginBlockEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginBlockEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginBlockEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginBlockStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginBlockStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginBlockStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginBottom:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginBottom);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginBottom)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginInlineEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginInlineEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginInlineEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginInlineStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginInlineStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginInlineStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginLeft:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginLeft);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginLeft)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginRight:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginRight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginRight)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowClipMarginTop:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::OverflowClipMarginTop);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowClipMarginTop)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::OverflowInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowWrap:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::OverflowWrap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowWrap)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowX:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::OverflowX);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowX)] = initial_value;
            return initial_value;
        }
        case PropertyID::OverflowY:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::OverflowY);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::OverflowY)] = initial_value;
            return initial_value;
        }
        case PropertyID::Padding:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Padding);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Padding)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingBlock:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingBlock);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingBlock)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingBlockEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingBlockEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingBlockEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingBlockStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingBlockStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingBlockStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingBottom:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingBottom);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingBottom)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingInline:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingInline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingInline)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingInlineEnd:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingInlineEnd);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingInlineEnd)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingInlineStart:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingInlineStart);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingInlineStart)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingLeft:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingLeft);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingLeft)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingRight:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingRight);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingRight)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaddingTop:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::PaddingTop);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaddingTop)] = initial_value;
            return initial_value;
        }
        case PropertyID::PaintOrder:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::PaintOrder);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PaintOrder)] = initial_value;
            return initial_value;
        }
        case PropertyID::Perspective:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Perspective);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Perspective)] = initial_value;
            return initial_value;
        }
        case PropertyID::PerspectiveOrigin:
        {
            auto parsed_value = parse_css_value(parsing_params, "50% 50%"sv, PropertyID::PerspectiveOrigin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PerspectiveOrigin)] = initial_value;
            return initial_value;
        }
        case PropertyID::PlaceContent:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::PlaceContent);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PlaceContent)] = initial_value;
            return initial_value;
        }
        case PropertyID::PlaceItems:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::PlaceItems);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PlaceItems)] = initial_value;
            return initial_value;
        }
        case PropertyID::PlaceSelf:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::PlaceSelf);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PlaceSelf)] = initial_value;
            return initial_value;
        }
        case PropertyID::PointerEvents:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::PointerEvents);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PointerEvents)] = initial_value;
            return initial_value;
        }
        case PropertyID::Position:
        {
            auto parsed_value = parse_css_value(parsing_params, "static"sv, PropertyID::Position);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Position)] = initial_value;
            return initial_value;
        }
        case PropertyID::PositionAnchor:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::PositionAnchor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PositionAnchor)] = initial_value;
            return initial_value;
        }
        case PropertyID::PositionArea:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::PositionArea);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PositionArea)] = initial_value;
            return initial_value;
        }
        case PropertyID::PositionTryFallbacks:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::PositionTryFallbacks);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PositionTryFallbacks)] = initial_value;
            return initial_value;
        }
        case PropertyID::PositionTryOrder:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::PositionTryOrder);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PositionTryOrder)] = initial_value;
            return initial_value;
        }
        case PropertyID::PositionVisibility:
        {
            auto parsed_value = parse_css_value(parsing_params, "anchors-visible"sv, PropertyID::PositionVisibility);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::PositionVisibility)] = initial_value;
            return initial_value;
        }
        case PropertyID::Quotes:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Quotes);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Quotes)] = initial_value;
            return initial_value;
        }
        case PropertyID::R:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::R);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::R)] = initial_value;
            return initial_value;
        }
        case PropertyID::Resize:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Resize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Resize)] = initial_value;
            return initial_value;
        }
        case PropertyID::Right:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Right);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Right)] = initial_value;
            return initial_value;
        }
        case PropertyID::Rotate:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Rotate);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Rotate)] = initial_value;
            return initial_value;
        }
        case PropertyID::RowGap:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::RowGap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::RowGap)] = initial_value;
            return initial_value;
        }
        case PropertyID::Rx:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Rx);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Rx)] = initial_value;
            return initial_value;
        }
        case PropertyID::Ry:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Ry);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Ry)] = initial_value;
            return initial_value;
        }
        case PropertyID::Scale:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Scale);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Scale)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollTimeline:
        {
            auto parsed_value = parse_css_value(parsing_params, "none block"sv, PropertyID::ScrollTimeline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollTimeline)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollTimelineAxis:
        {
            auto parsed_value = parse_css_value(parsing_params, "block"sv, PropertyID::ScrollTimelineAxis);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollTimelineAxis)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollTimelineName:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ScrollTimelineName);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollTimelineName)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollbarColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ScrollbarColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollbarColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollbarGutter:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ScrollbarGutter);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollbarGutter)] = initial_value;
            return initial_value;
        }
        case PropertyID::ScrollbarWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ScrollbarWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ScrollbarWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::ShapeImageThreshold:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::ShapeImageThreshold);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ShapeImageThreshold)] = initial_value;
            return initial_value;
        }
        case PropertyID::ShapeMargin:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::ShapeMargin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ShapeMargin)] = initial_value;
            return initial_value;
        }
        case PropertyID::ShapeOutside:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ShapeOutside);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ShapeOutside)] = initial_value;
            return initial_value;
        }
        case PropertyID::ShapeRendering:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ShapeRendering);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ShapeRendering)] = initial_value;
            return initial_value;
        }
        case PropertyID::StopColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "black"sv, PropertyID::StopColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StopColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::StopOpacity:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::StopOpacity);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StopOpacity)] = initial_value;
            return initial_value;
        }
        case PropertyID::Stroke:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Stroke);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Stroke)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeDasharray:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::StrokeDasharray);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeDasharray)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeDashoffset:
        {
            auto parsed_value = parse_css_value(parsing_params, "0px"sv, PropertyID::StrokeDashoffset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeDashoffset)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeLinecap:
        {
            auto parsed_value = parse_css_value(parsing_params, "butt"sv, PropertyID::StrokeLinecap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeLinecap)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeLinejoin:
        {
            auto parsed_value = parse_css_value(parsing_params, "miter"sv, PropertyID::StrokeLinejoin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeLinejoin)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeMiterlimit:
        {
            auto parsed_value = parse_css_value(parsing_params, "4"sv, PropertyID::StrokeMiterlimit);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeMiterlimit)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeOpacity:
        {
            auto parsed_value = parse_css_value(parsing_params, "1"sv, PropertyID::StrokeOpacity);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeOpacity)] = initial_value;
            return initial_value;
        }
        case PropertyID::StrokeWidth:
        {
            auto parsed_value = parse_css_value(parsing_params, "1px"sv, PropertyID::StrokeWidth);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::StrokeWidth)] = initial_value;
            return initial_value;
        }
        case PropertyID::TabSize:
        {
            auto parsed_value = parse_css_value(parsing_params, "8"sv, PropertyID::TabSize);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TabSize)] = initial_value;
            return initial_value;
        }
        case PropertyID::TableLayout:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TableLayout);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TableLayout)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextAlign:
        {
            auto parsed_value = parse_css_value(parsing_params, "start"sv, PropertyID::TextAlign);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextAlign)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextAnchor:
        {
            auto parsed_value = parse_css_value(parsing_params, "start"sv, PropertyID::TextAnchor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextAnchor)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextDecoration:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::TextDecoration);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextDecoration)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextDecorationColor:
        {
            auto parsed_value = parse_css_value(parsing_params, "currentcolor"sv, PropertyID::TextDecorationColor);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextDecorationColor)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextDecorationLine:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::TextDecorationLine);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextDecorationLine)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextDecorationStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "solid"sv, PropertyID::TextDecorationStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextDecorationStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextDecorationThickness:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextDecorationThickness);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextDecorationThickness)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextIndent:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::TextIndent);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextIndent)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextJustify:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextJustify);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextJustify)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextOverflow:
        {
            auto parsed_value = parse_css_value(parsing_params, "clip"sv, PropertyID::TextOverflow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextOverflow)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextRendering:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextRendering);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextRendering)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextShadow:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::TextShadow);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextShadow)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextTransform:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::TextTransform);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextTransform)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextUnderlineOffset:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextUnderlineOffset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextUnderlineOffset)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextUnderlinePosition:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextUnderlinePosition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextUnderlinePosition)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextWrap:
        {
            auto parsed_value = parse_css_value(parsing_params, "wrap"sv, PropertyID::TextWrap);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextWrap)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextWrapMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "wrap"sv, PropertyID::TextWrapMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextWrapMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::TextWrapStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TextWrapStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TextWrapStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::TimelineScope:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::TimelineScope);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TimelineScope)] = initial_value;
            return initial_value;
        }
        case PropertyID::Top:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Top);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Top)] = initial_value;
            return initial_value;
        }
        case PropertyID::TouchAction:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::TouchAction);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TouchAction)] = initial_value;
            return initial_value;
        }
        case PropertyID::Transform:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Transform);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Transform)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransformBox:
        {
            auto parsed_value = parse_css_value(parsing_params, "view-box"sv, PropertyID::TransformBox);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransformBox)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransformOrigin:
        {
            auto parsed_value = parse_css_value(parsing_params, "50% 50%"sv, PropertyID::TransformOrigin);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransformOrigin)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransformStyle:
        {
            auto parsed_value = parse_css_value(parsing_params, "flat"sv, PropertyID::TransformStyle);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransformStyle)] = initial_value;
            return initial_value;
        }
        case PropertyID::Transition:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Transition);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Transition)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransitionBehavior:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::TransitionBehavior);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransitionBehavior)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransitionDelay:
        {
            auto parsed_value = parse_css_value(parsing_params, "0s"sv, PropertyID::TransitionDelay);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransitionDelay)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransitionDuration:
        {
            auto parsed_value = parse_css_value(parsing_params, "0s"sv, PropertyID::TransitionDuration);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransitionDuration)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransitionProperty:
        {
            auto parsed_value = parse_css_value(parsing_params, "all"sv, PropertyID::TransitionProperty);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransitionProperty)] = initial_value;
            return initial_value;
        }
        case PropertyID::TransitionTimingFunction:
        {
            auto parsed_value = parse_css_value(parsing_params, "ease"sv, PropertyID::TransitionTimingFunction);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::TransitionTimingFunction)] = initial_value;
            return initial_value;
        }
        case PropertyID::Translate:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::Translate);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Translate)] = initial_value;
            return initial_value;
        }
        case PropertyID::UnicodeBidi:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::UnicodeBidi);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::UnicodeBidi)] = initial_value;
            return initial_value;
        }
        case PropertyID::UserSelect:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::UserSelect);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::UserSelect)] = initial_value;
            return initial_value;
        }
        case PropertyID::VerticalAlign:
        {
            auto parsed_value = parse_css_value(parsing_params, "baseline"sv, PropertyID::VerticalAlign);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::VerticalAlign)] = initial_value;
            return initial_value;
        }
        case PropertyID::ViewTimeline:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ViewTimeline);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ViewTimeline)] = initial_value;
            return initial_value;
        }
        case PropertyID::ViewTimelineAxis:
        {
            auto parsed_value = parse_css_value(parsing_params, "block"sv, PropertyID::ViewTimelineAxis);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ViewTimelineAxis)] = initial_value;
            return initial_value;
        }
        case PropertyID::ViewTimelineInset:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ViewTimelineInset);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ViewTimelineInset)] = initial_value;
            return initial_value;
        }
        case PropertyID::ViewTimelineName:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ViewTimelineName);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ViewTimelineName)] = initial_value;
            return initial_value;
        }
        case PropertyID::ViewTransitionName:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::ViewTransitionName);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ViewTransitionName)] = initial_value;
            return initial_value;
        }
        case PropertyID::Visibility:
        {
            auto parsed_value = parse_css_value(parsing_params, "visible"sv, PropertyID::Visibility);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Visibility)] = initial_value;
            return initial_value;
        }
        case PropertyID::WhiteSpace:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::WhiteSpace);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WhiteSpace)] = initial_value;
            return initial_value;
        }
        case PropertyID::WhiteSpaceCollapse:
        {
            auto parsed_value = parse_css_value(parsing_params, "collapse"sv, PropertyID::WhiteSpaceCollapse);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WhiteSpaceCollapse)] = initial_value;
            return initial_value;
        }
        case PropertyID::WhiteSpaceTrim:
        {
            auto parsed_value = parse_css_value(parsing_params, "none"sv, PropertyID::WhiteSpaceTrim);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WhiteSpaceTrim)] = initial_value;
            return initial_value;
        }
        case PropertyID::Widows:
        {
            auto parsed_value = parse_css_value(parsing_params, "2"sv, PropertyID::Widows);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Widows)] = initial_value;
            return initial_value;
        }
        case PropertyID::Width:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::Width);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Width)] = initial_value;
            return initial_value;
        }
        case PropertyID::WillChange:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::WillChange);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WillChange)] = initial_value;
            return initial_value;
        }
        case PropertyID::WordBreak:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::WordBreak);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WordBreak)] = initial_value;
            return initial_value;
        }
        case PropertyID::WordSpacing:
        {
            auto parsed_value = parse_css_value(parsing_params, "normal"sv, PropertyID::WordSpacing);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WordSpacing)] = initial_value;
            return initial_value;
        }
        case PropertyID::WritingMode:
        {
            auto parsed_value = parse_css_value(parsing_params, "horizontal-tb"sv, PropertyID::WritingMode);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::WritingMode)] = initial_value;
            return initial_value;
        }
        case PropertyID::X:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::X);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::X)] = initial_value;
            return initial_value;
        }
        case PropertyID::Y:
        {
            auto parsed_value = parse_css_value(parsing_params, "0"sv, PropertyID::Y);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::Y)] = initial_value;
            return initial_value;
        }
        case PropertyID::ZIndex:
        {
            auto parsed_value = parse_css_value(parsing_params, "auto"sv, PropertyID::ZIndex);
            VERIFY(!parsed_value.is_null());
            auto initial_value = parsed_value.release_nonnull();
            initial_values[to_underlying(PropertyID::ZIndex)] = initial_value;
            return initial_value;
        }
        default: VERIFY_NOT_REACHED();
    }
    VERIFY_NOT_REACHED();
}

PropertyMultiplicity property_multiplicity(PropertyID property_id)
{
    switch (property_id) {
    case PropertyID::Animation:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationComposition:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationDelay:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationDirection:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationDuration:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationFillMode:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationIterationCount:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationName:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationPlayState:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationTimeline:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::AnimationTimingFunction:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackdropFilter:
        return PropertyMultiplicity::List;
    case PropertyID::Background:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundAttachment:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundBlendMode:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundClip:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundImage:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundOrigin:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundPosition:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundPositionX:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundPositionY:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundRepeat:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BackgroundSize:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::BoxShadow:
        return PropertyMultiplicity::List;
    case PropertyID::CounterIncrement:
        return PropertyMultiplicity::List;
    case PropertyID::CounterReset:
        return PropertyMultiplicity::List;
    case PropertyID::CounterSet:
        return PropertyMultiplicity::List;
    case PropertyID::Filter:
        return PropertyMultiplicity::List;
    case PropertyID::FontFamily:
        return PropertyMultiplicity::List;
    case PropertyID::FontFeatureSettings:
        return PropertyMultiplicity::List;
    case PropertyID::FontVariationSettings:
        return PropertyMultiplicity::List;
    case PropertyID::Mask:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskClip:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskComposite:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskImage:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskMode:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskOrigin:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskPosition:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskRepeat:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::MaskSize:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::Quotes:
        return PropertyMultiplicity::List;
    case PropertyID::ScrollTimeline:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ScrollTimelineAxis:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ScrollTimelineName:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::Transition:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::TransitionBehavior:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::TransitionDelay:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::TransitionDuration:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::TransitionProperty:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::TransitionTimingFunction:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ViewTimeline:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ViewTimelineAxis:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ViewTimelineInset:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::ViewTimelineName:
        return PropertyMultiplicity::CoordinatingList;
    case PropertyID::WillChange:
        return PropertyMultiplicity::List;

    default:
        return PropertyMultiplicity::Single;
    }

    VERIFY_NOT_REACHED();
}

bool property_is_single_valued(PropertyID property_id)
{
    return !property_is_list_valued(property_id);
}

bool property_is_list_valued(PropertyID property_id)
{
    switch (property_id) {
    case PropertyID::Animation:
    case PropertyID::AnimationComposition:
    case PropertyID::AnimationDelay:
    case PropertyID::AnimationDirection:
    case PropertyID::AnimationDuration:
    case PropertyID::AnimationFillMode:
    case PropertyID::AnimationIterationCount:
    case PropertyID::AnimationName:
    case PropertyID::AnimationPlayState:
    case PropertyID::AnimationTimeline:
    case PropertyID::AnimationTimingFunction:
    case PropertyID::BackdropFilter:
    case PropertyID::Background:
    case PropertyID::BackgroundAttachment:
    case PropertyID::BackgroundBlendMode:
    case PropertyID::BackgroundClip:
    case PropertyID::BackgroundImage:
    case PropertyID::BackgroundOrigin:
    case PropertyID::BackgroundPosition:
    case PropertyID::BackgroundPositionX:
    case PropertyID::BackgroundPositionY:
    case PropertyID::BackgroundRepeat:
    case PropertyID::BackgroundSize:
    case PropertyID::BoxShadow:
    case PropertyID::CounterIncrement:
    case PropertyID::CounterReset:
    case PropertyID::CounterSet:
    case PropertyID::Filter:
    case PropertyID::FontFamily:
    case PropertyID::FontFeatureSettings:
    case PropertyID::FontVariationSettings:
    case PropertyID::Mask:
    case PropertyID::MaskClip:
    case PropertyID::MaskComposite:
    case PropertyID::MaskImage:
    case PropertyID::MaskMode:
    case PropertyID::MaskOrigin:
    case PropertyID::MaskPosition:
    case PropertyID::MaskRepeat:
    case PropertyID::MaskSize:
    case PropertyID::Quotes:
    case PropertyID::ScrollTimeline:
    case PropertyID::ScrollTimelineAxis:
    case PropertyID::ScrollTimelineName:
    case PropertyID::Transition:
    case PropertyID::TransitionBehavior:
    case PropertyID::TransitionDelay:
    case PropertyID::TransitionDuration:
    case PropertyID::TransitionProperty:
    case PropertyID::TransitionTimingFunction:
    case PropertyID::ViewTimeline:
    case PropertyID::ViewTimelineAxis:
    case PropertyID::ViewTimelineInset:
    case PropertyID::ViewTimelineName:
    case PropertyID::WillChange:

        return true;
    default:
        return false;
    }
}

bool property_has_quirk(PropertyID property_id, Quirk quirk)
{
    switch (property_id) {

    case PropertyID::BackgroundColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundPosition: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderLeftColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderLeftWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderRightColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderRightWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderSpacing: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderTopColor: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderTopWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Bottom: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Clip: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Color: {
        switch (quirk) {

        case Quirk::HashlessHexColor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Cx: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Cy: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontSize: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Height: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Left: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::LetterSpacing: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Margin: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginBottom: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginLeft: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginRight: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginTop: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaxHeight: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaxWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinHeight: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinWidth: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Padding: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingBottom: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingLeft: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingRight: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingTop: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::R: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Right: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Rx: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Ry: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TextIndent: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Top: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::VerticalAlign: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Width: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::WordSpacing: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::X: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Y: {
        switch (quirk) {

        case Quirk::UnitlessLength:
            return true;

        default:
            return false;
        }
    }

    default:
        return false;
    }
}

bool property_accepts_type(PropertyID property_id, ValueType value_type)
{
    switch (property_id) {

    case PropertyID::WebkitTextFillColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AccentColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AlignContent: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::AlignItems: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::AlignSelf: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::AnchorName: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnchorScope: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationComposition: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::AnimationDelay: {
        switch (value_type) {
        case ValueType::Time:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationDuration: {
        switch (value_type) {
        case ValueType::Time:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationIterationCount: {
        switch (value_type) {
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationName: {
        switch (value_type) {
        case ValueType::String:
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationTimeline: {
        switch (value_type) {
        case ValueType::DashedIdent:
        case ValueType::ScrollFunction:
        case ValueType::ViewFunction:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::AnimationTimingFunction: {
        switch (value_type) {
        case ValueType::EasingFunction:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Appearance: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::AspectRatio: {
        switch (value_type) {
        case ValueType::Ratio:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundAttachment: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BackgroundBlendMode: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BackgroundClip: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BackgroundColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundImage: {
        switch (value_type) {
        case ValueType::Image:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundOrigin: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BackgroundPosition: {
        switch (value_type) {
        case ValueType::BackgroundPosition:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundPositionX: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundPositionY: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BackgroundRepeat: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BackgroundSize: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BlockSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockEndColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockEndStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockEndWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockStartColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockStartStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockStartWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderBlockWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomLeftRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomRightRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderBottomWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderCollapse: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderEndEndRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderEndStartRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderImageOutset: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderImageRepeat: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderImageSlice: {
        switch (value_type) {
        case ValueType::Number:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderImageSource: {
        switch (value_type) {
        case ValueType::Image:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderImageWidth: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineEndColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineEndStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineEndWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineStartColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineStartStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineStartWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderInlineWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderLeftColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderLeftStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderLeftWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderRightColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderRightStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderRightWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderSpacing: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderStartEndRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderStartStartRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderTopColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderTopLeftRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderTopRightRadius: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderTopStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::BorderTopWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BorderWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Bottom: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::BoxSizing: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::CaptionSide: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::CaretColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Clear: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Clip: {
        switch (value_type) {
        case ValueType::Rect:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ClipPath: {
        switch (value_type) {
        case ValueType::BasicShape:
        case ValueType::Url:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ClipRule: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Color: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ColorInterpolation: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ColorScheme: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ColumnCount: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ColumnGap: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ColumnHeight: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ColumnSpan: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ColumnWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Contain: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Content: {
        switch (value_type) {
        case ValueType::Counter:
        case ValueType::Image:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ContentVisibility: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::CornerBlockEndShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerBlockStartShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerBottomLeftShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerBottomRightShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerBottomShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerEndEndShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerEndStartShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerInlineEndShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerInlineStartShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerLeftShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerRightShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerStartEndShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerStartStartShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerTopLeftShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerTopRightShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CornerTopShape: {
        switch (value_type) {
        case ValueType::CornerShape:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CounterIncrement: {
        switch (value_type) {
        case ValueType::CustomIdent:
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CounterReset: {
        switch (value_type) {
        case ValueType::CustomIdent:
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::CounterSet: {
        switch (value_type) {
        case ValueType::CustomIdent:
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Cursor: {
        switch (value_type) {
        case ValueType::Url:
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Cx: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Cy: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Direction: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Display: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::DominantBaseline: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::EmptyCells: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Fill: {
        switch (value_type) {
        case ValueType::Paint:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FillOpacity: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FillRule: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FlexBasis: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FlexDirection: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FlexGrow: {
        switch (value_type) {
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FlexShrink: {
        switch (value_type) {
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FlexWrap: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Float: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FloodColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FloodOpacity: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontFamily: {
        switch (value_type) {
        case ValueType::CustomIdent:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontFeatureSettings: {
        switch (value_type) {
        case ValueType::Integer:
        case ValueType::OpentypeTag:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontKerning: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FontLanguageOverride: {
        switch (value_type) {
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontOpticalSizing: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FontSize: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontStyle: {
        switch (value_type) {
        case ValueType::FontStyle:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariant: {
        switch (value_type) {
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariantAlternates: {
        switch (value_type) {
        case ValueType::FontVariantAlternates:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariantCaps: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FontVariantEastAsian: {
        switch (value_type) {
        case ValueType::FontVariantEastAsian:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariantEmoji: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FontVariantLigatures: {
        switch (value_type) {
        case ValueType::FontVariantLigatures:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariantNumeric: {
        switch (value_type) {
        case ValueType::FontVariantNumeric:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontVariantPosition: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::FontVariationSettings: {
        switch (value_type) {
        case ValueType::Number:
        case ValueType::OpentypeTag:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontWeight: {
        switch (value_type) {
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::FontWidth: {
        switch (value_type) {
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Gap: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Grid: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridArea: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridAutoColumns: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridAutoRows: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridColumn: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridColumnEnd: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridColumnStart: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridRow: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridRowEnd: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridRowStart: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridTemplate: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridTemplateAreas: {
        switch (value_type) {
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridTemplateColumns: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::GridTemplateRows: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Height: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ImageRendering: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::InlineSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Inset: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetBlock: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetBlockEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetBlockStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetInline: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetInlineEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::InsetInlineStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Isolation: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::JustifyContent: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::JustifyItems: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::JustifySelf: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Left: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::LetterSpacing: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::LineHeight: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Number:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ListStyleImage: {
        switch (value_type) {
        case ValueType::Image:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ListStylePosition: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ListStyleType: {
        switch (value_type) {
        case ValueType::CounterStyle:
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Margin: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginBlock: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginBlockEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginBlockStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginBottom: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginInline: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginInlineEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginInlineStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginLeft: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginRight: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MarginTop: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaskClip: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaskComposite: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaskImage: {
        switch (value_type) {
        case ValueType::Image:
        case ValueType::Url:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaskMode: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaskOrigin: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaskPosition: {
        switch (value_type) {
        case ValueType::Position:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaskRepeat: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaskSize: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaskType: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MathDepth: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MathShift: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MathStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::MaxBlockSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaxHeight: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaxInlineSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MaxWidth: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinBlockSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinHeight: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinInlineSize: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MinWidth: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::MixBlendMode: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ObjectFit: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ObjectPosition: {
        switch (value_type) {
        case ValueType::Position:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Opacity: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Order: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Orphans: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OutlineColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OutlineOffset: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OutlineStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::OutlineWidth: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Overflow: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::OverflowBlock: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMargin: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginBlock: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginBlockEnd: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginBlockStart: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginBottom: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginInline: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginInlineEnd: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginInlineStart: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginLeft: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginRight: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowClipMarginTop: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::OverflowInline: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::OverflowX: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::OverflowY: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Padding: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingBlock: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingBlockEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingBlockStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingBottom: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingInline: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingInlineEnd: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingInlineStart: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingLeft: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingRight: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaddingTop: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PaintOrder: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Perspective: {
        switch (value_type) {
        case ValueType::Length:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PerspectiveOrigin: {
        switch (value_type) {
        case ValueType::Position:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PointerEvents: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Position: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::PositionAnchor: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PositionArea: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::PositionTryFallbacks: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::PositionTryOrder: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Quotes: {
        switch (value_type) {
        case ValueType::String:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::R: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Resize: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Right: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::RowGap: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Rx: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Ry: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ScrollTimelineAxis: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ScrollTimelineName: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ScrollbarWidth: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ShapeImageThreshold: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ShapeMargin: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ShapeRendering: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::StopColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::StopOpacity: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Stroke: {
        switch (value_type) {
        case ValueType::Paint:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::StrokeDashoffset: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Number:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::StrokeLinecap: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::StrokeLinejoin: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::StrokeMiterlimit: {
        switch (value_type) {
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::StrokeOpacity: {
        switch (value_type) {
        case ValueType::Opacity:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::StrokeWidth: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Number:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TabSize: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Number:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TableLayout: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextAlign: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextAnchor: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextDecorationColor: {
        switch (value_type) {
        case ValueType::Color:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TextDecorationLine: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextDecorationStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextDecorationThickness: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TextIndent: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TextJustify: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextOverflow: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextRendering: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextTransform: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextUnderlineOffset: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TextUnderlinePosition: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextWrapMode: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TextWrapStyle: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Top: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
        case ValueType::Anchor:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TouchAction: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Transform: {
        switch (value_type) {
        case ValueType::TransformList:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TransformBox: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TransformOrigin: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TransitionBehavior: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::TransitionDelay: {
        switch (value_type) {
        case ValueType::Time:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TransitionDuration: {
        switch (value_type) {
        case ValueType::Time:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TransitionProperty: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::TransitionTimingFunction: {
        switch (value_type) {
        case ValueType::EasingFunction:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Translate: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::UnicodeBidi: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::UserSelect: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::VerticalAlign: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ViewTimelineAxis: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::ViewTimelineInset: {
        switch (value_type) {
        case ValueType::ViewTimelineInset:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ViewTimelineName: {
        switch (value_type) {
        case ValueType::DashedIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ViewTransitionName: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Visibility: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::WhiteSpace: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::WhiteSpaceCollapse: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::Widows: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Width: {
        switch (value_type) {
        case ValueType::FitContent:
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::WillChange: {
        switch (value_type) {
        case ValueType::CustomIdent:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::WordSpacing: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::WritingMode: {
        switch (value_type) {

        default:
            return false;
        }
    }

    case PropertyID::X: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::Y: {
        switch (value_type) {
        case ValueType::Length:
        case ValueType::Percentage:
            return true;

        default:
            return false;
        }
    }

    case PropertyID::ZIndex: {
        switch (value_type) {
        case ValueType::Integer:
            return true;

        default:
            return false;
        }
    }

    default:
        return false;
    }
}

AcceptedTypeRangeMap property_accepted_type_ranges(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::WebkitTextFillColor: {
        return {  };
    }
    case PropertyID::AccentColor: {
        return {  };
    }
    case PropertyID::AlignContent: {
        return {  };
    }
    case PropertyID::AlignItems: {
        return {  };
    }
    case PropertyID::AlignSelf: {
        return {  };
    }
    case PropertyID::AnchorName: {
        return {  };
    }
    case PropertyID::AnchorScope: {
        return {  };
    }
    case PropertyID::AnimationComposition: {
        return {  };
    }
    case PropertyID::AnimationDelay: {
        return { { ValueType::Time, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::AnimationDuration: {
        return { { ValueType::Time, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::AnimationIterationCount: {
        return { { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::AnimationName: {
        return {  };
    }
    case PropertyID::AnimationTimeline: {
        return {  };
    }
    case PropertyID::AnimationTimingFunction: {
        return {  };
    }
    case PropertyID::Appearance: {
        return {  };
    }
    case PropertyID::AspectRatio: {
        return {  };
    }
    case PropertyID::BackgroundAttachment: {
        return {  };
    }
    case PropertyID::BackgroundBlendMode: {
        return {  };
    }
    case PropertyID::BackgroundClip: {
        return {  };
    }
    case PropertyID::BackgroundColor: {
        return {  };
    }
    case PropertyID::BackgroundImage: {
        return {  };
    }
    case PropertyID::BackgroundOrigin: {
        return {  };
    }
    case PropertyID::BackgroundPosition: {
        return {  };
    }
    case PropertyID::BackgroundPositionX: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BackgroundPositionY: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BackgroundRepeat: {
        return {  };
    }
    case PropertyID::BackgroundSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BlockSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBlockColor: {
        return {  };
    }
    case PropertyID::BorderBlockEndColor: {
        return {  };
    }
    case PropertyID::BorderBlockEndStyle: {
        return {  };
    }
    case PropertyID::BorderBlockEndWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBlockStartColor: {
        return {  };
    }
    case PropertyID::BorderBlockStartStyle: {
        return {  };
    }
    case PropertyID::BorderBlockStartWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBlockStyle: {
        return {  };
    }
    case PropertyID::BorderBlockWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBottomColor: {
        return {  };
    }
    case PropertyID::BorderBottomLeftRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBottomRightRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderBottomStyle: {
        return {  };
    }
    case PropertyID::BorderBottomWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderCollapse: {
        return {  };
    }
    case PropertyID::BorderColor: {
        return {  };
    }
    case PropertyID::BorderEndEndRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderEndStartRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderImageOutset: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderImageRepeat: {
        return {  };
    }
    case PropertyID::BorderImageSlice: {
        return { { ValueType::Number, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderImageSource: {
        return {  };
    }
    case PropertyID::BorderImageWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderInlineColor: {
        return {  };
    }
    case PropertyID::BorderInlineEndColor: {
        return {  };
    }
    case PropertyID::BorderInlineEndStyle: {
        return {  };
    }
    case PropertyID::BorderInlineEndWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderInlineStartColor: {
        return {  };
    }
    case PropertyID::BorderInlineStartStyle: {
        return {  };
    }
    case PropertyID::BorderInlineStartWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderInlineStyle: {
        return {  };
    }
    case PropertyID::BorderInlineWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderLeftColor: {
        return {  };
    }
    case PropertyID::BorderLeftStyle: {
        return {  };
    }
    case PropertyID::BorderLeftWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderRightColor: {
        return {  };
    }
    case PropertyID::BorderRightStyle: {
        return {  };
    }
    case PropertyID::BorderRightWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderSpacing: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderStartEndRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderStartStartRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderStyle: {
        return {  };
    }
    case PropertyID::BorderTopColor: {
        return {  };
    }
    case PropertyID::BorderTopLeftRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderTopRightRadius: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderTopStyle: {
        return {  };
    }
    case PropertyID::BorderTopWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BorderWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Bottom: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::BoxSizing: {
        return {  };
    }
    case PropertyID::CaptionSide: {
        return {  };
    }
    case PropertyID::CaretColor: {
        return {  };
    }
    case PropertyID::Clear: {
        return {  };
    }
    case PropertyID::Clip: {
        return {  };
    }
    case PropertyID::ClipPath: {
        return {  };
    }
    case PropertyID::ClipRule: {
        return {  };
    }
    case PropertyID::Color: {
        return {  };
    }
    case PropertyID::ColorInterpolation: {
        return {  };
    }
    case PropertyID::ColorScheme: {
        return {  };
    }
    case PropertyID::ColumnCount: {
        return { { ValueType::Integer, { 1, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ColumnGap: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ColumnHeight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ColumnSpan: {
        return {  };
    }
    case PropertyID::ColumnWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Contain: {
        return {  };
    }
    case PropertyID::Content: {
        return {  };
    }
    case PropertyID::ContentVisibility: {
        return {  };
    }
    case PropertyID::CornerBlockEndShape: {
        return {  };
    }
    case PropertyID::CornerBlockStartShape: {
        return {  };
    }
    case PropertyID::CornerBottomLeftShape: {
        return {  };
    }
    case PropertyID::CornerBottomRightShape: {
        return {  };
    }
    case PropertyID::CornerBottomShape: {
        return {  };
    }
    case PropertyID::CornerEndEndShape: {
        return {  };
    }
    case PropertyID::CornerEndStartShape: {
        return {  };
    }
    case PropertyID::CornerInlineEndShape: {
        return {  };
    }
    case PropertyID::CornerInlineStartShape: {
        return {  };
    }
    case PropertyID::CornerLeftShape: {
        return {  };
    }
    case PropertyID::CornerRightShape: {
        return {  };
    }
    case PropertyID::CornerShape: {
        return {  };
    }
    case PropertyID::CornerStartEndShape: {
        return {  };
    }
    case PropertyID::CornerStartStartShape: {
        return {  };
    }
    case PropertyID::CornerTopLeftShape: {
        return {  };
    }
    case PropertyID::CornerTopRightShape: {
        return {  };
    }
    case PropertyID::CornerTopShape: {
        return {  };
    }
    case PropertyID::CounterIncrement: {
        return { { ValueType::Integer, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::CounterReset: {
        return { { ValueType::Integer, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::CounterSet: {
        return { { ValueType::Integer, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Cursor: {
        return { { ValueType::Number, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Cx: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Cy: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Direction: {
        return {  };
    }
    case PropertyID::Display: {
        return {  };
    }
    case PropertyID::DominantBaseline: {
        return {  };
    }
    case PropertyID::EmptyCells: {
        return {  };
    }
    case PropertyID::Fill: {
        return {  };
    }
    case PropertyID::FillOpacity: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::FillRule: {
        return {  };
    }
    case PropertyID::FlexBasis: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::FlexDirection: {
        return {  };
    }
    case PropertyID::FlexGrow: {
        return { { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::FlexShrink: {
        return { { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::FlexWrap: {
        return {  };
    }
    case PropertyID::Float: {
        return {  };
    }
    case PropertyID::FloodColor: {
        return {  };
    }
    case PropertyID::FloodOpacity: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::FontFamily: {
        return {  };
    }
    case PropertyID::FontFeatureSettings: {
        return { { ValueType::Integer, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::FontKerning: {
        return {  };
    }
    case PropertyID::FontLanguageOverride: {
        return {  };
    }
    case PropertyID::FontOpticalSizing: {
        return {  };
    }
    case PropertyID::FontSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::FontStyle: {
        return {  };
    }
    case PropertyID::FontVariant: {
        return {  };
    }
    case PropertyID::FontVariantAlternates: {
        return {  };
    }
    case PropertyID::FontVariantCaps: {
        return {  };
    }
    case PropertyID::FontVariantEastAsian: {
        return {  };
    }
    case PropertyID::FontVariantEmoji: {
        return {  };
    }
    case PropertyID::FontVariantLigatures: {
        return {  };
    }
    case PropertyID::FontVariantNumeric: {
        return {  };
    }
    case PropertyID::FontVariantPosition: {
        return {  };
    }
    case PropertyID::FontVariationSettings: {
        return {  };
    }
    case PropertyID::FontWeight: {
        return { { ValueType::Number, { 1, 1000 } } };
    }
    case PropertyID::FontWidth: {
        return { { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Gap: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Grid: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::GridArea: {
        return {  };
    }
    case PropertyID::GridAutoColumns: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::GridAutoRows: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::GridColumn: {
        return {  };
    }
    case PropertyID::GridColumnEnd: {
        return {  };
    }
    case PropertyID::GridColumnStart: {
        return {  };
    }
    case PropertyID::GridRow: {
        return {  };
    }
    case PropertyID::GridRowEnd: {
        return {  };
    }
    case PropertyID::GridRowStart: {
        return {  };
    }
    case PropertyID::GridTemplate: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::GridTemplateAreas: {
        return {  };
    }
    case PropertyID::GridTemplateColumns: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::GridTemplateRows: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Height: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ImageRendering: {
        return {  };
    }
    case PropertyID::InlineSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Inset: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetBlock: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetBlockEnd: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetBlockStart: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetInline: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetInlineEnd: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::InsetInlineStart: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Isolation: {
        return {  };
    }
    case PropertyID::JustifyContent: {
        return {  };
    }
    case PropertyID::JustifyItems: {
        return {  };
    }
    case PropertyID::JustifySelf: {
        return {  };
    }
    case PropertyID::Left: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::LetterSpacing: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::LineHeight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ListStyleImage: {
        return {  };
    }
    case PropertyID::ListStylePosition: {
        return {  };
    }
    case PropertyID::ListStyleType: {
        return {  };
    }
    case PropertyID::Margin: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginBlock: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginBlockEnd: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginBlockStart: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginBottom: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginInline: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginInlineEnd: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginInlineStart: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginLeft: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginRight: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MarginTop: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MaskClip: {
        return {  };
    }
    case PropertyID::MaskComposite: {
        return {  };
    }
    case PropertyID::MaskImage: {
        return {  };
    }
    case PropertyID::MaskMode: {
        return {  };
    }
    case PropertyID::MaskOrigin: {
        return {  };
    }
    case PropertyID::MaskPosition: {
        return {  };
    }
    case PropertyID::MaskRepeat: {
        return {  };
    }
    case PropertyID::MaskSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MaskType: {
        return {  };
    }
    case PropertyID::MathDepth: {
        return {  };
    }
    case PropertyID::MathShift: {
        return {  };
    }
    case PropertyID::MathStyle: {
        return {  };
    }
    case PropertyID::MaxBlockSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MaxHeight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MaxInlineSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MaxWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MinBlockSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MinHeight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MinInlineSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MinWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::MixBlendMode: {
        return {  };
    }
    case PropertyID::ObjectFit: {
        return {  };
    }
    case PropertyID::ObjectPosition: {
        return {  };
    }
    case PropertyID::Opacity: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::Order: {
        return { { ValueType::Integer, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Orphans: {
        return { { ValueType::Integer, { 1, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OutlineColor: {
        return {  };
    }
    case PropertyID::OutlineOffset: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OutlineStyle: {
        return {  };
    }
    case PropertyID::OutlineWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Overflow: {
        return {  };
    }
    case PropertyID::OverflowBlock: {
        return {  };
    }
    case PropertyID::OverflowClipMargin: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginBlock: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginBlockEnd: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginBlockStart: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginBottom: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginInline: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginInlineEnd: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginInlineStart: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginLeft: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginRight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowClipMarginTop: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::OverflowInline: {
        return {  };
    }
    case PropertyID::OverflowX: {
        return {  };
    }
    case PropertyID::OverflowY: {
        return {  };
    }
    case PropertyID::Padding: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingBlock: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingBlockEnd: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingBlockStart: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingBottom: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingInline: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingInlineEnd: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingInlineStart: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingLeft: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingRight: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaddingTop: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PaintOrder: {
        return {  };
    }
    case PropertyID::Perspective: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::PerspectiveOrigin: {
        return {  };
    }
    case PropertyID::PointerEvents: {
        return {  };
    }
    case PropertyID::Position: {
        return {  };
    }
    case PropertyID::PositionAnchor: {
        return {  };
    }
    case PropertyID::PositionArea: {
        return {  };
    }
    case PropertyID::PositionTryFallbacks: {
        return {  };
    }
    case PropertyID::PositionTryOrder: {
        return {  };
    }
    case PropertyID::Quotes: {
        return {  };
    }
    case PropertyID::R: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Resize: {
        return {  };
    }
    case PropertyID::Right: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::RowGap: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Rx: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Ry: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ScrollTimelineAxis: {
        return {  };
    }
    case PropertyID::ScrollTimelineName: {
        return {  };
    }
    case PropertyID::ScrollbarWidth: {
        return {  };
    }
    case PropertyID::ShapeImageThreshold: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::ShapeMargin: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ShapeRendering: {
        return {  };
    }
    case PropertyID::StopColor: {
        return {  };
    }
    case PropertyID::StopOpacity: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::Stroke: {
        return {  };
    }
    case PropertyID::StrokeDashoffset: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::StrokeLinecap: {
        return {  };
    }
    case PropertyID::StrokeLinejoin: {
        return {  };
    }
    case PropertyID::StrokeMiterlimit: {
        return { { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::StrokeOpacity: {
        return { { ValueType::Number, { 0, 1 } }, { ValueType::Percentage, { 0, 100 } } };
    }
    case PropertyID::StrokeWidth: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TabSize: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Number, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TableLayout: {
        return {  };
    }
    case PropertyID::TextAlign: {
        return {  };
    }
    case PropertyID::TextAnchor: {
        return {  };
    }
    case PropertyID::TextDecorationColor: {
        return {  };
    }
    case PropertyID::TextDecorationLine: {
        return {  };
    }
    case PropertyID::TextDecorationStyle: {
        return {  };
    }
    case PropertyID::TextDecorationThickness: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TextIndent: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TextJustify: {
        return {  };
    }
    case PropertyID::TextOverflow: {
        return {  };
    }
    case PropertyID::TextRendering: {
        return {  };
    }
    case PropertyID::TextTransform: {
        return {  };
    }
    case PropertyID::TextUnderlineOffset: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TextUnderlinePosition: {
        return {  };
    }
    case PropertyID::TextWrapMode: {
        return {  };
    }
    case PropertyID::TextWrapStyle: {
        return {  };
    }
    case PropertyID::Top: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TouchAction: {
        return {  };
    }
    case PropertyID::Transform: {
        return {  };
    }
    case PropertyID::TransformBox: {
        return {  };
    }
    case PropertyID::TransformOrigin: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TransitionBehavior: {
        return {  };
    }
    case PropertyID::TransitionDelay: {
        return {  };
    }
    case PropertyID::TransitionDuration: {
        return { { ValueType::Time, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::TransitionProperty: {
        return {  };
    }
    case PropertyID::TransitionTimingFunction: {
        return {  };
    }
    case PropertyID::Translate: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::UnicodeBidi: {
        return {  };
    }
    case PropertyID::UserSelect: {
        return {  };
    }
    case PropertyID::VerticalAlign: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ViewTimelineAxis: {
        return {  };
    }
    case PropertyID::ViewTimelineInset: {
        return {  };
    }
    case PropertyID::ViewTimelineName: {
        return {  };
    }
    case PropertyID::ViewTransitionName: {
        return {  };
    }
    case PropertyID::Visibility: {
        return {  };
    }
    case PropertyID::WhiteSpace: {
        return {  };
    }
    case PropertyID::WhiteSpaceCollapse: {
        return {  };
    }
    case PropertyID::Widows: {
        return { { ValueType::Integer, { 1, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Width: {
        return { { ValueType::Length, { 0, AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { 0, AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::WillChange: {
        return {  };
    }
    case PropertyID::WordSpacing: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::WritingMode: {
        return {  };
    }
    case PropertyID::X: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::Y: {
        return { { ValueType::Length, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } }, { ValueType::Percentage, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    case PropertyID::ZIndex: {
        return { { ValueType::Integer, { AK::NumericLimits<float>::lowest(), AK::NumericLimits<float>::max() } } };
    }
    default: {
        return { };
    }
    }
}

bool property_accepts_keyword(PropertyID property_id, Keyword keyword)
{
    switch (property_id) {
    case PropertyID::WebkitTextFillColor: {

        return false;
    }
    case PropertyID::AccentColor: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AlignContent: {

        if (keyword_to_align_content(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::AlignItems: {

        if (keyword_to_align_items(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::AlignSelf: {

        if (keyword_to_align_self(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::All: {

        return false;
    }
    case PropertyID::AnchorName: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnchorScope: {
        switch (keyword) {
        case Keyword::None:
        case Keyword::All:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Animation: {

        return false;
    }
    case PropertyID::AnimationComposition: {

        if (keyword_to_animation_composition(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::AnimationDelay: {

        return false;
    }
    case PropertyID::AnimationDirection: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::Reverse:
        case Keyword::Alternate:
        case Keyword::AlternateReverse:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationDuration: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationFillMode: {
        switch (keyword) {
        case Keyword::None:
        case Keyword::Forwards:
        case Keyword::Backwards:
        case Keyword::Both:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationIterationCount: {
        switch (keyword) {
        case Keyword::Infinite:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationName: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationPlayState: {
        switch (keyword) {
        case Keyword::Running:
        case Keyword::Paused:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationTimeline: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::AnimationTimingFunction: {

        if (keyword_to_easing_keyword(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Appearance: {

        if (keyword_to_appearance(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::AspectRatio: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BackdropFilter: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Background: {

        return false;
    }
    case PropertyID::BackgroundAttachment: {

        if (keyword_to_background_attachment(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BackgroundBlendMode: {

        if (keyword_to_mix_blend_mode(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BackgroundClip: {

        if (keyword_to_background_box(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BackgroundColor: {

        return false;
    }
    case PropertyID::BackgroundImage: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BackgroundOrigin: {

        if (keyword_to_background_box(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BackgroundPosition: {

        return false;
    }
    case PropertyID::BackgroundPositionX: {
        switch (keyword) {
        case Keyword::Center:
        case Keyword::Left:
        case Keyword::Right:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BackgroundPositionY: {
        switch (keyword) {
        case Keyword::Center:
        case Keyword::Bottom:
        case Keyword::Top:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BackgroundRepeat: {
        switch (keyword) {
        case Keyword::RepeatX:
        case Keyword::RepeatY:

            return true;
        default:
            break;
        }

        if (keyword_to_repetition(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BackgroundSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::Cover:
        case Keyword::Contain:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BlockSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Border: {

        return false;
    }
    case PropertyID::BorderBlock: {

        return false;
    }
    case PropertyID::BorderBlockColor: {

        return false;
    }
    case PropertyID::BorderBlockEnd: {

        return false;
    }
    case PropertyID::BorderBlockEndColor: {

        return false;
    }
    case PropertyID::BorderBlockEndStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBlockEndWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBlockStart: {

        return false;
    }
    case PropertyID::BorderBlockStartColor: {

        return false;
    }
    case PropertyID::BorderBlockStartStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBlockStartWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBlockStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBlockWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBottom: {

        return false;
    }
    case PropertyID::BorderBottomColor: {

        return false;
    }
    case PropertyID::BorderBottomLeftRadius: {

        return false;
    }
    case PropertyID::BorderBottomRightRadius: {

        return false;
    }
    case PropertyID::BorderBottomStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderBottomWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderCollapse: {

        if (keyword_to_border_collapse(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderColor: {

        return false;
    }
    case PropertyID::BorderEndEndRadius: {

        return false;
    }
    case PropertyID::BorderEndStartRadius: {

        return false;
    }
    case PropertyID::BorderImage: {

        return false;
    }
    case PropertyID::BorderImageOutset: {

        return false;
    }
    case PropertyID::BorderImageRepeat: {

        if (keyword_to_border_image_repeat(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderImageSlice: {
        switch (keyword) {
        case Keyword::Fill:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BorderImageSource: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BorderImageWidth: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BorderInline: {

        return false;
    }
    case PropertyID::BorderInlineColor: {

        return false;
    }
    case PropertyID::BorderInlineEnd: {

        return false;
    }
    case PropertyID::BorderInlineEndColor: {

        return false;
    }
    case PropertyID::BorderInlineEndStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderInlineEndWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderInlineStart: {

        return false;
    }
    case PropertyID::BorderInlineStartColor: {

        return false;
    }
    case PropertyID::BorderInlineStartStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderInlineStartWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderInlineStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderInlineWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderLeft: {

        return false;
    }
    case PropertyID::BorderLeftColor: {

        return false;
    }
    case PropertyID::BorderLeftStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderLeftWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderRadius: {

        return false;
    }
    case PropertyID::BorderRight: {

        return false;
    }
    case PropertyID::BorderRightColor: {

        return false;
    }
    case PropertyID::BorderRightStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderRightWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderSpacing: {

        return false;
    }
    case PropertyID::BorderStartEndRadius: {

        return false;
    }
    case PropertyID::BorderStartStartRadius: {

        return false;
    }
    case PropertyID::BorderStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderTop: {

        return false;
    }
    case PropertyID::BorderTopColor: {

        return false;
    }
    case PropertyID::BorderTopLeftRadius: {

        return false;
    }
    case PropertyID::BorderTopRightRadius: {

        return false;
    }
    case PropertyID::BorderTopStyle: {

        if (keyword_to_line_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderTopWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::BorderWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Bottom: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BoxShadow: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::BoxSizing: {

        if (keyword_to_box_sizing(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::CaptionSide: {

        if (keyword_to_caption_side(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::CaretColor: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Clear: {

        if (keyword_to_clear(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Clip: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ClipPath: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ClipRule: {

        if (keyword_to_fill_rule(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Color: {

        return false;
    }
    case PropertyID::ColorInterpolation: {

        if (keyword_to_color_interpolation(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ColorScheme: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::Light:
        case Keyword::Dark:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ColumnCount: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ColumnGap: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ColumnHeight: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ColumnSpan: {

        if (keyword_to_column_span(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ColumnWidth: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Columns: {

        return false;
    }
    case PropertyID::Contain: {

        if (keyword_to_contain(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ContainerType: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::Size:
        case Keyword::InlineSize:
        case Keyword::ScrollState:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Content: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::None:
        case Keyword::OpenQuote:
        case Keyword::CloseQuote:
        case Keyword::NoOpenQuote:
        case Keyword::NoCloseQuote:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ContentVisibility: {

        if (keyword_to_content_visibility(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::CornerBlockEndShape: {

        return false;
    }
    case PropertyID::CornerBlockStartShape: {

        return false;
    }
    case PropertyID::CornerBottomLeftShape: {

        return false;
    }
    case PropertyID::CornerBottomRightShape: {

        return false;
    }
    case PropertyID::CornerBottomShape: {

        return false;
    }
    case PropertyID::CornerEndEndShape: {

        return false;
    }
    case PropertyID::CornerEndStartShape: {

        return false;
    }
    case PropertyID::CornerInlineEndShape: {

        return false;
    }
    case PropertyID::CornerInlineStartShape: {

        return false;
    }
    case PropertyID::CornerLeftShape: {

        return false;
    }
    case PropertyID::CornerRightShape: {

        return false;
    }
    case PropertyID::CornerShape: {

        return false;
    }
    case PropertyID::CornerStartEndShape: {

        return false;
    }
    case PropertyID::CornerStartStartShape: {

        return false;
    }
    case PropertyID::CornerTopLeftShape: {

        return false;
    }
    case PropertyID::CornerTopRightShape: {

        return false;
    }
    case PropertyID::CornerTopShape: {

        return false;
    }
    case PropertyID::CounterIncrement: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::CounterReset: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::CounterSet: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Cursor: {

        if (keyword_to_cursor_predefined(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Cx: {

        return false;
    }
    case PropertyID::Cy: {

        return false;
    }
    case PropertyID::Direction: {

        if (keyword_to_direction(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Display: {
        switch (keyword) {
        case Keyword::ListItem:

            return true;
        default:
            break;
        }

        if (keyword_to_display_outside(keyword).has_value())
            return true;

        if (keyword_to_display_inside(keyword).has_value())
            return true;

        if (keyword_to_display_internal(keyword).has_value())
            return true;

        if (keyword_to_display_box(keyword).has_value())
            return true;

        if (keyword_to_display_legacy(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::DominantBaseline: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        if (keyword_to_baseline_metric(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::EmptyCells: {

        if (keyword_to_empty_cells(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Fill: {

        return false;
    }
    case PropertyID::FillOpacity: {

        return false;
    }
    case PropertyID::FillRule: {

        if (keyword_to_fill_rule(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Filter: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Flex: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FlexBasis: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::Content:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FlexDirection: {

        if (keyword_to_flex_direction(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FlexFlow: {

        return false;
    }
    case PropertyID::FlexGrow: {

        return false;
    }
    case PropertyID::FlexShrink: {

        return false;
    }
    case PropertyID::FlexWrap: {

        if (keyword_to_flex_wrap(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Float: {

        if (keyword_to_float(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FloodColor: {

        return false;
    }
    case PropertyID::FloodOpacity: {

        return false;
    }
    case PropertyID::Font: {

        return false;
    }
    case PropertyID::FontFamily: {

        return false;
    }
    case PropertyID::FontFeatureSettings: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::On:
        case Keyword::Off:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontKerning: {

        if (keyword_to_font_kerning(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontLanguageOverride: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontOpticalSizing: {

        if (keyword_to_font_optical_sizing(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontSize: {
        switch (keyword) {
        case Keyword::Math:

            return true;
        default:
            break;
        }

        if (keyword_to_absolute_size(keyword).has_value())
            return true;

        if (keyword_to_relative_size(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontStyle: {

        return false;
    }
    case PropertyID::FontVariant: {

        return false;
    }
    case PropertyID::FontVariantAlternates: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontVariantCaps: {

        if (keyword_to_font_variant_caps(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontVariantEastAsian: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontVariantEmoji: {

        if (keyword_to_font_variant_emoji(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontVariantLigatures: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontVariantNumeric: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontVariantPosition: {

        if (keyword_to_font_variant_position(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::FontVariationSettings: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontWeight: {
        switch (keyword) {
        case Keyword::Bold:
        case Keyword::Bolder:
        case Keyword::Lighter:
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::FontWidth: {

        if (keyword_to_font_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Gap: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Grid: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridArea: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridAutoColumns: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridAutoFlow: {

        return false;
    }
    case PropertyID::GridAutoRows: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridColumn: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridColumnEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridColumnStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridRow: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridRowEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridRowStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridTemplate: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridTemplateAreas: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridTemplateColumns: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::GridTemplateRows: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Height: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ImageRendering: {

        if (keyword_to_image_rendering(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::InlineSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Inset: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetBlock: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetBlockEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetBlockStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetInline: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetInlineEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::InsetInlineStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Isolation: {

        if (keyword_to_isolation(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::JustifyContent: {

        if (keyword_to_justify_content(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::JustifyItems: {

        if (keyword_to_justify_items(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::JustifySelf: {

        if (keyword_to_justify_self(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Left: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::LetterSpacing: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::LineHeight: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ListStyle: {

        return false;
    }
    case PropertyID::ListStyleImage: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ListStylePosition: {

        if (keyword_to_list_style_position(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ListStyleType: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Margin: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginBlock: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginBlockEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginBlockStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginBottom: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginInline: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginInlineEnd: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginInlineStart: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginLeft: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginRight: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MarginTop: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Mask: {

        return false;
    }
    case PropertyID::MaskClip: {
        switch (keyword) {
        case Keyword::NoClip:

            return true;
        default:
            break;
        }

        if (keyword_to_coord_box(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaskComposite: {

        if (keyword_to_compositing_operator(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaskImage: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MaskMode: {

        if (keyword_to_masking_mode(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaskOrigin: {

        if (keyword_to_coord_box(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaskPosition: {

        return false;
    }
    case PropertyID::MaskRepeat: {
        switch (keyword) {
        case Keyword::RepeatX:
        case Keyword::RepeatY:

            return true;
        default:
            break;
        }

        if (keyword_to_repetition(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaskSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::Cover:
        case Keyword::Contain:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MaskType: {

        if (keyword_to_mask_type(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MathDepth: {
        switch (keyword) {
        case Keyword::AutoAdd:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MathShift: {

        if (keyword_to_math_shift(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MathStyle: {

        if (keyword_to_math_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::MaxBlockSize: {
        switch (keyword) {
        case Keyword::MaxContent:
        case Keyword::MinContent:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MaxHeight: {
        switch (keyword) {
        case Keyword::MaxContent:
        case Keyword::MinContent:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MaxInlineSize: {
        switch (keyword) {
        case Keyword::MaxContent:
        case Keyword::MinContent:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MaxWidth: {
        switch (keyword) {
        case Keyword::MaxContent:
        case Keyword::MinContent:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MinBlockSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MinHeight: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MinInlineSize: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MinWidth: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::MixBlendMode: {

        if (keyword_to_mix_blend_mode(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ObjectFit: {

        if (keyword_to_object_fit(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ObjectPosition: {

        return false;
    }
    case PropertyID::Opacity: {

        return false;
    }
    case PropertyID::Order: {

        return false;
    }
    case PropertyID::Orphans: {

        return false;
    }
    case PropertyID::Outline: {

        return false;
    }
    case PropertyID::OutlineColor: {

        return false;
    }
    case PropertyID::OutlineOffset: {

        return false;
    }
    case PropertyID::OutlineStyle: {

        if (keyword_to_outline_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::OutlineWidth: {

        if (keyword_to_line_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Overflow: {
        switch (keyword) {
        case Keyword::Overlay:

            return true;
        default:
            break;
        }

        if (keyword_to_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::OverflowBlock: {
        switch (keyword) {
        case Keyword::Overlay:

            return true;
        default:
            break;
        }

        if (keyword_to_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::OverflowClipMargin: {

        return false;
    }
    case PropertyID::OverflowClipMarginBlock: {

        return false;
    }
    case PropertyID::OverflowClipMarginBlockEnd: {

        return false;
    }
    case PropertyID::OverflowClipMarginBlockStart: {

        return false;
    }
    case PropertyID::OverflowClipMarginBottom: {

        return false;
    }
    case PropertyID::OverflowClipMarginInline: {

        return false;
    }
    case PropertyID::OverflowClipMarginInlineEnd: {

        return false;
    }
    case PropertyID::OverflowClipMarginInlineStart: {

        return false;
    }
    case PropertyID::OverflowClipMarginLeft: {

        return false;
    }
    case PropertyID::OverflowClipMarginRight: {

        return false;
    }
    case PropertyID::OverflowClipMarginTop: {

        return false;
    }
    case PropertyID::OverflowInline: {
        switch (keyword) {
        case Keyword::Overlay:

            return true;
        default:
            break;
        }

        if (keyword_to_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::OverflowWrap: {
        switch (keyword) {
        case Keyword::Anywhere:
        case Keyword::BreakWord:
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::OverflowX: {
        switch (keyword) {
        case Keyword::Overlay:

            return true;
        default:
            break;
        }

        if (keyword_to_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::OverflowY: {
        switch (keyword) {
        case Keyword::Overlay:

            return true;
        default:
            break;
        }

        if (keyword_to_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Padding: {

        return false;
    }
    case PropertyID::PaddingBlock: {

        return false;
    }
    case PropertyID::PaddingBlockEnd: {

        return false;
    }
    case PropertyID::PaddingBlockStart: {

        return false;
    }
    case PropertyID::PaddingBottom: {

        return false;
    }
    case PropertyID::PaddingInline: {

        return false;
    }
    case PropertyID::PaddingInlineEnd: {

        return false;
    }
    case PropertyID::PaddingInlineStart: {

        return false;
    }
    case PropertyID::PaddingLeft: {

        return false;
    }
    case PropertyID::PaddingRight: {

        return false;
    }
    case PropertyID::PaddingTop: {

        return false;
    }
    case PropertyID::PaintOrder: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        if (keyword_to_paint_order(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Perspective: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::PerspectiveOrigin: {

        return false;
    }
    case PropertyID::PlaceContent: {

        return false;
    }
    case PropertyID::PlaceItems: {

        return false;
    }
    case PropertyID::PlaceSelf: {

        return false;
    }
    case PropertyID::PointerEvents: {

        if (keyword_to_pointer_events(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Position: {

        if (keyword_to_positioning(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::PositionAnchor: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::PositionArea: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        if (keyword_to_position_area(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::PositionTryFallbacks: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        if (keyword_to_try_tactic(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::PositionTryOrder: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        if (keyword_to_try_order(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::PositionVisibility: {
        switch (keyword) {
        case Keyword::Always:
        case Keyword::AnchorsValid:
        case Keyword::AnchorsVisible:
        case Keyword::NoOverflow:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Quotes: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::R: {

        return false;
    }
    case PropertyID::Resize: {

        if (keyword_to_resize(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Right: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Rotate: {

        return false;
    }
    case PropertyID::RowGap: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Rx: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Ry: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Scale: {

        return false;
    }
    case PropertyID::ScrollTimeline: {

        return false;
    }
    case PropertyID::ScrollTimelineAxis: {

        if (keyword_to_axis(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ScrollTimelineName: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ScrollbarColor: {

        return false;
    }
    case PropertyID::ScrollbarGutter: {

        return false;
    }
    case PropertyID::ScrollbarWidth: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::Thin:
        case Keyword::None:

            return true;
        default:
            break;
        }

        if (keyword_to_scrollbar_width(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ShapeImageThreshold: {

        return false;
    }
    case PropertyID::ShapeMargin: {

        return false;
    }
    case PropertyID::ShapeOutside: {

        return false;
    }
    case PropertyID::ShapeRendering: {

        if (keyword_to_shape_rendering(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::StopColor: {

        return false;
    }
    case PropertyID::StopOpacity: {

        return false;
    }
    case PropertyID::Stroke: {

        return false;
    }
    case PropertyID::StrokeDasharray: {

        return false;
    }
    case PropertyID::StrokeDashoffset: {

        return false;
    }
    case PropertyID::StrokeLinecap: {

        if (keyword_to_stroke_linecap(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::StrokeLinejoin: {

        if (keyword_to_stroke_linejoin(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::StrokeMiterlimit: {

        return false;
    }
    case PropertyID::StrokeOpacity: {

        return false;
    }
    case PropertyID::StrokeWidth: {

        return false;
    }
    case PropertyID::TabSize: {

        return false;
    }
    case PropertyID::TableLayout: {

        if (keyword_to_table_layout(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextAlign: {

        if (keyword_to_text_align(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextAnchor: {

        if (keyword_to_text_anchor(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextDecoration: {

        return false;
    }
    case PropertyID::TextDecorationColor: {

        return false;
    }
    case PropertyID::TextDecorationLine: {

        if (keyword_to_text_decoration_line(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextDecorationStyle: {

        if (keyword_to_text_decoration_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextDecorationThickness: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::FromFont:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TextIndent: {
        switch (keyword) {
        case Keyword::EachLine:
        case Keyword::Hanging:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TextJustify: {
        switch (keyword) {
        case Keyword::Distribute:

            return true;
        default:
            break;
        }

        if (keyword_to_text_justify(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextOverflow: {

        if (keyword_to_text_overflow(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextRendering: {

        if (keyword_to_text_rendering(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextShadow: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TextTransform: {

        if (keyword_to_text_transform(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextUnderlineOffset: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TextUnderlinePosition: {

        if (keyword_to_text_underline_position_horizontal(keyword).has_value())
            return true;

        if (keyword_to_text_underline_position_vertical(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextWrap: {

        return false;
    }
    case PropertyID::TextWrapMode: {

        if (keyword_to_text_wrap_mode(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TextWrapStyle: {

        if (keyword_to_text_wrap_style(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TimelineScope: {

        return false;
    }
    case PropertyID::Top: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TouchAction: {

        if (keyword_to_touch_action(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Transform: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TransformBox: {

        if (keyword_to_transform_box(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TransformOrigin: {
        switch (keyword) {
        case Keyword::Bottom:
        case Keyword::Center:
        case Keyword::Left:
        case Keyword::Right:
        case Keyword::Top:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TransformStyle: {
        switch (keyword) {
        case Keyword::Flat:
        case Keyword::Preserve3d:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Transition: {

        return false;
    }
    case PropertyID::TransitionBehavior: {

        if (keyword_to_transition_behavior(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::TransitionDelay: {

        return false;
    }
    case PropertyID::TransitionDuration: {

        return false;
    }
    case PropertyID::TransitionProperty: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::TransitionTimingFunction: {

        if (keyword_to_easing_keyword(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::Translate: {

        return false;
    }
    case PropertyID::UnicodeBidi: {

        if (keyword_to_unicode_bidi(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::UserSelect: {

        if (keyword_to_user_select(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::VerticalAlign: {

        if (keyword_to_vertical_align(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ViewTimeline: {

        return false;
    }
    case PropertyID::ViewTimelineAxis: {

        if (keyword_to_axis(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::ViewTimelineInset: {

        return false;
    }
    case PropertyID::ViewTimelineName: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::ViewTransitionName: {
        switch (keyword) {
        case Keyword::None:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Visibility: {

        if (keyword_to_visibility(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::WhiteSpace: {

        if (keyword_to_white_space(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::WhiteSpaceCollapse: {

        if (keyword_to_white_space_collapse(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::WhiteSpaceTrim: {
        switch (keyword) {
        case Keyword::None:
        case Keyword::DiscardBefore:
        case Keyword::DiscardAfter:
        case Keyword::DiscardInner:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::Widows: {

        return false;
    }
    case PropertyID::Width: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::MaxContent:
        case Keyword::MinContent:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::WillChange: {
        switch (keyword) {
        case Keyword::Auto:
        case Keyword::ScrollPosition:
        case Keyword::Contents:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::WordBreak: {
        switch (keyword) {
        case Keyword::Normal:
        case Keyword::KeepAll:
        case Keyword::BreakAll:
        case Keyword::BreakWord:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::WordSpacing: {
        switch (keyword) {
        case Keyword::Normal:

            return true;
        default:
            break;
        }

        return false;
    }
    case PropertyID::WritingMode: {

        if (keyword_to_writing_mode(keyword).has_value())
            return true;

        return false;
    }
    case PropertyID::X: {

        return false;
    }
    case PropertyID::Y: {

        return false;
    }
    case PropertyID::ZIndex: {
        switch (keyword) {
        case Keyword::Auto:

            return true;
        default:
            break;
        }

        return false;
    }

    default:
        return false;
    }
}

Optional<Keyword> resolve_legacy_value_alias(PropertyID property_id, Keyword keyword)
{
    switch (property_id) {

    case PropertyID::Overflow:
        switch (keyword) {
        case Keyword::Overlay:
            return Keyword::Auto;
        default:
            break;
        }
        break;

    case PropertyID::OverflowBlock:
        switch (keyword) {
        case Keyword::Overlay:
            return Keyword::Auto;
        default:
            break;
        }
        break;

    case PropertyID::OverflowInline:
        switch (keyword) {
        case Keyword::Overlay:
            return Keyword::Auto;
        default:
            break;
        }
        break;

    case PropertyID::OverflowX:
        switch (keyword) {
        case Keyword::Overlay:
            return Keyword::Auto;
        default:
            break;
        }
        break;

    case PropertyID::OverflowY:
        switch (keyword) {
        case Keyword::Overlay:
            return Keyword::Auto;
        default:
            break;
        }
        break;

    case PropertyID::TextJustify:
        switch (keyword) {
        case Keyword::Distribute:
            return Keyword::InterCharacter;
        default:
            break;
        }
        break;

    default:
        break;
    }
    return {};
}

Optional<ValueType> property_resolves_percentages_relative_to(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::BackgroundPosition:
        return ValueType::Length;

    case PropertyID::BackgroundPositionX:
        return ValueType::Length;

    case PropertyID::BackgroundPositionY:
        return ValueType::Length;

    case PropertyID::BackgroundSize:
        return ValueType::Length;

    case PropertyID::BlockSize:
        return ValueType::Length;

    case PropertyID::BorderBottomLeftRadius:
        return ValueType::Length;

    case PropertyID::BorderBottomRightRadius:
        return ValueType::Length;

    case PropertyID::BorderEndEndRadius:
        return ValueType::Length;

    case PropertyID::BorderEndStartRadius:
        return ValueType::Length;

    case PropertyID::BorderImageWidth:
        return ValueType::Length;

    case PropertyID::BorderRadius:
        return ValueType::Length;

    case PropertyID::BorderStartEndRadius:
        return ValueType::Length;

    case PropertyID::BorderStartStartRadius:
        return ValueType::Length;

    case PropertyID::BorderTopLeftRadius:
        return ValueType::Length;

    case PropertyID::BorderTopRightRadius:
        return ValueType::Length;

    case PropertyID::Bottom:
        return ValueType::Length;

    case PropertyID::ColumnGap:
        return ValueType::Length;

    case PropertyID::Cx:
        return ValueType::Length;

    case PropertyID::Cy:
        return ValueType::Length;

    case PropertyID::FlexBasis:
        return ValueType::Length;

    case PropertyID::FontSize:
        return ValueType::Length;

    case PropertyID::Gap:
        return ValueType::Length;

    case PropertyID::Grid:
        return ValueType::Length;

    case PropertyID::GridAutoColumns:
        return ValueType::Length;

    case PropertyID::GridAutoRows:
        return ValueType::Length;

    case PropertyID::GridTemplate:
        return ValueType::Length;

    case PropertyID::GridTemplateColumns:
        return ValueType::Length;

    case PropertyID::GridTemplateRows:
        return ValueType::Length;

    case PropertyID::Height:
        return ValueType::Length;

    case PropertyID::InlineSize:
        return ValueType::Length;

    case PropertyID::Inset:
        return ValueType::Length;

    case PropertyID::InsetBlock:
        return ValueType::Length;

    case PropertyID::InsetBlockEnd:
        return ValueType::Length;

    case PropertyID::InsetBlockStart:
        return ValueType::Length;

    case PropertyID::InsetInline:
        return ValueType::Length;

    case PropertyID::InsetInlineEnd:
        return ValueType::Length;

    case PropertyID::InsetInlineStart:
        return ValueType::Length;

    case PropertyID::Left:
        return ValueType::Length;

    case PropertyID::LetterSpacing:
        return ValueType::Length;

    case PropertyID::LineHeight:
        return ValueType::Length;

    case PropertyID::Margin:
        return ValueType::Length;

    case PropertyID::MarginBlock:
        return ValueType::Length;

    case PropertyID::MarginBlockEnd:
        return ValueType::Length;

    case PropertyID::MarginBlockStart:
        return ValueType::Length;

    case PropertyID::MarginBottom:
        return ValueType::Length;

    case PropertyID::MarginInline:
        return ValueType::Length;

    case PropertyID::MarginInlineEnd:
        return ValueType::Length;

    case PropertyID::MarginInlineStart:
        return ValueType::Length;

    case PropertyID::MarginLeft:
        return ValueType::Length;

    case PropertyID::MarginRight:
        return ValueType::Length;

    case PropertyID::MarginTop:
        return ValueType::Length;

    case PropertyID::MaskPosition:
        return ValueType::Length;

    case PropertyID::MaskSize:
        return ValueType::Length;

    case PropertyID::MaxBlockSize:
        return ValueType::Length;

    case PropertyID::MaxHeight:
        return ValueType::Length;

    case PropertyID::MaxInlineSize:
        return ValueType::Length;

    case PropertyID::MaxWidth:
        return ValueType::Length;

    case PropertyID::MinBlockSize:
        return ValueType::Length;

    case PropertyID::MinHeight:
        return ValueType::Length;

    case PropertyID::MinInlineSize:
        return ValueType::Length;

    case PropertyID::MinWidth:
        return ValueType::Length;

    case PropertyID::ObjectPosition:
        return ValueType::Length;

    case PropertyID::Padding:
        return ValueType::Length;

    case PropertyID::PaddingBlock:
        return ValueType::Length;

    case PropertyID::PaddingBlockEnd:
        return ValueType::Length;

    case PropertyID::PaddingBlockStart:
        return ValueType::Length;

    case PropertyID::PaddingBottom:
        return ValueType::Length;

    case PropertyID::PaddingInline:
        return ValueType::Length;

    case PropertyID::PaddingInlineEnd:
        return ValueType::Length;

    case PropertyID::PaddingInlineStart:
        return ValueType::Length;

    case PropertyID::PaddingLeft:
        return ValueType::Length;

    case PropertyID::PaddingRight:
        return ValueType::Length;

    case PropertyID::PaddingTop:
        return ValueType::Length;

    case PropertyID::PerspectiveOrigin:
        return ValueType::Length;

    case PropertyID::R:
        return ValueType::Length;

    case PropertyID::Right:
        return ValueType::Length;

    case PropertyID::RowGap:
        return ValueType::Length;

    case PropertyID::Rx:
        return ValueType::Length;

    case PropertyID::Ry:
        return ValueType::Length;

    case PropertyID::ShapeMargin:
        return ValueType::Length;

    case PropertyID::StrokeDasharray:
        return ValueType::Length;

    case PropertyID::StrokeDashoffset:
        return ValueType::Length;

    case PropertyID::StrokeWidth:
        return ValueType::Length;

    case PropertyID::TextDecorationThickness:
        return ValueType::Length;

    case PropertyID::TextIndent:
        return ValueType::Length;

    case PropertyID::TextUnderlineOffset:
        return ValueType::Length;

    case PropertyID::Top:
        return ValueType::Length;

    case PropertyID::Transform:
        return ValueType::Length;

    case PropertyID::TransformOrigin:
        return ValueType::Length;

    case PropertyID::Translate:
        return ValueType::Length;

    case PropertyID::VerticalAlign:
        return ValueType::Length;

    case PropertyID::ViewTimelineInset:
        return ValueType::Length;

    case PropertyID::Width:
        return ValueType::Length;

    case PropertyID::WordSpacing:
        return ValueType::Length;

    case PropertyID::X:
        return ValueType::Length;

    case PropertyID::Y:
        return ValueType::Length;

    default:
        return {};
    }
}

Vector<StringView> property_custom_ident_blacklist(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::AnimationName:
        return Vector { "none"sv, };

    case PropertyID::ColorScheme:
        return Vector { "normal"sv, "light"sv, "dark"sv, "only"sv, };

    case PropertyID::CounterIncrement:
        return Vector { "none"sv, };

    case PropertyID::CounterReset:
        return Vector { "none"sv, };

    case PropertyID::CounterSet:
        return Vector { "none"sv, };

    case PropertyID::GridArea:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridColumn:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridColumnEnd:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridColumnStart:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridRow:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridRowEnd:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::GridRowStart:
        return Vector { "auto"sv, "span"sv, };

    case PropertyID::TransitionProperty:
        return Vector { "none"sv, };

    case PropertyID::ViewTransitionName:
        return Vector { "auto"sv, "none"sv, };

    case PropertyID::WillChange:
        return Vector { "all"sv, "auto"sv, "contents"sv, "none"sv, "scroll-position"sv, "will-change"sv, };

    default:
        return {};
    }
}

size_t property_maximum_value_count(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::BackgroundPosition:
        return 4;

    case PropertyID::BackgroundRepeat:
        return 2;

    case PropertyID::BackgroundSize:
        return 2;

    case PropertyID::BlockSize:
        return 1;

    case PropertyID::BorderBlockEndColor:
        return 1;

    case PropertyID::BorderBlockEndStyle:
        return 1;

    case PropertyID::BorderBlockEndWidth:
        return 1;

    case PropertyID::BorderBlockStartColor:
        return 1;

    case PropertyID::BorderBlockStartStyle:
        return 1;

    case PropertyID::BorderBlockStartWidth:
        return 1;

    case PropertyID::BorderBottomLeftRadius:
        return 2;

    case PropertyID::BorderBottomRightRadius:
        return 2;

    case PropertyID::BorderEndEndRadius:
        return 1;

    case PropertyID::BorderEndStartRadius:
        return 1;

    case PropertyID::BorderImageOutset:
        return 4;

    case PropertyID::BorderImageRepeat:
        return 2;

    case PropertyID::BorderImageWidth:
        return 4;

    case PropertyID::BorderInlineEndColor:
        return 1;

    case PropertyID::BorderInlineEndStyle:
        return 1;

    case PropertyID::BorderInlineEndWidth:
        return 1;

    case PropertyID::BorderInlineStartColor:
        return 1;

    case PropertyID::BorderInlineStartStyle:
        return 1;

    case PropertyID::BorderInlineStartWidth:
        return 1;

    case PropertyID::BorderSpacing:
        return 2;

    case PropertyID::BorderStartEndRadius:
        return 1;

    case PropertyID::BorderStartStartRadius:
        return 1;

    case PropertyID::BorderTopLeftRadius:
        return 2;

    case PropertyID::BorderTopRightRadius:
        return 2;

    case PropertyID::Display:
        return 3;

    case PropertyID::GridTemplateColumns:
        return 4;

    case PropertyID::GridTemplateRows:
        return 4;

    case PropertyID::InlineSize:
        return 1;

    case PropertyID::InsetBlockEnd:
        return 1;

    case PropertyID::InsetBlockStart:
        return 1;

    case PropertyID::InsetInlineEnd:
        return 1;

    case PropertyID::InsetInlineStart:
        return 1;

    case PropertyID::MaskRepeat:
        return 2;

    case PropertyID::MaskSize:
        return 2;

    case PropertyID::TouchAction:
        return 2;

    case PropertyID::TransformOrigin:
        return 3;

    default:
        return 1;
    }
}
bool property_accepts_angle(PropertyID property_id, [[maybe_unused]] Angle const& value)
{
    switch (property_id) {

    default:
        return false;
    }
}

bool property_accepts_flex(PropertyID property_id, [[maybe_unused]] Flex const& value)
{
    switch (property_id) {

    default:
        return false;
    }
}

bool property_accepts_frequency(PropertyID property_id, [[maybe_unused]] Frequency const& value)
{
    switch (property_id) {

    default:
        return false;
    }
}

bool property_accepts_integer(PropertyID property_id, [[maybe_unused]] i64 const& value)
{
    switch (property_id) {

    case PropertyID::ColumnCount:
        return value >= 1;

    case PropertyID::CounterIncrement:
        return true;

    case PropertyID::CounterReset:
        return true;

    case PropertyID::CounterSet:
        return true;

    case PropertyID::FontFeatureSettings:
        return value >= 0;

    case PropertyID::MathDepth:
        return true;

    case PropertyID::Order:
        return true;

    case PropertyID::Orphans:
        return value >= 1;

    case PropertyID::Widows:
        return value >= 1;

    case PropertyID::ZIndex:
        return true;

    default:
        return false;
    }
}

bool property_accepts_length(PropertyID property_id, [[maybe_unused]] Length const& value)
{
    switch (property_id) {

    case PropertyID::BackgroundPositionX:
        return true;

    case PropertyID::BackgroundPositionY:
        return true;

    case PropertyID::BackgroundSize:
        return value.raw_value() >= 0;

    case PropertyID::BlockSize:
        return value.raw_value() >= 0;

    case PropertyID::BorderBlockEndWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderBlockStartWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderBlockWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderBottomLeftRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderBottomRightRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderBottomWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderEndEndRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderEndStartRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderImageOutset:
        return value.raw_value() >= 0;

    case PropertyID::BorderImageWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderInlineEndWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderInlineStartWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderInlineWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderLeftWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderRightWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderSpacing:
        return value.raw_value() >= 0;

    case PropertyID::BorderStartEndRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderStartStartRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderTopLeftRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderTopRightRadius:
        return value.raw_value() >= 0;

    case PropertyID::BorderTopWidth:
        return value.raw_value() >= 0;

    case PropertyID::BorderWidth:
        return value.raw_value() >= 0;

    case PropertyID::Bottom:
        return true;

    case PropertyID::ColumnGap:
        return value.raw_value() >= 0;

    case PropertyID::ColumnHeight:
        return value.raw_value() >= 0;

    case PropertyID::ColumnWidth:
        return value.raw_value() >= 0;

    case PropertyID::Cx:
        return true;

    case PropertyID::Cy:
        return true;

    case PropertyID::FlexBasis:
        return value.raw_value() >= 0;

    case PropertyID::FontSize:
        return value.raw_value() >= 0;

    case PropertyID::Gap:
        return value.raw_value() >= 0;

    case PropertyID::Grid:
        return value.raw_value() >= 0;

    case PropertyID::GridAutoColumns:
        return value.raw_value() >= 0;

    case PropertyID::GridAutoRows:
        return value.raw_value() >= 0;

    case PropertyID::GridTemplate:
        return value.raw_value() >= 0;

    case PropertyID::GridTemplateColumns:
        return value.raw_value() >= 0;

    case PropertyID::GridTemplateRows:
        return value.raw_value() >= 0;

    case PropertyID::Height:
        return value.raw_value() >= 0;

    case PropertyID::InlineSize:
        return value.raw_value() >= 0;

    case PropertyID::Inset:
        return true;

    case PropertyID::InsetBlock:
        return true;

    case PropertyID::InsetBlockEnd:
        return true;

    case PropertyID::InsetBlockStart:
        return true;

    case PropertyID::InsetInline:
        return true;

    case PropertyID::InsetInlineEnd:
        return true;

    case PropertyID::InsetInlineStart:
        return true;

    case PropertyID::Left:
        return true;

    case PropertyID::LetterSpacing:
        return true;

    case PropertyID::LineHeight:
        return value.raw_value() >= 0;

    case PropertyID::Margin:
        return true;

    case PropertyID::MarginBlock:
        return true;

    case PropertyID::MarginBlockEnd:
        return true;

    case PropertyID::MarginBlockStart:
        return true;

    case PropertyID::MarginBottom:
        return true;

    case PropertyID::MarginInline:
        return true;

    case PropertyID::MarginInlineEnd:
        return true;

    case PropertyID::MarginInlineStart:
        return true;

    case PropertyID::MarginLeft:
        return true;

    case PropertyID::MarginRight:
        return true;

    case PropertyID::MarginTop:
        return true;

    case PropertyID::MaskSize:
        return value.raw_value() >= 0;

    case PropertyID::MaxBlockSize:
        return value.raw_value() >= 0;

    case PropertyID::MaxHeight:
        return value.raw_value() >= 0;

    case PropertyID::MaxInlineSize:
        return value.raw_value() >= 0;

    case PropertyID::MaxWidth:
        return value.raw_value() >= 0;

    case PropertyID::MinBlockSize:
        return value.raw_value() >= 0;

    case PropertyID::MinHeight:
        return value.raw_value() >= 0;

    case PropertyID::MinInlineSize:
        return value.raw_value() >= 0;

    case PropertyID::MinWidth:
        return value.raw_value() >= 0;

    case PropertyID::OutlineOffset:
        return true;

    case PropertyID::OutlineWidth:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMargin:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginBlock:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginBlockEnd:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginBlockStart:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginBottom:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginInline:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginInlineEnd:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginInlineStart:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginLeft:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginRight:
        return value.raw_value() >= 0;

    case PropertyID::OverflowClipMarginTop:
        return value.raw_value() >= 0;

    case PropertyID::Padding:
        return value.raw_value() >= 0;

    case PropertyID::PaddingBlock:
        return value.raw_value() >= 0;

    case PropertyID::PaddingBlockEnd:
        return value.raw_value() >= 0;

    case PropertyID::PaddingBlockStart:
        return value.raw_value() >= 0;

    case PropertyID::PaddingBottom:
        return value.raw_value() >= 0;

    case PropertyID::PaddingInline:
        return value.raw_value() >= 0;

    case PropertyID::PaddingInlineEnd:
        return value.raw_value() >= 0;

    case PropertyID::PaddingInlineStart:
        return value.raw_value() >= 0;

    case PropertyID::PaddingLeft:
        return value.raw_value() >= 0;

    case PropertyID::PaddingRight:
        return value.raw_value() >= 0;

    case PropertyID::PaddingTop:
        return value.raw_value() >= 0;

    case PropertyID::Perspective:
        return value.raw_value() >= 0;

    case PropertyID::R:
        return value.raw_value() >= 0;

    case PropertyID::Right:
        return true;

    case PropertyID::RowGap:
        return value.raw_value() >= 0;

    case PropertyID::Rx:
        return value.raw_value() >= 0;

    case PropertyID::Ry:
        return value.raw_value() >= 0;

    case PropertyID::ShapeMargin:
        return value.raw_value() >= 0;

    case PropertyID::StrokeDashoffset:
        return value.raw_value() >= 0;

    case PropertyID::StrokeWidth:
        return value.raw_value() >= 0;

    case PropertyID::TabSize:
        return value.raw_value() >= 0;

    case PropertyID::TextDecorationThickness:
        return true;

    case PropertyID::TextIndent:
        return true;

    case PropertyID::TextUnderlineOffset:
        return true;

    case PropertyID::Top:
        return true;

    case PropertyID::TransformOrigin:
        return true;

    case PropertyID::Translate:
        return true;

    case PropertyID::VerticalAlign:
        return true;

    case PropertyID::Width:
        return value.raw_value() >= 0;

    case PropertyID::WordSpacing:
        return true;

    case PropertyID::X:
        return true;

    case PropertyID::Y:
        return true;

    default:
        return false;
    }
}

bool property_accepts_number(PropertyID property_id, [[maybe_unused]] double const& value)
{
    switch (property_id) {

    case PropertyID::AnimationIterationCount:
        return value >= 0;

    case PropertyID::BorderImageOutset:
        return value >= 0;

    case PropertyID::BorderImageSlice:
        return value >= 0;

    case PropertyID::BorderImageWidth:
        return value >= 0;

    case PropertyID::Cursor:
        return true;

    case PropertyID::FlexGrow:
        return value >= 0;

    case PropertyID::FlexShrink:
        return value >= 0;

    case PropertyID::FontVariationSettings:
        return true;

    case PropertyID::FontWeight:
        return value >= 1 && value <= 1000;

    case PropertyID::LineHeight:
        return value >= 0;

    case PropertyID::StrokeDashoffset:
        return value >= 0;

    case PropertyID::StrokeMiterlimit:
        return value >= 0;

    case PropertyID::StrokeWidth:
        return value >= 0;

    case PropertyID::TabSize:
        return value >= 0;

    default:
        return false;
    }
}

bool property_accepts_percentage(PropertyID property_id, [[maybe_unused]] Percentage const& value)
{
    switch (property_id) {

    case PropertyID::BackgroundPositionX:
        return true;

    case PropertyID::BackgroundPositionY:
        return true;

    case PropertyID::BackgroundSize:
        return value.value() >= 0;

    case PropertyID::BlockSize:
        return value.value() >= 0;

    case PropertyID::BorderBottomLeftRadius:
        return value.value() >= 0;

    case PropertyID::BorderBottomRightRadius:
        return value.value() >= 0;

    case PropertyID::BorderEndEndRadius:
        return value.value() >= 0;

    case PropertyID::BorderEndStartRadius:
        return value.value() >= 0;

    case PropertyID::BorderImageSlice:
        return value.value() >= 0;

    case PropertyID::BorderImageWidth:
        return value.value() >= 0;

    case PropertyID::BorderRadius:
        return value.value() >= 0;

    case PropertyID::BorderStartEndRadius:
        return value.value() >= 0;

    case PropertyID::BorderStartStartRadius:
        return value.value() >= 0;

    case PropertyID::BorderTopLeftRadius:
        return value.value() >= 0;

    case PropertyID::BorderTopRightRadius:
        return value.value() >= 0;

    case PropertyID::Bottom:
        return true;

    case PropertyID::ColumnGap:
        return value.value() >= 0;

    case PropertyID::Cx:
        return true;

    case PropertyID::Cy:
        return true;

    case PropertyID::FlexBasis:
        return value.value() >= 0;

    case PropertyID::FontSize:
        return value.value() >= 0;

    case PropertyID::FontWidth:
        return value.value() >= 0;

    case PropertyID::Gap:
        return value.value() >= 0;

    case PropertyID::Grid:
        return value.value() >= 0;

    case PropertyID::GridAutoColumns:
        return value.value() >= 0;

    case PropertyID::GridAutoRows:
        return value.value() >= 0;

    case PropertyID::GridTemplate:
        return value.value() >= 0;

    case PropertyID::GridTemplateColumns:
        return value.value() >= 0;

    case PropertyID::GridTemplateRows:
        return value.value() >= 0;

    case PropertyID::Height:
        return value.value() >= 0;

    case PropertyID::InlineSize:
        return value.value() >= 0;

    case PropertyID::Inset:
        return true;

    case PropertyID::InsetBlock:
        return true;

    case PropertyID::InsetBlockEnd:
        return true;

    case PropertyID::InsetBlockStart:
        return true;

    case PropertyID::InsetInline:
        return true;

    case PropertyID::InsetInlineEnd:
        return true;

    case PropertyID::InsetInlineStart:
        return true;

    case PropertyID::Left:
        return true;

    case PropertyID::LetterSpacing:
        return true;

    case PropertyID::LineHeight:
        return value.value() >= 0;

    case PropertyID::Margin:
        return true;

    case PropertyID::MarginBlock:
        return true;

    case PropertyID::MarginBlockEnd:
        return true;

    case PropertyID::MarginBlockStart:
        return true;

    case PropertyID::MarginBottom:
        return true;

    case PropertyID::MarginInline:
        return true;

    case PropertyID::MarginInlineEnd:
        return true;

    case PropertyID::MarginInlineStart:
        return true;

    case PropertyID::MarginLeft:
        return true;

    case PropertyID::MarginRight:
        return true;

    case PropertyID::MarginTop:
        return true;

    case PropertyID::MaskSize:
        return value.value() >= 0;

    case PropertyID::MaxBlockSize:
        return value.value() >= 0;

    case PropertyID::MaxHeight:
        return value.value() >= 0;

    case PropertyID::MaxInlineSize:
        return value.value() >= 0;

    case PropertyID::MaxWidth:
        return value.value() >= 0;

    case PropertyID::MinBlockSize:
        return value.value() >= 0;

    case PropertyID::MinHeight:
        return value.value() >= 0;

    case PropertyID::MinInlineSize:
        return value.value() >= 0;

    case PropertyID::MinWidth:
        return value.value() >= 0;

    case PropertyID::Padding:
        return value.value() >= 0;

    case PropertyID::PaddingBlock:
        return value.value() >= 0;

    case PropertyID::PaddingBlockEnd:
        return value.value() >= 0;

    case PropertyID::PaddingBlockStart:
        return value.value() >= 0;

    case PropertyID::PaddingBottom:
        return value.value() >= 0;

    case PropertyID::PaddingInline:
        return value.value() >= 0;

    case PropertyID::PaddingInlineEnd:
        return value.value() >= 0;

    case PropertyID::PaddingInlineStart:
        return value.value() >= 0;

    case PropertyID::PaddingLeft:
        return value.value() >= 0;

    case PropertyID::PaddingRight:
        return value.value() >= 0;

    case PropertyID::PaddingTop:
        return value.value() >= 0;

    case PropertyID::R:
        return value.value() >= 0;

    case PropertyID::Right:
        return true;

    case PropertyID::RowGap:
        return value.value() >= 0;

    case PropertyID::Rx:
        return value.value() >= 0;

    case PropertyID::Ry:
        return value.value() >= 0;

    case PropertyID::ShapeMargin:
        return value.value() >= 0;

    case PropertyID::StrokeDashoffset:
        return value.value() >= 0;

    case PropertyID::StrokeWidth:
        return value.value() >= 0;

    case PropertyID::TextDecorationThickness:
        return true;

    case PropertyID::TextIndent:
        return true;

    case PropertyID::TextUnderlineOffset:
        return true;

    case PropertyID::Top:
        return true;

    case PropertyID::TransformOrigin:
        return true;

    case PropertyID::Translate:
        return true;

    case PropertyID::VerticalAlign:
        return true;

    case PropertyID::Width:
        return value.value() >= 0;

    case PropertyID::WordSpacing:
        return true;

    case PropertyID::X:
        return true;

    case PropertyID::Y:
        return true;

    default:
        return false;
    }
}

bool property_accepts_resolution(PropertyID property_id, [[maybe_unused]] Resolution const& value)
{
    switch (property_id) {

    default:
        return false;
    }
}

bool property_accepts_time(PropertyID property_id, [[maybe_unused]] Time const& value)
{
    switch (property_id) {

    case PropertyID::AnimationDelay:
        return true;

    case PropertyID::AnimationDuration:
        return value.raw_value() >= 0;

    case PropertyID::TransitionDelay:
        return true;

    case PropertyID::TransitionDuration:
        return value.raw_value() >= 0;

    default:
        return false;
    }
}

bool property_is_shorthand(PropertyID property_id)
{
    switch (property_id) {

        case PropertyID::All:

        case PropertyID::Animation:

        case PropertyID::Background:

        case PropertyID::BackgroundPosition:

        case PropertyID::Border:

        case PropertyID::BorderBlock:

        case PropertyID::BorderBlockColor:

        case PropertyID::BorderBlockEnd:

        case PropertyID::BorderBlockStart:

        case PropertyID::BorderBlockStyle:

        case PropertyID::BorderBlockWidth:

        case PropertyID::BorderBottom:

        case PropertyID::BorderColor:

        case PropertyID::BorderImage:

        case PropertyID::BorderInline:

        case PropertyID::BorderInlineColor:

        case PropertyID::BorderInlineEnd:

        case PropertyID::BorderInlineStart:

        case PropertyID::BorderInlineStyle:

        case PropertyID::BorderInlineWidth:

        case PropertyID::BorderLeft:

        case PropertyID::BorderRadius:

        case PropertyID::BorderRight:

        case PropertyID::BorderStyle:

        case PropertyID::BorderTop:

        case PropertyID::BorderWidth:

        case PropertyID::Columns:

        case PropertyID::CornerBlockEndShape:

        case PropertyID::CornerBlockStartShape:

        case PropertyID::CornerBottomShape:

        case PropertyID::CornerInlineEndShape:

        case PropertyID::CornerInlineStartShape:

        case PropertyID::CornerLeftShape:

        case PropertyID::CornerRightShape:

        case PropertyID::CornerShape:

        case PropertyID::CornerTopShape:

        case PropertyID::Flex:

        case PropertyID::FlexFlow:

        case PropertyID::Font:

        case PropertyID::FontVariant:

        case PropertyID::Gap:

        case PropertyID::Grid:

        case PropertyID::GridArea:

        case PropertyID::GridColumn:

        case PropertyID::GridRow:

        case PropertyID::GridTemplate:

        case PropertyID::Inset:

        case PropertyID::InsetBlock:

        case PropertyID::InsetInline:

        case PropertyID::ListStyle:

        case PropertyID::Margin:

        case PropertyID::MarginBlock:

        case PropertyID::MarginInline:

        case PropertyID::Mask:

        case PropertyID::Outline:

        case PropertyID::Overflow:

        case PropertyID::OverflowClipMargin:

        case PropertyID::OverflowClipMarginBlock:

        case PropertyID::OverflowClipMarginInline:

        case PropertyID::Padding:

        case PropertyID::PaddingBlock:

        case PropertyID::PaddingInline:

        case PropertyID::PlaceContent:

        case PropertyID::PlaceItems:

        case PropertyID::PlaceSelf:

        case PropertyID::ScrollTimeline:

        case PropertyID::TextDecoration:

        case PropertyID::TextWrap:

        case PropertyID::Transition:

        case PropertyID::ViewTimeline:

        case PropertyID::WhiteSpace:

            return true;
        default:
            return false;
        }
}

Vector<PropertyID> const& longhands_for_shorthand(PropertyID property_id)
{
    switch (property_id) {

        case PropertyID::All: {
            static Vector<PropertyID> longhands = { PropertyID::WebkitTextFillColor, PropertyID::AccentColor, PropertyID::AlignContent, PropertyID::AlignItems, PropertyID::AlignSelf, PropertyID::AnchorName, PropertyID::AnchorScope, PropertyID::AnimationComposition, PropertyID::AnimationDelay, PropertyID::AnimationDirection, PropertyID::AnimationDuration, PropertyID::AnimationFillMode, PropertyID::AnimationIterationCount, PropertyID::AnimationName, PropertyID::AnimationPlayState, PropertyID::AnimationTimeline, PropertyID::AnimationTimingFunction, PropertyID::Appearance, PropertyID::AspectRatio, PropertyID::BackdropFilter, PropertyID::BackgroundAttachment, PropertyID::BackgroundBlendMode, PropertyID::BackgroundClip, PropertyID::BackgroundColor, PropertyID::BackgroundImage, PropertyID::BackgroundOrigin, PropertyID::BackgroundPositionX, PropertyID::BackgroundPositionY, PropertyID::BackgroundRepeat, PropertyID::BackgroundSize, PropertyID::BlockSize, PropertyID::BorderBlockEndColor, PropertyID::BorderBlockEndStyle, PropertyID::BorderBlockEndWidth, PropertyID::BorderBlockStartColor, PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockStartWidth, PropertyID::BorderBottomColor, PropertyID::BorderBottomLeftRadius, PropertyID::BorderBottomRightRadius, PropertyID::BorderBottomStyle, PropertyID::BorderBottomWidth, PropertyID::BorderCollapse, PropertyID::BorderEndEndRadius, PropertyID::BorderEndStartRadius, PropertyID::BorderImageOutset, PropertyID::BorderImageRepeat, PropertyID::BorderImageSlice, PropertyID::BorderImageSource, PropertyID::BorderImageWidth, PropertyID::BorderInlineEndColor, PropertyID::BorderInlineEndStyle, PropertyID::BorderInlineEndWidth, PropertyID::BorderInlineStartColor, PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineStartWidth, PropertyID::BorderLeftColor, PropertyID::BorderLeftStyle, PropertyID::BorderLeftWidth, PropertyID::BorderRightColor, PropertyID::BorderRightStyle, PropertyID::BorderRightWidth, PropertyID::BorderSpacing, PropertyID::BorderStartEndRadius, PropertyID::BorderStartStartRadius, PropertyID::BorderTopColor, PropertyID::BorderTopLeftRadius, PropertyID::BorderTopRightRadius, PropertyID::BorderTopStyle, PropertyID::BorderTopWidth, PropertyID::Bottom, PropertyID::BoxShadow, PropertyID::BoxSizing, PropertyID::CaptionSide, PropertyID::CaretColor, PropertyID::Clear, PropertyID::Clip, PropertyID::ClipPath, PropertyID::ClipRule, PropertyID::Color, PropertyID::ColorInterpolation, PropertyID::ColorScheme, PropertyID::ColumnCount, PropertyID::ColumnGap, PropertyID::ColumnHeight, PropertyID::ColumnSpan, PropertyID::ColumnWidth, PropertyID::Contain, PropertyID::ContainerType, PropertyID::Content, PropertyID::ContentVisibility, PropertyID::CornerBottomLeftShape, PropertyID::CornerBottomRightShape, PropertyID::CornerEndEndShape, PropertyID::CornerEndStartShape, PropertyID::CornerStartEndShape, PropertyID::CornerStartStartShape, PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape, PropertyID::CounterIncrement, PropertyID::CounterReset, PropertyID::CounterSet, PropertyID::Cursor, PropertyID::Cx, PropertyID::Cy, PropertyID::Display, PropertyID::DominantBaseline, PropertyID::EmptyCells, PropertyID::Fill, PropertyID::FillOpacity, PropertyID::FillRule, PropertyID::Filter, PropertyID::FlexBasis, PropertyID::FlexDirection, PropertyID::FlexGrow, PropertyID::FlexShrink, PropertyID::FlexWrap, PropertyID::Float, PropertyID::FloodColor, PropertyID::FloodOpacity, PropertyID::FontFamily, PropertyID::FontFeatureSettings, PropertyID::FontKerning, PropertyID::FontLanguageOverride, PropertyID::FontOpticalSizing, PropertyID::FontSize, PropertyID::FontStyle, PropertyID::FontVariantAlternates, PropertyID::FontVariantCaps, PropertyID::FontVariantEastAsian, PropertyID::FontVariantEmoji, PropertyID::FontVariantLigatures, PropertyID::FontVariantNumeric, PropertyID::FontVariantPosition, PropertyID::FontVariationSettings, PropertyID::FontWeight, PropertyID::FontWidth, PropertyID::GridAutoColumns, PropertyID::GridAutoFlow, PropertyID::GridAutoRows, PropertyID::GridColumnEnd, PropertyID::GridColumnStart, PropertyID::GridRowEnd, PropertyID::GridRowStart, PropertyID::GridTemplateAreas, PropertyID::GridTemplateColumns, PropertyID::GridTemplateRows, PropertyID::Height, PropertyID::ImageRendering, PropertyID::InlineSize, PropertyID::InsetBlockEnd, PropertyID::InsetBlockStart, PropertyID::InsetInlineEnd, PropertyID::InsetInlineStart, PropertyID::Isolation, PropertyID::JustifyContent, PropertyID::JustifyItems, PropertyID::JustifySelf, PropertyID::Left, PropertyID::LetterSpacing, PropertyID::LineHeight, PropertyID::ListStyleImage, PropertyID::ListStylePosition, PropertyID::ListStyleType, PropertyID::MarginBlockEnd, PropertyID::MarginBlockStart, PropertyID::MarginBottom, PropertyID::MarginInlineEnd, PropertyID::MarginInlineStart, PropertyID::MarginLeft, PropertyID::MarginRight, PropertyID::MarginTop, PropertyID::MaskClip, PropertyID::MaskComposite, PropertyID::MaskImage, PropertyID::MaskMode, PropertyID::MaskOrigin, PropertyID::MaskPosition, PropertyID::MaskRepeat, PropertyID::MaskSize, PropertyID::MaskType, PropertyID::MathDepth, PropertyID::MathShift, PropertyID::MathStyle, PropertyID::MaxBlockSize, PropertyID::MaxHeight, PropertyID::MaxInlineSize, PropertyID::MaxWidth, PropertyID::MinBlockSize, PropertyID::MinHeight, PropertyID::MinInlineSize, PropertyID::MinWidth, PropertyID::MixBlendMode, PropertyID::ObjectFit, PropertyID::ObjectPosition, PropertyID::Opacity, PropertyID::Order, PropertyID::Orphans, PropertyID::OutlineColor, PropertyID::OutlineOffset, PropertyID::OutlineStyle, PropertyID::OutlineWidth, PropertyID::OverflowBlock, PropertyID::OverflowClipMarginBlockEnd, PropertyID::OverflowClipMarginBlockStart, PropertyID::OverflowClipMarginBottom, PropertyID::OverflowClipMarginInlineEnd, PropertyID::OverflowClipMarginInlineStart, PropertyID::OverflowClipMarginLeft, PropertyID::OverflowClipMarginRight, PropertyID::OverflowClipMarginTop, PropertyID::OverflowInline, PropertyID::OverflowWrap, PropertyID::OverflowX, PropertyID::OverflowY, PropertyID::PaddingBlockEnd, PropertyID::PaddingBlockStart, PropertyID::PaddingBottom, PropertyID::PaddingInlineEnd, PropertyID::PaddingInlineStart, PropertyID::PaddingLeft, PropertyID::PaddingRight, PropertyID::PaddingTop, PropertyID::PaintOrder, PropertyID::Perspective, PropertyID::PerspectiveOrigin, PropertyID::PointerEvents, PropertyID::Position, PropertyID::PositionAnchor, PropertyID::PositionArea, PropertyID::PositionTryFallbacks, PropertyID::PositionTryOrder, PropertyID::PositionVisibility, PropertyID::Quotes, PropertyID::R, PropertyID::Resize, PropertyID::Right, PropertyID::Rotate, PropertyID::RowGap, PropertyID::Rx, PropertyID::Ry, PropertyID::Scale, PropertyID::ScrollTimelineAxis, PropertyID::ScrollTimelineName, PropertyID::ScrollbarColor, PropertyID::ScrollbarGutter, PropertyID::ScrollbarWidth, PropertyID::ShapeImageThreshold, PropertyID::ShapeMargin, PropertyID::ShapeOutside, PropertyID::ShapeRendering, PropertyID::StopColor, PropertyID::StopOpacity, PropertyID::Stroke, PropertyID::StrokeDasharray, PropertyID::StrokeDashoffset, PropertyID::StrokeLinecap, PropertyID::StrokeLinejoin, PropertyID::StrokeMiterlimit, PropertyID::StrokeOpacity, PropertyID::StrokeWidth, PropertyID::TabSize, PropertyID::TableLayout, PropertyID::TextAlign, PropertyID::TextAnchor, PropertyID::TextDecorationColor, PropertyID::TextDecorationLine, PropertyID::TextDecorationStyle, PropertyID::TextDecorationThickness, PropertyID::TextIndent, PropertyID::TextJustify, PropertyID::TextOverflow, PropertyID::TextRendering, PropertyID::TextShadow, PropertyID::TextTransform, PropertyID::TextUnderlineOffset, PropertyID::TextUnderlinePosition, PropertyID::TextWrapMode, PropertyID::TextWrapStyle, PropertyID::TimelineScope, PropertyID::Top, PropertyID::TouchAction, PropertyID::Transform, PropertyID::TransformBox, PropertyID::TransformOrigin, PropertyID::TransformStyle, PropertyID::TransitionBehavior, PropertyID::TransitionDelay, PropertyID::TransitionDuration, PropertyID::TransitionProperty, PropertyID::TransitionTimingFunction, PropertyID::Translate, PropertyID::UserSelect, PropertyID::VerticalAlign, PropertyID::ViewTimelineAxis, PropertyID::ViewTimelineInset, PropertyID::ViewTimelineName, PropertyID::ViewTransitionName, PropertyID::Visibility, PropertyID::WhiteSpaceCollapse, PropertyID::WhiteSpaceTrim, PropertyID::Widows, PropertyID::Width, PropertyID::WillChange, PropertyID::WordBreak, PropertyID::WordSpacing, PropertyID::WritingMode, PropertyID::X, PropertyID::Y, PropertyID::ZIndex };
            return longhands;
        }
        case PropertyID::Animation: {
            static Vector<PropertyID> longhands = { PropertyID::AnimationDuration, PropertyID::AnimationTimingFunction, PropertyID::AnimationDelay, PropertyID::AnimationIterationCount, PropertyID::AnimationDirection, PropertyID::AnimationFillMode, PropertyID::AnimationPlayState, PropertyID::AnimationName, PropertyID::AnimationTimeline };
            return longhands;
        }
        case PropertyID::Background: {
            static Vector<PropertyID> longhands = { PropertyID::BackgroundAttachment, PropertyID::BackgroundClip, PropertyID::BackgroundColor, PropertyID::BackgroundImage, PropertyID::BackgroundOrigin, PropertyID::BackgroundPosition, PropertyID::BackgroundRepeat, PropertyID::BackgroundSize };
            return longhands;
        }
        case PropertyID::BackgroundPosition: {
            static Vector<PropertyID> longhands = { PropertyID::BackgroundPositionX, PropertyID::BackgroundPositionY };
            return longhands;
        }
        case PropertyID::Border: {
            static Vector<PropertyID> longhands = { PropertyID::BorderWidth, PropertyID::BorderStyle, PropertyID::BorderColor, PropertyID::BorderImage };
            return longhands;
        }
        case PropertyID::BorderBlock: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockWidth, PropertyID::BorderBlockStyle, PropertyID::BorderBlockColor };
            return longhands;
        }
        case PropertyID::BorderBlockColor: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartColor, PropertyID::BorderBlockEndColor };
            return longhands;
        }
        case PropertyID::BorderBlockEnd: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockEndWidth, PropertyID::BorderBlockEndStyle, PropertyID::BorderBlockEndColor };
            return longhands;
        }
        case PropertyID::BorderBlockStart: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartWidth, PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockStartColor };
            return longhands;
        }
        case PropertyID::BorderBlockStyle: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockEndStyle };
            return longhands;
        }
        case PropertyID::BorderBlockWidth: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartWidth, PropertyID::BorderBlockEndWidth };
            return longhands;
        }
        case PropertyID::BorderBottom: {
            static Vector<PropertyID> longhands = { PropertyID::BorderBottomWidth, PropertyID::BorderBottomStyle, PropertyID::BorderBottomColor };
            return longhands;
        }
        case PropertyID::BorderColor: {
            static Vector<PropertyID> longhands = { PropertyID::BorderTopColor, PropertyID::BorderRightColor, PropertyID::BorderBottomColor, PropertyID::BorderLeftColor };
            return longhands;
        }
        case PropertyID::BorderImage: {
            static Vector<PropertyID> longhands = { PropertyID::BorderImageSource, PropertyID::BorderImageSlice, PropertyID::BorderImageWidth, PropertyID::BorderImageOutset, PropertyID::BorderImageRepeat };
            return longhands;
        }
        case PropertyID::BorderInline: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineWidth, PropertyID::BorderInlineStyle, PropertyID::BorderInlineColor };
            return longhands;
        }
        case PropertyID::BorderInlineColor: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartColor, PropertyID::BorderInlineEndColor };
            return longhands;
        }
        case PropertyID::BorderInlineEnd: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineEndWidth, PropertyID::BorderInlineEndStyle, PropertyID::BorderInlineEndColor };
            return longhands;
        }
        case PropertyID::BorderInlineStart: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartWidth, PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineStartColor };
            return longhands;
        }
        case PropertyID::BorderInlineStyle: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineEndStyle };
            return longhands;
        }
        case PropertyID::BorderInlineWidth: {
            static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartWidth, PropertyID::BorderInlineEndWidth };
            return longhands;
        }
        case PropertyID::BorderLeft: {
            static Vector<PropertyID> longhands = { PropertyID::BorderLeftWidth, PropertyID::BorderLeftStyle, PropertyID::BorderLeftColor };
            return longhands;
        }
        case PropertyID::BorderRadius: {
            static Vector<PropertyID> longhands = { PropertyID::BorderTopLeftRadius, PropertyID::BorderTopRightRadius, PropertyID::BorderBottomLeftRadius, PropertyID::BorderBottomRightRadius };
            return longhands;
        }
        case PropertyID::BorderRight: {
            static Vector<PropertyID> longhands = { PropertyID::BorderRightWidth, PropertyID::BorderRightStyle, PropertyID::BorderRightColor };
            return longhands;
        }
        case PropertyID::BorderStyle: {
            static Vector<PropertyID> longhands = { PropertyID::BorderTopStyle, PropertyID::BorderRightStyle, PropertyID::BorderBottomStyle, PropertyID::BorderLeftStyle };
            return longhands;
        }
        case PropertyID::BorderTop: {
            static Vector<PropertyID> longhands = { PropertyID::BorderTopWidth, PropertyID::BorderTopStyle, PropertyID::BorderTopColor };
            return longhands;
        }
        case PropertyID::BorderWidth: {
            static Vector<PropertyID> longhands = { PropertyID::BorderTopWidth, PropertyID::BorderRightWidth, PropertyID::BorderBottomWidth, PropertyID::BorderLeftWidth };
            return longhands;
        }
        case PropertyID::Columns: {
            static Vector<PropertyID> longhands = { PropertyID::ColumnWidth, PropertyID::ColumnCount, PropertyID::ColumnHeight };
            return longhands;
        }
        case PropertyID::CornerBlockEndShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerEndStartShape, PropertyID::CornerEndEndShape };
            return longhands;
        }
        case PropertyID::CornerBlockStartShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerStartStartShape, PropertyID::CornerStartEndShape };
            return longhands;
        }
        case PropertyID::CornerBottomShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerBottomLeftShape, PropertyID::CornerBottomRightShape };
            return longhands;
        }
        case PropertyID::CornerInlineEndShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerStartEndShape, PropertyID::CornerEndEndShape };
            return longhands;
        }
        case PropertyID::CornerInlineStartShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerStartStartShape, PropertyID::CornerEndStartShape };
            return longhands;
        }
        case PropertyID::CornerLeftShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerBottomLeftShape };
            return longhands;
        }
        case PropertyID::CornerRightShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerTopRightShape, PropertyID::CornerBottomRightShape };
            return longhands;
        }
        case PropertyID::CornerShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape, PropertyID::CornerBottomRightShape, PropertyID::CornerBottomLeftShape };
            return longhands;
        }
        case PropertyID::CornerTopShape: {
            static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape };
            return longhands;
        }
        case PropertyID::Flex: {
            static Vector<PropertyID> longhands = { PropertyID::FlexGrow, PropertyID::FlexShrink, PropertyID::FlexBasis };
            return longhands;
        }
        case PropertyID::FlexFlow: {
            static Vector<PropertyID> longhands = { PropertyID::FlexDirection, PropertyID::FlexWrap };
            return longhands;
        }
        case PropertyID::Font: {
            static Vector<PropertyID> longhands = { PropertyID::FontFamily, PropertyID::FontSize, PropertyID::FontWidth, PropertyID::FontStyle, PropertyID::FontVariant, PropertyID::FontWeight, PropertyID::LineHeight, PropertyID::FontFeatureSettings, PropertyID::FontKerning, PropertyID::FontLanguageOverride, PropertyID::FontOpticalSizing, PropertyID::FontVariationSettings };
            return longhands;
        }
        case PropertyID::FontVariant: {
            static Vector<PropertyID> longhands = { PropertyID::FontVariantAlternates, PropertyID::FontVariantCaps, PropertyID::FontVariantEastAsian, PropertyID::FontVariantEmoji, PropertyID::FontVariantLigatures, PropertyID::FontVariantNumeric, PropertyID::FontVariantPosition };
            return longhands;
        }
        case PropertyID::Gap: {
            static Vector<PropertyID> longhands = { PropertyID::RowGap, PropertyID::ColumnGap };
            return longhands;
        }
        case PropertyID::Grid: {
            static Vector<PropertyID> longhands = { PropertyID::GridAutoFlow, PropertyID::GridAutoRows, PropertyID::GridAutoColumns, PropertyID::GridTemplateAreas, PropertyID::GridTemplateRows, PropertyID::GridTemplateColumns };
            return longhands;
        }
        case PropertyID::GridArea: {
            static Vector<PropertyID> longhands = { PropertyID::GridColumnEnd, PropertyID::GridColumnStart, PropertyID::GridRowEnd, PropertyID::GridRowStart };
            return longhands;
        }
        case PropertyID::GridColumn: {
            static Vector<PropertyID> longhands = { PropertyID::GridColumnEnd, PropertyID::GridColumnStart };
            return longhands;
        }
        case PropertyID::GridRow: {
            static Vector<PropertyID> longhands = { PropertyID::GridRowEnd, PropertyID::GridRowStart };
            return longhands;
        }
        case PropertyID::GridTemplate: {
            static Vector<PropertyID> longhands = { PropertyID::GridTemplateAreas, PropertyID::GridTemplateRows, PropertyID::GridTemplateColumns };
            return longhands;
        }
        case PropertyID::Inset: {
            static Vector<PropertyID> longhands = { PropertyID::Top, PropertyID::Right, PropertyID::Bottom, PropertyID::Left };
            return longhands;
        }
        case PropertyID::InsetBlock: {
            static Vector<PropertyID> longhands = { PropertyID::InsetBlockStart, PropertyID::InsetBlockEnd };
            return longhands;
        }
        case PropertyID::InsetInline: {
            static Vector<PropertyID> longhands = { PropertyID::InsetInlineStart, PropertyID::InsetInlineEnd };
            return longhands;
        }
        case PropertyID::ListStyle: {
            static Vector<PropertyID> longhands = { PropertyID::ListStylePosition, PropertyID::ListStyleImage, PropertyID::ListStyleType };
            return longhands;
        }
        case PropertyID::Margin: {
            static Vector<PropertyID> longhands = { PropertyID::MarginTop, PropertyID::MarginRight, PropertyID::MarginBottom, PropertyID::MarginLeft };
            return longhands;
        }
        case PropertyID::MarginBlock: {
            static Vector<PropertyID> longhands = { PropertyID::MarginBlockStart, PropertyID::MarginBlockEnd };
            return longhands;
        }
        case PropertyID::MarginInline: {
            static Vector<PropertyID> longhands = { PropertyID::MarginInlineStart, PropertyID::MarginInlineEnd };
            return longhands;
        }
        case PropertyID::Mask: {
            static Vector<PropertyID> longhands = { PropertyID::MaskClip, PropertyID::MaskComposite, PropertyID::MaskImage, PropertyID::MaskMode, PropertyID::MaskOrigin, PropertyID::MaskPosition, PropertyID::MaskRepeat, PropertyID::MaskSize };
            return longhands;
        }
        case PropertyID::Outline: {
            static Vector<PropertyID> longhands = { PropertyID::OutlineColor, PropertyID::OutlineStyle, PropertyID::OutlineWidth };
            return longhands;
        }
        case PropertyID::Overflow: {
            static Vector<PropertyID> longhands = { PropertyID::OverflowX, PropertyID::OverflowY };
            return longhands;
        }
        case PropertyID::OverflowClipMargin: {
            static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginTop, PropertyID::OverflowClipMarginRight, PropertyID::OverflowClipMarginBottom, PropertyID::OverflowClipMarginLeft };
            return longhands;
        }
        case PropertyID::OverflowClipMarginBlock: {
            static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginBlockStart, PropertyID::OverflowClipMarginBlockEnd };
            return longhands;
        }
        case PropertyID::OverflowClipMarginInline: {
            static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginInlineStart, PropertyID::OverflowClipMarginInlineEnd };
            return longhands;
        }
        case PropertyID::Padding: {
            static Vector<PropertyID> longhands = { PropertyID::PaddingTop, PropertyID::PaddingRight, PropertyID::PaddingBottom, PropertyID::PaddingLeft };
            return longhands;
        }
        case PropertyID::PaddingBlock: {
            static Vector<PropertyID> longhands = { PropertyID::PaddingBlockStart, PropertyID::PaddingBlockEnd };
            return longhands;
        }
        case PropertyID::PaddingInline: {
            static Vector<PropertyID> longhands = { PropertyID::PaddingInlineStart, PropertyID::PaddingInlineEnd };
            return longhands;
        }
        case PropertyID::PlaceContent: {
            static Vector<PropertyID> longhands = { PropertyID::AlignContent, PropertyID::JustifyContent };
            return longhands;
        }
        case PropertyID::PlaceItems: {
            static Vector<PropertyID> longhands = { PropertyID::AlignItems, PropertyID::JustifyItems };
            return longhands;
        }
        case PropertyID::PlaceSelf: {
            static Vector<PropertyID> longhands = { PropertyID::AlignSelf, PropertyID::JustifySelf };
            return longhands;
        }
        case PropertyID::ScrollTimeline: {
            static Vector<PropertyID> longhands = { PropertyID::ScrollTimelineName, PropertyID::ScrollTimelineAxis };
            return longhands;
        }
        case PropertyID::TextDecoration: {
            static Vector<PropertyID> longhands = { PropertyID::TextDecorationColor, PropertyID::TextDecorationLine, PropertyID::TextDecorationStyle, PropertyID::TextDecorationThickness };
            return longhands;
        }
        case PropertyID::TextWrap: {
            static Vector<PropertyID> longhands = { PropertyID::TextWrapMode, PropertyID::TextWrapStyle };
            return longhands;
        }
        case PropertyID::Transition: {
            static Vector<PropertyID> longhands = { PropertyID::TransitionProperty, PropertyID::TransitionDuration, PropertyID::TransitionTimingFunction, PropertyID::TransitionDelay, PropertyID::TransitionBehavior };
            return longhands;
        }
        case PropertyID::ViewTimeline: {
            static Vector<PropertyID> longhands = { PropertyID::ViewTimelineName, PropertyID::ViewTimelineAxis, PropertyID::ViewTimelineInset };
            return longhands;
        }
        case PropertyID::WhiteSpace: {
            static Vector<PropertyID> longhands = { PropertyID::WhiteSpaceCollapse, PropertyID::TextWrapMode, PropertyID::WhiteSpaceTrim };
            return longhands;
        }
        default:
            static Vector<PropertyID> empty_longhands;
            return empty_longhands;
        }
}

Vector<PropertyID> const& expanded_longhands_for_shorthand(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::All: {
        static Vector<PropertyID> longhands = { PropertyID::WebkitTextFillColor, PropertyID::AccentColor, PropertyID::AlignContent, PropertyID::AlignItems, PropertyID::AlignSelf, PropertyID::AnchorName, PropertyID::AnchorScope, PropertyID::AnimationComposition, PropertyID::AnimationDelay, PropertyID::AnimationDirection, PropertyID::AnimationDuration, PropertyID::AnimationFillMode, PropertyID::AnimationIterationCount, PropertyID::AnimationName, PropertyID::AnimationPlayState, PropertyID::AnimationTimeline, PropertyID::AnimationTimingFunction, PropertyID::Appearance, PropertyID::AspectRatio, PropertyID::BackdropFilter, PropertyID::BackgroundAttachment, PropertyID::BackgroundBlendMode, PropertyID::BackgroundClip, PropertyID::BackgroundColor, PropertyID::BackgroundImage, PropertyID::BackgroundOrigin, PropertyID::BackgroundPositionX, PropertyID::BackgroundPositionY, PropertyID::BackgroundRepeat, PropertyID::BackgroundSize, PropertyID::BlockSize, PropertyID::BorderBlockEndColor, PropertyID::BorderBlockEndStyle, PropertyID::BorderBlockEndWidth, PropertyID::BorderBlockStartColor, PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockStartWidth, PropertyID::BorderBottomColor, PropertyID::BorderBottomLeftRadius, PropertyID::BorderBottomRightRadius, PropertyID::BorderBottomStyle, PropertyID::BorderBottomWidth, PropertyID::BorderCollapse, PropertyID::BorderEndEndRadius, PropertyID::BorderEndStartRadius, PropertyID::BorderImageOutset, PropertyID::BorderImageRepeat, PropertyID::BorderImageSlice, PropertyID::BorderImageSource, PropertyID::BorderImageWidth, PropertyID::BorderInlineEndColor, PropertyID::BorderInlineEndStyle, PropertyID::BorderInlineEndWidth, PropertyID::BorderInlineStartColor, PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineStartWidth, PropertyID::BorderLeftColor, PropertyID::BorderLeftStyle, PropertyID::BorderLeftWidth, PropertyID::BorderRightColor, PropertyID::BorderRightStyle, PropertyID::BorderRightWidth, PropertyID::BorderSpacing, PropertyID::BorderStartEndRadius, PropertyID::BorderStartStartRadius, PropertyID::BorderTopColor, PropertyID::BorderTopLeftRadius, PropertyID::BorderTopRightRadius, PropertyID::BorderTopStyle, PropertyID::BorderTopWidth, PropertyID::Bottom, PropertyID::BoxShadow, PropertyID::BoxSizing, PropertyID::CaptionSide, PropertyID::CaretColor, PropertyID::Clear, PropertyID::Clip, PropertyID::ClipPath, PropertyID::ClipRule, PropertyID::Color, PropertyID::ColorInterpolation, PropertyID::ColorScheme, PropertyID::ColumnCount, PropertyID::ColumnGap, PropertyID::ColumnHeight, PropertyID::ColumnSpan, PropertyID::ColumnWidth, PropertyID::Contain, PropertyID::ContainerType, PropertyID::Content, PropertyID::ContentVisibility, PropertyID::CornerBottomLeftShape, PropertyID::CornerBottomRightShape, PropertyID::CornerEndEndShape, PropertyID::CornerEndStartShape, PropertyID::CornerStartEndShape, PropertyID::CornerStartStartShape, PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape, PropertyID::CounterIncrement, PropertyID::CounterReset, PropertyID::CounterSet, PropertyID::Cursor, PropertyID::Cx, PropertyID::Cy, PropertyID::Display, PropertyID::DominantBaseline, PropertyID::EmptyCells, PropertyID::Fill, PropertyID::FillOpacity, PropertyID::FillRule, PropertyID::Filter, PropertyID::FlexBasis, PropertyID::FlexDirection, PropertyID::FlexGrow, PropertyID::FlexShrink, PropertyID::FlexWrap, PropertyID::Float, PropertyID::FloodColor, PropertyID::FloodOpacity, PropertyID::FontFamily, PropertyID::FontFeatureSettings, PropertyID::FontKerning, PropertyID::FontLanguageOverride, PropertyID::FontOpticalSizing, PropertyID::FontSize, PropertyID::FontStyle, PropertyID::FontVariantAlternates, PropertyID::FontVariantCaps, PropertyID::FontVariantEastAsian, PropertyID::FontVariantEmoji, PropertyID::FontVariantLigatures, PropertyID::FontVariantNumeric, PropertyID::FontVariantPosition, PropertyID::FontVariationSettings, PropertyID::FontWeight, PropertyID::FontWidth, PropertyID::GridAutoColumns, PropertyID::GridAutoFlow, PropertyID::GridAutoRows, PropertyID::GridColumnEnd, PropertyID::GridColumnStart, PropertyID::GridRowEnd, PropertyID::GridRowStart, PropertyID::GridTemplateAreas, PropertyID::GridTemplateColumns, PropertyID::GridTemplateRows, PropertyID::Height, PropertyID::ImageRendering, PropertyID::InlineSize, PropertyID::InsetBlockEnd, PropertyID::InsetBlockStart, PropertyID::InsetInlineEnd, PropertyID::InsetInlineStart, PropertyID::Isolation, PropertyID::JustifyContent, PropertyID::JustifyItems, PropertyID::JustifySelf, PropertyID::Left, PropertyID::LetterSpacing, PropertyID::LineHeight, PropertyID::ListStyleImage, PropertyID::ListStylePosition, PropertyID::ListStyleType, PropertyID::MarginBlockEnd, PropertyID::MarginBlockStart, PropertyID::MarginBottom, PropertyID::MarginInlineEnd, PropertyID::MarginInlineStart, PropertyID::MarginLeft, PropertyID::MarginRight, PropertyID::MarginTop, PropertyID::MaskClip, PropertyID::MaskComposite, PropertyID::MaskImage, PropertyID::MaskMode, PropertyID::MaskOrigin, PropertyID::MaskPosition, PropertyID::MaskRepeat, PropertyID::MaskSize, PropertyID::MaskType, PropertyID::MathDepth, PropertyID::MathShift, PropertyID::MathStyle, PropertyID::MaxBlockSize, PropertyID::MaxHeight, PropertyID::MaxInlineSize, PropertyID::MaxWidth, PropertyID::MinBlockSize, PropertyID::MinHeight, PropertyID::MinInlineSize, PropertyID::MinWidth, PropertyID::MixBlendMode, PropertyID::ObjectFit, PropertyID::ObjectPosition, PropertyID::Opacity, PropertyID::Order, PropertyID::Orphans, PropertyID::OutlineColor, PropertyID::OutlineOffset, PropertyID::OutlineStyle, PropertyID::OutlineWidth, PropertyID::OverflowBlock, PropertyID::OverflowClipMarginBlockEnd, PropertyID::OverflowClipMarginBlockStart, PropertyID::OverflowClipMarginBottom, PropertyID::OverflowClipMarginInlineEnd, PropertyID::OverflowClipMarginInlineStart, PropertyID::OverflowClipMarginLeft, PropertyID::OverflowClipMarginRight, PropertyID::OverflowClipMarginTop, PropertyID::OverflowInline, PropertyID::OverflowWrap, PropertyID::OverflowX, PropertyID::OverflowY, PropertyID::PaddingBlockEnd, PropertyID::PaddingBlockStart, PropertyID::PaddingBottom, PropertyID::PaddingInlineEnd, PropertyID::PaddingInlineStart, PropertyID::PaddingLeft, PropertyID::PaddingRight, PropertyID::PaddingTop, PropertyID::PaintOrder, PropertyID::Perspective, PropertyID::PerspectiveOrigin, PropertyID::PointerEvents, PropertyID::Position, PropertyID::PositionAnchor, PropertyID::PositionArea, PropertyID::PositionTryFallbacks, PropertyID::PositionTryOrder, PropertyID::PositionVisibility, PropertyID::Quotes, PropertyID::R, PropertyID::Resize, PropertyID::Right, PropertyID::Rotate, PropertyID::RowGap, PropertyID::Rx, PropertyID::Ry, PropertyID::Scale, PropertyID::ScrollTimelineAxis, PropertyID::ScrollTimelineName, PropertyID::ScrollbarColor, PropertyID::ScrollbarGutter, PropertyID::ScrollbarWidth, PropertyID::ShapeImageThreshold, PropertyID::ShapeMargin, PropertyID::ShapeOutside, PropertyID::ShapeRendering, PropertyID::StopColor, PropertyID::StopOpacity, PropertyID::Stroke, PropertyID::StrokeDasharray, PropertyID::StrokeDashoffset, PropertyID::StrokeLinecap, PropertyID::StrokeLinejoin, PropertyID::StrokeMiterlimit, PropertyID::StrokeOpacity, PropertyID::StrokeWidth, PropertyID::TabSize, PropertyID::TableLayout, PropertyID::TextAlign, PropertyID::TextAnchor, PropertyID::TextDecorationColor, PropertyID::TextDecorationLine, PropertyID::TextDecorationStyle, PropertyID::TextDecorationThickness, PropertyID::TextIndent, PropertyID::TextJustify, PropertyID::TextOverflow, PropertyID::TextRendering, PropertyID::TextShadow, PropertyID::TextTransform, PropertyID::TextUnderlineOffset, PropertyID::TextUnderlinePosition, PropertyID::TextWrapMode, PropertyID::TextWrapStyle, PropertyID::TimelineScope, PropertyID::Top, PropertyID::TouchAction, PropertyID::Transform, PropertyID::TransformBox, PropertyID::TransformOrigin, PropertyID::TransformStyle, PropertyID::TransitionBehavior, PropertyID::TransitionDelay, PropertyID::TransitionDuration, PropertyID::TransitionProperty, PropertyID::TransitionTimingFunction, PropertyID::Translate, PropertyID::UserSelect, PropertyID::VerticalAlign, PropertyID::ViewTimelineAxis, PropertyID::ViewTimelineInset, PropertyID::ViewTimelineName, PropertyID::ViewTransitionName, PropertyID::Visibility, PropertyID::WhiteSpaceCollapse, PropertyID::WhiteSpaceTrim, PropertyID::Widows, PropertyID::Width, PropertyID::WillChange, PropertyID::WordBreak, PropertyID::WordSpacing, PropertyID::WritingMode, PropertyID::X, PropertyID::Y, PropertyID::ZIndex };
        return longhands;
    }
    case PropertyID::Animation: {
        static Vector<PropertyID> longhands = { PropertyID::AnimationDuration, PropertyID::AnimationTimingFunction, PropertyID::AnimationDelay, PropertyID::AnimationIterationCount, PropertyID::AnimationDirection, PropertyID::AnimationFillMode, PropertyID::AnimationPlayState, PropertyID::AnimationName, PropertyID::AnimationTimeline };
        return longhands;
    }
    case PropertyID::Background: {
        static Vector<PropertyID> longhands = { PropertyID::BackgroundAttachment, PropertyID::BackgroundClip, PropertyID::BackgroundColor, PropertyID::BackgroundImage, PropertyID::BackgroundOrigin, PropertyID::BackgroundPositionX, PropertyID::BackgroundPositionY, PropertyID::BackgroundRepeat, PropertyID::BackgroundSize };
        return longhands;
    }
    case PropertyID::BackgroundPosition: {
        static Vector<PropertyID> longhands = { PropertyID::BackgroundPositionX, PropertyID::BackgroundPositionY };
        return longhands;
    }
    case PropertyID::Border: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopWidth, PropertyID::BorderRightWidth, PropertyID::BorderBottomWidth, PropertyID::BorderLeftWidth, PropertyID::BorderTopStyle, PropertyID::BorderRightStyle, PropertyID::BorderBottomStyle, PropertyID::BorderLeftStyle, PropertyID::BorderTopColor, PropertyID::BorderRightColor, PropertyID::BorderBottomColor, PropertyID::BorderLeftColor, PropertyID::BorderImageSource, PropertyID::BorderImageSlice, PropertyID::BorderImageWidth, PropertyID::BorderImageOutset, PropertyID::BorderImageRepeat };
        return longhands;
    }
    case PropertyID::BorderBlock: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartWidth, PropertyID::BorderBlockEndWidth, PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockEndStyle, PropertyID::BorderBlockStartColor, PropertyID::BorderBlockEndColor };
        return longhands;
    }
    case PropertyID::BorderBlockColor: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartColor, PropertyID::BorderBlockEndColor };
        return longhands;
    }
    case PropertyID::BorderBlockEnd: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockEndWidth, PropertyID::BorderBlockEndStyle, PropertyID::BorderBlockEndColor };
        return longhands;
    }
    case PropertyID::BorderBlockStart: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartWidth, PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockStartColor };
        return longhands;
    }
    case PropertyID::BorderBlockStyle: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartStyle, PropertyID::BorderBlockEndStyle };
        return longhands;
    }
    case PropertyID::BorderBlockWidth: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBlockStartWidth, PropertyID::BorderBlockEndWidth };
        return longhands;
    }
    case PropertyID::BorderBottom: {
        static Vector<PropertyID> longhands = { PropertyID::BorderBottomWidth, PropertyID::BorderBottomStyle, PropertyID::BorderBottomColor };
        return longhands;
    }
    case PropertyID::BorderColor: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopColor, PropertyID::BorderRightColor, PropertyID::BorderBottomColor, PropertyID::BorderLeftColor };
        return longhands;
    }
    case PropertyID::BorderImage: {
        static Vector<PropertyID> longhands = { PropertyID::BorderImageSource, PropertyID::BorderImageSlice, PropertyID::BorderImageWidth, PropertyID::BorderImageOutset, PropertyID::BorderImageRepeat };
        return longhands;
    }
    case PropertyID::BorderInline: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartWidth, PropertyID::BorderInlineEndWidth, PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineEndStyle, PropertyID::BorderInlineStartColor, PropertyID::BorderInlineEndColor };
        return longhands;
    }
    case PropertyID::BorderInlineColor: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartColor, PropertyID::BorderInlineEndColor };
        return longhands;
    }
    case PropertyID::BorderInlineEnd: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineEndWidth, PropertyID::BorderInlineEndStyle, PropertyID::BorderInlineEndColor };
        return longhands;
    }
    case PropertyID::BorderInlineStart: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartWidth, PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineStartColor };
        return longhands;
    }
    case PropertyID::BorderInlineStyle: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartStyle, PropertyID::BorderInlineEndStyle };
        return longhands;
    }
    case PropertyID::BorderInlineWidth: {
        static Vector<PropertyID> longhands = { PropertyID::BorderInlineStartWidth, PropertyID::BorderInlineEndWidth };
        return longhands;
    }
    case PropertyID::BorderLeft: {
        static Vector<PropertyID> longhands = { PropertyID::BorderLeftWidth, PropertyID::BorderLeftStyle, PropertyID::BorderLeftColor };
        return longhands;
    }
    case PropertyID::BorderRadius: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopLeftRadius, PropertyID::BorderTopRightRadius, PropertyID::BorderBottomLeftRadius, PropertyID::BorderBottomRightRadius };
        return longhands;
    }
    case PropertyID::BorderRight: {
        static Vector<PropertyID> longhands = { PropertyID::BorderRightWidth, PropertyID::BorderRightStyle, PropertyID::BorderRightColor };
        return longhands;
    }
    case PropertyID::BorderStyle: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopStyle, PropertyID::BorderRightStyle, PropertyID::BorderBottomStyle, PropertyID::BorderLeftStyle };
        return longhands;
    }
    case PropertyID::BorderTop: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopWidth, PropertyID::BorderTopStyle, PropertyID::BorderTopColor };
        return longhands;
    }
    case PropertyID::BorderWidth: {
        static Vector<PropertyID> longhands = { PropertyID::BorderTopWidth, PropertyID::BorderRightWidth, PropertyID::BorderBottomWidth, PropertyID::BorderLeftWidth };
        return longhands;
    }
    case PropertyID::Columns: {
        static Vector<PropertyID> longhands = { PropertyID::ColumnWidth, PropertyID::ColumnCount, PropertyID::ColumnHeight };
        return longhands;
    }
    case PropertyID::CornerBlockEndShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerEndStartShape, PropertyID::CornerEndEndShape };
        return longhands;
    }
    case PropertyID::CornerBlockStartShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerStartStartShape, PropertyID::CornerStartEndShape };
        return longhands;
    }
    case PropertyID::CornerBottomShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerBottomLeftShape, PropertyID::CornerBottomRightShape };
        return longhands;
    }
    case PropertyID::CornerInlineEndShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerStartEndShape, PropertyID::CornerEndEndShape };
        return longhands;
    }
    case PropertyID::CornerInlineStartShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerStartStartShape, PropertyID::CornerEndStartShape };
        return longhands;
    }
    case PropertyID::CornerLeftShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerBottomLeftShape };
        return longhands;
    }
    case PropertyID::CornerRightShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerTopRightShape, PropertyID::CornerBottomRightShape };
        return longhands;
    }
    case PropertyID::CornerShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape, PropertyID::CornerBottomRightShape, PropertyID::CornerBottomLeftShape };
        return longhands;
    }
    case PropertyID::CornerTopShape: {
        static Vector<PropertyID> longhands = { PropertyID::CornerTopLeftShape, PropertyID::CornerTopRightShape };
        return longhands;
    }
    case PropertyID::Flex: {
        static Vector<PropertyID> longhands = { PropertyID::FlexGrow, PropertyID::FlexShrink, PropertyID::FlexBasis };
        return longhands;
    }
    case PropertyID::FlexFlow: {
        static Vector<PropertyID> longhands = { PropertyID::FlexDirection, PropertyID::FlexWrap };
        return longhands;
    }
    case PropertyID::Font: {
        static Vector<PropertyID> longhands = { PropertyID::FontFamily, PropertyID::FontSize, PropertyID::FontWidth, PropertyID::FontStyle, PropertyID::FontVariantAlternates, PropertyID::FontVariantCaps, PropertyID::FontVariantEastAsian, PropertyID::FontVariantEmoji, PropertyID::FontVariantLigatures, PropertyID::FontVariantNumeric, PropertyID::FontVariantPosition, PropertyID::FontWeight, PropertyID::LineHeight, PropertyID::FontFeatureSettings, PropertyID::FontKerning, PropertyID::FontLanguageOverride, PropertyID::FontOpticalSizing, PropertyID::FontVariationSettings };
        return longhands;
    }
    case PropertyID::FontVariant: {
        static Vector<PropertyID> longhands = { PropertyID::FontVariantAlternates, PropertyID::FontVariantCaps, PropertyID::FontVariantEastAsian, PropertyID::FontVariantEmoji, PropertyID::FontVariantLigatures, PropertyID::FontVariantNumeric, PropertyID::FontVariantPosition };
        return longhands;
    }
    case PropertyID::Gap: {
        static Vector<PropertyID> longhands = { PropertyID::RowGap, PropertyID::ColumnGap };
        return longhands;
    }
    case PropertyID::Grid: {
        static Vector<PropertyID> longhands = { PropertyID::GridAutoFlow, PropertyID::GridAutoRows, PropertyID::GridAutoColumns, PropertyID::GridTemplateAreas, PropertyID::GridTemplateRows, PropertyID::GridTemplateColumns };
        return longhands;
    }
    case PropertyID::GridArea: {
        static Vector<PropertyID> longhands = { PropertyID::GridColumnEnd, PropertyID::GridColumnStart, PropertyID::GridRowEnd, PropertyID::GridRowStart };
        return longhands;
    }
    case PropertyID::GridColumn: {
        static Vector<PropertyID> longhands = { PropertyID::GridColumnEnd, PropertyID::GridColumnStart };
        return longhands;
    }
    case PropertyID::GridRow: {
        static Vector<PropertyID> longhands = { PropertyID::GridRowEnd, PropertyID::GridRowStart };
        return longhands;
    }
    case PropertyID::GridTemplate: {
        static Vector<PropertyID> longhands = { PropertyID::GridTemplateAreas, PropertyID::GridTemplateRows, PropertyID::GridTemplateColumns };
        return longhands;
    }
    case PropertyID::Inset: {
        static Vector<PropertyID> longhands = { PropertyID::Top, PropertyID::Right, PropertyID::Bottom, PropertyID::Left };
        return longhands;
    }
    case PropertyID::InsetBlock: {
        static Vector<PropertyID> longhands = { PropertyID::InsetBlockStart, PropertyID::InsetBlockEnd };
        return longhands;
    }
    case PropertyID::InsetInline: {
        static Vector<PropertyID> longhands = { PropertyID::InsetInlineStart, PropertyID::InsetInlineEnd };
        return longhands;
    }
    case PropertyID::ListStyle: {
        static Vector<PropertyID> longhands = { PropertyID::ListStylePosition, PropertyID::ListStyleImage, PropertyID::ListStyleType };
        return longhands;
    }
    case PropertyID::Margin: {
        static Vector<PropertyID> longhands = { PropertyID::MarginTop, PropertyID::MarginRight, PropertyID::MarginBottom, PropertyID::MarginLeft };
        return longhands;
    }
    case PropertyID::MarginBlock: {
        static Vector<PropertyID> longhands = { PropertyID::MarginBlockStart, PropertyID::MarginBlockEnd };
        return longhands;
    }
    case PropertyID::MarginInline: {
        static Vector<PropertyID> longhands = { PropertyID::MarginInlineStart, PropertyID::MarginInlineEnd };
        return longhands;
    }
    case PropertyID::Mask: {
        static Vector<PropertyID> longhands = { PropertyID::MaskClip, PropertyID::MaskComposite, PropertyID::MaskImage, PropertyID::MaskMode, PropertyID::MaskOrigin, PropertyID::MaskPosition, PropertyID::MaskRepeat, PropertyID::MaskSize };
        return longhands;
    }
    case PropertyID::Outline: {
        static Vector<PropertyID> longhands = { PropertyID::OutlineColor, PropertyID::OutlineStyle, PropertyID::OutlineWidth };
        return longhands;
    }
    case PropertyID::Overflow: {
        static Vector<PropertyID> longhands = { PropertyID::OverflowX, PropertyID::OverflowY };
        return longhands;
    }
    case PropertyID::OverflowClipMargin: {
        static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginTop, PropertyID::OverflowClipMarginRight, PropertyID::OverflowClipMarginBottom, PropertyID::OverflowClipMarginLeft };
        return longhands;
    }
    case PropertyID::OverflowClipMarginBlock: {
        static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginBlockStart, PropertyID::OverflowClipMarginBlockEnd };
        return longhands;
    }
    case PropertyID::OverflowClipMarginInline: {
        static Vector<PropertyID> longhands = { PropertyID::OverflowClipMarginInlineStart, PropertyID::OverflowClipMarginInlineEnd };
        return longhands;
    }
    case PropertyID::Padding: {
        static Vector<PropertyID> longhands = { PropertyID::PaddingTop, PropertyID::PaddingRight, PropertyID::PaddingBottom, PropertyID::PaddingLeft };
        return longhands;
    }
    case PropertyID::PaddingBlock: {
        static Vector<PropertyID> longhands = { PropertyID::PaddingBlockStart, PropertyID::PaddingBlockEnd };
        return longhands;
    }
    case PropertyID::PaddingInline: {
        static Vector<PropertyID> longhands = { PropertyID::PaddingInlineStart, PropertyID::PaddingInlineEnd };
        return longhands;
    }
    case PropertyID::PlaceContent: {
        static Vector<PropertyID> longhands = { PropertyID::AlignContent, PropertyID::JustifyContent };
        return longhands;
    }
    case PropertyID::PlaceItems: {
        static Vector<PropertyID> longhands = { PropertyID::AlignItems, PropertyID::JustifyItems };
        return longhands;
    }
    case PropertyID::PlaceSelf: {
        static Vector<PropertyID> longhands = { PropertyID::AlignSelf, PropertyID::JustifySelf };
        return longhands;
    }
    case PropertyID::ScrollTimeline: {
        static Vector<PropertyID> longhands = { PropertyID::ScrollTimelineName, PropertyID::ScrollTimelineAxis };
        return longhands;
    }
    case PropertyID::TextDecoration: {
        static Vector<PropertyID> longhands = { PropertyID::TextDecorationColor, PropertyID::TextDecorationLine, PropertyID::TextDecorationStyle, PropertyID::TextDecorationThickness };
        return longhands;
    }
    case PropertyID::TextWrap: {
        static Vector<PropertyID> longhands = { PropertyID::TextWrapMode, PropertyID::TextWrapStyle };
        return longhands;
    }
    case PropertyID::Transition: {
        static Vector<PropertyID> longhands = { PropertyID::TransitionProperty, PropertyID::TransitionDuration, PropertyID::TransitionTimingFunction, PropertyID::TransitionDelay, PropertyID::TransitionBehavior };
        return longhands;
    }
    case PropertyID::ViewTimeline: {
        static Vector<PropertyID> longhands = { PropertyID::ViewTimelineName, PropertyID::ViewTimelineAxis, PropertyID::ViewTimelineInset };
        return longhands;
    }
    case PropertyID::WhiteSpace: {
        static Vector<PropertyID> longhands = { PropertyID::WhiteSpaceCollapse, PropertyID::TextWrapMode, PropertyID::WhiteSpaceTrim };
        return longhands;
    }
    default: {
        static Vector<PropertyID> empty_longhands;
        return empty_longhands;
    }
    }
}

bool property_maps_to_shorthand(PropertyID property_id)
{
    switch (property_id) {

        case PropertyID::GridColumnStart:

        case PropertyID::WordSpacing:

        case PropertyID::PositionArea:

        case PropertyID::BorderInlineWidth:

        case PropertyID::TableLayout:

        case PropertyID::FillOpacity:

        case PropertyID::BorderLeftColor:

        case PropertyID::OverflowX:

        case PropertyID::TransformOrigin:

        case PropertyID::Orphans:

        case PropertyID::FontLanguageOverride:

        case PropertyID::AnchorName:

        case PropertyID::FontVariantLigatures:

        case PropertyID::OverflowWrap:

        case PropertyID::BackgroundPositionY:

        case PropertyID::BorderBlockStartColor:

        case PropertyID::PaddingRight:

        case PropertyID::MaxHeight:

        case PropertyID::Isolation:

        case PropertyID::BackgroundBlendMode:

        case PropertyID::FontVariantEmoji:

        case PropertyID::Cy:

        case PropertyID::ShapeRendering:

        case PropertyID::BorderStartStartRadius:

        case PropertyID::MaskPosition:

        case PropertyID::Rotate:

        case PropertyID::TextDecorationLine:

        case PropertyID::ColumnGap:

        case PropertyID::FlexWrap:

        case PropertyID::PaddingLeft:

        case PropertyID::BorderInlineColor:

        case PropertyID::BorderCollapse:

        case PropertyID::OverflowBlock:

        case PropertyID::BorderColor:

        case PropertyID::Cursor:

        case PropertyID::WordBreak:

        case PropertyID::BackgroundSize:

        case PropertyID::Float:

        case PropertyID::BackgroundColor:

        case PropertyID::MarginInlineEnd:

        case PropertyID::BackgroundOrigin:

        case PropertyID::BorderStartEndRadius:

        case PropertyID::BorderRightStyle:

        case PropertyID::ScrollbarColor:

        case PropertyID::ViewTimelineInset:

        case PropertyID::Left:

        case PropertyID::TouchAction:

        case PropertyID::BorderBlockStartStyle:

        case PropertyID::Display:

        case PropertyID::Order:

        case PropertyID::AnimationDuration:

        case PropertyID::ImageRendering:

        case PropertyID::InsetBlockStart:

        case PropertyID::MixBlendMode:

        case PropertyID::TextOverflow:

        case PropertyID::MaskType:

        case PropertyID::FontVariantAlternates:

        case PropertyID::BorderImage:

        case PropertyID::AnimationTimingFunction:

        case PropertyID::TransitionDelay:

        case PropertyID::ColumnWidth:

        case PropertyID::BorderBottomStyle:

        case PropertyID::AnimationFillMode:

        case PropertyID::ClipRule:

        case PropertyID::MarginTop:

        case PropertyID::AccentColor:

        case PropertyID::CornerTopRightShape:

        case PropertyID::CornerTopLeftShape:

        case PropertyID::BackgroundPositionX:

        case PropertyID::Ry:

        case PropertyID::TransitionProperty:

        case PropertyID::AnimationTimeline:

        case PropertyID::MaxWidth:

        case PropertyID::ColumnSpan:

        case PropertyID::PointerEvents:

        case PropertyID::EmptyCells:

        case PropertyID::TransitionBehavior:

        case PropertyID::FontSize:

        case PropertyID::BorderSpacing:

        case PropertyID::ViewTimelineName:

        case PropertyID::BorderBottomColor:

        case PropertyID::CounterReset:

        case PropertyID::MaskComposite:

        case PropertyID::FlexGrow:

        case PropertyID::ListStyleImage:

        case PropertyID::Content:

        case PropertyID::BorderBottomWidth:

        case PropertyID::StrokeLinejoin:

        case PropertyID::RowGap:

        case PropertyID::Cx:

        case PropertyID::BorderImageOutset:

        case PropertyID::BorderInlineEndWidth:

        case PropertyID::GridAutoRows:

        case PropertyID::MaxInlineSize:

        case PropertyID::CounterIncrement:

        case PropertyID::StrokeMiterlimit:

        case PropertyID::MaskOrigin:

        case PropertyID::MinWidth:

        case PropertyID::ShapeMargin:

        case PropertyID::MathStyle:

        case PropertyID::BorderLeftWidth:

        case PropertyID::Top:

        case PropertyID::TransitionDuration:

        case PropertyID::BorderTopWidth:

        case PropertyID::MinHeight:

        case PropertyID::PositionTryFallbacks:

        case PropertyID::Width:

        case PropertyID::TextShadow:

        case PropertyID::OutlineStyle:

        case PropertyID::BorderWidth:

        case PropertyID::BorderInlineStartWidth:

        case PropertyID::AnimationComposition:

        case PropertyID::TextAlign:

        case PropertyID::ListStylePosition:

        case PropertyID::MarginBottom:

        case PropertyID::FontWeight:

        case PropertyID::StrokeOpacity:

        case PropertyID::ViewTimelineAxis:

        case PropertyID::Bottom:

        case PropertyID::TextTransform:

        case PropertyID::BorderLeftStyle:

        case PropertyID::FontFeatureSettings:

        case PropertyID::StopColor:

        case PropertyID::AnimationPlayState:

        case PropertyID::InlineSize:

        case PropertyID::BorderStyle:

        case PropertyID::AnimationDelay:

        case PropertyID::BackgroundAttachment:

        case PropertyID::UserSelect:

        case PropertyID::WritingMode:

        case PropertyID::BoxSizing:

        case PropertyID::LineHeight:

        case PropertyID::PositionVisibility:

        case PropertyID::BorderBottomRightRadius:

        case PropertyID::BackgroundPosition:

        case PropertyID::Scale:

        case PropertyID::BackgroundImage:

        case PropertyID::AlignItems:

        case PropertyID::AnimationIterationCount:

        case PropertyID::BorderBlockStyle:

        case PropertyID::CornerStartEndShape:

        case PropertyID::AlignContent:

        case PropertyID::OverflowClipMarginInlineStart:

        case PropertyID::BorderRightColor:

        case PropertyID::FontKerning:

        case PropertyID::MarginLeft:

        case PropertyID::ObjectFit:

        case PropertyID::WhiteSpaceTrim:

        case PropertyID::MarginBlockStart:

        case PropertyID::BackdropFilter:

        case PropertyID::MaskMode:

        case PropertyID::Rx:

        case PropertyID::TextDecorationStyle:

        case PropertyID::BorderBottomLeftRadius:

        case PropertyID::ScrollTimelineAxis:

        case PropertyID::LetterSpacing:

        case PropertyID::GridTemplateRows:

        case PropertyID::GridAutoFlow:

        case PropertyID::TextUnderlinePosition:

        case PropertyID::TimelineScope:

        case PropertyID::FontVariant:

        case PropertyID::CaretColor:

        case PropertyID::ZIndex:

        case PropertyID::OverflowClipMarginBlockEnd:

        case PropertyID::GridAutoColumns:

        case PropertyID::ScrollTimelineName:

        case PropertyID::PerspectiveOrigin:

        case PropertyID::PaddingInlineEnd:

        case PropertyID::Visibility:

        case PropertyID::TransformBox:

        case PropertyID::Y:

        case PropertyID::OverflowClipMarginLeft:

        case PropertyID::TextDecorationColor:

        case PropertyID::ColumnCount:

        case PropertyID::WhiteSpaceCollapse:

        case PropertyID::MinBlockSize:

        case PropertyID::TransformStyle:

        case PropertyID::CaptionSide:

        case PropertyID::WillChange:

        case PropertyID::BorderInlineEndStyle:

        case PropertyID::CounterSet:

        case PropertyID::AlignSelf:

        case PropertyID::AnimationName:

        case PropertyID::Translate:

        case PropertyID::BorderInlineStartColor:

        case PropertyID::OutlineColor:

        case PropertyID::OverflowClipMarginInlineEnd:

        case PropertyID::PositionAnchor:

        case PropertyID::ContentVisibility:

        case PropertyID::Opacity:

        case PropertyID::Color:

        case PropertyID::Clip:

        case PropertyID::FontVariantNumeric:

        case PropertyID::MaskSize:

        case PropertyID::Transform:

        case PropertyID::ColorInterpolation:

        case PropertyID::PaddingBlockStart:

        case PropertyID::PaddingInlineStart:

        case PropertyID::BorderInlineStartStyle:

        case PropertyID::Height:

        case PropertyID::CornerBottomRightShape:

        case PropertyID::FontVariantCaps:

        case PropertyID::Right:

        case PropertyID::Widows:

        case PropertyID::FloodOpacity:

        case PropertyID::ListStyleType:

        case PropertyID::DominantBaseline:

        case PropertyID::GridTemplateColumns:

        case PropertyID::OverflowClipMarginTop:

        case PropertyID::BorderImageSlice:

        case PropertyID::OutlineWidth:

        case PropertyID::BorderBlockStartWidth:

        case PropertyID::BorderRightWidth:

        case PropertyID::BorderTopStyle:

        case PropertyID::TextJustify:

        case PropertyID::PaddingBlockEnd:

        case PropertyID::Contain:

        case PropertyID::StrokeDashoffset:

        case PropertyID::ViewTransitionName:

        case PropertyID::TextWrapMode:

        case PropertyID::WebkitTextFillColor:

        case PropertyID::BlockSize:

        case PropertyID::CornerEndStartShape:

        case PropertyID::BorderBlockEndStyle:

        case PropertyID::AnchorScope:

        case PropertyID::BorderTopLeftRadius:

        case PropertyID::Position:

        case PropertyID::Fill:

        case PropertyID::InsetInlineEnd:

        case PropertyID::CornerEndEndShape:

        case PropertyID::BorderInlineStyle:

        case PropertyID::CornerBottomLeftShape:

        case PropertyID::MathShift:

        case PropertyID::Quotes:

        case PropertyID::BorderBlockColor:

        case PropertyID::ObjectPosition:

        case PropertyID::JustifyItems:

        case PropertyID::StrokeDasharray:

        case PropertyID::ClipPath:

        case PropertyID::TransitionTimingFunction:

        case PropertyID::BorderImageRepeat:

        case PropertyID::BorderEndEndRadius:

        case PropertyID::ContainerType:

        case PropertyID::JustifySelf:

        case PropertyID::FontWidth:

        case PropertyID::InsetBlockEnd:

        case PropertyID::BackgroundRepeat:

        case PropertyID::BorderBlockEndColor:

        case PropertyID::BoxShadow:

        case PropertyID::Clear:

        case PropertyID::ColumnHeight:

        case PropertyID::BorderTopRightRadius:

        case PropertyID::FlexShrink:

        case PropertyID::TextUnderlineOffset:

        case PropertyID::TextWrapStyle:

        case PropertyID::StrokeLinecap:

        case PropertyID::BorderBlockWidth:

        case PropertyID::BorderTopColor:

        case PropertyID::ScrollbarGutter:

        case PropertyID::OutlineOffset:

        case PropertyID::PaintOrder:

        case PropertyID::FontVariantPosition:

        case PropertyID::TextIndent:

        case PropertyID::CornerStartStartShape:

        case PropertyID::MaxBlockSize:

        case PropertyID::AspectRatio:

        case PropertyID::FillRule:

        case PropertyID::OverflowClipMarginBottom:

        case PropertyID::OverflowInline:

        case PropertyID::OverflowY:

        case PropertyID::AnimationDirection:

        case PropertyID::BorderBlockEndWidth:

        case PropertyID::ColorScheme:

        case PropertyID::TextRendering:

        case PropertyID::InsetInlineStart:

        case PropertyID::FloodColor:

        case PropertyID::FontOpticalSizing:

        case PropertyID::Filter:

        case PropertyID::GridColumnEnd:

        case PropertyID::TabSize:

        case PropertyID::GridRowStart:

        case PropertyID::FontStyle:

        case PropertyID::PaddingBottom:

        case PropertyID::OverflowClipMarginRight:

        case PropertyID::PaddingTop:

        case PropertyID::Perspective:

        case PropertyID::MathDepth:

        case PropertyID::BorderInlineEndColor:

        case PropertyID::JustifyContent:

        case PropertyID::BorderEndStartRadius:

        case PropertyID::BorderImageSource:

        case PropertyID::GridTemplateAreas:

        case PropertyID::MaskImage:

        case PropertyID::MaskClip:

        case PropertyID::MaskRepeat:

        case PropertyID::Resize:

        case PropertyID::GridRowEnd:

        case PropertyID::FlexDirection:

        case PropertyID::FontVariationSettings:

        case PropertyID::R:

        case PropertyID::ScrollbarWidth:

        case PropertyID::FontFamily:

        case PropertyID::FontVariantEastAsian:

        case PropertyID::ShapeImageThreshold:

        case PropertyID::X:

        case PropertyID::MinInlineSize:

        case PropertyID::Appearance:

        case PropertyID::TextAnchor:

        case PropertyID::PositionTryOrder:

        case PropertyID::MarginBlockEnd:

        case PropertyID::StrokeWidth:

        case PropertyID::ShapeOutside:

        case PropertyID::OverflowClipMarginBlockStart:

        case PropertyID::StopOpacity:

        case PropertyID::Stroke:

        case PropertyID::BackgroundClip:

        case PropertyID::MarginInlineStart:

        case PropertyID::VerticalAlign:

        case PropertyID::TextDecorationThickness:

        case PropertyID::BorderImageWidth:

        case PropertyID::FlexBasis:

        case PropertyID::MarginRight:

            return true;
        default:
            return false;
        }
}

Vector<PropertyID> const& shorthands_for_longhand(PropertyID property_id)
{
    switch (property_id) {

    case PropertyID::GridColumnStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::GridArea, PropertyID::GridColumn };
        return shorthands;
    }
    case PropertyID::WordSpacing: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PositionArea: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderInlineWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderInline };
        return shorthands;
    }
    case PropertyID::TableLayout: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FillOpacity: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderLeftColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderColor, PropertyID::BorderLeft };
        return shorthands;
    }
    case PropertyID::OverflowX: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Overflow };
        return shorthands;
    }
    case PropertyID::TransformOrigin: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Orphans: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontLanguageOverride: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::AnchorName: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariantLigatures: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::OverflowWrap: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundPositionY: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background, PropertyID::BackgroundPosition };
        return shorthands;
    }
    case PropertyID::BorderBlockStartColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockStart, PropertyID::BorderBlockColor };
        return shorthands;
    }
    case PropertyID::PaddingRight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Padding };
        return shorthands;
    }
    case PropertyID::MaxHeight: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Isolation: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundBlendMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariantEmoji: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::Cy: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ShapeRendering: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderStartStartRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MaskPosition: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::Rotate: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextDecorationLine: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::TextDecoration };
        return shorthands;
    }
    case PropertyID::ColumnGap: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Gap };
        return shorthands;
    }
    case PropertyID::FlexWrap: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::FlexFlow };
        return shorthands;
    }
    case PropertyID::PaddingLeft: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Padding };
        return shorthands;
    }
    case PropertyID::BorderInlineColor: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderInline };
        return shorthands;
    }
    case PropertyID::BorderCollapse: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowBlock: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderColor: {
        static Vector<PropertyID> shorthands = { PropertyID::Border };
        return shorthands;
    }
    case PropertyID::Cursor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::WordBreak: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::Float: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::MarginInlineEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::MarginInline };
        return shorthands;
    }
    case PropertyID::BackgroundOrigin: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::BorderStartEndRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderRightStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderStyle, PropertyID::BorderRight };
        return shorthands;
    }
    case PropertyID::ScrollbarColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ViewTimelineInset: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ViewTimeline };
        return shorthands;
    }
    case PropertyID::Left: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Inset };
        return shorthands;
    }
    case PropertyID::TouchAction: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderBlockStartStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockStart, PropertyID::BorderBlockStyle };
        return shorthands;
    }
    case PropertyID::Display: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Order: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::AnimationDuration: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::ImageRendering: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::InsetBlockStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::InsetBlock };
        return shorthands;
    }
    case PropertyID::MixBlendMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextOverflow: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MaskType: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariantAlternates: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::BorderImage: {
        static Vector<PropertyID> shorthands = { PropertyID::Border };
        return shorthands;
    }
    case PropertyID::AnimationTimingFunction: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::TransitionDelay: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Transition };
        return shorthands;
    }
    case PropertyID::ColumnWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Columns };
        return shorthands;
    }
    case PropertyID::BorderBottomStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderStyle, PropertyID::BorderBottom };
        return shorthands;
    }
    case PropertyID::AnimationFillMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::ClipRule: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MarginTop: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Margin };
        return shorthands;
    }
    case PropertyID::AccentColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CornerTopRightShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerShape, PropertyID::CornerRightShape, PropertyID::CornerTopShape };
        return shorthands;
    }
    case PropertyID::CornerTopLeftShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerShape, PropertyID::CornerLeftShape, PropertyID::CornerTopShape };
        return shorthands;
    }
    case PropertyID::BackgroundPositionX: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background, PropertyID::BackgroundPosition };
        return shorthands;
    }
    case PropertyID::Ry: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TransitionProperty: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Transition };
        return shorthands;
    }
    case PropertyID::AnimationTimeline: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::MaxWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ColumnSpan: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PointerEvents: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::EmptyCells: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TransitionBehavior: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Transition };
        return shorthands;
    }
    case PropertyID::FontSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::BorderSpacing: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ViewTimelineName: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ViewTimeline };
        return shorthands;
    }
    case PropertyID::BorderBottomColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderColor, PropertyID::BorderBottom };
        return shorthands;
    }
    case PropertyID::CounterReset: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MaskComposite: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::FlexGrow: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Flex };
        return shorthands;
    }
    case PropertyID::ListStyleImage: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ListStyle };
        return shorthands;
    }
    case PropertyID::Content: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderBottomWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderWidth, PropertyID::BorderBottom };
        return shorthands;
    }
    case PropertyID::StrokeLinejoin: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::RowGap: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Gap };
        return shorthands;
    }
    case PropertyID::Cx: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderImageOutset: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderImage };
        return shorthands;
    }
    case PropertyID::BorderInlineEndWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineEnd, PropertyID::BorderInlineWidth };
        return shorthands;
    }
    case PropertyID::GridAutoRows: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid };
        return shorthands;
    }
    case PropertyID::MaxInlineSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CounterIncrement: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::StrokeMiterlimit: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MaskOrigin: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::MinWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ShapeMargin: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MathStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderLeftWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderWidth, PropertyID::BorderLeft };
        return shorthands;
    }
    case PropertyID::Top: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Inset };
        return shorthands;
    }
    case PropertyID::TransitionDuration: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Transition };
        return shorthands;
    }
    case PropertyID::BorderTopWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderWidth, PropertyID::BorderTop };
        return shorthands;
    }
    case PropertyID::MinHeight: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PositionTryFallbacks: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Width: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextShadow: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OutlineStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Outline };
        return shorthands;
    }
    case PropertyID::BorderWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::Border };
        return shorthands;
    }
    case PropertyID::BorderInlineStartWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineStart, PropertyID::BorderInlineWidth };
        return shorthands;
    }
    case PropertyID::AnimationComposition: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextAlign: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ListStylePosition: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ListStyle };
        return shorthands;
    }
    case PropertyID::MarginBottom: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Margin };
        return shorthands;
    }
    case PropertyID::FontWeight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::StrokeOpacity: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ViewTimelineAxis: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ViewTimeline };
        return shorthands;
    }
    case PropertyID::Bottom: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Inset };
        return shorthands;
    }
    case PropertyID::TextTransform: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderLeftStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderStyle, PropertyID::BorderLeft };
        return shorthands;
    }
    case PropertyID::FontFeatureSettings: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::StopColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::AnimationPlayState: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::InlineSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::Border };
        return shorthands;
    }
    case PropertyID::AnimationDelay: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::BackgroundAttachment: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::UserSelect: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::WritingMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BoxSizing: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::LineHeight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::PositionVisibility: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderBottomRightRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderRadius };
        return shorthands;
    }
    case PropertyID::BackgroundPosition: {
        static Vector<PropertyID> shorthands = { PropertyID::Background };
        return shorthands;
    }
    case PropertyID::Scale: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundImage: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::AlignItems: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceItems };
        return shorthands;
    }
    case PropertyID::AnimationIterationCount: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::BorderBlockStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderBlock };
        return shorthands;
    }
    case PropertyID::CornerStartEndShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerBlockStartShape, PropertyID::CornerInlineEndShape };
        return shorthands;
    }
    case PropertyID::AlignContent: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceContent };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginInlineStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMarginInline };
        return shorthands;
    }
    case PropertyID::BorderRightColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderColor, PropertyID::BorderRight };
        return shorthands;
    }
    case PropertyID::FontKerning: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::MarginLeft: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Margin };
        return shorthands;
    }
    case PropertyID::ObjectFit: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::WhiteSpaceTrim: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::WhiteSpace };
        return shorthands;
    }
    case PropertyID::MarginBlockStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::MarginBlock };
        return shorthands;
    }
    case PropertyID::BackdropFilter: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MaskMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::Rx: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextDecorationStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::TextDecoration };
        return shorthands;
    }
    case PropertyID::BorderBottomLeftRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderRadius };
        return shorthands;
    }
    case PropertyID::ScrollTimelineAxis: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ScrollTimeline };
        return shorthands;
    }
    case PropertyID::LetterSpacing: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::GridTemplateRows: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid, PropertyID::GridTemplate };
        return shorthands;
    }
    case PropertyID::GridAutoFlow: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid };
        return shorthands;
    }
    case PropertyID::TextUnderlinePosition: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TimelineScope: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariant: {
        static Vector<PropertyID> shorthands = { PropertyID::Font };
        return shorthands;
    }
    case PropertyID::CaretColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ZIndex: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginBlockEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMarginBlock };
        return shorthands;
    }
    case PropertyID::GridAutoColumns: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid };
        return shorthands;
    }
    case PropertyID::ScrollTimelineName: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ScrollTimeline };
        return shorthands;
    }
    case PropertyID::PerspectiveOrigin: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PaddingInlineEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PaddingInline };
        return shorthands;
    }
    case PropertyID::Visibility: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TransformBox: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Y: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginLeft: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMargin };
        return shorthands;
    }
    case PropertyID::TextDecorationColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::TextDecoration };
        return shorthands;
    }
    case PropertyID::ColumnCount: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Columns };
        return shorthands;
    }
    case PropertyID::WhiteSpaceCollapse: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::WhiteSpace };
        return shorthands;
    }
    case PropertyID::MinBlockSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TransformStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CaptionSide: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::WillChange: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderInlineEndStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineEnd, PropertyID::BorderInlineStyle };
        return shorthands;
    }
    case PropertyID::CounterSet: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::AlignSelf: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceSelf };
        return shorthands;
    }
    case PropertyID::AnimationName: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::Translate: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderInlineStartColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineStart, PropertyID::BorderInlineColor };
        return shorthands;
    }
    case PropertyID::OutlineColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Outline };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginInlineEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMarginInline };
        return shorthands;
    }
    case PropertyID::PositionAnchor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ContentVisibility: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Opacity: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Color: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Clip: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariantNumeric: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::MaskSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::Transform: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ColorInterpolation: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PaddingBlockStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PaddingBlock };
        return shorthands;
    }
    case PropertyID::PaddingInlineStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PaddingInline };
        return shorthands;
    }
    case PropertyID::BorderInlineStartStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineStart, PropertyID::BorderInlineStyle };
        return shorthands;
    }
    case PropertyID::Height: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CornerBottomRightShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerShape, PropertyID::CornerBottomShape, PropertyID::CornerRightShape };
        return shorthands;
    }
    case PropertyID::FontVariantCaps: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::Right: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Inset };
        return shorthands;
    }
    case PropertyID::Widows: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FloodOpacity: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ListStyleType: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::ListStyle };
        return shorthands;
    }
    case PropertyID::DominantBaseline: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::GridTemplateColumns: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid, PropertyID::GridTemplate };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginTop: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMargin };
        return shorthands;
    }
    case PropertyID::BorderImageSlice: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderImage };
        return shorthands;
    }
    case PropertyID::OutlineWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Outline };
        return shorthands;
    }
    case PropertyID::BorderBlockStartWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockStart, PropertyID::BorderBlockWidth };
        return shorthands;
    }
    case PropertyID::BorderRightWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderWidth, PropertyID::BorderRight };
        return shorthands;
    }
    case PropertyID::BorderTopStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderStyle, PropertyID::BorderTop };
        return shorthands;
    }
    case PropertyID::TextJustify: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PaddingBlockEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PaddingBlock };
        return shorthands;
    }
    case PropertyID::Contain: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::StrokeDashoffset: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ViewTransitionName: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextWrapMode: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::WhiteSpace, PropertyID::TextWrap };
        return shorthands;
    }
    case PropertyID::WebkitTextFillColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BlockSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CornerEndStartShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerBlockEndShape, PropertyID::CornerInlineStartShape };
        return shorthands;
    }
    case PropertyID::BorderBlockEndStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockEnd, PropertyID::BorderBlockStyle };
        return shorthands;
    }
    case PropertyID::AnchorScope: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderTopLeftRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderRadius };
        return shorthands;
    }
    case PropertyID::Position: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Fill: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::InsetInlineEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::InsetInline };
        return shorthands;
    }
    case PropertyID::CornerEndEndShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerBlockEndShape, PropertyID::CornerInlineEndShape };
        return shorthands;
    }
    case PropertyID::BorderInlineStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderInline };
        return shorthands;
    }
    case PropertyID::CornerBottomLeftShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerShape, PropertyID::CornerBottomShape, PropertyID::CornerLeftShape };
        return shorthands;
    }
    case PropertyID::MathShift: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Quotes: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderBlockColor: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderBlock };
        return shorthands;
    }
    case PropertyID::ObjectPosition: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::JustifyItems: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceItems };
        return shorthands;
    }
    case PropertyID::StrokeDasharray: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ClipPath: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TransitionTimingFunction: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Transition };
        return shorthands;
    }
    case PropertyID::BorderImageRepeat: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderImage };
        return shorthands;
    }
    case PropertyID::BorderEndEndRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ContainerType: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::JustifySelf: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceSelf };
        return shorthands;
    }
    case PropertyID::FontWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::InsetBlockEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::InsetBlock };
        return shorthands;
    }
    case PropertyID::BackgroundRepeat: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::BorderBlockEndColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockEnd, PropertyID::BorderBlockColor };
        return shorthands;
    }
    case PropertyID::BoxShadow: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Clear: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ColumnHeight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Columns };
        return shorthands;
    }
    case PropertyID::BorderTopRightRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderRadius };
        return shorthands;
    }
    case PropertyID::FlexShrink: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Flex };
        return shorthands;
    }
    case PropertyID::TextUnderlineOffset: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextWrapStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::TextWrap };
        return shorthands;
    }
    case PropertyID::StrokeLinecap: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderBlockWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::BorderBlock };
        return shorthands;
    }
    case PropertyID::BorderTopColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderColor, PropertyID::BorderTop };
        return shorthands;
    }
    case PropertyID::ScrollbarGutter: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OutlineOffset: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PaintOrder: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontVariantPosition: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::TextIndent: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::CornerStartStartShape: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::CornerBlockStartShape, PropertyID::CornerInlineStartShape };
        return shorthands;
    }
    case PropertyID::MaxBlockSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::AspectRatio: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FillRule: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginBottom: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMargin };
        return shorthands;
    }
    case PropertyID::OverflowInline: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowY: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Overflow };
        return shorthands;
    }
    case PropertyID::AnimationDirection: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Animation };
        return shorthands;
    }
    case PropertyID::BorderBlockEndWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderBlock, PropertyID::BorderBlockEnd, PropertyID::BorderBlockWidth };
        return shorthands;
    }
    case PropertyID::ColorScheme: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextRendering: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::InsetInlineStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::InsetInline };
        return shorthands;
    }
    case PropertyID::FloodColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontOpticalSizing: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::Filter: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::GridColumnEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::GridArea, PropertyID::GridColumn };
        return shorthands;
    }
    case PropertyID::TabSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::GridRowStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::GridArea, PropertyID::GridRow };
        return shorthands;
    }
    case PropertyID::FontStyle: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::PaddingBottom: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Padding };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginRight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMargin };
        return shorthands;
    }
    case PropertyID::PaddingTop: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Padding };
        return shorthands;
    }
    case PropertyID::Perspective: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MathDepth: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderInlineEndColor: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::BorderInline, PropertyID::BorderInlineEnd, PropertyID::BorderInlineColor };
        return shorthands;
    }
    case PropertyID::JustifyContent: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::PlaceContent };
        return shorthands;
    }
    case PropertyID::BorderEndStartRadius: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BorderImageSource: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderImage };
        return shorthands;
    }
    case PropertyID::GridTemplateAreas: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Grid, PropertyID::GridTemplate };
        return shorthands;
    }
    case PropertyID::MaskImage: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::MaskClip: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::MaskRepeat: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Mask };
        return shorthands;
    }
    case PropertyID::Resize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::GridRowEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::GridArea, PropertyID::GridRow };
        return shorthands;
    }
    case PropertyID::FlexDirection: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::FlexFlow };
        return shorthands;
    }
    case PropertyID::FontVariationSettings: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::R: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ScrollbarWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::FontFamily: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font };
        return shorthands;
    }
    case PropertyID::FontVariantEastAsian: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Font, PropertyID::FontVariant };
        return shorthands;
    }
    case PropertyID::ShapeImageThreshold: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::X: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MinInlineSize: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Appearance: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextAnchor: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::PositionTryOrder: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::MarginBlockEnd: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::MarginBlock };
        return shorthands;
    }
    case PropertyID::StrokeWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::ShapeOutside: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::OverflowClipMarginBlockStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::OverflowClipMarginBlock };
        return shorthands;
    }
    case PropertyID::StopOpacity: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::Stroke: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::BackgroundClip: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Background };
        return shorthands;
    }
    case PropertyID::MarginInlineStart: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::MarginInline };
        return shorthands;
    }
    case PropertyID::VerticalAlign: {
        static Vector<PropertyID> shorthands = { PropertyID::All };
        return shorthands;
    }
    case PropertyID::TextDecorationThickness: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::TextDecoration };
        return shorthands;
    }
    case PropertyID::BorderImageWidth: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Border, PropertyID::BorderImage };
        return shorthands;
    }
    case PropertyID::FlexBasis: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Flex };
        return shorthands;
    }
    case PropertyID::MarginRight: {
        static Vector<PropertyID> shorthands = { PropertyID::All, PropertyID::Margin };
        return shorthands;
    }
    default: {
        static Vector<PropertyID> empty_shorthands;
        return empty_shorthands;
    }
    }
}

Vector<PropertyID> const& property_computation_order() {
    static Vector<PropertyID> order = {
        PropertyID::MathDepth,
        PropertyID::FontFamily,
        PropertyID::FontFeatureSettings,
        PropertyID::FontKerning,
        PropertyID::FontOpticalSizing,
        PropertyID::FontSize,
        PropertyID::FontStyle,
        PropertyID::FontVariantAlternates,
        PropertyID::FontVariantCaps,
        PropertyID::FontVariantEastAsian,
        PropertyID::FontVariantEmoji,
        PropertyID::FontVariantLigatures,
        PropertyID::FontVariantNumeric,
        PropertyID::FontVariantPosition,
        PropertyID::FontVariationSettings,
        PropertyID::FontWeight,
        PropertyID::FontWidth,
        PropertyID::TextRendering,
        PropertyID::LineHeight,
        PropertyID::ColorScheme,
        PropertyID::BackgroundImage,
        PropertyID::WebkitTextFillColor,
        PropertyID::AccentColor,
        PropertyID::AlignContent,
        PropertyID::AlignItems,
        PropertyID::AlignSelf,
        PropertyID::AnchorName,
        PropertyID::AnchorScope,
        PropertyID::AnimationComposition,
        PropertyID::AnimationDelay,
        PropertyID::AnimationDirection,
        PropertyID::AnimationDuration,
        PropertyID::AnimationFillMode,
        PropertyID::AnimationIterationCount,
        PropertyID::AnimationName,
        PropertyID::AnimationPlayState,
        PropertyID::AnimationTimeline,
        PropertyID::AnimationTimingFunction,
        PropertyID::Appearance,
        PropertyID::AspectRatio,
        PropertyID::BackdropFilter,
        PropertyID::BackgroundAttachment,
        PropertyID::BackgroundBlendMode,
        PropertyID::BackgroundClip,
        PropertyID::BackgroundColor,
        PropertyID::BackgroundOrigin,
        PropertyID::BackgroundPositionX,
        PropertyID::BackgroundPositionY,
        PropertyID::BackgroundRepeat,
        PropertyID::BackgroundSize,
        PropertyID::BlockSize,
        PropertyID::BorderBlockEndColor,
        PropertyID::BorderBlockEndStyle,
        PropertyID::BorderBlockEndWidth,
        PropertyID::BorderBlockStartColor,
        PropertyID::BorderBlockStartStyle,
        PropertyID::BorderBlockStartWidth,
        PropertyID::BorderBottomColor,
        PropertyID::BorderBottomLeftRadius,
        PropertyID::BorderBottomRightRadius,
        PropertyID::BorderBottomStyle,
        PropertyID::BorderBottomWidth,
        PropertyID::BorderCollapse,
        PropertyID::BorderEndEndRadius,
        PropertyID::BorderEndStartRadius,
        PropertyID::BorderImageOutset,
        PropertyID::BorderImageRepeat,
        PropertyID::BorderImageSlice,
        PropertyID::BorderImageSource,
        PropertyID::BorderImageWidth,
        PropertyID::BorderInlineEndColor,
        PropertyID::BorderInlineEndStyle,
        PropertyID::BorderInlineEndWidth,
        PropertyID::BorderInlineStartColor,
        PropertyID::BorderInlineStartStyle,
        PropertyID::BorderInlineStartWidth,
        PropertyID::BorderLeftColor,
        PropertyID::BorderLeftStyle,
        PropertyID::BorderLeftWidth,
        PropertyID::BorderRightColor,
        PropertyID::BorderRightStyle,
        PropertyID::BorderRightWidth,
        PropertyID::BorderSpacing,
        PropertyID::BorderStartEndRadius,
        PropertyID::BorderStartStartRadius,
        PropertyID::BorderTopColor,
        PropertyID::BorderTopLeftRadius,
        PropertyID::BorderTopRightRadius,
        PropertyID::BorderTopStyle,
        PropertyID::BorderTopWidth,
        PropertyID::Bottom,
        PropertyID::BoxShadow,
        PropertyID::BoxSizing,
        PropertyID::CaptionSide,
        PropertyID::CaretColor,
        PropertyID::Clear,
        PropertyID::Clip,
        PropertyID::ClipPath,
        PropertyID::ClipRule,
        PropertyID::Color,
        PropertyID::ColorInterpolation,
        PropertyID::ColumnCount,
        PropertyID::ColumnGap,
        PropertyID::ColumnHeight,
        PropertyID::ColumnSpan,
        PropertyID::ColumnWidth,
        PropertyID::Contain,
        PropertyID::ContainerType,
        PropertyID::Content,
        PropertyID::ContentVisibility,
        PropertyID::CornerBottomLeftShape,
        PropertyID::CornerBottomRightShape,
        PropertyID::CornerEndEndShape,
        PropertyID::CornerEndStartShape,
        PropertyID::CornerStartEndShape,
        PropertyID::CornerStartStartShape,
        PropertyID::CornerTopLeftShape,
        PropertyID::CornerTopRightShape,
        PropertyID::CounterIncrement,
        PropertyID::CounterReset,
        PropertyID::CounterSet,
        PropertyID::Cursor,
        PropertyID::Cx,
        PropertyID::Cy,
        PropertyID::Direction,
        PropertyID::Display,
        PropertyID::DominantBaseline,
        PropertyID::EmptyCells,
        PropertyID::Fill,
        PropertyID::FillOpacity,
        PropertyID::FillRule,
        PropertyID::Filter,
        PropertyID::FlexBasis,
        PropertyID::FlexDirection,
        PropertyID::FlexGrow,
        PropertyID::FlexShrink,
        PropertyID::FlexWrap,
        PropertyID::Float,
        PropertyID::FloodColor,
        PropertyID::FloodOpacity,
        PropertyID::FontLanguageOverride,
        PropertyID::GridAutoColumns,
        PropertyID::GridAutoFlow,
        PropertyID::GridAutoRows,
        PropertyID::GridColumnEnd,
        PropertyID::GridColumnStart,
        PropertyID::GridRowEnd,
        PropertyID::GridRowStart,
        PropertyID::GridTemplateAreas,
        PropertyID::GridTemplateColumns,
        PropertyID::GridTemplateRows,
        PropertyID::Height,
        PropertyID::ImageRendering,
        PropertyID::InlineSize,
        PropertyID::InsetBlockEnd,
        PropertyID::InsetBlockStart,
        PropertyID::InsetInlineEnd,
        PropertyID::InsetInlineStart,
        PropertyID::Isolation,
        PropertyID::JustifyContent,
        PropertyID::JustifyItems,
        PropertyID::JustifySelf,
        PropertyID::Left,
        PropertyID::LetterSpacing,
        PropertyID::ListStyleImage,
        PropertyID::ListStylePosition,
        PropertyID::ListStyleType,
        PropertyID::MarginBlockEnd,
        PropertyID::MarginBlockStart,
        PropertyID::MarginBottom,
        PropertyID::MarginInlineEnd,
        PropertyID::MarginInlineStart,
        PropertyID::MarginLeft,
        PropertyID::MarginRight,
        PropertyID::MarginTop,
        PropertyID::MaskClip,
        PropertyID::MaskComposite,
        PropertyID::MaskImage,
        PropertyID::MaskMode,
        PropertyID::MaskOrigin,
        PropertyID::MaskPosition,
        PropertyID::MaskRepeat,
        PropertyID::MaskSize,
        PropertyID::MaskType,
        PropertyID::MathShift,
        PropertyID::MathStyle,
        PropertyID::MaxBlockSize,
        PropertyID::MaxHeight,
        PropertyID::MaxInlineSize,
        PropertyID::MaxWidth,
        PropertyID::MinBlockSize,
        PropertyID::MinHeight,
        PropertyID::MinInlineSize,
        PropertyID::MinWidth,
        PropertyID::MixBlendMode,
        PropertyID::ObjectFit,
        PropertyID::ObjectPosition,
        PropertyID::Opacity,
        PropertyID::Order,
        PropertyID::Orphans,
        PropertyID::OutlineColor,
        PropertyID::OutlineOffset,
        PropertyID::OutlineStyle,
        PropertyID::OutlineWidth,
        PropertyID::OverflowBlock,
        PropertyID::OverflowClipMarginBlockEnd,
        PropertyID::OverflowClipMarginBlockStart,
        PropertyID::OverflowClipMarginBottom,
        PropertyID::OverflowClipMarginInlineEnd,
        PropertyID::OverflowClipMarginInlineStart,
        PropertyID::OverflowClipMarginLeft,
        PropertyID::OverflowClipMarginRight,
        PropertyID::OverflowClipMarginTop,
        PropertyID::OverflowInline,
        PropertyID::OverflowWrap,
        PropertyID::OverflowX,
        PropertyID::OverflowY,
        PropertyID::PaddingBlockEnd,
        PropertyID::PaddingBlockStart,
        PropertyID::PaddingBottom,
        PropertyID::PaddingInlineEnd,
        PropertyID::PaddingInlineStart,
        PropertyID::PaddingLeft,
        PropertyID::PaddingRight,
        PropertyID::PaddingTop,
        PropertyID::PaintOrder,
        PropertyID::Perspective,
        PropertyID::PerspectiveOrigin,
        PropertyID::PointerEvents,
        PropertyID::Position,
        PropertyID::PositionAnchor,
        PropertyID::PositionArea,
        PropertyID::PositionTryFallbacks,
        PropertyID::PositionTryOrder,
        PropertyID::PositionVisibility,
        PropertyID::Quotes,
        PropertyID::R,
        PropertyID::Resize,
        PropertyID::Right,
        PropertyID::Rotate,
        PropertyID::RowGap,
        PropertyID::Rx,
        PropertyID::Ry,
        PropertyID::Scale,
        PropertyID::ScrollTimelineAxis,
        PropertyID::ScrollTimelineName,
        PropertyID::ScrollbarColor,
        PropertyID::ScrollbarGutter,
        PropertyID::ScrollbarWidth,
        PropertyID::ShapeImageThreshold,
        PropertyID::ShapeMargin,
        PropertyID::ShapeOutside,
        PropertyID::ShapeRendering,
        PropertyID::StopColor,
        PropertyID::StopOpacity,
        PropertyID::Stroke,
        PropertyID::StrokeDasharray,
        PropertyID::StrokeDashoffset,
        PropertyID::StrokeLinecap,
        PropertyID::StrokeLinejoin,
        PropertyID::StrokeMiterlimit,
        PropertyID::StrokeOpacity,
        PropertyID::StrokeWidth,
        PropertyID::TabSize,
        PropertyID::TableLayout,
        PropertyID::TextAlign,
        PropertyID::TextAnchor,
        PropertyID::TextDecorationColor,
        PropertyID::TextDecorationLine,
        PropertyID::TextDecorationStyle,
        PropertyID::TextDecorationThickness,
        PropertyID::TextIndent,
        PropertyID::TextJustify,
        PropertyID::TextOverflow,
        PropertyID::TextShadow,
        PropertyID::TextTransform,
        PropertyID::TextUnderlineOffset,
        PropertyID::TextUnderlinePosition,
        PropertyID::TextWrapMode,
        PropertyID::TextWrapStyle,
        PropertyID::TimelineScope,
        PropertyID::Top,
        PropertyID::TouchAction,
        PropertyID::Transform,
        PropertyID::TransformBox,
        PropertyID::TransformOrigin,
        PropertyID::TransformStyle,
        PropertyID::TransitionBehavior,
        PropertyID::TransitionDelay,
        PropertyID::TransitionDuration,
        PropertyID::TransitionProperty,
        PropertyID::TransitionTimingFunction,
        PropertyID::Translate,
        PropertyID::UnicodeBidi,
        PropertyID::UserSelect,
        PropertyID::VerticalAlign,
        PropertyID::ViewTimelineAxis,
        PropertyID::ViewTimelineInset,
        PropertyID::ViewTimelineName,
        PropertyID::ViewTransitionName,
        PropertyID::Visibility,
        PropertyID::WhiteSpaceCollapse,
        PropertyID::WhiteSpaceTrim,
        PropertyID::Widows,
        PropertyID::Width,
        PropertyID::WillChange,
        PropertyID::WordBreak,
        PropertyID::WordSpacing,
        PropertyID::WritingMode,
        PropertyID::X,
        PropertyID::Y,
        PropertyID::ZIndex,

    };

    return order;
}

bool property_is_positional_value_list_shorthand(PropertyID property_id)
{
    switch (property_id)
    {

    case PropertyID::BorderBlockColor:
            
    case PropertyID::BorderBlockStyle:
            
    case PropertyID::BorderBlockWidth:
            
    case PropertyID::BorderColor:
            
    case PropertyID::BorderInlineColor:
            
    case PropertyID::BorderInlineStyle:
            
    case PropertyID::BorderInlineWidth:
            
    case PropertyID::BorderStyle:
            
    case PropertyID::BorderWidth:
            
    case PropertyID::CornerBlockEndShape:
            
    case PropertyID::CornerBlockStartShape:
            
    case PropertyID::CornerBottomShape:
            
    case PropertyID::CornerInlineEndShape:
            
    case PropertyID::CornerInlineStartShape:
            
    case PropertyID::CornerLeftShape:
            
    case PropertyID::CornerRightShape:
            
    case PropertyID::CornerShape:
            
    case PropertyID::CornerTopShape:
            
    case PropertyID::Gap:
            
    case PropertyID::Inset:
            
    case PropertyID::InsetBlock:
            
    case PropertyID::InsetInline:
            
    case PropertyID::Margin:
            
    case PropertyID::MarginBlock:
            
    case PropertyID::MarginInline:
            
    case PropertyID::Overflow:
            
    case PropertyID::Padding:
            
    case PropertyID::PaddingBlock:
            
    case PropertyID::PaddingInline:
            
        return true;
    default:
        return false;
    }
}

bool property_requires_computation_with_inherited_value(PropertyID property_id)
{
    switch(property_id) {
        case PropertyID::BackgroundAttachment:
    case PropertyID::BackgroundBlendMode:
    case PropertyID::BackgroundClip:
    case PropertyID::BackgroundOrigin:
    case PropertyID::BackgroundPositionX:
    case PropertyID::BackgroundPositionY:
    case PropertyID::BackgroundRepeat:
    case PropertyID::BackgroundSize:

        return true;
    default:
        return false;
    }
}

bool property_requires_computation_with_initial_value(PropertyID property_id)
{
    switch(property_id) {
        case PropertyID::BackgroundAttachment:
    case PropertyID::BackgroundBlendMode:
    case PropertyID::BackgroundClip:
    case PropertyID::BackgroundColor:
    case PropertyID::BackgroundOrigin:
    case PropertyID::BackgroundPositionX:
    case PropertyID::BackgroundPositionY:
    case PropertyID::BackgroundRepeat:
    case PropertyID::BackgroundSize:
    case PropertyID::BorderBlockEndWidth:
    case PropertyID::BorderBlockStartWidth:
    case PropertyID::BorderBottomWidth:
    case PropertyID::BorderInlineEndWidth:
    case PropertyID::BorderInlineStartWidth:
    case PropertyID::BorderLeftWidth:
    case PropertyID::BorderRightWidth:
    case PropertyID::BorderTopWidth:
    case PropertyID::Color:
    case PropertyID::CornerBottomLeftShape:
    case PropertyID::CornerBottomRightShape:
    case PropertyID::CornerEndEndShape:
    case PropertyID::CornerEndStartShape:
    case PropertyID::CornerStartEndShape:
    case PropertyID::CornerStartStartShape:
    case PropertyID::CornerTopLeftShape:
    case PropertyID::CornerTopRightShape:
    case PropertyID::Fill:
    case PropertyID::FloodColor:
    case PropertyID::FontSize:
    case PropertyID::FontWeight:
    case PropertyID::FontWidth:
    case PropertyID::LetterSpacing:
    case PropertyID::OutlineWidth:
    case PropertyID::StopColor:
    case PropertyID::WordSpacing:

        return true;
    default:
        return false;
    }
}

bool property_requires_computation_with_cascaded_value(PropertyID property_id)
{
    switch(property_id) {
        case PropertyID::WebkitTextFillColor:
    case PropertyID::AccentColor:
    case PropertyID::AnimationDelay:
    case PropertyID::AnimationDuration:
    case PropertyID::AnimationIterationCount:
    case PropertyID::AnimationName:
    case PropertyID::AnimationTimeline:
    case PropertyID::AnimationTimingFunction:
    case PropertyID::AspectRatio:
    case PropertyID::BackdropFilter:
    case PropertyID::BackgroundAttachment:
    case PropertyID::BackgroundBlendMode:
    case PropertyID::BackgroundClip:
    case PropertyID::BackgroundColor:
    case PropertyID::BackgroundImage:
    case PropertyID::BackgroundOrigin:
    case PropertyID::BackgroundPositionX:
    case PropertyID::BackgroundPositionY:
    case PropertyID::BackgroundRepeat:
    case PropertyID::BackgroundSize:
    case PropertyID::BlockSize:
    case PropertyID::BorderBlockEndColor:
    case PropertyID::BorderBlockEndWidth:
    case PropertyID::BorderBlockStartColor:
    case PropertyID::BorderBlockStartWidth:
    case PropertyID::BorderBottomColor:
    case PropertyID::BorderBottomLeftRadius:
    case PropertyID::BorderBottomRightRadius:
    case PropertyID::BorderBottomWidth:
    case PropertyID::BorderEndEndRadius:
    case PropertyID::BorderEndStartRadius:
    case PropertyID::BorderImageOutset:
    case PropertyID::BorderImageSlice:
    case PropertyID::BorderImageSource:
    case PropertyID::BorderImageWidth:
    case PropertyID::BorderInlineEndColor:
    case PropertyID::BorderInlineEndWidth:
    case PropertyID::BorderInlineStartColor:
    case PropertyID::BorderInlineStartWidth:
    case PropertyID::BorderLeftColor:
    case PropertyID::BorderLeftWidth:
    case PropertyID::BorderRightColor:
    case PropertyID::BorderRightWidth:
    case PropertyID::BorderSpacing:
    case PropertyID::BorderStartEndRadius:
    case PropertyID::BorderStartStartRadius:
    case PropertyID::BorderTopColor:
    case PropertyID::BorderTopLeftRadius:
    case PropertyID::BorderTopRightRadius:
    case PropertyID::BorderTopWidth:
    case PropertyID::Bottom:
    case PropertyID::BoxShadow:
    case PropertyID::CaretColor:
    case PropertyID::Clip:
    case PropertyID::ClipPath:
    case PropertyID::Color:
    case PropertyID::ColumnCount:
    case PropertyID::ColumnGap:
    case PropertyID::ColumnHeight:
    case PropertyID::ColumnWidth:
    case PropertyID::Content:
    case PropertyID::CornerBottomLeftShape:
    case PropertyID::CornerBottomRightShape:
    case PropertyID::CornerEndEndShape:
    case PropertyID::CornerEndStartShape:
    case PropertyID::CornerStartEndShape:
    case PropertyID::CornerStartStartShape:
    case PropertyID::CornerTopLeftShape:
    case PropertyID::CornerTopRightShape:
    case PropertyID::CounterIncrement:
    case PropertyID::CounterReset:
    case PropertyID::CounterSet:
    case PropertyID::Cursor:
    case PropertyID::Cx:
    case PropertyID::Cy:
    case PropertyID::Fill:
    case PropertyID::FillOpacity:
    case PropertyID::Filter:
    case PropertyID::FlexBasis:
    case PropertyID::FlexGrow:
    case PropertyID::FlexShrink:
    case PropertyID::FloodColor:
    case PropertyID::FloodOpacity:
    case PropertyID::FontFeatureSettings:
    case PropertyID::FontSize:
    case PropertyID::FontStyle:
    case PropertyID::FontVariationSettings:
    case PropertyID::FontWeight:
    case PropertyID::FontWidth:
    case PropertyID::GridAutoColumns:
    case PropertyID::GridAutoRows:
    case PropertyID::GridTemplateColumns:
    case PropertyID::GridTemplateRows:
    case PropertyID::Height:
    case PropertyID::InlineSize:
    case PropertyID::InsetBlockEnd:
    case PropertyID::InsetBlockStart:
    case PropertyID::InsetInlineEnd:
    case PropertyID::InsetInlineStart:
    case PropertyID::Left:
    case PropertyID::LetterSpacing:
    case PropertyID::LineHeight:
    case PropertyID::ListStyleImage:
    case PropertyID::MarginBlockEnd:
    case PropertyID::MarginBlockStart:
    case PropertyID::MarginBottom:
    case PropertyID::MarginInlineEnd:
    case PropertyID::MarginInlineStart:
    case PropertyID::MarginLeft:
    case PropertyID::MarginRight:
    case PropertyID::MarginTop:
    case PropertyID::MaskImage:
    case PropertyID::MaskPosition:
    case PropertyID::MaskSize:
    case PropertyID::MathDepth:
    case PropertyID::MaxBlockSize:
    case PropertyID::MaxHeight:
    case PropertyID::MaxInlineSize:
    case PropertyID::MaxWidth:
    case PropertyID::MinBlockSize:
    case PropertyID::MinHeight:
    case PropertyID::MinInlineSize:
    case PropertyID::MinWidth:
    case PropertyID::ObjectPosition:
    case PropertyID::Opacity:
    case PropertyID::Order:
    case PropertyID::Orphans:
    case PropertyID::OutlineColor:
    case PropertyID::OutlineOffset:
    case PropertyID::OutlineWidth:
    case PropertyID::OverflowClipMarginBlockEnd:
    case PropertyID::OverflowClipMarginBlockStart:
    case PropertyID::OverflowClipMarginBottom:
    case PropertyID::OverflowClipMarginInlineEnd:
    case PropertyID::OverflowClipMarginInlineStart:
    case PropertyID::OverflowClipMarginLeft:
    case PropertyID::OverflowClipMarginRight:
    case PropertyID::OverflowClipMarginTop:
    case PropertyID::PaddingBlockEnd:
    case PropertyID::PaddingBlockStart:
    case PropertyID::PaddingBottom:
    case PropertyID::PaddingInlineEnd:
    case PropertyID::PaddingInlineStart:
    case PropertyID::PaddingLeft:
    case PropertyID::PaddingRight:
    case PropertyID::PaddingTop:
    case PropertyID::Perspective:
    case PropertyID::PerspectiveOrigin:
    case PropertyID::PositionArea:
    case PropertyID::R:
    case PropertyID::Right:
    case PropertyID::Rotate:
    case PropertyID::RowGap:
    case PropertyID::Rx:
    case PropertyID::Ry:
    case PropertyID::Scale:
    case PropertyID::ScrollbarColor:
    case PropertyID::ShapeImageThreshold:
    case PropertyID::ShapeMargin:
    case PropertyID::ShapeOutside:
    case PropertyID::StopColor:
    case PropertyID::StopOpacity:
    case PropertyID::Stroke:
    case PropertyID::StrokeDasharray:
    case PropertyID::StrokeDashoffset:
    case PropertyID::StrokeMiterlimit:
    case PropertyID::StrokeOpacity:
    case PropertyID::StrokeWidth:
    case PropertyID::TabSize:
    case PropertyID::TextDecorationColor:
    case PropertyID::TextDecorationThickness:
    case PropertyID::TextIndent:
    case PropertyID::TextShadow:
    case PropertyID::TextUnderlineOffset:
    case PropertyID::Top:
    case PropertyID::Transform:
    case PropertyID::TransformOrigin:
    case PropertyID::TransitionDelay:
    case PropertyID::TransitionDuration:
    case PropertyID::TransitionTimingFunction:
    case PropertyID::Translate:
    case PropertyID::VerticalAlign:
    case PropertyID::ViewTimelineInset:
    case PropertyID::Widows:
    case PropertyID::Width:
    case PropertyID::WordSpacing:
    case PropertyID::X:
    case PropertyID::Y:
    case PropertyID::ZIndex:

        return true;
    default:
        return false;
    }
}

bool property_is_logical_alias(PropertyID property_id)
{
    switch(property_id) {

    case PropertyID::BlockSize:

    case PropertyID::BorderBlockEndColor:

    case PropertyID::BorderBlockEndStyle:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStartColor:

    case PropertyID::BorderBlockStartStyle:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderEndEndRadius:

    case PropertyID::BorderEndStartRadius:

    case PropertyID::BorderInlineEndColor:

    case PropertyID::BorderInlineEndStyle:

    case PropertyID::BorderInlineEndWidth:

    case PropertyID::BorderInlineStartColor:

    case PropertyID::BorderInlineStartStyle:

    case PropertyID::BorderInlineStartWidth:

    case PropertyID::BorderStartEndRadius:

    case PropertyID::BorderStartStartRadius:

    case PropertyID::CornerEndEndShape:

    case PropertyID::CornerEndStartShape:

    case PropertyID::CornerStartEndShape:

    case PropertyID::CornerStartStartShape:

    case PropertyID::InlineSize:

    case PropertyID::InsetBlockEnd:

    case PropertyID::InsetBlockStart:

    case PropertyID::InsetInlineEnd:

    case PropertyID::InsetInlineStart:

    case PropertyID::MarginBlockEnd:

    case PropertyID::MarginBlockStart:

    case PropertyID::MarginInlineEnd:

    case PropertyID::MarginInlineStart:

    case PropertyID::MaxBlockSize:

    case PropertyID::MaxInlineSize:

    case PropertyID::MinBlockSize:

    case PropertyID::MinInlineSize:

    case PropertyID::OverflowBlock:

    case PropertyID::OverflowClipMarginBlockEnd:

    case PropertyID::OverflowClipMarginBlockStart:

    case PropertyID::OverflowClipMarginInlineEnd:

    case PropertyID::OverflowClipMarginInlineStart:

    case PropertyID::OverflowInline:

    case PropertyID::PaddingBlockEnd:

    case PropertyID::PaddingBlockStart:

    case PropertyID::PaddingInlineEnd:

    case PropertyID::PaddingInlineStart:

        return true;
    default:
        return false;
    }
}

PropertyID map_logical_alias_to_physical_property(PropertyID property_id, LogicalAliasMappingContext const& mapping_context)
{
    // https://drafts.csswg.org/css-writing-modes-4/#logical-to-physical
    // FIXME: Note: The used direction depends on the computed writing-mode and text-orientation: in vertical writing
    //              modes, a text-orientation value of upright forces the used direction to ltr.
    auto used_direction = mapping_context.direction;
    switch(property_id) {

    case PropertyID::BlockSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::Height;
        return PropertyID::Width;

    case PropertyID::BorderBlockEndColor:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderBottomColor;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderLeftColor;
        return PropertyID::BorderRightColor;

    case PropertyID::BorderBlockEndStyle:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderBottomStyle;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderLeftStyle;
        return PropertyID::BorderRightStyle;

    case PropertyID::BorderBlockEndWidth:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderBottomWidth;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderLeftWidth;
        return PropertyID::BorderRightWidth;

    case PropertyID::BorderBlockStartColor:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderTopColor;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderRightColor;
        return PropertyID::BorderLeftColor;

    case PropertyID::BorderBlockStartStyle:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderTopStyle;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderRightStyle;
        return PropertyID::BorderLeftStyle;

    case PropertyID::BorderBlockStartWidth:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::BorderTopWidth;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::BorderRightWidth;
        return PropertyID::BorderLeftWidth;

    case PropertyID::BorderEndEndRadius:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomRightRadius;
            return PropertyID::BorderBottomLeftRadius;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomLeftRadius;
            return PropertyID::BorderTopLeftRadius;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomRightRadius;
            return PropertyID::BorderTopRightRadius;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderTopRightRadius;
        return PropertyID::BorderBottomRightRadius;

    case PropertyID::BorderEndStartRadius:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomLeftRadius;
            return PropertyID::BorderBottomRightRadius;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopLeftRadius;
            return PropertyID::BorderBottomLeftRadius;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopRightRadius;
            return PropertyID::BorderBottomRightRadius;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderBottomRightRadius;
        return PropertyID::BorderTopRightRadius;

    case PropertyID::BorderInlineEndColor:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderRightColor;
            return PropertyID::BorderLeftColor;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomColor;
            return PropertyID::BorderTopColor;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderTopColor;
        return PropertyID::BorderBottomColor;

    case PropertyID::BorderInlineEndStyle:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderRightStyle;
            return PropertyID::BorderLeftStyle;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomStyle;
            return PropertyID::BorderTopStyle;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderTopStyle;
        return PropertyID::BorderBottomStyle;

    case PropertyID::BorderInlineEndWidth:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderRightWidth;
            return PropertyID::BorderLeftWidth;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomWidth;
            return PropertyID::BorderTopWidth;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderTopWidth;
        return PropertyID::BorderBottomWidth;

    case PropertyID::BorderInlineStartColor:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderLeftColor;
            return PropertyID::BorderRightColor;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopColor;
            return PropertyID::BorderBottomColor;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderBottomColor;
        return PropertyID::BorderTopColor;

    case PropertyID::BorderInlineStartStyle:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderLeftStyle;
            return PropertyID::BorderRightStyle;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopStyle;
            return PropertyID::BorderBottomStyle;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderBottomStyle;
        return PropertyID::BorderTopStyle;

    case PropertyID::BorderInlineStartWidth:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderLeftWidth;
            return PropertyID::BorderRightWidth;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopWidth;
            return PropertyID::BorderBottomWidth;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderBottomWidth;
        return PropertyID::BorderTopWidth;

    case PropertyID::BorderStartEndRadius:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopRightRadius;
            return PropertyID::BorderTopLeftRadius;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomRightRadius;
            return PropertyID::BorderTopRightRadius;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderBottomLeftRadius;
            return PropertyID::BorderTopLeftRadius;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::BorderTopLeftRadius;
        return PropertyID::BorderBottomLeftRadius;

    case PropertyID::BorderStartStartRadius:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopLeftRadius;
            return PropertyID::BorderTopRightRadius;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopRightRadius;
            return PropertyID::BorderBottomRightRadius;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::BorderTopLeftRadius;
            return PropertyID::BorderBottomLeftRadius;
        }
        if (used_direction == Direction::Ltr)
            return PropertyID::BorderBottomLeftRadius;
        return PropertyID::BorderTopLeftRadius;

    case PropertyID::CornerEndEndShape:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomRightShape;
            return PropertyID::CornerBottomLeftShape;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomLeftShape;
            return PropertyID::CornerTopLeftShape;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomRightShape;
            return PropertyID::CornerTopRightShape;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::CornerTopRightShape;
        return PropertyID::CornerBottomRightShape;

    case PropertyID::CornerEndStartShape:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomLeftShape;
            return PropertyID::CornerBottomRightShape;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopLeftShape;
            return PropertyID::CornerBottomLeftShape;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopRightShape;
            return PropertyID::CornerBottomRightShape;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::CornerBottomRightShape;
        return PropertyID::CornerTopRightShape;

    case PropertyID::CornerStartEndShape:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopRightShape;
            return PropertyID::CornerTopLeftShape;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomRightShape;
            return PropertyID::CornerTopRightShape;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerBottomLeftShape;
            return PropertyID::CornerTopLeftShape;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::CornerTopLeftShape;
        return PropertyID::CornerBottomLeftShape;

    case PropertyID::CornerStartStartShape:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopLeftShape;
            return PropertyID::CornerTopRightShape;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopRightShape;
            return PropertyID::CornerBottomRightShape;
        }

        if (mapping_context.writing_mode == WritingMode::VerticalLr) {
            if (used_direction == Direction::Ltr)
                return PropertyID::CornerTopLeftShape;
            return PropertyID::CornerBottomLeftShape;
        }
        if (used_direction == Direction::Ltr)
            return PropertyID::CornerBottomLeftShape;
        return PropertyID::CornerTopLeftShape;

    case PropertyID::InlineSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::Width;
        return PropertyID::Height;

    case PropertyID::InsetBlockEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::Bottom;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::Left;
        return PropertyID::Right;

    case PropertyID::InsetBlockStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::Top;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::Right;
        return PropertyID::Left;

    case PropertyID::InsetInlineEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::Right;
            return PropertyID::Left;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::Bottom;
            return PropertyID::Top;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::Top;
        return PropertyID::Bottom;

    case PropertyID::InsetInlineStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::Left;
            return PropertyID::Right;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::Top;
            return PropertyID::Bottom;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::Bottom;
        return PropertyID::Top;

    case PropertyID::MarginBlockEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MarginBottom;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::MarginLeft;
        return PropertyID::MarginRight;

    case PropertyID::MarginBlockStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MarginTop;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::MarginRight;
        return PropertyID::MarginLeft;

    case PropertyID::MarginInlineEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::MarginRight;
            return PropertyID::MarginLeft;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::MarginBottom;
            return PropertyID::MarginTop;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::MarginTop;
        return PropertyID::MarginBottom;

    case PropertyID::MarginInlineStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::MarginLeft;
            return PropertyID::MarginRight;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::MarginTop;
            return PropertyID::MarginBottom;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::MarginBottom;
        return PropertyID::MarginTop;

    case PropertyID::MaxBlockSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MaxHeight;
        return PropertyID::MaxWidth;

    case PropertyID::MaxInlineSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MaxWidth;
        return PropertyID::MaxHeight;

    case PropertyID::MinBlockSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MinHeight;
        return PropertyID::MinWidth;

    case PropertyID::MinInlineSize:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::MinWidth;
        return PropertyID::MinHeight;

    case PropertyID::OverflowBlock:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::OverflowY;
        return PropertyID::OverflowX;

    case PropertyID::OverflowClipMarginBlockEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::OverflowClipMarginBottom;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::OverflowClipMarginLeft;
        return PropertyID::OverflowClipMarginRight;

    case PropertyID::OverflowClipMarginBlockStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::OverflowClipMarginTop;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::OverflowClipMarginRight;
        return PropertyID::OverflowClipMarginLeft;

    case PropertyID::OverflowClipMarginInlineEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::OverflowClipMarginRight;
            return PropertyID::OverflowClipMarginLeft;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::OverflowClipMarginBottom;
            return PropertyID::OverflowClipMarginTop;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::OverflowClipMarginTop;
        return PropertyID::OverflowClipMarginBottom;

    case PropertyID::OverflowClipMarginInlineStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::OverflowClipMarginLeft;
            return PropertyID::OverflowClipMarginRight;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::OverflowClipMarginTop;
            return PropertyID::OverflowClipMarginBottom;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::OverflowClipMarginBottom;
        return PropertyID::OverflowClipMarginTop;

    case PropertyID::OverflowInline:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::OverflowX;
        return PropertyID::OverflowY;

    case PropertyID::PaddingBlockEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::PaddingBottom;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::PaddingLeft;
        return PropertyID::PaddingRight;

    case PropertyID::PaddingBlockStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb)
            return PropertyID::PaddingTop;
        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl))
            return PropertyID::PaddingRight;
        return PropertyID::PaddingLeft;

    case PropertyID::PaddingInlineEnd:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::PaddingRight;
            return PropertyID::PaddingLeft;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::PaddingBottom;
            return PropertyID::PaddingTop;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::PaddingTop;
        return PropertyID::PaddingBottom;

    case PropertyID::PaddingInlineStart:

        if (mapping_context.writing_mode == WritingMode::HorizontalTb) {
            if (used_direction == Direction::Ltr)
                return PropertyID::PaddingLeft;
            return PropertyID::PaddingRight;
        }

        if (first_is_one_of(mapping_context.writing_mode, WritingMode::VerticalRl, WritingMode::SidewaysRl, WritingMode::VerticalLr)) {
            if (used_direction == Direction::Ltr)
                return PropertyID::PaddingTop;
            return PropertyID::PaddingBottom;
        }

        if (used_direction == Direction::Ltr)
            return PropertyID::PaddingBottom;
        return PropertyID::PaddingTop;

    default:
        VERIFY(!property_is_logical_alias(property_id));
        return property_id;
    }
}

Optional<LogicalPropertyGroup> logical_property_group_for_property(PropertyID property_id)
{
    switch(property_id) {

    case PropertyID::BorderTopWidth:

    case PropertyID::BorderRightWidth:

    case PropertyID::BorderBottomWidth:

    case PropertyID::BorderLeftWidth:

    case PropertyID::BorderBlockEndWidth:

    case PropertyID::BorderBlockStartWidth:

    case PropertyID::BorderInlineEndWidth:

    case PropertyID::BorderInlineStartWidth:

        return LogicalPropertyGroup::BorderWidth;

    case PropertyID::BorderTopStyle:

    case PropertyID::BorderRightStyle:

    case PropertyID::BorderBottomStyle:

    case PropertyID::BorderLeftStyle:

    case PropertyID::BorderBlockEndStyle:

    case PropertyID::BorderBlockStartStyle:

    case PropertyID::BorderInlineEndStyle:

    case PropertyID::BorderInlineStartStyle:

        return LogicalPropertyGroup::BorderStyle;

    case PropertyID::CornerTopLeftShape:

    case PropertyID::CornerBottomLeftShape:

    case PropertyID::CornerTopRightShape:

    case PropertyID::CornerBottomRightShape:

    case PropertyID::CornerEndEndShape:

    case PropertyID::CornerEndStartShape:

    case PropertyID::CornerStartEndShape:

    case PropertyID::CornerStartStartShape:

        return LogicalPropertyGroup::CornerShape;

    case PropertyID::Width:

    case PropertyID::Height:

    case PropertyID::BlockSize:

    case PropertyID::InlineSize:

        return LogicalPropertyGroup::Size;

    case PropertyID::MinWidth:

    case PropertyID::MinHeight:

    case PropertyID::MinBlockSize:

    case PropertyID::MinInlineSize:

        return LogicalPropertyGroup::MinSize;

    case PropertyID::OverflowX:

    case PropertyID::OverflowY:

    case PropertyID::OverflowBlock:

    case PropertyID::OverflowInline:

        return LogicalPropertyGroup::Overflow;

    case PropertyID::MaxWidth:

    case PropertyID::MaxHeight:

    case PropertyID::MaxBlockSize:

    case PropertyID::MaxInlineSize:

        return LogicalPropertyGroup::MaxSize;

    case PropertyID::PaddingTop:

    case PropertyID::PaddingRight:

    case PropertyID::PaddingBottom:

    case PropertyID::PaddingLeft:

    case PropertyID::PaddingBlockEnd:

    case PropertyID::PaddingBlockStart:

    case PropertyID::PaddingInlineEnd:

    case PropertyID::PaddingInlineStart:

        return LogicalPropertyGroup::Padding;

    case PropertyID::OverflowClipMarginTop:

    case PropertyID::OverflowClipMarginRight:

    case PropertyID::OverflowClipMarginBottom:

    case PropertyID::OverflowClipMarginLeft:

    case PropertyID::OverflowClipMarginBlockEnd:

    case PropertyID::OverflowClipMarginBlockStart:

    case PropertyID::OverflowClipMarginInlineEnd:

    case PropertyID::OverflowClipMarginInlineStart:

        return LogicalPropertyGroup::OverflowClipMargin;

    case PropertyID::Top:

    case PropertyID::Right:

    case PropertyID::Bottom:

    case PropertyID::Left:

    case PropertyID::InsetBlockEnd:

    case PropertyID::InsetBlockStart:

    case PropertyID::InsetInlineEnd:

    case PropertyID::InsetInlineStart:

        return LogicalPropertyGroup::Inset;

    case PropertyID::BorderTopLeftRadius:

    case PropertyID::BorderBottomLeftRadius:

    case PropertyID::BorderTopRightRadius:

    case PropertyID::BorderBottomRightRadius:

    case PropertyID::BorderEndEndRadius:

    case PropertyID::BorderEndStartRadius:

    case PropertyID::BorderStartEndRadius:

    case PropertyID::BorderStartStartRadius:

        return LogicalPropertyGroup::BorderRadius;

    case PropertyID::MarginTop:

    case PropertyID::MarginRight:

    case PropertyID::MarginBottom:

    case PropertyID::MarginLeft:

    case PropertyID::MarginBlockEnd:

    case PropertyID::MarginBlockStart:

    case PropertyID::MarginInlineEnd:

    case PropertyID::MarginInlineStart:

        return LogicalPropertyGroup::Margin;

    case PropertyID::BorderTopColor:

    case PropertyID::BorderRightColor:

    case PropertyID::BorderBottomColor:

    case PropertyID::BorderLeftColor:

    case PropertyID::BorderBlockEndColor:

    case PropertyID::BorderBlockStartColor:

    case PropertyID::BorderInlineEndColor:

    case PropertyID::BorderInlineStartColor:

        return LogicalPropertyGroup::BorderColor;

    default:
        return {};
    }
}

} // namespace Web::CSS
