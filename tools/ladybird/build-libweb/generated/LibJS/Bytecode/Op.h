#pragma once

#include <AK/Span.h>
#include <AK/StdLibExtras.h>
#include <LibJS/Bytecode/Builtins.h>
#include <LibJS/Bytecode/IdentifierTable.h>
#include <LibJS/Bytecode/Instruction.h>
#include <LibJS/Bytecode/Label.h>
#include <LibJS/Bytecode/Operand.h>
#include <LibJS/Bytecode/PutKind.h>
#include <LibJS/Bytecode/RegexTable.h>
#include <LibJS/Bytecode/Register.h>
#include <LibJS/Bytecode/ScopedOperand.h>
#include <LibJS/Bytecode/StringTable.h>
#include <LibJS/Runtime/BigInt.h>
#include <LibJS/Runtime/Environment.h>
#include <LibJS/Runtime/Iterator.h>
#include <LibJS/Runtime/Value.h>

namespace JS::Bytecode::Op {

class Add final : public Instruction {
public:
    Add(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Add)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Add>);

class AddPrivateName final : public Instruction {
public:
    AddPrivateName(IdentifierTableIndex name)
        : Instruction(Type::AddPrivateName)
        , m_name(name)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

    auto const& name() const { return m_name; }

private:
    IdentifierTableIndex m_name;
};
static_assert(IsTriviallyDestructible<AddPrivateName>);

class ArrayAppend final : public Instruction {
public:
    ArrayAppend(Operand dst, Operand src, bool is_spread)
        : Instruction(Type::ArrayAppend)
        , m_dst(dst)
        , m_src(src)
        , m_is_spread(is_spread)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }
    auto const& is_spread() const { return m_is_spread; }

private:
    Operand m_dst;
    Operand m_src;
    bool m_is_spread;
};
static_assert(IsTriviallyDestructible<ArrayAppend>);

class Await final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    Await(Label continuation_label, Operand argument)
        : Instruction(Type::Await)
        , m_continuation_label(continuation_label)
        , m_argument(argument)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_argument);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_continuation_label);
    }

    auto const& continuation_label() const { return m_continuation_label; }
    auto const& argument() const { return m_argument; }

private:
    Label m_continuation_label;
    Operand m_argument;
};
static_assert(IsTriviallyDestructible<Await>);

class BitwiseAnd final : public Instruction {
public:
    BitwiseAnd(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::BitwiseAnd)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<BitwiseAnd>);

class BitwiseNot final : public Instruction {
public:
    BitwiseNot(Operand dst, Operand src)
        : Instruction(Type::BitwiseNot)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<BitwiseNot>);

class BitwiseOr final : public Instruction {
public:
    BitwiseOr(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::BitwiseOr)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<BitwiseOr>);

class ToInt32 final : public Instruction {
public:
    ToInt32(Operand dst, Operand value)
        : Instruction(Type::ToInt32)
        , m_dst(dst)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToInt32>);

class ToString final : public Instruction {
public:
    ToString(Operand dst, Operand value)
        : Instruction(Type::ToString)
        , m_dst(dst)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToString>);

class ToPrimitiveWithStringHint final : public Instruction {
public:
    ToPrimitiveWithStringHint(Operand dst, Operand value)
        : Instruction(Type::ToPrimitiveWithStringHint)
        , m_dst(dst)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToPrimitiveWithStringHint>);

class BitwiseXor final : public Instruction {
public:
    BitwiseXor(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::BitwiseXor)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<BitwiseXor>);

