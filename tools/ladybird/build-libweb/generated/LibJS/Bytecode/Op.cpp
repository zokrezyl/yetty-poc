#include <AK/StringBuilder.h>
#include <AK/StringView.h>
#include <LibJS/Bytecode/FormatOperand.h>
#include <LibJS/Bytecode/Op.h>

namespace JS::Bytecode::Op {

ByteString Add::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Add"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString AddPrivateName::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("AddPrivateName"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("name:{}", executable.identifier_table->get(m_name)));

    return builder.to_byte_string();
}

ByteString ArrayAppend::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ArrayAppend"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("is_spread:{}", m_is_spread));

    return builder.to_byte_string();
}

ByteString Await::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Await"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("continuation_label:{}", m_continuation_label));

    append_piece(format_operand("argument"sv, m_argument, executable));

    return builder.to_byte_string();
}

ByteString BitwiseAnd::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("BitwiseAnd"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString BitwiseNot::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("BitwiseNot"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString BitwiseOr::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("BitwiseOr"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString ToInt32::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToInt32"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString ToString::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToString"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString ToPrimitiveWithStringHint::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToPrimitiveWithStringHint"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString BitwiseXor::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("BitwiseXor"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Call::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Call"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    if (m_argument_count != 0)
        append_piece(format_operand_list("arguments"sv, { m_arguments, m_argument_count }, executable));

    return builder.to_byte_string();
}

ByteString CallBuiltin::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallBuiltin"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    if (m_argument_count != 0)
        append_piece(format_operand_list("arguments"sv, { m_arguments, m_argument_count }, executable));

    return builder.to_byte_string();
}

ByteString CallConstruct::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallConstruct"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    if (m_argument_count != 0)
        append_piece(format_operand_list("arguments"sv, { m_arguments, m_argument_count }, executable));

    return builder.to_byte_string();
}

ByteString CallConstructWithArgumentArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallConstructWithArgumentArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(format_operand("arguments"sv, m_arguments, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    return builder.to_byte_string();
}

ByteString CallDirectEval::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallDirectEval"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    if (m_argument_count != 0)
        append_piece(format_operand_list("arguments"sv, { m_arguments, m_argument_count }, executable));

    return builder.to_byte_string();
}

ByteString CallDirectEvalWithArgumentArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallDirectEvalWithArgumentArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(format_operand("arguments"sv, m_arguments, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    return builder.to_byte_string();
}

ByteString CallWithArgumentArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CallWithArgumentArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(format_operand("arguments"sv, m_arguments, executable));

    if (m_expression_string.has_value())
        append_piece(executable.get_string(m_expression_string.value()));

    return builder.to_byte_string();
}

ByteString Catch::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Catch"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString ConcatString::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ConcatString"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString CopyObjectExcludingProperties::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CopyObjectExcludingProperties"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("from_object"sv, m_from_object, executable));

    if (m_excluded_names_count != 0)
        append_piece(format_operand_list("excluded_names"sv, { m_excluded_names, m_excluded_names_count }, executable));

    return builder.to_byte_string();
}

ByteString CreateArguments::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateArguments"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    if (m_dst.has_value())
        append_piece(format_operand("dst"sv, m_dst.value(), executable));

    append_piece(ByteString::formatted("is_immutable:{}", m_is_immutable));

    return builder.to_byte_string();
}

ByteString CreateAsyncFromSyncIterator::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateAsyncFromSyncIterator"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("iterator"sv, m_iterator, executable));

    append_piece(format_operand("next_method"sv, m_next_method, executable));

    append_piece(format_operand("done"sv, m_done, executable));

    return builder.to_byte_string();
}

ByteString CreateDataPropertyOrThrow::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateDataPropertyOrThrow"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("object"sv, m_object, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString CreateLexicalEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateLexicalEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("parent"sv, m_parent, executable));

    append_piece(ByteString::formatted("capacity:{}", m_capacity));

    return builder.to_byte_string();
}

