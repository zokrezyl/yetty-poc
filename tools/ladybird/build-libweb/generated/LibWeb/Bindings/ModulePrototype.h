
#pragma once

#include <LibJS/Runtime/Object.h>

namespace Web::Bindings {

class ModulePrototype : public JS::Object {
    JS_OBJECT(ModulePrototype, JS::Object);
    GC_DECLARE_ALLOCATOR(ModulePrototype);
public:
    static void define_unforgeable_attributes(JS::Realm&, JS::Object&);

    explicit ModulePrototype(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
    virtual ~ModulePrototype() override;
private:


};


enum class ImportExportKind : u8 {

    Function,

    Table,

    Memory,

    Global,

};

inline String idl_enum_to_string(ImportExportKind value)
{
    switch (value) {

    case ImportExportKind::Function:
        return "function"_string;

    case ImportExportKind::Table:
        return "table"_string;

    case ImportExportKind::Memory:
        return "memory"_string;

    case ImportExportKind::Global:
        return "global"_string;

    }
    VERIFY_NOT_REACHED();
}

} // namespace Web::Bindings
    