
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableStreamBYOBReaderPrototype : public JS::Object {
    JS_OBJECT(ReadableStreamBYOBReaderPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableStreamBYOBReaderPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ReadableStreamBYOBReaderPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamBYOBReaderPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(read);
        
    JS_DECLARE_NATIVE_FUNCTION(release_lock);
        
    JS_DECLARE_NATIVE_FUNCTION(cancel);
        
    JS_DECLARE_NATIVE_FUNCTION(closed_getter);


};


} // namespace Web::Bindings
    