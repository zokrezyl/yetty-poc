
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FormDataEventPrototype : public JS::Object {
    JS_OBJECT(FormDataEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FormDataEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FormDataEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FormDataEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(form_data_getter);


};


} // namespace Web::Bindings
    