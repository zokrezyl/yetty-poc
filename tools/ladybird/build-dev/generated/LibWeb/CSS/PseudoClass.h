
#pragma once

#include <AK/Optional.h>
#include <AK/StringView.h>

namespace Web::CSS {

enum class PseudoClass {
    Active,
    AnyLink,
    Autofill,
    Buffering,
    Checked,
    Default,
    Defined,
    Dir,
    Disabled,
    Empty,
    Enabled,
    EvenLessGoodValue,
    FirstChild,
    FirstOfType,
    Focus,
    FocusVisible,
    FocusWithin,
    Fullscreen,
    Has,
    Heading,
    HighValue,
    Host,
    Hover,
    Indeterminate,
    Invalid,
    Is,
    Lang,
    LastChild,
    LastOfType,
    Link,
    LocalLink,
    LowValue,
    Modal,
    Muted,
    Not,
    NthChild,
    NthLastChild,
    NthLastOfType,
    NthOfType,
    OnlyChild,
    OnlyOfType,
    Open,
    OptimalValue,
    Optional,
    PopoverOpen,
    Paused,
    PlaceholderShown,
    Playing,
    ReadOnly,
    ReadWrite,
    Required,
    Root,
    Scope,
    Seeking,
    Stalled,
    State,
    SuboptimalValue,
    Target,
    Unchecked,
    UserInvalid,
    UserValid,
    Valid,
    Visited,
    VolumeLocked,
    Where,

    __Count,
};

Optional<PseudoClass> pseudo_class_from_string(StringView);
StringView pseudo_class_name(PseudoClass);

struct PseudoClassMetadata {
    enum class ParameterType {
        None,
        ANPlusB,
        ANPlusBOf,
        CompoundSelector,
        ForgivingSelectorList,
        ForgivingRelativeSelectorList,
        Ident,
        LanguageRanges,
        LevelList,
        RelativeSelectorList,
        SelectorList,
    } parameter_type;
    bool is_valid_as_function;
    bool is_valid_as_identifier;
};
PseudoClassMetadata pseudo_class_metadata(PseudoClass);

}
