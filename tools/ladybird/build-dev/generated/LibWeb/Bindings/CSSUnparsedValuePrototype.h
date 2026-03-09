
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSUnparsedValuePrototype : public JS::Object {
    JS_OBJECT(CSSUnparsedValuePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSUnparsedValuePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSUnparsedValuePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSUnparsedValuePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    