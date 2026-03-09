
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CredentialsContainerPrototype : public JS::Object {
    JS_OBJECT(CredentialsContainerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CredentialsContainerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CredentialsContainerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CredentialsContainerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get);
        
    JS_DECLARE_NATIVE_FUNCTION(store);
        
    JS_DECLARE_NATIVE_FUNCTION(create);
        
    JS_DECLARE_NATIVE_FUNCTION(prevent_silent_access);
        

};


} // namespace Web::Bindings
    