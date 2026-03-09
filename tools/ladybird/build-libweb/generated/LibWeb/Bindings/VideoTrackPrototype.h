
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class VideoTrackPrototype : public JS::Object {
    JS_OBJECT(VideoTrackPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(VideoTrackPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit VideoTrackPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VideoTrackPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(kind_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(language_getter);

    JS_DECLARE_NATIVE_FUNCTION(selected_getter);

    JS_DECLARE_NATIVE_FUNCTION(selected_setter);


};


} // namespace Web::Bindings
    