class Call final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    Call(Operand dst, Operand callee, Operand this_value, Optional<StringTableIndex> expression_string, ReadonlySpan<ScopedOperand> arguments)
        : Instruction(Type::Call)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * arguments.size()))
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_argument_count(arguments.size())
        , m_expression_string(expression_string)
    {
        for (size_t i = 0; i < arguments.size(); ++i)
            m_arguments[i] = arguments[i];
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        for (size_t i = 0; i < m_argument_count; ++i)
            visitor(m_arguments[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& argument_count() const { return m_argument_count; }
    auto const& expression_string() const { return m_expression_string; }
    ReadonlySpan<Operand> arguments() const { return ReadonlySpan<Operand> { m_arguments, m_argument_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    u32 m_argument_count;
    Optional<StringTableIndex> m_expression_string;
    Operand m_arguments[];
};
static_assert(IsTriviallyDestructible<Call>);

class CallBuiltin final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    CallBuiltin(Operand dst, Operand callee, Operand this_value, Builtin builtin, Optional<StringTableIndex> expression_string, ReadonlySpan<ScopedOperand> arguments)
        : Instruction(Type::CallBuiltin)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * arguments.size()))
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_argument_count(arguments.size())
        , m_builtin(builtin)
        , m_expression_string(expression_string)
    {
        for (size_t i = 0; i < arguments.size(); ++i)
            m_arguments[i] = arguments[i];
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        for (size_t i = 0; i < m_argument_count; ++i)
            visitor(m_arguments[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& argument_count() const { return m_argument_count; }
    auto const& builtin() const { return m_builtin; }
    auto const& expression_string() const { return m_expression_string; }
    ReadonlySpan<Operand> arguments() const { return ReadonlySpan<Operand> { m_arguments, m_argument_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    u32 m_argument_count;
    Builtin m_builtin;
    Optional<StringTableIndex> m_expression_string;
    Operand m_arguments[];
};
static_assert(IsTriviallyDestructible<CallBuiltin>);

class CallConstruct final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    CallConstruct(Operand dst, Operand callee, Optional<StringTableIndex> expression_string, ReadonlySpan<ScopedOperand> arguments)
        : Instruction(Type::CallConstruct)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * arguments.size()))
        , m_dst(dst)
        , m_callee(callee)
        , m_argument_count(arguments.size())
        , m_expression_string(expression_string)
    {
        for (size_t i = 0; i < arguments.size(); ++i)
            m_arguments[i] = arguments[i];
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        for (size_t i = 0; i < m_argument_count; ++i)
            visitor(m_arguments[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& argument_count() const { return m_argument_count; }
    auto const& expression_string() const { return m_expression_string; }
    ReadonlySpan<Operand> arguments() const { return ReadonlySpan<Operand> { m_arguments, m_argument_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Operand m_callee;
    u32 m_argument_count;
    Optional<StringTableIndex> m_expression_string;
    Operand m_arguments[];
};
static_assert(IsTriviallyDestructible<CallConstruct>);

class CallConstructWithArgumentArray final : public Instruction {
public:
    CallConstructWithArgumentArray(Operand dst, Operand callee, Operand this_value, Operand arguments, Optional<StringTableIndex> expression_string)
        : Instruction(Type::CallConstructWithArgumentArray)
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_arguments(arguments)
        , m_expression_string(expression_string)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        visitor(m_arguments);
    }

    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& arguments() const { return m_arguments; }
    auto const& expression_string() const { return m_expression_string; }

private:
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    Operand m_arguments;
    Optional<StringTableIndex> m_expression_string;
};
static_assert(IsTriviallyDestructible<CallConstructWithArgumentArray>);

class CallDirectEval final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    CallDirectEval(Operand dst, Operand callee, Operand this_value, Optional<StringTableIndex> expression_string, ReadonlySpan<ScopedOperand> arguments)
        : Instruction(Type::CallDirectEval)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * arguments.size()))
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_argument_count(arguments.size())
        , m_expression_string(expression_string)
    {
        for (size_t i = 0; i < arguments.size(); ++i)
            m_arguments[i] = arguments[i];
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        for (size_t i = 0; i < m_argument_count; ++i)
            visitor(m_arguments[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& argument_count() const { return m_argument_count; }
    auto const& expression_string() const { return m_expression_string; }
    ReadonlySpan<Operand> arguments() const { return ReadonlySpan<Operand> { m_arguments, m_argument_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    u32 m_argument_count;
    Optional<StringTableIndex> m_expression_string;
    Operand m_arguments[];
};
static_assert(IsTriviallyDestructible<CallDirectEval>);

class CallDirectEvalWithArgumentArray final : public Instruction {
public:
    CallDirectEvalWithArgumentArray(Operand dst, Operand callee, Operand this_value, Operand arguments, Optional<StringTableIndex> expression_string)
        : Instruction(Type::CallDirectEvalWithArgumentArray)
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_arguments(arguments)
        , m_expression_string(expression_string)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        visitor(m_arguments);
    }

    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& arguments() const { return m_arguments; }
    auto const& expression_string() const { return m_expression_string; }

private:
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    Operand m_arguments;
    Optional<StringTableIndex> m_expression_string;
};
static_assert(IsTriviallyDestructible<CallDirectEvalWithArgumentArray>);

class CallWithArgumentArray final : public Instruction {
public:
    CallWithArgumentArray(Operand dst, Operand callee, Operand this_value, Operand arguments, Optional<StringTableIndex> expression_string)
        : Instruction(Type::CallWithArgumentArray)
        , m_dst(dst)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_arguments(arguments)
        , m_expression_string(expression_string)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_callee);
        visitor(m_this_value);
        visitor(m_arguments);
    }

    auto const& dst() const { return m_dst; }
    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& arguments() const { return m_arguments; }
    auto const& expression_string() const { return m_expression_string; }

private:
    Operand m_dst;
    Operand m_callee;
    Operand m_this_value;
    Operand m_arguments;
    Optional<StringTableIndex> m_expression_string;
};
static_assert(IsTriviallyDestructible<CallWithArgumentArray>);

class Catch final : public Instruction {
public:
    Catch(Operand dst)
        : Instruction(Type::Catch)
        , m_dst(dst)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<Catch>);

class ConcatString final : public Instruction {
public:
    ConcatString(Operand dst, Operand src)
        : Instruction(Type::ConcatString)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<ConcatString>);

class CopyObjectExcludingProperties final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    CopyObjectExcludingProperties(Operand dst, Operand from_object, ReadonlySpan<ScopedOperand> excluded_names)
        : Instruction(Type::CopyObjectExcludingProperties)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * excluded_names.size()))
        , m_dst(dst)
        , m_from_object(from_object)
        , m_excluded_names_count(excluded_names.size())
    {
        for (size_t i = 0; i < excluded_names.size(); ++i)
            m_excluded_names[i] = excluded_names[i];
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_from_object);
        for (size_t i = 0; i < m_excluded_names_count; ++i)
            visitor(m_excluded_names[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& from_object() const { return m_from_object; }
    auto const& excluded_names_count() const { return m_excluded_names_count; }
    ReadonlySpan<Operand> excluded_names() const { return ReadonlySpan<Operand> { m_excluded_names, m_excluded_names_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Operand m_from_object;
    u32 m_excluded_names_count;
    Operand m_excluded_names[];
};
static_assert(IsTriviallyDestructible<CopyObjectExcludingProperties>);

class CreateArguments final : public Instruction {
public:
    CreateArguments(Optional<Operand> dst, ArgumentsKind kind, bool is_immutable)
        : Instruction(Type::CreateArguments)
        , m_dst(dst)
        , m_kind(kind)
        , m_is_immutable(is_immutable)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        if (m_dst.has_value())
            visitor(m_dst.value());
    }

    auto const& dst() const { return m_dst; }
    auto const& kind() const { return m_kind; }
    auto const& is_immutable() const { return m_is_immutable; }

private:
    Optional<Operand> m_dst;
    ArgumentsKind m_kind;
    bool m_is_immutable;
};
static_assert(IsTriviallyDestructible<CreateArguments>);

class CreateAsyncFromSyncIterator final : public Instruction {
public:
    CreateAsyncFromSyncIterator(Operand dst, Operand iterator, Operand next_method, Operand done)
        : Instruction(Type::CreateAsyncFromSyncIterator)
        , m_dst(dst)
        , m_iterator(iterator)
        , m_next_method(next_method)
        , m_done(done)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_iterator);
        visitor(m_next_method);
        visitor(m_done);
    }

    auto const& dst() const { return m_dst; }
    auto const& iterator() const { return m_iterator; }
    auto const& next_method() const { return m_next_method; }
    auto const& done() const { return m_done; }

private:
    Operand m_dst;
    Operand m_iterator;
    Operand m_next_method;
    Operand m_done;
};
static_assert(IsTriviallyDestructible<CreateAsyncFromSyncIterator>);

class CreateDataPropertyOrThrow final : public Instruction {
public:
    CreateDataPropertyOrThrow(Operand object, Operand property, Operand value)
        : Instruction(Type::CreateDataPropertyOrThrow)
        , m_object(object)
        , m_property(property)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_object);
        visitor(m_property);
        visitor(m_value);
    }

    auto const& object() const { return m_object; }
    auto const& property() const { return m_property; }
    auto const& value() const { return m_value; }

private:
    Operand m_object;
    Operand m_property;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<CreateDataPropertyOrThrow>);

class CreateLexicalEnvironment final : public Instruction {
public:
    CreateLexicalEnvironment(Operand dst, Operand parent, u32 capacity)
        : Instruction(Type::CreateLexicalEnvironment)
        , m_dst(dst)
        , m_parent(parent)
        , m_capacity(capacity)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_parent);
    }

    auto const& dst() const { return m_dst; }
    auto const& parent() const { return m_parent; }
    auto const& capacity() const { return m_capacity; }

private:
    Operand m_dst;
    Operand m_parent;
    u32 m_capacity;
};
static_assert(IsTriviallyDestructible<CreateLexicalEnvironment>);

class CreateImmutableBinding final : public Instruction {
public:
    CreateImmutableBinding(Operand environment, IdentifierTableIndex identifier, bool strict_binding)
        : Instruction(Type::CreateImmutableBinding)
        , m_environment(environment)
        , m_identifier(identifier)
        , m_strict_binding(strict_binding)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_environment);
    }

    auto const& environment() const { return m_environment; }
    auto const& identifier() const { return m_identifier; }
    auto const& strict_binding() const { return m_strict_binding; }

private:
    Operand m_environment;
    IdentifierTableIndex m_identifier;
    bool m_strict_binding;
};
static_assert(IsTriviallyDestructible<CreateImmutableBinding>);

class CreateMutableBinding final : public Instruction {
public:
    CreateMutableBinding(Operand environment, IdentifierTableIndex identifier, bool can_be_deleted)
        : Instruction(Type::CreateMutableBinding)
        , m_environment(environment)
        , m_identifier(identifier)
        , m_can_be_deleted(can_be_deleted)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_environment);
    }

    auto const& environment() const { return m_environment; }
    auto const& identifier() const { return m_identifier; }
    auto const& can_be_deleted() const { return m_can_be_deleted; }

private:
    Operand m_environment;
    IdentifierTableIndex m_identifier;
    bool m_can_be_deleted;
};
static_assert(IsTriviallyDestructible<CreateMutableBinding>);

class CreatePrivateEnvironment final : public Instruction {
public:
    CreatePrivateEnvironment()
        : Instruction(Type::CreatePrivateEnvironment)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

private:
};
static_assert(IsTriviallyDestructible<CreatePrivateEnvironment>);

class CreateRestParams final : public Instruction {
public:
    CreateRestParams(Operand dst, u32 rest_index)
        : Instruction(Type::CreateRestParams)
        , m_dst(dst)
        , m_rest_index(rest_index)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& rest_index() const { return m_rest_index; }

private:
    Operand m_dst;
    u32 m_rest_index;
};
static_assert(IsTriviallyDestructible<CreateRestParams>);

