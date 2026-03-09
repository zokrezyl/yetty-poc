
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigationDestinationPrototype : public JS::Object {
    JS_OBJECT(NavigationDestinationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigationDestinationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigationDestinationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationDestinationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_state);
        
    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(key_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(index_getter);

    JS_DECLARE_NATIVE_FUNCTION(same_document_getter);


};


} // namespace Web::Bindings
    