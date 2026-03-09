
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaErrorPrototype : public JS::Object {
    JS_OBJECT(MediaErrorPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaErrorPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaErrorPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaErrorPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(code_getter);

    JS_DECLARE_NATIVE_FUNCTION(message_getter);


};


} // namespace Web::Bindings
    