
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ResizeObserverSizePrototype : public JS::Object {
    JS_OBJECT(ResizeObserverSizePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ResizeObserverSizePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ResizeObserverSizePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ResizeObserverSizePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(inline_size_getter);

    JS_DECLARE_NATIVE_FUNCTION(block_size_getter);


};


} // namespace Web::Bindings
    