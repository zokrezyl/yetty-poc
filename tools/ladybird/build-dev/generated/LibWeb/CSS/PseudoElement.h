
#pragma once

#include <AK/Optional.h>
#include <AK/StringView.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Export.h>

namespace Web::CSS {

enum class PseudoElement : u8 {
    After,
    Backdrop,
    Before,
    DetailsContent,
    FileSelectorButton,
    FirstLetter,
    FirstLine,
    Marker,
    Part,
    Placeholder,
    Selection,
    SliderFill,
    SliderThumb,
    SliderTrack,
    Slotted,
    ViewTransition,
    ViewTransitionGroup,
    ViewTransitionImagePair,
    ViewTransitionNew,
    ViewTransitionOld,

    KnownPseudoElementCount,

    UnknownWebKit,
};

Optional<PseudoElement> pseudo_element_from_string(StringView);
Optional<PseudoElement> aliased_pseudo_element_from_string(StringView);
WEB_API StringView pseudo_element_name(PseudoElement);

bool is_has_allowed_pseudo_element(PseudoElement);
bool is_pseudo_element_root(PseudoElement);
bool pseudo_element_supports_property(PseudoElement, PropertyID);

struct PseudoElementMetadata {
    enum class ParameterType {
        None,
        CompoundSelector,
        IdentList,
        PTNameSelector,
    } parameter_type;
    bool is_valid_as_function;
    bool is_valid_as_identifier;
};
PseudoElementMetadata pseudo_element_metadata(PseudoElement);

}
