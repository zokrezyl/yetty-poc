
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioBufferPrototype : public JS::Object {
    JS_OBJECT(AudioBufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioBufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioBufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioBufferPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_channel_data);
        
    JS_DECLARE_NATIVE_FUNCTION(copy_from_channel);
        
    JS_DECLARE_NATIVE_FUNCTION(copy_to_channel);
        
    JS_DECLARE_NATIVE_FUNCTION(sample_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(duration_getter);

    JS_DECLARE_NATIVE_FUNCTION(number_of_channels_getter);


};


} // namespace Web::Bindings
    