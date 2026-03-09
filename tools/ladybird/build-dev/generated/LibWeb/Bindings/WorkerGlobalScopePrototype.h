
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WorkerGlobalScopePrototype : public JS::Object {
    JS_OBJECT(WorkerGlobalScopePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WorkerGlobalScopePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WorkerGlobalScopePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkerGlobalScopePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(import_scripts);
        
    JS_DECLARE_NATIVE_FUNCTION(btoa);
        
    JS_DECLARE_NATIVE_FUNCTION(atob);
        
    JS_DECLARE_NATIVE_FUNCTION(queue_microtask);
        
    JS_DECLARE_NATIVE_FUNCTION(structured_clone);
        
    JS_DECLARE_NATIVE_FUNCTION(report_error);
        
    JS_DECLARE_NATIVE_FUNCTION(set_timeout);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_timeout);
        
    JS_DECLARE_NATIVE_FUNCTION(set_interval);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_interval);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap);
        
    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap0);

    JS_DECLARE_NATIVE_FUNCTION(create_image_bitmap1);

    JS_DECLARE_NATIVE_FUNCTION(fetch);
        
    JS_DECLARE_NATIVE_FUNCTION(self_getter);

    JS_DECLARE_NATIVE_FUNCTION(location_getter);

    JS_DECLARE_NATIVE_FUNCTION(navigator_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onlanguagechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_getter);

    JS_DECLARE_NATIVE_FUNCTION(onoffline_setter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_getter);

    JS_DECLARE_NATIVE_FUNCTION(ononline_setter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_getter);

    JS_DECLARE_NATIVE_FUNCTION(onrejectionhandled_setter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_getter);

    JS_DECLARE_NATIVE_FUNCTION(onunhandledrejection_setter);

    JS_DECLARE_NATIVE_FUNCTION(origin_getter);

    JS_DECLARE_NATIVE_FUNCTION(origin_setter);

    JS_DECLARE_NATIVE_FUNCTION(is_secure_context_getter);

    JS_DECLARE_NATIVE_FUNCTION(cross_origin_isolated_getter);

    JS_DECLARE_NATIVE_FUNCTION(performance_getter);

    JS_DECLARE_NATIVE_FUNCTION(performance_setter);

    JS_DECLARE_NATIVE_FUNCTION(indexed_db_getter);

    JS_DECLARE_NATIVE_FUNCTION(crypto_getter);

    JS_DECLARE_NATIVE_FUNCTION(caches_getter);

    JS_DECLARE_NATIVE_FUNCTION(trusted_types_getter);

    JS_DECLARE_NATIVE_FUNCTION(fonts_getter);


};


} // namespace Web::Bindings
    