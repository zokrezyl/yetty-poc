
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceEventTimingPrototype : public JS::Object {
    JS_OBJECT(PerformanceEventTimingPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceEventTimingPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceEventTimingPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceEventTimingPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(processing_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(processing_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(cancelable_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(interaction_id_getter);


};


} // namespace Web::Bindings
    