
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceMeasurePrototype : public JS::Object {
    JS_OBJECT(PerformanceMeasurePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceMeasurePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceMeasurePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceMeasurePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(detail_getter);


};


} // namespace Web::Bindings
    