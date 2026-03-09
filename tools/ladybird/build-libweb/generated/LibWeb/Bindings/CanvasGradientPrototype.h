
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CanvasGradientPrototype : public JS::Object {
    JS_OBJECT(CanvasGradientPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CanvasGradientPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CanvasGradientPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CanvasGradientPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add_color_stop);
        

};


} // namespace Web::Bindings
    