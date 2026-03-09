
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BaseAudioContextPrototype : public JS::Object {
    JS_OBJECT(BaseAudioContextPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BaseAudioContextPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BaseAudioContextPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BaseAudioContextPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(create_analyser);
        
    JS_DECLARE_NATIVE_FUNCTION(create_biquad_filter);
        
    JS_DECLARE_NATIVE_FUNCTION(create_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(create_buffer_source);
        
    JS_DECLARE_NATIVE_FUNCTION(create_channel_merger);
        
    JS_DECLARE_NATIVE_FUNCTION(create_channel_splitter);
        
    JS_DECLARE_NATIVE_FUNCTION(create_constant_source);
        
    JS_DECLARE_NATIVE_FUNCTION(create_delay);
        
    JS_DECLARE_NATIVE_FUNCTION(create_dynamics_compressor);
        
    JS_DECLARE_NATIVE_FUNCTION(create_gain);
        
    JS_DECLARE_NATIVE_FUNCTION(create_oscillator);
        
    JS_DECLARE_NATIVE_FUNCTION(create_panner);
        
    JS_DECLARE_NATIVE_FUNCTION(create_periodic_wave);
        
    JS_DECLARE_NATIVE_FUNCTION(create_script_processor);
        
    JS_DECLARE_NATIVE_FUNCTION(create_stereo_panner);
        
    JS_DECLARE_NATIVE_FUNCTION(decode_audio_data);
        
    JS_DECLARE_NATIVE_FUNCTION(destination_getter);

    JS_DECLARE_NATIVE_FUNCTION(sample_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(current_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(listener_getter);

    JS_DECLARE_NATIVE_FUNCTION(state_getter);

    JS_DECLARE_NATIVE_FUNCTION(render_quantum_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstatechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstatechange_setter);


};


enum class AudioContextState : u8 {

    Suspended,

    Running,

    Closed,

};

inline String idl_enum_to_string(AudioContextState value)
{
    switch (value) {

    case AudioContextState::Suspended:
        return "suspended"_string;

    case AudioContextState::Running:
        return "running"_string;

    case AudioContextState::Closed:
        return "closed"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    