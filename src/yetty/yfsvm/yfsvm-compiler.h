#pragma once

// yfsvm-compiler - Compiles YAST expressions to yfsvm bytecode
//
// Takes the AST from YAST parser and generates bytecode that can
// be executed by the yfsvm interpreter in the fragment shader.

#include "yfsvm.h"
#include "../yast/yast.h"

namespace yetty::yfsvm {

//=============================================================================
// Compiler
//=============================================================================

class Compiler {
public:
    // Compile a single expression to a program with one function
    Result<Program> compile(const yast::Node* ast);

    // Compile a single expression string
    Result<Program> compileExpression(std::string_view source);

    // Compile a multi-plot expression (multiple functions)
    Result<Program> compileMulti(const yast::PlotExprNode* ast);

    // Compile multi-plot expression string
    Result<Program> compileMultiExpression(std::string_view source);

private:
    InstructionBuilder _builder;

    // Compile a node, returns the register holding the result
    // Returns -1 on error (error stored in _error)
    Result<uint8_t> compileNode(const yast::Node* node);

    // Specific node types
    Result<uint8_t> compileNumber(const yast::NumberNode* node);
    Result<uint8_t> compileIdentifier(const yast::IdentifierNode* node);
    Result<uint8_t> compileBufferRef(const yast::BufferRefNode* node);
    Result<uint8_t> compileBinaryOp(const yast::BinaryOpNode* node);
    Result<uint8_t> compileUnaryOp(const yast::UnaryOpNode* node);
    Result<uint8_t> compileCall(const yast::CallNode* node);
};

//=============================================================================
// Convenience functions
//=============================================================================

// Compile expression string directly to bytecode
Result<Program> compileExpression(std::string_view source);

// Compile multi-plot expression string
Result<Program> compileMultiExpression(std::string_view source);

} // namespace yetty::yfsvm
