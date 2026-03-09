
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceObserverPrototype : public JS::Object {
    JS_OBJECT(PerformanceObserverPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceObserverPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceObserverPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceObserverPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(observe);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        
    JS_DECLARE_NATIVE_FUNCTION(take_records);
        

};


} // namespace Web::Bindings
    