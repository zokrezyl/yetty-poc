
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ViewTransitionPrototype : public JS::Object {
    JS_OBJECT(ViewTransitionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ViewTransitionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ViewTransitionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ViewTransitionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(skip_transition);
        
    JS_DECLARE_NATIVE_FUNCTION(update_callback_done_getter);

    JS_DECLARE_NATIVE_FUNCTION(ready_getter);

    JS_DECLARE_NATIVE_FUNCTION(finished_getter);


};


} // namespace Web::Bindings
    