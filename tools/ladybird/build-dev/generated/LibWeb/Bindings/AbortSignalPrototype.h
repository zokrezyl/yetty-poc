
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AbortSignalPrototype : public JS::Object {
    JS_OBJECT(AbortSignalPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AbortSignalPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AbortSignalPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AbortSignalPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(throw_if_aborted);
        
    JS_DECLARE_NATIVE_FUNCTION(aborted_getter);

    JS_DECLARE_NATIVE_FUNCTION(reason_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);


};


} // namespace Web::Bindings
    