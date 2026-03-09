
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLAllCollectionPrototype : public JS::Object {
    JS_OBJECT(HTMLAllCollectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLAllCollectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLAllCollectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLAllCollectionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(named_item);
        
    JS_DECLARE_NATIVE_FUNCTION(item);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);


};


} // namespace Web::Bindings
    