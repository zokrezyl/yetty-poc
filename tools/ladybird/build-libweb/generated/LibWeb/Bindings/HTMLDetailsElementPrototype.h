
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDetailsElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDetailsElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDetailsElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDetailsElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDetailsElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_setter);

    JS_DECLARE_NATIVE_FUNCTION(open_getter);

    JS_DECLARE_NATIVE_FUNCTION(open_setter);


};


} // namespace Web::Bindings
    