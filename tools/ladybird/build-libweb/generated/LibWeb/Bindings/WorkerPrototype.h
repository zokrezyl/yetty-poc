
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WorkerPrototype : public JS::Object {
    JS_OBJECT(WorkerPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WorkerPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WorkerPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WorkerPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(terminate);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message);
        
    JS_DECLARE_NATIVE_FUNCTION(post_message0);

    JS_DECLARE_NATIVE_FUNCTION(post_message1);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessage_setter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onmessageerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);


};


enum class WorkerType : u8 {

    Classic,

    Module,

};

inline String idl_enum_to_string(WorkerType value)
{
    switch (value) {

    case WorkerType::Classic:
        return "classic"_string;

    case WorkerType::Module:
        return "module"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    