class CreateVariable final : public Instruction {
public:
    CreateVariable(IdentifierTableIndex identifier, EnvironmentMode mode, bool is_immutable, bool is_global, bool is_strict)
        : Instruction(Type::CreateVariable)
        , m_identifier(identifier)
        , m_mode(mode)
        , m_is_immutable(is_immutable)
        , m_is_global(is_global)
        , m_is_strict(is_strict)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

    auto const& identifier() const { return m_identifier; }
    auto const& mode() const { return m_mode; }
    auto const& is_immutable() const { return m_is_immutable; }
    auto const& is_global() const { return m_is_global; }
    auto const& is_strict() const { return m_is_strict; }

private:
    IdentifierTableIndex m_identifier;
    EnvironmentMode m_mode;
    bool m_is_immutable;
    bool m_is_global;
    bool m_is_strict;
};
static_assert(IsTriviallyDestructible<CreateVariable>);

class CreateVariableEnvironment final : public Instruction {
public:
    CreateVariableEnvironment(u32 capacity)
        : Instruction(Type::CreateVariableEnvironment)
        , m_capacity(capacity)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

    auto const& capacity() const { return m_capacity; }

private:
    u32 m_capacity;
};
static_assert(IsTriviallyDestructible<CreateVariableEnvironment>);

class Decrement final : public Instruction {
public:
    Decrement(Operand dst)
        : Instruction(Type::Decrement)
        , m_dst(dst)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<Decrement>);

class DeleteById final : public Instruction {
public:
    DeleteById(Operand dst, Operand base, PropertyKeyTableIndex property)
        : Instruction(Type::DeleteById)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }

private:
    Operand m_dst;
    Operand m_base;
    PropertyKeyTableIndex m_property;
};
static_assert(IsTriviallyDestructible<DeleteById>);

class DeleteByValue final : public Instruction {
public:
    DeleteByValue(Operand dst, Operand base, Operand property)
        : Instruction(Type::DeleteByValue)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
        visitor(m_property);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }

private:
    Operand m_dst;
    Operand m_base;
    Operand m_property;
};
static_assert(IsTriviallyDestructible<DeleteByValue>);

class DeleteVariable final : public Instruction {
public:
    DeleteVariable(Operand dst, IdentifierTableIndex identifier)
        : Instruction(Type::DeleteVariable)
        , m_dst(dst)
        , m_identifier(identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& identifier() const { return m_identifier; }

private:
    Operand m_dst;
    IdentifierTableIndex m_identifier;
};
static_assert(IsTriviallyDestructible<DeleteVariable>);

class Div final : public Instruction {
public:
    Div(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Div)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Div>);

class End final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    End(Operand value)
        : Instruction(Type::End)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_value);
    }

    auto const& value() const { return m_value; }

private:
    Operand m_value;
};
static_assert(IsTriviallyDestructible<End>);

class EnterObjectEnvironment final : public Instruction {
public:
    EnterObjectEnvironment(Operand dst, Operand object)
        : Instruction(Type::EnterObjectEnvironment)
        , m_dst(dst)
        , m_object(object)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_object);
    }

    auto const& dst() const { return m_dst; }
    auto const& object() const { return m_object; }

private:
    Operand m_dst;
    Operand m_object;
};
static_assert(IsTriviallyDestructible<EnterObjectEnvironment>);

class Exp final : public Instruction {
public:
    Exp(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Exp)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Exp>);

class GetById final : public Instruction {
public:
    GetById(Operand dst, Operand base, PropertyKeyTableIndex property, Optional<IdentifierTableIndex> base_identifier, u32 cache_index)
        : Instruction(Type::GetById)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
        , m_base_identifier(base_identifier)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& base_identifier() const { return m_base_identifier; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    Operand m_base;
    PropertyKeyTableIndex m_property;
    Optional<IdentifierTableIndex> m_base_identifier;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<GetById>);

class GetByIdWithThis final : public Instruction {
public:
    GetByIdWithThis(Operand dst, Operand base, PropertyKeyTableIndex property, Operand this_value, u32 cache_index)
        : Instruction(Type::GetByIdWithThis)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
        , m_this_value(this_value)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
        visitor(m_this_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& this_value() const { return m_this_value; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    Operand m_base;
    PropertyKeyTableIndex m_property;
    Operand m_this_value;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<GetByIdWithThis>);

class GetByValue final : public Instruction {
public:
    GetByValue(Operand dst, Operand base, Operand property, Optional<IdentifierTableIndex> base_identifier)
        : Instruction(Type::GetByValue)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
        , m_base_identifier(base_identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
        visitor(m_property);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& base_identifier() const { return m_base_identifier; }

private:
    Operand m_dst;
    Operand m_base;
    Operand m_property;
    Optional<IdentifierTableIndex> m_base_identifier;
};
static_assert(IsTriviallyDestructible<GetByValue>);

class GetByValueWithThis final : public Instruction {
public:
    GetByValueWithThis(Operand dst, Operand base, Operand property, Operand this_value)
        : Instruction(Type::GetByValueWithThis)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
        , m_this_value(this_value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
        visitor(m_property);
        visitor(m_this_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& this_value() const { return m_this_value; }

private:
    Operand m_dst;
    Operand m_base;
    Operand m_property;
    Operand m_this_value;
};
static_assert(IsTriviallyDestructible<GetByValueWithThis>);

class GetCalleeAndThisFromEnvironment final : public Instruction {
public:
    GetCalleeAndThisFromEnvironment(Operand callee, Operand this_value, IdentifierTableIndex identifier)
        : Instruction(Type::GetCalleeAndThisFromEnvironment)
        , m_callee(callee)
        , m_this_value(this_value)
        , m_identifier(identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_callee);
        visitor(m_this_value);
    }

    auto const& callee() const { return m_callee; }
    auto const& this_value() const { return m_this_value; }
    auto const& identifier() const { return m_identifier; }
    auto const& cache() const { return m_cache; }

private:
    Operand m_callee;
    Operand m_this_value;
    IdentifierTableIndex m_identifier;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<GetCalleeAndThisFromEnvironment>);

class GetCompletionFields final : public Instruction {
public:
    GetCompletionFields(Operand type_dst, Operand value_dst, Operand completion)
        : Instruction(Type::GetCompletionFields)
        , m_type_dst(type_dst)
        , m_value_dst(value_dst)
        , m_completion(completion)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_type_dst);
        visitor(m_value_dst);
        visitor(m_completion);
    }

    auto const& type_dst() const { return m_type_dst; }
    auto const& value_dst() const { return m_value_dst; }
    auto const& completion() const { return m_completion; }

private:
    Operand m_type_dst;
    Operand m_value_dst;
    Operand m_completion;
};
static_assert(IsTriviallyDestructible<GetCompletionFields>);

class GetGlobal final : public Instruction {
public:
    GetGlobal(Operand dst, IdentifierTableIndex identifier, u32 cache_index)
        : Instruction(Type::GetGlobal)
        , m_dst(dst)
        , m_identifier(identifier)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& identifier() const { return m_identifier; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    IdentifierTableIndex m_identifier;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<GetGlobal>);

class GetImportMeta final : public Instruction {
public:
    GetImportMeta(Operand dst)
        : Instruction(Type::GetImportMeta)
        , m_dst(dst)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<GetImportMeta>);

class GetLexicalEnvironment final : public Instruction {
public:
    GetLexicalEnvironment(Operand dst)
        : Instruction(Type::GetLexicalEnvironment)
        , m_dst(dst)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<GetLexicalEnvironment>);

class GetIterator final : public Instruction {
public:
    GetIterator(Operand dst_iterator_object, Operand dst_iterator_next, Operand dst_iterator_done, Operand iterable, IteratorHint hint)
        : Instruction(Type::GetIterator)
        , m_dst_iterator_object(dst_iterator_object)
        , m_dst_iterator_next(dst_iterator_next)
        , m_dst_iterator_done(dst_iterator_done)
        , m_iterable(iterable)
        , m_hint(hint)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst_iterator_object);
        visitor(m_dst_iterator_next);
        visitor(m_dst_iterator_done);
        visitor(m_iterable);
    }

    auto const& dst_iterator_object() const { return m_dst_iterator_object; }
    auto const& dst_iterator_next() const { return m_dst_iterator_next; }
    auto const& dst_iterator_done() const { return m_dst_iterator_done; }
    auto const& iterable() const { return m_iterable; }
    auto const& hint() const { return m_hint; }

private:
    Operand m_dst_iterator_object;
    Operand m_dst_iterator_next;
    Operand m_dst_iterator_done;
    Operand m_iterable;
    IteratorHint m_hint;
};
static_assert(IsTriviallyDestructible<GetIterator>);

class GetLength final : public Instruction {
public:
    GetLength(Operand dst, Operand base, Optional<IdentifierTableIndex> base_identifier, u32 cache_index)
        : Instruction(Type::GetLength)
        , m_dst(dst)
        , m_base(base)
        , m_base_identifier(base_identifier)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& base_identifier() const { return m_base_identifier; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    Operand m_base;
    Optional<IdentifierTableIndex> m_base_identifier;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<GetLength>);

class GetLengthWithThis final : public Instruction {
public:
    GetLengthWithThis(Operand dst, Operand base, Operand this_value, u32 cache_index)
        : Instruction(Type::GetLengthWithThis)
        , m_dst(dst)
        , m_base(base)
        , m_this_value(this_value)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
        visitor(m_this_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& this_value() const { return m_this_value; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    Operand m_base;
    Operand m_this_value;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<GetLengthWithThis>);

class GetMethod final : public Instruction {
public:
    GetMethod(Operand dst, Operand object, PropertyKeyTableIndex property)
        : Instruction(Type::GetMethod)
        , m_dst(dst)
        , m_object(object)
        , m_property(property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_object);
    }

    auto const& dst() const { return m_dst; }
    auto const& object() const { return m_object; }
    auto const& property() const { return m_property; }

private:
    Operand m_dst;
    Operand m_object;
    PropertyKeyTableIndex m_property;
};
static_assert(IsTriviallyDestructible<GetMethod>);

class GetNewTarget final : public Instruction {
public:
    GetNewTarget(Operand dst)
        : Instruction(Type::GetNewTarget)
        , m_dst(dst)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<GetNewTarget>);

class GetObjectPropertyIterator final : public Instruction {
public:
    GetObjectPropertyIterator(Operand dst_iterator_object, Operand dst_iterator_next, Operand dst_iterator_done, Operand object)
        : Instruction(Type::GetObjectPropertyIterator)
        , m_dst_iterator_object(dst_iterator_object)
        , m_dst_iterator_next(dst_iterator_next)
        , m_dst_iterator_done(dst_iterator_done)
        , m_object(object)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst_iterator_object);
        visitor(m_dst_iterator_next);
        visitor(m_dst_iterator_done);
        visitor(m_object);
    }

