
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TimeRangesPrototype : public JS::Object {
    JS_OBJECT(TimeRangesPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TimeRangesPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TimeRangesPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TimeRangesPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(start);
        
    JS_DECLARE_NATIVE_FUNCTION(end);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    