ByteString CreateImmutableBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateImmutableBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("environment"sv, m_environment, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(ByteString::formatted("strict_binding:{}", m_strict_binding));

    return builder.to_byte_string();
}

ByteString CreateMutableBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateMutableBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("environment"sv, m_environment, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(ByteString::formatted("can_be_deleted:{}", m_can_be_deleted));

    return builder.to_byte_string();
}

ByteString CreatePrivateEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreatePrivateEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    return builder.to_byte_string();
}

ByteString CreateRestParams::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateRestParams"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("rest_index:{}", m_rest_index));

    return builder.to_byte_string();
}

ByteString CreateVariable::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateVariable"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(ByteString::formatted("is_immutable:{}", m_is_immutable));

    append_piece(ByteString::formatted("is_global:{}", m_is_global));

    append_piece(ByteString::formatted("is_strict:{}", m_is_strict));

    return builder.to_byte_string();
}

ByteString CreateVariableEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CreateVariableEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("capacity:{}", m_capacity));

    return builder.to_byte_string();
}

ByteString Decrement::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Decrement"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString DeleteById::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("DeleteById"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    return builder.to_byte_string();
}

ByteString DeleteByValue::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("DeleteByValue"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    return builder.to_byte_string();
}

ByteString DeleteVariable::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("DeleteVariable"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString Div::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Div"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString End::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("End"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString EnterObjectEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("EnterObjectEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("object"sv, m_object, executable));

    return builder.to_byte_string();
}

ByteString Exp::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Exp"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString GetById::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetById"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    if (m_base_identifier.has_value())
        append_piece(ByteString::formatted("base_identifier:{}", executable.identifier_table->get(m_base_identifier.value())));

    return builder.to_byte_string();
}

ByteString GetByIdWithThis::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetByIdWithThis"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    return builder.to_byte_string();
}

ByteString GetByValue::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetByValue"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    if (m_base_identifier.has_value())
        append_piece(ByteString::formatted("base_identifier:{}", executable.identifier_table->get(m_base_identifier.value())));

    return builder.to_byte_string();
}

ByteString GetByValueWithThis::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetByValueWithThis"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    return builder.to_byte_string();
}

ByteString GetCalleeAndThisFromEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetCalleeAndThisFromEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("callee"sv, m_callee, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString GetCompletionFields::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetCompletionFields"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("type_dst"sv, m_type_dst, executable));

    append_piece(format_operand("value_dst"sv, m_value_dst, executable));

    append_piece(format_operand("completion"sv, m_completion, executable));

    return builder.to_byte_string();
}

ByteString GetGlobal::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetGlobal"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString GetImportMeta::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetImportMeta"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString GetLexicalEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetLexicalEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString GetIterator::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetIterator"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst_iterator_object"sv, m_dst_iterator_object, executable));

    append_piece(format_operand("dst_iterator_next"sv, m_dst_iterator_next, executable));

    append_piece(format_operand("dst_iterator_done"sv, m_dst_iterator_done, executable));

    append_piece(format_operand("iterable"sv, m_iterable, executable));

    return builder.to_byte_string();
}

ByteString GetLength::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetLength"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    if (m_base_identifier.has_value())
        append_piece(ByteString::formatted("base_identifier:{}", executable.identifier_table->get(m_base_identifier.value())));

    return builder.to_byte_string();
}

ByteString GetLengthWithThis::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetLengthWithThis"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    return builder.to_byte_string();
}

ByteString GetMethod::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetMethod"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("object"sv, m_object, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    return builder.to_byte_string();
}

ByteString GetNewTarget::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetNewTarget"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString GetObjectPropertyIterator::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetObjectPropertyIterator"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst_iterator_object"sv, m_dst_iterator_object, executable));

    append_piece(format_operand("dst_iterator_next"sv, m_dst_iterator_next, executable));

    append_piece(format_operand("dst_iterator_done"sv, m_dst_iterator_done, executable));

    append_piece(format_operand("object"sv, m_object, executable));

    return builder.to_byte_string();
}