    auto const& dst_iterator_object() const { return m_dst_iterator_object; }
    auto const& dst_iterator_next() const { return m_dst_iterator_next; }
    auto const& dst_iterator_done() const { return m_dst_iterator_done; }
    auto const& object() const { return m_object; }

private:
    Operand m_dst_iterator_object;
    Operand m_dst_iterator_next;
    Operand m_dst_iterator_done;
    Operand m_object;
};
static_assert(IsTriviallyDestructible<GetObjectPropertyIterator>);

class GetPrivateById final : public Instruction {
public:
    GetPrivateById(Operand dst, Operand base, IdentifierTableIndex property)
        : Instruction(Type::GetPrivateById)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }

private:
    Operand m_dst;
    Operand m_base;
    IdentifierTableIndex m_property;
};
static_assert(IsTriviallyDestructible<GetPrivateById>);

class GetTemplateObject final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    GetTemplateObject(Operand dst, u32 cache_index, ReadonlySpan<ScopedOperand> strings)
        : Instruction(Type::GetTemplateObject)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * strings.size()))
        , m_dst(dst)
        , m_strings_count(strings.size())
        , m_cache_index(cache_index)
    {
        for (size_t i = 0; i < strings.size(); ++i)
            m_strings[i] = strings[i];
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        for (size_t i = 0; i < m_strings_count; ++i)
            visitor(m_strings[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& strings_count() const { return m_strings_count; }
    auto const& cache_index() const { return m_cache_index; }
    ReadonlySpan<Operand> strings() const { return ReadonlySpan<Operand> { m_strings, m_strings_count }; }

private:
    u32 m_length;
    Operand m_dst;
    u32 m_strings_count;
    u32 m_cache_index;
    Operand m_strings[];
};
static_assert(IsTriviallyDestructible<GetTemplateObject>);

class GetBinding final : public Instruction {
public:
    GetBinding(Operand dst, IdentifierTableIndex identifier)
        : Instruction(Type::GetBinding)
        , m_dst(dst)
        , m_identifier(identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& identifier() const { return m_identifier; }
    auto const& cache() const { return m_cache; }

private:
    Operand m_dst;
    IdentifierTableIndex m_identifier;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<GetBinding>);

class GetInitializedBinding final : public Instruction {
public:
    GetInitializedBinding(Operand dst, IdentifierTableIndex identifier)
        : Instruction(Type::GetInitializedBinding)
        , m_dst(dst)
        , m_identifier(identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& identifier() const { return m_identifier; }
    auto const& cache() const { return m_cache; }

private:
    Operand m_dst;
    IdentifierTableIndex m_identifier;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<GetInitializedBinding>);

class GreaterThan final : public Instruction {
public:
    GreaterThan(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::GreaterThan)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<GreaterThan>);

class GreaterThanEquals final : public Instruction {
public:
    GreaterThanEquals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::GreaterThanEquals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<GreaterThanEquals>);

class HasPrivateId final : public Instruction {
public:
    HasPrivateId(Operand dst, Operand base, IdentifierTableIndex property)
        : Instruction(Type::HasPrivateId)
        , m_dst(dst)
        , m_base(base)
        , m_property(property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_base);
    }

    auto const& dst() const { return m_dst; }
    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }

private:
    Operand m_dst;
    Operand m_base;
    IdentifierTableIndex m_property;
};
static_assert(IsTriviallyDestructible<HasPrivateId>);

class ImportCall final : public Instruction {
public:
    ImportCall(Operand dst, Operand specifier, Operand options)
        : Instruction(Type::ImportCall)
        , m_dst(dst)
        , m_specifier(specifier)
        , m_options(options)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_specifier);
        visitor(m_options);
    }

    auto const& dst() const { return m_dst; }
    auto const& specifier() const { return m_specifier; }
    auto const& options() const { return m_options; }

private:
    Operand m_dst;
    Operand m_specifier;
    Operand m_options;
};
static_assert(IsTriviallyDestructible<ImportCall>);

class In final : public Instruction {
public:
    In(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::In)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<In>);

class Increment final : public Instruction {
public:
    Increment(Operand dst)
        : Instruction(Type::Increment)
        , m_dst(dst)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<Increment>);

class InitializeLexicalBinding final : public Instruction {
public:
    InitializeLexicalBinding(IdentifierTableIndex identifier, Operand src)
        : Instruction(Type::InitializeLexicalBinding)
        , m_identifier(identifier)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& identifier() const { return m_identifier; }
    auto const& src() const { return m_src; }
    auto const& cache() const { return m_cache; }

private:
    IdentifierTableIndex m_identifier;
    Operand m_src;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<InitializeLexicalBinding>);

