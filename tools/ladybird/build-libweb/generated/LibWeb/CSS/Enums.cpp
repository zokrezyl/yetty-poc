
#include <LibWeb/CSS/Enums.h>
#include <LibWeb/CSS/Keyword.h>

namespace Web::CSS {

Optional<AbsoluteSize> keyword_to_absolute_size(Keyword keyword)
{
    switch (keyword) {
    case Keyword::XxSmall:
        return AbsoluteSize::XxSmall;
    case Keyword::XSmall:
        return AbsoluteSize::XSmall;
    case Keyword::Small:
        return AbsoluteSize::Small;
    case Keyword::Medium:
        return AbsoluteSize::Medium;
    case Keyword::Large:
        return AbsoluteSize::Large;
    case Keyword::XLarge:
        return AbsoluteSize::XLarge;
    case Keyword::XxLarge:
        return AbsoluteSize::XxLarge;
    case Keyword::XxxLarge:
        return AbsoluteSize::XxxLarge;
    default:
        return {};
    }
}

Keyword to_keyword(AbsoluteSize absolute_size_value)
{
    switch (absolute_size_value) {
    case AbsoluteSize::XxSmall:
        return Keyword::XxSmall;
    case AbsoluteSize::XSmall:
        return Keyword::XSmall;
    case AbsoluteSize::Small:
        return Keyword::Small;
    case AbsoluteSize::Medium:
        return Keyword::Medium;
    case AbsoluteSize::Large:
        return Keyword::Large;
    case AbsoluteSize::XLarge:
        return Keyword::XLarge;
    case AbsoluteSize::XxLarge:
        return Keyword::XxLarge;
    case AbsoluteSize::XxxLarge:
        return Keyword::XxxLarge;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AbsoluteSize value)
{
    switch (value) {
    case AbsoluteSize::XxSmall:
        return "xx-small"sv;
    case AbsoluteSize::XSmall:
        return "x-small"sv;
    case AbsoluteSize::Small:
        return "small"sv;
    case AbsoluteSize::Medium:
        return "medium"sv;
    case AbsoluteSize::Large:
        return "large"sv;
    case AbsoluteSize::XLarge:
        return "x-large"sv;
    case AbsoluteSize::XxLarge:
        return "xx-large"sv;
    case AbsoluteSize::XxxLarge:
        return "xxx-large"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AlignContent> keyword_to_align_content(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return AlignContent::Normal;
    case Keyword::Start:
        return AlignContent::Start;
    case Keyword::FlexStart:
        return AlignContent::FlexStart;
    case Keyword::End:
        return AlignContent::End;
    case Keyword::FlexEnd:
        return AlignContent::FlexEnd;
    case Keyword::Center:
        return AlignContent::Center;
    case Keyword::SpaceBetween:
        return AlignContent::SpaceBetween;
    case Keyword::SpaceAround:
        return AlignContent::SpaceAround;
    case Keyword::SpaceEvenly:
        return AlignContent::SpaceEvenly;
    case Keyword::Stretch:
        return AlignContent::Stretch;
    default:
        return {};
    }
}

Keyword to_keyword(AlignContent align_content_value)
{
    switch (align_content_value) {
    case AlignContent::Normal:
        return Keyword::Normal;
    case AlignContent::Start:
        return Keyword::Start;
    case AlignContent::FlexStart:
        return Keyword::FlexStart;
    case AlignContent::End:
        return Keyword::End;
    case AlignContent::FlexEnd:
        return Keyword::FlexEnd;
    case AlignContent::Center:
        return Keyword::Center;
    case AlignContent::SpaceBetween:
        return Keyword::SpaceBetween;
    case AlignContent::SpaceAround:
        return Keyword::SpaceAround;
    case AlignContent::SpaceEvenly:
        return Keyword::SpaceEvenly;
    case AlignContent::Stretch:
        return Keyword::Stretch;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AlignContent value)
{
    switch (value) {
    case AlignContent::Normal:
        return "normal"sv;
    case AlignContent::Start:
        return "start"sv;
    case AlignContent::FlexStart:
        return "flex-start"sv;
    case AlignContent::End:
        return "end"sv;
    case AlignContent::FlexEnd:
        return "flex-end"sv;
    case AlignContent::Center:
        return "center"sv;
    case AlignContent::SpaceBetween:
        return "space-between"sv;
    case AlignContent::SpaceAround:
        return "space-around"sv;
    case AlignContent::SpaceEvenly:
        return "space-evenly"sv;
    case AlignContent::Stretch:
        return "stretch"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AlignItems> keyword_to_align_items(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Baseline:
        return AlignItems::Baseline;
    case Keyword::Center:
        return AlignItems::Center;
    case Keyword::End:
        return AlignItems::End;
    case Keyword::FlexEnd:
        return AlignItems::FlexEnd;
    case Keyword::FlexStart:
        return AlignItems::FlexStart;
    case Keyword::Normal:
        return AlignItems::Normal;
    case Keyword::Safe:
        return AlignItems::Safe;
    case Keyword::SelfEnd:
        return AlignItems::SelfEnd;
    case Keyword::SelfStart:
        return AlignItems::SelfStart;
    case Keyword::Start:
        return AlignItems::Start;
    case Keyword::Stretch:
        return AlignItems::Stretch;
    case Keyword::Unsafe:
        return AlignItems::Unsafe;
    default:
        return {};
    }
}

Keyword to_keyword(AlignItems align_items_value)
{
    switch (align_items_value) {
    case AlignItems::Baseline:
        return Keyword::Baseline;
    case AlignItems::Center:
        return Keyword::Center;
    case AlignItems::End:
        return Keyword::End;
    case AlignItems::FlexEnd:
        return Keyword::FlexEnd;
    case AlignItems::FlexStart:
        return Keyword::FlexStart;
    case AlignItems::Normal:
        return Keyword::Normal;
    case AlignItems::Safe:
        return Keyword::Safe;
    case AlignItems::SelfEnd:
        return Keyword::SelfEnd;
    case AlignItems::SelfStart:
        return Keyword::SelfStart;
    case AlignItems::Start:
        return Keyword::Start;
    case AlignItems::Stretch:
        return Keyword::Stretch;
    case AlignItems::Unsafe:
        return Keyword::Unsafe;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AlignItems value)
{
    switch (value) {
    case AlignItems::Baseline:
        return "baseline"sv;
    case AlignItems::Center:
        return "center"sv;
    case AlignItems::End:
        return "end"sv;
    case AlignItems::FlexEnd:
        return "flex-end"sv;
    case AlignItems::FlexStart:
        return "flex-start"sv;
    case AlignItems::Normal:
        return "normal"sv;
    case AlignItems::Safe:
        return "safe"sv;
    case AlignItems::SelfEnd:
        return "self-end"sv;
    case AlignItems::SelfStart:
        return "self-start"sv;
    case AlignItems::Start:
        return "start"sv;
    case AlignItems::Stretch:
        return "stretch"sv;
    case AlignItems::Unsafe:
        return "unsafe"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AlignSelf> keyword_to_align_self(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return AlignSelf::Auto;
    case Keyword::Baseline:
        return AlignSelf::Baseline;
    case Keyword::Center:
        return AlignSelf::Center;
    case Keyword::End:
        return AlignSelf::End;
    case Keyword::FlexEnd:
        return AlignSelf::FlexEnd;
    case Keyword::FlexStart:
        return AlignSelf::FlexStart;
    case Keyword::Normal:
        return AlignSelf::Normal;
    case Keyword::Safe:
        return AlignSelf::Safe;
    case Keyword::SelfEnd:
        return AlignSelf::SelfEnd;
    case Keyword::SelfStart:
        return AlignSelf::SelfStart;
    case Keyword::Start:
        return AlignSelf::Start;
    case Keyword::Stretch:
        return AlignSelf::Stretch;
    case Keyword::Unsafe:
        return AlignSelf::Unsafe;
    default:
        return {};
    }
}

Keyword to_keyword(AlignSelf align_self_value)
{
    switch (align_self_value) {
    case AlignSelf::Auto:
        return Keyword::Auto;
    case AlignSelf::Baseline:
        return Keyword::Baseline;
    case AlignSelf::Center:
        return Keyword::Center;
    case AlignSelf::End:
        return Keyword::End;
    case AlignSelf::FlexEnd:
        return Keyword::FlexEnd;
    case AlignSelf::FlexStart:
        return Keyword::FlexStart;
    case AlignSelf::Normal:
        return Keyword::Normal;
    case AlignSelf::Safe:
        return Keyword::Safe;
    case AlignSelf::SelfEnd:
        return Keyword::SelfEnd;
    case AlignSelf::SelfStart:
        return Keyword::SelfStart;
    case AlignSelf::Start:
        return Keyword::Start;
    case AlignSelf::Stretch:
        return Keyword::Stretch;
    case AlignSelf::Unsafe:
        return Keyword::Unsafe;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AlignSelf value)
{
    switch (value) {
    case AlignSelf::Auto:
        return "auto"sv;
    case AlignSelf::Baseline:
        return "baseline"sv;
    case AlignSelf::Center:
        return "center"sv;
    case AlignSelf::End:
        return "end"sv;
    case AlignSelf::FlexEnd:
        return "flex-end"sv;
    case AlignSelf::FlexStart:
        return "flex-start"sv;
    case AlignSelf::Normal:
        return "normal"sv;
    case AlignSelf::Safe:
        return "safe"sv;
    case AlignSelf::SelfEnd:
        return "self-end"sv;
    case AlignSelf::SelfStart:
        return "self-start"sv;
    case AlignSelf::Start:
        return "start"sv;
    case AlignSelf::Stretch:
        return "stretch"sv;
    case AlignSelf::Unsafe:
        return "unsafe"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnchorSide> keyword_to_anchor_side(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Inside:
        return AnchorSide::Inside;
    case Keyword::Outside:
        return AnchorSide::Outside;
    case Keyword::Top:
        return AnchorSide::Top;
    case Keyword::Left:
        return AnchorSide::Left;
    case Keyword::Right:
        return AnchorSide::Right;
    case Keyword::Bottom:
        return AnchorSide::Bottom;
    case Keyword::Start:
        return AnchorSide::Start;
    case Keyword::End:
        return AnchorSide::End;
    case Keyword::SelfStart:
        return AnchorSide::SelfStart;
    case Keyword::SelfEnd:
        return AnchorSide::SelfEnd;
    case Keyword::Center:
        return AnchorSide::Center;
    default:
        return {};
    }
}

Keyword to_keyword(AnchorSide anchor_side_value)
{
    switch (anchor_side_value) {
    case AnchorSide::Inside:
        return Keyword::Inside;
    case AnchorSide::Outside:
        return Keyword::Outside;
    case AnchorSide::Top:
        return Keyword::Top;
    case AnchorSide::Left:
        return Keyword::Left;
    case AnchorSide::Right:
        return Keyword::Right;
    case AnchorSide::Bottom:
        return Keyword::Bottom;
    case AnchorSide::Start:
        return Keyword::Start;
    case AnchorSide::End:
        return Keyword::End;
    case AnchorSide::SelfStart:
        return Keyword::SelfStart;
    case AnchorSide::SelfEnd:
        return Keyword::SelfEnd;
    case AnchorSide::Center:
        return Keyword::Center;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnchorSide value)
{
    switch (value) {
    case AnchorSide::Inside:
        return "inside"sv;
    case AnchorSide::Outside:
        return "outside"sv;
    case AnchorSide::Top:
        return "top"sv;
    case AnchorSide::Left:
        return "left"sv;
    case AnchorSide::Right:
        return "right"sv;
    case AnchorSide::Bottom:
        return "bottom"sv;
    case AnchorSide::Start:
        return "start"sv;
    case AnchorSide::End:
        return "end"sv;
    case AnchorSide::SelfStart:
        return "self-start"sv;
    case AnchorSide::SelfEnd:
        return "self-end"sv;
    case AnchorSide::Center:
        return "center"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnchorSize> keyword_to_anchor_size(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Block:
        return AnchorSize::Block;
    case Keyword::Height:
        return AnchorSize::Height;
    case Keyword::Inline:
        return AnchorSize::Inline;
    case Keyword::SelfBlock:
        return AnchorSize::SelfBlock;
    case Keyword::SelfInline:
        return AnchorSize::SelfInline;
    case Keyword::Width:
        return AnchorSize::Width;
    default:
        return {};
    }
}

Keyword to_keyword(AnchorSize anchor_size_value)
{
    switch (anchor_size_value) {
    case AnchorSize::Block:
        return Keyword::Block;
    case AnchorSize::Height:
        return Keyword::Height;
    case AnchorSize::Inline:
        return Keyword::Inline;
    case AnchorSize::SelfBlock:
        return Keyword::SelfBlock;
    case AnchorSize::SelfInline:
        return Keyword::SelfInline;
    case AnchorSize::Width:
        return Keyword::Width;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnchorSize value)
{
    switch (value) {
    case AnchorSize::Block:
        return "block"sv;
    case AnchorSize::Height:
        return "height"sv;
    case AnchorSize::Inline:
        return "inline"sv;
    case AnchorSize::SelfBlock:
        return "self-block"sv;
    case AnchorSize::SelfInline:
        return "self-inline"sv;
    case AnchorSize::Width:
        return "width"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnimationComposition> keyword_to_animation_composition(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Replace:
        return AnimationComposition::Replace;
    case Keyword::Add:
        return AnimationComposition::Add;
    case Keyword::Accumulate:
        return AnimationComposition::Accumulate;
    default:
        return {};
    }
}

Keyword to_keyword(AnimationComposition animation_composition_value)
{
    switch (animation_composition_value) {
    case AnimationComposition::Replace:
        return Keyword::Replace;
    case AnimationComposition::Add:
        return Keyword::Add;
    case AnimationComposition::Accumulate:
        return Keyword::Accumulate;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnimationComposition value)
{
    switch (value) {
    case AnimationComposition::Replace:
        return "replace"sv;
    case AnimationComposition::Add:
        return "add"sv;
    case AnimationComposition::Accumulate:
        return "accumulate"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnimationDirection> keyword_to_animation_direction(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Alternate:
        return AnimationDirection::Alternate;
    case Keyword::AlternateReverse:
        return AnimationDirection::AlternateReverse;
    case Keyword::Normal:
        return AnimationDirection::Normal;
    case Keyword::Reverse:
        return AnimationDirection::Reverse;
    default:
        return {};
    }
}

Keyword to_keyword(AnimationDirection animation_direction_value)
{
    switch (animation_direction_value) {
    case AnimationDirection::Alternate:
        return Keyword::Alternate;
    case AnimationDirection::AlternateReverse:
        return Keyword::AlternateReverse;
    case AnimationDirection::Normal:
        return Keyword::Normal;
    case AnimationDirection::Reverse:
        return Keyword::Reverse;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnimationDirection value)
{
    switch (value) {
    case AnimationDirection::Alternate:
        return "alternate"sv;
    case AnimationDirection::AlternateReverse:
        return "alternate-reverse"sv;
    case AnimationDirection::Normal:
        return "normal"sv;
    case AnimationDirection::Reverse:
        return "reverse"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnimationFillMode> keyword_to_animation_fill_mode(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Backwards:
        return AnimationFillMode::Backwards;
    case Keyword::Both:
        return AnimationFillMode::Both;
    case Keyword::Forwards:
        return AnimationFillMode::Forwards;
    case Keyword::None:
        return AnimationFillMode::None;
    default:
        return {};
    }
}

Keyword to_keyword(AnimationFillMode animation_fill_mode_value)
{
    switch (animation_fill_mode_value) {
    case AnimationFillMode::Backwards:
        return Keyword::Backwards;
    case AnimationFillMode::Both:
        return Keyword::Both;
    case AnimationFillMode::Forwards:
        return Keyword::Forwards;
    case AnimationFillMode::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnimationFillMode value)
{
    switch (value) {
    case AnimationFillMode::Backwards:
        return "backwards"sv;
    case AnimationFillMode::Both:
        return "both"sv;
    case AnimationFillMode::Forwards:
        return "forwards"sv;
    case AnimationFillMode::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<AnimationPlayState> keyword_to_animation_play_state(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Paused:
        return AnimationPlayState::Paused;
    case Keyword::Running:
        return AnimationPlayState::Running;
    default:
        return {};
    }
}

Keyword to_keyword(AnimationPlayState animation_play_state_value)
{
    switch (animation_play_state_value) {
    case AnimationPlayState::Paused:
        return Keyword::Paused;
    case AnimationPlayState::Running:
        return Keyword::Running;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(AnimationPlayState value)
{
    switch (value) {
    case AnimationPlayState::Paused:
        return "paused"sv;
    case AnimationPlayState::Running:
        return "running"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Appearance> keyword_to_appearance(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return Appearance::Auto;
    case Keyword::Button:
        return Appearance::Button;
    case Keyword::Checkbox:
        return Appearance::Checkbox;
    case Keyword::Listbox:
        return Appearance::Listbox;
    case Keyword::Menulist:
        return Appearance::Menulist;
    case Keyword::Meter:
        return Appearance::Meter;
    case Keyword::MenulistButton:
        return Appearance::MenulistButton;
    case Keyword::None:
        return Appearance::None;
    case Keyword::PushButton:
        return Appearance::PushButton;
    case Keyword::ProgressBar:
        return Appearance::ProgressBar;
    case Keyword::Radio:
        return Appearance::Radio;
    case Keyword::Searchfield:
        return Appearance::Searchfield;
    case Keyword::SliderHorizontal:
        return Appearance::SliderHorizontal;
    case Keyword::SquareButton:
        return Appearance::SquareButton;
    case Keyword::Textarea:
        return Appearance::Textarea;
    case Keyword::Textfield:
        return Appearance::Textfield;
    default:
        return {};
    }
}

Keyword to_keyword(Appearance appearance_value)
{
    switch (appearance_value) {
    case Appearance::Auto:
        return Keyword::Auto;
    case Appearance::Button:
        return Keyword::Button;
    case Appearance::Checkbox:
        return Keyword::Checkbox;
    case Appearance::Listbox:
        return Keyword::Listbox;
    case Appearance::Menulist:
        return Keyword::Menulist;
    case Appearance::Meter:
        return Keyword::Meter;
    case Appearance::MenulistButton:
        return Keyword::MenulistButton;
    case Appearance::None:
        return Keyword::None;
    case Appearance::PushButton:
        return Keyword::PushButton;
    case Appearance::ProgressBar:
        return Keyword::ProgressBar;
    case Appearance::Radio:
        return Keyword::Radio;
    case Appearance::Searchfield:
        return Keyword::Searchfield;
    case Appearance::SliderHorizontal:
        return Keyword::SliderHorizontal;
    case Appearance::SquareButton:
        return Keyword::SquareButton;
    case Appearance::Textarea:
        return Keyword::Textarea;
    case Appearance::Textfield:
        return Keyword::Textfield;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Appearance value)
{
    switch (value) {
    case Appearance::Auto:
        return "auto"sv;
    case Appearance::Button:
        return "button"sv;
    case Appearance::Checkbox:
        return "checkbox"sv;
    case Appearance::Listbox:
        return "listbox"sv;
    case Appearance::Menulist:
        return "menulist"sv;
    case Appearance::Meter:
        return "meter"sv;
    case Appearance::MenulistButton:
        return "menulist-button"sv;
    case Appearance::None:
        return "none"sv;
    case Appearance::PushButton:
        return "push-button"sv;
    case Appearance::ProgressBar:
        return "progress-bar"sv;
    case Appearance::Radio:
        return "radio"sv;
    case Appearance::Searchfield:
        return "searchfield"sv;
    case Appearance::SliderHorizontal:
        return "slider-horizontal"sv;
    case Appearance::SquareButton:
        return "square-button"sv;
    case Appearance::Textarea:
        return "textarea"sv;
    case Appearance::Textfield:
        return "textfield"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Axis> keyword_to_axis(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Block:
        return Axis::Block;
    case Keyword::Inline:
        return Axis::Inline;
    case Keyword::X:
        return Axis::X;
    case Keyword::Y:
        return Axis::Y;
    default:
        return {};
    }
}

Keyword to_keyword(Axis axis_value)
{
    switch (axis_value) {
    case Axis::Block:
        return Keyword::Block;
    case Axis::Inline:
        return Keyword::Inline;
    case Axis::X:
        return Keyword::X;
    case Axis::Y:
        return Keyword::Y;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Axis value)
{
    switch (value) {
    case Axis::Block:
        return "block"sv;
    case Axis::Inline:
        return "inline"sv;
    case Axis::X:
        return "x"sv;
    case Axis::Y:
        return "y"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BackgroundAttachment> keyword_to_background_attachment(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Fixed:
        return BackgroundAttachment::Fixed;
    case Keyword::Local:
        return BackgroundAttachment::Local;
    case Keyword::Scroll:
        return BackgroundAttachment::Scroll;
    default:
        return {};
    }
}

Keyword to_keyword(BackgroundAttachment background_attachment_value)
{
    switch (background_attachment_value) {
    case BackgroundAttachment::Fixed:
        return Keyword::Fixed;
    case BackgroundAttachment::Local:
        return Keyword::Local;
    case BackgroundAttachment::Scroll:
        return Keyword::Scroll;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BackgroundAttachment value)
{
    switch (value) {
    case BackgroundAttachment::Fixed:
        return "fixed"sv;
    case BackgroundAttachment::Local:
        return "local"sv;
    case BackgroundAttachment::Scroll:
        return "scroll"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BackgroundBox> keyword_to_background_box(Keyword keyword)
{
    switch (keyword) {
    case Keyword::BorderBox:
        return BackgroundBox::BorderBox;
    case Keyword::ContentBox:
        return BackgroundBox::ContentBox;
    case Keyword::PaddingBox:
        return BackgroundBox::PaddingBox;
    case Keyword::Text:
        return BackgroundBox::Text;
    default:
        return {};
    }
}

Keyword to_keyword(BackgroundBox background_box_value)
{
    switch (background_box_value) {
    case BackgroundBox::BorderBox:
        return Keyword::BorderBox;
    case BackgroundBox::ContentBox:
        return Keyword::ContentBox;
    case BackgroundBox::PaddingBox:
        return Keyword::PaddingBox;
    case BackgroundBox::Text:
        return Keyword::Text;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BackgroundBox value)
{
    switch (value) {
    case BackgroundBox::BorderBox:
        return "border-box"sv;
    case BackgroundBox::ContentBox:
        return "content-box"sv;
    case BackgroundBox::PaddingBox:
        return "padding-box"sv;
    case BackgroundBox::Text:
        return "text"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BaselineMetric> keyword_to_baseline_metric(Keyword keyword)
{
    switch (keyword) {
    case Keyword::TextBottom:
        return BaselineMetric::TextBottom;
    case Keyword::Alphabetic:
        return BaselineMetric::Alphabetic;
    case Keyword::Ideographic:
        return BaselineMetric::Ideographic;
    case Keyword::Middle:
        return BaselineMetric::Middle;
    case Keyword::Central:
        return BaselineMetric::Central;
    case Keyword::Mathematical:
        return BaselineMetric::Mathematical;
    case Keyword::Hanging:
        return BaselineMetric::Hanging;
    case Keyword::TextTop:
        return BaselineMetric::TextTop;
    default:
        return {};
    }
}

Keyword to_keyword(BaselineMetric baseline_metric_value)
{
    switch (baseline_metric_value) {
    case BaselineMetric::TextBottom:
        return Keyword::TextBottom;
    case BaselineMetric::Alphabetic:
        return Keyword::Alphabetic;
    case BaselineMetric::Ideographic:
        return Keyword::Ideographic;
    case BaselineMetric::Middle:
        return Keyword::Middle;
    case BaselineMetric::Central:
        return Keyword::Central;
    case BaselineMetric::Mathematical:
        return Keyword::Mathematical;
    case BaselineMetric::Hanging:
        return Keyword::Hanging;
    case BaselineMetric::TextTop:
        return Keyword::TextTop;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BaselineMetric value)
{
    switch (value) {
    case BaselineMetric::TextBottom:
        return "text-bottom"sv;
    case BaselineMetric::Alphabetic:
        return "alphabetic"sv;
    case BaselineMetric::Ideographic:
        return "ideographic"sv;
    case BaselineMetric::Middle:
        return "middle"sv;
    case BaselineMetric::Central:
        return "central"sv;
    case BaselineMetric::Mathematical:
        return "mathematical"sv;
    case BaselineMetric::Hanging:
        return "hanging"sv;
    case BaselineMetric::TextTop:
        return "text-top"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BorderCollapse> keyword_to_border_collapse(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Separate:
        return BorderCollapse::Separate;
    case Keyword::Collapse:
        return BorderCollapse::Collapse;
    default:
        return {};
    }
}

Keyword to_keyword(BorderCollapse border_collapse_value)
{
    switch (border_collapse_value) {
    case BorderCollapse::Separate:
        return Keyword::Separate;
    case BorderCollapse::Collapse:
        return Keyword::Collapse;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BorderCollapse value)
{
    switch (value) {
    case BorderCollapse::Separate:
        return "separate"sv;
    case BorderCollapse::Collapse:
        return "collapse"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BorderImageRepeat> keyword_to_border_image_repeat(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Stretch:
        return BorderImageRepeat::Stretch;
    case Keyword::Repeat:
        return BorderImageRepeat::Repeat;
    case Keyword::Round:
        return BorderImageRepeat::Round;
    case Keyword::Space:
        return BorderImageRepeat::Space;
    default:
        return {};
    }
}

Keyword to_keyword(BorderImageRepeat border_image_repeat_value)
{
    switch (border_image_repeat_value) {
    case BorderImageRepeat::Stretch:
        return Keyword::Stretch;
    case BorderImageRepeat::Repeat:
        return Keyword::Repeat;
    case BorderImageRepeat::Round:
        return Keyword::Round;
    case BorderImageRepeat::Space:
        return Keyword::Space;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BorderImageRepeat value)
{
    switch (value) {
    case BorderImageRepeat::Stretch:
        return "stretch"sv;
    case BorderImageRepeat::Repeat:
        return "repeat"sv;
    case BorderImageRepeat::Round:
        return "round"sv;
    case BorderImageRepeat::Space:
        return "space"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<BoxSizing> keyword_to_box_sizing(Keyword keyword)
{
    switch (keyword) {
    case Keyword::BorderBox:
        return BoxSizing::BorderBox;
    case Keyword::ContentBox:
        return BoxSizing::ContentBox;
    default:
        return {};
    }
}

Keyword to_keyword(BoxSizing box_sizing_value)
{
    switch (box_sizing_value) {
    case BoxSizing::BorderBox:
        return Keyword::BorderBox;
    case BoxSizing::ContentBox:
        return Keyword::ContentBox;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(BoxSizing value)
{
    switch (value) {
    case BoxSizing::BorderBox:
        return "border-box"sv;
    case BoxSizing::ContentBox:
        return "content-box"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CaptionSide> keyword_to_caption_side(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Top:
        return CaptionSide::Top;
    case Keyword::Bottom:
        return CaptionSide::Bottom;
    default:
        return {};
    }
}

Keyword to_keyword(CaptionSide caption_side_value)
{
    switch (caption_side_value) {
    case CaptionSide::Top:
        return Keyword::Top;
    case CaptionSide::Bottom:
        return Keyword::Bottom;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CaptionSide value)
{
    switch (value) {
    case CaptionSide::Top:
        return "top"sv;
    case CaptionSide::Bottom:
        return "bottom"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Clear> keyword_to_clear(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return Clear::None;
    case Keyword::Left:
        return Clear::Left;
    case Keyword::Right:
        return Clear::Right;
    case Keyword::Both:
        return Clear::Both;
    case Keyword::InlineStart:
        return Clear::InlineStart;
    case Keyword::InlineEnd:
        return Clear::InlineEnd;
    default:
        return {};
    }
}

Keyword to_keyword(Clear clear_value)
{
    switch (clear_value) {
    case Clear::None:
        return Keyword::None;
    case Clear::Left:
        return Keyword::Left;
    case Clear::Right:
        return Keyword::Right;
    case Clear::Both:
        return Keyword::Both;
    case Clear::InlineStart:
        return Keyword::InlineStart;
    case Clear::InlineEnd:
        return Keyword::InlineEnd;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Clear value)
{
    switch (value) {
    case Clear::None:
        return "none"sv;
    case Clear::Left:
        return "left"sv;
    case Clear::Right:
        return "right"sv;
    case Clear::Both:
        return "both"sv;
    case Clear::InlineStart:
        return "inline-start"sv;
    case Clear::InlineEnd:
        return "inline-end"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ColorInterpolation> keyword_to_color_interpolation(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return ColorInterpolation::Auto;
    case Keyword::Linearrgb:
        return ColorInterpolation::Linearrgb;
    case Keyword::Srgb:
        return ColorInterpolation::Srgb;
    default:
        return {};
    }
}

Keyword to_keyword(ColorInterpolation color_interpolation_value)
{
    switch (color_interpolation_value) {
    case ColorInterpolation::Auto:
        return Keyword::Auto;
    case ColorInterpolation::Linearrgb:
        return Keyword::Linearrgb;
    case ColorInterpolation::Srgb:
        return Keyword::Srgb;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ColorInterpolation value)
{
    switch (value) {
    case ColorInterpolation::Auto:
        return "auto"sv;
    case ColorInterpolation::Linearrgb:
        return "linearrgb"sv;
    case ColorInterpolation::Srgb:
        return "srgb"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ColumnSpan> keyword_to_column_span(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return ColumnSpan::None;
    case Keyword::All:
        return ColumnSpan::All;
    default:
        return {};
    }
}

Keyword to_keyword(ColumnSpan column_span_value)
{
    switch (column_span_value) {
    case ColumnSpan::None:
        return Keyword::None;
    case ColumnSpan::All:
        return Keyword::All;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ColumnSpan value)
{
    switch (value) {
    case ColumnSpan::None:
        return "none"sv;
    case ColumnSpan::All:
        return "all"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CommonLigValue> keyword_to_common_lig_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::CommonLigatures:
        return CommonLigValue::CommonLigatures;
    case Keyword::NoCommonLigatures:
        return CommonLigValue::NoCommonLigatures;
    default:
        return {};
    }
}

Keyword to_keyword(CommonLigValue common_lig_value_value)
{
    switch (common_lig_value_value) {
    case CommonLigValue::CommonLigatures:
        return Keyword::CommonLigatures;
    case CommonLigValue::NoCommonLigatures:
        return Keyword::NoCommonLigatures;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CommonLigValue value)
{
    switch (value) {
    case CommonLigValue::CommonLigatures:
        return "common-ligatures"sv;
    case CommonLigValue::NoCommonLigatures:
        return "no-common-ligatures"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CompositingOperator> keyword_to_compositing_operator(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Add:
        return CompositingOperator::Add;
    case Keyword::Subtract:
        return CompositingOperator::Subtract;
    case Keyword::Intersect:
        return CompositingOperator::Intersect;
    case Keyword::Exclude:
        return CompositingOperator::Exclude;
    default:
        return {};
    }
}

Keyword to_keyword(CompositingOperator compositing_operator_value)
{
    switch (compositing_operator_value) {
    case CompositingOperator::Add:
        return Keyword::Add;
    case CompositingOperator::Subtract:
        return Keyword::Subtract;
    case CompositingOperator::Intersect:
        return Keyword::Intersect;
    case CompositingOperator::Exclude:
        return Keyword::Exclude;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CompositingOperator value)
{
    switch (value) {
    case CompositingOperator::Add:
        return "add"sv;
    case CompositingOperator::Subtract:
        return "subtract"sv;
    case CompositingOperator::Intersect:
        return "intersect"sv;
    case CompositingOperator::Exclude:
        return "exclude"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Contain> keyword_to_contain(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return Contain::None;
    case Keyword::Strict:
        return Contain::Strict;
    case Keyword::Content:
        return Contain::Content;
    case Keyword::Size:
        return Contain::Size;
    case Keyword::InlineSize:
        return Contain::InlineSize;
    case Keyword::Layout:
        return Contain::Layout;
    case Keyword::Style:
        return Contain::Style;
    case Keyword::Paint:
        return Contain::Paint;
    default:
        return {};
    }
}

Keyword to_keyword(Contain contain_value)
{
    switch (contain_value) {
    case Contain::None:
        return Keyword::None;
    case Contain::Strict:
        return Keyword::Strict;
    case Contain::Content:
        return Keyword::Content;
    case Contain::Size:
        return Keyword::Size;
    case Contain::InlineSize:
        return Keyword::InlineSize;
    case Contain::Layout:
        return Keyword::Layout;
    case Contain::Style:
        return Keyword::Style;
    case Contain::Paint:
        return Keyword::Paint;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Contain value)
{
    switch (value) {
    case Contain::None:
        return "none"sv;
    case Contain::Strict:
        return "strict"sv;
    case Contain::Content:
        return "content"sv;
    case Contain::Size:
        return "size"sv;
    case Contain::InlineSize:
        return "inline-size"sv;
    case Contain::Layout:
        return "layout"sv;
    case Contain::Style:
        return "style"sv;
    case Contain::Paint:
        return "paint"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ContentVisibility> keyword_to_content_visibility(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Visible:
        return ContentVisibility::Visible;
    case Keyword::Auto:
        return ContentVisibility::Auto;
    case Keyword::Hidden:
        return ContentVisibility::Hidden;
    default:
        return {};
    }
}

Keyword to_keyword(ContentVisibility content_visibility_value)
{
    switch (content_visibility_value) {
    case ContentVisibility::Visible:
        return Keyword::Visible;
    case ContentVisibility::Auto:
        return Keyword::Auto;
    case ContentVisibility::Hidden:
        return Keyword::Hidden;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ContentVisibility value)
{
    switch (value) {
    case ContentVisibility::Visible:
        return "visible"sv;
    case ContentVisibility::Auto:
        return "auto"sv;
    case ContentVisibility::Hidden:
        return "hidden"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ContextualAltValue> keyword_to_contextual_alt_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Contextual:
        return ContextualAltValue::Contextual;
    case Keyword::NoContextual:
        return ContextualAltValue::NoContextual;
    default:
        return {};
    }
}

Keyword to_keyword(ContextualAltValue contextual_alt_value_value)
{
    switch (contextual_alt_value_value) {
    case ContextualAltValue::Contextual:
        return Keyword::Contextual;
    case ContextualAltValue::NoContextual:
        return Keyword::NoContextual;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ContextualAltValue value)
{
    switch (value) {
    case ContextualAltValue::Contextual:
        return "contextual"sv;
    case ContextualAltValue::NoContextual:
        return "no-contextual"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CoordBox> keyword_to_coord_box(Keyword keyword)
{
    switch (keyword) {
    case Keyword::ContentBox:
        return CoordBox::ContentBox;
    case Keyword::PaddingBox:
        return CoordBox::PaddingBox;
    case Keyword::BorderBox:
        return CoordBox::BorderBox;
    case Keyword::FillBox:
        return CoordBox::FillBox;
    case Keyword::StrokeBox:
        return CoordBox::StrokeBox;
    case Keyword::ViewBox:
        return CoordBox::ViewBox;
    default:
        return {};
    }
}

Keyword to_keyword(CoordBox coord_box_value)
{
    switch (coord_box_value) {
    case CoordBox::ContentBox:
        return Keyword::ContentBox;
    case CoordBox::PaddingBox:
        return Keyword::PaddingBox;
    case CoordBox::BorderBox:
        return Keyword::BorderBox;
    case CoordBox::FillBox:
        return Keyword::FillBox;
    case CoordBox::StrokeBox:
        return Keyword::StrokeBox;
    case CoordBox::ViewBox:
        return Keyword::ViewBox;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CoordBox value)
{
    switch (value) {
    case CoordBox::ContentBox:
        return "content-box"sv;
    case CoordBox::PaddingBox:
        return "padding-box"sv;
    case CoordBox::BorderBox:
        return "border-box"sv;
    case CoordBox::FillBox:
        return "fill-box"sv;
    case CoordBox::StrokeBox:
        return "stroke-box"sv;
    case CoordBox::ViewBox:
        return "view-box"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CounterStyleNameKeyword> keyword_to_counter_style_name_keyword(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Adlam:
        return CounterStyleNameKeyword::Adlam;
    case Keyword::Afar:
        return CounterStyleNameKeyword::Afar;
    case Keyword::Agaw:
        return CounterStyleNameKeyword::Agaw;
    case Keyword::AncientTamil:
        return CounterStyleNameKeyword::AncientTamil;
    case Keyword::ArabicAbjad:
        return CounterStyleNameKeyword::ArabicAbjad;
    case Keyword::ArabicIndic:
        return CounterStyleNameKeyword::ArabicIndic;
    case Keyword::Ari:
        return CounterStyleNameKeyword::Ari;
    case Keyword::Armenian:
        return CounterStyleNameKeyword::Armenian;
    case Keyword::Balinese:
        return CounterStyleNameKeyword::Balinese;
    case Keyword::Bamum:
        return CounterStyleNameKeyword::Bamum;
    case Keyword::Bangla:
        return CounterStyleNameKeyword::Bangla;
    case Keyword::Bengali:
        return CounterStyleNameKeyword::Bengali;
    case Keyword::Binary:
        return CounterStyleNameKeyword::Binary;
    case Keyword::Blin:
        return CounterStyleNameKeyword::Blin;
    case Keyword::Bodo:
        return CounterStyleNameKeyword::Bodo;
    case Keyword::Cambodian:
        return CounterStyleNameKeyword::Cambodian;
    case Keyword::Circle:
        return CounterStyleNameKeyword::Circle;
    case Keyword::CircledDecimal:
        return CounterStyleNameKeyword::CircledDecimal;
    case Keyword::CircledIdeograph:
        return CounterStyleNameKeyword::CircledIdeograph;
    case Keyword::CircledKatakana:
        return CounterStyleNameKeyword::CircledKatakana;
    case Keyword::CircledKoreanConsonant:
        return CounterStyleNameKeyword::CircledKoreanConsonant;
    case Keyword::CircledKoreanSyllable:
        return CounterStyleNameKeyword::CircledKoreanSyllable;
    case Keyword::CircledLowerLatin:
        return CounterStyleNameKeyword::CircledLowerLatin;
    case Keyword::CircledUpperLatin:
        return CounterStyleNameKeyword::CircledUpperLatin;
    case Keyword::CjkDecimal:
        return CounterStyleNameKeyword::CjkDecimal;
    case Keyword::CjkEarthlyBranch:
        return CounterStyleNameKeyword::CjkEarthlyBranch;
    case Keyword::CjkHeavenlyStem:
        return CounterStyleNameKeyword::CjkHeavenlyStem;
    case Keyword::CjkIdeographic:
        return CounterStyleNameKeyword::CjkIdeographic;
    case Keyword::CjkStemBranch:
        return CounterStyleNameKeyword::CjkStemBranch;
    case Keyword::CjkTallyMark:
        return CounterStyleNameKeyword::CjkTallyMark;
    case Keyword::Decimal:
        return CounterStyleNameKeyword::Decimal;
    case Keyword::DecimalLeadingZero:
        return CounterStyleNameKeyword::DecimalLeadingZero;
    case Keyword::Devanagari:
        return CounterStyleNameKeyword::Devanagari;
    case Keyword::Disc:
        return CounterStyleNameKeyword::Disc;
    case Keyword::DisclosureClosed:
        return CounterStyleNameKeyword::DisclosureClosed;
    case Keyword::DisclosureOpen:
        return CounterStyleNameKeyword::DisclosureOpen;
    case Keyword::Dizi:
        return CounterStyleNameKeyword::Dizi;
    case Keyword::Dogri:
        return CounterStyleNameKeyword::Dogri;
    case Keyword::DottedDecimal:
        return CounterStyleNameKeyword::DottedDecimal;
    case Keyword::DoubleCircledDecimal:
        return CounterStyleNameKeyword::DoubleCircledDecimal;
    case Keyword::EthiopicHalehameAm:
        return CounterStyleNameKeyword::EthiopicHalehameAm;
    case Keyword::EthiopicHalehameTiEr:
        return CounterStyleNameKeyword::EthiopicHalehameTiEr;
    case Keyword::EthiopicHalehameTiEt:
        return CounterStyleNameKeyword::EthiopicHalehameTiEt;
    case Keyword::EthiopicHalehame:
        return CounterStyleNameKeyword::EthiopicHalehame;
    case Keyword::EthiopicNumeric:
        return CounterStyleNameKeyword::EthiopicNumeric;
    case Keyword::FilledCircledDecimal:
        return CounterStyleNameKeyword::FilledCircledDecimal;
    case Keyword::FullwidthDecimal:
        return CounterStyleNameKeyword::FullwidthDecimal;
    case Keyword::FullwidthLowerAlpha:
        return CounterStyleNameKeyword::FullwidthLowerAlpha;
    case Keyword::FullwidthUpperAlpha:
        return CounterStyleNameKeyword::FullwidthUpperAlpha;
    case Keyword::FullwidthUpperRoman:
        return CounterStyleNameKeyword::FullwidthUpperRoman;
    case Keyword::Gedeo:
        return CounterStyleNameKeyword::Gedeo;
    case Keyword::Georgian:
        return CounterStyleNameKeyword::Georgian;
    case Keyword::GreekLowerAncient:
        return CounterStyleNameKeyword::GreekLowerAncient;
    case Keyword::GreekLowerModern:
        return CounterStyleNameKeyword::GreekLowerModern;
    case Keyword::GreekUpperAncient:
        return CounterStyleNameKeyword::GreekUpperAncient;
    case Keyword::GreekUpperModern:
        return CounterStyleNameKeyword::GreekUpperModern;
    case Keyword::GujaratiAlpha:
        return CounterStyleNameKeyword::GujaratiAlpha;
    case Keyword::Gujarati:
        return CounterStyleNameKeyword::Gujarati;
    case Keyword::Gumuz:
        return CounterStyleNameKeyword::Gumuz;
    case Keyword::Gurmukhi:
        return CounterStyleNameKeyword::Gurmukhi;
    case Keyword::Hadiyaa:
        return CounterStyleNameKeyword::Hadiyaa;
    case Keyword::HangulConsonant:
        return CounterStyleNameKeyword::HangulConsonant;
    case Keyword::Hangul:
        return CounterStyleNameKeyword::Hangul;
    case Keyword::HanifiRohingya:
        return CounterStyleNameKeyword::HanifiRohingya;
    case Keyword::Harari:
        return CounterStyleNameKeyword::Harari;
    case Keyword::Hebrew:
        return CounterStyleNameKeyword::Hebrew;
    case Keyword::Hindi:
        return CounterStyleNameKeyword::Hindi;
    case Keyword::HiraganaIroha:
        return CounterStyleNameKeyword::HiraganaIroha;
    case Keyword::Hiragana:
        return CounterStyleNameKeyword::Hiragana;
    case Keyword::JapaneseFormal:
        return CounterStyleNameKeyword::JapaneseFormal;
    case Keyword::JapaneseInformal:
        return CounterStyleNameKeyword::JapaneseInformal;
    case Keyword::Javanese:
        return CounterStyleNameKeyword::Javanese;
    case Keyword::Kaffa:
        return CounterStyleNameKeyword::Kaffa;
    case Keyword::KannadaAlpha:
        return CounterStyleNameKeyword::KannadaAlpha;
    case Keyword::Kannada:
        return CounterStyleNameKeyword::Kannada;
    case Keyword::Kashmiri:
        return CounterStyleNameKeyword::Kashmiri;
    case Keyword::KatakanaIroha:
        return CounterStyleNameKeyword::KatakanaIroha;
    case Keyword::Katakana:
        return CounterStyleNameKeyword::Katakana;
    case Keyword::KayahLi:
        return CounterStyleNameKeyword::KayahLi;
    case Keyword::Kebena:
        return CounterStyleNameKeyword::Kebena;
    case Keyword::Kembata:
        return CounterStyleNameKeyword::Kembata;
    case Keyword::KhmerConsonant:
        return CounterStyleNameKeyword::KhmerConsonant;
    case Keyword::Khmer:
        return CounterStyleNameKeyword::Khmer;
    case Keyword::Konkani:
        return CounterStyleNameKeyword::Konkani;
    case Keyword::Konso:
        return CounterStyleNameKeyword::Konso;
    case Keyword::KoreanConsonant:
        return CounterStyleNameKeyword::KoreanConsonant;
    case Keyword::KoreanHangulFormal:
        return CounterStyleNameKeyword::KoreanHangulFormal;
    case Keyword::KoreanHanjaFormal:
        return CounterStyleNameKeyword::KoreanHanjaFormal;
    case Keyword::KoreanHanjaInformal:
        return CounterStyleNameKeyword::KoreanHanjaInformal;
    case Keyword::KoreanSyllable:
        return CounterStyleNameKeyword::KoreanSyllable;
    case Keyword::Kunama:
        return CounterStyleNameKeyword::Kunama;
    case Keyword::LannaHora:
        return CounterStyleNameKeyword::LannaHora;
    case Keyword::LannaTham:
        return CounterStyleNameKeyword::LannaTham;
    case Keyword::Lao:
        return CounterStyleNameKeyword::Lao;
    case Keyword::Lepcha:
        return CounterStyleNameKeyword::Lepcha;
    case Keyword::Limbu:
        return CounterStyleNameKeyword::Limbu;
    case Keyword::LowerAlphaSymbolic:
        return CounterStyleNameKeyword::LowerAlphaSymbolic;
    case Keyword::LowerAlpha:
        return CounterStyleNameKeyword::LowerAlpha;
    case Keyword::LowerArmenian:
        return CounterStyleNameKeyword::LowerArmenian;
    case Keyword::LowerBelorussian:
        return CounterStyleNameKeyword::LowerBelorussian;
    case Keyword::LowerBulgarian:
        return CounterStyleNameKeyword::LowerBulgarian;
    case Keyword::LowerGreek:
        return CounterStyleNameKeyword::LowerGreek;
    case Keyword::LowerHexadecimal:
        return CounterStyleNameKeyword::LowerHexadecimal;
    case Keyword::LowerLatin:
        return CounterStyleNameKeyword::LowerLatin;
    case Keyword::LowerMacedonian:
        return CounterStyleNameKeyword::LowerMacedonian;
    case Keyword::LowerOromoQubee:
        return CounterStyleNameKeyword::LowerOromoQubee;
    case Keyword::LowerRoman:
        return CounterStyleNameKeyword::LowerRoman;
    case Keyword::LowerRussianFull:
        return CounterStyleNameKeyword::LowerRussianFull;
    case Keyword::LowerRussian:
        return CounterStyleNameKeyword::LowerRussian;
    case Keyword::LowerSerbian:
        return CounterStyleNameKeyword::LowerSerbian;
    case Keyword::LowerUkrainianFull:
        return CounterStyleNameKeyword::LowerUkrainianFull;
    case Keyword::LowerUkrainian:
        return CounterStyleNameKeyword::LowerUkrainian;
    case Keyword::MaghrebiAbjad:
        return CounterStyleNameKeyword::MaghrebiAbjad;
    case Keyword::Maithili:
        return CounterStyleNameKeyword::Maithili;
    case Keyword::MalayalamAlpha:
        return CounterStyleNameKeyword::MalayalamAlpha;
    case Keyword::Malayalam:
        return CounterStyleNameKeyword::Malayalam;
    case Keyword::Manipuri:
        return CounterStyleNameKeyword::Manipuri;
    case Keyword::Marathi:
        return CounterStyleNameKeyword::Marathi;
    case Keyword::Meen:
        return CounterStyleNameKeyword::Meen;
    case Keyword::Meetei:
        return CounterStyleNameKeyword::Meetei;
    case Keyword::Mongolian:
        return CounterStyleNameKeyword::Mongolian;
    case Keyword::Mro:
        return CounterStyleNameKeyword::Mro;
    case Keyword::Myanmar:
        return CounterStyleNameKeyword::Myanmar;
    case Keyword::NagMundari:
        return CounterStyleNameKeyword::NagMundari;
    case Keyword::NewBase60:
        return CounterStyleNameKeyword::NewBase60;
    case Keyword::Newa:
        return CounterStyleNameKeyword::Newa;
    case Keyword::NkoCardinal:
        return CounterStyleNameKeyword::NkoCardinal;
    case Keyword::Octal:
        return CounterStyleNameKeyword::Octal;
    case Keyword::Odia:
        return CounterStyleNameKeyword::Odia;
    case Keyword::OlChiki:
        return CounterStyleNameKeyword::OlChiki;
    case Keyword::Oriya:
        return CounterStyleNameKeyword::Oriya;
    case Keyword::Oromo:
        return CounterStyleNameKeyword::Oromo;
    case Keyword::ParenthesizedDecimal:
        return CounterStyleNameKeyword::ParenthesizedDecimal;
    case Keyword::ParenthesizedHangulConsonant:
        return CounterStyleNameKeyword::ParenthesizedHangulConsonant;
    case Keyword::ParenthesizedHangulSyllable:
        return CounterStyleNameKeyword::ParenthesizedHangulSyllable;
    case Keyword::ParenthesizedIdeograph:
        return CounterStyleNameKeyword::ParenthesizedIdeograph;
    case Keyword::ParenthesizedLowerLatin:
        return CounterStyleNameKeyword::ParenthesizedLowerLatin;
    case Keyword::PersianAbjad:
        return CounterStyleNameKeyword::PersianAbjad;
    case Keyword::PersianAlphabetic:
        return CounterStyleNameKeyword::PersianAlphabetic;
    case Keyword::Persian:
        return CounterStyleNameKeyword::Persian;
    case Keyword::Punjabi:
        return CounterStyleNameKeyword::Punjabi;
    case Keyword::Saho:
        return CounterStyleNameKeyword::Saho;
    case Keyword::Sanskrit:
        return CounterStyleNameKeyword::Sanskrit;
    case Keyword::Santali:
        return CounterStyleNameKeyword::Santali;
    case Keyword::Shan:
        return CounterStyleNameKeyword::Shan;
    case Keyword::Sidama:
        return CounterStyleNameKeyword::Sidama;
    case Keyword::Silti:
        return CounterStyleNameKeyword::Silti;
    case Keyword::SimpChineseFormal:
        return CounterStyleNameKeyword::SimpChineseFormal;
    case Keyword::SimpChineseInformal:
        return CounterStyleNameKeyword::SimpChineseInformal;
    case Keyword::SimpleLowerRoman:
        return CounterStyleNameKeyword::SimpleLowerRoman;
    case Keyword::SimpleUpperRoman:
        return CounterStyleNameKeyword::SimpleUpperRoman;
    case Keyword::Sindhi:
        return CounterStyleNameKeyword::Sindhi;
    case Keyword::Square:
        return CounterStyleNameKeyword::Square;
    case Keyword::Sundanese:
        return CounterStyleNameKeyword::Sundanese;
    case Keyword::SuperDecimal:
        return CounterStyleNameKeyword::SuperDecimal;
    case Keyword::TaiLue:
        return CounterStyleNameKeyword::TaiLue;
    case Keyword::TallyMark:
        return CounterStyleNameKeyword::TallyMark;
    case Keyword::Tamil:
        return CounterStyleNameKeyword::Tamil;
    case Keyword::TeluguAlpha:
        return CounterStyleNameKeyword::TeluguAlpha;
    case Keyword::Telugu:
        return CounterStyleNameKeyword::Telugu;
    case Keyword::ThaiAlpha:
        return CounterStyleNameKeyword::ThaiAlpha;
    case Keyword::Thai:
        return CounterStyleNameKeyword::Thai;
    case Keyword::Tibetan:
        return CounterStyleNameKeyword::Tibetan;
    case Keyword::Tigre:
        return CounterStyleNameKeyword::Tigre;
    case Keyword::TradChineseFormal:
        return CounterStyleNameKeyword::TradChineseFormal;
    case Keyword::TradChineseInformal:
        return CounterStyleNameKeyword::TradChineseInformal;
    case Keyword::UpperAlphaSymbolic:
        return CounterStyleNameKeyword::UpperAlphaSymbolic;
    case Keyword::UpperAlpha:
        return CounterStyleNameKeyword::UpperAlpha;
    case Keyword::UpperArmenian:
        return CounterStyleNameKeyword::UpperArmenian;
    case Keyword::UpperBelorussian:
        return CounterStyleNameKeyword::UpperBelorussian;
    case Keyword::UpperBulgarian:
        return CounterStyleNameKeyword::UpperBulgarian;
    case Keyword::UpperHexadecimal:
        return CounterStyleNameKeyword::UpperHexadecimal;
    case Keyword::UpperLatin:
        return CounterStyleNameKeyword::UpperLatin;
    case Keyword::UpperMacedonian:
        return CounterStyleNameKeyword::UpperMacedonian;
    case Keyword::UpperOromoQubee:
        return CounterStyleNameKeyword::UpperOromoQubee;
    case Keyword::UpperRoman:
        return CounterStyleNameKeyword::UpperRoman;
    case Keyword::UpperRussianFull:
        return CounterStyleNameKeyword::UpperRussianFull;
    case Keyword::UpperRussian:
        return CounterStyleNameKeyword::UpperRussian;
    case Keyword::UpperSerbian:
        return CounterStyleNameKeyword::UpperSerbian;
    case Keyword::UpperUkrainianFull:
        return CounterStyleNameKeyword::UpperUkrainianFull;
    case Keyword::UpperUkrainian:
        return CounterStyleNameKeyword::UpperUkrainian;
    case Keyword::UrduAbjad:
        return CounterStyleNameKeyword::UrduAbjad;
    case Keyword::UrduAlphabetic:
        return CounterStyleNameKeyword::UrduAlphabetic;
    case Keyword::Urdu:
        return CounterStyleNameKeyword::Urdu;
    case Keyword::WarangCiti:
        return CounterStyleNameKeyword::WarangCiti;
    case Keyword::Wolaita:
        return CounterStyleNameKeyword::Wolaita;
    case Keyword::Yemsa:
        return CounterStyleNameKeyword::Yemsa;
    case Keyword::Zhuyin:
        return CounterStyleNameKeyword::Zhuyin;
    default:
        return {};
    }
}

Keyword to_keyword(CounterStyleNameKeyword counter_style_name_keyword_value)
{
    switch (counter_style_name_keyword_value) {
    case CounterStyleNameKeyword::Adlam:
        return Keyword::Adlam;
    case CounterStyleNameKeyword::Afar:
        return Keyword::Afar;
    case CounterStyleNameKeyword::Agaw:
        return Keyword::Agaw;
    case CounterStyleNameKeyword::AncientTamil:
        return Keyword::AncientTamil;
    case CounterStyleNameKeyword::ArabicAbjad:
        return Keyword::ArabicAbjad;
    case CounterStyleNameKeyword::ArabicIndic:
        return Keyword::ArabicIndic;
    case CounterStyleNameKeyword::Ari:
        return Keyword::Ari;
    case CounterStyleNameKeyword::Armenian:
        return Keyword::Armenian;
    case CounterStyleNameKeyword::Balinese:
        return Keyword::Balinese;
    case CounterStyleNameKeyword::Bamum:
        return Keyword::Bamum;
    case CounterStyleNameKeyword::Bangla:
        return Keyword::Bangla;
    case CounterStyleNameKeyword::Bengali:
        return Keyword::Bengali;
    case CounterStyleNameKeyword::Binary:
        return Keyword::Binary;
    case CounterStyleNameKeyword::Blin:
        return Keyword::Blin;
    case CounterStyleNameKeyword::Bodo:
        return Keyword::Bodo;
    case CounterStyleNameKeyword::Cambodian:
        return Keyword::Cambodian;
    case CounterStyleNameKeyword::Circle:
        return Keyword::Circle;
    case CounterStyleNameKeyword::CircledDecimal:
        return Keyword::CircledDecimal;
    case CounterStyleNameKeyword::CircledIdeograph:
        return Keyword::CircledIdeograph;
    case CounterStyleNameKeyword::CircledKatakana:
        return Keyword::CircledKatakana;
    case CounterStyleNameKeyword::CircledKoreanConsonant:
        return Keyword::CircledKoreanConsonant;
    case CounterStyleNameKeyword::CircledKoreanSyllable:
        return Keyword::CircledKoreanSyllable;
    case CounterStyleNameKeyword::CircledLowerLatin:
        return Keyword::CircledLowerLatin;
    case CounterStyleNameKeyword::CircledUpperLatin:
        return Keyword::CircledUpperLatin;
    case CounterStyleNameKeyword::CjkDecimal:
        return Keyword::CjkDecimal;
    case CounterStyleNameKeyword::CjkEarthlyBranch:
        return Keyword::CjkEarthlyBranch;
    case CounterStyleNameKeyword::CjkHeavenlyStem:
        return Keyword::CjkHeavenlyStem;
    case CounterStyleNameKeyword::CjkIdeographic:
        return Keyword::CjkIdeographic;
    case CounterStyleNameKeyword::CjkStemBranch:
        return Keyword::CjkStemBranch;
    case CounterStyleNameKeyword::CjkTallyMark:
        return Keyword::CjkTallyMark;
    case CounterStyleNameKeyword::Decimal:
        return Keyword::Decimal;
    case CounterStyleNameKeyword::DecimalLeadingZero:
        return Keyword::DecimalLeadingZero;
    case CounterStyleNameKeyword::Devanagari:
        return Keyword::Devanagari;
    case CounterStyleNameKeyword::Disc:
        return Keyword::Disc;
    case CounterStyleNameKeyword::DisclosureClosed:
        return Keyword::DisclosureClosed;
    case CounterStyleNameKeyword::DisclosureOpen:
        return Keyword::DisclosureOpen;
    case CounterStyleNameKeyword::Dizi:
        return Keyword::Dizi;
    case CounterStyleNameKeyword::Dogri:
        return Keyword::Dogri;
    case CounterStyleNameKeyword::DottedDecimal:
        return Keyword::DottedDecimal;
    case CounterStyleNameKeyword::DoubleCircledDecimal:
        return Keyword::DoubleCircledDecimal;
    case CounterStyleNameKeyword::EthiopicHalehameAm:
        return Keyword::EthiopicHalehameAm;
    case CounterStyleNameKeyword::EthiopicHalehameTiEr:
        return Keyword::EthiopicHalehameTiEr;
    case CounterStyleNameKeyword::EthiopicHalehameTiEt:
        return Keyword::EthiopicHalehameTiEt;
    case CounterStyleNameKeyword::EthiopicHalehame:
        return Keyword::EthiopicHalehame;
    case CounterStyleNameKeyword::EthiopicNumeric:
        return Keyword::EthiopicNumeric;
    case CounterStyleNameKeyword::FilledCircledDecimal:
        return Keyword::FilledCircledDecimal;
    case CounterStyleNameKeyword::FullwidthDecimal:
        return Keyword::FullwidthDecimal;
    case CounterStyleNameKeyword::FullwidthLowerAlpha:
        return Keyword::FullwidthLowerAlpha;
    case CounterStyleNameKeyword::FullwidthUpperAlpha:
        return Keyword::FullwidthUpperAlpha;
    case CounterStyleNameKeyword::FullwidthUpperRoman:
        return Keyword::FullwidthUpperRoman;
    case CounterStyleNameKeyword::Gedeo:
        return Keyword::Gedeo;
    case CounterStyleNameKeyword::Georgian:
        return Keyword::Georgian;
    case CounterStyleNameKeyword::GreekLowerAncient:
        return Keyword::GreekLowerAncient;
    case CounterStyleNameKeyword::GreekLowerModern:
        return Keyword::GreekLowerModern;
    case CounterStyleNameKeyword::GreekUpperAncient:
        return Keyword::GreekUpperAncient;
    case CounterStyleNameKeyword::GreekUpperModern:
        return Keyword::GreekUpperModern;
    case CounterStyleNameKeyword::GujaratiAlpha:
        return Keyword::GujaratiAlpha;
    case CounterStyleNameKeyword::Gujarati:
        return Keyword::Gujarati;
    case CounterStyleNameKeyword::Gumuz:
        return Keyword::Gumuz;
    case CounterStyleNameKeyword::Gurmukhi:
        return Keyword::Gurmukhi;
    case CounterStyleNameKeyword::Hadiyaa:
        return Keyword::Hadiyaa;
    case CounterStyleNameKeyword::HangulConsonant:
        return Keyword::HangulConsonant;
    case CounterStyleNameKeyword::Hangul:
        return Keyword::Hangul;
    case CounterStyleNameKeyword::HanifiRohingya:
        return Keyword::HanifiRohingya;
    case CounterStyleNameKeyword::Harari:
        return Keyword::Harari;
    case CounterStyleNameKeyword::Hebrew:
        return Keyword::Hebrew;
    case CounterStyleNameKeyword::Hindi:
        return Keyword::Hindi;
    case CounterStyleNameKeyword::HiraganaIroha:
        return Keyword::HiraganaIroha;
    case CounterStyleNameKeyword::Hiragana:
        return Keyword::Hiragana;
    case CounterStyleNameKeyword::JapaneseFormal:
        return Keyword::JapaneseFormal;
    case CounterStyleNameKeyword::JapaneseInformal:
        return Keyword::JapaneseInformal;
    case CounterStyleNameKeyword::Javanese:
        return Keyword::Javanese;
    case CounterStyleNameKeyword::Kaffa:
        return Keyword::Kaffa;
    case CounterStyleNameKeyword::KannadaAlpha:
        return Keyword::KannadaAlpha;
    case CounterStyleNameKeyword::Kannada:
        return Keyword::Kannada;
    case CounterStyleNameKeyword::Kashmiri:
        return Keyword::Kashmiri;
    case CounterStyleNameKeyword::KatakanaIroha:
        return Keyword::KatakanaIroha;
    case CounterStyleNameKeyword::Katakana:
        return Keyword::Katakana;
    case CounterStyleNameKeyword::KayahLi:
        return Keyword::KayahLi;
    case CounterStyleNameKeyword::Kebena:
        return Keyword::Kebena;
    case CounterStyleNameKeyword::Kembata:
        return Keyword::Kembata;
    case CounterStyleNameKeyword::KhmerConsonant:
        return Keyword::KhmerConsonant;
    case CounterStyleNameKeyword::Khmer:
        return Keyword::Khmer;
    case CounterStyleNameKeyword::Konkani:
        return Keyword::Konkani;
    case CounterStyleNameKeyword::Konso:
        return Keyword::Konso;
    case CounterStyleNameKeyword::KoreanConsonant:
        return Keyword::KoreanConsonant;
    case CounterStyleNameKeyword::KoreanHangulFormal:
        return Keyword::KoreanHangulFormal;
    case CounterStyleNameKeyword::KoreanHanjaFormal:
        return Keyword::KoreanHanjaFormal;
    case CounterStyleNameKeyword::KoreanHanjaInformal:
        return Keyword::KoreanHanjaInformal;
    case CounterStyleNameKeyword::KoreanSyllable:
        return Keyword::KoreanSyllable;
    case CounterStyleNameKeyword::Kunama:
        return Keyword::Kunama;
    case CounterStyleNameKeyword::LannaHora:
        return Keyword::LannaHora;
    case CounterStyleNameKeyword::LannaTham:
        return Keyword::LannaTham;
    case CounterStyleNameKeyword::Lao:
        return Keyword::Lao;
    case CounterStyleNameKeyword::Lepcha:
        return Keyword::Lepcha;
    case CounterStyleNameKeyword::Limbu:
        return Keyword::Limbu;
    case CounterStyleNameKeyword::LowerAlphaSymbolic:
        return Keyword::LowerAlphaSymbolic;
    case CounterStyleNameKeyword::LowerAlpha:
        return Keyword::LowerAlpha;
    case CounterStyleNameKeyword::LowerArmenian:
        return Keyword::LowerArmenian;
    case CounterStyleNameKeyword::LowerBelorussian:
        return Keyword::LowerBelorussian;
    case CounterStyleNameKeyword::LowerBulgarian:
        return Keyword::LowerBulgarian;
    case CounterStyleNameKeyword::LowerGreek:
        return Keyword::LowerGreek;
    case CounterStyleNameKeyword::LowerHexadecimal:
        return Keyword::LowerHexadecimal;
    case CounterStyleNameKeyword::LowerLatin:
        return Keyword::LowerLatin;
    case CounterStyleNameKeyword::LowerMacedonian:
        return Keyword::LowerMacedonian;
    case CounterStyleNameKeyword::LowerOromoQubee:
        return Keyword::LowerOromoQubee;
    case CounterStyleNameKeyword::LowerRoman:
        return Keyword::LowerRoman;
    case CounterStyleNameKeyword::LowerRussianFull:
        return Keyword::LowerRussianFull;
    case CounterStyleNameKeyword::LowerRussian:
        return Keyword::LowerRussian;
    case CounterStyleNameKeyword::LowerSerbian:
        return Keyword::LowerSerbian;
    case CounterStyleNameKeyword::LowerUkrainianFull:
        return Keyword::LowerUkrainianFull;
    case CounterStyleNameKeyword::LowerUkrainian:
        return Keyword::LowerUkrainian;
    case CounterStyleNameKeyword::MaghrebiAbjad:
        return Keyword::MaghrebiAbjad;
    case CounterStyleNameKeyword::Maithili:
        return Keyword::Maithili;
    case CounterStyleNameKeyword::MalayalamAlpha:
        return Keyword::MalayalamAlpha;
    case CounterStyleNameKeyword::Malayalam:
        return Keyword::Malayalam;
    case CounterStyleNameKeyword::Manipuri:
        return Keyword::Manipuri;
    case CounterStyleNameKeyword::Marathi:
        return Keyword::Marathi;
    case CounterStyleNameKeyword::Meen:
        return Keyword::Meen;
    case CounterStyleNameKeyword::Meetei:
        return Keyword::Meetei;
    case CounterStyleNameKeyword::Mongolian:
        return Keyword::Mongolian;
    case CounterStyleNameKeyword::Mro:
        return Keyword::Mro;
    case CounterStyleNameKeyword::Myanmar:
        return Keyword::Myanmar;
    case CounterStyleNameKeyword::NagMundari:
        return Keyword::NagMundari;
    case CounterStyleNameKeyword::NewBase60:
        return Keyword::NewBase60;
    case CounterStyleNameKeyword::Newa:
        return Keyword::Newa;
    case CounterStyleNameKeyword::NkoCardinal:
        return Keyword::NkoCardinal;
    case CounterStyleNameKeyword::Octal:
        return Keyword::Octal;
    case CounterStyleNameKeyword::Odia:
        return Keyword::Odia;
    case CounterStyleNameKeyword::OlChiki:
        return Keyword::OlChiki;
    case CounterStyleNameKeyword::Oriya:
        return Keyword::Oriya;
    case CounterStyleNameKeyword::Oromo:
        return Keyword::Oromo;
    case CounterStyleNameKeyword::ParenthesizedDecimal:
        return Keyword::ParenthesizedDecimal;
    case CounterStyleNameKeyword::ParenthesizedHangulConsonant:
        return Keyword::ParenthesizedHangulConsonant;
    case CounterStyleNameKeyword::ParenthesizedHangulSyllable:
        return Keyword::ParenthesizedHangulSyllable;
    case CounterStyleNameKeyword::ParenthesizedIdeograph:
        return Keyword::ParenthesizedIdeograph;
    case CounterStyleNameKeyword::ParenthesizedLowerLatin:
        return Keyword::ParenthesizedLowerLatin;
    case CounterStyleNameKeyword::PersianAbjad:
        return Keyword::PersianAbjad;
    case CounterStyleNameKeyword::PersianAlphabetic:
        return Keyword::PersianAlphabetic;
    case CounterStyleNameKeyword::Persian:
        return Keyword::Persian;
    case CounterStyleNameKeyword::Punjabi:
        return Keyword::Punjabi;
    case CounterStyleNameKeyword::Saho:
        return Keyword::Saho;
    case CounterStyleNameKeyword::Sanskrit:
        return Keyword::Sanskrit;
    case CounterStyleNameKeyword::Santali:
        return Keyword::Santali;
    case CounterStyleNameKeyword::Shan:
        return Keyword::Shan;
    case CounterStyleNameKeyword::Sidama:
        return Keyword::Sidama;
    case CounterStyleNameKeyword::Silti:
        return Keyword::Silti;
    case CounterStyleNameKeyword::SimpChineseFormal:
        return Keyword::SimpChineseFormal;
    case CounterStyleNameKeyword::SimpChineseInformal:
        return Keyword::SimpChineseInformal;
    case CounterStyleNameKeyword::SimpleLowerRoman:
        return Keyword::SimpleLowerRoman;
    case CounterStyleNameKeyword::SimpleUpperRoman:
        return Keyword::SimpleUpperRoman;
    case CounterStyleNameKeyword::Sindhi:
        return Keyword::Sindhi;
    case CounterStyleNameKeyword::Square:
        return Keyword::Square;
    case CounterStyleNameKeyword::Sundanese:
        return Keyword::Sundanese;
    case CounterStyleNameKeyword::SuperDecimal:
        return Keyword::SuperDecimal;
    case CounterStyleNameKeyword::TaiLue:
        return Keyword::TaiLue;
    case CounterStyleNameKeyword::TallyMark:
        return Keyword::TallyMark;
    case CounterStyleNameKeyword::Tamil:
        return Keyword::Tamil;
    case CounterStyleNameKeyword::TeluguAlpha:
        return Keyword::TeluguAlpha;
    case CounterStyleNameKeyword::Telugu:
        return Keyword::Telugu;
    case CounterStyleNameKeyword::ThaiAlpha:
        return Keyword::ThaiAlpha;
    case CounterStyleNameKeyword::Thai:
        return Keyword::Thai;
    case CounterStyleNameKeyword::Tibetan:
        return Keyword::Tibetan;
    case CounterStyleNameKeyword::Tigre:
        return Keyword::Tigre;
    case CounterStyleNameKeyword::TradChineseFormal:
        return Keyword::TradChineseFormal;
    case CounterStyleNameKeyword::TradChineseInformal:
        return Keyword::TradChineseInformal;
    case CounterStyleNameKeyword::UpperAlphaSymbolic:
        return Keyword::UpperAlphaSymbolic;
    case CounterStyleNameKeyword::UpperAlpha:
        return Keyword::UpperAlpha;
    case CounterStyleNameKeyword::UpperArmenian:
        return Keyword::UpperArmenian;
    case CounterStyleNameKeyword::UpperBelorussian:
        return Keyword::UpperBelorussian;
    case CounterStyleNameKeyword::UpperBulgarian:
        return Keyword::UpperBulgarian;
    case CounterStyleNameKeyword::UpperHexadecimal:
        return Keyword::UpperHexadecimal;
    case CounterStyleNameKeyword::UpperLatin:
        return Keyword::UpperLatin;
    case CounterStyleNameKeyword::UpperMacedonian:
        return Keyword::UpperMacedonian;
    case CounterStyleNameKeyword::UpperOromoQubee:
        return Keyword::UpperOromoQubee;
    case CounterStyleNameKeyword::UpperRoman:
        return Keyword::UpperRoman;
    case CounterStyleNameKeyword::UpperRussianFull:
        return Keyword::UpperRussianFull;
    case CounterStyleNameKeyword::UpperRussian:
        return Keyword::UpperRussian;
    case CounterStyleNameKeyword::UpperSerbian:
        return Keyword::UpperSerbian;
    case CounterStyleNameKeyword::UpperUkrainianFull:
        return Keyword::UpperUkrainianFull;
    case CounterStyleNameKeyword::UpperUkrainian:
        return Keyword::UpperUkrainian;
    case CounterStyleNameKeyword::UrduAbjad:
        return Keyword::UrduAbjad;
    case CounterStyleNameKeyword::UrduAlphabetic:
        return Keyword::UrduAlphabetic;
    case CounterStyleNameKeyword::Urdu:
        return Keyword::Urdu;
    case CounterStyleNameKeyword::WarangCiti:
        return Keyword::WarangCiti;
    case CounterStyleNameKeyword::Wolaita:
        return Keyword::Wolaita;
    case CounterStyleNameKeyword::Yemsa:
        return Keyword::Yemsa;
    case CounterStyleNameKeyword::Zhuyin:
        return Keyword::Zhuyin;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CounterStyleNameKeyword value)
{
    switch (value) {
    case CounterStyleNameKeyword::Adlam:
        return "adlam"sv;
    case CounterStyleNameKeyword::Afar:
        return "afar"sv;
    case CounterStyleNameKeyword::Agaw:
        return "agaw"sv;
    case CounterStyleNameKeyword::AncientTamil:
        return "ancient-tamil"sv;
    case CounterStyleNameKeyword::ArabicAbjad:
        return "arabic-abjad"sv;
    case CounterStyleNameKeyword::ArabicIndic:
        return "arabic-indic"sv;
    case CounterStyleNameKeyword::Ari:
        return "ari"sv;
    case CounterStyleNameKeyword::Armenian:
        return "armenian"sv;
    case CounterStyleNameKeyword::Balinese:
        return "balinese"sv;
    case CounterStyleNameKeyword::Bamum:
        return "bamum"sv;
    case CounterStyleNameKeyword::Bangla:
        return "bangla"sv;
    case CounterStyleNameKeyword::Bengali:
        return "bengali"sv;
    case CounterStyleNameKeyword::Binary:
        return "binary"sv;
    case CounterStyleNameKeyword::Blin:
        return "blin"sv;
    case CounterStyleNameKeyword::Bodo:
        return "bodo"sv;
    case CounterStyleNameKeyword::Cambodian:
        return "cambodian"sv;
    case CounterStyleNameKeyword::Circle:
        return "circle"sv;
    case CounterStyleNameKeyword::CircledDecimal:
        return "circled-decimal"sv;
    case CounterStyleNameKeyword::CircledIdeograph:
        return "circled-ideograph"sv;
    case CounterStyleNameKeyword::CircledKatakana:
        return "circled-katakana"sv;
    case CounterStyleNameKeyword::CircledKoreanConsonant:
        return "circled-korean-consonant"sv;
    case CounterStyleNameKeyword::CircledKoreanSyllable:
        return "circled-korean-syllable"sv;
    case CounterStyleNameKeyword::CircledLowerLatin:
        return "circled-lower-latin"sv;
    case CounterStyleNameKeyword::CircledUpperLatin:
        return "circled-upper-latin"sv;
    case CounterStyleNameKeyword::CjkDecimal:
        return "cjk-decimal"sv;
    case CounterStyleNameKeyword::CjkEarthlyBranch:
        return "cjk-earthly-branch"sv;
    case CounterStyleNameKeyword::CjkHeavenlyStem:
        return "cjk-heavenly-stem"sv;
    case CounterStyleNameKeyword::CjkIdeographic:
        return "cjk-ideographic"sv;
    case CounterStyleNameKeyword::CjkStemBranch:
        return "cjk-stem-branch"sv;
    case CounterStyleNameKeyword::CjkTallyMark:
        return "cjk-tally-mark"sv;
    case CounterStyleNameKeyword::Decimal:
        return "decimal"sv;
    case CounterStyleNameKeyword::DecimalLeadingZero:
        return "decimal-leading-zero"sv;
    case CounterStyleNameKeyword::Devanagari:
        return "devanagari"sv;
    case CounterStyleNameKeyword::Disc:
        return "disc"sv;
    case CounterStyleNameKeyword::DisclosureClosed:
        return "disclosure-closed"sv;
    case CounterStyleNameKeyword::DisclosureOpen:
        return "disclosure-open"sv;
    case CounterStyleNameKeyword::Dizi:
        return "dizi"sv;
    case CounterStyleNameKeyword::Dogri:
        return "dogri"sv;
    case CounterStyleNameKeyword::DottedDecimal:
        return "dotted-decimal"sv;
    case CounterStyleNameKeyword::DoubleCircledDecimal:
        return "double-circled-decimal"sv;
    case CounterStyleNameKeyword::EthiopicHalehameAm:
        return "ethiopic-halehame-am"sv;
    case CounterStyleNameKeyword::EthiopicHalehameTiEr:
        return "ethiopic-halehame-ti-er"sv;
    case CounterStyleNameKeyword::EthiopicHalehameTiEt:
        return "ethiopic-halehame-ti-et"sv;
    case CounterStyleNameKeyword::EthiopicHalehame:
        return "ethiopic-halehame"sv;
    case CounterStyleNameKeyword::EthiopicNumeric:
        return "ethiopic-numeric"sv;
    case CounterStyleNameKeyword::FilledCircledDecimal:
        return "filled-circled-decimal"sv;
    case CounterStyleNameKeyword::FullwidthDecimal:
        return "fullwidth-decimal"sv;
    case CounterStyleNameKeyword::FullwidthLowerAlpha:
        return "fullwidth-lower-alpha"sv;
    case CounterStyleNameKeyword::FullwidthUpperAlpha:
        return "fullwidth-upper-alpha"sv;
    case CounterStyleNameKeyword::FullwidthUpperRoman:
        return "fullwidth-upper-roman"sv;
    case CounterStyleNameKeyword::Gedeo:
        return "gedeo"sv;
    case CounterStyleNameKeyword::Georgian:
        return "georgian"sv;
    case CounterStyleNameKeyword::GreekLowerAncient:
        return "greek-lower-ancient"sv;
    case CounterStyleNameKeyword::GreekLowerModern:
        return "greek-lower-modern"sv;
    case CounterStyleNameKeyword::GreekUpperAncient:
        return "greek-upper-ancient"sv;
    case CounterStyleNameKeyword::GreekUpperModern:
        return "greek-upper-modern"sv;
    case CounterStyleNameKeyword::GujaratiAlpha:
        return "gujarati-alpha"sv;
    case CounterStyleNameKeyword::Gujarati:
        return "gujarati"sv;
    case CounterStyleNameKeyword::Gumuz:
        return "gumuz"sv;
    case CounterStyleNameKeyword::Gurmukhi:
        return "gurmukhi"sv;
    case CounterStyleNameKeyword::Hadiyaa:
        return "hadiyaa"sv;
    case CounterStyleNameKeyword::HangulConsonant:
        return "hangul-consonant"sv;
    case CounterStyleNameKeyword::Hangul:
        return "hangul"sv;
    case CounterStyleNameKeyword::HanifiRohingya:
        return "hanifi-rohingya"sv;
    case CounterStyleNameKeyword::Harari:
        return "harari"sv;
    case CounterStyleNameKeyword::Hebrew:
        return "hebrew"sv;
    case CounterStyleNameKeyword::Hindi:
        return "hindi"sv;
    case CounterStyleNameKeyword::HiraganaIroha:
        return "hiragana-iroha"sv;
    case CounterStyleNameKeyword::Hiragana:
        return "hiragana"sv;
    case CounterStyleNameKeyword::JapaneseFormal:
        return "japanese-formal"sv;
    case CounterStyleNameKeyword::JapaneseInformal:
        return "japanese-informal"sv;
    case CounterStyleNameKeyword::Javanese:
        return "javanese"sv;
    case CounterStyleNameKeyword::Kaffa:
        return "kaffa"sv;
    case CounterStyleNameKeyword::KannadaAlpha:
        return "kannada-alpha"sv;
    case CounterStyleNameKeyword::Kannada:
        return "kannada"sv;
    case CounterStyleNameKeyword::Kashmiri:
        return "kashmiri"sv;
    case CounterStyleNameKeyword::KatakanaIroha:
        return "katakana-iroha"sv;
    case CounterStyleNameKeyword::Katakana:
        return "katakana"sv;
    case CounterStyleNameKeyword::KayahLi:
        return "kayah-li"sv;
    case CounterStyleNameKeyword::Kebena:
        return "kebena"sv;
    case CounterStyleNameKeyword::Kembata:
        return "kembata"sv;
    case CounterStyleNameKeyword::KhmerConsonant:
        return "khmer-consonant"sv;
    case CounterStyleNameKeyword::Khmer:
        return "khmer"sv;
    case CounterStyleNameKeyword::Konkani:
        return "konkani"sv;
    case CounterStyleNameKeyword::Konso:
        return "konso"sv;
    case CounterStyleNameKeyword::KoreanConsonant:
        return "korean-consonant"sv;
    case CounterStyleNameKeyword::KoreanHangulFormal:
        return "korean-hangul-formal"sv;
    case CounterStyleNameKeyword::KoreanHanjaFormal:
        return "korean-hanja-formal"sv;
    case CounterStyleNameKeyword::KoreanHanjaInformal:
        return "korean-hanja-informal"sv;
    case CounterStyleNameKeyword::KoreanSyllable:
        return "korean-syllable"sv;
    case CounterStyleNameKeyword::Kunama:
        return "kunama"sv;
    case CounterStyleNameKeyword::LannaHora:
        return "lanna-hora"sv;
    case CounterStyleNameKeyword::LannaTham:
        return "lanna-tham"sv;
    case CounterStyleNameKeyword::Lao:
        return "lao"sv;
    case CounterStyleNameKeyword::Lepcha:
        return "lepcha"sv;
    case CounterStyleNameKeyword::Limbu:
        return "limbu"sv;
    case CounterStyleNameKeyword::LowerAlphaSymbolic:
        return "lower-alpha-symbolic"sv;
    case CounterStyleNameKeyword::LowerAlpha:
        return "lower-alpha"sv;
    case CounterStyleNameKeyword::LowerArmenian:
        return "lower-armenian"sv;
    case CounterStyleNameKeyword::LowerBelorussian:
        return "lower-belorussian"sv;
    case CounterStyleNameKeyword::LowerBulgarian:
        return "lower-bulgarian"sv;
    case CounterStyleNameKeyword::LowerGreek:
        return "lower-greek"sv;
    case CounterStyleNameKeyword::LowerHexadecimal:
        return "lower-hexadecimal"sv;
    case CounterStyleNameKeyword::LowerLatin:
        return "lower-latin"sv;
    case CounterStyleNameKeyword::LowerMacedonian:
        return "lower-macedonian"sv;
    case CounterStyleNameKeyword::LowerOromoQubee:
        return "lower-oromo-qubee"sv;
    case CounterStyleNameKeyword::LowerRoman:
        return "lower-roman"sv;
    case CounterStyleNameKeyword::LowerRussianFull:
        return "lower-russian-full"sv;
    case CounterStyleNameKeyword::LowerRussian:
        return "lower-russian"sv;
    case CounterStyleNameKeyword::LowerSerbian:
        return "lower-serbian"sv;
    case CounterStyleNameKeyword::LowerUkrainianFull:
        return "lower-ukrainian-full"sv;
    case CounterStyleNameKeyword::LowerUkrainian:
        return "lower-ukrainian"sv;
    case CounterStyleNameKeyword::MaghrebiAbjad:
        return "maghrebi-abjad"sv;
    case CounterStyleNameKeyword::Maithili:
        return "maithili"sv;
    case CounterStyleNameKeyword::MalayalamAlpha:
        return "malayalam-alpha"sv;
    case CounterStyleNameKeyword::Malayalam:
        return "malayalam"sv;
    case CounterStyleNameKeyword::Manipuri:
        return "manipuri"sv;
    case CounterStyleNameKeyword::Marathi:
        return "marathi"sv;
    case CounterStyleNameKeyword::Meen:
        return "meen"sv;
    case CounterStyleNameKeyword::Meetei:
        return "meetei"sv;
    case CounterStyleNameKeyword::Mongolian:
        return "mongolian"sv;
    case CounterStyleNameKeyword::Mro:
        return "mro"sv;
    case CounterStyleNameKeyword::Myanmar:
        return "myanmar"sv;
    case CounterStyleNameKeyword::NagMundari:
        return "nag-mundari"sv;
    case CounterStyleNameKeyword::NewBase60:
        return "new-base-60"sv;
    case CounterStyleNameKeyword::Newa:
        return "newa"sv;
    case CounterStyleNameKeyword::NkoCardinal:
        return "nko-cardinal"sv;
    case CounterStyleNameKeyword::Octal:
        return "octal"sv;
    case CounterStyleNameKeyword::Odia:
        return "odia"sv;
    case CounterStyleNameKeyword::OlChiki:
        return "ol-chiki"sv;
    case CounterStyleNameKeyword::Oriya:
        return "oriya"sv;
    case CounterStyleNameKeyword::Oromo:
        return "oromo"sv;
    case CounterStyleNameKeyword::ParenthesizedDecimal:
        return "parenthesized-decimal"sv;
    case CounterStyleNameKeyword::ParenthesizedHangulConsonant:
        return "parenthesized-hangul-consonant"sv;
    case CounterStyleNameKeyword::ParenthesizedHangulSyllable:
        return "parenthesized-hangul-syllable"sv;
    case CounterStyleNameKeyword::ParenthesizedIdeograph:
        return "parenthesized-ideograph"sv;
    case CounterStyleNameKeyword::ParenthesizedLowerLatin:
        return "parenthesized-lower-latin"sv;
    case CounterStyleNameKeyword::PersianAbjad:
        return "persian-abjad"sv;
    case CounterStyleNameKeyword::PersianAlphabetic:
        return "persian-alphabetic"sv;
    case CounterStyleNameKeyword::Persian:
        return "persian"sv;
    case CounterStyleNameKeyword::Punjabi:
        return "punjabi"sv;
    case CounterStyleNameKeyword::Saho:
        return "saho"sv;
    case CounterStyleNameKeyword::Sanskrit:
        return "sanskrit"sv;
    case CounterStyleNameKeyword::Santali:
        return "santali"sv;
    case CounterStyleNameKeyword::Shan:
        return "shan"sv;
    case CounterStyleNameKeyword::Sidama:
        return "sidama"sv;
    case CounterStyleNameKeyword::Silti:
        return "silti"sv;
    case CounterStyleNameKeyword::SimpChineseFormal:
        return "simp-chinese-formal"sv;
    case CounterStyleNameKeyword::SimpChineseInformal:
        return "simp-chinese-informal"sv;
    case CounterStyleNameKeyword::SimpleLowerRoman:
        return "simple-lower-roman"sv;
    case CounterStyleNameKeyword::SimpleUpperRoman:
        return "simple-upper-roman"sv;
    case CounterStyleNameKeyword::Sindhi:
        return "sindhi"sv;
    case CounterStyleNameKeyword::Square:
        return "square"sv;
    case CounterStyleNameKeyword::Sundanese:
        return "sundanese"sv;
    case CounterStyleNameKeyword::SuperDecimal:
        return "super-decimal"sv;
    case CounterStyleNameKeyword::TaiLue:
        return "tai-lue"sv;
    case CounterStyleNameKeyword::TallyMark:
        return "tally-mark"sv;
    case CounterStyleNameKeyword::Tamil:
        return "tamil"sv;
    case CounterStyleNameKeyword::TeluguAlpha:
        return "telugu-alpha"sv;
    case CounterStyleNameKeyword::Telugu:
        return "telugu"sv;
    case CounterStyleNameKeyword::ThaiAlpha:
        return "thai-alpha"sv;
    case CounterStyleNameKeyword::Thai:
        return "thai"sv;
    case CounterStyleNameKeyword::Tibetan:
        return "tibetan"sv;
    case CounterStyleNameKeyword::Tigre:
        return "tigre"sv;
    case CounterStyleNameKeyword::TradChineseFormal:
        return "trad-chinese-formal"sv;
    case CounterStyleNameKeyword::TradChineseInformal:
        return "trad-chinese-informal"sv;
    case CounterStyleNameKeyword::UpperAlphaSymbolic:
        return "upper-alpha-symbolic"sv;
    case CounterStyleNameKeyword::UpperAlpha:
        return "upper-alpha"sv;
    case CounterStyleNameKeyword::UpperArmenian:
        return "upper-armenian"sv;
    case CounterStyleNameKeyword::UpperBelorussian:
        return "upper-belorussian"sv;
    case CounterStyleNameKeyword::UpperBulgarian:
        return "upper-bulgarian"sv;
    case CounterStyleNameKeyword::UpperHexadecimal:
        return "upper-hexadecimal"sv;
    case CounterStyleNameKeyword::UpperLatin:
        return "upper-latin"sv;
    case CounterStyleNameKeyword::UpperMacedonian:
        return "upper-macedonian"sv;
    case CounterStyleNameKeyword::UpperOromoQubee:
        return "upper-oromo-qubee"sv;
    case CounterStyleNameKeyword::UpperRoman:
        return "upper-roman"sv;
    case CounterStyleNameKeyword::UpperRussianFull:
        return "upper-russian-full"sv;
    case CounterStyleNameKeyword::UpperRussian:
        return "upper-russian"sv;
    case CounterStyleNameKeyword::UpperSerbian:
        return "upper-serbian"sv;
    case CounterStyleNameKeyword::UpperUkrainianFull:
        return "upper-ukrainian-full"sv;
    case CounterStyleNameKeyword::UpperUkrainian:
        return "upper-ukrainian"sv;
    case CounterStyleNameKeyword::UrduAbjad:
        return "urdu-abjad"sv;
    case CounterStyleNameKeyword::UrduAlphabetic:
        return "urdu-alphabetic"sv;
    case CounterStyleNameKeyword::Urdu:
        return "urdu"sv;
    case CounterStyleNameKeyword::WarangCiti:
        return "warang-citi"sv;
    case CounterStyleNameKeyword::Wolaita:
        return "wolaita"sv;
    case CounterStyleNameKeyword::Yemsa:
        return "yemsa"sv;
    case CounterStyleNameKeyword::Zhuyin:
        return "zhuyin"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CounterStyleSystem> keyword_to_counter_style_system(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Cyclic:
        return CounterStyleSystem::Cyclic;
    case Keyword::Numeric:
        return CounterStyleSystem::Numeric;
    case Keyword::Alphabetic:
        return CounterStyleSystem::Alphabetic;
    case Keyword::Symbolic:
        return CounterStyleSystem::Symbolic;
    case Keyword::Additive:
        return CounterStyleSystem::Additive;
    default:
        return {};
    }
}

Keyword to_keyword(CounterStyleSystem counter_style_system_value)
{
    switch (counter_style_system_value) {
    case CounterStyleSystem::Cyclic:
        return Keyword::Cyclic;
    case CounterStyleSystem::Numeric:
        return Keyword::Numeric;
    case CounterStyleSystem::Alphabetic:
        return Keyword::Alphabetic;
    case CounterStyleSystem::Symbolic:
        return Keyword::Symbolic;
    case CounterStyleSystem::Additive:
        return Keyword::Additive;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CounterStyleSystem value)
{
    switch (value) {
    case CounterStyleSystem::Cyclic:
        return "cyclic"sv;
    case CounterStyleSystem::Numeric:
        return "numeric"sv;
    case CounterStyleSystem::Alphabetic:
        return "alphabetic"sv;
    case CounterStyleSystem::Symbolic:
        return "symbolic"sv;
    case CounterStyleSystem::Additive:
        return "additive"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CrossOriginModifierValue> keyword_to_cross_origin_modifier_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Anonymous:
        return CrossOriginModifierValue::Anonymous;
    case Keyword::UseCredentials:
        return CrossOriginModifierValue::UseCredentials;
    default:
        return {};
    }
}

Keyword to_keyword(CrossOriginModifierValue cross_origin_modifier_value_value)
{
    switch (cross_origin_modifier_value_value) {
    case CrossOriginModifierValue::Anonymous:
        return Keyword::Anonymous;
    case CrossOriginModifierValue::UseCredentials:
        return Keyword::UseCredentials;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CrossOriginModifierValue value)
{
    switch (value) {
    case CrossOriginModifierValue::Anonymous:
        return "anonymous"sv;
    case CrossOriginModifierValue::UseCredentials:
        return "use-credentials"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<CursorPredefined> keyword_to_cursor_predefined(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return CursorPredefined::Auto;
    case Keyword::Default:
        return CursorPredefined::Default;
    case Keyword::None:
        return CursorPredefined::None;
    case Keyword::ContextMenu:
        return CursorPredefined::ContextMenu;
    case Keyword::Help:
        return CursorPredefined::Help;
    case Keyword::Pointer:
        return CursorPredefined::Pointer;
    case Keyword::Progress:
        return CursorPredefined::Progress;
    case Keyword::Wait:
        return CursorPredefined::Wait;
    case Keyword::Cell:
        return CursorPredefined::Cell;
    case Keyword::Crosshair:
        return CursorPredefined::Crosshair;
    case Keyword::Text:
        return CursorPredefined::Text;
    case Keyword::VerticalText:
        return CursorPredefined::VerticalText;
    case Keyword::Alias:
        return CursorPredefined::Alias;
    case Keyword::Copy:
        return CursorPredefined::Copy;
    case Keyword::Move:
        return CursorPredefined::Move;
    case Keyword::NoDrop:
        return CursorPredefined::NoDrop;
    case Keyword::NotAllowed:
        return CursorPredefined::NotAllowed;
    case Keyword::Grab:
        return CursorPredefined::Grab;
    case Keyword::Grabbing:
        return CursorPredefined::Grabbing;
    case Keyword::EResize:
        return CursorPredefined::EResize;
    case Keyword::NResize:
        return CursorPredefined::NResize;
    case Keyword::NeResize:
        return CursorPredefined::NeResize;
    case Keyword::NwResize:
        return CursorPredefined::NwResize;
    case Keyword::SResize:
        return CursorPredefined::SResize;
    case Keyword::SeResize:
        return CursorPredefined::SeResize;
    case Keyword::SwResize:
        return CursorPredefined::SwResize;
    case Keyword::WResize:
        return CursorPredefined::WResize;
    case Keyword::EwResize:
        return CursorPredefined::EwResize;
    case Keyword::NsResize:
        return CursorPredefined::NsResize;
    case Keyword::NeswResize:
        return CursorPredefined::NeswResize;
    case Keyword::NwseResize:
        return CursorPredefined::NwseResize;
    case Keyword::ColResize:
        return CursorPredefined::ColResize;
    case Keyword::RowResize:
        return CursorPredefined::RowResize;
    case Keyword::AllScroll:
        return CursorPredefined::AllScroll;
    case Keyword::ZoomIn:
        return CursorPredefined::ZoomIn;
    case Keyword::ZoomOut:
        return CursorPredefined::ZoomOut;
    default:
        return {};
    }
}

Keyword to_keyword(CursorPredefined cursor_predefined_value)
{
    switch (cursor_predefined_value) {
    case CursorPredefined::Auto:
        return Keyword::Auto;
    case CursorPredefined::Default:
        return Keyword::Default;
    case CursorPredefined::None:
        return Keyword::None;
    case CursorPredefined::ContextMenu:
        return Keyword::ContextMenu;
    case CursorPredefined::Help:
        return Keyword::Help;
    case CursorPredefined::Pointer:
        return Keyword::Pointer;
    case CursorPredefined::Progress:
        return Keyword::Progress;
    case CursorPredefined::Wait:
        return Keyword::Wait;
    case CursorPredefined::Cell:
        return Keyword::Cell;
    case CursorPredefined::Crosshair:
        return Keyword::Crosshair;
    case CursorPredefined::Text:
        return Keyword::Text;
    case CursorPredefined::VerticalText:
        return Keyword::VerticalText;
    case CursorPredefined::Alias:
        return Keyword::Alias;
    case CursorPredefined::Copy:
        return Keyword::Copy;
    case CursorPredefined::Move:
        return Keyword::Move;
    case CursorPredefined::NoDrop:
        return Keyword::NoDrop;
    case CursorPredefined::NotAllowed:
        return Keyword::NotAllowed;
    case CursorPredefined::Grab:
        return Keyword::Grab;
    case CursorPredefined::Grabbing:
        return Keyword::Grabbing;
    case CursorPredefined::EResize:
        return Keyword::EResize;
    case CursorPredefined::NResize:
        return Keyword::NResize;
    case CursorPredefined::NeResize:
        return Keyword::NeResize;
    case CursorPredefined::NwResize:
        return Keyword::NwResize;
    case CursorPredefined::SResize:
        return Keyword::SResize;
    case CursorPredefined::SeResize:
        return Keyword::SeResize;
    case CursorPredefined::SwResize:
        return Keyword::SwResize;
    case CursorPredefined::WResize:
        return Keyword::WResize;
    case CursorPredefined::EwResize:
        return Keyword::EwResize;
    case CursorPredefined::NsResize:
        return Keyword::NsResize;
    case CursorPredefined::NeswResize:
        return Keyword::NeswResize;
    case CursorPredefined::NwseResize:
        return Keyword::NwseResize;
    case CursorPredefined::ColResize:
        return Keyword::ColResize;
    case CursorPredefined::RowResize:
        return Keyword::RowResize;
    case CursorPredefined::AllScroll:
        return Keyword::AllScroll;
    case CursorPredefined::ZoomIn:
        return Keyword::ZoomIn;
    case CursorPredefined::ZoomOut:
        return Keyword::ZoomOut;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(CursorPredefined value)
{
    switch (value) {
    case CursorPredefined::Auto:
        return "auto"sv;
    case CursorPredefined::Default:
        return "default"sv;
    case CursorPredefined::None:
        return "none"sv;
    case CursorPredefined::ContextMenu:
        return "context-menu"sv;
    case CursorPredefined::Help:
        return "help"sv;
    case CursorPredefined::Pointer:
        return "pointer"sv;
    case CursorPredefined::Progress:
        return "progress"sv;
    case CursorPredefined::Wait:
        return "wait"sv;
    case CursorPredefined::Cell:
        return "cell"sv;
    case CursorPredefined::Crosshair:
        return "crosshair"sv;
    case CursorPredefined::Text:
        return "text"sv;
    case CursorPredefined::VerticalText:
        return "vertical-text"sv;
    case CursorPredefined::Alias:
        return "alias"sv;
    case CursorPredefined::Copy:
        return "copy"sv;
    case CursorPredefined::Move:
        return "move"sv;
    case CursorPredefined::NoDrop:
        return "no-drop"sv;
    case CursorPredefined::NotAllowed:
        return "not-allowed"sv;
    case CursorPredefined::Grab:
        return "grab"sv;
    case CursorPredefined::Grabbing:
        return "grabbing"sv;
    case CursorPredefined::EResize:
        return "e-resize"sv;
    case CursorPredefined::NResize:
        return "n-resize"sv;
    case CursorPredefined::NeResize:
        return "ne-resize"sv;
    case CursorPredefined::NwResize:
        return "nw-resize"sv;
    case CursorPredefined::SResize:
        return "s-resize"sv;
    case CursorPredefined::SeResize:
        return "se-resize"sv;
    case CursorPredefined::SwResize:
        return "sw-resize"sv;
    case CursorPredefined::WResize:
        return "w-resize"sv;
    case CursorPredefined::EwResize:
        return "ew-resize"sv;
    case CursorPredefined::NsResize:
        return "ns-resize"sv;
    case CursorPredefined::NeswResize:
        return "nesw-resize"sv;
    case CursorPredefined::NwseResize:
        return "nwse-resize"sv;
    case CursorPredefined::ColResize:
        return "col-resize"sv;
    case CursorPredefined::RowResize:
        return "row-resize"sv;
    case CursorPredefined::AllScroll:
        return "all-scroll"sv;
    case CursorPredefined::ZoomIn:
        return "zoom-in"sv;
    case CursorPredefined::ZoomOut:
        return "zoom-out"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Direction> keyword_to_direction(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Ltr:
        return Direction::Ltr;
    case Keyword::Rtl:
        return Direction::Rtl;
    default:
        return {};
    }
}

Keyword to_keyword(Direction direction_value)
{
    switch (direction_value) {
    case Direction::Ltr:
        return Keyword::Ltr;
    case Direction::Rtl:
        return Keyword::Rtl;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Direction value)
{
    switch (value) {
    case Direction::Ltr:
        return "ltr"sv;
    case Direction::Rtl:
        return "rtl"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DiscretionaryLigValue> keyword_to_discretionary_lig_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::DiscretionaryLigatures:
        return DiscretionaryLigValue::DiscretionaryLigatures;
    case Keyword::NoDiscretionaryLigatures:
        return DiscretionaryLigValue::NoDiscretionaryLigatures;
    default:
        return {};
    }
}

Keyword to_keyword(DiscretionaryLigValue discretionary_lig_value_value)
{
    switch (discretionary_lig_value_value) {
    case DiscretionaryLigValue::DiscretionaryLigatures:
        return Keyword::DiscretionaryLigatures;
    case DiscretionaryLigValue::NoDiscretionaryLigatures:
        return Keyword::NoDiscretionaryLigatures;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DiscretionaryLigValue value)
{
    switch (value) {
    case DiscretionaryLigValue::DiscretionaryLigatures:
        return "discretionary-ligatures"sv;
    case DiscretionaryLigValue::NoDiscretionaryLigatures:
        return "no-discretionary-ligatures"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DisplayBox> keyword_to_display_box(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Contents:
        return DisplayBox::Contents;
    case Keyword::None:
        return DisplayBox::None;
    default:
        return {};
    }
}

Keyword to_keyword(DisplayBox display_box_value)
{
    switch (display_box_value) {
    case DisplayBox::Contents:
        return Keyword::Contents;
    case DisplayBox::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DisplayBox value)
{
    switch (value) {
    case DisplayBox::Contents:
        return "contents"sv;
    case DisplayBox::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DisplayInside> keyword_to_display_inside(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Flow:
        return DisplayInside::Flow;
    case Keyword::FlowRoot:
        return DisplayInside::FlowRoot;
    case Keyword::Table:
        return DisplayInside::Table;
    case Keyword::Flex:
        return DisplayInside::Flex;
    case Keyword::Grid:
        return DisplayInside::Grid;
    case Keyword::Ruby:
        return DisplayInside::Ruby;
    case Keyword::Math:
        return DisplayInside::Math;
    default:
        return {};
    }
}

Keyword to_keyword(DisplayInside display_inside_value)
{
    switch (display_inside_value) {
    case DisplayInside::Flow:
        return Keyword::Flow;
    case DisplayInside::FlowRoot:
        return Keyword::FlowRoot;
    case DisplayInside::Table:
        return Keyword::Table;
    case DisplayInside::Flex:
        return Keyword::Flex;
    case DisplayInside::Grid:
        return Keyword::Grid;
    case DisplayInside::Ruby:
        return Keyword::Ruby;
    case DisplayInside::Math:
        return Keyword::Math;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DisplayInside value)
{
    switch (value) {
    case DisplayInside::Flow:
        return "flow"sv;
    case DisplayInside::FlowRoot:
        return "flow-root"sv;
    case DisplayInside::Table:
        return "table"sv;
    case DisplayInside::Flex:
        return "flex"sv;
    case DisplayInside::Grid:
        return "grid"sv;
    case DisplayInside::Ruby:
        return "ruby"sv;
    case DisplayInside::Math:
        return "math"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DisplayInternal> keyword_to_display_internal(Keyword keyword)
{
    switch (keyword) {
    case Keyword::TableRowGroup:
        return DisplayInternal::TableRowGroup;
    case Keyword::TableHeaderGroup:
        return DisplayInternal::TableHeaderGroup;
    case Keyword::TableFooterGroup:
        return DisplayInternal::TableFooterGroup;
    case Keyword::TableRow:
        return DisplayInternal::TableRow;
    case Keyword::TableCell:
        return DisplayInternal::TableCell;
    case Keyword::TableColumnGroup:
        return DisplayInternal::TableColumnGroup;
    case Keyword::TableColumn:
        return DisplayInternal::TableColumn;
    case Keyword::TableCaption:
        return DisplayInternal::TableCaption;
    case Keyword::RubyBase:
        return DisplayInternal::RubyBase;
    case Keyword::RubyText:
        return DisplayInternal::RubyText;
    case Keyword::RubyBaseContainer:
        return DisplayInternal::RubyBaseContainer;
    case Keyword::RubyTextContainer:
        return DisplayInternal::RubyTextContainer;
    default:
        return {};
    }
}

Keyword to_keyword(DisplayInternal display_internal_value)
{
    switch (display_internal_value) {
    case DisplayInternal::TableRowGroup:
        return Keyword::TableRowGroup;
    case DisplayInternal::TableHeaderGroup:
        return Keyword::TableHeaderGroup;
    case DisplayInternal::TableFooterGroup:
        return Keyword::TableFooterGroup;
    case DisplayInternal::TableRow:
        return Keyword::TableRow;
    case DisplayInternal::TableCell:
        return Keyword::TableCell;
    case DisplayInternal::TableColumnGroup:
        return Keyword::TableColumnGroup;
    case DisplayInternal::TableColumn:
        return Keyword::TableColumn;
    case DisplayInternal::TableCaption:
        return Keyword::TableCaption;
    case DisplayInternal::RubyBase:
        return Keyword::RubyBase;
    case DisplayInternal::RubyText:
        return Keyword::RubyText;
    case DisplayInternal::RubyBaseContainer:
        return Keyword::RubyBaseContainer;
    case DisplayInternal::RubyTextContainer:
        return Keyword::RubyTextContainer;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DisplayInternal value)
{
    switch (value) {
    case DisplayInternal::TableRowGroup:
        return "table-row-group"sv;
    case DisplayInternal::TableHeaderGroup:
        return "table-header-group"sv;
    case DisplayInternal::TableFooterGroup:
        return "table-footer-group"sv;
    case DisplayInternal::TableRow:
        return "table-row"sv;
    case DisplayInternal::TableCell:
        return "table-cell"sv;
    case DisplayInternal::TableColumnGroup:
        return "table-column-group"sv;
    case DisplayInternal::TableColumn:
        return "table-column"sv;
    case DisplayInternal::TableCaption:
        return "table-caption"sv;
    case DisplayInternal::RubyBase:
        return "ruby-base"sv;
    case DisplayInternal::RubyText:
        return "ruby-text"sv;
    case DisplayInternal::RubyBaseContainer:
        return "ruby-base-container"sv;
    case DisplayInternal::RubyTextContainer:
        return "ruby-text-container"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DisplayLegacy> keyword_to_display_legacy(Keyword keyword)
{
    switch (keyword) {
    case Keyword::InlineBlock:
        return DisplayLegacy::InlineBlock;
    case Keyword::InlineTable:
        return DisplayLegacy::InlineTable;
    case Keyword::InlineFlex:
        return DisplayLegacy::InlineFlex;
    case Keyword::InlineGrid:
        return DisplayLegacy::InlineGrid;
    default:
        return {};
    }
}

Keyword to_keyword(DisplayLegacy display_legacy_value)
{
    switch (display_legacy_value) {
    case DisplayLegacy::InlineBlock:
        return Keyword::InlineBlock;
    case DisplayLegacy::InlineTable:
        return Keyword::InlineTable;
    case DisplayLegacy::InlineFlex:
        return Keyword::InlineFlex;
    case DisplayLegacy::InlineGrid:
        return Keyword::InlineGrid;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DisplayLegacy value)
{
    switch (value) {
    case DisplayLegacy::InlineBlock:
        return "inline-block"sv;
    case DisplayLegacy::InlineTable:
        return "inline-table"sv;
    case DisplayLegacy::InlineFlex:
        return "inline-flex"sv;
    case DisplayLegacy::InlineGrid:
        return "inline-grid"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<DisplayOutside> keyword_to_display_outside(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Block:
        return DisplayOutside::Block;
    case Keyword::Inline:
        return DisplayOutside::Inline;
    case Keyword::RunIn:
        return DisplayOutside::RunIn;
    default:
        return {};
    }
}

Keyword to_keyword(DisplayOutside display_outside_value)
{
    switch (display_outside_value) {
    case DisplayOutside::Block:
        return Keyword::Block;
    case DisplayOutside::Inline:
        return Keyword::Inline;
    case DisplayOutside::RunIn:
        return Keyword::RunIn;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(DisplayOutside value)
{
    switch (value) {
    case DisplayOutside::Block:
        return "block"sv;
    case DisplayOutside::Inline:
        return "inline"sv;
    case DisplayOutside::RunIn:
        return "run-in"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<EasingKeyword> keyword_to_easing_keyword(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Linear:
        return EasingKeyword::Linear;
    case Keyword::Ease:
        return EasingKeyword::Ease;
    case Keyword::EaseIn:
        return EasingKeyword::EaseIn;
    case Keyword::EaseOut:
        return EasingKeyword::EaseOut;
    case Keyword::EaseInOut:
        return EasingKeyword::EaseInOut;
    default:
        return {};
    }
}

Keyword to_keyword(EasingKeyword easing_keyword_value)
{
    switch (easing_keyword_value) {
    case EasingKeyword::Linear:
        return Keyword::Linear;
    case EasingKeyword::Ease:
        return Keyword::Ease;
    case EasingKeyword::EaseIn:
        return Keyword::EaseIn;
    case EasingKeyword::EaseOut:
        return Keyword::EaseOut;
    case EasingKeyword::EaseInOut:
        return Keyword::EaseInOut;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(EasingKeyword value)
{
    switch (value) {
    case EasingKeyword::Linear:
        return "linear"sv;
    case EasingKeyword::Ease:
        return "ease"sv;
    case EasingKeyword::EaseIn:
        return "ease-in"sv;
    case EasingKeyword::EaseOut:
        return "ease-out"sv;
    case EasingKeyword::EaseInOut:
        return "ease-in-out"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<EastAsianVariant> keyword_to_east_asian_variant(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Jis78:
        return EastAsianVariant::Jis78;
    case Keyword::Jis83:
        return EastAsianVariant::Jis83;
    case Keyword::Jis90:
        return EastAsianVariant::Jis90;
    case Keyword::Jis04:
        return EastAsianVariant::Jis04;
    case Keyword::Simplified:
        return EastAsianVariant::Simplified;
    case Keyword::Traditional:
        return EastAsianVariant::Traditional;
    default:
        return {};
    }
}

Keyword to_keyword(EastAsianVariant east_asian_variant_value)
{
    switch (east_asian_variant_value) {
    case EastAsianVariant::Jis78:
        return Keyword::Jis78;
    case EastAsianVariant::Jis83:
        return Keyword::Jis83;
    case EastAsianVariant::Jis90:
        return Keyword::Jis90;
    case EastAsianVariant::Jis04:
        return Keyword::Jis04;
    case EastAsianVariant::Simplified:
        return Keyword::Simplified;
    case EastAsianVariant::Traditional:
        return Keyword::Traditional;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(EastAsianVariant value)
{
    switch (value) {
    case EastAsianVariant::Jis78:
        return "jis78"sv;
    case EastAsianVariant::Jis83:
        return "jis83"sv;
    case EastAsianVariant::Jis90:
        return "jis90"sv;
    case EastAsianVariant::Jis04:
        return "jis04"sv;
    case EastAsianVariant::Simplified:
        return "simplified"sv;
    case EastAsianVariant::Traditional:
        return "traditional"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<EastAsianWidth> keyword_to_east_asian_width(Keyword keyword)
{
    switch (keyword) {
    case Keyword::FullWidth:
        return EastAsianWidth::FullWidth;
    case Keyword::ProportionalWidth:
        return EastAsianWidth::ProportionalWidth;
    default:
        return {};
    }
}

Keyword to_keyword(EastAsianWidth east_asian_width_value)
{
    switch (east_asian_width_value) {
    case EastAsianWidth::FullWidth:
        return Keyword::FullWidth;
    case EastAsianWidth::ProportionalWidth:
        return Keyword::ProportionalWidth;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(EastAsianWidth value)
{
    switch (value) {
    case EastAsianWidth::FullWidth:
        return "full-width"sv;
    case EastAsianWidth::ProportionalWidth:
        return "proportional-width"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<EmptyCells> keyword_to_empty_cells(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Show:
        return EmptyCells::Show;
    case Keyword::Hide:
        return EmptyCells::Hide;
    default:
        return {};
    }
}

Keyword to_keyword(EmptyCells empty_cells_value)
{
    switch (empty_cells_value) {
    case EmptyCells::Show:
        return Keyword::Show;
    case EmptyCells::Hide:
        return Keyword::Hide;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(EmptyCells value)
{
    switch (value) {
    case EmptyCells::Show:
        return "show"sv;
    case EmptyCells::Hide:
        return "hide"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FillRule> keyword_to_fill_rule(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Nonzero:
        return FillRule::Nonzero;
    case Keyword::Evenodd:
        return FillRule::Evenodd;
    default:
        return {};
    }
}

Keyword to_keyword(FillRule fill_rule_value)
{
    switch (fill_rule_value) {
    case FillRule::Nonzero:
        return Keyword::Nonzero;
    case FillRule::Evenodd:
        return Keyword::Evenodd;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FillRule value)
{
    switch (value) {
    case FillRule::Nonzero:
        return "nonzero"sv;
    case FillRule::Evenodd:
        return "evenodd"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FlexDirection> keyword_to_flex_direction(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Row:
        return FlexDirection::Row;
    case Keyword::RowReverse:
        return FlexDirection::RowReverse;
    case Keyword::Column:
        return FlexDirection::Column;
    case Keyword::ColumnReverse:
        return FlexDirection::ColumnReverse;
    default:
        return {};
    }
}

Keyword to_keyword(FlexDirection flex_direction_value)
{
    switch (flex_direction_value) {
    case FlexDirection::Row:
        return Keyword::Row;
    case FlexDirection::RowReverse:
        return Keyword::RowReverse;
    case FlexDirection::Column:
        return Keyword::Column;
    case FlexDirection::ColumnReverse:
        return Keyword::ColumnReverse;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FlexDirection value)
{
    switch (value) {
    case FlexDirection::Row:
        return "row"sv;
    case FlexDirection::RowReverse:
        return "row-reverse"sv;
    case FlexDirection::Column:
        return "column"sv;
    case FlexDirection::ColumnReverse:
        return "column-reverse"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FlexWrap> keyword_to_flex_wrap(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Nowrap:
        return FlexWrap::Nowrap;
    case Keyword::Wrap:
        return FlexWrap::Wrap;
    case Keyword::WrapReverse:
        return FlexWrap::WrapReverse;
    default:
        return {};
    }
}

Keyword to_keyword(FlexWrap flex_wrap_value)
{
    switch (flex_wrap_value) {
    case FlexWrap::Nowrap:
        return Keyword::Nowrap;
    case FlexWrap::Wrap:
        return Keyword::Wrap;
    case FlexWrap::WrapReverse:
        return Keyword::WrapReverse;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FlexWrap value)
{
    switch (value) {
    case FlexWrap::Nowrap:
        return "nowrap"sv;
    case FlexWrap::Wrap:
        return "wrap"sv;
    case FlexWrap::WrapReverse:
        return "wrap-reverse"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Float> keyword_to_float(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return Float::None;
    case Keyword::Left:
        return Float::Left;
    case Keyword::Right:
        return Float::Right;
    case Keyword::InlineStart:
        return Float::InlineStart;
    case Keyword::InlineEnd:
        return Float::InlineEnd;
    default:
        return {};
    }
}

Keyword to_keyword(Float float_value)
{
    switch (float_value) {
    case Float::None:
        return Keyword::None;
    case Float::Left:
        return Keyword::Left;
    case Float::Right:
        return Keyword::Right;
    case Float::InlineStart:
        return Keyword::InlineStart;
    case Float::InlineEnd:
        return Keyword::InlineEnd;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Float value)
{
    switch (value) {
    case Float::None:
        return "none"sv;
    case Float::Left:
        return "left"sv;
    case Float::Right:
        return "right"sv;
    case Float::InlineStart:
        return "inline-start"sv;
    case Float::InlineEnd:
        return "inline-end"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontDisplay> keyword_to_font_display(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return FontDisplay::Auto;
    case Keyword::Block:
        return FontDisplay::Block;
    case Keyword::Swap:
        return FontDisplay::Swap;
    case Keyword::Fallback:
        return FontDisplay::Fallback;
    case Keyword::Optional:
        return FontDisplay::Optional;
    default:
        return {};
    }
}

Keyword to_keyword(FontDisplay font_display_value)
{
    switch (font_display_value) {
    case FontDisplay::Auto:
        return Keyword::Auto;
    case FontDisplay::Block:
        return Keyword::Block;
    case FontDisplay::Swap:
        return Keyword::Swap;
    case FontDisplay::Fallback:
        return Keyword::Fallback;
    case FontDisplay::Optional:
        return Keyword::Optional;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontDisplay value)
{
    switch (value) {
    case FontDisplay::Auto:
        return "auto"sv;
    case FontDisplay::Block:
        return "block"sv;
    case FontDisplay::Swap:
        return "swap"sv;
    case FontDisplay::Fallback:
        return "fallback"sv;
    case FontDisplay::Optional:
        return "optional"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontKerning> keyword_to_font_kerning(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return FontKerning::Auto;
    case Keyword::Normal:
        return FontKerning::Normal;
    case Keyword::None:
        return FontKerning::None;
    default:
        return {};
    }
}

Keyword to_keyword(FontKerning font_kerning_value)
{
    switch (font_kerning_value) {
    case FontKerning::Auto:
        return Keyword::Auto;
    case FontKerning::Normal:
        return Keyword::Normal;
    case FontKerning::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontKerning value)
{
    switch (value) {
    case FontKerning::Auto:
        return "auto"sv;
    case FontKerning::Normal:
        return "normal"sv;
    case FontKerning::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontOpticalSizing> keyword_to_font_optical_sizing(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return FontOpticalSizing::Auto;
    case Keyword::None:
        return FontOpticalSizing::None;
    default:
        return {};
    }
}

Keyword to_keyword(FontOpticalSizing font_optical_sizing_value)
{
    switch (font_optical_sizing_value) {
    case FontOpticalSizing::Auto:
        return Keyword::Auto;
    case FontOpticalSizing::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontOpticalSizing value)
{
    switch (value) {
    case FontOpticalSizing::Auto:
        return "auto"sv;
    case FontOpticalSizing::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontStyleKeyword> keyword_to_font_style_keyword(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return FontStyleKeyword::Normal;
    case Keyword::Italic:
        return FontStyleKeyword::Italic;
    case Keyword::Left:
        return FontStyleKeyword::Left;
    case Keyword::Right:
        return FontStyleKeyword::Right;
    case Keyword::Oblique:
        return FontStyleKeyword::Oblique;
    default:
        return {};
    }
}

Keyword to_keyword(FontStyleKeyword font_style_keyword_value)
{
    switch (font_style_keyword_value) {
    case FontStyleKeyword::Normal:
        return Keyword::Normal;
    case FontStyleKeyword::Italic:
        return Keyword::Italic;
    case FontStyleKeyword::Left:
        return Keyword::Left;
    case FontStyleKeyword::Right:
        return Keyword::Right;
    case FontStyleKeyword::Oblique:
        return Keyword::Oblique;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontStyleKeyword value)
{
    switch (value) {
    case FontStyleKeyword::Normal:
        return "normal"sv;
    case FontStyleKeyword::Italic:
        return "italic"sv;
    case FontStyleKeyword::Left:
        return "left"sv;
    case FontStyleKeyword::Right:
        return "right"sv;
    case FontStyleKeyword::Oblique:
        return "oblique"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontTech> keyword_to_font_tech(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Avar2:
        return FontTech::Avar2;
    case Keyword::ColorCbdt:
        return FontTech::ColorCbdt;
    case Keyword::ColorColrv0:
        return FontTech::ColorColrv0;
    case Keyword::ColorColrv1:
        return FontTech::ColorColrv1;
    case Keyword::ColorSbix:
        return FontTech::ColorSbix;
    case Keyword::ColorSvg:
        return FontTech::ColorSvg;
    case Keyword::FeaturesAat:
        return FontTech::FeaturesAat;
    case Keyword::FeaturesGraphite:
        return FontTech::FeaturesGraphite;
    case Keyword::FeaturesOpentype:
        return FontTech::FeaturesOpentype;
    case Keyword::Incremental:
        return FontTech::Incremental;
    case Keyword::Palettes:
        return FontTech::Palettes;
    case Keyword::Variations:
        return FontTech::Variations;
    default:
        return {};
    }
}

Keyword to_keyword(FontTech font_tech_value)
{
    switch (font_tech_value) {
    case FontTech::Avar2:
        return Keyword::Avar2;
    case FontTech::ColorCbdt:
        return Keyword::ColorCbdt;
    case FontTech::ColorColrv0:
        return Keyword::ColorColrv0;
    case FontTech::ColorColrv1:
        return Keyword::ColorColrv1;
    case FontTech::ColorSbix:
        return Keyword::ColorSbix;
    case FontTech::ColorSvg:
        return Keyword::ColorSvg;
    case FontTech::FeaturesAat:
        return Keyword::FeaturesAat;
    case FontTech::FeaturesGraphite:
        return Keyword::FeaturesGraphite;
    case FontTech::FeaturesOpentype:
        return Keyword::FeaturesOpentype;
    case FontTech::Incremental:
        return Keyword::Incremental;
    case FontTech::Palettes:
        return Keyword::Palettes;
    case FontTech::Variations:
        return Keyword::Variations;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontTech value)
{
    switch (value) {
    case FontTech::Avar2:
        return "avar2"sv;
    case FontTech::ColorCbdt:
        return "color-cbdt"sv;
    case FontTech::ColorColrv0:
        return "color-colrv0"sv;
    case FontTech::ColorColrv1:
        return "color-colrv1"sv;
    case FontTech::ColorSbix:
        return "color-sbix"sv;
    case FontTech::ColorSvg:
        return "color-svg"sv;
    case FontTech::FeaturesAat:
        return "features-aat"sv;
    case FontTech::FeaturesGraphite:
        return "features-graphite"sv;
    case FontTech::FeaturesOpentype:
        return "features-opentype"sv;
    case FontTech::Incremental:
        return "incremental"sv;
    case FontTech::Palettes:
        return "palettes"sv;
    case FontTech::Variations:
        return "variations"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontVariantCaps> keyword_to_font_variant_caps(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return FontVariantCaps::Normal;
    case Keyword::SmallCaps:
        return FontVariantCaps::SmallCaps;
    case Keyword::AllSmallCaps:
        return FontVariantCaps::AllSmallCaps;
    case Keyword::PetiteCaps:
        return FontVariantCaps::PetiteCaps;
    case Keyword::AllPetiteCaps:
        return FontVariantCaps::AllPetiteCaps;
    case Keyword::Unicase:
        return FontVariantCaps::Unicase;
    case Keyword::TitlingCaps:
        return FontVariantCaps::TitlingCaps;
    default:
        return {};
    }
}

Keyword to_keyword(FontVariantCaps font_variant_caps_value)
{
    switch (font_variant_caps_value) {
    case FontVariantCaps::Normal:
        return Keyword::Normal;
    case FontVariantCaps::SmallCaps:
        return Keyword::SmallCaps;
    case FontVariantCaps::AllSmallCaps:
        return Keyword::AllSmallCaps;
    case FontVariantCaps::PetiteCaps:
        return Keyword::PetiteCaps;
    case FontVariantCaps::AllPetiteCaps:
        return Keyword::AllPetiteCaps;
    case FontVariantCaps::Unicase:
        return Keyword::Unicase;
    case FontVariantCaps::TitlingCaps:
        return Keyword::TitlingCaps;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontVariantCaps value)
{
    switch (value) {
    case FontVariantCaps::Normal:
        return "normal"sv;
    case FontVariantCaps::SmallCaps:
        return "small-caps"sv;
    case FontVariantCaps::AllSmallCaps:
        return "all-small-caps"sv;
    case FontVariantCaps::PetiteCaps:
        return "petite-caps"sv;
    case FontVariantCaps::AllPetiteCaps:
        return "all-petite-caps"sv;
    case FontVariantCaps::Unicase:
        return "unicase"sv;
    case FontVariantCaps::TitlingCaps:
        return "titling-caps"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontVariantEmoji> keyword_to_font_variant_emoji(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return FontVariantEmoji::Normal;
    case Keyword::Text:
        return FontVariantEmoji::Text;
    case Keyword::Emoji:
        return FontVariantEmoji::Emoji;
    case Keyword::Unicode:
        return FontVariantEmoji::Unicode;
    default:
        return {};
    }
}

Keyword to_keyword(FontVariantEmoji font_variant_emoji_value)
{
    switch (font_variant_emoji_value) {
    case FontVariantEmoji::Normal:
        return Keyword::Normal;
    case FontVariantEmoji::Text:
        return Keyword::Text;
    case FontVariantEmoji::Emoji:
        return Keyword::Emoji;
    case FontVariantEmoji::Unicode:
        return Keyword::Unicode;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontVariantEmoji value)
{
    switch (value) {
    case FontVariantEmoji::Normal:
        return "normal"sv;
    case FontVariantEmoji::Text:
        return "text"sv;
    case FontVariantEmoji::Emoji:
        return "emoji"sv;
    case FontVariantEmoji::Unicode:
        return "unicode"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontVariantPosition> keyword_to_font_variant_position(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return FontVariantPosition::Normal;
    case Keyword::Sub:
        return FontVariantPosition::Sub;
    case Keyword::Super:
        return FontVariantPosition::Super;
    default:
        return {};
    }
}

Keyword to_keyword(FontVariantPosition font_variant_position_value)
{
    switch (font_variant_position_value) {
    case FontVariantPosition::Normal:
        return Keyword::Normal;
    case FontVariantPosition::Sub:
        return Keyword::Sub;
    case FontVariantPosition::Super:
        return Keyword::Super;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontVariantPosition value)
{
    switch (value) {
    case FontVariantPosition::Normal:
        return "normal"sv;
    case FontVariantPosition::Sub:
        return "sub"sv;
    case FontVariantPosition::Super:
        return "super"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<FontWidth> keyword_to_font_width(Keyword keyword)
{
    switch (keyword) {
    case Keyword::UltraCondensed:
        return FontWidth::UltraCondensed;
    case Keyword::ExtraCondensed:
        return FontWidth::ExtraCondensed;
    case Keyword::Condensed:
        return FontWidth::Condensed;
    case Keyword::SemiCondensed:
        return FontWidth::SemiCondensed;
    case Keyword::Normal:
        return FontWidth::Normal;
    case Keyword::SemiExpanded:
        return FontWidth::SemiExpanded;
    case Keyword::Expanded:
        return FontWidth::Expanded;
    case Keyword::ExtraExpanded:
        return FontWidth::ExtraExpanded;
    case Keyword::UltraExpanded:
        return FontWidth::UltraExpanded;
    default:
        return {};
    }
}

Keyword to_keyword(FontWidth font_width_value)
{
    switch (font_width_value) {
    case FontWidth::UltraCondensed:
        return Keyword::UltraCondensed;
    case FontWidth::ExtraCondensed:
        return Keyword::ExtraCondensed;
    case FontWidth::Condensed:
        return Keyword::Condensed;
    case FontWidth::SemiCondensed:
        return Keyword::SemiCondensed;
    case FontWidth::Normal:
        return Keyword::Normal;
    case FontWidth::SemiExpanded:
        return Keyword::SemiExpanded;
    case FontWidth::Expanded:
        return Keyword::Expanded;
    case FontWidth::ExtraExpanded:
        return Keyword::ExtraExpanded;
    case FontWidth::UltraExpanded:
        return Keyword::UltraExpanded;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(FontWidth value)
{
    switch (value) {
    case FontWidth::UltraCondensed:
        return "ultra-condensed"sv;
    case FontWidth::ExtraCondensed:
        return "extra-condensed"sv;
    case FontWidth::Condensed:
        return "condensed"sv;
    case FontWidth::SemiCondensed:
        return "semi-condensed"sv;
    case FontWidth::Normal:
        return "normal"sv;
    case FontWidth::SemiExpanded:
        return "semi-expanded"sv;
    case FontWidth::Expanded:
        return "expanded"sv;
    case FontWidth::ExtraExpanded:
        return "extra-expanded"sv;
    case FontWidth::UltraExpanded:
        return "ultra-expanded"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<GenericFontFamily> keyword_to_generic_font_family(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Serif:
        return GenericFontFamily::Serif;
    case Keyword::SansSerif:
        return GenericFontFamily::SansSerif;
    case Keyword::Cursive:
        return GenericFontFamily::Cursive;
    case Keyword::Fantasy:
        return GenericFontFamily::Fantasy;
    case Keyword::Monospace:
        return GenericFontFamily::Monospace;
    case Keyword::Math:
        return GenericFontFamily::Math;
    case Keyword::UiSerif:
        return GenericFontFamily::UiSerif;
    case Keyword::UiSansSerif:
        return GenericFontFamily::UiSansSerif;
    case Keyword::UiMonospace:
        return GenericFontFamily::UiMonospace;
    case Keyword::UiRounded:
        return GenericFontFamily::UiRounded;
    default:
        return {};
    }
}

Keyword to_keyword(GenericFontFamily generic_font_family_value)
{
    switch (generic_font_family_value) {
    case GenericFontFamily::Serif:
        return Keyword::Serif;
    case GenericFontFamily::SansSerif:
        return Keyword::SansSerif;
    case GenericFontFamily::Cursive:
        return Keyword::Cursive;
    case GenericFontFamily::Fantasy:
        return Keyword::Fantasy;
    case GenericFontFamily::Monospace:
        return Keyword::Monospace;
    case GenericFontFamily::Math:
        return Keyword::Math;
    case GenericFontFamily::UiSerif:
        return Keyword::UiSerif;
    case GenericFontFamily::UiSansSerif:
        return Keyword::UiSansSerif;
    case GenericFontFamily::UiMonospace:
        return Keyword::UiMonospace;
    case GenericFontFamily::UiRounded:
        return Keyword::UiRounded;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(GenericFontFamily value)
{
    switch (value) {
    case GenericFontFamily::Serif:
        return "serif"sv;
    case GenericFontFamily::SansSerif:
        return "sans-serif"sv;
    case GenericFontFamily::Cursive:
        return "cursive"sv;
    case GenericFontFamily::Fantasy:
        return "fantasy"sv;
    case GenericFontFamily::Monospace:
        return "monospace"sv;
    case GenericFontFamily::Math:
        return "math"sv;
    case GenericFontFamily::UiSerif:
        return "ui-serif"sv;
    case GenericFontFamily::UiSansSerif:
        return "ui-sans-serif"sv;
    case GenericFontFamily::UiMonospace:
        return "ui-monospace"sv;
    case GenericFontFamily::UiRounded:
        return "ui-rounded"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<HistoricalLigValue> keyword_to_historical_lig_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::HistoricalLigatures:
        return HistoricalLigValue::HistoricalLigatures;
    case Keyword::NoHistoricalLigatures:
        return HistoricalLigValue::NoHistoricalLigatures;
    default:
        return {};
    }
}

Keyword to_keyword(HistoricalLigValue historical_lig_value_value)
{
    switch (historical_lig_value_value) {
    case HistoricalLigValue::HistoricalLigatures:
        return Keyword::HistoricalLigatures;
    case HistoricalLigValue::NoHistoricalLigatures:
        return Keyword::NoHistoricalLigatures;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(HistoricalLigValue value)
{
    switch (value) {
    case HistoricalLigValue::HistoricalLigatures:
        return "historical-ligatures"sv;
    case HistoricalLigValue::NoHistoricalLigatures:
        return "no-historical-ligatures"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<HueInterpolationMethod> keyword_to_hue_interpolation_method(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Shorter:
        return HueInterpolationMethod::Shorter;
    case Keyword::Longer:
        return HueInterpolationMethod::Longer;
    case Keyword::Increasing:
        return HueInterpolationMethod::Increasing;
    case Keyword::Decreasing:
        return HueInterpolationMethod::Decreasing;
    default:
        return {};
    }
}

Keyword to_keyword(HueInterpolationMethod hue_interpolation_method_value)
{
    switch (hue_interpolation_method_value) {
    case HueInterpolationMethod::Shorter:
        return Keyword::Shorter;
    case HueInterpolationMethod::Longer:
        return Keyword::Longer;
    case HueInterpolationMethod::Increasing:
        return Keyword::Increasing;
    case HueInterpolationMethod::Decreasing:
        return Keyword::Decreasing;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(HueInterpolationMethod value)
{
    switch (value) {
    case HueInterpolationMethod::Shorter:
        return "shorter"sv;
    case HueInterpolationMethod::Longer:
        return "longer"sv;
    case HueInterpolationMethod::Increasing:
        return "increasing"sv;
    case HueInterpolationMethod::Decreasing:
        return "decreasing"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ImageRendering> keyword_to_image_rendering(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return ImageRendering::Auto;
    case Keyword::CrispEdges:
        return ImageRendering::CrispEdges;
    case Keyword::HighQuality:
        return ImageRendering::HighQuality;
    case Keyword::Pixelated:
        return ImageRendering::Pixelated;
    case Keyword::Smooth:
        return ImageRendering::Smooth;
    case Keyword::Optimizespeed:
        return ImageRendering::Pixelated;
    case Keyword::Optimizequality:
        return ImageRendering::Smooth;
    default:
        return {};
    }
}

Keyword to_keyword(ImageRendering image_rendering_value)
{
    switch (image_rendering_value) {
    case ImageRendering::Auto:
        return Keyword::Auto;
    case ImageRendering::CrispEdges:
        return Keyword::CrispEdges;
    case ImageRendering::HighQuality:
        return Keyword::HighQuality;
    case ImageRendering::Pixelated:
        return Keyword::Pixelated;
    case ImageRendering::Smooth:
        return Keyword::Smooth;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ImageRendering value)
{
    switch (value) {
    case ImageRendering::Auto:
        return "auto"sv;
    case ImageRendering::CrispEdges:
        return "crisp-edges"sv;
    case ImageRendering::HighQuality:
        return "high-quality"sv;
    case ImageRendering::Pixelated:
        return "pixelated"sv;
    case ImageRendering::Smooth:
        return "smooth"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Isolation> keyword_to_isolation(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return Isolation::Auto;
    case Keyword::Isolate:
        return Isolation::Isolate;
    default:
        return {};
    }
}

Keyword to_keyword(Isolation isolation_value)
{
    switch (isolation_value) {
    case Isolation::Auto:
        return Keyword::Auto;
    case Isolation::Isolate:
        return Keyword::Isolate;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Isolation value)
{
    switch (value) {
    case Isolation::Auto:
        return "auto"sv;
    case Isolation::Isolate:
        return "isolate"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<JustifyContent> keyword_to_justify_content(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return JustifyContent::Normal;
    case Keyword::Start:
        return JustifyContent::Start;
    case Keyword::End:
        return JustifyContent::End;
    case Keyword::FlexStart:
        return JustifyContent::FlexStart;
    case Keyword::FlexEnd:
        return JustifyContent::FlexEnd;
    case Keyword::Center:
        return JustifyContent::Center;
    case Keyword::SpaceBetween:
        return JustifyContent::SpaceBetween;
    case Keyword::SpaceAround:
        return JustifyContent::SpaceAround;
    case Keyword::SpaceEvenly:
        return JustifyContent::SpaceEvenly;
    case Keyword::Stretch:
        return JustifyContent::Stretch;
    case Keyword::Left:
        return JustifyContent::Left;
    case Keyword::Right:
        return JustifyContent::Right;
    default:
        return {};
    }
}

Keyword to_keyword(JustifyContent justify_content_value)
{
    switch (justify_content_value) {
    case JustifyContent::Normal:
        return Keyword::Normal;
    case JustifyContent::Start:
        return Keyword::Start;
    case JustifyContent::End:
        return Keyword::End;
    case JustifyContent::FlexStart:
        return Keyword::FlexStart;
    case JustifyContent::FlexEnd:
        return Keyword::FlexEnd;
    case JustifyContent::Center:
        return Keyword::Center;
    case JustifyContent::SpaceBetween:
        return Keyword::SpaceBetween;
    case JustifyContent::SpaceAround:
        return Keyword::SpaceAround;
    case JustifyContent::SpaceEvenly:
        return Keyword::SpaceEvenly;
    case JustifyContent::Stretch:
        return Keyword::Stretch;
    case JustifyContent::Left:
        return Keyword::Left;
    case JustifyContent::Right:
        return Keyword::Right;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(JustifyContent value)
{
    switch (value) {
    case JustifyContent::Normal:
        return "normal"sv;
    case JustifyContent::Start:
        return "start"sv;
    case JustifyContent::End:
        return "end"sv;
    case JustifyContent::FlexStart:
        return "flex-start"sv;
    case JustifyContent::FlexEnd:
        return "flex-end"sv;
    case JustifyContent::Center:
        return "center"sv;
    case JustifyContent::SpaceBetween:
        return "space-between"sv;
    case JustifyContent::SpaceAround:
        return "space-around"sv;
    case JustifyContent::SpaceEvenly:
        return "space-evenly"sv;
    case JustifyContent::Stretch:
        return "stretch"sv;
    case JustifyContent::Left:
        return "left"sv;
    case JustifyContent::Right:
        return "right"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<JustifyItems> keyword_to_justify_items(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Baseline:
        return JustifyItems::Baseline;
    case Keyword::Center:
        return JustifyItems::Center;
    case Keyword::End:
        return JustifyItems::End;
    case Keyword::FlexEnd:
        return JustifyItems::FlexEnd;
    case Keyword::FlexStart:
        return JustifyItems::FlexStart;
    case Keyword::Legacy:
        return JustifyItems::Legacy;
    case Keyword::Normal:
        return JustifyItems::Normal;
    case Keyword::Safe:
        return JustifyItems::Safe;
    case Keyword::SelfEnd:
        return JustifyItems::SelfEnd;
    case Keyword::SelfStart:
        return JustifyItems::SelfStart;
    case Keyword::Start:
        return JustifyItems::Start;
    case Keyword::Stretch:
        return JustifyItems::Stretch;
    case Keyword::Unsafe:
        return JustifyItems::Unsafe;
    case Keyword::Left:
        return JustifyItems::Left;
    case Keyword::Right:
        return JustifyItems::Right;
    default:
        return {};
    }
}

Keyword to_keyword(JustifyItems justify_items_value)
{
    switch (justify_items_value) {
    case JustifyItems::Baseline:
        return Keyword::Baseline;
    case JustifyItems::Center:
        return Keyword::Center;
    case JustifyItems::End:
        return Keyword::End;
    case JustifyItems::FlexEnd:
        return Keyword::FlexEnd;
    case JustifyItems::FlexStart:
        return Keyword::FlexStart;
    case JustifyItems::Legacy:
        return Keyword::Legacy;
    case JustifyItems::Normal:
        return Keyword::Normal;
    case JustifyItems::Safe:
        return Keyword::Safe;
    case JustifyItems::SelfEnd:
        return Keyword::SelfEnd;
    case JustifyItems::SelfStart:
        return Keyword::SelfStart;
    case JustifyItems::Start:
        return Keyword::Start;
    case JustifyItems::Stretch:
        return Keyword::Stretch;
    case JustifyItems::Unsafe:
        return Keyword::Unsafe;
    case JustifyItems::Left:
        return Keyword::Left;
    case JustifyItems::Right:
        return Keyword::Right;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(JustifyItems value)
{
    switch (value) {
    case JustifyItems::Baseline:
        return "baseline"sv;
    case JustifyItems::Center:
        return "center"sv;
    case JustifyItems::End:
        return "end"sv;
    case JustifyItems::FlexEnd:
        return "flex-end"sv;
    case JustifyItems::FlexStart:
        return "flex-start"sv;
    case JustifyItems::Legacy:
        return "legacy"sv;
    case JustifyItems::Normal:
        return "normal"sv;
    case JustifyItems::Safe:
        return "safe"sv;
    case JustifyItems::SelfEnd:
        return "self-end"sv;
    case JustifyItems::SelfStart:
        return "self-start"sv;
    case JustifyItems::Start:
        return "start"sv;
    case JustifyItems::Stretch:
        return "stretch"sv;
    case JustifyItems::Unsafe:
        return "unsafe"sv;
    case JustifyItems::Left:
        return "left"sv;
    case JustifyItems::Right:
        return "right"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<JustifySelf> keyword_to_justify_self(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return JustifySelf::Auto;
    case Keyword::Baseline:
        return JustifySelf::Baseline;
    case Keyword::Center:
        return JustifySelf::Center;
    case Keyword::End:
        return JustifySelf::End;
    case Keyword::FlexEnd:
        return JustifySelf::FlexEnd;
    case Keyword::FlexStart:
        return JustifySelf::FlexStart;
    case Keyword::Normal:
        return JustifySelf::Normal;
    case Keyword::Safe:
        return JustifySelf::Safe;
    case Keyword::SelfEnd:
        return JustifySelf::SelfEnd;
    case Keyword::SelfStart:
        return JustifySelf::SelfStart;
    case Keyword::Start:
        return JustifySelf::Start;
    case Keyword::Stretch:
        return JustifySelf::Stretch;
    case Keyword::Unsafe:
        return JustifySelf::Unsafe;
    case Keyword::Left:
        return JustifySelf::Left;
    case Keyword::Right:
        return JustifySelf::Right;
    default:
        return {};
    }
}

Keyword to_keyword(JustifySelf justify_self_value)
{
    switch (justify_self_value) {
    case JustifySelf::Auto:
        return Keyword::Auto;
    case JustifySelf::Baseline:
        return Keyword::Baseline;
    case JustifySelf::Center:
        return Keyword::Center;
    case JustifySelf::End:
        return Keyword::End;
    case JustifySelf::FlexEnd:
        return Keyword::FlexEnd;
    case JustifySelf::FlexStart:
        return Keyword::FlexStart;
    case JustifySelf::Normal:
        return Keyword::Normal;
    case JustifySelf::Safe:
        return Keyword::Safe;
    case JustifySelf::SelfEnd:
        return Keyword::SelfEnd;
    case JustifySelf::SelfStart:
        return Keyword::SelfStart;
    case JustifySelf::Start:
        return Keyword::Start;
    case JustifySelf::Stretch:
        return Keyword::Stretch;
    case JustifySelf::Unsafe:
        return Keyword::Unsafe;
    case JustifySelf::Left:
        return Keyword::Left;
    case JustifySelf::Right:
        return Keyword::Right;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(JustifySelf value)
{
    switch (value) {
    case JustifySelf::Auto:
        return "auto"sv;
    case JustifySelf::Baseline:
        return "baseline"sv;
    case JustifySelf::Center:
        return "center"sv;
    case JustifySelf::End:
        return "end"sv;
    case JustifySelf::FlexEnd:
        return "flex-end"sv;
    case JustifySelf::FlexStart:
        return "flex-start"sv;
    case JustifySelf::Normal:
        return "normal"sv;
    case JustifySelf::Safe:
        return "safe"sv;
    case JustifySelf::SelfEnd:
        return "self-end"sv;
    case JustifySelf::SelfStart:
        return "self-start"sv;
    case JustifySelf::Start:
        return "start"sv;
    case JustifySelf::Stretch:
        return "stretch"sv;
    case JustifySelf::Unsafe:
        return "unsafe"sv;
    case JustifySelf::Left:
        return "left"sv;
    case JustifySelf::Right:
        return "right"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<LineStyle> keyword_to_line_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return LineStyle::None;
    case Keyword::Hidden:
        return LineStyle::Hidden;
    case Keyword::Dotted:
        return LineStyle::Dotted;
    case Keyword::Dashed:
        return LineStyle::Dashed;
    case Keyword::Solid:
        return LineStyle::Solid;
    case Keyword::Double:
        return LineStyle::Double;
    case Keyword::Groove:
        return LineStyle::Groove;
    case Keyword::Ridge:
        return LineStyle::Ridge;
    case Keyword::Inset:
        return LineStyle::Inset;
    case Keyword::Outset:
        return LineStyle::Outset;
    default:
        return {};
    }
}

Keyword to_keyword(LineStyle line_style_value)
{
    switch (line_style_value) {
    case LineStyle::None:
        return Keyword::None;
    case LineStyle::Hidden:
        return Keyword::Hidden;
    case LineStyle::Dotted:
        return Keyword::Dotted;
    case LineStyle::Dashed:
        return Keyword::Dashed;
    case LineStyle::Solid:
        return Keyword::Solid;
    case LineStyle::Double:
        return Keyword::Double;
    case LineStyle::Groove:
        return Keyword::Groove;
    case LineStyle::Ridge:
        return Keyword::Ridge;
    case LineStyle::Inset:
        return Keyword::Inset;
    case LineStyle::Outset:
        return Keyword::Outset;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(LineStyle value)
{
    switch (value) {
    case LineStyle::None:
        return "none"sv;
    case LineStyle::Hidden:
        return "hidden"sv;
    case LineStyle::Dotted:
        return "dotted"sv;
    case LineStyle::Dashed:
        return "dashed"sv;
    case LineStyle::Solid:
        return "solid"sv;
    case LineStyle::Double:
        return "double"sv;
    case LineStyle::Groove:
        return "groove"sv;
    case LineStyle::Ridge:
        return "ridge"sv;
    case LineStyle::Inset:
        return "inset"sv;
    case LineStyle::Outset:
        return "outset"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<LineWidth> keyword_to_line_width(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Thin:
        return LineWidth::Thin;
    case Keyword::Medium:
        return LineWidth::Medium;
    case Keyword::Thick:
        return LineWidth::Thick;
    default:
        return {};
    }
}

Keyword to_keyword(LineWidth line_width_value)
{
    switch (line_width_value) {
    case LineWidth::Thin:
        return Keyword::Thin;
    case LineWidth::Medium:
        return Keyword::Medium;
    case LineWidth::Thick:
        return Keyword::Thick;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(LineWidth value)
{
    switch (value) {
    case LineWidth::Thin:
        return "thin"sv;
    case LineWidth::Medium:
        return "medium"sv;
    case LineWidth::Thick:
        return "thick"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ListStylePosition> keyword_to_list_style_position(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Inside:
        return ListStylePosition::Inside;
    case Keyword::Outside:
        return ListStylePosition::Outside;
    default:
        return {};
    }
}

Keyword to_keyword(ListStylePosition list_style_position_value)
{
    switch (list_style_position_value) {
    case ListStylePosition::Inside:
        return Keyword::Inside;
    case ListStylePosition::Outside:
        return Keyword::Outside;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ListStylePosition value)
{
    switch (value) {
    case ListStylePosition::Inside:
        return "inside"sv;
    case ListStylePosition::Outside:
        return "outside"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<MaskType> keyword_to_mask_type(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Luminance:
        return MaskType::Luminance;
    case Keyword::Alpha:
        return MaskType::Alpha;
    default:
        return {};
    }
}

Keyword to_keyword(MaskType mask_type_value)
{
    switch (mask_type_value) {
    case MaskType::Luminance:
        return Keyword::Luminance;
    case MaskType::Alpha:
        return Keyword::Alpha;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(MaskType value)
{
    switch (value) {
    case MaskType::Luminance:
        return "luminance"sv;
    case MaskType::Alpha:
        return "alpha"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<MaskingMode> keyword_to_masking_mode(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Alpha:
        return MaskingMode::Alpha;
    case Keyword::Luminance:
        return MaskingMode::Luminance;
    case Keyword::MatchSource:
        return MaskingMode::MatchSource;
    default:
        return {};
    }
}

Keyword to_keyword(MaskingMode masking_mode_value)
{
    switch (masking_mode_value) {
    case MaskingMode::Alpha:
        return Keyword::Alpha;
    case MaskingMode::Luminance:
        return Keyword::Luminance;
    case MaskingMode::MatchSource:
        return Keyword::MatchSource;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(MaskingMode value)
{
    switch (value) {
    case MaskingMode::Alpha:
        return "alpha"sv;
    case MaskingMode::Luminance:
        return "luminance"sv;
    case MaskingMode::MatchSource:
        return "match-source"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<MathShift> keyword_to_math_shift(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return MathShift::Normal;
    case Keyword::Compact:
        return MathShift::Compact;
    default:
        return {};
    }
}

Keyword to_keyword(MathShift math_shift_value)
{
    switch (math_shift_value) {
    case MathShift::Normal:
        return Keyword::Normal;
    case MathShift::Compact:
        return Keyword::Compact;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(MathShift value)
{
    switch (value) {
    case MathShift::Normal:
        return "normal"sv;
    case MathShift::Compact:
        return "compact"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<MathStyle> keyword_to_math_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return MathStyle::Normal;
    case Keyword::Compact:
        return MathStyle::Compact;
    default:
        return {};
    }
}

Keyword to_keyword(MathStyle math_style_value)
{
    switch (math_style_value) {
    case MathStyle::Normal:
        return Keyword::Normal;
    case MathStyle::Compact:
        return Keyword::Compact;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(MathStyle value)
{
    switch (value) {
    case MathStyle::Normal:
        return "normal"sv;
    case MathStyle::Compact:
        return "compact"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<MixBlendMode> keyword_to_mix_blend_mode(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return MixBlendMode::Normal;
    case Keyword::Multiply:
        return MixBlendMode::Multiply;
    case Keyword::Screen:
        return MixBlendMode::Screen;
    case Keyword::Overlay:
        return MixBlendMode::Overlay;
    case Keyword::Darken:
        return MixBlendMode::Darken;
    case Keyword::Lighten:
        return MixBlendMode::Lighten;
    case Keyword::ColorDodge:
        return MixBlendMode::ColorDodge;
    case Keyword::ColorBurn:
        return MixBlendMode::ColorBurn;
    case Keyword::HardLight:
        return MixBlendMode::HardLight;
    case Keyword::SoftLight:
        return MixBlendMode::SoftLight;
    case Keyword::Difference:
        return MixBlendMode::Difference;
    case Keyword::Exclusion:
        return MixBlendMode::Exclusion;
    case Keyword::Hue:
        return MixBlendMode::Hue;
    case Keyword::Saturation:
        return MixBlendMode::Saturation;
    case Keyword::Color:
        return MixBlendMode::Color;
    case Keyword::Luminosity:
        return MixBlendMode::Luminosity;
    case Keyword::PlusDarker:
        return MixBlendMode::PlusDarker;
    case Keyword::PlusLighter:
        return MixBlendMode::PlusLighter;
    default:
        return {};
    }
}

Keyword to_keyword(MixBlendMode mix_blend_mode_value)
{
    switch (mix_blend_mode_value) {
    case MixBlendMode::Normal:
        return Keyword::Normal;
    case MixBlendMode::Multiply:
        return Keyword::Multiply;
    case MixBlendMode::Screen:
        return Keyword::Screen;
    case MixBlendMode::Overlay:
        return Keyword::Overlay;
    case MixBlendMode::Darken:
        return Keyword::Darken;
    case MixBlendMode::Lighten:
        return Keyword::Lighten;
    case MixBlendMode::ColorDodge:
        return Keyword::ColorDodge;
    case MixBlendMode::ColorBurn:
        return Keyword::ColorBurn;
    case MixBlendMode::HardLight:
        return Keyword::HardLight;
    case MixBlendMode::SoftLight:
        return Keyword::SoftLight;
    case MixBlendMode::Difference:
        return Keyword::Difference;
    case MixBlendMode::Exclusion:
        return Keyword::Exclusion;
    case MixBlendMode::Hue:
        return Keyword::Hue;
    case MixBlendMode::Saturation:
        return Keyword::Saturation;
    case MixBlendMode::Color:
        return Keyword::Color;
    case MixBlendMode::Luminosity:
        return Keyword::Luminosity;
    case MixBlendMode::PlusDarker:
        return Keyword::PlusDarker;
    case MixBlendMode::PlusLighter:
        return Keyword::PlusLighter;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(MixBlendMode value)
{
    switch (value) {
    case MixBlendMode::Normal:
        return "normal"sv;
    case MixBlendMode::Multiply:
        return "multiply"sv;
    case MixBlendMode::Screen:
        return "screen"sv;
    case MixBlendMode::Overlay:
        return "overlay"sv;
    case MixBlendMode::Darken:
        return "darken"sv;
    case MixBlendMode::Lighten:
        return "lighten"sv;
    case MixBlendMode::ColorDodge:
        return "color-dodge"sv;
    case MixBlendMode::ColorBurn:
        return "color-burn"sv;
    case MixBlendMode::HardLight:
        return "hard-light"sv;
    case MixBlendMode::SoftLight:
        return "soft-light"sv;
    case MixBlendMode::Difference:
        return "difference"sv;
    case MixBlendMode::Exclusion:
        return "exclusion"sv;
    case MixBlendMode::Hue:
        return "hue"sv;
    case MixBlendMode::Saturation:
        return "saturation"sv;
    case MixBlendMode::Color:
        return "color"sv;
    case MixBlendMode::Luminosity:
        return "luminosity"sv;
    case MixBlendMode::PlusDarker:
        return "plus-darker"sv;
    case MixBlendMode::PlusLighter:
        return "plus-lighter"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<NumericFigureValue> keyword_to_numeric_figure_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::LiningNums:
        return NumericFigureValue::LiningNums;
    case Keyword::OldstyleNums:
        return NumericFigureValue::OldstyleNums;
    default:
        return {};
    }
}

Keyword to_keyword(NumericFigureValue numeric_figure_value_value)
{
    switch (numeric_figure_value_value) {
    case NumericFigureValue::LiningNums:
        return Keyword::LiningNums;
    case NumericFigureValue::OldstyleNums:
        return Keyword::OldstyleNums;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(NumericFigureValue value)
{
    switch (value) {
    case NumericFigureValue::LiningNums:
        return "lining-nums"sv;
    case NumericFigureValue::OldstyleNums:
        return "oldstyle-nums"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<NumericFractionValue> keyword_to_numeric_fraction_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::DiagonalFractions:
        return NumericFractionValue::DiagonalFractions;
    case Keyword::StackedFractions:
        return NumericFractionValue::StackedFractions;
    default:
        return {};
    }
}

Keyword to_keyword(NumericFractionValue numeric_fraction_value_value)
{
    switch (numeric_fraction_value_value) {
    case NumericFractionValue::DiagonalFractions:
        return Keyword::DiagonalFractions;
    case NumericFractionValue::StackedFractions:
        return Keyword::StackedFractions;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(NumericFractionValue value)
{
    switch (value) {
    case NumericFractionValue::DiagonalFractions:
        return "diagonal-fractions"sv;
    case NumericFractionValue::StackedFractions:
        return "stacked-fractions"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<NumericSpacingValue> keyword_to_numeric_spacing_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::ProportionalNums:
        return NumericSpacingValue::ProportionalNums;
    case Keyword::TabularNums:
        return NumericSpacingValue::TabularNums;
    default:
        return {};
    }
}

Keyword to_keyword(NumericSpacingValue numeric_spacing_value_value)
{
    switch (numeric_spacing_value_value) {
    case NumericSpacingValue::ProportionalNums:
        return Keyword::ProportionalNums;
    case NumericSpacingValue::TabularNums:
        return Keyword::TabularNums;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(NumericSpacingValue value)
{
    switch (value) {
    case NumericSpacingValue::ProportionalNums:
        return "proportional-nums"sv;
    case NumericSpacingValue::TabularNums:
        return "tabular-nums"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ObjectFit> keyword_to_object_fit(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Fill:
        return ObjectFit::Fill;
    case Keyword::Contain:
        return ObjectFit::Contain;
    case Keyword::Cover:
        return ObjectFit::Cover;
    case Keyword::None:
        return ObjectFit::None;
    case Keyword::ScaleDown:
        return ObjectFit::ScaleDown;
    default:
        return {};
    }
}

Keyword to_keyword(ObjectFit object_fit_value)
{
    switch (object_fit_value) {
    case ObjectFit::Fill:
        return Keyword::Fill;
    case ObjectFit::Contain:
        return Keyword::Contain;
    case ObjectFit::Cover:
        return Keyword::Cover;
    case ObjectFit::None:
        return Keyword::None;
    case ObjectFit::ScaleDown:
        return Keyword::ScaleDown;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ObjectFit value)
{
    switch (value) {
    case ObjectFit::Fill:
        return "fill"sv;
    case ObjectFit::Contain:
        return "contain"sv;
    case ObjectFit::Cover:
        return "cover"sv;
    case ObjectFit::None:
        return "none"sv;
    case ObjectFit::ScaleDown:
        return "scale-down"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<OutlineStyle> keyword_to_outline_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return OutlineStyle::Auto;
    case Keyword::None:
        return OutlineStyle::None;
    case Keyword::Dotted:
        return OutlineStyle::Dotted;
    case Keyword::Dashed:
        return OutlineStyle::Dashed;
    case Keyword::Solid:
        return OutlineStyle::Solid;
    case Keyword::Double:
        return OutlineStyle::Double;
    case Keyword::Groove:
        return OutlineStyle::Groove;
    case Keyword::Ridge:
        return OutlineStyle::Ridge;
    case Keyword::Inset:
        return OutlineStyle::Inset;
    case Keyword::Outset:
        return OutlineStyle::Outset;
    default:
        return {};
    }
}

Keyword to_keyword(OutlineStyle outline_style_value)
{
    switch (outline_style_value) {
    case OutlineStyle::Auto:
        return Keyword::Auto;
    case OutlineStyle::None:
        return Keyword::None;
    case OutlineStyle::Dotted:
        return Keyword::Dotted;
    case OutlineStyle::Dashed:
        return Keyword::Dashed;
    case OutlineStyle::Solid:
        return Keyword::Solid;
    case OutlineStyle::Double:
        return Keyword::Double;
    case OutlineStyle::Groove:
        return Keyword::Groove;
    case OutlineStyle::Ridge:
        return Keyword::Ridge;
    case OutlineStyle::Inset:
        return Keyword::Inset;
    case OutlineStyle::Outset:
        return Keyword::Outset;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(OutlineStyle value)
{
    switch (value) {
    case OutlineStyle::Auto:
        return "auto"sv;
    case OutlineStyle::None:
        return "none"sv;
    case OutlineStyle::Dotted:
        return "dotted"sv;
    case OutlineStyle::Dashed:
        return "dashed"sv;
    case OutlineStyle::Solid:
        return "solid"sv;
    case OutlineStyle::Double:
        return "double"sv;
    case OutlineStyle::Groove:
        return "groove"sv;
    case OutlineStyle::Ridge:
        return "ridge"sv;
    case OutlineStyle::Inset:
        return "inset"sv;
    case OutlineStyle::Outset:
        return "outset"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Overflow> keyword_to_overflow(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return Overflow::Auto;
    case Keyword::Clip:
        return Overflow::Clip;
    case Keyword::Hidden:
        return Overflow::Hidden;
    case Keyword::Scroll:
        return Overflow::Scroll;
    case Keyword::Visible:
        return Overflow::Visible;
    default:
        return {};
    }
}

Keyword to_keyword(Overflow overflow_value)
{
    switch (overflow_value) {
    case Overflow::Auto:
        return Keyword::Auto;
    case Overflow::Clip:
        return Keyword::Clip;
    case Overflow::Hidden:
        return Keyword::Hidden;
    case Overflow::Scroll:
        return Keyword::Scroll;
    case Overflow::Visible:
        return Keyword::Visible;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Overflow value)
{
    switch (value) {
    case Overflow::Auto:
        return "auto"sv;
    case Overflow::Clip:
        return "clip"sv;
    case Overflow::Hidden:
        return "hidden"sv;
    case Overflow::Scroll:
        return "scroll"sv;
    case Overflow::Visible:
        return "visible"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PageSize> keyword_to_page_size(Keyword keyword)
{
    switch (keyword) {
    case Keyword::A5:
        return PageSize::A5;
    case Keyword::A4:
        return PageSize::A4;
    case Keyword::A3:
        return PageSize::A3;
    case Keyword::B5:
        return PageSize::B5;
    case Keyword::B4:
        return PageSize::B4;
    case Keyword::JisB5:
        return PageSize::JisB5;
    case Keyword::JisB4:
        return PageSize::JisB4;
    case Keyword::Letter:
        return PageSize::Letter;
    case Keyword::Legal:
        return PageSize::Legal;
    case Keyword::Ledger:
        return PageSize::Ledger;
    default:
        return {};
    }
}

Keyword to_keyword(PageSize page_size_value)
{
    switch (page_size_value) {
    case PageSize::A5:
        return Keyword::A5;
    case PageSize::A4:
        return Keyword::A4;
    case PageSize::A3:
        return Keyword::A3;
    case PageSize::B5:
        return Keyword::B5;
    case PageSize::B4:
        return Keyword::B4;
    case PageSize::JisB5:
        return Keyword::JisB5;
    case PageSize::JisB4:
        return Keyword::JisB4;
    case PageSize::Letter:
        return Keyword::Letter;
    case PageSize::Legal:
        return Keyword::Legal;
    case PageSize::Ledger:
        return Keyword::Ledger;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PageSize value)
{
    switch (value) {
    case PageSize::A5:
        return "a5"sv;
    case PageSize::A4:
        return "a4"sv;
    case PageSize::A3:
        return "a3"sv;
    case PageSize::B5:
        return "b5"sv;
    case PageSize::B4:
        return "b4"sv;
    case PageSize::JisB5:
        return "jis-b5"sv;
    case PageSize::JisB4:
        return "jis-b4"sv;
    case PageSize::Letter:
        return "letter"sv;
    case PageSize::Legal:
        return "legal"sv;
    case PageSize::Ledger:
        return "ledger"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PaintOrder> keyword_to_paint_order(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Fill:
        return PaintOrder::Fill;
    case Keyword::Stroke:
        return PaintOrder::Stroke;
    case Keyword::Markers:
        return PaintOrder::Markers;
    default:
        return {};
    }
}

Keyword to_keyword(PaintOrder paint_order_value)
{
    switch (paint_order_value) {
    case PaintOrder::Fill:
        return Keyword::Fill;
    case PaintOrder::Stroke:
        return Keyword::Stroke;
    case PaintOrder::Markers:
        return Keyword::Markers;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PaintOrder value)
{
    switch (value) {
    case PaintOrder::Fill:
        return "fill"sv;
    case PaintOrder::Stroke:
        return "stroke"sv;
    case PaintOrder::Markers:
        return "markers"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PointerEvents> keyword_to_pointer_events(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return PointerEvents::Auto;
    case Keyword::All:
        return PointerEvents::All;
    case Keyword::None:
        return PointerEvents::None;
    default:
        return {};
    }
}

Keyword to_keyword(PointerEvents pointer_events_value)
{
    switch (pointer_events_value) {
    case PointerEvents::Auto:
        return Keyword::Auto;
    case PointerEvents::All:
        return Keyword::All;
    case PointerEvents::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PointerEvents value)
{
    switch (value) {
    case PointerEvents::Auto:
        return "auto"sv;
    case PointerEvents::All:
        return "all"sv;
    case PointerEvents::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PolarColorSpace> keyword_to_polar_color_space(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Hsl:
        return PolarColorSpace::Hsl;
    case Keyword::Hwb:
        return PolarColorSpace::Hwb;
    case Keyword::Lch:
        return PolarColorSpace::Lch;
    case Keyword::Oklch:
        return PolarColorSpace::Oklch;
    default:
        return {};
    }
}

Keyword to_keyword(PolarColorSpace polar_color_space_value)
{
    switch (polar_color_space_value) {
    case PolarColorSpace::Hsl:
        return Keyword::Hsl;
    case PolarColorSpace::Hwb:
        return Keyword::Hwb;
    case PolarColorSpace::Lch:
        return Keyword::Lch;
    case PolarColorSpace::Oklch:
        return Keyword::Oklch;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PolarColorSpace value)
{
    switch (value) {
    case PolarColorSpace::Hsl:
        return "hsl"sv;
    case PolarColorSpace::Hwb:
        return "hwb"sv;
    case PolarColorSpace::Lch:
        return "lch"sv;
    case PolarColorSpace::Oklch:
        return "oklch"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PositionArea> keyword_to_position_area(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Left:
        return PositionArea::Left;
    case Keyword::Center:
        return PositionArea::Center;
    case Keyword::Right:
        return PositionArea::Right;
    case Keyword::SpanLeft:
        return PositionArea::SpanLeft;
    case Keyword::SpanRight:
        return PositionArea::SpanRight;
    case Keyword::XStart:
        return PositionArea::XStart;
    case Keyword::XEnd:
        return PositionArea::XEnd;
    case Keyword::SpanXStart:
        return PositionArea::SpanXStart;
    case Keyword::SpanXEnd:
        return PositionArea::SpanXEnd;
    case Keyword::SelfXStart:
        return PositionArea::SelfXStart;
    case Keyword::SelfXEnd:
        return PositionArea::SelfXEnd;
    case Keyword::SpanSelfXStart:
        return PositionArea::SpanSelfXStart;
    case Keyword::SpanSelfXEnd:
        return PositionArea::SpanSelfXEnd;
    case Keyword::SpanAll:
        return PositionArea::SpanAll;
    case Keyword::Top:
        return PositionArea::Top;
    case Keyword::Bottom:
        return PositionArea::Bottom;
    case Keyword::SpanTop:
        return PositionArea::SpanTop;
    case Keyword::SpanBottom:
        return PositionArea::SpanBottom;
    case Keyword::YStart:
        return PositionArea::YStart;
    case Keyword::YEnd:
        return PositionArea::YEnd;
    case Keyword::SpanYStart:
        return PositionArea::SpanYStart;
    case Keyword::SpanYEnd:
        return PositionArea::SpanYEnd;
    case Keyword::SelfYStart:
        return PositionArea::SelfYStart;
    case Keyword::SelfYEnd:
        return PositionArea::SelfYEnd;
    case Keyword::SpanSelfYStart:
        return PositionArea::SpanSelfYStart;
    case Keyword::SpanSelfYEnd:
        return PositionArea::SpanSelfYEnd;
    case Keyword::BlockStart:
        return PositionArea::BlockStart;
    case Keyword::BlockEnd:
        return PositionArea::BlockEnd;
    case Keyword::SpanBlockStart:
        return PositionArea::SpanBlockStart;
    case Keyword::SpanBlockEnd:
        return PositionArea::SpanBlockEnd;
    case Keyword::InlineStart:
        return PositionArea::InlineStart;
    case Keyword::InlineEnd:
        return PositionArea::InlineEnd;
    case Keyword::SpanInlineStart:
        return PositionArea::SpanInlineStart;
    case Keyword::SpanInlineEnd:
        return PositionArea::SpanInlineEnd;
    case Keyword::SelfBlockStart:
        return PositionArea::SelfBlockStart;
    case Keyword::SelfBlockEnd:
        return PositionArea::SelfBlockEnd;
    case Keyword::SpanSelfBlockStart:
        return PositionArea::SpanSelfBlockStart;
    case Keyword::SpanSelfBlockEnd:
        return PositionArea::SpanSelfBlockEnd;
    case Keyword::SelfInlineStart:
        return PositionArea::SelfInlineStart;
    case Keyword::SelfInlineEnd:
        return PositionArea::SelfInlineEnd;
    case Keyword::SpanSelfInlineStart:
        return PositionArea::SpanSelfInlineStart;
    case Keyword::SpanSelfInlineEnd:
        return PositionArea::SpanSelfInlineEnd;
    case Keyword::Start:
        return PositionArea::Start;
    case Keyword::End:
        return PositionArea::End;
    case Keyword::SpanStart:
        return PositionArea::SpanStart;
    case Keyword::SpanEnd:
        return PositionArea::SpanEnd;
    case Keyword::SelfStart:
        return PositionArea::SelfStart;
    case Keyword::SelfEnd:
        return PositionArea::SelfEnd;
    case Keyword::SpanSelfStart:
        return PositionArea::SpanSelfStart;
    case Keyword::SpanSelfEnd:
        return PositionArea::SpanSelfEnd;
    default:
        return {};
    }
}

Keyword to_keyword(PositionArea position_area_value)
{
    switch (position_area_value) {
    case PositionArea::Left:
        return Keyword::Left;
    case PositionArea::Center:
        return Keyword::Center;
    case PositionArea::Right:
        return Keyword::Right;
    case PositionArea::SpanLeft:
        return Keyword::SpanLeft;
    case PositionArea::SpanRight:
        return Keyword::SpanRight;
    case PositionArea::XStart:
        return Keyword::XStart;
    case PositionArea::XEnd:
        return Keyword::XEnd;
    case PositionArea::SpanXStart:
        return Keyword::SpanXStart;
    case PositionArea::SpanXEnd:
        return Keyword::SpanXEnd;
    case PositionArea::SelfXStart:
        return Keyword::SelfXStart;
    case PositionArea::SelfXEnd:
        return Keyword::SelfXEnd;
    case PositionArea::SpanSelfXStart:
        return Keyword::SpanSelfXStart;
    case PositionArea::SpanSelfXEnd:
        return Keyword::SpanSelfXEnd;
    case PositionArea::SpanAll:
        return Keyword::SpanAll;
    case PositionArea::Top:
        return Keyword::Top;
    case PositionArea::Bottom:
        return Keyword::Bottom;
    case PositionArea::SpanTop:
        return Keyword::SpanTop;
    case PositionArea::SpanBottom:
        return Keyword::SpanBottom;
    case PositionArea::YStart:
        return Keyword::YStart;
    case PositionArea::YEnd:
        return Keyword::YEnd;
    case PositionArea::SpanYStart:
        return Keyword::SpanYStart;
    case PositionArea::SpanYEnd:
        return Keyword::SpanYEnd;
    case PositionArea::SelfYStart:
        return Keyword::SelfYStart;
    case PositionArea::SelfYEnd:
        return Keyword::SelfYEnd;
    case PositionArea::SpanSelfYStart:
        return Keyword::SpanSelfYStart;
    case PositionArea::SpanSelfYEnd:
        return Keyword::SpanSelfYEnd;
    case PositionArea::BlockStart:
        return Keyword::BlockStart;
    case PositionArea::BlockEnd:
        return Keyword::BlockEnd;
    case PositionArea::SpanBlockStart:
        return Keyword::SpanBlockStart;
    case PositionArea::SpanBlockEnd:
        return Keyword::SpanBlockEnd;
    case PositionArea::InlineStart:
        return Keyword::InlineStart;
    case PositionArea::InlineEnd:
        return Keyword::InlineEnd;
    case PositionArea::SpanInlineStart:
        return Keyword::SpanInlineStart;
    case PositionArea::SpanInlineEnd:
        return Keyword::SpanInlineEnd;
    case PositionArea::SelfBlockStart:
        return Keyword::SelfBlockStart;
    case PositionArea::SelfBlockEnd:
        return Keyword::SelfBlockEnd;
    case PositionArea::SpanSelfBlockStart:
        return Keyword::SpanSelfBlockStart;
    case PositionArea::SpanSelfBlockEnd:
        return Keyword::SpanSelfBlockEnd;
    case PositionArea::SelfInlineStart:
        return Keyword::SelfInlineStart;
    case PositionArea::SelfInlineEnd:
        return Keyword::SelfInlineEnd;
    case PositionArea::SpanSelfInlineStart:
        return Keyword::SpanSelfInlineStart;
    case PositionArea::SpanSelfInlineEnd:
        return Keyword::SpanSelfInlineEnd;
    case PositionArea::Start:
        return Keyword::Start;
    case PositionArea::End:
        return Keyword::End;
    case PositionArea::SpanStart:
        return Keyword::SpanStart;
    case PositionArea::SpanEnd:
        return Keyword::SpanEnd;
    case PositionArea::SelfStart:
        return Keyword::SelfStart;
    case PositionArea::SelfEnd:
        return Keyword::SelfEnd;
    case PositionArea::SpanSelfStart:
        return Keyword::SpanSelfStart;
    case PositionArea::SpanSelfEnd:
        return Keyword::SpanSelfEnd;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PositionArea value)
{
    switch (value) {
    case PositionArea::Left:
        return "left"sv;
    case PositionArea::Center:
        return "center"sv;
    case PositionArea::Right:
        return "right"sv;
    case PositionArea::SpanLeft:
        return "span-left"sv;
    case PositionArea::SpanRight:
        return "span-right"sv;
    case PositionArea::XStart:
        return "x-start"sv;
    case PositionArea::XEnd:
        return "x-end"sv;
    case PositionArea::SpanXStart:
        return "span-x-start"sv;
    case PositionArea::SpanXEnd:
        return "span-x-end"sv;
    case PositionArea::SelfXStart:
        return "self-x-start"sv;
    case PositionArea::SelfXEnd:
        return "self-x-end"sv;
    case PositionArea::SpanSelfXStart:
        return "span-self-x-start"sv;
    case PositionArea::SpanSelfXEnd:
        return "span-self-x-end"sv;
    case PositionArea::SpanAll:
        return "span-all"sv;
    case PositionArea::Top:
        return "top"sv;
    case PositionArea::Bottom:
        return "bottom"sv;
    case PositionArea::SpanTop:
        return "span-top"sv;
    case PositionArea::SpanBottom:
        return "span-bottom"sv;
    case PositionArea::YStart:
        return "y-start"sv;
    case PositionArea::YEnd:
        return "y-end"sv;
    case PositionArea::SpanYStart:
        return "span-y-start"sv;
    case PositionArea::SpanYEnd:
        return "span-y-end"sv;
    case PositionArea::SelfYStart:
        return "self-y-start"sv;
    case PositionArea::SelfYEnd:
        return "self-y-end"sv;
    case PositionArea::SpanSelfYStart:
        return "span-self-y-start"sv;
    case PositionArea::SpanSelfYEnd:
        return "span-self-y-end"sv;
    case PositionArea::BlockStart:
        return "block-start"sv;
    case PositionArea::BlockEnd:
        return "block-end"sv;
    case PositionArea::SpanBlockStart:
        return "span-block-start"sv;
    case PositionArea::SpanBlockEnd:
        return "span-block-end"sv;
    case PositionArea::InlineStart:
        return "inline-start"sv;
    case PositionArea::InlineEnd:
        return "inline-end"sv;
    case PositionArea::SpanInlineStart:
        return "span-inline-start"sv;
    case PositionArea::SpanInlineEnd:
        return "span-inline-end"sv;
    case PositionArea::SelfBlockStart:
        return "self-block-start"sv;
    case PositionArea::SelfBlockEnd:
        return "self-block-end"sv;
    case PositionArea::SpanSelfBlockStart:
        return "span-self-block-start"sv;
    case PositionArea::SpanSelfBlockEnd:
        return "span-self-block-end"sv;
    case PositionArea::SelfInlineStart:
        return "self-inline-start"sv;
    case PositionArea::SelfInlineEnd:
        return "self-inline-end"sv;
    case PositionArea::SpanSelfInlineStart:
        return "span-self-inline-start"sv;
    case PositionArea::SpanSelfInlineEnd:
        return "span-self-inline-end"sv;
    case PositionArea::Start:
        return "start"sv;
    case PositionArea::End:
        return "end"sv;
    case PositionArea::SpanStart:
        return "span-start"sv;
    case PositionArea::SpanEnd:
        return "span-end"sv;
    case PositionArea::SelfStart:
        return "self-start"sv;
    case PositionArea::SelfEnd:
        return "self-end"sv;
    case PositionArea::SpanSelfStart:
        return "span-self-start"sv;
    case PositionArea::SpanSelfEnd:
        return "span-self-end"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<PositionEdge> keyword_to_position_edge(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Center:
        return PositionEdge::Center;
    case Keyword::Left:
        return PositionEdge::Left;
    case Keyword::Right:
        return PositionEdge::Right;
    case Keyword::Top:
        return PositionEdge::Top;
    case Keyword::Bottom:
        return PositionEdge::Bottom;
    default:
        return {};
    }
}

Keyword to_keyword(PositionEdge position_edge_value)
{
    switch (position_edge_value) {
    case PositionEdge::Center:
        return Keyword::Center;
    case PositionEdge::Left:
        return Keyword::Left;
    case PositionEdge::Right:
        return Keyword::Right;
    case PositionEdge::Top:
        return Keyword::Top;
    case PositionEdge::Bottom:
        return Keyword::Bottom;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(PositionEdge value)
{
    switch (value) {
    case PositionEdge::Center:
        return "center"sv;
    case PositionEdge::Left:
        return "left"sv;
    case PositionEdge::Right:
        return "right"sv;
    case PositionEdge::Top:
        return "top"sv;
    case PositionEdge::Bottom:
        return "bottom"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Positioning> keyword_to_positioning(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Absolute:
        return Positioning::Absolute;
    case Keyword::Fixed:
        return Positioning::Fixed;
    case Keyword::Relative:
        return Positioning::Relative;
    case Keyword::Static:
        return Positioning::Static;
    case Keyword::Sticky:
        return Positioning::Sticky;
    default:
        return {};
    }
}

Keyword to_keyword(Positioning positioning_value)
{
    switch (positioning_value) {
    case Positioning::Absolute:
        return Keyword::Absolute;
    case Positioning::Fixed:
        return Keyword::Fixed;
    case Positioning::Relative:
        return Keyword::Relative;
    case Positioning::Static:
        return Keyword::Static;
    case Positioning::Sticky:
        return Keyword::Sticky;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Positioning value)
{
    switch (value) {
    case Positioning::Absolute:
        return "absolute"sv;
    case Positioning::Fixed:
        return "fixed"sv;
    case Positioning::Relative:
        return "relative"sv;
    case Positioning::Static:
        return "static"sv;
    case Positioning::Sticky:
        return "sticky"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<RadialExtent> keyword_to_radial_extent(Keyword keyword)
{
    switch (keyword) {
    case Keyword::ClosestCorner:
        return RadialExtent::ClosestCorner;
    case Keyword::ClosestSide:
        return RadialExtent::ClosestSide;
    case Keyword::FarthestCorner:
        return RadialExtent::FarthestCorner;
    case Keyword::FarthestSide:
        return RadialExtent::FarthestSide;
    default:
        return {};
    }
}

Keyword to_keyword(RadialExtent radial_extent_value)
{
    switch (radial_extent_value) {
    case RadialExtent::ClosestCorner:
        return Keyword::ClosestCorner;
    case RadialExtent::ClosestSide:
        return Keyword::ClosestSide;
    case RadialExtent::FarthestCorner:
        return Keyword::FarthestCorner;
    case RadialExtent::FarthestSide:
        return Keyword::FarthestSide;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(RadialExtent value)
{
    switch (value) {
    case RadialExtent::ClosestCorner:
        return "closest-corner"sv;
    case RadialExtent::ClosestSide:
        return "closest-side"sv;
    case RadialExtent::FarthestCorner:
        return "farthest-corner"sv;
    case RadialExtent::FarthestSide:
        return "farthest-side"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ReferrerPolicyModifierValue> keyword_to_referrer_policy_modifier_value(Keyword keyword)
{
    switch (keyword) {
    case Keyword::NoReferrer:
        return ReferrerPolicyModifierValue::NoReferrer;
    case Keyword::NoReferrerWhenDowngrade:
        return ReferrerPolicyModifierValue::NoReferrerWhenDowngrade;
    case Keyword::SameOrigin:
        return ReferrerPolicyModifierValue::SameOrigin;
    case Keyword::Origin:
        return ReferrerPolicyModifierValue::Origin;
    case Keyword::StrictOrigin:
        return ReferrerPolicyModifierValue::StrictOrigin;
    case Keyword::OriginWhenCrossOrigin:
        return ReferrerPolicyModifierValue::OriginWhenCrossOrigin;
    case Keyword::StrictOriginWhenCrossOrigin:
        return ReferrerPolicyModifierValue::StrictOriginWhenCrossOrigin;
    case Keyword::UnsafeUrl:
        return ReferrerPolicyModifierValue::UnsafeUrl;
    default:
        return {};
    }
}

Keyword to_keyword(ReferrerPolicyModifierValue referrer_policy_modifier_value_value)
{
    switch (referrer_policy_modifier_value_value) {
    case ReferrerPolicyModifierValue::NoReferrer:
        return Keyword::NoReferrer;
    case ReferrerPolicyModifierValue::NoReferrerWhenDowngrade:
        return Keyword::NoReferrerWhenDowngrade;
    case ReferrerPolicyModifierValue::SameOrigin:
        return Keyword::SameOrigin;
    case ReferrerPolicyModifierValue::Origin:
        return Keyword::Origin;
    case ReferrerPolicyModifierValue::StrictOrigin:
        return Keyword::StrictOrigin;
    case ReferrerPolicyModifierValue::OriginWhenCrossOrigin:
        return Keyword::OriginWhenCrossOrigin;
    case ReferrerPolicyModifierValue::StrictOriginWhenCrossOrigin:
        return Keyword::StrictOriginWhenCrossOrigin;
    case ReferrerPolicyModifierValue::UnsafeUrl:
        return Keyword::UnsafeUrl;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ReferrerPolicyModifierValue value)
{
    switch (value) {
    case ReferrerPolicyModifierValue::NoReferrer:
        return "no-referrer"sv;
    case ReferrerPolicyModifierValue::NoReferrerWhenDowngrade:
        return "no-referrer-when-downgrade"sv;
    case ReferrerPolicyModifierValue::SameOrigin:
        return "same-origin"sv;
    case ReferrerPolicyModifierValue::Origin:
        return "origin"sv;
    case ReferrerPolicyModifierValue::StrictOrigin:
        return "strict-origin"sv;
    case ReferrerPolicyModifierValue::OriginWhenCrossOrigin:
        return "origin-when-cross-origin"sv;
    case ReferrerPolicyModifierValue::StrictOriginWhenCrossOrigin:
        return "strict-origin-when-cross-origin"sv;
    case ReferrerPolicyModifierValue::UnsafeUrl:
        return "unsafe-url"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<RelativeSize> keyword_to_relative_size(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Smaller:
        return RelativeSize::Smaller;
    case Keyword::Larger:
        return RelativeSize::Larger;
    default:
        return {};
    }
}

Keyword to_keyword(RelativeSize relative_size_value)
{
    switch (relative_size_value) {
    case RelativeSize::Smaller:
        return Keyword::Smaller;
    case RelativeSize::Larger:
        return Keyword::Larger;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(RelativeSize value)
{
    switch (value) {
    case RelativeSize::Smaller:
        return "smaller"sv;
    case RelativeSize::Larger:
        return "larger"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Repetition> keyword_to_repetition(Keyword keyword)
{
    switch (keyword) {
    case Keyword::NoRepeat:
        return Repetition::NoRepeat;
    case Keyword::Repeat:
        return Repetition::Repeat;
    case Keyword::Round:
        return Repetition::Round;
    case Keyword::Space:
        return Repetition::Space;
    default:
        return {};
    }
}

Keyword to_keyword(Repetition repetition_value)
{
    switch (repetition_value) {
    case Repetition::NoRepeat:
        return Keyword::NoRepeat;
    case Repetition::Repeat:
        return Keyword::Repeat;
    case Repetition::Round:
        return Keyword::Round;
    case Repetition::Space:
        return Keyword::Space;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Repetition value)
{
    switch (value) {
    case Repetition::NoRepeat:
        return "no-repeat"sv;
    case Repetition::Repeat:
        return "repeat"sv;
    case Repetition::Round:
        return "round"sv;
    case Repetition::Space:
        return "space"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Resize> keyword_to_resize(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return Resize::None;
    case Keyword::Both:
        return Resize::Both;
    case Keyword::Horizontal:
        return Resize::Horizontal;
    case Keyword::Vertical:
        return Resize::Vertical;
    case Keyword::Inline:
        return Resize::Inline;
    case Keyword::Block:
        return Resize::Block;
    default:
        return {};
    }
}

Keyword to_keyword(Resize resize_value)
{
    switch (resize_value) {
    case Resize::None:
        return Keyword::None;
    case Resize::Both:
        return Keyword::Both;
    case Resize::Horizontal:
        return Keyword::Horizontal;
    case Resize::Vertical:
        return Keyword::Vertical;
    case Resize::Inline:
        return Keyword::Inline;
    case Resize::Block:
        return Keyword::Block;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Resize value)
{
    switch (value) {
    case Resize::None:
        return "none"sv;
    case Resize::Both:
        return "both"sv;
    case Resize::Horizontal:
        return "horizontal"sv;
    case Resize::Vertical:
        return "vertical"sv;
    case Resize::Inline:
        return "inline"sv;
    case Resize::Block:
        return "block"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<RoundingStrategy> keyword_to_rounding_strategy(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Down:
        return RoundingStrategy::Down;
    case Keyword::Nearest:
        return RoundingStrategy::Nearest;
    case Keyword::ToZero:
        return RoundingStrategy::ToZero;
    case Keyword::Up:
        return RoundingStrategy::Up;
    default:
        return {};
    }
}

Keyword to_keyword(RoundingStrategy rounding_strategy_value)
{
    switch (rounding_strategy_value) {
    case RoundingStrategy::Down:
        return Keyword::Down;
    case RoundingStrategy::Nearest:
        return Keyword::Nearest;
    case RoundingStrategy::ToZero:
        return Keyword::ToZero;
    case RoundingStrategy::Up:
        return Keyword::Up;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(RoundingStrategy value)
{
    switch (value) {
    case RoundingStrategy::Down:
        return "down"sv;
    case RoundingStrategy::Nearest:
        return "nearest"sv;
    case RoundingStrategy::ToZero:
        return "to-zero"sv;
    case RoundingStrategy::Up:
        return "up"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ScrollbarGutter> keyword_to_scrollbar_gutter(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return ScrollbarGutter::Auto;
    case Keyword::Stable:
        return ScrollbarGutter::Stable;
    case Keyword::BothEdges:
        return ScrollbarGutter::BothEdges;
    default:
        return {};
    }
}

Keyword to_keyword(ScrollbarGutter scrollbar_gutter_value)
{
    switch (scrollbar_gutter_value) {
    case ScrollbarGutter::Auto:
        return Keyword::Auto;
    case ScrollbarGutter::Stable:
        return Keyword::Stable;
    case ScrollbarGutter::BothEdges:
        return Keyword::BothEdges;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ScrollbarGutter value)
{
    switch (value) {
    case ScrollbarGutter::Auto:
        return "auto"sv;
    case ScrollbarGutter::Stable:
        return "stable"sv;
    case ScrollbarGutter::BothEdges:
        return "both-edges"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ScrollbarWidth> keyword_to_scrollbar_width(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return ScrollbarWidth::Auto;
    case Keyword::Thin:
        return ScrollbarWidth::Thin;
    case Keyword::None:
        return ScrollbarWidth::None;
    default:
        return {};
    }
}

Keyword to_keyword(ScrollbarWidth scrollbar_width_value)
{
    switch (scrollbar_width_value) {
    case ScrollbarWidth::Auto:
        return Keyword::Auto;
    case ScrollbarWidth::Thin:
        return Keyword::Thin;
    case ScrollbarWidth::None:
        return Keyword::None;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ScrollbarWidth value)
{
    switch (value) {
    case ScrollbarWidth::Auto:
        return "auto"sv;
    case ScrollbarWidth::Thin:
        return "thin"sv;
    case ScrollbarWidth::None:
        return "none"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Scroller> keyword_to_scroller(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Root:
        return Scroller::Root;
    case Keyword::Nearest:
        return Scroller::Nearest;
    case Keyword::Self:
        return Scroller::Self;
    default:
        return {};
    }
}

Keyword to_keyword(Scroller scroller_value)
{
    switch (scroller_value) {
    case Scroller::Root:
        return Keyword::Root;
    case Scroller::Nearest:
        return Keyword::Nearest;
    case Scroller::Self:
        return Keyword::Self;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Scroller value)
{
    switch (value) {
    case Scroller::Root:
        return "root"sv;
    case Scroller::Nearest:
        return "nearest"sv;
    case Scroller::Self:
        return "self"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ShapeBox> keyword_to_shape_box(Keyword keyword)
{
    switch (keyword) {
    case Keyword::ContentBox:
        return ShapeBox::ContentBox;
    case Keyword::PaddingBox:
        return ShapeBox::PaddingBox;
    case Keyword::BorderBox:
        return ShapeBox::BorderBox;
    case Keyword::MarginBox:
        return ShapeBox::MarginBox;
    default:
        return {};
    }
}

Keyword to_keyword(ShapeBox shape_box_value)
{
    switch (shape_box_value) {
    case ShapeBox::ContentBox:
        return Keyword::ContentBox;
    case ShapeBox::PaddingBox:
        return Keyword::PaddingBox;
    case ShapeBox::BorderBox:
        return Keyword::BorderBox;
    case ShapeBox::MarginBox:
        return Keyword::MarginBox;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ShapeBox value)
{
    switch (value) {
    case ShapeBox::ContentBox:
        return "content-box"sv;
    case ShapeBox::PaddingBox:
        return "padding-box"sv;
    case ShapeBox::BorderBox:
        return "border-box"sv;
    case ShapeBox::MarginBox:
        return "margin-box"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<ShapeRendering> keyword_to_shape_rendering(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return ShapeRendering::Auto;
    case Keyword::Optimizespeed:
        return ShapeRendering::Optimizespeed;
    case Keyword::Crispedges:
        return ShapeRendering::Crispedges;
    case Keyword::Geometricprecision:
        return ShapeRendering::Geometricprecision;
    default:
        return {};
    }
}

Keyword to_keyword(ShapeRendering shape_rendering_value)
{
    switch (shape_rendering_value) {
    case ShapeRendering::Auto:
        return Keyword::Auto;
    case ShapeRendering::Optimizespeed:
        return Keyword::Optimizespeed;
    case ShapeRendering::Crispedges:
        return Keyword::Crispedges;
    case ShapeRendering::Geometricprecision:
        return Keyword::Geometricprecision;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(ShapeRendering value)
{
    switch (value) {
    case ShapeRendering::Auto:
        return "auto"sv;
    case ShapeRendering::Optimizespeed:
        return "optimizespeed"sv;
    case ShapeRendering::Crispedges:
        return "crispedges"sv;
    case ShapeRendering::Geometricprecision:
        return "geometricprecision"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<StepPosition> keyword_to_step_position(Keyword keyword)
{
    switch (keyword) {
    case Keyword::JumpStart:
        return StepPosition::JumpStart;
    case Keyword::JumpEnd:
        return StepPosition::JumpEnd;
    case Keyword::JumpNone:
        return StepPosition::JumpNone;
    case Keyword::JumpBoth:
        return StepPosition::JumpBoth;
    case Keyword::Start:
        return StepPosition::Start;
    case Keyword::End:
        return StepPosition::End;
    default:
        return {};
    }
}

Keyword to_keyword(StepPosition step_position_value)
{
    switch (step_position_value) {
    case StepPosition::JumpStart:
        return Keyword::JumpStart;
    case StepPosition::JumpEnd:
        return Keyword::JumpEnd;
    case StepPosition::JumpNone:
        return Keyword::JumpNone;
    case StepPosition::JumpBoth:
        return Keyword::JumpBoth;
    case StepPosition::Start:
        return Keyword::Start;
    case StepPosition::End:
        return Keyword::End;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(StepPosition value)
{
    switch (value) {
    case StepPosition::JumpStart:
        return "jump-start"sv;
    case StepPosition::JumpEnd:
        return "jump-end"sv;
    case StepPosition::JumpNone:
        return "jump-none"sv;
    case StepPosition::JumpBoth:
        return "jump-both"sv;
    case StepPosition::Start:
        return "start"sv;
    case StepPosition::End:
        return "end"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<StrokeLinecap> keyword_to_stroke_linecap(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Butt:
        return StrokeLinecap::Butt;
    case Keyword::Square:
        return StrokeLinecap::Square;
    case Keyword::Round:
        return StrokeLinecap::Round;
    default:
        return {};
    }
}

Keyword to_keyword(StrokeLinecap stroke_linecap_value)
{
    switch (stroke_linecap_value) {
    case StrokeLinecap::Butt:
        return Keyword::Butt;
    case StrokeLinecap::Square:
        return Keyword::Square;
    case StrokeLinecap::Round:
        return Keyword::Round;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(StrokeLinecap value)
{
    switch (value) {
    case StrokeLinecap::Butt:
        return "butt"sv;
    case StrokeLinecap::Square:
        return "square"sv;
    case StrokeLinecap::Round:
        return "round"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<StrokeLinejoin> keyword_to_stroke_linejoin(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Miter:
        return StrokeLinejoin::Miter;
    case Keyword::Round:
        return StrokeLinejoin::Round;
    case Keyword::Bevel:
        return StrokeLinejoin::Bevel;
    default:
        return {};
    }
}

Keyword to_keyword(StrokeLinejoin stroke_linejoin_value)
{
    switch (stroke_linejoin_value) {
    case StrokeLinejoin::Miter:
        return Keyword::Miter;
    case StrokeLinejoin::Round:
        return Keyword::Round;
    case StrokeLinejoin::Bevel:
        return Keyword::Bevel;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(StrokeLinejoin value)
{
    switch (value) {
    case StrokeLinejoin::Miter:
        return "miter"sv;
    case StrokeLinejoin::Round:
        return "round"sv;
    case StrokeLinejoin::Bevel:
        return "bevel"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<SymbolsType> keyword_to_symbols_type(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Cyclic:
        return SymbolsType::Cyclic;
    case Keyword::Numeric:
        return SymbolsType::Numeric;
    case Keyword::Alphabetic:
        return SymbolsType::Alphabetic;
    case Keyword::Symbolic:
        return SymbolsType::Symbolic;
    case Keyword::Fixed:
        return SymbolsType::Fixed;
    default:
        return {};
    }
}

Keyword to_keyword(SymbolsType symbols_type_value)
{
    switch (symbols_type_value) {
    case SymbolsType::Cyclic:
        return Keyword::Cyclic;
    case SymbolsType::Numeric:
        return Keyword::Numeric;
    case SymbolsType::Alphabetic:
        return Keyword::Alphabetic;
    case SymbolsType::Symbolic:
        return Keyword::Symbolic;
    case SymbolsType::Fixed:
        return Keyword::Fixed;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(SymbolsType value)
{
    switch (value) {
    case SymbolsType::Cyclic:
        return "cyclic"sv;
    case SymbolsType::Numeric:
        return "numeric"sv;
    case SymbolsType::Alphabetic:
        return "alphabetic"sv;
    case SymbolsType::Symbolic:
        return "symbolic"sv;
    case SymbolsType::Fixed:
        return "fixed"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TableLayout> keyword_to_table_layout(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TableLayout::Auto;
    case Keyword::Fixed:
        return TableLayout::Fixed;
    default:
        return {};
    }
}

Keyword to_keyword(TableLayout table_layout_value)
{
    switch (table_layout_value) {
    case TableLayout::Auto:
        return Keyword::Auto;
    case TableLayout::Fixed:
        return Keyword::Fixed;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TableLayout value)
{
    switch (value) {
    case TableLayout::Auto:
        return "auto"sv;
    case TableLayout::Fixed:
        return "fixed"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextAlign> keyword_to_text_align(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Center:
        return TextAlign::Center;
    case Keyword::Justify:
        return TextAlign::Justify;
    case Keyword::Start:
        return TextAlign::Start;
    case Keyword::End:
        return TextAlign::End;
    case Keyword::Left:
        return TextAlign::Left;
    case Keyword::Right:
        return TextAlign::Right;
    case Keyword::MatchParent:
        return TextAlign::MatchParent;
    case Keyword::LibwebCenter:
        return TextAlign::LibwebCenter;
    case Keyword::LibwebInheritOrCenter:
        return TextAlign::LibwebInheritOrCenter;
    case Keyword::LibwebLeft:
        return TextAlign::LibwebLeft;
    case Keyword::LibwebRight:
        return TextAlign::LibwebRight;
    default:
        return {};
    }
}

Keyword to_keyword(TextAlign text_align_value)
{
    switch (text_align_value) {
    case TextAlign::Center:
        return Keyword::Center;
    case TextAlign::Justify:
        return Keyword::Justify;
    case TextAlign::Start:
        return Keyword::Start;
    case TextAlign::End:
        return Keyword::End;
    case TextAlign::Left:
        return Keyword::Left;
    case TextAlign::Right:
        return Keyword::Right;
    case TextAlign::MatchParent:
        return Keyword::MatchParent;
    case TextAlign::LibwebCenter:
        return Keyword::LibwebCenter;
    case TextAlign::LibwebInheritOrCenter:
        return Keyword::LibwebInheritOrCenter;
    case TextAlign::LibwebLeft:
        return Keyword::LibwebLeft;
    case TextAlign::LibwebRight:
        return Keyword::LibwebRight;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextAlign value)
{
    switch (value) {
    case TextAlign::Center:
        return "center"sv;
    case TextAlign::Justify:
        return "justify"sv;
    case TextAlign::Start:
        return "start"sv;
    case TextAlign::End:
        return "end"sv;
    case TextAlign::Left:
        return "left"sv;
    case TextAlign::Right:
        return "right"sv;
    case TextAlign::MatchParent:
        return "match-parent"sv;
    case TextAlign::LibwebCenter:
        return "-libweb-center"sv;
    case TextAlign::LibwebInheritOrCenter:
        return "-libweb-inherit-or-center"sv;
    case TextAlign::LibwebLeft:
        return "-libweb-left"sv;
    case TextAlign::LibwebRight:
        return "-libweb-right"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextAnchor> keyword_to_text_anchor(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Start:
        return TextAnchor::Start;
    case Keyword::Middle:
        return TextAnchor::Middle;
    case Keyword::End:
        return TextAnchor::End;
    default:
        return {};
    }
}

Keyword to_keyword(TextAnchor text_anchor_value)
{
    switch (text_anchor_value) {
    case TextAnchor::Start:
        return Keyword::Start;
    case TextAnchor::Middle:
        return Keyword::Middle;
    case TextAnchor::End:
        return Keyword::End;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextAnchor value)
{
    switch (value) {
    case TextAnchor::Start:
        return "start"sv;
    case TextAnchor::Middle:
        return "middle"sv;
    case TextAnchor::End:
        return "end"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextDecorationLine> keyword_to_text_decoration_line(Keyword keyword)
{
    switch (keyword) {
    case Keyword::None:
        return TextDecorationLine::None;
    case Keyword::Underline:
        return TextDecorationLine::Underline;
    case Keyword::Overline:
        return TextDecorationLine::Overline;
    case Keyword::LineThrough:
        return TextDecorationLine::LineThrough;
    case Keyword::Blink:
        return TextDecorationLine::Blink;
    case Keyword::SpellingError:
        return TextDecorationLine::SpellingError;
    case Keyword::GrammarError:
        return TextDecorationLine::GrammarError;
    default:
        return {};
    }
}

Keyword to_keyword(TextDecorationLine text_decoration_line_value)
{
    switch (text_decoration_line_value) {
    case TextDecorationLine::None:
        return Keyword::None;
    case TextDecorationLine::Underline:
        return Keyword::Underline;
    case TextDecorationLine::Overline:
        return Keyword::Overline;
    case TextDecorationLine::LineThrough:
        return Keyword::LineThrough;
    case TextDecorationLine::Blink:
        return Keyword::Blink;
    case TextDecorationLine::SpellingError:
        return Keyword::SpellingError;
    case TextDecorationLine::GrammarError:
        return Keyword::GrammarError;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextDecorationLine value)
{
    switch (value) {
    case TextDecorationLine::None:
        return "none"sv;
    case TextDecorationLine::Underline:
        return "underline"sv;
    case TextDecorationLine::Overline:
        return "overline"sv;
    case TextDecorationLine::LineThrough:
        return "line-through"sv;
    case TextDecorationLine::Blink:
        return "blink"sv;
    case TextDecorationLine::SpellingError:
        return "spelling-error"sv;
    case TextDecorationLine::GrammarError:
        return "grammar-error"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextDecorationStyle> keyword_to_text_decoration_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Dashed:
        return TextDecorationStyle::Dashed;
    case Keyword::Dotted:
        return TextDecorationStyle::Dotted;
    case Keyword::Double:
        return TextDecorationStyle::Double;
    case Keyword::Solid:
        return TextDecorationStyle::Solid;
    case Keyword::Wavy:
        return TextDecorationStyle::Wavy;
    default:
        return {};
    }
}

Keyword to_keyword(TextDecorationStyle text_decoration_style_value)
{
    switch (text_decoration_style_value) {
    case TextDecorationStyle::Dashed:
        return Keyword::Dashed;
    case TextDecorationStyle::Dotted:
        return Keyword::Dotted;
    case TextDecorationStyle::Double:
        return Keyword::Double;
    case TextDecorationStyle::Solid:
        return Keyword::Solid;
    case TextDecorationStyle::Wavy:
        return Keyword::Wavy;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextDecorationStyle value)
{
    switch (value) {
    case TextDecorationStyle::Dashed:
        return "dashed"sv;
    case TextDecorationStyle::Dotted:
        return "dotted"sv;
    case TextDecorationStyle::Double:
        return "double"sv;
    case TextDecorationStyle::Solid:
        return "solid"sv;
    case TextDecorationStyle::Wavy:
        return "wavy"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextJustify> keyword_to_text_justify(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TextJustify::Auto;
    case Keyword::None:
        return TextJustify::None;
    case Keyword::InterWord:
        return TextJustify::InterWord;
    case Keyword::InterCharacter:
        return TextJustify::InterCharacter;
    default:
        return {};
    }
}

Keyword to_keyword(TextJustify text_justify_value)
{
    switch (text_justify_value) {
    case TextJustify::Auto:
        return Keyword::Auto;
    case TextJustify::None:
        return Keyword::None;
    case TextJustify::InterWord:
        return Keyword::InterWord;
    case TextJustify::InterCharacter:
        return Keyword::InterCharacter;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextJustify value)
{
    switch (value) {
    case TextJustify::Auto:
        return "auto"sv;
    case TextJustify::None:
        return "none"sv;
    case TextJustify::InterWord:
        return "inter-word"sv;
    case TextJustify::InterCharacter:
        return "inter-character"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextOverflow> keyword_to_text_overflow(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Clip:
        return TextOverflow::Clip;
    case Keyword::Ellipsis:
        return TextOverflow::Ellipsis;
    default:
        return {};
    }
}

Keyword to_keyword(TextOverflow text_overflow_value)
{
    switch (text_overflow_value) {
    case TextOverflow::Clip:
        return Keyword::Clip;
    case TextOverflow::Ellipsis:
        return Keyword::Ellipsis;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextOverflow value)
{
    switch (value) {
    case TextOverflow::Clip:
        return "clip"sv;
    case TextOverflow::Ellipsis:
        return "ellipsis"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextRendering> keyword_to_text_rendering(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TextRendering::Auto;
    case Keyword::Optimizespeed:
        return TextRendering::Optimizespeed;
    case Keyword::Optimizelegibility:
        return TextRendering::Optimizelegibility;
    case Keyword::Geometricprecision:
        return TextRendering::Geometricprecision;
    default:
        return {};
    }
}

Keyword to_keyword(TextRendering text_rendering_value)
{
    switch (text_rendering_value) {
    case TextRendering::Auto:
        return Keyword::Auto;
    case TextRendering::Optimizespeed:
        return Keyword::Optimizespeed;
    case TextRendering::Optimizelegibility:
        return Keyword::Optimizelegibility;
    case TextRendering::Geometricprecision:
        return Keyword::Geometricprecision;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextRendering value)
{
    switch (value) {
    case TextRendering::Auto:
        return "auto"sv;
    case TextRendering::Optimizespeed:
        return "optimizespeed"sv;
    case TextRendering::Optimizelegibility:
        return "optimizelegibility"sv;
    case TextRendering::Geometricprecision:
        return "geometricprecision"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextTransform> keyword_to_text_transform(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Capitalize:
        return TextTransform::Capitalize;
    case Keyword::FullSizeKana:
        return TextTransform::FullSizeKana;
    case Keyword::FullWidth:
        return TextTransform::FullWidth;
    case Keyword::Lowercase:
        return TextTransform::Lowercase;
    case Keyword::MathAuto:
        return TextTransform::MathAuto;
    case Keyword::None:
        return TextTransform::None;
    case Keyword::Uppercase:
        return TextTransform::Uppercase;
    default:
        return {};
    }
}

Keyword to_keyword(TextTransform text_transform_value)
{
    switch (text_transform_value) {
    case TextTransform::Capitalize:
        return Keyword::Capitalize;
    case TextTransform::FullSizeKana:
        return Keyword::FullSizeKana;
    case TextTransform::FullWidth:
        return Keyword::FullWidth;
    case TextTransform::Lowercase:
        return Keyword::Lowercase;
    case TextTransform::MathAuto:
        return Keyword::MathAuto;
    case TextTransform::None:
        return Keyword::None;
    case TextTransform::Uppercase:
        return Keyword::Uppercase;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextTransform value)
{
    switch (value) {
    case TextTransform::Capitalize:
        return "capitalize"sv;
    case TextTransform::FullSizeKana:
        return "full-size-kana"sv;
    case TextTransform::FullWidth:
        return "full-width"sv;
    case TextTransform::Lowercase:
        return "lowercase"sv;
    case TextTransform::MathAuto:
        return "math-auto"sv;
    case TextTransform::None:
        return "none"sv;
    case TextTransform::Uppercase:
        return "uppercase"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextUnderlinePositionHorizontal> keyword_to_text_underline_position_horizontal(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TextUnderlinePositionHorizontal::Auto;
    case Keyword::FromFont:
        return TextUnderlinePositionHorizontal::FromFont;
    case Keyword::Under:
        return TextUnderlinePositionHorizontal::Under;
    default:
        return {};
    }
}

Keyword to_keyword(TextUnderlinePositionHorizontal text_underline_position_horizontal_value)
{
    switch (text_underline_position_horizontal_value) {
    case TextUnderlinePositionHorizontal::Auto:
        return Keyword::Auto;
    case TextUnderlinePositionHorizontal::FromFont:
        return Keyword::FromFont;
    case TextUnderlinePositionHorizontal::Under:
        return Keyword::Under;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextUnderlinePositionHorizontal value)
{
    switch (value) {
    case TextUnderlinePositionHorizontal::Auto:
        return "auto"sv;
    case TextUnderlinePositionHorizontal::FromFont:
        return "from-font"sv;
    case TextUnderlinePositionHorizontal::Under:
        return "under"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextUnderlinePositionVertical> keyword_to_text_underline_position_vertical(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TextUnderlinePositionVertical::Auto;
    case Keyword::Left:
        return TextUnderlinePositionVertical::Left;
    case Keyword::Right:
        return TextUnderlinePositionVertical::Right;
    default:
        return {};
    }
}

Keyword to_keyword(TextUnderlinePositionVertical text_underline_position_vertical_value)
{
    switch (text_underline_position_vertical_value) {
    case TextUnderlinePositionVertical::Auto:
        return Keyword::Auto;
    case TextUnderlinePositionVertical::Left:
        return Keyword::Left;
    case TextUnderlinePositionVertical::Right:
        return Keyword::Right;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextUnderlinePositionVertical value)
{
    switch (value) {
    case TextUnderlinePositionVertical::Auto:
        return "auto"sv;
    case TextUnderlinePositionVertical::Left:
        return "left"sv;
    case TextUnderlinePositionVertical::Right:
        return "right"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextWrapMode> keyword_to_text_wrap_mode(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Wrap:
        return TextWrapMode::Wrap;
    case Keyword::Nowrap:
        return TextWrapMode::Nowrap;
    default:
        return {};
    }
}

Keyword to_keyword(TextWrapMode text_wrap_mode_value)
{
    switch (text_wrap_mode_value) {
    case TextWrapMode::Wrap:
        return Keyword::Wrap;
    case TextWrapMode::Nowrap:
        return Keyword::Nowrap;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextWrapMode value)
{
    switch (value) {
    case TextWrapMode::Wrap:
        return "wrap"sv;
    case TextWrapMode::Nowrap:
        return "nowrap"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TextWrapStyle> keyword_to_text_wrap_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TextWrapStyle::Auto;
    case Keyword::Balance:
        return TextWrapStyle::Balance;
    case Keyword::Stable:
        return TextWrapStyle::Stable;
    case Keyword::Pretty:
        return TextWrapStyle::Pretty;
    default:
        return {};
    }
}

Keyword to_keyword(TextWrapStyle text_wrap_style_value)
{
    switch (text_wrap_style_value) {
    case TextWrapStyle::Auto:
        return Keyword::Auto;
    case TextWrapStyle::Balance:
        return Keyword::Balance;
    case TextWrapStyle::Stable:
        return Keyword::Stable;
    case TextWrapStyle::Pretty:
        return Keyword::Pretty;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TextWrapStyle value)
{
    switch (value) {
    case TextWrapStyle::Auto:
        return "auto"sv;
    case TextWrapStyle::Balance:
        return "balance"sv;
    case TextWrapStyle::Stable:
        return "stable"sv;
    case TextWrapStyle::Pretty:
        return "pretty"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TouchAction> keyword_to_touch_action(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Auto:
        return TouchAction::Auto;
    case Keyword::Manipulation:
        return TouchAction::Manipulation;
    case Keyword::None:
        return TouchAction::None;
    case Keyword::PanDown:
        return TouchAction::PanDown;
    case Keyword::PanLeft:
        return TouchAction::PanLeft;
    case Keyword::PanRight:
        return TouchAction::PanRight;
    case Keyword::PanUp:
        return TouchAction::PanUp;
    case Keyword::PanX:
        return TouchAction::PanX;
    case Keyword::PanY:
        return TouchAction::PanY;
    default:
        return {};
    }
}

Keyword to_keyword(TouchAction touch_action_value)
{
    switch (touch_action_value) {
    case TouchAction::Auto:
        return Keyword::Auto;
    case TouchAction::Manipulation:
        return Keyword::Manipulation;
    case TouchAction::None:
        return Keyword::None;
    case TouchAction::PanDown:
        return Keyword::PanDown;
    case TouchAction::PanLeft:
        return Keyword::PanLeft;
    case TouchAction::PanRight:
        return Keyword::PanRight;
    case TouchAction::PanUp:
        return Keyword::PanUp;
    case TouchAction::PanX:
        return Keyword::PanX;
    case TouchAction::PanY:
        return Keyword::PanY;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TouchAction value)
{
    switch (value) {
    case TouchAction::Auto:
        return "auto"sv;
    case TouchAction::Manipulation:
        return "manipulation"sv;
    case TouchAction::None:
        return "none"sv;
    case TouchAction::PanDown:
        return "pan-down"sv;
    case TouchAction::PanLeft:
        return "pan-left"sv;
    case TouchAction::PanRight:
        return "pan-right"sv;
    case TouchAction::PanUp:
        return "pan-up"sv;
    case TouchAction::PanX:
        return "pan-x"sv;
    case TouchAction::PanY:
        return "pan-y"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TransformBox> keyword_to_transform_box(Keyword keyword)
{
    switch (keyword) {
    case Keyword::ContentBox:
        return TransformBox::ContentBox;
    case Keyword::BorderBox:
        return TransformBox::BorderBox;
    case Keyword::FillBox:
        return TransformBox::FillBox;
    case Keyword::StrokeBox:
        return TransformBox::StrokeBox;
    case Keyword::ViewBox:
        return TransformBox::ViewBox;
    default:
        return {};
    }
}

Keyword to_keyword(TransformBox transform_box_value)
{
    switch (transform_box_value) {
    case TransformBox::ContentBox:
        return Keyword::ContentBox;
    case TransformBox::BorderBox:
        return Keyword::BorderBox;
    case TransformBox::FillBox:
        return Keyword::FillBox;
    case TransformBox::StrokeBox:
        return Keyword::StrokeBox;
    case TransformBox::ViewBox:
        return Keyword::ViewBox;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TransformBox value)
{
    switch (value) {
    case TransformBox::ContentBox:
        return "content-box"sv;
    case TransformBox::BorderBox:
        return "border-box"sv;
    case TransformBox::FillBox:
        return "fill-box"sv;
    case TransformBox::StrokeBox:
        return "stroke-box"sv;
    case TransformBox::ViewBox:
        return "view-box"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TransformStyle> keyword_to_transform_style(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Flat:
        return TransformStyle::Flat;
    case Keyword::Preserve3d:
        return TransformStyle::Preserve3d;
    default:
        return {};
    }
}

Keyword to_keyword(TransformStyle transform_style_value)
{
    switch (transform_style_value) {
    case TransformStyle::Flat:
        return Keyword::Flat;
    case TransformStyle::Preserve3d:
        return Keyword::Preserve3d;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TransformStyle value)
{
    switch (value) {
    case TransformStyle::Flat:
        return "flat"sv;
    case TransformStyle::Preserve3d:
        return "preserve-3d"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TransitionBehavior> keyword_to_transition_behavior(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return TransitionBehavior::Normal;
    case Keyword::AllowDiscrete:
        return TransitionBehavior::AllowDiscrete;
    default:
        return {};
    }
}

Keyword to_keyword(TransitionBehavior transition_behavior_value)
{
    switch (transition_behavior_value) {
    case TransitionBehavior::Normal:
        return Keyword::Normal;
    case TransitionBehavior::AllowDiscrete:
        return Keyword::AllowDiscrete;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TransitionBehavior value)
{
    switch (value) {
    case TransitionBehavior::Normal:
        return "normal"sv;
    case TransitionBehavior::AllowDiscrete:
        return "allow-discrete"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TryOrder> keyword_to_try_order(Keyword keyword)
{
    switch (keyword) {
    case Keyword::MostWidth:
        return TryOrder::MostWidth;
    case Keyword::MostHeight:
        return TryOrder::MostHeight;
    case Keyword::MostBlockSize:
        return TryOrder::MostBlockSize;
    case Keyword::MostInlineSize:
        return TryOrder::MostInlineSize;
    default:
        return {};
    }
}

Keyword to_keyword(TryOrder try_order_value)
{
    switch (try_order_value) {
    case TryOrder::MostWidth:
        return Keyword::MostWidth;
    case TryOrder::MostHeight:
        return Keyword::MostHeight;
    case TryOrder::MostBlockSize:
        return Keyword::MostBlockSize;
    case TryOrder::MostInlineSize:
        return Keyword::MostInlineSize;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TryOrder value)
{
    switch (value) {
    case TryOrder::MostWidth:
        return "most-width"sv;
    case TryOrder::MostHeight:
        return "most-height"sv;
    case TryOrder::MostBlockSize:
        return "most-block-size"sv;
    case TryOrder::MostInlineSize:
        return "most-inline-size"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<TryTactic> keyword_to_try_tactic(Keyword keyword)
{
    switch (keyword) {
    case Keyword::FlipBlock:
        return TryTactic::FlipBlock;
    case Keyword::FlipInline:
        return TryTactic::FlipInline;
    case Keyword::FlipStart:
        return TryTactic::FlipStart;
    default:
        return {};
    }
}

Keyword to_keyword(TryTactic try_tactic_value)
{
    switch (try_tactic_value) {
    case TryTactic::FlipBlock:
        return Keyword::FlipBlock;
    case TryTactic::FlipInline:
        return Keyword::FlipInline;
    case TryTactic::FlipStart:
        return Keyword::FlipStart;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(TryTactic value)
{
    switch (value) {
    case TryTactic::FlipBlock:
        return "flip-block"sv;
    case TryTactic::FlipInline:
        return "flip-inline"sv;
    case TryTactic::FlipStart:
        return "flip-start"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<UnicodeBidi> keyword_to_unicode_bidi(Keyword keyword)
{
    switch (keyword) {
    case Keyword::BidiOverride:
        return UnicodeBidi::BidiOverride;
    case Keyword::Embed:
        return UnicodeBidi::Embed;
    case Keyword::Isolate:
        return UnicodeBidi::Isolate;
    case Keyword::IsolateOverride:
        return UnicodeBidi::IsolateOverride;
    case Keyword::Normal:
        return UnicodeBidi::Normal;
    case Keyword::Plaintext:
        return UnicodeBidi::Plaintext;
    default:
        return {};
    }
}

Keyword to_keyword(UnicodeBidi unicode_bidi_value)
{
    switch (unicode_bidi_value) {
    case UnicodeBidi::BidiOverride:
        return Keyword::BidiOverride;
    case UnicodeBidi::Embed:
        return Keyword::Embed;
    case UnicodeBidi::Isolate:
        return Keyword::Isolate;
    case UnicodeBidi::IsolateOverride:
        return Keyword::IsolateOverride;
    case UnicodeBidi::Normal:
        return Keyword::Normal;
    case UnicodeBidi::Plaintext:
        return Keyword::Plaintext;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(UnicodeBidi value)
{
    switch (value) {
    case UnicodeBidi::BidiOverride:
        return "bidi-override"sv;
    case UnicodeBidi::Embed:
        return "embed"sv;
    case UnicodeBidi::Isolate:
        return "isolate"sv;
    case UnicodeBidi::IsolateOverride:
        return "isolate-override"sv;
    case UnicodeBidi::Normal:
        return "normal"sv;
    case UnicodeBidi::Plaintext:
        return "plaintext"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<UserSelect> keyword_to_user_select(Keyword keyword)
{
    switch (keyword) {
    case Keyword::All:
        return UserSelect::All;
    case Keyword::Auto:
        return UserSelect::Auto;
    case Keyword::Contain:
        return UserSelect::Contain;
    case Keyword::None:
        return UserSelect::None;
    case Keyword::Text:
        return UserSelect::Text;
    default:
        return {};
    }
}

Keyword to_keyword(UserSelect user_select_value)
{
    switch (user_select_value) {
    case UserSelect::All:
        return Keyword::All;
    case UserSelect::Auto:
        return Keyword::Auto;
    case UserSelect::Contain:
        return Keyword::Contain;
    case UserSelect::None:
        return Keyword::None;
    case UserSelect::Text:
        return Keyword::Text;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(UserSelect value)
{
    switch (value) {
    case UserSelect::All:
        return "all"sv;
    case UserSelect::Auto:
        return "auto"sv;
    case UserSelect::Contain:
        return "contain"sv;
    case UserSelect::None:
        return "none"sv;
    case UserSelect::Text:
        return "text"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<VerticalAlign> keyword_to_vertical_align(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Baseline:
        return VerticalAlign::Baseline;
    case Keyword::Bottom:
        return VerticalAlign::Bottom;
    case Keyword::Middle:
        return VerticalAlign::Middle;
    case Keyword::Sub:
        return VerticalAlign::Sub;
    case Keyword::Super:
        return VerticalAlign::Super;
    case Keyword::TextBottom:
        return VerticalAlign::TextBottom;
    case Keyword::TextTop:
        return VerticalAlign::TextTop;
    case Keyword::Top:
        return VerticalAlign::Top;
    default:
        return {};
    }
}

Keyword to_keyword(VerticalAlign vertical_align_value)
{
    switch (vertical_align_value) {
    case VerticalAlign::Baseline:
        return Keyword::Baseline;
    case VerticalAlign::Bottom:
        return Keyword::Bottom;
    case VerticalAlign::Middle:
        return Keyword::Middle;
    case VerticalAlign::Sub:
        return Keyword::Sub;
    case VerticalAlign::Super:
        return Keyword::Super;
    case VerticalAlign::TextBottom:
        return Keyword::TextBottom;
    case VerticalAlign::TextTop:
        return Keyword::TextTop;
    case VerticalAlign::Top:
        return Keyword::Top;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(VerticalAlign value)
{
    switch (value) {
    case VerticalAlign::Baseline:
        return "baseline"sv;
    case VerticalAlign::Bottom:
        return "bottom"sv;
    case VerticalAlign::Middle:
        return "middle"sv;
    case VerticalAlign::Sub:
        return "sub"sv;
    case VerticalAlign::Super:
        return "super"sv;
    case VerticalAlign::TextBottom:
        return "text-bottom"sv;
    case VerticalAlign::TextTop:
        return "text-top"sv;
    case VerticalAlign::Top:
        return "top"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<Visibility> keyword_to_visibility(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Collapse:
        return Visibility::Collapse;
    case Keyword::Hidden:
        return Visibility::Hidden;
    case Keyword::Visible:
        return Visibility::Visible;
    default:
        return {};
    }
}

Keyword to_keyword(Visibility visibility_value)
{
    switch (visibility_value) {
    case Visibility::Collapse:
        return Keyword::Collapse;
    case Visibility::Hidden:
        return Keyword::Hidden;
    case Visibility::Visible:
        return Keyword::Visible;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(Visibility value)
{
    switch (value) {
    case Visibility::Collapse:
        return "collapse"sv;
    case Visibility::Hidden:
        return "hidden"sv;
    case Visibility::Visible:
        return "visible"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<WhiteSpace> keyword_to_white_space(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return WhiteSpace::Normal;
    case Keyword::Pre:
        return WhiteSpace::Pre;
    case Keyword::PreLine:
        return WhiteSpace::PreLine;
    case Keyword::PreWrap:
        return WhiteSpace::PreWrap;
    default:
        return {};
    }
}

Keyword to_keyword(WhiteSpace white_space_value)
{
    switch (white_space_value) {
    case WhiteSpace::Normal:
        return Keyword::Normal;
    case WhiteSpace::Pre:
        return Keyword::Pre;
    case WhiteSpace::PreLine:
        return Keyword::PreLine;
    case WhiteSpace::PreWrap:
        return Keyword::PreWrap;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(WhiteSpace value)
{
    switch (value) {
    case WhiteSpace::Normal:
        return "normal"sv;
    case WhiteSpace::Pre:
        return "pre"sv;
    case WhiteSpace::PreLine:
        return "pre-line"sv;
    case WhiteSpace::PreWrap:
        return "pre-wrap"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<WhiteSpaceCollapse> keyword_to_white_space_collapse(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Collapse:
        return WhiteSpaceCollapse::Collapse;
    case Keyword::Discard:
        return WhiteSpaceCollapse::Discard;
    case Keyword::Preserve:
        return WhiteSpaceCollapse::Preserve;
    case Keyword::PreserveBreaks:
        return WhiteSpaceCollapse::PreserveBreaks;
    case Keyword::PreserveSpaces:
        return WhiteSpaceCollapse::PreserveSpaces;
    case Keyword::BreakSpaces:
        return WhiteSpaceCollapse::BreakSpaces;
    default:
        return {};
    }
}

Keyword to_keyword(WhiteSpaceCollapse white_space_collapse_value)
{
    switch (white_space_collapse_value) {
    case WhiteSpaceCollapse::Collapse:
        return Keyword::Collapse;
    case WhiteSpaceCollapse::Discard:
        return Keyword::Discard;
    case WhiteSpaceCollapse::Preserve:
        return Keyword::Preserve;
    case WhiteSpaceCollapse::PreserveBreaks:
        return Keyword::PreserveBreaks;
    case WhiteSpaceCollapse::PreserveSpaces:
        return Keyword::PreserveSpaces;
    case WhiteSpaceCollapse::BreakSpaces:
        return Keyword::BreakSpaces;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(WhiteSpaceCollapse value)
{
    switch (value) {
    case WhiteSpaceCollapse::Collapse:
        return "collapse"sv;
    case WhiteSpaceCollapse::Discard:
        return "discard"sv;
    case WhiteSpaceCollapse::Preserve:
        return "preserve"sv;
    case WhiteSpaceCollapse::PreserveBreaks:
        return "preserve-breaks"sv;
    case WhiteSpaceCollapse::PreserveSpaces:
        return "preserve-spaces"sv;
    case WhiteSpaceCollapse::BreakSpaces:
        return "break-spaces"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<WordBreak> keyword_to_word_break(Keyword keyword)
{
    switch (keyword) {
    case Keyword::Normal:
        return WordBreak::Normal;
    case Keyword::KeepAll:
        return WordBreak::KeepAll;
    case Keyword::BreakAll:
        return WordBreak::BreakAll;
    case Keyword::BreakWord:
        return WordBreak::BreakWord;
    default:
        return {};
    }
}

Keyword to_keyword(WordBreak word_break_value)
{
    switch (word_break_value) {
    case WordBreak::Normal:
        return Keyword::Normal;
    case WordBreak::KeepAll:
        return Keyword::KeepAll;
    case WordBreak::BreakAll:
        return Keyword::BreakAll;
    case WordBreak::BreakWord:
        return Keyword::BreakWord;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(WordBreak value)
{
    switch (value) {
    case WordBreak::Normal:
        return "normal"sv;
    case WordBreak::KeepAll:
        return "keep-all"sv;
    case WordBreak::BreakAll:
        return "break-all"sv;
    case WordBreak::BreakWord:
        return "break-word"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}

Optional<WritingMode> keyword_to_writing_mode(Keyword keyword)
{
    switch (keyword) {
    case Keyword::HorizontalTb:
        return WritingMode::HorizontalTb;
    case Keyword::VerticalRl:
        return WritingMode::VerticalRl;
    case Keyword::VerticalLr:
        return WritingMode::VerticalLr;
    case Keyword::SidewaysRl:
        return WritingMode::SidewaysRl;
    case Keyword::SidewaysLr:
        return WritingMode::SidewaysLr;
    default:
        return {};
    }
}

Keyword to_keyword(WritingMode writing_mode_value)
{
    switch (writing_mode_value) {
    case WritingMode::HorizontalTb:
        return Keyword::HorizontalTb;
    case WritingMode::VerticalRl:
        return Keyword::VerticalRl;
    case WritingMode::VerticalLr:
        return Keyword::VerticalLr;
    case WritingMode::SidewaysRl:
        return Keyword::SidewaysRl;
    case WritingMode::SidewaysLr:
        return Keyword::SidewaysLr;
    default:
        VERIFY_NOT_REACHED();
    }
}

StringView to_string(WritingMode value)
{
    switch (value) {
    case WritingMode::HorizontalTb:
        return "horizontal-tb"sv;
    case WritingMode::VerticalRl:
        return "vertical-rl"sv;
    case WritingMode::VerticalLr:
        return "vertical-lr"sv;
    case WritingMode::SidewaysRl:
        return "sideways-rl"sv;
    case WritingMode::SidewaysLr:
        return "sideways-lr"sv;
    default:
        VERIFY_NOT_REACHED();
    }
}
}