ByteString GetPrivateById::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetPrivateById"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.identifier_table->get(m_property)));

    return builder.to_byte_string();
}

ByteString GetTemplateObject::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetTemplateObject"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    if (m_strings_count != 0)
        append_piece(format_operand_list("strings"sv, { m_strings, m_strings_count }, executable));

    return builder.to_byte_string();
}

ByteString GetBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString GetInitializedBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GetInitializedBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString GreaterThan::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GreaterThan"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString GreaterThanEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("GreaterThanEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString HasPrivateId::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("HasPrivateId"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.identifier_table->get(m_property)));

    return builder.to_byte_string();
}

ByteString ImportCall::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ImportCall"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("specifier"sv, m_specifier, executable));

    append_piece(format_operand("options"sv, m_options, executable));

    return builder.to_byte_string();
}

ByteString In::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("In"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Increment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Increment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString InitializeLexicalBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("InitializeLexicalBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString InitializeVariableBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("InitializeVariableBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString InstanceOf::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("InstanceOf"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString IsCallable::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IsCallable"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString IsConstructor::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IsConstructor"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString IteratorClose::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IteratorClose"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("iterator_object"sv, m_iterator_object, executable));

    append_piece(format_operand("iterator_next"sv, m_iterator_next, executable));

    append_piece(format_operand("iterator_done"sv, m_iterator_done, executable));

    append_piece(format_operand("completion_value"sv, m_completion_value, executable));

    return builder.to_byte_string();
}

ByteString IteratorNext::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IteratorNext"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("iterator_object"sv, m_iterator_object, executable));

    append_piece(format_operand("iterator_next"sv, m_iterator_next, executable));

    append_piece(format_operand("iterator_done"sv, m_iterator_done, executable));

    return builder.to_byte_string();
}

ByteString IteratorNextUnpack::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IteratorNextUnpack"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst_value"sv, m_dst_value, executable));

    append_piece(format_operand("dst_done"sv, m_dst_done, executable));

    append_piece(format_operand("iterator_object"sv, m_iterator_object, executable));

    append_piece(format_operand("iterator_next"sv, m_iterator_next, executable));

    append_piece(format_operand("iterator_done"sv, m_iterator_done, executable));

    return builder.to_byte_string();
}

ByteString IteratorToArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("IteratorToArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("iterator_object"sv, m_iterator_object, executable));

    append_piece(format_operand("iterator_next_method"sv, m_iterator_next_method, executable));

    append_piece(format_operand("iterator_done_property"sv, m_iterator_done_property, executable));

    return builder.to_byte_string();
}

ByteString Jump::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Jump"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("target:{}", m_target));

    return builder.to_byte_string();
}

ByteString JumpFalse::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpFalse"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("condition"sv, m_condition, executable));

    append_piece(ByteString::formatted("target:{}", m_target));

    return builder.to_byte_string();
}

ByteString JumpGreaterThan::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpGreaterThan"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpGreaterThanEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpGreaterThanEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpIf::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpIf"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("condition"sv, m_condition, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpLessThan::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpLessThan"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpLessThanEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpLessThanEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpLooselyEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpLooselyEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpLooselyInequals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpLooselyInequals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpNullish::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpNullish"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("condition"sv, m_condition, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpStrictlyEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpStrictlyEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpStrictlyInequals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpStrictlyInequals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString JumpTrue::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpTrue"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("condition"sv, m_condition, executable));

    append_piece(ByteString::formatted("target:{}", m_target));

    return builder.to_byte_string();
}

ByteString JumpUndefined::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("JumpUndefined"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("condition"sv, m_condition, executable));

    append_piece(ByteString::formatted("true_target:{}", m_true_target));

    append_piece(ByteString::formatted("false_target:{}", m_false_target));

    return builder.to_byte_string();
}

