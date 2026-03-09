
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLVideoElementPrototype : public JS::Object {
    JS_OBJECT(HTMLVideoElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLVideoElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLVideoElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLVideoElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(video_width_getter);

    JS_DECLARE_NATIVE_FUNCTION(video_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(poster_getter);

    JS_DECLARE_NATIVE_FUNCTION(poster_setter);

    JS_DECLARE_NATIVE_FUNCTION(plays_inline_getter);

    JS_DECLARE_NATIVE_FUNCTION(plays_inline_setter);


};


} // namespace Web::Bindings
    