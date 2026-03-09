
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigatorPrototype : public JS::Object {
    JS_OBJECT(NavigatorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigatorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigatorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigatorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_battery);
        
    JS_DECLARE_NATIVE_FUNCTION(request_media_key_system_access);
        
    JS_DECLARE_NATIVE_FUNCTION(get_gamepads);
        
    JS_DECLARE_NATIVE_FUNCTION(send_beacon);
        
    JS_DECLARE_NATIVE_FUNCTION(java_enabled);
        
    JS_DECLARE_NATIVE_FUNCTION(clipboard_getter);

    JS_DECLARE_NATIVE_FUNCTION(geolocation_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_touch_points_getter);

    JS_DECLARE_NATIVE_FUNCTION(serial_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_activation_getter);

    JS_DECLARE_NATIVE_FUNCTION(service_worker_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_capabilities_getter);

    JS_DECLARE_NATIVE_FUNCTION(credentials_getter);

    JS_DECLARE_NATIVE_FUNCTION(xr_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_code_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(app_version_getter);

    JS_DECLARE_NATIVE_FUNCTION(platform_getter);

    JS_DECLARE_NATIVE_FUNCTION(product_getter);

    JS_DECLARE_NATIVE_FUNCTION(product_sub_getter);

    JS_DECLARE_NATIVE_FUNCTION(user_agent_getter);

    JS_DECLARE_NATIVE_FUNCTION(vendor_getter);

    JS_DECLARE_NATIVE_FUNCTION(vendor_sub_getter);

    JS_DECLARE_NATIVE_FUNCTION(plugins_getter);

    JS_DECLARE_NATIVE_FUNCTION(mime_types_getter);

    JS_DECLARE_NATIVE_FUNCTION(pdf_viewer_enabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(webdriver_getter);

    JS_DECLARE_NATIVE_FUNCTION(cookie_enabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(on_line_getter);

    JS_DECLARE_NATIVE_FUNCTION(storage_getter);

    JS_DECLARE_NATIVE_FUNCTION(global_privacy_control_getter);

    JS_DECLARE_NATIVE_FUNCTION(language_getter);

    JS_DECLARE_NATIVE_FUNCTION(languages_getter);

    JS_DECLARE_NATIVE_FUNCTION(hardware_concurrency_getter);

    JS_DECLARE_NATIVE_FUNCTION(device_memory_getter);


};


} // namespace Web::Bindings
    