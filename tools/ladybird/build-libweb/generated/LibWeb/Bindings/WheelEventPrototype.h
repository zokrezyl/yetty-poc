
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WheelEventPrototype : public JS::Object {
    JS_OBJECT(WheelEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WheelEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WheelEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WheelEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(delta_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(delta_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(delta_z_getter);

    JS_DECLARE_NATIVE_FUNCTION(delta_mode_getter);


};


} // namespace Web::Bindings
    