
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLOptionsCollectionPrototype : public JS::Object {
    JS_OBJECT(HTMLOptionsCollectionPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLOptionsCollectionPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLOptionsCollectionPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLOptionsCollectionPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add);
        
    JS_DECLARE_NATIVE_FUNCTION(remove);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(length_setter);

    JS_DECLARE_NATIVE_FUNCTION(selected_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(selected_index_setter);


};


} // namespace Web::Bindings
    