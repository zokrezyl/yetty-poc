
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class InstancePrototype : public JS::Object {
    JS_OBJECT(InstancePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(InstancePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit InstancePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~InstancePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(exports_getter);


};


} // namespace Web::Bindings
    