
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TransformStreamDefaultControllerPrototype : public JS::Object {
    JS_OBJECT(TransformStreamDefaultControllerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TransformStreamDefaultControllerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TransformStreamDefaultControllerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TransformStreamDefaultControllerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(enqueue);
        
    JS_DECLARE_NATIVE_FUNCTION(error);
        
    JS_DECLARE_NATIVE_FUNCTION(terminate);
        
    JS_DECLARE_NATIVE_FUNCTION(desired_size_getter);


};


} // namespace Web::Bindings
    