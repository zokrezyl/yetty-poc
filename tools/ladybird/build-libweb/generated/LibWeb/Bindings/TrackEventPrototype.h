
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrackEventPrototype : public JS::Object {
    JS_OBJECT(TrackEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrackEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrackEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrackEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(track_getter);


};


} // namespace Web::Bindings
    