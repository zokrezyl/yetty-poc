
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class SourceBufferListPrototype : public JS::Object {
    JS_OBJECT(SourceBufferListPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(SourceBufferListPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit SourceBufferListPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~SourceBufferListPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(onaddsourcebuffer_getter);

    JS_DECLARE_NATIVE_FUNCTION(onaddsourcebuffer_setter);

    JS_DECLARE_NATIVE_FUNCTION(onremovesourcebuffer_getter);

    JS_DECLARE_NATIVE_FUNCTION(onremovesourcebuffer_setter);


};


} // namespace Web::Bindings
    