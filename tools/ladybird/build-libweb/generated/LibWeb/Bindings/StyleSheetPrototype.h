
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class StyleSheetPrototype : public JS::Object {
    JS_OBJECT(StyleSheetPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(StyleSheetPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit StyleSheetPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~StyleSheetPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(href_getter);

    JS_DECLARE_NATIVE_FUNCTION(owner_node_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_style_sheet_getter);

    JS_DECLARE_NATIVE_FUNCTION(title_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_getter);

    JS_DECLARE_NATIVE_FUNCTION(disabled_setter);


};


} // namespace Web::Bindings
    