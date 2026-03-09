#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/ElementFactory.h>
#include <LibWeb/HTML/AttributeNames.h>
#include <LibWeb/HTML/TagNames.h>
#include <LibWeb/Namespace.h>
#include <LibWeb/SVG/AttributeNames.h>
#include <LibWeb/SVG/TagNames.h>

#include "MediaControlsDOM.h"

namespace Web::HTML {

static String s_stylesheet_source_1 = R"~~~(
.container {
    --accent-color: #9d7cf2;
    --dark-accent-color: #8a64e5;
    --light-accent-color: #e0d4ff;
    --foreground-color: white;
    --track-color: rgb(20, 20, 20);
    --panel-color: rgb(38, 38, 38);

    overflow: hidden;
}

.container.video {
    position: relative;
    width: 100%;
    height: 100%;
}

.controls {
    display: flex;
    flex-direction: column;
    font-family: sans-serif;
    font-size: 12px;
    color: var(--foreground-color);
    user-select: none;
}

.container.video > .controls {
    position: absolute;
    bottom: 0;
    left: 0;
    right: 0;
    background: color-mix(in srgb, var(--panel-color) 82%, transparent);
    opacity: 0;
    pointer-events: none;
    transition: opacity 150ms ease-in-out;
}

.container.video > .controls.visible {
    opacity: 1;
    pointer-events: auto;
}

.container.audio > .controls {
    inset: 0;
    background: var(--panel-color);
    justify-content: center;
}

.timeline {
    z-index: 1;
    height: 5px;
    padding: 5px 0;
    margin: -5px 0;
    background: var(--track-color);
    background-clip: content-box;
    cursor: pointer;
    flex-shrink: 0;
}

.timeline-fill {
    height: 100%;
    background: var(--accent-color);
    width: 0%;
    pointer-events: none;
}

.button-bar {
    display: flex;
    align-items: center;
    padding: 4px 8px;
    gap: 8px;
    min-height: 30px;
}

.control-button {
    background: none;
    border: none;
    cursor: pointer;
    padding: 0;
    display: flex;
    align-items: center;
    justify-content: center;
    width: 24px;
    height: 24px;
}

.icon {
    width: 24px;
    height: 24px;
}

.control-button .icon path {
    fill: var(--foreground-color);
}

.control-button:hover .icon path {
    fill: var(--light-accent-color);
}

.play-pause-icon path {
    display: none;
}
.play-pause-icon:not(.playing) .play-path {
    display: inline;
}
.play-pause-icon.playing .pause-path {
    display: inline;
}

.mute-button {
    margin-left: auto;
}

.mute-button:not(.low, .high) > .icon > .volume-low {
    display: none;
}
.mute-button:not(.high) > .icon > .volume-high {
    display: none;
}

.mute-button.muted > .icon > .volume-low,
.mute-button.muted > .icon > .volume-high,
.mute-button.muted > .icon > .speaker {
    clip-path: url(#muted-clip);
}
.mute-button:not(.muted) > .icon > .muted-line {
    display: none;
}

.timestamp {
    min-width: 0;
    overflow: clip;
    white-space: nowrap;
    font-variant-numeric: tabular-nums;
}

.volume-area {
    max-width: 60px;
    flex-grow: 1;
    padding: 5px;
    margin: -5px;
    cursor: pointer;
}

.volume-area.hidden {
    display: none;
}

.volume {
    height: 6px;
    background: var(--track-color);
    border-radius: 3px;
    overflow: hidden;
    position: relative;
}

.volume-fill {
    height: 100%;
    width: 100%;
    background: var(--accent-color);
    pointer-events: none;
}

.video-overlay {
    position: absolute;
    inset: 0;
    display: flex;
    align-items: center;
    justify-content: center;
}

.fullscreen-icon path {
    display: none;
}
.fullscreen-icon:not(.fullscreen) .fullscreen-maximize-path {
    display: inline;
}
.fullscreen-icon.fullscreen .fullscreen-minimize-path {
    display: inline;
}

.placeholder-circle {
    width: 80px;
    height: 80px;
    border-radius: 50%;
    background: var(--panel-color);
    opacity: 80%;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
}

.placeholder-circle:hover {
    background: var(--panel-color);
    opacity: 100%;
}

.placeholder-circle path {
    fill: var(--light-accent-color);
}

.placeholder-icon {
    width: 36px;
    height: 36px;
    margin-left: 4px;
}
)~~~"_string;

MediaControlsDOM::MediaControlsDOM(DOM::Document& document, DOM::Node& parent, Options options)
{
    auto element_1 = MUST(DOM::create_element(document, HTML::TagNames::style, Namespace::HTML));
    MUST(element_1->set_text_content(Utf16String::from_utf8(s_stylesheet_source_1)));
    MUST(parent.append_child(element_1));

    auto container = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->container = container;
    container->set_attribute_value(HTML::AttributeNames::class_, "container"_string);
    MUST(parent.append_child(container));

    if (has_flag(options, MediaControlsDOM::Options::Video)) {
        auto video_overlay = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
        this->video_overlay = video_overlay;
        video_overlay->set_attribute_value(HTML::AttributeNames::class_, "video-overlay"_string);
        MUST(container->append_child(video_overlay));

        auto placeholder_circle = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
        this->placeholder_circle = placeholder_circle;
        placeholder_circle->set_attribute_value(HTML::AttributeNames::class_, "placeholder-circle"_string);
        MUST(video_overlay->append_child(placeholder_circle));

        auto element_2 = MUST(DOM::create_element(document, SVG::TagNames::svg, Namespace::SVG));
        element_2->set_attribute_value(SVG::AttributeNames::class_, "placeholder-icon"_string);
        element_2->set_attribute_value(SVG::AttributeNames::viewBox, "0 0 24 24"_string);
        MUST(placeholder_circle->append_child(element_2));

        auto element_3 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
        element_3->set_attribute_value(SVG::AttributeNames::class_, "play-path"_string);
        element_3->set_attribute_value(SVG::AttributeNames::d, "m6 5 13 7-13 7Z"_string);
        MUST(element_2->append_child(element_3));
    } // Options::Video

    auto control_bar = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->control_bar = control_bar;
    control_bar->set_attribute_value(HTML::AttributeNames::class_, "controls"_string);
    MUST(container->append_child(control_bar));

    auto timeline_element = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->timeline_element = timeline_element;
    timeline_element->set_attribute_value(HTML::AttributeNames::class_, "timeline"_string);
    MUST(control_bar->append_child(timeline_element));

    auto timeline_fill = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->timeline_fill = timeline_fill;
    timeline_fill->set_attribute_value(HTML::AttributeNames::class_, "timeline-fill"_string);
    MUST(timeline_element->append_child(timeline_fill));

    auto element_4 = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    element_4->set_attribute_value(HTML::AttributeNames::class_, "button-bar"_string);
    MUST(control_bar->append_child(element_4));

    auto play_button = MUST(DOM::create_element(document, HTML::TagNames::button, Namespace::HTML));
    this->play_button = play_button;
    play_button->set_attribute_value(HTML::AttributeNames::class_, "control-button play-pause-button"_string);
    MUST(element_4->append_child(play_button));

    auto play_pause_icon = MUST(DOM::create_element(document, SVG::TagNames::svg, Namespace::SVG));
    this->play_pause_icon = play_pause_icon;
    play_pause_icon->set_attribute_value(SVG::AttributeNames::class_, "icon play-pause-icon"_string);
    play_pause_icon->set_attribute_value(SVG::AttributeNames::viewBox, "0 0 24 24"_string);
    MUST(play_button->append_child(play_pause_icon));

    auto element_5 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_5->set_attribute_value(SVG::AttributeNames::class_, "play-path"_string);
    element_5->set_attribute_value(SVG::AttributeNames::d, "m6 5 13 7-13 7Z"_string);
    MUST(play_pause_icon->append_child(element_5));

    auto element_6 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_6->set_attribute_value(SVG::AttributeNames::class_, "pause-path"_string);
    element_6->set_attribute_value(SVG::AttributeNames::d, "M14 5h4v14h-4Zm-4 0H6v14h4z"_string);
    MUST(play_pause_icon->append_child(element_6));

    auto timestamp_element = MUST(DOM::create_element(document, HTML::TagNames::span, Namespace::HTML));
    this->timestamp_element = timestamp_element;
    timestamp_element->set_attribute_value(HTML::AttributeNames::class_, "timestamp"_string);
    MUST(element_4->append_child(timestamp_element));

    MUST(timestamp_element->set_text_content(R"~~~(0:00 / 0:00)~~~"_utf16));

    auto mute_button = MUST(DOM::create_element(document, HTML::TagNames::button, Namespace::HTML));
    this->mute_button = mute_button;
    mute_button->set_attribute_value(HTML::AttributeNames::class_, "control-button mute-button"_string);
    MUST(element_4->append_child(mute_button));

    auto element_7 = MUST(DOM::create_element(document, SVG::TagNames::svg, Namespace::SVG));
    element_7->set_attribute_value(SVG::AttributeNames::class_, "icon"_string);
    element_7->set_attribute_value(SVG::AttributeNames::viewBox, "0 0 24 24"_string);
    MUST(mute_button->append_child(element_7));

    auto element_8 = MUST(DOM::create_element(document, SVG::TagNames::defs, Namespace::SVG));
    MUST(element_7->append_child(element_8));

    auto element_9 = MUST(DOM::create_element(document, SVG::TagNames::clipPath, Namespace::SVG));
    element_9->set_attribute_value(HTML::AttributeNames::id, "muted-clip"_string);
    MUST(element_8->append_child(element_9));

    auto element_10 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_10->set_attribute_value(SVG::AttributeNames::d, "M3 0h21v21ZM0 0v24h24z"_string);
    MUST(element_9->append_child(element_10));

    auto element_11 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_11->set_attribute_value(SVG::AttributeNames::class_, "muted-line"_string);
    element_11->set_attribute_value(SVG::AttributeNames::d, "m5 5 14 14-1.5 1.5-14-14z"_string);
    MUST(element_7->append_child(element_11));

    auto element_12 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_12->set_attribute_value(SVG::AttributeNames::class_, "volume-high"_string);
    element_12->set_attribute_value(SVG::AttributeNames::d, "M14 4.08v2.04c2.23.55 4 2.9 4 5.88 0 2.97-1.77 5.33-4 5.88v2.04c3.45-.56 6-3.96 6-7.92s-2.55-7.36-6-7.92Z"_string);
    MUST(element_7->append_child(element_12));

    auto element_13 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_13->set_attribute_value(SVG::AttributeNames::class_, "volume-low"_string);
    element_13->set_attribute_value(SVG::AttributeNames::d, "M14 7.67v8.66c.35-.25.66-.55.92-.9A5.7 5.7 0 0 0 16 12c0-1.3-.39-2.5-1.08-3.43a4.24 4.24 0 0 0-.92-.9Z"_string);
    MUST(element_7->append_child(element_13));

    auto element_14 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
    element_14->set_attribute_value(SVG::AttributeNames::class_, "speaker"_string);
    element_14->set_attribute_value(SVG::AttributeNames::d, "M4 9v6h4l4 5V4L8 9Z"_string);
    MUST(element_7->append_child(element_14));

    auto volume_area = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->volume_area = volume_area;
    volume_area->set_attribute_value(HTML::AttributeNames::class_, "volume-area"_string);
    MUST(element_4->append_child(volume_area));

    auto volume_element = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->volume_element = volume_element;
    volume_element->set_attribute_value(HTML::AttributeNames::class_, "volume"_string);
    MUST(volume_area->append_child(volume_element));

    auto volume_fill = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
    this->volume_fill = volume_fill;
    volume_fill->set_attribute_value(HTML::AttributeNames::class_, "volume-fill"_string);
    MUST(volume_element->append_child(volume_fill));

    if (has_flag(options, MediaControlsDOM::Options::Video)) {
        auto fullscreen_button = MUST(DOM::create_element(document, HTML::TagNames::div, Namespace::HTML));
        this->fullscreen_button = fullscreen_button;
        fullscreen_button->set_attribute_value(HTML::AttributeNames::class_, "control-button fullscreen-button"_string);
        MUST(element_4->append_child(fullscreen_button));

        auto fullscreen_icon = MUST(DOM::create_element(document, SVG::TagNames::svg, Namespace::SVG));
        this->fullscreen_icon = fullscreen_icon;
        fullscreen_icon->set_attribute_value(SVG::AttributeNames::class_, "icon fullscreen-icon"_string);
        fullscreen_icon->set_attribute_value(SVG::AttributeNames::viewBox, "0 0 24 24"_string);
        MUST(fullscreen_button->append_child(fullscreen_icon));

        auto element_15 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
        element_15->set_attribute_value(SVG::AttributeNames::class_, "fullscreen-maximize-path"_string);
        element_15->set_attribute_value(SVG::AttributeNames::d, "M3 3h6v2H5v4H3Zm12 0h6v6h-2V5h-4Zm6 12v6h-6v-2h4v-4ZM3 15v6h6v-2H5v-4Z"_string);
        MUST(fullscreen_icon->append_child(element_15));

        auto element_16 = MUST(DOM::create_element(document, SVG::TagNames::path, Namespace::SVG));
        element_16->set_attribute_value(SVG::AttributeNames::class_, "fullscreen-minimize-path"_string);
        element_16->set_attribute_value(SVG::AttributeNames::d, "M9 3v6H3V7h4V3Zm6 0v6h6V7h-4V3ZM3 15h6v6H7v-4H3Zm12 0v6h2v-4h4v-2Z"_string);
        MUST(fullscreen_icon->append_child(element_16));
    } // Options::Video
}

} // namespace Web::HTML
