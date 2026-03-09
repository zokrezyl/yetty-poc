
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SubmitEventPrototype : public JS::Object {
    JS_OBJECT(SubmitEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SubmitEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SubmitEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SubmitEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(submitter_getter);


};


} // namespace Web::Bindings
    