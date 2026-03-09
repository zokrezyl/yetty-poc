
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WebAssemblyNamespace final : public JS::Object {
    JS_OBJECT(WebAssemblyNamespace, JS::Object);
    GC_DECLARE_ALLOCATOR(WebAssemblyNamespace);
public:
    explicit WebAssemblyNamespace(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WebAssemblyNamespace() override;

private:

    virtual void visit_edges(JS::Cell::Visitor&) override;

public:
    static constexpr bool OVERRIDES_FINALIZE = true;

private:
    virtual void finalize() override;

    JS_DECLARE_NATIVE_FUNCTION(validate);

    JS_DECLARE_NATIVE_FUNCTION(compile);

    JS_DECLARE_NATIVE_FUNCTION(compile_streaming);

    JS_DECLARE_NATIVE_FUNCTION(instantiate);

    JS_DECLARE_NATIVE_FUNCTION(instantiate0);

    JS_DECLARE_NATIVE_FUNCTION(instantiate1);

    JS_DECLARE_NATIVE_FUNCTION(instantiate_streaming);

};

} // namespace Web::Bindings
