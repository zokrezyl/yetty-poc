
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLHtmlElementPrototype : public JS::Object {
    JS_OBJECT(HTMLHtmlElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLHtmlElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLHtmlElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLHtmlElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(version_getter);

    JS_DECLARE_NATIVE_FUNCTION(version_setter);


};


} // namespace Web::Bindings
    