
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigationTransitionPrototype : public JS::Object {
    JS_OBJECT(NavigationTransitionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigationTransitionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigationTransitionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationTransitionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(navigation_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(from_getter);

    JS_DECLARE_NATIVE_FUNCTION(to_getter);

    JS_DECLARE_NATIVE_FUNCTION(committed_getter);

    JS_DECLARE_NATIVE_FUNCTION(finished_getter);


};


} // namespace Web::Bindings
    