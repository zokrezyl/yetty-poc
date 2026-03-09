
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLMediaElementPrototype : public JS::Object {
    JS_OBJECT(HTMLMediaElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLMediaElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLMediaElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLMediaElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(load);
        
    JS_DECLARE_NATIVE_FUNCTION(can_play_type);
        
    JS_DECLARE_NATIVE_FUNCTION(fast_seek);
        
    JS_DECLARE_NATIVE_FUNCTION(play);
        
    JS_DECLARE_NATIVE_FUNCTION(pause);
        
    JS_DECLARE_NATIVE_FUNCTION(add_text_track);
        
    JS_DECLARE_NATIVE_FUNCTION(error_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_getter);

    JS_DECLARE_NATIVE_FUNCTION(src_setter);

    JS_DECLARE_NATIVE_FUNCTION(current_src_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(network_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(preload_getter);

    JS_DECLARE_NATIVE_FUNCTION(preload_setter);

    JS_DECLARE_NATIVE_FUNCTION(buffered_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(seeking_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_time_setter);

    JS_DECLARE_NATIVE_FUNCTION(duration_getter);

    JS_DECLARE_NATIVE_FUNCTION(paused_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_playback_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_playback_rate_setter);

    JS_DECLARE_NATIVE_FUNCTION(playback_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(playback_rate_setter);

    JS_DECLARE_NATIVE_FUNCTION(played_getter);

    JS_DECLARE_NATIVE_FUNCTION(seekable_getter);

    JS_DECLARE_NATIVE_FUNCTION(ended_getter);

    JS_DECLARE_NATIVE_FUNCTION(autoplay_getter);

    JS_DECLARE_NATIVE_FUNCTION(autoplay_setter);

    JS_DECLARE_NATIVE_FUNCTION(loop_getter);

    JS_DECLARE_NATIVE_FUNCTION(loop_setter);

    JS_DECLARE_NATIVE_FUNCTION(controls_getter);

    JS_DECLARE_NATIVE_FUNCTION(controls_setter);

    JS_DECLARE_NATIVE_FUNCTION(volume_getter);

    JS_DECLARE_NATIVE_FUNCTION(volume_setter);

    JS_DECLARE_NATIVE_FUNCTION(muted_getter);

    JS_DECLARE_NATIVE_FUNCTION(muted_setter);

    JS_DECLARE_NATIVE_FUNCTION(default_muted_getter);

    JS_DECLARE_NATIVE_FUNCTION(default_muted_setter);

    JS_DECLARE_NATIVE_FUNCTION(audio_tracks_getter);

    JS_DECLARE_NATIVE_FUNCTION(video_tracks_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_tracks_getter);


};


enum class Preload : u8 {

    Auto,

    None,

    Metadata,

};

inline String idl_enum_to_string(Preload value)
{
    switch (value) {

    case Preload::Auto:
        return "auto"_string;

    case Preload::None:
        return "none"_string;

    case Preload::Metadata:
        return "metadata"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CanPlayTypeResult : u8 {

    Empty,

    Maybe,

    Probably,

};

inline String idl_enum_to_string(CanPlayTypeResult value)
{
    switch (value) {

    case CanPlayTypeResult::Empty:
        return ""_string;

    case CanPlayTypeResult::Maybe:
        return "maybe"_string;

    case CanPlayTypeResult::Probably:
        return "probably"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    