
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MediaSourcePrototype : public JS::Object {
    JS_OBJECT(MediaSourcePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MediaSourcePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MediaSourcePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MediaSourcePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onsourceopen_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsourceopen_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsourceended_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsourceended_setter);

    JS_DECLARE_NATIVE_FUNCTION(onsourceclose_getter);

    JS_DECLARE_NATIVE_FUNCTION(onsourceclose_setter);


};


enum class ReadyState : u8 {

    Closed,

    Open,

    Ended,

};

inline String idl_enum_to_string(ReadyState value)
{
    switch (value) {

    case ReadyState::Closed:
        return "closed"_string;

    case ReadyState::Open:
        return "open"_string;

    case ReadyState::Ended:
        return "ended"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class EndOfStreamError : u8 {

    Network,

    Decode,

};

inline String idl_enum_to_string(EndOfStreamError value)
{
    switch (value) {

    case EndOfStreamError::Network:
        return "network"_string;

    case EndOfStreamError::Decode:
        return "decode"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    