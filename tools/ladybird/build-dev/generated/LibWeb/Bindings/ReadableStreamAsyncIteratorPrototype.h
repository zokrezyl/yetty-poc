
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableStreamAsyncIteratorPrototype : public JS::Object {
    JS_OBJECT(ReadableStreamAsyncIteratorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableStreamAsyncIteratorPrototype);

public:
    explicit ReadableStreamAsyncIteratorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamAsyncIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
    
    JS_DECLARE_NATIVE_FUNCTION(return_);

};

} // namespace Web::Bindings
    