class InitializeVariableBinding final : public Instruction {
public:
    InitializeVariableBinding(IdentifierTableIndex identifier, Operand src)
        : Instruction(Type::InitializeVariableBinding)
        , m_identifier(identifier)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& identifier() const { return m_identifier; }
    auto const& src() const { return m_src; }
    auto const& cache() const { return m_cache; }

private:
    IdentifierTableIndex m_identifier;
    Operand m_src;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<InitializeVariableBinding>);

class InstanceOf final : public Instruction {
public:
    InstanceOf(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::InstanceOf)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<InstanceOf>);

class IsCallable final : public Instruction {
public:
    IsCallable(Operand dst, Operand value)
        : Instruction(Type::IsCallable)
        , m_dst(dst)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<IsCallable>);

class IsConstructor final : public Instruction {
public:
    IsConstructor(Operand dst, Operand value)
        : Instruction(Type::IsConstructor)
        , m_dst(dst)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<IsConstructor>);

class IteratorClose final : public Instruction {
public:
    IteratorClose(Operand iterator_object, Operand iterator_next, Operand iterator_done, Completion::Type completion_type, Operand completion_value)
        : Instruction(Type::IteratorClose)
        , m_iterator_object(iterator_object)
        , m_iterator_next(iterator_next)
        , m_iterator_done(iterator_done)
        , m_completion_type(completion_type)
        , m_completion_value(completion_value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_iterator_object);
        visitor(m_iterator_next);
        visitor(m_iterator_done);
        visitor(m_completion_value);
    }

    auto const& iterator_object() const { return m_iterator_object; }
    auto const& iterator_next() const { return m_iterator_next; }
    auto const& iterator_done() const { return m_iterator_done; }
    auto const& completion_type() const { return m_completion_type; }
    auto const& completion_value() const { return m_completion_value; }

private:
    Operand m_iterator_object;
    Operand m_iterator_next;
    Operand m_iterator_done;
    Completion::Type m_completion_type;
    Operand m_completion_value;
};
static_assert(IsTriviallyDestructible<IteratorClose>);

class IteratorNext final : public Instruction {
public:
    IteratorNext(Operand dst, Operand iterator_object, Operand iterator_next, Operand iterator_done)
        : Instruction(Type::IteratorNext)
        , m_dst(dst)
        , m_iterator_object(iterator_object)
        , m_iterator_next(iterator_next)
        , m_iterator_done(iterator_done)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_iterator_object);
        visitor(m_iterator_next);
        visitor(m_iterator_done);
    }

    auto const& dst() const { return m_dst; }
    auto const& iterator_object() const { return m_iterator_object; }
    auto const& iterator_next() const { return m_iterator_next; }
    auto const& iterator_done() const { return m_iterator_done; }

private:
    Operand m_dst;
    Operand m_iterator_object;
    Operand m_iterator_next;
    Operand m_iterator_done;
};
static_assert(IsTriviallyDestructible<IteratorNext>);

class IteratorNextUnpack final : public Instruction {
public:
    IteratorNextUnpack(Operand dst_value, Operand dst_done, Operand iterator_object, Operand iterator_next, Operand iterator_done)
        : Instruction(Type::IteratorNextUnpack)
        , m_dst_value(dst_value)
        , m_dst_done(dst_done)
        , m_iterator_object(iterator_object)
        , m_iterator_next(iterator_next)
        , m_iterator_done(iterator_done)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst_value);
        visitor(m_dst_done);
        visitor(m_iterator_object);
        visitor(m_iterator_next);
        visitor(m_iterator_done);
    }

    auto const& dst_value() const { return m_dst_value; }
    auto const& dst_done() const { return m_dst_done; }
    auto const& iterator_object() const { return m_iterator_object; }
    auto const& iterator_next() const { return m_iterator_next; }
    auto const& iterator_done() const { return m_iterator_done; }

private:
    Operand m_dst_value;
    Operand m_dst_done;
    Operand m_iterator_object;
    Operand m_iterator_next;
    Operand m_iterator_done;
};
static_assert(IsTriviallyDestructible<IteratorNextUnpack>);

class IteratorToArray final : public Instruction {
public:
    IteratorToArray(Operand dst, Operand iterator_object, Operand iterator_next_method, Operand iterator_done_property)
        : Instruction(Type::IteratorToArray)
        , m_dst(dst)
        , m_iterator_object(iterator_object)
        , m_iterator_next_method(iterator_next_method)
        , m_iterator_done_property(iterator_done_property)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_iterator_object);
        visitor(m_iterator_next_method);
        visitor(m_iterator_done_property);
    }

    auto const& dst() const { return m_dst; }
    auto const& iterator_object() const { return m_iterator_object; }
    auto const& iterator_next_method() const { return m_iterator_next_method; }
    auto const& iterator_done_property() const { return m_iterator_done_property; }

private:
    Operand m_dst;
    Operand m_iterator_object;
    Operand m_iterator_next_method;
    Operand m_iterator_done_property;
};
static_assert(IsTriviallyDestructible<IteratorToArray>);

class Jump final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    Jump(Label target)
        : Instruction(Type::Jump)
        , m_target(target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_target);
    }

    auto const& target() const { return m_target; }

private:
    Label m_target;
};
static_assert(IsTriviallyDestructible<Jump>);

class JumpFalse final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpFalse(Operand condition, Label target)
        : Instruction(Type::JumpFalse)
        , m_condition(condition)
        , m_target(target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_condition);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_target);
    }

    auto const& condition() const { return m_condition; }
    auto const& target() const { return m_target; }

private:
    Operand m_condition;
    Label m_target;
};
static_assert(IsTriviallyDestructible<JumpFalse>);

class JumpGreaterThan final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpGreaterThan(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpGreaterThan)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpGreaterThan>);

class JumpGreaterThanEquals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpGreaterThanEquals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpGreaterThanEquals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpGreaterThanEquals>);

class JumpIf final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpIf(Operand condition, Label true_target, Label false_target)
        : Instruction(Type::JumpIf)
        , m_condition(condition)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_condition);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& condition() const { return m_condition; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_condition;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpIf>);

class JumpLessThan final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpLessThan(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpLessThan)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpLessThan>);

class JumpLessThanEquals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpLessThanEquals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpLessThanEquals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpLessThanEquals>);

class JumpLooselyEquals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpLooselyEquals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpLooselyEquals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpLooselyEquals>);

class JumpLooselyInequals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpLooselyInequals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpLooselyInequals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpLooselyInequals>);

class JumpNullish final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpNullish(Operand condition, Label true_target, Label false_target)
        : Instruction(Type::JumpNullish)
        , m_condition(condition)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_condition);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& condition() const { return m_condition; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_condition;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpNullish>);

class JumpStrictlyEquals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpStrictlyEquals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpStrictlyEquals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpStrictlyEquals>);

class JumpStrictlyInequals final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpStrictlyInequals(Operand lhs, Operand rhs, Label true_target, Label false_target)
        : Instruction(Type::JumpStrictlyInequals)
        , m_lhs(lhs)
        , m_rhs(rhs)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_lhs);
        visitor(m_rhs);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_lhs;
    Operand m_rhs;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpStrictlyInequals>);

class JumpTrue final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpTrue(Operand condition, Label target)
        : Instruction(Type::JumpTrue)
        , m_condition(condition)
        , m_target(target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_condition);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_target);
    }

    auto const& condition() const { return m_condition; }
    auto const& target() const { return m_target; }

private:
    Operand m_condition;
    Label m_target;
};
static_assert(IsTriviallyDestructible<JumpTrue>);

class JumpUndefined final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    JumpUndefined(Operand condition, Label true_target, Label false_target)
        : Instruction(Type::JumpUndefined)
        , m_condition(condition)
        , m_true_target(true_target)
        , m_false_target(false_target)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_condition);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        visitor(m_true_target);
        visitor(m_false_target);
    }

    auto const& condition() const { return m_condition; }
    auto const& true_target() const { return m_true_target; }
    auto const& false_target() const { return m_false_target; }

