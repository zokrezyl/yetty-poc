
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PromiseRejectionEventPrototype : public JS::Object {
    JS_OBJECT(PromiseRejectionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PromiseRejectionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PromiseRejectionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PromiseRejectionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(promise_getter);

    JS_DECLARE_NATIVE_FUNCTION(reason_getter);


};


} // namespace Web::Bindings
    