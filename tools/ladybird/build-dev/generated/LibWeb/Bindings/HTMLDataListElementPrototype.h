
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLDataListElementPrototype : public JS::Object {
    JS_OBJECT(HTMLDataListElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLDataListElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLDataListElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLDataListElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(options_getter);


};


} // namespace Web::Bindings
    