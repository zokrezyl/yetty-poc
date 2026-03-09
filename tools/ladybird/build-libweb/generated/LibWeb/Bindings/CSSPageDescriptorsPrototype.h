
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSPageDescriptorsPrototype : public JS::Object {
    JS_OBJECT(CSSPageDescriptorsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSPageDescriptorsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSPageDescriptorsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSPageDescriptorsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(margin_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_top_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_right_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_bottom_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(margin_left_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(size_getter);

    JS_DECLARE_NATIVE_FUNCTION(size_setter);

    JS_DECLARE_NATIVE_FUNCTION(page_orientation_regular_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_orientation_regular_setter);

    JS_DECLARE_NATIVE_FUNCTION(page_orientation_dashed_getter);

    JS_DECLARE_NATIVE_FUNCTION(page_orientation_dashed_setter);

    JS_DECLARE_NATIVE_FUNCTION(marks_getter);

    JS_DECLARE_NATIVE_FUNCTION(marks_setter);

    JS_DECLARE_NATIVE_FUNCTION(bleed_getter);

    JS_DECLARE_NATIVE_FUNCTION(bleed_setter);


};


} // namespace Web::Bindings
    