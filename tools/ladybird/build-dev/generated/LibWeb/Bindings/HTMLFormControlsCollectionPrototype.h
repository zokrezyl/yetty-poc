
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLFormControlsCollectionPrototype : public JS::Object {
    JS_OBJECT(HTMLFormControlsCollectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLFormControlsCollectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLFormControlsCollectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLFormControlsCollectionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(named_item);
        

};


} // namespace Web::Bindings
    