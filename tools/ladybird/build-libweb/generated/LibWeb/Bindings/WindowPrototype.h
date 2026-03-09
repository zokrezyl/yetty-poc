
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class WindowPrototype : public JS::Object {
    JS_OBJECT(WindowPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(WindowPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit WindowPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WindowPrototype() override;
private:

};

class WindowProperties : public JS::Object {
    JS_OBJECT(WindowProperties, JS::Object);
    GC_DECLARE_ALLOCATOR(WindowProperties);
public:
    explicit WindowProperties(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~WindowProperties() override;

    JS::Realm& realm() const { return m_realm; }
private:
    virtual JS::ThrowCompletionOr<Optional<JS::PropertyDescriptor>> internal_get_own_property(JS::PropertyKey const&) const override;
    virtual JS::ThrowCompletionOr<bool> internal_define_own_property(JS::PropertyKey const&, JS::PropertyDescriptor&, Optional<JS::PropertyDescriptor>* precomputed_get_own_property = nullptr) override;
    virtual JS::ThrowCompletionOr<bool> internal_delete(JS::PropertyKey const&) override;
    virtual JS::ThrowCompletionOr<bool> internal_set_prototype_of(JS::Object* prototype) override;
    virtual JS::ThrowCompletionOr<bool> internal_prevent_extensions() override;

    virtual bool eligible_for_own_property_enumeration_fast_path() const override final { return false; }

    virtual void visit_edges(Visitor&) override;

    GC::Ref<JS::Realm> m_realm; // [[Realm]]
};

} // namespace Web::Bindings
    