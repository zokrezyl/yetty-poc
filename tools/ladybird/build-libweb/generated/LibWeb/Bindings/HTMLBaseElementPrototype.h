
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLBaseElementPrototype : public JS::Object {
    JS_OBJECT(HTMLBaseElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLBaseElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLBaseElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLBaseElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_setter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_setter);


};


} // namespace Web::Bindings
    