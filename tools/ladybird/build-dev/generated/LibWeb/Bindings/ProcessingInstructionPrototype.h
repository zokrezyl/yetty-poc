
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ProcessingInstructionPrototype : public JS::Object {
    JS_OBJECT(ProcessingInstructionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ProcessingInstructionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ProcessingInstructionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ProcessingInstructionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(target_getter);


};


} // namespace Web::Bindings
    