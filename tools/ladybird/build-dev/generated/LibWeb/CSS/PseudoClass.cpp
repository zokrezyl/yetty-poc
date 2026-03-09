
#include <LibWeb/CSS/PseudoClass.h>

namespace Web::CSS {

Optional<PseudoClass> pseudo_class_from_string(StringView string)
{

    if (string.equals_ignoring_ascii_case("-webkit-autofill"sv))
        return PseudoClass::Autofill;

    if (string.equals_ignoring_ascii_case("active"sv))
        return PseudoClass::Active;

    if (string.equals_ignoring_ascii_case("any-link"sv))
        return PseudoClass::AnyLink;

    if (string.equals_ignoring_ascii_case("autofill"sv))
        return PseudoClass::Autofill;

    if (string.equals_ignoring_ascii_case("buffering"sv))
        return PseudoClass::Buffering;

    if (string.equals_ignoring_ascii_case("checked"sv))
        return PseudoClass::Checked;

    if (string.equals_ignoring_ascii_case("default"sv))
        return PseudoClass::Default;

    if (string.equals_ignoring_ascii_case("defined"sv))
        return PseudoClass::Defined;

    if (string.equals_ignoring_ascii_case("dir"sv))
        return PseudoClass::Dir;

    if (string.equals_ignoring_ascii_case("disabled"sv))
        return PseudoClass::Disabled;

    if (string.equals_ignoring_ascii_case("empty"sv))
        return PseudoClass::Empty;

    if (string.equals_ignoring_ascii_case("enabled"sv))
        return PseudoClass::Enabled;

    if (string.equals_ignoring_ascii_case("even-less-good-value"sv))
        return PseudoClass::EvenLessGoodValue;

    if (string.equals_ignoring_ascii_case("first-child"sv))
        return PseudoClass::FirstChild;

    if (string.equals_ignoring_ascii_case("first-of-type"sv))
        return PseudoClass::FirstOfType;

    if (string.equals_ignoring_ascii_case("focus"sv))
        return PseudoClass::Focus;

    if (string.equals_ignoring_ascii_case("focus-visible"sv))
        return PseudoClass::FocusVisible;

    if (string.equals_ignoring_ascii_case("focus-within"sv))
        return PseudoClass::FocusWithin;

    if (string.equals_ignoring_ascii_case("fullscreen"sv))
        return PseudoClass::Fullscreen;

    if (string.equals_ignoring_ascii_case("has"sv))
        return PseudoClass::Has;

    if (string.equals_ignoring_ascii_case("heading"sv))
        return PseudoClass::Heading;

    if (string.equals_ignoring_ascii_case("high-value"sv))
        return PseudoClass::HighValue;

    if (string.equals_ignoring_ascii_case("host"sv))
        return PseudoClass::Host;

    if (string.equals_ignoring_ascii_case("hover"sv))
        return PseudoClass::Hover;

    if (string.equals_ignoring_ascii_case("indeterminate"sv))
        return PseudoClass::Indeterminate;

    if (string.equals_ignoring_ascii_case("invalid"sv))
        return PseudoClass::Invalid;

    if (string.equals_ignoring_ascii_case("is"sv))
        return PseudoClass::Is;

    if (string.equals_ignoring_ascii_case("lang"sv))
        return PseudoClass::Lang;

    if (string.equals_ignoring_ascii_case("last-child"sv))
        return PseudoClass::LastChild;

    if (string.equals_ignoring_ascii_case("last-of-type"sv))
        return PseudoClass::LastOfType;

    if (string.equals_ignoring_ascii_case("link"sv))
        return PseudoClass::Link;

    if (string.equals_ignoring_ascii_case("local-link"sv))
        return PseudoClass::LocalLink;

    if (string.equals_ignoring_ascii_case("low-value"sv))
        return PseudoClass::LowValue;

    if (string.equals_ignoring_ascii_case("modal"sv))
        return PseudoClass::Modal;

    if (string.equals_ignoring_ascii_case("muted"sv))
        return PseudoClass::Muted;

    if (string.equals_ignoring_ascii_case("not"sv))
        return PseudoClass::Not;

    if (string.equals_ignoring_ascii_case("nth-child"sv))
        return PseudoClass::NthChild;

    if (string.equals_ignoring_ascii_case("nth-last-child"sv))
        return PseudoClass::NthLastChild;

    if (string.equals_ignoring_ascii_case("nth-last-of-type"sv))
        return PseudoClass::NthLastOfType;

    if (string.equals_ignoring_ascii_case("nth-of-type"sv))
        return PseudoClass::NthOfType;

    if (string.equals_ignoring_ascii_case("only-child"sv))
        return PseudoClass::OnlyChild;

    if (string.equals_ignoring_ascii_case("only-of-type"sv))
        return PseudoClass::OnlyOfType;

    if (string.equals_ignoring_ascii_case("open"sv))
        return PseudoClass::Open;

    if (string.equals_ignoring_ascii_case("optimal-value"sv))
        return PseudoClass::OptimalValue;

    if (string.equals_ignoring_ascii_case("optional"sv))
        return PseudoClass::Optional;

    if (string.equals_ignoring_ascii_case("popover-open"sv))
        return PseudoClass::PopoverOpen;

    if (string.equals_ignoring_ascii_case("paused"sv))
        return PseudoClass::Paused;

    if (string.equals_ignoring_ascii_case("placeholder-shown"sv))
        return PseudoClass::PlaceholderShown;

    if (string.equals_ignoring_ascii_case("playing"sv))
        return PseudoClass::Playing;

    if (string.equals_ignoring_ascii_case("read-only"sv))
        return PseudoClass::ReadOnly;

    if (string.equals_ignoring_ascii_case("read-write"sv))
        return PseudoClass::ReadWrite;

    if (string.equals_ignoring_ascii_case("required"sv))
        return PseudoClass::Required;

    if (string.equals_ignoring_ascii_case("root"sv))
        return PseudoClass::Root;

    if (string.equals_ignoring_ascii_case("scope"sv))
        return PseudoClass::Scope;

    if (string.equals_ignoring_ascii_case("seeking"sv))
        return PseudoClass::Seeking;

    if (string.equals_ignoring_ascii_case("stalled"sv))
        return PseudoClass::Stalled;

    if (string.equals_ignoring_ascii_case("state"sv))
        return PseudoClass::State;

    if (string.equals_ignoring_ascii_case("suboptimal-value"sv))
        return PseudoClass::SuboptimalValue;

    if (string.equals_ignoring_ascii_case("target"sv))
        return PseudoClass::Target;

    if (string.equals_ignoring_ascii_case("unchecked"sv))
        return PseudoClass::Unchecked;

    if (string.equals_ignoring_ascii_case("user-invalid"sv))
        return PseudoClass::UserInvalid;

    if (string.equals_ignoring_ascii_case("user-valid"sv))
        return PseudoClass::UserValid;

    if (string.equals_ignoring_ascii_case("valid"sv))
        return PseudoClass::Valid;

    if (string.equals_ignoring_ascii_case("visited"sv))
        return PseudoClass::Visited;

    if (string.equals_ignoring_ascii_case("volume-locked"sv))
        return PseudoClass::VolumeLocked;

    if (string.equals_ignoring_ascii_case("where"sv))
        return PseudoClass::Where;


    return {};
}

StringView pseudo_class_name(PseudoClass pseudo_class)
{
    switch (pseudo_class) {
    case PseudoClass::__Count:
        VERIFY_NOT_REACHED();

    case PseudoClass::Active:
        return "active"sv;

    case PseudoClass::AnyLink:
        return "any-link"sv;

    case PseudoClass::Autofill:
        return "autofill"sv;

    case PseudoClass::Buffering:
        return "buffering"sv;

    case PseudoClass::Checked:
        return "checked"sv;

    case PseudoClass::Default:
        return "default"sv;

    case PseudoClass::Defined:
        return "defined"sv;

    case PseudoClass::Dir:
        return "dir"sv;

    case PseudoClass::Disabled:
        return "disabled"sv;

    case PseudoClass::Empty:
        return "empty"sv;

    case PseudoClass::Enabled:
        return "enabled"sv;

    case PseudoClass::EvenLessGoodValue:
        return "even-less-good-value"sv;

    case PseudoClass::FirstChild:
        return "first-child"sv;

    case PseudoClass::FirstOfType:
        return "first-of-type"sv;

    case PseudoClass::Focus:
        return "focus"sv;

    case PseudoClass::FocusVisible:
        return "focus-visible"sv;

    case PseudoClass::FocusWithin:
        return "focus-within"sv;

    case PseudoClass::Fullscreen:
        return "fullscreen"sv;

    case PseudoClass::Has:
        return "has"sv;

    case PseudoClass::Heading:
        return "heading"sv;

    case PseudoClass::HighValue:
        return "high-value"sv;

    case PseudoClass::Host:
        return "host"sv;

    case PseudoClass::Hover:
        return "hover"sv;

    case PseudoClass::Indeterminate:
        return "indeterminate"sv;

    case PseudoClass::Invalid:
        return "invalid"sv;

    case PseudoClass::Is:
        return "is"sv;

    case PseudoClass::Lang:
        return "lang"sv;

    case PseudoClass::LastChild:
        return "last-child"sv;

    case PseudoClass::LastOfType:
        return "last-of-type"sv;

    case PseudoClass::Link:
        return "link"sv;

    case PseudoClass::LocalLink:
        return "local-link"sv;

    case PseudoClass::LowValue:
        return "low-value"sv;

    case PseudoClass::Modal:
        return "modal"sv;

    case PseudoClass::Muted:
        return "muted"sv;

    case PseudoClass::Not:
        return "not"sv;

    case PseudoClass::NthChild:
        return "nth-child"sv;

    case PseudoClass::NthLastChild:
        return "nth-last-child"sv;

    case PseudoClass::NthLastOfType:
        return "nth-last-of-type"sv;

    case PseudoClass::NthOfType:
        return "nth-of-type"sv;

    case PseudoClass::OnlyChild:
        return "only-child"sv;

    case PseudoClass::OnlyOfType:
        return "only-of-type"sv;

    case PseudoClass::Open:
        return "open"sv;

    case PseudoClass::OptimalValue:
        return "optimal-value"sv;

    case PseudoClass::Optional:
        return "optional"sv;

    case PseudoClass::PopoverOpen:
        return "popover-open"sv;

    case PseudoClass::Paused:
        return "paused"sv;

    case PseudoClass::PlaceholderShown:
        return "placeholder-shown"sv;

    case PseudoClass::Playing:
        return "playing"sv;

    case PseudoClass::ReadOnly:
        return "read-only"sv;

    case PseudoClass::ReadWrite:
        return "read-write"sv;

    case PseudoClass::Required:
        return "required"sv;

    case PseudoClass::Root:
        return "root"sv;

    case PseudoClass::Scope:
        return "scope"sv;

    case PseudoClass::Seeking:
        return "seeking"sv;

    case PseudoClass::Stalled:
        return "stalled"sv;

    case PseudoClass::State:
        return "state"sv;

    case PseudoClass::SuboptimalValue:
        return "suboptimal-value"sv;

    case PseudoClass::Target:
        return "target"sv;

    case PseudoClass::Unchecked:
        return "unchecked"sv;

    case PseudoClass::UserInvalid:
        return "user-invalid"sv;

    case PseudoClass::UserValid:
        return "user-valid"sv;

    case PseudoClass::Valid:
        return "valid"sv;

    case PseudoClass::Visited:
        return "visited"sv;

    case PseudoClass::VolumeLocked:
        return "volume-locked"sv;

    case PseudoClass::Where:
        return "where"sv;

    }
    VERIFY_NOT_REACHED();
}

PseudoClassMetadata pseudo_class_metadata(PseudoClass pseudo_class)
{
    switch (pseudo_class) {
    case PseudoClass::__Count:
        VERIFY_NOT_REACHED();

    case PseudoClass::Active:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::AnyLink:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Autofill:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Buffering:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Checked:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Default:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Defined:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Dir:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::Ident,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::Disabled:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Empty:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Enabled:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::EvenLessGoodValue:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::FirstChild:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::FirstOfType:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Focus:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::FocusVisible:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::FocusWithin:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Fullscreen:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Has:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::RelativeSelectorList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::Heading:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::LevelList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::HighValue:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Host:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::CompoundSelector,
            .is_valid_as_function = true,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Hover:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Indeterminate:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Invalid:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Is:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ForgivingSelectorList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::Lang:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::LanguageRanges,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::LastChild:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::LastOfType:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Link:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::LocalLink:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::LowValue:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Modal:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Muted:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Not:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::SelectorList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::NthChild:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ANPlusBOf,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::NthLastChild:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ANPlusBOf,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::NthLastOfType:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ANPlusB,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::NthOfType:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ANPlusB,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::OnlyChild:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::OnlyOfType:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Open:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::OptimalValue:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Optional:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::PopoverOpen:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Paused:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::PlaceholderShown:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Playing:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::ReadOnly:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::ReadWrite:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Required:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Root:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Scope:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Seeking:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Stalled:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::State:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::Ident,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    case PseudoClass::SuboptimalValue:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Target:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Unchecked:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::UserInvalid:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::UserValid:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Valid:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Visited:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::VolumeLocked:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::None,
            .is_valid_as_function = false,
            .is_valid_as_identifier = true,
        };

    case PseudoClass::Where:
        return {
            .parameter_type = PseudoClassMetadata::ParameterType::ForgivingSelectorList,
            .is_valid_as_function = true,
            .is_valid_as_identifier = false,
        };

    }
    VERIFY_NOT_REACHED();
}

}
