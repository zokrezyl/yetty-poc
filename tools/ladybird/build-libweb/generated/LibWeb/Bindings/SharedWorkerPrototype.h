
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SharedWorkerPrototype : public JS::Object {
    JS_OBJECT(SharedWorkerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SharedWorkerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SharedWorkerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SharedWorkerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(port_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


} // namespace Web::Bindings
    