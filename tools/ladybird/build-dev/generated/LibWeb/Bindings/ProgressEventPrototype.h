
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ProgressEventPrototype : public JS::Object {
    JS_OBJECT(ProgressEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ProgressEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ProgressEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ProgressEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(length_computable_getter);

    JS_DECLARE_NATIVE_FUNCTION(loaded_getter);

    JS_DECLARE_NATIVE_FUNCTION(total_getter);


};


} // namespace Web::Bindings
    