
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSTranslatePrototype : public JS::Object {
    JS_OBJECT(CSSTranslatePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSTranslatePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSTranslatePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSTranslatePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(x_getter);

    JS_DECLARE_NATIVE_FUNCTION(x_setter);

    JS_DECLARE_NATIVE_FUNCTION(y_getter);

    JS_DECLARE_NATIVE_FUNCTION(y_setter);

    JS_DECLARE_NATIVE_FUNCTION(z_getter);

    JS_DECLARE_NATIVE_FUNCTION(z_setter);


};


} // namespace Web::Bindings
    