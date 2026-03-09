
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ManagedSourceBufferPrototype : public JS::Object {
    JS_OBJECT(ManagedSourceBufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ManagedSourceBufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ManagedSourceBufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ManagedSourceBufferPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onbufferedchange_getter);

    JS_DECLARE_NATIVE_FUNCTION(onbufferedchange_setter);


};


} // namespace Web::Bindings
    