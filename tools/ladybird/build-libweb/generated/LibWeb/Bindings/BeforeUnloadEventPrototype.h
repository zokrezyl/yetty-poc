
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class BeforeUnloadEventPrototype : public JS::Object {
    JS_OBJECT(BeforeUnloadEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(BeforeUnloadEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit BeforeUnloadEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~BeforeUnloadEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(return_value_getter);

    JS_DECLARE_NATIVE_FUNCTION(return_value_setter);


};


} // namespace Web::Bindings
    