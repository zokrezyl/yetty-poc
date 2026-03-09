
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextTrackListPrototype : public JS::Object {
    JS_OBJECT(TextTrackListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextTrackListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextTrackListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextTrackListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_track_by_id);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onchange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onaddtrack_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaddtrack_setter);

    JS_DECLARE_NATIVE_FUNCTION(onremovetrack_getter);

    JS_DECLARE_NATIVE_FUNCTION(onremovetrack_setter);


};


} // namespace Web::Bindings
    