private:
    Operand m_condition;
    Label m_true_target;
    Label m_false_target;
};
static_assert(IsTriviallyDestructible<JumpUndefined>);

class LeavePrivateEnvironment final : public Instruction {
public:
    LeavePrivateEnvironment()
        : Instruction(Type::LeavePrivateEnvironment)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

private:
};
static_assert(IsTriviallyDestructible<LeavePrivateEnvironment>);

class LeftShift final : public Instruction {
public:
    LeftShift(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::LeftShift)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<LeftShift>);

class LessThan final : public Instruction {
public:
    LessThan(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::LessThan)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<LessThan>);

class LessThanEquals final : public Instruction {
public:
    LessThanEquals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::LessThanEquals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<LessThanEquals>);

class LooselyEquals final : public Instruction {
public:
    LooselyEquals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::LooselyEquals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<LooselyEquals>);

class LooselyInequals final : public Instruction {
public:
    LooselyInequals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::LooselyInequals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<LooselyInequals>);

class Mod final : public Instruction {
public:
    Mod(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Mod)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Mod>);

class Mov final : public Instruction {
public:
    Mov(Operand dst, Operand src)
        : Instruction(Type::Mov)
        , m_dst(dst)
        , m_src(src)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<Mov>);

class Mul final : public Instruction {
public:
    Mul(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Mul)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Mul>);

class NewArray final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    NewArray(Operand dst, ReadonlySpan<ScopedOperand> elements)
        : Instruction(Type::NewArray)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Operand) * elements.size()))
        , m_dst(dst)
        , m_element_count(elements.size())
    {
        for (size_t i = 0; i < elements.size(); ++i)
            m_elements[i] = elements[i];
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        for (size_t i = 0; i < m_element_count; ++i)
            visitor(m_elements[i]);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& element_count() const { return m_element_count; }
    ReadonlySpan<Operand> elements() const { return ReadonlySpan<Operand> { m_elements, m_element_count }; }

private:
    u32 m_length;
    Operand m_dst;
    u32 m_element_count;
    Operand m_elements[];
};
static_assert(IsTriviallyDestructible<NewArray>);

class NewArrayWithLength final : public Instruction {
public:
    NewArrayWithLength(Operand dst, Operand array_length)
        : Instruction(Type::NewArrayWithLength)
        , m_dst(dst)
        , m_array_length(array_length)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_array_length);
    }

    auto const& dst() const { return m_dst; }
    auto const& array_length() const { return m_array_length; }

private:
    Operand m_dst;
    Operand m_array_length;
};
static_assert(IsTriviallyDestructible<NewArrayWithLength>);

class NewClass final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    NewClass(Operand dst, Optional<Operand> super_class, Operand class_environment, u32 class_blueprint_index, Optional<IdentifierTableIndex> lhs_name, ReadonlySpan<Optional<ScopedOperand>> element_keys)
        : Instruction(Type::NewClass)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Optional<Operand>) * element_keys.size()))
        , m_dst(dst)
        , m_super_class(super_class)
        , m_class_environment(class_environment)
        , m_class_blueprint_index(class_blueprint_index)
        , m_lhs_name(lhs_name)
        , m_element_keys_count(element_keys.size())
    {
        for (size_t i = 0; i < element_keys.size(); ++i) {
            if (element_keys[i].has_value())
                m_element_keys[i] = element_keys[i].value();
            else
                m_element_keys[i] = {};
        }
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        if (m_super_class.has_value())
            visitor(m_super_class.value());
        visitor(m_class_environment);
        for (size_t i = 0; i < m_element_keys_count; ++i) {
            if (m_element_keys[i].has_value())
                visitor(m_element_keys[i].value());
        }
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& super_class() const { return m_super_class; }
    auto const& class_environment() const { return m_class_environment; }
    auto const& class_blueprint_index() const { return m_class_blueprint_index; }
    auto const& lhs_name() const { return m_lhs_name; }
    auto const& element_keys_count() const { return m_element_keys_count; }
    ReadonlySpan<Optional<Operand>> element_keys() const { return ReadonlySpan<Optional<Operand>> { m_element_keys, m_element_keys_count }; }

private:
    u32 m_length;
    Operand m_dst;
    Optional<Operand> m_super_class;
    Operand m_class_environment;
    u32 m_class_blueprint_index;
    Optional<IdentifierTableIndex> m_lhs_name;
    u32 m_element_keys_count;
    Optional<Operand> m_element_keys[];
};
static_assert(IsTriviallyDestructible<NewClass>);

class NewFunction final : public Instruction {
public:
    NewFunction(Operand dst, u32 shared_function_data_index, Optional<IdentifierTableIndex> lhs_name, Optional<Operand> home_object)
        : Instruction(Type::NewFunction)
        , m_dst(dst)
        , m_shared_function_data_index(shared_function_data_index)
        , m_lhs_name(lhs_name)
        , m_home_object(home_object)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        if (m_home_object.has_value())
            visitor(m_home_object.value());
    }

    auto const& dst() const { return m_dst; }
    auto const& shared_function_data_index() const { return m_shared_function_data_index; }
    auto const& lhs_name() const { return m_lhs_name; }
    auto const& home_object() const { return m_home_object; }

private:
    Operand m_dst;
    u32 m_shared_function_data_index;
    Optional<IdentifierTableIndex> m_lhs_name;
    Optional<Operand> m_home_object;
};
static_assert(IsTriviallyDestructible<NewFunction>);

class NewObject final : public Instruction {
public:
    NewObject(Operand dst, u32 cache_index)
        : Instruction(Type::NewObject)
        , m_dst(dst)
        , m_cache_index(cache_index)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_dst;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<NewObject>);

class NewObjectWithNoPrototype final : public Instruction {
public:
    NewObjectWithNoPrototype(Operand dst)
        : Instruction(Type::NewObjectWithNoPrototype)
        , m_dst(dst)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<NewObjectWithNoPrototype>);

