
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextTrackCuePrototype : public JS::Object {
    JS_OBJECT(TextTrackCuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextTrackCuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextTrackCuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextTrackCuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(track_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_setter);

    JS_DECLARE_NATIVE_FUNCTION(start_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(start_time_setter);

    JS_DECLARE_NATIVE_FUNCTION(end_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(end_time_setter);

    JS_DECLARE_NATIVE_FUNCTION(pause_on_exit_getter);

    JS_DECLARE_NATIVE_FUNCTION(pause_on_exit_setter);

    JS_DECLARE_NATIVE_FUNCTION(onenter_getter);

    JS_DECLARE_NATIVE_FUNCTION(onenter_setter);

    JS_DECLARE_NATIVE_FUNCTION(onexit_getter);

    JS_DECLARE_NATIVE_FUNCTION(onexit_setter);


};


} // namespace Web::Bindings
    