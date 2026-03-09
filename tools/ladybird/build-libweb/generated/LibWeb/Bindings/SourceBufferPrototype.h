
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SourceBufferPrototype : public JS::Object {
    JS_OBJECT(SourceBufferPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SourceBufferPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SourceBufferPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SourceBufferPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onupdatestart_getter);

    JS_DECLARE_NATIVE_FUNCTION(onupdatestart_setter);

    JS_DECLARE_NATIVE_FUNCTION(onupdate_getter);

    JS_DECLARE_NATIVE_FUNCTION(onupdate_setter);

    JS_DECLARE_NATIVE_FUNCTION(onupdateend_getter);

    JS_DECLARE_NATIVE_FUNCTION(onupdateend_setter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_getter);

    JS_DECLARE_NATIVE_FUNCTION(onerror_setter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_getter);

    JS_DECLARE_NATIVE_FUNCTION(onabort_setter);


};


enum class AppendMode : u8 {

    Segments,

    Sequence,

};

inline String idl_enum_to_string(AppendMode value)
{
    switch (value) {

    case AppendMode::Segments:
        return "segments"_string;

    case AppendMode::Sequence:
        return "sequence"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    