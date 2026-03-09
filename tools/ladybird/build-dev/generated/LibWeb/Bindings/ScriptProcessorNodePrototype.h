
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ScriptProcessorNodePrototype : public JS::Object {
    JS_OBJECT(ScriptProcessorNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ScriptProcessorNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ScriptProcessorNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ScriptProcessorNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onaudioprocess_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaudioprocess_setter);

    JS_DECLARE_NATIVE_FUNCTION(buffer_size_getter);


};


} // namespace Web::Bindings
    