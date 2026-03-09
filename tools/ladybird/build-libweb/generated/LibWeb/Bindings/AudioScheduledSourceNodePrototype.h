
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class AudioScheduledSourceNodePrototype : public JS::Object {
    JS_OBJECT(AudioScheduledSourceNodePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(AudioScheduledSourceNodePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit AudioScheduledSourceNodePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~AudioScheduledSourceNodePrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(start);
        
    JS_DECLARE_NATIVE_FUNCTION(stop);
        
    JS_DECLARE_NATIVE_FUNCTION(onended_getter);

    JS_DECLARE_NATIVE_FUNCTION(onended_setter);


};


} // namespace Web::Bindings
    