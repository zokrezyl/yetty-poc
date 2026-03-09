#pragma once

#include <AK/EnumBits.h>
#include <LibGC/Weak.h>
#include <LibWeb/Forward.h>

namespace Web::HTML {

struct MediaControlsDOM {
    enum class Options : u8 {
        None = 0,
        Video = 1 << 0,
    };

    MediaControlsDOM(DOM::Document&, DOM::Node& parent, Options = Options::None);

    GC::Weak<DOM::Element> container;
    GC::Weak<DOM::Element> video_overlay;
    GC::Weak<DOM::Element> placeholder_circle;
    GC::Weak<DOM::Element> control_bar;
    GC::Weak<DOM::Element> timeline_element;
    GC::Weak<DOM::Element> timeline_fill;
    GC::Weak<DOM::Element> play_button;
    GC::Weak<DOM::Element> play_pause_icon;
    GC::Weak<DOM::Element> timestamp_element;
    GC::Weak<DOM::Element> mute_button;
    GC::Weak<DOM::Element> volume_area;
    GC::Weak<DOM::Element> volume_element;
    GC::Weak<DOM::Element> volume_fill;
    GC::Weak<DOM::Element> fullscreen_button;
    GC::Weak<DOM::Element> fullscreen_icon;
};

AK_ENUM_BITWISE_OPERATORS(MediaControlsDOM::Options);

} // namespace Web::HTML
