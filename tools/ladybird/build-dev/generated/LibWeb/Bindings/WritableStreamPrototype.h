
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WritableStreamPrototype : public JS::Object {
    JS_OBJECT(WritableStreamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WritableStreamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WritableStreamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WritableStreamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(get_writer);
        
    JS_DECLARE_NATIVE_FUNCTION(locked_getter);


};


} // namespace Web::Bindings
    