
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class HTMLAudioElementPrototype : public JS::Object {
    JS_OBJECT(HTMLAudioElementPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(HTMLAudioElementPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit HTMLAudioElementPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~HTMLAudioElementPrototype() override;
private:


};


} // namespace Web::Bindings
    