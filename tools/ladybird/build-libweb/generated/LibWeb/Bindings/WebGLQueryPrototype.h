
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLQueryPrototype : public JS::Object {
    JS_OBJECT(WebGLQueryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLQueryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLQueryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLQueryPrototype() override;
private:


};


} // namespace Web::Bindings
    