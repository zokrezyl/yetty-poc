
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ReadableStreamPrototype : public JS::Object {
    JS_OBJECT(ReadableStreamPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ReadableStreamPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ReadableStreamPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ReadableStreamPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(cancel);
        
    JS_DECLARE_NATIVE_FUNCTION(get_reader);
        
    JS_DECLARE_NATIVE_FUNCTION(pipe_through);
        
    JS_DECLARE_NATIVE_FUNCTION(pipe_to);
        
    JS_DECLARE_NATIVE_FUNCTION(tee);
        
    JS_DECLARE_NATIVE_FUNCTION(values);
        
    JS_DECLARE_NATIVE_FUNCTION(locked_getter);


};


enum class ReadableStreamReaderMode : u8 {

    Byob,

};

inline String idl_enum_to_string(ReadableStreamReaderMode value)
{
    switch (value) {

    case ReadableStreamReaderMode::Byob:
        return "byob"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    