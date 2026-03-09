
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSTransitionPrototype : public JS::Object {
    JS_OBJECT(CSSTransitionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSTransitionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSTransitionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSTransitionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(transition_property_getter);


};


} // namespace Web::Bindings
    