ByteString LeavePrivateEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LeavePrivateEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    return builder.to_byte_string();
}

ByteString LeftShift::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LeftShift"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString LessThan::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LessThan"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString LessThanEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LessThanEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString LooselyEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LooselyEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString LooselyInequals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("LooselyInequals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Mod::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Mod"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Mov::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Mov"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString Mul::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Mul"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString NewArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    if (m_element_count != 0)
        append_piece(format_operand_list("elements"sv, { m_elements, m_element_count }, executable));

    return builder.to_byte_string();
}

ByteString NewArrayWithLength::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewArrayWithLength"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("array_length"sv, m_array_length, executable));

    return builder.to_byte_string();
}

ByteString NewClass::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewClass"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    if (m_super_class.has_value())
        append_piece(format_operand("super_class"sv, m_super_class.value(), executable));

    append_piece(format_operand("class_environment"sv, m_class_environment, executable));

    append_piece(ByteString::formatted("class_blueprint_index:{}", m_class_blueprint_index));

    if (m_lhs_name.has_value())
        append_piece(ByteString::formatted("lhs_name:{}", executable.identifier_table->get(m_lhs_name.value())));

    if (m_element_keys_count != 0) {
        StringBuilder list_builder;
        list_builder.appendff("{}:[", "element_keys"sv);
        bool first_elem = true;
        for (size_t i = 0; i < m_element_keys_count; ++i) {
            if (!m_element_keys[i].has_value())
                continue;
            if (!first_elem)
                list_builder.append(", "sv);
            first_elem = false;
            list_builder.append(format_operand("element_keys"sv, m_element_keys[i].value(), executable));
        }
        list_builder.append(']');
        append_piece(list_builder.to_byte_string());
    }

    return builder.to_byte_string();
}

ByteString NewFunction::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewFunction"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("shared_function_data_index:{}", m_shared_function_data_index));

    if (m_lhs_name.has_value())
        append_piece(ByteString::formatted("lhs_name:{}", executable.identifier_table->get(m_lhs_name.value())));

    if (m_home_object.has_value())
        append_piece(format_operand("home_object"sv, m_home_object.value(), executable));

    return builder.to_byte_string();
}

ByteString NewObject::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewObject"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString NewObjectWithNoPrototype::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewObjectWithNoPrototype"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString NewPrimitiveArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewPrimitiveArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    if (m_element_count != 0)
        append_piece(format_value_list("elements"sv, ReadonlySpan<Value> { m_elements, m_element_count }));

    return builder.to_byte_string();
}

ByteString NewRegExp::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewRegExp"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(executable.get_string(m_source_index));

    append_piece(executable.get_string(m_flags_index));

    return builder.to_byte_string();
}

ByteString NewTypeError::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewTypeError"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(executable.get_string(m_error_string));

    return builder.to_byte_string();
}

ByteString NewReferenceError::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("NewReferenceError"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(executable.get_string(m_error_string));

    return builder.to_byte_string();
}

ByteString Not::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Not"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString PostfixDecrement::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PostfixDecrement"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString PostfixIncrement::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PostfixIncrement"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString PutById::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutById"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("kind:{}", put_kind_to_string(m_kind)));

    if (m_base_identifier.has_value())
        append_piece(ByteString::formatted("base_identifier:{}", executable.identifier_table->get(m_base_identifier.value())));

    return builder.to_byte_string();
}

ByteString PutByIdWithThis::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutByIdWithThis"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("kind:{}", put_kind_to_string(m_kind)));

    return builder.to_byte_string();
}

ByteString PutBySpread::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutBySpread"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString PutByValue::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutByValue"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("kind:{}", put_kind_to_string(m_kind)));

    if (m_base_identifier.has_value())
        append_piece(ByteString::formatted("base_identifier:{}", executable.identifier_table->get(m_base_identifier.value())));

    return builder.to_byte_string();
}

