
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioTrackPrototype : public JS::Object {
    JS_OBJECT(AudioTrackPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioTrackPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioTrackPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioTrackPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(kind_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(language_getter);

    JS_DECLARE_NATIVE_FUNCTION(enabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(enabled_setter);


};


} // namespace Web::Bindings
    