#include "yfsvm-compiler.h"
#include <cmath>

namespace yetty::yfsvm {

//=============================================================================
// Compiler - node compilation
//=============================================================================

Result<uint8_t> Compiler::compileNumber(const yast::NumberNode* node) {
    uint8_t reg = _builder.loadConst(static_cast<float>(node->value));
    return Ok(reg);
}

Result<uint8_t> Compiler::compileIdentifier(const yast::IdentifierNode* node) {
    const auto& name = node->name;

    // Check for x (domain value)
    if (name == "x") {
        uint8_t reg = _builder.allocReg();
        _builder.loadX(reg);
        _builder.markUsesX();
        return Ok(reg);
    }

    // Check for time
    if (name == "t" || name == "time") {
        uint8_t reg = _builder.allocReg();
        _builder.loadT(reg);
        _builder.markUsesTime();
        return Ok(reg);
    }

    // Check for common constants
    if (name == "pi" || name == "PI") {
        return Ok(_builder.loadConst(3.14159265358979323846f));
    }
    if (name == "e" || name == "E") {
        return Ok(_builder.loadConst(2.71828182845904523536f));
    }
    if (name == "tau" || name == "TAU") {
        return Ok(_builder.loadConst(6.28318530717958647693f));
    }

    // Check for sampler inputs (s0-s7)
    if (name.size() == 2 && name[0] == 's' && name[1] >= '0' && name[1] <= '7') {
        uint8_t idx = name[1] - '0';
        uint8_t reg = _builder.allocReg();
        _builder.loadSampler(reg, idx);
        return Ok(reg);
    }

    return Err<uint8_t>("unknown identifier: " + name);
}

Result<uint8_t> Compiler::compileBufferRef(const yast::BufferRefNode* node) {
    // Buffer references map to sampler slots
    // @buffer1 -> s0, @buffer2 -> s1, etc.
    int samplerIdx = node->index - 1;  // buffer1 = s0
    if (samplerIdx < 0 || samplerIdx > 7) {
        return Err<uint8_t>("buffer index out of range: " + std::to_string(node->index));
    }

    _builder.addBufferRef(node->index);
    uint8_t reg = _builder.allocReg();
    _builder.loadSampler(reg, static_cast<uint8_t>(samplerIdx));
    return Ok(reg);
}

Result<uint8_t> Compiler::compileBinaryOp(const yast::BinaryOpNode* node) {
    auto leftRes = compileNode(node->left.get());
    if (!leftRes) return leftRes;
    uint8_t left = *leftRes;

    auto rightRes = compileNode(node->right.get());
    if (!rightRes) {
        _builder.freeReg(left);
        return rightRes;
    }
    uint8_t right = *rightRes;

    uint8_t dst = _builder.allocReg();

    switch (node->op) {
        case yast::BinaryOpNode::Op::Add:
            _builder.add(dst, left, right);
            break;
        case yast::BinaryOpNode::Op::Sub:
            _builder.sub(dst, left, right);
            break;
        case yast::BinaryOpNode::Op::Mul:
            _builder.mul(dst, left, right);
            break;
        case yast::BinaryOpNode::Op::Div:
            _builder.div(dst, left, right);
            break;
        case yast::BinaryOpNode::Op::Pow:
            _builder.pow(dst, left, right);
            break;
    }

    _builder.freeReg(left);
    _builder.freeReg(right);
    return Ok(dst);
}

Result<uint8_t> Compiler::compileUnaryOp(const yast::UnaryOpNode* node) {
    auto operandRes = compileNode(node->operand.get());
    if (!operandRes) return operandRes;
    uint8_t operand = *operandRes;

    uint8_t dst = _builder.allocReg();

    switch (node->op) {
        case yast::UnaryOpNode::Op::Neg:
            _builder.neg(dst, operand);
            break;
    }

    _builder.freeReg(operand);
    return Ok(dst);
}

Result<uint8_t> Compiler::compileCall(const yast::CallNode* node) {
    const auto& name = node->name;
    const auto& args = node->args;

    // Single-argument functions
    if (args.size() == 1) {
        auto argRes = compileNode(args[0].get());
        if (!argRes) return argRes;
        uint8_t arg = *argRes;
        uint8_t dst = _builder.allocReg();

        if (name == "sin") { _builder.sin(dst, arg); }
        else if (name == "cos") { _builder.cos(dst, arg); }
        else if (name == "tan") { _builder.tan(dst, arg); }
        else if (name == "asin") { _builder.asin(dst, arg); }
        else if (name == "acos") { _builder.acos(dst, arg); }
        else if (name == "atan") { _builder.atan(dst, arg); }
        else if (name == "sinh") { _builder.sinh(dst, arg); }
        else if (name == "cosh") { _builder.cosh(dst, arg); }
        else if (name == "tanh") { _builder.tanh(dst, arg); }
        else if (name == "exp") { _builder.exp(dst, arg); }
        else if (name == "exp2") { _builder.exp2(dst, arg); }
        else if (name == "log" || name == "ln") { _builder.log(dst, arg); }
        else if (name == "log2") { _builder.log2(dst, arg); }
        else if (name == "sqrt") { _builder.sqrt(dst, arg); }
        else if (name == "rsqrt" || name == "inverseSqrt") { _builder.rsqrt(dst, arg); }
        else if (name == "abs") { _builder.abs(dst, arg); }
        else if (name == "floor") { _builder.floor(dst, arg); }
        else if (name == "ceil") { _builder.ceil(dst, arg); }
        else if (name == "round") { _builder.round(dst, arg); }
        else if (name == "fract" || name == "frac") { _builder.fract(dst, arg); }
        else if (name == "sign") { _builder.sign(dst, arg); }
        else if (name == "saturate") { _builder.clamp01(dst, arg); }
        else {
            _builder.freeReg(arg);
            _builder.freeReg(dst);
            return Err<uint8_t>("unknown function: " + name);
        }

        _builder.freeReg(arg);
        return Ok(dst);
    }

    // Two-argument functions
    if (args.size() == 2) {
        auto arg0Res = compileNode(args[0].get());
        if (!arg0Res) return arg0Res;
        uint8_t arg0 = *arg0Res;

        auto arg1Res = compileNode(args[1].get());
        if (!arg1Res) {
            _builder.freeReg(arg0);
            return arg1Res;
        }
        uint8_t arg1 = *arg1Res;

        uint8_t dst = _builder.allocReg();

        if (name == "pow") { _builder.pow(dst, arg0, arg1); }
        else if (name == "atan2") { _builder.atan2(dst, arg0, arg1); }
        else if (name == "min") { _builder.min(dst, arg0, arg1); }
        else if (name == "max") { _builder.max(dst, arg0, arg1); }
        else if (name == "mod" || name == "fmod") { _builder.mod(dst, arg0, arg1); }
        else if (name == "step") { _builder.step(dst, arg0, arg1); }
        else {
            _builder.freeReg(arg0);
            _builder.freeReg(arg1);
            _builder.freeReg(dst);
            return Err<uint8_t>("unknown function: " + name);
        }

        _builder.freeReg(arg0);
        _builder.freeReg(arg1);
        return Ok(dst);
    }

    // Three-argument functions
    if (args.size() == 3) {
        auto arg0Res = compileNode(args[0].get());
        if (!arg0Res) return arg0Res;
        uint8_t arg0 = *arg0Res;

        auto arg1Res = compileNode(args[1].get());
        if (!arg1Res) {
            _builder.freeReg(arg0);
            return arg1Res;
        }
        uint8_t arg1 = *arg1Res;

        auto arg2Res = compileNode(args[2].get());
        if (!arg2Res) {
            _builder.freeReg(arg0);
            _builder.freeReg(arg1);
            return arg2Res;
        }
        uint8_t arg2 = *arg2Res;

        uint8_t dst = _builder.allocReg();

        if (name == "mix" || name == "lerp") {
            _builder.mix(dst, arg0, arg1, arg2);
        }
        else if (name == "clamp") {
            // clamp(x, lo, hi) = min(max(x, lo), hi)
            uint8_t tmp = _builder.allocReg();
            _builder.max(tmp, arg0, arg1);  // tmp = max(x, lo)
            _builder.min(dst, tmp, arg2);   // dst = min(tmp, hi)
            _builder.freeReg(tmp);
        }
        else {
            _builder.freeReg(arg0);
            _builder.freeReg(arg1);
            _builder.freeReg(arg2);
            _builder.freeReg(dst);
            return Err<uint8_t>("unknown function: " + name);
        }

        _builder.freeReg(arg0);
        _builder.freeReg(arg1);
        _builder.freeReg(arg2);
        return Ok(dst);
    }

    return Err<uint8_t>("function " + name + " expects different argument count");
}

Result<uint8_t> Compiler::compileNode(const yast::Node* node) {
    if (node->isNumber()) {
        return compileNumber(node->as<yast::NumberNode>());
    }
    if (node->isIdentifier()) {
        return compileIdentifier(node->as<yast::IdentifierNode>());
    }
    if (node->isBufferRef()) {
        return compileBufferRef(node->as<yast::BufferRefNode>());
    }
    if (node->isBinaryOp()) {
        return compileBinaryOp(node->as<yast::BinaryOpNode>());
    }
    if (node->isUnaryOp()) {
        return compileUnaryOp(node->as<yast::UnaryOpNode>());
    }
    if (node->isCall()) {
        return compileCall(node->as<yast::CallNode>());
    }

    return Err<uint8_t>("unsupported node type");
}

//=============================================================================
// Compiler - main entry points
//=============================================================================

Result<Program> Compiler::compile(const yast::Node* ast) {
    if (!ast) {
        return Err<Program>("null AST");
    }

    _builder.beginFunction();

    auto resultReg = compileNode(ast);
    if (!resultReg) {
        return Err<Program>("compilation failed", resultReg);
    }

    // Move result to r0 if not already there
    if (*resultReg != 0) {
        _builder.mov(0, *resultReg);
        _builder.freeReg(*resultReg);
    }

    _builder.ret();
    _builder.endFunction();

    return Ok(_builder.build());
}

Result<Program> Compiler::compileExpression(std::string_view source) {
    auto ast = yast::parseExpression(source);
    if (!ast) {
        return Err<Program>("failed to parse expression");
    }
    return compile(ast.get());
}

Result<Program> Compiler::compileMulti(const yast::PlotExprNode* ast) {
    if (!ast) {
        return Err<Program>("null AST");
    }

    for (const auto& def : ast->definitions) {
        _builder.beginFunction();

        auto resultReg = compileNode(def->expression.get());
        if (!resultReg) {
            return Err<Program>("compilation failed for function: " + def->name, resultReg);
        }

        // Move result to r0 if not already there
        if (*resultReg != 0) {
            _builder.mov(0, *resultReg);
            _builder.freeReg(*resultReg);
        }

        _builder.ret();
        _builder.endFunction();
    }

    return Ok(_builder.build());
}

Result<Program> Compiler::compileMultiExpression(std::string_view source) {
    auto ast = yast::parsePlotExpression(source);
    if (!ast) {
        return Err<Program>("failed to parse multi-plot expression");
    }
    return compileMulti(ast.get());
}

//=============================================================================
// Convenience functions
//=============================================================================

Result<Program> compileExpression(std::string_view source) {
    Compiler compiler;
    return compiler.compileExpression(source);
}

Result<Program> compileMultiExpression(std::string_view source) {
    Compiler compiler;
    return compiler.compileMultiExpression(source);
}

} // namespace yetty::yfsvm
