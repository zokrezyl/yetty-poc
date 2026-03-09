
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceEntryPrototype : public JS::Object {
    JS_OBJECT(PerformanceEntryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceEntryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceEntryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceEntryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(entry_type_getter);

    JS_DECLARE_NATIVE_FUNCTION(start_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(duration_getter);


};


} // namespace Web::Bindings
    