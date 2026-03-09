
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLOptGroupElementPrototype : public JS::Object {
    JS_OBJECT(HTMLOptGroupElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLOptGroupElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLOptGroupElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLOptGroupElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);

    JS_DECLARE_NATIVE_FUNCTION(label_getter);

    JS_DECLARE_NATIVE_FUNCTION(label_setter);


};


} // namespace Web::Bindings
    