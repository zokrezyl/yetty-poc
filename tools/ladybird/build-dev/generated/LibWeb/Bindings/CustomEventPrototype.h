
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CustomEventPrototype : public JS::Object {
    JS_OBJECT(CustomEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CustomEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CustomEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CustomEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(init_custom_event);
        
    JS_DECLARE_NATIVE_FUNCTION(detail_getter);


};


} // namespace Web::Bindings
    