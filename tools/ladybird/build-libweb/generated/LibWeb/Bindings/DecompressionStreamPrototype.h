
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DecompressionStreamPrototype : public JS::Object {
    JS_OBJECT(DecompressionStreamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DecompressionStreamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DecompressionStreamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DecompressionStreamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(readable_getter);

    JS_DECLARE_NATIVE_FUNCTION(writable_getter);


};


} // namespace Web::Bindings
    