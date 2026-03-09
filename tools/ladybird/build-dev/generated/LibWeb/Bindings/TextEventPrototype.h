
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextEventPrototype : public JS::Object {
    JS_OBJECT(TextEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(init_text_event);
        
    JS_DECLARE_NATIVE_FUNCTION(data_getter);


};


} // namespace Web::Bindings
    