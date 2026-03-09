
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class VideoTrackListPrototype : public JS::Object {
    JS_OBJECT(VideoTrackListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(VideoTrackListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit VideoTrackListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~VideoTrackListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_track_by_id);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(selected_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onaddtrack_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaddtrack_setter);

    JS_DECLARE_NATIVE_FUNCTION(onremovetrack_getter);

    JS_DECLARE_NATIVE_FUNCTION(onremovetrack_setter);


};


} // namespace Web::Bindings
    