
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLModElementPrototype : public JS::Object {
    JS_OBJECT(HTMLModElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLModElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLModElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLModElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cite_getter);

    JS_DECLARE_NATIVE_FUNCTION(cite_setter);

    JS_DECLARE_NATIVE_FUNCTION(date_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(date_time_setter);


};


} // namespace Web::Bindings
    