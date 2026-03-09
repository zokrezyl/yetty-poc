
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSSkewPrototype : public JS::Object {
    JS_OBJECT(CSSSkewPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSSkewPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSSkewPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSSkewPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(ax_getter);

    JS_DECLARE_NATIVE_FUNCTION(ax_setter);

    JS_DECLARE_NATIVE_FUNCTION(ay_getter);

    JS_DECLARE_NATIVE_FUNCTION(ay_setter);


};


} // namespace Web::Bindings
    