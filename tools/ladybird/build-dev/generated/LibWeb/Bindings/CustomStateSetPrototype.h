
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CustomStateSetPrototype : public JS::Object {
    JS_OBJECT(CustomStateSetPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CustomStateSetPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CustomStateSetPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CustomStateSetPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_size);
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(values);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(has);

    JS_DECLARE_NATIVE_FUNCTION(add);

    JS_DECLARE_NATIVE_FUNCTION(delete_);

    JS_DECLARE_NATIVE_FUNCTION(clear);


};


} // namespace Web::Bindings
    