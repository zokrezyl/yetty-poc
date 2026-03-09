
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class KeyframeEffectPrototype : public JS::Object {
    JS_OBJECT(KeyframeEffectPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(KeyframeEffectPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit KeyframeEffectPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~KeyframeEffectPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(get_keyframes);
        
    JS_DECLARE_NATIVE_FUNCTION(set_keyframes);
        
    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_setter);

    JS_DECLARE_NATIVE_FUNCTION(pseudo_element_getter);

    JS_DECLARE_NATIVE_FUNCTION(pseudo_element_setter);

    JS_DECLARE_NATIVE_FUNCTION(composite_getter);

    JS_DECLARE_NATIVE_FUNCTION(composite_setter);


};


enum class CompositeOperation : u8 {

    Replace,

    Add,

    Accumulate,

};

inline String idl_enum_to_string(CompositeOperation value)
{
    switch (value) {

    case CompositeOperation::Replace:
        return "replace"_string;

    case CompositeOperation::Add:
        return "add"_string;

    case CompositeOperation::Accumulate:
        return "accumulate"_string;

    }
    VERIFY_NOT_REACHED();
}

enum class CompositeOperationOrAuto : u8 {

    Replace,

    Add,

    Accumulate,

    Auto,

};

inline String idl_enum_to_string(CompositeOperationOrAuto value)
{
    switch (value) {

    case CompositeOperationOrAuto::Replace:
        return "replace"_string;

    case CompositeOperationOrAuto::Add:
        return "add"_string;

    case CompositeOperationOrAuto::Accumulate:
        return "accumulate"_string;

    case CompositeOperationOrAuto::Auto:
        return "auto"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    