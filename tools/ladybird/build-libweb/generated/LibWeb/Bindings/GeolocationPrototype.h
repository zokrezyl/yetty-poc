
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GeolocationPrototype : public JS::Object {
    JS_OBJECT(GeolocationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GeolocationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GeolocationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GeolocationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_current_position);
        
    JS_DECLARE_NATIVE_FUNCTION(watch_position);
        
    JS_DECLARE_NATIVE_FUNCTION(clear_watch);
        

};


} // namespace Web::Bindings
    