
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MutationObserverPrototype : public JS::Object {
    JS_OBJECT(MutationObserverPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MutationObserverPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MutationObserverPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MutationObserverPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(observe);
        
    JS_DECLARE_NATIVE_FUNCTION(disconnect);
        
    JS_DECLARE_NATIVE_FUNCTION(take_records);
        

};


} // namespace Web::Bindings
    