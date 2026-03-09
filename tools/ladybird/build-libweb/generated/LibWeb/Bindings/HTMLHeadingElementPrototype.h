
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLHeadingElementPrototype : public JS::Object {
    JS_OBJECT(HTMLHeadingElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLHeadingElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLHeadingElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHeadingElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);


};


} // namespace Web::Bindings
    