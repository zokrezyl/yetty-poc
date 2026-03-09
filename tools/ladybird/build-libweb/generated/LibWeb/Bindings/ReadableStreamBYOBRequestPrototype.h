
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableStreamBYOBRequestPrototype : public JS::Object {
    JS_OBJECT(ReadableStreamBYOBRequestPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableStreamBYOBRequestPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ReadableStreamBYOBRequestPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamBYOBRequestPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(respond);
        
    JS_DECLARE_NATIVE_FUNCTION(respond_with_new_view);
        
    JS_DECLARE_NATIVE_FUNCTION(view_getter);


};


} // namespace Web::Bindings
    