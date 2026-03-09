
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class CloseWatcherPrototype : public JS::Object {
    JS_OBJECT(CloseWatcherPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(CloseWatcherPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit CloseWatcherPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~CloseWatcherPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(request_close);
        
    JS_DECLARE_NATIVE_FUNCTION(close);
        
    JS_DECLARE_NATIVE_FUNCTION(destroy);
        
    JS_DECLARE_NATIVE_FUNCTION(oncancel_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncancel_setter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_getter);

    JS_DECLARE_NATIVE_FUNCTION(onclose_setter);


};


} // namespace Web::Bindings
    