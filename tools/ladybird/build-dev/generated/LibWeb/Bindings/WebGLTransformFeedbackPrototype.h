
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLTransformFeedbackPrototype : public JS::Object {
    JS_OBJECT(WebGLTransformFeedbackPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLTransformFeedbackPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLTransformFeedbackPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLTransformFeedbackPrototype() override;
private:


};


} // namespace Web::Bindings
    