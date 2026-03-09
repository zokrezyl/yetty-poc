
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSFontFaceRulePrototype : public JS::Object {
    JS_OBJECT(CSSFontFaceRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSFontFaceRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSFontFaceRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSFontFaceRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(style_getter);


};


} // namespace Web::Bindings
    