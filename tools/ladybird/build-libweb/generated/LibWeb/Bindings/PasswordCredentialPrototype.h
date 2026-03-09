
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class PasswordCredentialPrototype : public JS::Object {
    JS_OBJECT(PasswordCredentialPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(PasswordCredentialPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit PasswordCredentialPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~PasswordCredentialPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(password_getter);

    JS_DECLARE_NATIVE_FUNCTION(name_getter);

    JS_DECLARE_NATIVE_FUNCTION(icon_url_getter);


};


} // namespace Web::Bindings
    