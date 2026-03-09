
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CountQueuingStrategyPrototype : public JS::Object {
    JS_OBJECT(CountQueuingStrategyPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CountQueuingStrategyPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CountQueuingStrategyPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CountQueuingStrategyPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(high_water_mark_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);


};


} // namespace Web::Bindings
    