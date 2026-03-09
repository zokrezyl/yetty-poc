
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CompositionEventPrototype : public JS::Object {
    JS_OBJECT(CompositionEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CompositionEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CompositionEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CompositionEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(init_composition_event);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);


};


} // namespace Web::Bindings
    