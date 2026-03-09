
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CloseEventPrototype : public JS::Object {
    JS_OBJECT(CloseEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CloseEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CloseEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CloseEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(was_clean_getter);

    JS_DECLARE_NATIVE_FUNCTION(code_getter);

    JS_DECLARE_NATIVE_FUNCTION(reason_getter);


};


} // namespace Web::Bindings
    