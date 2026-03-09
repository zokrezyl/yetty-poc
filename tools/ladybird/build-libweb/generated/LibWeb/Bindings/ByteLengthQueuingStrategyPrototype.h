
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ByteLengthQueuingStrategyPrototype : public JS::Object {
    JS_OBJECT(ByteLengthQueuingStrategyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ByteLengthQueuingStrategyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ByteLengthQueuingStrategyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ByteLengthQueuingStrategyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(high_water_mark_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);


};


} // namespace Web::Bindings
    