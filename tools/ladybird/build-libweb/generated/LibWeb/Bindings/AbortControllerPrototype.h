
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AbortControllerPrototype : public JS::Object {
    JS_OBJECT(AbortControllerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AbortControllerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AbortControllerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AbortControllerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(signal_getter);


};


} // namespace Web::Bindings
    