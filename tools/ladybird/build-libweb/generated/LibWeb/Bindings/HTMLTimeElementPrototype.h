
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTimeElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTimeElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTimeElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTimeElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTimeElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(date_time_getter);

    JS_DECLARE_NATIVE_FUNCTION(date_time_setter);


};


} // namespace Web::Bindings
    