
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WorkerNavigatorPrototype : public JS::Object {
    JS_OBJECT(WorkerNavigatorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WorkerNavigatorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WorkerNavigatorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkerNavigatorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(media_capabilities_getter);

    JS_DECLARE_NATIVE_FUNCTION(serial_getter);

    JS_DECLARE_NATIVE_FUNCTION(service_worker_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_code_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_version_getter);

    JS_DECLARE_NATIVE_FUNCTION(platform_getter);

    JS_DECLARE_NATIVE_FUNCTION(product_getter);

    JS_DECLARE_NATIVE_FUNCTION(product_sub_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_agent_getter);

    JS_DECLARE_NATIVE_FUNCTION(vendor_getter);

    JS_DECLARE_NATIVE_FUNCTION(vendor_sub_getter);

    JS_DECLARE_NATIVE_FUNCTION(on_line_getter);

    JS_DECLARE_NATIVE_FUNCTION(storage_getter);

    JS_DECLARE_NATIVE_FUNCTION(language_getter);

    JS_DECLARE_NATIVE_FUNCTION(languages_getter);

    JS_DECLARE_NATIVE_FUNCTION(hardware_concurrency_getter);

    JS_DECLARE_NATIVE_FUNCTION(device_memory_getter);


};


} // namespace Web::Bindings
    