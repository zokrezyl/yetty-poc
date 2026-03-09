
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformancePrototype : public JS::Object {
    JS_OBJECT(PerformancePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformancePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformancePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformancePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(now);
        
    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(mark);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_marks);
        
    JS_DECLARE_NATIVE_FUNCTION(measure);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_measures);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_resource_timings);
        
    JS_DECLARE_NATIVE_FUNCTION(set_resource_timing_buffer_size);
        
    JS_DECLARE_NATIVE_FUNCTION(get_entries);
        
    JS_DECLARE_NATIVE_FUNCTION(get_entries_by_type);
        
    JS_DECLARE_NATIVE_FUNCTION(get_entries_by_name);
        
    JS_DECLARE_NATIVE_FUNCTION(time_origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresourcetimingbufferfull_getter);

    JS_DECLARE_NATIVE_FUNCTION(onresourcetimingbufferfull_setter);

    JS_DECLARE_NATIVE_FUNCTION(timing_getter);

    JS_DECLARE_NATIVE_FUNCTION(navigation_getter);


};


} // namespace Web::Bindings
    