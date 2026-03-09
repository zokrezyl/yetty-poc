
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLTemplateElementPrototype : public JS::Object {
    JS_OBJECT(HTMLTemplateElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLTemplateElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLTemplateElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLTemplateElementPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(content_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_mode_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_mode_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_delegates_focus_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_delegates_focus_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_clonable_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_clonable_setter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_serializable_getter);

    JS_DECLARE_NATIVE_FUNCTION(shadow_root_serializable_setter);


};


} // namespace Web::Bindings
    