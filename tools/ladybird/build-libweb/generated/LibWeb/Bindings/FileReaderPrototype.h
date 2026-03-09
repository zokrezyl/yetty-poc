
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class FileReaderPrototype : public JS::Object {
    JS_OBJECT(FileReaderPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(FileReaderPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit FileReaderPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~FileReaderPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(read_as_array_buffer);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_binary_string);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_text);
        
    JS_DECLARE_NATIVE_FUNCTION(read_as_data_url);
        
    JS_DECLARE_NATIVE_FUNCTION(abort);
        
    JS_DECLARE_NATIVE_FUNCTION(ready_state_getter);

    JS_DECLARE_NATIVE_FUNCTION(result_getter);

    JS_DECLARE_NATIVE_FUNCTION(error_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadstart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadstart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onprogress_getter);

    JS_DECLARE_NATIVE_FUNCTION(onprogress_setter);

    JS_DECLARE_NATIVE_FUNCTION(onload_getter);

    JS_DECLARE_NATIVE_FUNCTION(onload_setter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onloadend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onloadend_setter);


};


} // namespace Web::Bindings
    