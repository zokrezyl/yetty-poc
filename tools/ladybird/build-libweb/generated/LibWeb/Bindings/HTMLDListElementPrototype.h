
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDListElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDListElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDListElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDListElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDListElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(compact_getter);

    JS_DECLARE_NATIVE_FUNCTION(compact_setter);


};


} // namespace Web::Bindings
    