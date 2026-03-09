
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DragEventPrototype : public JS::Object {
    JS_OBJECT(DragEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DragEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DragEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DragEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(data_transfer_getter);


};


} // namespace Web::Bindings
    