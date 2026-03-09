
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FocusEventPrototype : public JS::Object {
    JS_OBJECT(FocusEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FocusEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FocusEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FocusEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(related_target_getter);


};


} // namespace Web::Bindings
    