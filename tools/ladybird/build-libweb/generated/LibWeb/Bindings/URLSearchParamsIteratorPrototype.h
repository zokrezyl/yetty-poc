
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class URLSearchParamsIteratorPrototype : public JS::Object {
    JS_OBJECT(URLSearchParamsIteratorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(URLSearchParamsIteratorPrototype);
public:
    explicit URLSearchParamsIteratorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~URLSearchParamsIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

} // namespace Web::Bindings
    