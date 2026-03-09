
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FontFaceSetLoadEventPrototype : public JS::Object {
    JS_OBJECT(FontFaceSetLoadEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FontFaceSetLoadEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FontFaceSetLoadEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FontFaceSetLoadEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(fontfaces_getter);


};


} // namespace Web::Bindings
    