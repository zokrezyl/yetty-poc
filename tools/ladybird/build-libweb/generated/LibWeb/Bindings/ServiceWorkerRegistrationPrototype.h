
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ServiceWorkerRegistrationPrototype : public JS::Object {
    JS_OBJECT(ServiceWorkerRegistrationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ServiceWorkerRegistrationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ServiceWorkerRegistrationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ServiceWorkerRegistrationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(installing_getter);

    JS_DECLARE_NATIVE_FUNCTION(waiting_getter);

    JS_DECLARE_NATIVE_FUNCTION(active_getter);

    JS_DECLARE_NATIVE_FUNCTION(scope_getter);

    JS_DECLARE_NATIVE_FUNCTION(update_via_cache_getter);


};


enum class ServiceWorkerUpdateViaCache : u8 {

    Imports,

    All,

    None,

};

inline String idl_enum_to_string(ServiceWorkerUpdateViaCache value)
{
    switch (value) {

    case ServiceWorkerUpdateViaCache::Imports:
        return "imports"_string;

    case ServiceWorkerUpdateViaCache::All:
        return "all"_string;

    case ServiceWorkerUpdateViaCache::None:
        return "none"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    