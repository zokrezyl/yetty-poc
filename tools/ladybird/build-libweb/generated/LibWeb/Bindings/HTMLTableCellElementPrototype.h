
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTableCellElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTableCellElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTableCellElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTableCellElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTableCellElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(col_span_getter);

    JS_DECLARE_NATIVE_FUNCTION(col_span_setter);

    JS_DECLARE_NATIVE_FUNCTION(row_span_getter);

    JS_DECLARE_NATIVE_FUNCTION(row_span_setter);

    JS_DECLARE_NATIVE_FUNCTION(headers_getter);

    JS_DECLARE_NATIVE_FUNCTION(headers_setter);

    JS_DECLARE_NATIVE_FUNCTION(cell_index_getter);

    JS_DECLARE_NATIVE_FUNCTION(scope_getter);

    JS_DECLARE_NATIVE_FUNCTION(scope_setter);

    JS_DECLARE_NATIVE_FUNCTION(abbr_getter);

    JS_DECLARE_NATIVE_FUNCTION(abbr_setter);

    JS_DECLARE_NATIVE_FUNCTION(align_getter);

    JS_DECLARE_NATIVE_FUNCTION(align_setter);

    JS_DECLARE_NATIVE_FUNCTION(axis_getter);

    JS_DECLARE_NATIVE_FUNCTION(axis_setter);

    JS_DECLARE_NATIVE_FUNCTION(height_getter);

    JS_DECLARE_NATIVE_FUNCTION(height_setter);

    JS_DECLARE_NATIVE_FUNCTION(width_getter);

    JS_DECLARE_NATIVE_FUNCTION(width_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_setter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_getter);

    JS_DECLARE_NATIVE_FUNCTION(ch_off_setter);

    JS_DECLARE_NATIVE_FUNCTION(no_wrap_getter);

    JS_DECLARE_NATIVE_FUNCTION(no_wrap_setter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_getter);

    JS_DECLARE_NATIVE_FUNCTION(v_align_setter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_getter);

    JS_DECLARE_NATIVE_FUNCTION(bg_color_setter);


};


enum class ScopeAttribute : u8 {

    Row,

    Col,

    Rowgroup,

    Colgroup,

};

inline String idl_enum_to_string(ScopeAttribute value)
{
    switch (value) {

    case ScopeAttribute::Row:
        return "row"_string;

    case ScopeAttribute::Col:
        return "col"_string;

    case ScopeAttribute::Rowgroup:
        return "rowgroup"_string;

    case ScopeAttribute::Colgroup:
        return "colgroup"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    