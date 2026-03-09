
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ScrollTimelinePrototype : public JS::Object {
    JS_OBJECT(ScrollTimelinePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ScrollTimelinePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ScrollTimelinePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ScrollTimelinePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(source_getter);

    JS_DECLARE_NATIVE_FUNCTION(axis_getter);


};


enum class ScrollAxis : u8 {

    Block,

    Inline,

    X,

    Y,

};

inline String idl_enum_to_string(ScrollAxis value)
{
    switch (value) {

    case ScrollAxis::Block:
        return "block"_string;

    case ScrollAxis::Inline:
        return "inline"_string;

    case ScrollAxis::X:
        return "x"_string;

    case ScrollAxis::Y:
        return "y"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    