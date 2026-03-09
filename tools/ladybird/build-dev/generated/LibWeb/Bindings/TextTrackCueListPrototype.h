
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextTrackCueListPrototype : public JS::Object {
    JS_OBJECT(TextTrackCueListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextTrackCueListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextTrackCueListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextTrackCueListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_cue_by_id);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    