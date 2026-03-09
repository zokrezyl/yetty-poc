
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FederatedCredentialPrototype : public JS::Object {
    JS_OBJECT(FederatedCredentialPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FederatedCredentialPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FederatedCredentialPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FederatedCredentialPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(provider_getter);

    JS_DECLARE_NATIVE_FUNCTION(protocol_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(icon_url_getter);


};


} // namespace Web::Bindings
    