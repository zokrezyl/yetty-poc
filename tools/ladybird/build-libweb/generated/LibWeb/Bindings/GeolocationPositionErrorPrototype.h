
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GeolocationPositionErrorPrototype : public JS::Object {
    JS_OBJECT(GeolocationPositionErrorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GeolocationPositionErrorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GeolocationPositionErrorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GeolocationPositionErrorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(code_getter);

    JS_DECLARE_NATIVE_FUNCTION(message_getter);


};


} // namespace Web::Bindings
    