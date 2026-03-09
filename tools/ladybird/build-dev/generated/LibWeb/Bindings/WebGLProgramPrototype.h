
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLProgramPrototype : public JS::Object {
    JS_OBJECT(WebGLProgramPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLProgramPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLProgramPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLProgramPrototype() override;
private:


};


} // namespace Web::Bindings
    