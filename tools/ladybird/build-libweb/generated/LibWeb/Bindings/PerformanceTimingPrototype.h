
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceTimingPrototype : public JS::Object {
    JS_OBJECT(PerformanceTimingPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceTimingPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceTimingPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceTimingPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(navigation_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(unload_event_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(unload_event_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(fetch_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(domain_lookup_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(domain_lookup_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(connect_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(connect_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(secure_connection_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(request_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(response_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(dom_loading_getter);

    JS_DECLARE_NATIVE_FUNCTION(dom_interactive_getter);

    JS_DECLARE_NATIVE_FUNCTION(dom_content_loaded_event_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(dom_content_loaded_event_end_getter);

    JS_DECLARE_NATIVE_FUNCTION(dom_complete_getter);

    JS_DECLARE_NATIVE_FUNCTION(load_event_start_getter);

    JS_DECLARE_NATIVE_FUNCTION(load_event_end_getter);


};


} // namespace Web::Bindings
    