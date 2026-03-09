
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class TextMetricsPrototype : public JS::Object {
    JS_OBJECT(TextMetricsPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(TextMetricsPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit TextMetricsPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~TextMetricsPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(actual_bounding_box_left_getter);

    JS_DECLARE_NATIVE_FUNCTION(actual_bounding_box_right_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_bounding_box_ascent_getter);

    JS_DECLARE_NATIVE_FUNCTION(font_bounding_box_descent_getter);

    JS_DECLARE_NATIVE_FUNCTION(actual_bounding_box_ascent_getter);

    JS_DECLARE_NATIVE_FUNCTION(actual_bounding_box_descent_getter);

    JS_DECLARE_NATIVE_FUNCTION(em_height_ascent_getter);

    JS_DECLARE_NATIVE_FUNCTION(em_height_descent_getter);

    JS_DECLARE_NATIVE_FUNCTION(hanging_baseline_getter);

    JS_DECLARE_NATIVE_FUNCTION(alphabetic_baseline_getter);

    JS_DECLARE_NATIVE_FUNCTION(ideographic_baseline_getter);


};


} // namespace Web::Bindings
    