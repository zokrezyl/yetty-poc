
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLContextEventPrototype : public JS::Object {
    JS_OBJECT(WebGLContextEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLContextEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLContextEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLContextEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(status_message_getter);


};


} // namespace Web::Bindings
    