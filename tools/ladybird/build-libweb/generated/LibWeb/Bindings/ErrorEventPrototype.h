
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ErrorEventPrototype : public JS::Object {
    JS_OBJECT(ErrorEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ErrorEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ErrorEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ErrorEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(message_getter);

    JS_DECLARE_NATIVE_FUNCTION(filename_getter);

    JS_DECLARE_NATIVE_FUNCTION(lineno_getter);

    JS_DECLARE_NATIVE_FUNCTION(colno_getter);

    JS_DECLARE_NATIVE_FUNCTION(error_getter);


};


} // namespace Web::Bindings
    