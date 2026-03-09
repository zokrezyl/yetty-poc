
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLParagraphElementPrototype : public JS::Object {
    JS_OBJECT(HTMLParagraphElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLParagraphElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLParagraphElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLParagraphElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);


};


} // namespace Web::Bindings
    