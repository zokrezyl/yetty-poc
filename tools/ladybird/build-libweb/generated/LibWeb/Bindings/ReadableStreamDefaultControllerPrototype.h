
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableStreamDefaultControllerPrototype : public JS::Object {
    JS_OBJECT(ReadableStreamDefaultControllerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableStreamDefaultControllerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ReadableStreamDefaultControllerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamDefaultControllerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(enqueue);
        
    JS_DECLARE_NATIVE_FUNCTION(error);
        
    JS_DECLARE_NATIVE_FUNCTION(desired_size_getter);


};


} // namespace Web::Bindings
    