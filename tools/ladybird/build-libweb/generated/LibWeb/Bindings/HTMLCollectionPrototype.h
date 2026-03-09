
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLCollectionPrototype : public JS::Object {
    JS_OBJECT(HTMLCollectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLCollectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLCollectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLCollectionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    