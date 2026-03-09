
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ExternalPrototype : public JS::Object {
    JS_OBJECT(ExternalPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ExternalPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ExternalPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ExternalPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(add_search_provider);
        
    JS_DECLARE_NATIVE_FUNCTION(is_search_provider_installed);
        

};


} // namespace Web::Bindings
    