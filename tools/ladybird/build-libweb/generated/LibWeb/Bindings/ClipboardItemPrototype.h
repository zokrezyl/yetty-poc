
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ClipboardItemPrototype : public JS::Object {
    JS_OBJECT(ClipboardItemPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ClipboardItemPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ClipboardItemPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ClipboardItemPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_type);
        
    JS_DECLARE_NATIVE_FUNCTION(presentation_style_getter);

    JS_DECLARE_NATIVE_FUNCTION(types_getter);


};


enum class PresentationStyle : u8 {

    Unspecified,

    Inline,

    Attachment,

};

inline String idl_enum_to_string(PresentationStyle value)
{
    switch (value) {

    case PresentationStyle::Unspecified:
        return "unspecified"_string;

    case PresentationStyle::Inline:
        return "inline"_string;

    case PresentationStyle::Attachment:
        return "attachment"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    