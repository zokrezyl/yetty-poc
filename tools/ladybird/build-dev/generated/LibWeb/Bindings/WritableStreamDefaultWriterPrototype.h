
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WritableStreamDefaultWriterPrototype : public JS::Object {
    JS_OBJECT(WritableStreamDefaultWriterPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WritableStreamDefaultWriterPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WritableStreamDefaultWriterPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WritableStreamDefaultWriterPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(release_lock);
        
    JS_DECLARE_NATIVE_FUNCTION(write);
        
    JS_DECLARE_NATIVE_FUNCTION(closed_getter);

    JS_DECLARE_NATIVE_FUNCTION(desired_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_getter);


};


} // namespace Web::Bindings
    