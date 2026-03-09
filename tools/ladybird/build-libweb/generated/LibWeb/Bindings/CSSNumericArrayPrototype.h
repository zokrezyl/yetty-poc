
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSNumericArrayPrototype : public JS::Object {
    JS_OBJECT(CSSNumericArrayPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSNumericArrayPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSNumericArrayPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSNumericArrayPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    