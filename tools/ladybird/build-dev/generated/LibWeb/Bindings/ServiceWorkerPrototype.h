
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ServiceWorkerPrototype : public JS::Object {
    JS_OBJECT(ServiceWorkerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ServiceWorkerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ServiceWorkerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ServiceWorkerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(script_url_getter);

    JS_DECLARE_NATIVE_FUNCTION(state_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstatechange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onstatechange_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


enum class ServiceWorkerState : u8 {

    Parsed,

    Installing,

    Installed,

    Activating,

    Activated,

    Redundant,

};

inline String idl_enum_to_string(ServiceWorkerState value)
{
    switch (value) {

    case ServiceWorkerState::Parsed:
        return "parsed"_string;

    case ServiceWorkerState::Installing:
        return "installing"_string;

    case ServiceWorkerState::Installed:
        return "installed"_string;

    case ServiceWorkerState::Activating:
        return "activating"_string;

    case ServiceWorkerState::Activated:
        return "activated"_string;

    case ServiceWorkerState::Redundant:
        return "redundant"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    