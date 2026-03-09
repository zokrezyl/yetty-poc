
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceMarkPrototype : public JS::Object {
    JS_OBJECT(PerformanceMarkPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceMarkPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceMarkPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceMarkPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(detail_getter);


};


} // namespace Web::Bindings
    