class NewPrimitiveArray final : public Instruction {
public:
    static constexpr bool IsVariableLength = true;
    size_t length_impl() const { return m_length; }
    NewPrimitiveArray(Operand dst, ReadonlySpan<Value> elements)
        : Instruction(Type::NewPrimitiveArray)
        , m_length(round_up_to_power_of_two(alignof(void*), sizeof(*this) + sizeof(Value) * elements.size()))
        , m_dst(dst)
        , m_element_count(elements.size())
    {
        for (size_t i = 0; i < elements.size(); ++i)
            m_elements[i] = elements[i];
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& length() const { return m_length; }
    auto const& dst() const { return m_dst; }
    auto const& element_count() const { return m_element_count; }
    ReadonlySpan<Value> elements() const { return ReadonlySpan<Value> { m_elements, m_element_count }; }

private:
    u32 m_length;
    Operand m_dst;
    u32 m_element_count;
    Value m_elements[];
};
static_assert(IsTriviallyDestructible<NewPrimitiveArray>);

class NewRegExp final : public Instruction {
public:
    NewRegExp(Operand dst, StringTableIndex source_index, StringTableIndex flags_index, RegexTableIndex regex_index)
        : Instruction(Type::NewRegExp)
        , m_dst(dst)
        , m_source_index(source_index)
        , m_flags_index(flags_index)
        , m_regex_index(regex_index)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& source_index() const { return m_source_index; }
    auto const& flags_index() const { return m_flags_index; }
    auto const& regex_index() const { return m_regex_index; }

private:
    Operand m_dst;
    StringTableIndex m_source_index;
    StringTableIndex m_flags_index;
    RegexTableIndex m_regex_index;
};
static_assert(IsTriviallyDestructible<NewRegExp>);

class NewTypeError final : public Instruction {
public:
    NewTypeError(Operand dst, StringTableIndex error_string)
        : Instruction(Type::NewTypeError)
        , m_dst(dst)
        , m_error_string(error_string)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& error_string() const { return m_error_string; }

private:
    Operand m_dst;
    StringTableIndex m_error_string;
};
static_assert(IsTriviallyDestructible<NewTypeError>);

class NewReferenceError final : public Instruction {
public:
    NewReferenceError(Operand dst, StringTableIndex error_string)
        : Instruction(Type::NewReferenceError)
        , m_dst(dst)
        , m_error_string(error_string)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& error_string() const { return m_error_string; }

private:
    Operand m_dst;
    StringTableIndex m_error_string;
};
static_assert(IsTriviallyDestructible<NewReferenceError>);

class Not final : public Instruction {
public:
    Not(Operand dst, Operand src)
        : Instruction(Type::Not)
        , m_dst(dst)
        , m_src(src)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<Not>);

class PostfixDecrement final : public Instruction {
public:
    PostfixDecrement(Operand dst, Operand src)
        : Instruction(Type::PostfixDecrement)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<PostfixDecrement>);

class PostfixIncrement final : public Instruction {
public:
    PostfixIncrement(Operand dst, Operand src)
        : Instruction(Type::PostfixIncrement)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<PostfixIncrement>);

class PutById final : public Instruction {
public:
    PutById(Operand base, PropertyKeyTableIndex property, Operand src, PutKind kind, u32 cache_index, Optional<IdentifierTableIndex> base_identifier)
        : Instruction(Type::PutById)
        , m_base(base)
        , m_property(property)
        , m_src(src)
        , m_kind(kind)
        , m_cache_index(cache_index)
        , m_base_identifier(base_identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& src() const { return m_src; }
    auto const& kind() const { return m_kind; }
    auto const& cache_index() const { return m_cache_index; }
    auto const& base_identifier() const { return m_base_identifier; }

private:
    Operand m_base;
    PropertyKeyTableIndex m_property;
    Operand m_src;
    PutKind m_kind;
    u32 m_cache_index;
    Optional<IdentifierTableIndex> m_base_identifier;
};
static_assert(IsTriviallyDestructible<PutById>);

class PutByIdWithThis final : public Instruction {
public:
    PutByIdWithThis(Operand base, Operand this_value, PropertyKeyTableIndex property, Operand src, PutKind kind, u32 cache_index)
        : Instruction(Type::PutByIdWithThis)
        , m_base(base)
        , m_this_value(this_value)
        , m_property(property)
        , m_src(src)
        , m_kind(kind)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_this_value);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& this_value() const { return m_this_value; }
    auto const& property() const { return m_property; }
    auto const& src() const { return m_src; }
    auto const& kind() const { return m_kind; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_base;
    Operand m_this_value;
    PropertyKeyTableIndex m_property;
    Operand m_src;
    PutKind m_kind;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<PutByIdWithThis>);

class PutBySpread final : public Instruction {
public:
    PutBySpread(Operand base, Operand src)
        : Instruction(Type::PutBySpread)
        , m_base(base)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& src() const { return m_src; }

private:
    Operand m_base;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<PutBySpread>);

class PutByValue final : public Instruction {
public:
    PutByValue(Operand base, Operand property, Operand src, PutKind kind, Optional<IdentifierTableIndex> base_identifier)
        : Instruction(Type::PutByValue)
        , m_base(base)
        , m_property(property)
        , m_src(src)
        , m_kind(kind)
        , m_base_identifier(base_identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_property);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& src() const { return m_src; }
    auto const& kind() const { return m_kind; }
    auto const& base_identifier() const { return m_base_identifier; }

private:
    Operand m_base;
    Operand m_property;
    Operand m_src;
    PutKind m_kind;
    Optional<IdentifierTableIndex> m_base_identifier;
};
static_assert(IsTriviallyDestructible<PutByValue>);

class PutByValueWithThis final : public Instruction {
public:
    PutByValueWithThis(Operand base, Operand property, Operand this_value, Operand src, PutKind kind)
        : Instruction(Type::PutByValueWithThis)
        , m_base(base)
        , m_property(property)
        , m_this_value(this_value)
        , m_src(src)
        , m_kind(kind)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_property);
        visitor(m_this_value);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& this_value() const { return m_this_value; }
    auto const& src() const { return m_src; }
    auto const& kind() const { return m_kind; }

private:
    Operand m_base;
    Operand m_property;
    Operand m_this_value;
    Operand m_src;
    PutKind m_kind;
};
static_assert(IsTriviallyDestructible<PutByValueWithThis>);

class PutPrivateById final : public Instruction {
public:
    PutPrivateById(Operand base, IdentifierTableIndex property, Operand src)
        : Instruction(Type::PutPrivateById)
        , m_base(base)
        , m_property(property)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_base);
        visitor(m_src);
    }

    auto const& base() const { return m_base; }
    auto const& property() const { return m_property; }
    auto const& src() const { return m_src; }

private:
    Operand m_base;
    IdentifierTableIndex m_property;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<PutPrivateById>);

class ResolveSuperBase final : public Instruction {
public:
    ResolveSuperBase(Operand dst)
        : Instruction(Type::ResolveSuperBase)
        , m_dst(dst)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }

private:
    Operand m_dst;
};
static_assert(IsTriviallyDestructible<ResolveSuperBase>);

class ResolveThisBinding final : public Instruction {
public:
    ResolveThisBinding()
        : Instruction(Type::ResolveThisBinding)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

private:
};
static_assert(IsTriviallyDestructible<ResolveThisBinding>);

class Return final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    Return(Operand value)
        : Instruction(Type::Return)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_value);
    }

    auto const& value() const { return m_value; }

private:
    Operand m_value;
};
static_assert(IsTriviallyDestructible<Return>);

class RightShift final : public Instruction {
public:
    RightShift(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::RightShift)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<RightShift>);

class SetCompletionType final : public Instruction {
public:
    SetCompletionType(Operand completion, Completion::Type completion_type)
        : Instruction(Type::SetCompletionType)
        , m_completion(completion)
        , m_completion_type(completion_type)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_completion);
    }

    auto const& completion() const { return m_completion; }
    auto const& completion_type() const { return m_completion_type; }

private:
    Operand m_completion;
    Completion::Type m_completion_type;
};
static_assert(IsTriviallyDestructible<SetCompletionType>);

class SetGlobal final : public Instruction {
public:
    SetGlobal(IdentifierTableIndex identifier, Operand src, u32 cache_index)
        : Instruction(Type::SetGlobal)
        , m_identifier(identifier)
        , m_src(src)
        , m_cache_index(cache_index)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& identifier() const { return m_identifier; }
    auto const& src() const { return m_src; }
    auto const& cache_index() const { return m_cache_index; }

private:
    IdentifierTableIndex m_identifier;
    Operand m_src;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<SetGlobal>);

class SetLexicalEnvironment final : public Instruction {
public:
    SetLexicalEnvironment(Operand environment)
        : Instruction(Type::SetLexicalEnvironment)
        , m_environment(environment)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_environment);
    }

    auto const& environment() const { return m_environment; }

private:
    Operand m_environment;
};
static_assert(IsTriviallyDestructible<SetLexicalEnvironment>);

class SetLexicalBinding final : public Instruction {
public:
    SetLexicalBinding(IdentifierTableIndex identifier, Operand src)
        : Instruction(Type::SetLexicalBinding)
        , m_identifier(identifier)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& identifier() const { return m_identifier; }
    auto const& src() const { return m_src; }
    auto const& cache() const { return m_cache; }

private:
    IdentifierTableIndex m_identifier;
    Operand m_src;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<SetLexicalBinding>);

