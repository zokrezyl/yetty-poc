
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebUIPrototype : public JS::Object {
    JS_OBJECT(WebUIPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WebUIPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WebUIPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebUIPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(send_message);
        

};


} // namespace Web::Bindings
    