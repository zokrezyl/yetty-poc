
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class OfflineAudioContextPrototype : public JS::Object {
    JS_OBJECT(OfflineAudioContextPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(OfflineAudioContextPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit OfflineAudioContextPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~OfflineAudioContextPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(start_rendering);
        
    JS_DECLARE_NATIVE_FUNCTION(resume);
        
    JS_DECLARE_NATIVE_FUNCTION(suspend);
        
    JS_DECLARE_NATIVE_FUNCTION(length_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncomplete_getter);

    JS_DECLARE_NATIVE_FUNCTION(oncomplete_setter);


};


} // namespace Web::Bindings
    