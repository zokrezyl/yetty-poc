
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GamepadEventPrototype : public JS::Object {
    JS_OBJECT(GamepadEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GamepadEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GamepadEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GamepadEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(gamepad_getter);


};


} // namespace Web::Bindings
    