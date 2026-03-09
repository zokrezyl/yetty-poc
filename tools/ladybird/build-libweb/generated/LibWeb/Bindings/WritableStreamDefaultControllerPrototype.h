
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WritableStreamDefaultControllerPrototype : public JS::Object {
    JS_OBJECT(WritableStreamDefaultControllerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WritableStreamDefaultControllerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WritableStreamDefaultControllerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WritableStreamDefaultControllerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(error);
        
    JS_DECLARE_NATIVE_FUNCTION(signal_getter);


};


} // namespace Web::Bindings
    