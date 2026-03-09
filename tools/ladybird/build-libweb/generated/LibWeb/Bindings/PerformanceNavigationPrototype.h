
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceNavigationPrototype : public JS::Object {
    JS_OBJECT(PerformanceNavigationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceNavigationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceNavigationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceNavigationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(redirect_count_getter);


};


} // namespace Web::Bindings
    