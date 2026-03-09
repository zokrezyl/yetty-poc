
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PopStateEventPrototype : public JS::Object {
    JS_OBJECT(PopStateEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PopStateEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PopStateEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PopStateEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(state_getter);


};


} // namespace Web::Bindings
    