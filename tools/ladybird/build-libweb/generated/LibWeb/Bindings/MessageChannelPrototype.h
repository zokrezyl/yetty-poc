
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MessageChannelPrototype : public JS::Object {
    JS_OBJECT(MessageChannelPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MessageChannelPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MessageChannelPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MessageChannelPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(port1_getter);

    JS_DECLARE_NATIVE_FUNCTION(port2_getter);


};


} // namespace Web::Bindings
    