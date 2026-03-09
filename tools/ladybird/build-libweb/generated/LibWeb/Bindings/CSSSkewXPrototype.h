
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSSkewXPrototype : public JS::Object {
    JS_OBJECT(CSSSkewXPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSSkewXPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSSkewXPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSSkewXPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(ax_getter);

    JS_DECLARE_NATIVE_FUNCTION(ax_setter);


};


} // namespace Web::Bindings
    