
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HeadersIteratorPrototype : public JS::Object {
    JS_OBJECT(HeadersIteratorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HeadersIteratorPrototype);
public:
    explicit HeadersIteratorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HeadersIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

} // namespace Web::Bindings
    