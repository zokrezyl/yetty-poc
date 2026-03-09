
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class MutationRecordPrototype : public JS::Object {
    JS_OBJECT(MutationRecordPrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(MutationRecordPrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit MutationRecordPrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~MutationRecordPrototype() override;
private:

    JS_DECLARE_NATIVE_FUNCTION(type_getter);

    JS_DECLARE_NATIVE_FUNCTION(target_getter);

    JS_DECLARE_NATIVE_FUNCTION(added_nodes_getter);

    JS_DECLARE_NATIVE_FUNCTION(removed_nodes_getter);

    JS_DECLARE_NATIVE_FUNCTION(previous_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(next_sibling_getter);

    JS_DECLARE_NATIVE_FUNCTION(attribute_name_getter);

    JS_DECLARE_NATIVE_FUNCTION(attribute_namespace_getter);

    JS_DECLARE_NATIVE_FUNCTION(old_value_getter);


};


} // namespace Web::Bindings
    