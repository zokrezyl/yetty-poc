
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMExceptionPrototype : public JS::Object {
    JS_OBJECT(DOMExceptionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMExceptionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMExceptionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMExceptionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(message_getter);

    JS_DECLARE_NATIVE_FUNCTION(code_getter);


};


} // namespace Web::Bindings
    