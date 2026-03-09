
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CommandEventPrototype : public JS::Object {
    JS_OBJECT(CommandEventPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CommandEventPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CommandEventPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CommandEventPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(source_getter);

    JS_DECLARE_NATIVE_FUNCTION(command_getter);


};


} // namespace Web::Bindings
    