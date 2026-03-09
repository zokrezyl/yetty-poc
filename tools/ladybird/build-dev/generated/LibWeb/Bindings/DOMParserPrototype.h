
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class DOMParserPrototype : public JS::Object {
    JS_OBJECT(DOMParserPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(DOMParserPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit DOMParserPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~DOMParserPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(parse_from_string);
        

};


enum class DOMParserSupportedType : u8 {

    Text_Html,

    Text_Xml,

    Application_Xml,

    Application_Xhtml_Xml,

    Image_Svg_Xml,

};

inline String idl_enum_to_string(DOMParserSupportedType value)
{
    switch (value) {

    case DOMParserSupportedType::Text_Html:
        return "text/html"_string;

    case DOMParserSupportedType::Text_Xml:
        return "text/xml"_string;

    case DOMParserSupportedType::Application_Xml:
        return "application/xml"_string;

    case DOMParserSupportedType::Application_Xhtml_Xml:
        return "application/xhtml+xml"_string;

    case DOMParserSupportedType::Image_Svg_Xml:
        return "image/svg+xml"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    