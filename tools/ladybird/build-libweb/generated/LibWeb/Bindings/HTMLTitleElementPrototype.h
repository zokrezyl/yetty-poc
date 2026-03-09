
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTitleElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTitleElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTitleElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTitleElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTitleElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(text_getter);

    JS_DECLARE_NATIVE_FUNCTION(text_setter);


};


} // namespace Web::Bindings
    