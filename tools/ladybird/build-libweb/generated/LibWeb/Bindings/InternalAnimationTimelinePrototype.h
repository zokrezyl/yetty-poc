
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class InternalAnimationTimelinePrototype : public JS::Object {
    JS_OBJECT(InternalAnimationTimelinePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(InternalAnimationTimelinePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit InternalAnimationTimelinePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~InternalAnimationTimelinePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_time);
        

};


} // namespace Web::Bindings
    