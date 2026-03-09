
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PerformanceObserverEntryListPrototype : public JS::Object {
    JS_OBJECT(PerformanceObserverEntryListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PerformanceObserverEntryListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PerformanceObserverEntryListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PerformanceObserverEntryListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_entries);
        
    JS_DECLARE_NATIVE_FUNCTION(get_entries_by_type);
        
    JS_DECLARE_NATIVE_FUNCTION(get_entries_by_name);
        

};


} // namespace Web::Bindings
    