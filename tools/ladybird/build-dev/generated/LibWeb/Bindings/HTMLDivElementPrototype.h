
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDivElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDivElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDivElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDivElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDivElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);


};


} // namespace Web::Bindings
    