ByteString PutByValueWithThis::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutByValueWithThis"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(format_operand("property"sv, m_property, executable));

    append_piece(format_operand("this_value"sv, m_this_value, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("kind:{}", put_kind_to_string(m_kind)));

    return builder.to_byte_string();
}

ByteString PutPrivateById::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("PutPrivateById"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("base"sv, m_base, executable));

    append_piece(ByteString::formatted("property:{}", executable.identifier_table->get(m_property)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString ResolveSuperBase::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ResolveSuperBase"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    return builder.to_byte_string();
}

ByteString ResolveThisBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ResolveThisBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    return builder.to_byte_string();
}

ByteString Return::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Return"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString RightShift::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("RightShift"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString SetCompletionType::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SetCompletionType"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("completion"sv, m_completion, executable));

    return builder.to_byte_string();
}

ByteString SetGlobal::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SetGlobal"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString SetLexicalEnvironment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SetLexicalEnvironment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("environment"sv, m_environment, executable));

    return builder.to_byte_string();
}

ByteString SetLexicalBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SetLexicalBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString SetVariableBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SetVariableBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString StrictlyEquals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("StrictlyEquals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString StrictlyInequals::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("StrictlyInequals"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Sub::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Sub"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString SuperCallWithArgumentArray::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("SuperCallWithArgumentArray"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("arguments"sv, m_arguments, executable));

    append_piece(ByteString::formatted("is_synthetic:{}", m_is_synthetic));

    return builder.to_byte_string();
}

ByteString Throw::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Throw"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString ThrowIfNotObject::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ThrowIfNotObject"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString ThrowIfNullish::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ThrowIfNullish"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString ThrowIfTDZ::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ThrowIfTDZ"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString ThrowConstAssignment::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ThrowConstAssignment"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    return builder.to_byte_string();
}

ByteString ToBoolean::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToBoolean"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString ToLength::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToLength"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString ToObject::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("ToObject"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString Typeof::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Typeof"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString TypeofBinding::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("TypeofBinding"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(ByteString::formatted("identifier:{}", executable.identifier_table->get(m_identifier)));

    return builder.to_byte_string();
}

ByteString UnaryMinus::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("UnaryMinus"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString UnaryPlus::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("UnaryPlus"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("src"sv, m_src, executable));

    return builder.to_byte_string();
}

ByteString UnsignedRightShift::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("UnsignedRightShift"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("dst"sv, m_dst, executable));

    append_piece(format_operand("lhs"sv, m_lhs, executable));

    append_piece(format_operand("rhs"sv, m_rhs, executable));

    return builder.to_byte_string();
}

ByteString Yield::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("Yield"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    if (m_continuation_label.has_value())
        append_piece(ByteString::formatted("continuation_label:{}", m_continuation_label.value()));

    append_piece(format_operand("value"sv, m_value, executable));

    return builder.to_byte_string();
}

ByteString CacheObjectShape::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("CacheObjectShape"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("object"sv, m_object, executable));

    return builder.to_byte_string();
}

ByteString InitObjectLiteralProperty::to_byte_string_impl([[maybe_unused]] Bytecode::Executable const& executable) const
{
    StringBuilder builder;
    builder.append("InitObjectLiteralProperty"sv);

    bool first = true;
    [[maybe_unused]] auto append_piece = [&](auto const& piece) {
        if (first) {
            builder.append(' ');
            first = false;
        } else {
            builder.append(", "sv);
        }
        builder.append(piece);
    };

    append_piece(format_operand("object"sv, m_object, executable));

    append_piece(ByteString::formatted("property:{}", executable.property_key_table->get(m_property)));

    append_piece(format_operand("src"sv, m_src, executable));

    append_piece(ByteString::formatted("shape_cache_index:{}", m_shape_cache_index));

    append_piece(ByteString::formatted("property_slot:{}", m_property_slot));

    return builder.to_byte_string();
}

} // namespace JS::Bytecode::Op