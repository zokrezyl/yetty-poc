
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLPreElementPrototype : public JS::Object {
    JS_OBJECT(HTMLPreElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLPreElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLPreElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLPreElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);


};


} // namespace Web::Bindings
    