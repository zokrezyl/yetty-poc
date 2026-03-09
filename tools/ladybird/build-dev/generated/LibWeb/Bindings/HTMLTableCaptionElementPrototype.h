
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTableCaptionElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTableCaptionElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTableCaptionElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTableCaptionElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableCaptionElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);


};


} // namespace Web::Bindings
    