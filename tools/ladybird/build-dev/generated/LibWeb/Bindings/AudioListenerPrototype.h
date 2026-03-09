
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioListenerPrototype : public JS::Object {
    JS_OBJECT(AudioListenerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioListenerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioListenerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioListenerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_position);
        
    JS_DECLARE_NATIVE_FUNCTION(set_orientation);
        
    JS_DECLARE_NATIVE_FUNCTION(position_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(position_z_getter);

    JS_DECLARE_NATIVE_FUNCTION(forward_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(forward_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(forward_z_getter);

    JS_DECLARE_NATIVE_FUNCTION(up_x_getter);

    JS_DECLARE_NATIVE_FUNCTION(up_y_getter);

    JS_DECLARE_NATIVE_FUNCTION(up_z_getter);


};


} // namespace Web::Bindings
    