
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLProgressElementPrototype : public JS::Object {
    JS_OBJECT(HTMLProgressElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLProgressElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLProgressElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLProgressElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(value_getter);

    JS_DECLARE_NATIVE_FUNCTION(value_setter);

    JS_DECLARE_NATIVE_FUNCTION(max_getter);

    JS_DECLARE_NATIVE_FUNCTION(max_setter);

    JS_DECLARE_NATIVE_FUNCTION(position_getter);

    JS_DECLARE_NATIVE_FUNCTION(labels_getter);


};


} // namespace Web::Bindings
    