class SetVariableBinding final : public Instruction {
public:
    SetVariableBinding(IdentifierTableIndex identifier, Operand src)
        : Instruction(Type::SetVariableBinding)
        , m_identifier(identifier)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& identifier() const { return m_identifier; }
    auto const& src() const { return m_src; }
    auto const& cache() const { return m_cache; }

private:
    IdentifierTableIndex m_identifier;
    Operand m_src;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<SetVariableBinding>);

class StrictlyEquals final : public Instruction {
public:
    StrictlyEquals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::StrictlyEquals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<StrictlyEquals>);

class StrictlyInequals final : public Instruction {
public:
    StrictlyInequals(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::StrictlyInequals)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<StrictlyInequals>);

class Sub final : public Instruction {
public:
    Sub(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::Sub)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<Sub>);

class SuperCallWithArgumentArray final : public Instruction {
public:
    SuperCallWithArgumentArray(Operand dst, Operand arguments, bool is_synthetic)
        : Instruction(Type::SuperCallWithArgumentArray)
        , m_dst(dst)
        , m_arguments(arguments)
        , m_is_synthetic(is_synthetic)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_arguments);
    }

    auto const& dst() const { return m_dst; }
    auto const& arguments() const { return m_arguments; }
    auto const& is_synthetic() const { return m_is_synthetic; }

private:
    Operand m_dst;
    Operand m_arguments;
    bool m_is_synthetic;
};
static_assert(IsTriviallyDestructible<SuperCallWithArgumentArray>);

class Throw final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    Throw(Operand src)
        : Instruction(Type::Throw)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& src() const { return m_src; }

private:
    Operand m_src;
};
static_assert(IsTriviallyDestructible<Throw>);

class ThrowIfNotObject final : public Instruction {
public:
    ThrowIfNotObject(Operand src)
        : Instruction(Type::ThrowIfNotObject)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& src() const { return m_src; }

private:
    Operand m_src;
};
static_assert(IsTriviallyDestructible<ThrowIfNotObject>);

class ThrowIfNullish final : public Instruction {
public:
    ThrowIfNullish(Operand src)
        : Instruction(Type::ThrowIfNullish)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& src() const { return m_src; }

private:
    Operand m_src;
};
static_assert(IsTriviallyDestructible<ThrowIfNullish>);

class ThrowIfTDZ final : public Instruction {
public:
    ThrowIfTDZ(Operand src)
        : Instruction(Type::ThrowIfTDZ)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_src);
    }

    auto const& src() const { return m_src; }

private:
    Operand m_src;
};
static_assert(IsTriviallyDestructible<ThrowIfTDZ>);

class ThrowConstAssignment final : public Instruction {
public:
    ThrowConstAssignment()
        : Instruction(Type::ThrowConstAssignment)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;

private:
};
static_assert(IsTriviallyDestructible<ThrowConstAssignment>);

class ToBoolean final : public Instruction {
public:
    ToBoolean(Operand dst, Operand value)
        : Instruction(Type::ToBoolean)
        , m_dst(dst)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToBoolean>);

class ToLength final : public Instruction {
public:
    ToLength(Operand dst, Operand value)
        : Instruction(Type::ToLength)
        , m_dst(dst)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToLength>);

class ToObject final : public Instruction {
public:
    ToObject(Operand dst, Operand value)
        : Instruction(Type::ToObject)
        , m_dst(dst)
        , m_value(value)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_value);
    }

    auto const& dst() const { return m_dst; }
    auto const& value() const { return m_value; }

private:
    Operand m_dst;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<ToObject>);

class Typeof final : public Instruction {
public:
    Typeof(Operand dst, Operand src)
        : Instruction(Type::Typeof)
        , m_dst(dst)
        , m_src(src)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<Typeof>);

class TypeofBinding final : public Instruction {
public:
    TypeofBinding(Operand dst, IdentifierTableIndex identifier)
        : Instruction(Type::TypeofBinding)
        , m_dst(dst)
        , m_identifier(identifier)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
    }

    auto const& dst() const { return m_dst; }
    auto const& identifier() const { return m_identifier; }
    auto const& cache() const { return m_cache; }

private:
    Operand m_dst;
    IdentifierTableIndex m_identifier;
    mutable EnvironmentCoordinate m_cache;
};
static_assert(IsTriviallyDestructible<TypeofBinding>);

class UnaryMinus final : public Instruction {
public:
    UnaryMinus(Operand dst, Operand src)
        : Instruction(Type::UnaryMinus)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<UnaryMinus>);

class UnaryPlus final : public Instruction {
public:
    UnaryPlus(Operand dst, Operand src)
        : Instruction(Type::UnaryPlus)
        , m_dst(dst)
        , m_src(src)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_src);
    }

    auto const& dst() const { return m_dst; }
    auto const& src() const { return m_src; }

private:
    Operand m_dst;
    Operand m_src;
};
static_assert(IsTriviallyDestructible<UnaryPlus>);

class UnsignedRightShift final : public Instruction {
public:
    UnsignedRightShift(Operand dst, Operand lhs, Operand rhs)
        : Instruction(Type::UnsignedRightShift)
        , m_dst(dst)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }

    ThrowCompletionOr<void> execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_dst);
        visitor(m_lhs);
        visitor(m_rhs);
    }

    auto const& dst() const { return m_dst; }
    auto const& lhs() const { return m_lhs; }
    auto const& rhs() const { return m_rhs; }

private:
    Operand m_dst;
    Operand m_lhs;
    Operand m_rhs;
};
static_assert(IsTriviallyDestructible<UnsignedRightShift>);

class Yield final : public Instruction {
public:
    static constexpr bool IsTerminator = true;
    Yield(Optional<Label> continuation_label, Operand value)
        : Instruction(Type::Yield)
        , m_continuation_label(continuation_label)
        , m_value(value)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_value);
    }
    void visit_labels_impl(Function<void(Label&)> visitor)
    {
        if (m_continuation_label.has_value())
            visitor(m_continuation_label.value());
    }

    auto const& continuation_label() const { return m_continuation_label; }
    auto const& value() const { return m_value; }

private:
    Optional<Label> m_continuation_label;
    Operand m_value;
};
static_assert(IsTriviallyDestructible<Yield>);

class CacheObjectShape final : public Instruction {
public:
    CacheObjectShape(Operand object, u32 cache_index)
        : Instruction(Type::CacheObjectShape)
        , m_object(object)
        , m_cache_index(cache_index)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_object);
    }

    auto const& object() const { return m_object; }
    auto const& cache_index() const { return m_cache_index; }

private:
    Operand m_object;
    u32 m_cache_index;
};
static_assert(IsTriviallyDestructible<CacheObjectShape>);

class InitObjectLiteralProperty final : public Instruction {
public:
    InitObjectLiteralProperty(Operand object, PropertyKeyTableIndex property, Operand src, u32 shape_cache_index, u32 property_slot)
        : Instruction(Type::InitObjectLiteralProperty)
        , m_object(object)
        , m_property(property)
        , m_src(src)
        , m_shape_cache_index(shape_cache_index)
        , m_property_slot(property_slot)
    {
    }

    void execute_impl(Bytecode::Interpreter&) const;
    ByteString to_byte_string_impl(Bytecode::Executable const&) const;
    void visit_operands_impl(Function<void(Operand&)> visitor)
    {
        visitor(m_object);
        visitor(m_src);
    }

    auto const& object() const { return m_object; }
    auto const& property() const { return m_property; }
    auto const& src() const { return m_src; }
    auto const& shape_cache_index() const { return m_shape_cache_index; }
    auto const& property_slot() const { return m_property_slot; }

private:
    Operand m_object;
    PropertyKeyTableIndex m_property;
    Operand m_src;
    u32 m_shape_cache_index;
    u32 m_property_slot;
};
static_assert(IsTriviallyDestructible<InitObjectLiteralProperty>);

} // namespace JS::Bytecode::Op
