
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class NavigationHistoryEntryPrototype : public JS::Object {
    JS_OBJECT(NavigationHistoryEntryPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(NavigationHistoryEntryPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit NavigationHistoryEntryPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~NavigationHistoryEntryPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_state);
        
    JS_DECLARE_NATIVE_FUNCTION(url_getter);

    JS_DECLARE_NATIVE_FUNCTION(key_getter);

    JS_DECLARE_NATIVE_FUNCTION(id_getter);

    JS_DECLARE_NATIVE_FUNCTION(index_getter);

    JS_DECLARE_NATIVE_FUNCTION(same_document_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondispose_getter);

    JS_DECLARE_NATIVE_FUNCTION(ondispose_setter);


};


} // namespace Web::Bindings
    