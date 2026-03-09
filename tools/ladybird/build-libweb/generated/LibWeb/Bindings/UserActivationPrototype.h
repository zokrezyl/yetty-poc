
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class UserActivationPrototype : public JS::Object {
    JS_OBJECT(UserActivationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(UserActivationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit UserActivationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~UserActivationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(has_been_active_getter);

    JS_DECLARE_NATIVE_FUNCTION(is_active_getter);


};


} // namespace Web::Bindings
    