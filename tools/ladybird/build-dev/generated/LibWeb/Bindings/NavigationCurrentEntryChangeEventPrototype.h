
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigationCurrentEntryChangeEventPrototype : public JS::Object {
    JS_OBJECT(NavigationCurrentEntryChangeEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigationCurrentEntryChangeEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigationCurrentEntryChangeEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationCurrentEntryChangeEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(navigation_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(from_getter);


};


} // namespace Web::Bindings
    