
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSSkewYPrototype : public JS::Object {
    JS_OBJECT(CSSSkewYPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSSkewYPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSSkewYPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSSkewYPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(ay_getter);

    JS_DECLARE_NATIVE_FUNCTION(ay_setter);


};


} // namespace Web::Bindings
    