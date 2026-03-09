
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CookieChangeEventPrototype : public JS::Object {
    JS_OBJECT(CookieChangeEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CookieChangeEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CookieChangeEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CookieChangeEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(changed_getter);

    JS_DECLARE_NATIVE_FUNCTION(deleted_getter);


};


} // namespace Web::Bindings
    