
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioBufferSourceNodePrototype : public JS::Object {
    JS_OBJECT(AudioBufferSourceNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioBufferSourceNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioBufferSourceNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioBufferSourceNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(start);
        
    JS_DECLARE_NATIVE_FUNCTION(buffer_getter);

    JS_DECLARE_NATIVE_FUNCTION(buffer_setter);

    JS_DECLARE_NATIVE_FUNCTION(playback_rate_getter);

    JS_DECLARE_NATIVE_FUNCTION(detune_getter);

    JS_DECLARE_NATIVE_FUNCTION(loop_getter);

    JS_DECLARE_NATIVE_FUNCTION(loop_setter);

    JS_DECLARE_NATIVE_FUNCTION(loop_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(loop_start_setter);

    JS_DECLARE_NATIVE_FUNCTION(loop_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(loop_end_setter);


};


} // namespace Web::Bindings
    