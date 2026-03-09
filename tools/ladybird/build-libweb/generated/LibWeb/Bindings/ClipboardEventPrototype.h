
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ClipboardEventPrototype : public JS::Object {
    JS_OBJECT(ClipboardEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ClipboardEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ClipboardEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ClipboardEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(clipboard_data_getter);


};


} // namespace Web::Bindings
    