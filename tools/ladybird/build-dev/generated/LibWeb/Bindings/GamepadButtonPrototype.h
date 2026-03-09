
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class GamepadButtonPrototype : public JS::Object {
    JS_OBJECT(GamepadButtonPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(GamepadButtonPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit GamepadButtonPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~GamepadButtonPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(pressed_getter);

    JS_DECLARE_NATIVE_FUNCTION(touched_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_getter);


};


} // namespace Web::Bindings
    