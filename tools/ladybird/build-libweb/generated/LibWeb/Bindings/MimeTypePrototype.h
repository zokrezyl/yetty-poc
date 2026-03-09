
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MimeTypePrototype : public JS::Object {
    JS_OBJECT(MimeTypePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MimeTypePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MimeTypePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MimeTypePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(description_getter);

    JS_DECLARE_NATIVE_FUNCTION(suffixes_getter);

    JS_DECLARE_NATIVE_FUNCTION(enabled_plugin_getter);


};


} // namespace Web::Bindings
    