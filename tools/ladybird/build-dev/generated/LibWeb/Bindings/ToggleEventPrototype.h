
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ToggleEventPrototype : public JS::Object {
    JS_OBJECT(ToggleEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ToggleEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ToggleEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ToggleEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(old_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(new_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(source_getter);


};


} // namespace Web::Bindings
    