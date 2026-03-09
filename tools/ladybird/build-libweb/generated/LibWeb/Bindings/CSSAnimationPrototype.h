
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSAnimationPrototype : public JS::Object {
    JS_OBJECT(CSSAnimationPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSAnimationPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSAnimationPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSAnimationPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(animation_name_getter);


};


} // namespace Web::Bindings
    