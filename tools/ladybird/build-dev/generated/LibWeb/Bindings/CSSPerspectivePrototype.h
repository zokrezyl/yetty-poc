
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSPerspectivePrototype : public JS::Object {
    JS_OBJECT(CSSPerspectivePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSPerspectivePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSPerspectivePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSPerspectivePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_setter);


};


} // namespace Web::Bindings
    