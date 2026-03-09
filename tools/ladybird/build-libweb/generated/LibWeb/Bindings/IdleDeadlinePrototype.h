
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class IdleDeadlinePrototype : public JS::Object {
    JS_OBJECT(IdleDeadlinePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(IdleDeadlinePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit IdleDeadlinePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~IdleDeadlinePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(time_remaining);
        
    JS_DECLARE_NATIVE_FUNCTION(did_timeout_getter);


};


} // namespace Web::Bindings
    