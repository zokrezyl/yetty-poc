
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IntersectionObserverEntryPrototype : public JS::Object {
    JS_OBJECT(IntersectionObserverEntryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IntersectionObserverEntryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IntersectionObserverEntryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IntersectionObserverEntryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(time_getter);

    JS_DECLARE_NATIVE_FUNCTION(root_bounds_getter);

    JS_DECLARE_NATIVE_FUNCTION(bounding_client_rect_getter);

    JS_DECLARE_NATIVE_FUNCTION(intersection_rect_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_intersecting_getter);

    JS_DECLARE_NATIVE_FUNCTION(intersection_ratio_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);


};


} // namespace Web::Bindings
    