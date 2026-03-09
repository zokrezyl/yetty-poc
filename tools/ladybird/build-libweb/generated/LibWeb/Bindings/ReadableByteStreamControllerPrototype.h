
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableByteStreamControllerPrototype : public JS::Object {
    JS_OBJECT(ReadableByteStreamControllerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableByteStreamControllerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ReadableByteStreamControllerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableByteStreamControllerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(error);
        
    JS_DECLARE_NATIVE_FUNCTION(enqueue);
        
    JS_DECLARE_NATIVE_FUNCTION(byob_request_getter);

    JS_DECLARE_NATIVE_FUNCTION(desired_size_getter);


};


} // namespace Web::Bindings
    