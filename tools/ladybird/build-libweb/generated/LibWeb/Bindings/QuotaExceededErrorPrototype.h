
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class QuotaExceededErrorPrototype : public JS::Object {
    JS_OBJECT(QuotaExceededErrorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(QuotaExceededErrorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit QuotaExceededErrorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~QuotaExceededErrorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(quota_getter);

    JS_DECLARE_NATIVE_FUNCTION(requested_getter);


};


} // namespace Web::Bindings
    