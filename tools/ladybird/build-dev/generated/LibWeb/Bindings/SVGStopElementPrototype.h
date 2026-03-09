
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SVGStopElementPrototype : public JS::Object {
    JS_OBJECT(SVGStopElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SVGStopElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SVGStopElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SVGStopElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(offset_getter);


};


} // namespace Web::Bindings
    