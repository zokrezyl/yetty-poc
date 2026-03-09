
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FormDataIteratorPrototype : public JS::Object {
    JS_OBJECT(FormDataIteratorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FormDataIteratorPrototype);
public:
    explicit FormDataIteratorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FormDataIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

} // namespace Web::Bindings
    