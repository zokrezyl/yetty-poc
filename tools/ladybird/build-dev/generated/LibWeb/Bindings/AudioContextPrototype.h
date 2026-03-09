
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioContextPrototype : public JS::Object {
    JS_OBJECT(AudioContextPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioContextPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioContextPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioContextPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_output_timestamp);
        
    JS_DECLARE_NATIVE_FUNCTION(resume);
        
    JS_DECLARE_NATIVE_FUNCTION(suspend);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(create_media_element_source);
        
    JS_DECLARE_NATIVE_FUNCTION(base_latency_getter);

    JS_DECLARE_NATIVE_FUNCTION(output_latency_getter);


};


enum class AudioContextLatencyCategory : u8 {

    Balanced,

    Interactive,

    Playback,

};

inline String idl_enum_to_string(AudioContextLatencyCategory value)
{
    switch (value) {

    case AudioContextLatencyCategory::Balanced:
        return "balanced"_string;

    case AudioContextLatencyCategory::Interactive:
        return "interactive"_string;

    case AudioContextLatencyCategory::Playback:
        return "playback"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    