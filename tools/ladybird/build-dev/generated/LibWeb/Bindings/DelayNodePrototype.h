
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DelayNodePrototype : public JS::Object {
    JS_OBJECT(DelayNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DelayNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DelayNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DelayNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(delay_time_getter);


};


} // namespace Web::Bindings
    