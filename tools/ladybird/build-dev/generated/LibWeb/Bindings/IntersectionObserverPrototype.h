
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IntersectionObserverPrototype : public JS::Object {
    JS_OBJECT(IntersectionObserverPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IntersectionObserverPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IntersectionObserverPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IntersectionObserverPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(observe);
        
    JS_DECLARE_NATIVE_FUNCTION(unobserve);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        
    JS_DECLARE_NATIVE_FUNCTION(take_records);
        
    JS_DECLARE_NATIVE_FUNCTION(root_getter);

    JS_DECLARE_NATIVE_FUNCTION(root_margin_getter);

    JS_DECLARE_NATIVE_FUNCTION(scroll_margin_getter);

    JS_DECLARE_NATIVE_FUNCTION(thresholds_getter);

    JS_DECLARE_NATIVE_FUNCTION(delay_getter);

    JS_DECLARE_NATIVE_FUNCTION(track_visibility_getter);


};


} // namespace Web::Bindings
    