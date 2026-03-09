
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLSyncPrototype : public JS::Object {
    JS_OBJECT(WebGLSyncPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLSyncPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLSyncPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLSyncPrototype() override;
private:


};


} // namespace Web::Bindings
    