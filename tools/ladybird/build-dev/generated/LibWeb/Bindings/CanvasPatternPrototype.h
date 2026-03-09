
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CanvasPatternPrototype : public JS::Object {
    JS_OBJECT(CanvasPatternPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CanvasPatternPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CanvasPatternPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CanvasPatternPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(set_transform);
        

};


} // namespace Web::Bindings
    