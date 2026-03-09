
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FakeXRDevicePrototype : public JS::Object {
    JS_OBJECT(FakeXRDevicePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FakeXRDevicePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FakeXRDevicePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FakeXRDevicePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        

};


} // namespace Web::Bindings
    