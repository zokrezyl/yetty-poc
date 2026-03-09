
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CSSMediaRulePrototype : public JS::Object {
    JS_OBJECT(CSSMediaRulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CSSMediaRulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CSSMediaRulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CSSMediaRulePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(media_getter);

    JS_DECLARE_NATIVE_FUNCTION(media_setter);

    JS_DECLARE_NATIVE_FUNCTION(matches_getter);


};


} // namespace Web::Bindings
    