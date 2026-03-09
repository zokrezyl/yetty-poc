
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TransitionEventPrototype : public JS::Object {
    JS_OBJECT(TransitionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TransitionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TransitionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TransitionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(property_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(elapsed_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(pseudo_element_getter);


};


} // namespace Web::Bindings
    