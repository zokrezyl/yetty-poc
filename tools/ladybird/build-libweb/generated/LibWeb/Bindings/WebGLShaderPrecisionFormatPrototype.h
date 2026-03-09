
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebGLShaderPrecisionFormatPrototype : public JS::Object {
    JS_OBJECT(WebGLShaderPrecisionFormatPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebGLShaderPrecisionFormatPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebGLShaderPrecisionFormatPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebGLShaderPrecisionFormatPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(range_min_getter);

    JS_DECLARE_NATIVE_FUNCTION(range_max_getter);

    JS_DECLARE_NATIVE_FUNCTION(precision_getter);


};


} // namespace Web::Bindings
    