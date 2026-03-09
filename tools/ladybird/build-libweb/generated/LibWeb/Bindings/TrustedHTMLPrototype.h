
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TrustedHTMLPrototype : public JS::Object {
    JS_OBJECT(TrustedHTMLPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TrustedHTMLPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TrustedHTMLPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TrustedHTMLPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(to_json);
        
    JS_DECLARE_NATIVE_FUNCTION(to_string);
        

};


} // namespace Web::Bindings
    