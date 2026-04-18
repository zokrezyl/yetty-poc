//=============================================================================
// YFSVM Tests
//
// Tests for the Yetty Fragment Shader Virtual Machine
//=============================================================================

// Include C++ standard headers before boost/ut.hpp
#include <cstddef>
#include <version>
#include <algorithm>
#include <cmath>
#include <array>

#include <boost/ut.hpp>
#include "yetty/yfsvm/yfsvm.h"
#include "yetty/yfsvm/yfsvm-compiler.h"

//=============================================================================
// CPU Evaluator for testing
// Mirrors GPU yfsvm_execute() for validation
//=============================================================================

namespace {

struct CPUEvaluator {
    std::array<float, 16> regs{};
    const float* constants = nullptr;
    const uint32_t* code = nullptr;
    float x = 0.0f;
    float t = 0.0f;
    std::array<float, 8> samplers{};

    float evaluate(const yetty::yfsvm::Program& prog, uint32_t funcIdx) {
        using namespace yetty::yfsvm;

        if (funcIdx >= prog.functions.size()) return 0.0f;

        constants = prog.constants.data();
        code = prog.code.data();

        const auto& func = prog.functions[funcIdx];
        uint32_t pc = func.codeOffset;
        uint32_t endPc = pc + func.codeLength;

        // Clear registers
        regs.fill(0.0f);

        while (pc < endPc) {
            Instruction instr(code[pc++]);
            auto op = instr.opcode();
            uint8_t dst = instr.dst();
            uint8_t src1 = instr.src1();
            uint8_t src2 = instr.src2();
            uint16_t imm = instr.imm12();

            switch (op) {
            case Opcode::NOP: break;
            case Opcode::RET: return regs[0];
            case Opcode::LOAD_C: regs[dst] = constants[imm]; break;
            case Opcode::LOAD_X: regs[dst] = x; break;
            case Opcode::LOAD_T: regs[dst] = t; break;
            case Opcode::LOAD_S: regs[dst] = samplers[imm & 7]; break;
            case Opcode::MOV: regs[dst] = regs[src1]; break;

            case Opcode::ADD: regs[dst] = regs[src1] + regs[src2]; break;
            case Opcode::SUB: regs[dst] = regs[src1] - regs[src2]; break;
            case Opcode::MUL: regs[dst] = regs[src1] * regs[src2]; break;
            case Opcode::DIV: regs[dst] = regs[src1] / regs[src2]; break;
            case Opcode::NEG: regs[dst] = -regs[src1]; break;
            case Opcode::MOD: regs[dst] = std::fmod(regs[src1], regs[src2]); break;

            case Opcode::SIN: regs[dst] = std::sin(regs[src1]); break;
            case Opcode::COS: regs[dst] = std::cos(regs[src1]); break;
            case Opcode::TAN: regs[dst] = std::tan(regs[src1]); break;
            case Opcode::ASIN: regs[dst] = std::asin(regs[src1]); break;
            case Opcode::ACOS: regs[dst] = std::acos(regs[src1]); break;
            case Opcode::ATAN: regs[dst] = std::atan(regs[src1]); break;
            case Opcode::ATAN2: regs[dst] = std::atan2(regs[src1], regs[src2]); break;
            case Opcode::SINH: regs[dst] = std::sinh(regs[src1]); break;
            case Opcode::COSH: regs[dst] = std::cosh(regs[src1]); break;
            case Opcode::TANH: regs[dst] = std::tanh(regs[src1]); break;

            case Opcode::EXP: regs[dst] = std::exp(regs[src1]); break;
            case Opcode::EXP2: regs[dst] = std::exp2(regs[src1]); break;
            case Opcode::LOG: regs[dst] = std::log(regs[src1]); break;
            case Opcode::LOG2: regs[dst] = std::log2(regs[src1]); break;
            case Opcode::POW: regs[dst] = std::pow(regs[src1], regs[src2]); break;
            case Opcode::SQRT: regs[dst] = std::sqrt(regs[src1]); break;
            case Opcode::RSQRT: regs[dst] = 1.0f / std::sqrt(regs[src1]); break;

            case Opcode::ABS: regs[dst] = std::abs(regs[src1]); break;
            case Opcode::MIN: regs[dst] = std::min(regs[src1], regs[src2]); break;
            case Opcode::MAX: regs[dst] = std::max(regs[src1], regs[src2]); break;
            case Opcode::FLOOR: regs[dst] = std::floor(regs[src1]); break;
            case Opcode::CEIL: regs[dst] = std::ceil(regs[src1]); break;
            case Opcode::ROUND: regs[dst] = std::round(regs[src1]); break;
            case Opcode::FRACT: regs[dst] = regs[src1] - std::floor(regs[src1]); break;
            case Opcode::SIGN: regs[dst] = (regs[src1] > 0.0f) ? 1.0f : ((regs[src1] < 0.0f) ? -1.0f : 0.0f); break;
            case Opcode::CLAMP01: regs[dst] = std::clamp(regs[src1], 0.0f, 1.0f); break;
            case Opcode::STEP: regs[dst] = (regs[src2] < regs[src1]) ? 0.0f : 1.0f; break;
            case Opcode::MIX: {
                float tVal = regs[imm & 0xF];
                regs[dst] = regs[src1] * (1.0f - tVal) + regs[src2] * tVal;
                break;
            }
            default: break;
            }
        }
        return regs[0];
    }
};

// Helper to evaluate an expression at a given x value
float evalExpr(const std::string& expr, float xVal, float tVal = 0.0f) {
    auto result = yetty::yfsvm::compileExpression(expr);
    if (!result) return std::numeric_limits<float>::quiet_NaN();

    CPUEvaluator eval;
    eval.x = xVal;
    eval.t = tVal;
    return eval.evaluate(*result, 0);
}

// Helper for multi-expression evaluation
float evalMultiExpr(const std::string& expr, uint32_t funcIdx, float xVal, float tVal = 0.0f) {
    auto result = yetty::yfsvm::compileMultiExpression(expr);
    if (!result) return std::numeric_limits<float>::quiet_NaN();

    CPUEvaluator eval;
    eval.x = xVal;
    eval.t = tVal;
    return eval.evaluate(*result, funcIdx);
}

} // anonymous namespace

using namespace boost::ut;
using namespace yetty::yfsvm;

//=============================================================================
// Instruction encoding tests
//=============================================================================

suite instruction_tests = [] {
    "instruction encodes opcode"_test = [] {
        auto instr = Instruction::encode(Opcode::ADD, 0, 1, 2);
        expect(instr.opcode() == Opcode::ADD);
    };

    "instruction encodes dst register"_test = [] {
        auto instr = Instruction::encode(Opcode::ADD, 5, 1, 2);
        expect(instr.dst() == 5_u);
    };

    "instruction encodes src1 register"_test = [] {
        auto instr = Instruction::encode(Opcode::ADD, 0, 7, 2);
        expect(instr.src1() == 7_u);
    };

    "instruction encodes src2 register"_test = [] {
        auto instr = Instruction::encode(Opcode::ADD, 0, 1, 9);
        expect(instr.src2() == 9_u);
    };

    "instruction encodes imm12"_test = [] {
        auto instr = Instruction::encode(Opcode::LOAD_C, 3, 0, 0, 1234);
        expect(instr.imm12() == 1234_u);
    };

    "instruction masks dst to 4 bits"_test = [] {
        auto instr = Instruction::encode(Opcode::ADD, 0xFF, 0, 0);
        expect(instr.dst() == 15_u);  // Only lower 4 bits (0xF)
    };

    "instruction masks imm12 to 12 bits"_test = [] {
        auto instr = Instruction::encode(Opcode::LOAD_C, 0, 0, 0, 0xFFFF);
        expect(instr.imm12() == 4095_u);  // Only lower 12 bits (0xFFF)
    };
};

//=============================================================================
// InstructionBuilder tests
//=============================================================================

suite builder_tests = [] {
    "builder allocates registers"_test = [] {
        InstructionBuilder builder;
        uint8_t r0 = builder.allocReg();
        uint8_t r1 = builder.allocReg();
        uint8_t r2 = builder.allocReg();
        expect(r0 != r1);
        expect(r1 != r2);
        expect(r0 != r2);
    };

    "builder frees registers"_test = [] {
        InstructionBuilder builder;
        uint8_t r0 = builder.allocReg();
        builder.freeReg(r0);
        uint8_t r1 = builder.allocReg();
        expect(r0 == r1);  // Should reuse freed register
    };

    "builder creates program with constants"_test = [] {
        InstructionBuilder builder;
        builder.beginFunction();
        builder.loadConst(3.14f);
        builder.ret();
        builder.endFunction();
        auto prog = builder.build();
        expect(prog.constants.size() >= 1_u);
        expect(std::abs(prog.constants[0] - 3.14f) < 0.001f);
    };

    "builder deduplicates constants"_test = [] {
        InstructionBuilder builder;
        builder.beginFunction();
        builder.loadConst(1.0f);
        builder.loadConst(1.0f);  // Same value
        builder.loadConst(2.0f);  // Different value
        builder.ret();
        builder.endFunction();
        auto prog = builder.build();
        expect(prog.constants.size() == 2_u);  // Only 2 unique constants
    };

    "builder generates code"_test = [] {
        InstructionBuilder builder;
        builder.beginFunction();
        uint8_t r0 = builder.allocReg();
        builder.loadX(r0);
        builder.sin(r0, r0);
        builder.ret();
        builder.endFunction();
        auto prog = builder.build();
        expect(prog.code.size() >= 2_u);
        expect(prog.functions.size() == 1_u);
    };

    "builder tracks multiple functions"_test = [] {
        InstructionBuilder builder;

        builder.beginFunction();
        uint8_t r = builder.allocReg();
        builder.loadX(r);
        builder.sin(r, r);
        builder.ret();
        builder.endFunction();

        builder.beginFunction();
        r = builder.allocReg();
        builder.loadX(r);
        builder.cos(r, r);
        builder.ret();
        builder.endFunction();

        auto prog = builder.build();
        expect(prog.functions.size() == 2_u);
    };
};

//=============================================================================
// Program serialization tests
//=============================================================================

suite serialization_tests = [] {
    "program serializes and deserializes"_test = [] {
        InstructionBuilder builder;
        builder.beginFunction();
        builder.loadConst(42.0f);
        builder.ret();
        builder.endFunction();
        auto prog = builder.build();

        auto buffer = prog.serialize();
        auto result = Program::deserialize(buffer);
        expect(result.has_value());
        expect(result->constants.size() == prog.constants.size());
        expect(result->code.size() == prog.code.size());
        expect(result->functions.size() == prog.functions.size());
    };

    "program deserialize rejects invalid magic"_test = [] {
        std::vector<uint32_t> buffer(100, 0);
        buffer[0] = 0xDEADBEEF;  // Wrong magic
        auto result = Program::deserialize(buffer);
        expect(!result.has_value());
    };

    "program deserialize rejects small buffer"_test = [] {
        std::vector<uint32_t> buffer(2, 0);  // Too small
        auto result = Program::deserialize(buffer);
        expect(!result.has_value());
    };
};

//=============================================================================
// Compiler tests
//=============================================================================

suite compiler_tests = [] {
    "compiler compiles number"_test = [] {
        auto result = compileExpression("42");
        expect(result.has_value());
        expect(result->functions.size() == 1_u);
        expect(result->constants.size() >= 1_u);
    };

    "compiler compiles x variable"_test = [] {
        auto result = compileExpression("x");
        expect(result.has_value());
        expect(result->usesX);
    };

    "compiler compiles time variable"_test = [] {
        auto result = compileExpression("time");
        expect(result.has_value());
        expect(result->usesTime);
    };

    "compiler compiles pi constant"_test = [] {
        auto result = compileExpression("pi");
        expect(result.has_value());
        bool foundPi = false;
        for (float c : result->constants) {
            if (std::abs(c - 3.14159f) < 0.001f) {
                foundPi = true;
                break;
            }
        }
        expect(foundPi);
    };

    "compiler compiles sin function"_test = [] {
        auto result = compileExpression("sin(x)");
        expect(result.has_value());
        expect(result->usesX);
    };

    "compiler compiles cos function"_test = [] {
        auto result = compileExpression("cos(x)");
        expect(result.has_value());
    };

    "compiler compiles addition"_test = [] {
        auto result = compileExpression("x + 1");
        expect(result.has_value());
    };

    "compiler compiles subtraction"_test = [] {
        auto result = compileExpression("x - 1");
        expect(result.has_value());
    };

    "compiler compiles multiplication"_test = [] {
        auto result = compileExpression("x * 2");
        expect(result.has_value());
    };

    "compiler compiles division"_test = [] {
        auto result = compileExpression("x / 2");
        expect(result.has_value());
    };

    "compiler compiles power"_test = [] {
        auto result = compileExpression("x ^ 2");
        expect(result.has_value());
    };

    "compiler compiles unary minus"_test = [] {
        auto result = compileExpression("-x");
        expect(result.has_value());
    };

    "compiler compiles complex expression"_test = [] {
        auto result = compileExpression("sin(x) + cos(2 * x) / 3");
        expect(result.has_value());
        expect(result->usesX);
    };

    "compiler compiles nested functions"_test = [] {
        auto result = compileExpression("sin(cos(x))");
        expect(result.has_value());
    };

    "compiler compiles pow function"_test = [] {
        auto result = compileExpression("pow(x, 2)");
        expect(result.has_value());
    };

    "compiler compiles sqrt function"_test = [] {
        auto result = compileExpression("sqrt(x)");
        expect(result.has_value());
    };

    "compiler compiles abs function"_test = [] {
        auto result = compileExpression("abs(x)");
        expect(result.has_value());
    };

    "compiler compiles min function"_test = [] {
        auto result = compileExpression("min(x, 0)");
        expect(result.has_value());
    };

    "compiler compiles max function"_test = [] {
        auto result = compileExpression("max(x, 0)");
        expect(result.has_value());
    };

    "compiler compiles clamp function"_test = [] {
        auto result = compileExpression("clamp(x, 0, 1)");
        expect(result.has_value());
    };

    "compiler compiles mix function"_test = [] {
        auto result = compileExpression("mix(0, 1, x)");
        expect(result.has_value());
    };

    "compiler compiles sampler reference"_test = [] {
        auto result = compileExpression("s0");
        expect(result.has_value());
    };

    "compiler compiles buffer reference"_test = [] {
        auto result = compileExpression("@buffer1");
        expect(result.has_value());
        expect(result->bufferRefs.size() == 1_u);
        expect(result->bufferRefs[0] == 1_i);
    };

    "compiler rejects unknown function"_test = [] {
        auto result = compileExpression("unknown_func(x)");
        expect(!result.has_value());
    };

    "compiler rejects unknown identifier"_test = [] {
        auto result = compileExpression("unknown_var");
        expect(!result.has_value());
    };
};

//=============================================================================
// Multi-expression compiler tests
//=============================================================================

suite multi_compiler_tests = [] {
    "compiler compiles multi-plot expression"_test = [] {
        auto result = compileMultiExpression("f1 = sin(x); f2 = cos(x)");
        expect(result.has_value());
        expect(result->functions.size() == 2_u);
    };

    "compiler compiles single plot in multi mode"_test = [] {
        auto result = compileMultiExpression("wave = sin(x)");
        expect(result.has_value());
        expect(result->functions.size() == 1_u);
    };
};

//=============================================================================
// Disassembler tests
//=============================================================================

suite disassembler_tests = [] {
    "disassemble produces output"_test = [] {
        auto result = compileExpression("sin(x) + 1");
        expect(result.has_value());
        auto dis = result->disassemble();
        expect(!dis.empty());
        expect(dis.find("function_0") != std::string::npos);
    };

    "disassemble shows constants"_test = [] {
        auto result = compileExpression("3.14");
        expect(result.has_value());
        auto dis = result->disassemble();
        expect(dis.find("3.14") != std::string::npos);
    };

    "disassemble shows opcodes"_test = [] {
        auto result = compileExpression("sin(x)");
        expect(result.has_value());
        auto dis = result->disassemble();
        expect(dis.find("LOAD_X") != std::string::npos);
        expect(dis.find("SIN") != std::string::npos);
        expect(dis.find("RET") != std::string::npos);
    };
};

//=============================================================================
// CPU evaluation tests - verify actual computation results
//=============================================================================

suite cpu_evaluation_tests = [] {
    "eval constant returns constant"_test = [] {
        float result = evalExpr("42", 0.0f);
        expect(std::abs(result - 42.0f) < 0.001f);
    };

    "eval x returns x value"_test = [] {
        float result = evalExpr("x", 3.5f);
        expect(std::abs(result - 3.5f) < 0.001f);
    };

    "eval sin(0) returns 0"_test = [] {
        float result = evalExpr("sin(x)", 0.0f);
        expect(std::abs(result - 0.0f) < 0.001f);
    };

    "eval sin(pi/2) returns 1"_test = [] {
        float result = evalExpr("sin(x)", 3.14159265f / 2.0f);
        expect(std::abs(result - 1.0f) < 0.001f);
    };

    "eval cos(0) returns 1"_test = [] {
        float result = evalExpr("cos(x)", 0.0f);
        expect(std::abs(result - 1.0f) < 0.001f);
    };

    "eval cos(pi) returns -1"_test = [] {
        float result = evalExpr("cos(x)", 3.14159265f);
        expect(std::abs(result - (-1.0f)) < 0.001f);
    };

    "eval addition"_test = [] {
        float result = evalExpr("x + 5", 3.0f);
        expect(std::abs(result - 8.0f) < 0.001f);
    };

    "eval subtraction"_test = [] {
        float result = evalExpr("x - 2", 5.0f);
        expect(std::abs(result - 3.0f) < 0.001f);
    };

    "eval multiplication"_test = [] {
        float result = evalExpr("x * 3", 4.0f);
        expect(std::abs(result - 12.0f) < 0.001f);
    };

    "eval division"_test = [] {
        float result = evalExpr("x / 2", 10.0f);
        expect(std::abs(result - 5.0f) < 0.001f);
    };

    "eval negation"_test = [] {
        float result = evalExpr("-x", 7.0f);
        expect(std::abs(result - (-7.0f)) < 0.001f);
    };

    "eval sqrt"_test = [] {
        float result = evalExpr("sqrt(x)", 16.0f);
        expect(std::abs(result - 4.0f) < 0.001f);
    };

    "eval exp"_test = [] {
        float result = evalExpr("exp(x)", 0.0f);
        expect(std::abs(result - 1.0f) < 0.001f);
    };

    "eval log"_test = [] {
        float result = evalExpr("log(x)", 2.71828182f);
        expect(std::abs(result - 1.0f) < 0.01f);
    };

    "eval abs positive"_test = [] {
        float result = evalExpr("abs(x)", 5.0f);
        expect(std::abs(result - 5.0f) < 0.001f);
    };

    "eval abs negative"_test = [] {
        float result = evalExpr("abs(x)", -5.0f);
        expect(std::abs(result - 5.0f) < 0.001f);
    };

    "eval complex expression"_test = [] {
        // sin(x) + cos(x) at x=0 should be 0 + 1 = 1
        float result = evalExpr("sin(x) + cos(x)", 0.0f);
        expect(std::abs(result - 1.0f) < 0.001f);
    };

    "eval nested functions"_test = [] {
        // sin(cos(0)) = sin(1) ~ 0.841
        float result = evalExpr("sin(cos(x))", 0.0f);
        expect(std::abs(result - std::sin(1.0f)) < 0.001f);
    };

    "eval power"_test = [] {
        float result = evalExpr("x^2", 3.0f);
        expect(std::abs(result - 9.0f) < 0.001f);
    };

    "eval pow function"_test = [] {
        float result = evalExpr("pow(x, 3)", 2.0f);
        expect(std::abs(result - 8.0f) < 0.001f);
    };

    "eval floor"_test = [] {
        float result = evalExpr("floor(x)", 3.7f);
        expect(std::abs(result - 3.0f) < 0.001f);
    };

    "eval ceil"_test = [] {
        float result = evalExpr("ceil(x)", 3.2f);
        expect(std::abs(result - 4.0f) < 0.001f);
    };

    "eval min"_test = [] {
        float result = evalExpr("min(x, 5)", 3.0f);
        expect(std::abs(result - 3.0f) < 0.001f);
    };

    "eval max"_test = [] {
        float result = evalExpr("max(x, 5)", 3.0f);
        expect(std::abs(result - 5.0f) < 0.001f);
    };

    "eval time variable"_test = [] {
        float result = evalExpr("time", 0.0f, 2.5f);
        expect(std::abs(result - 2.5f) < 0.001f);
    };

    "eval expression with time"_test = [] {
        float result = evalExpr("sin(x + time)", 0.0f, 3.14159265f / 2.0f);
        expect(std::abs(result - 1.0f) < 0.001f);
    };
};

//=============================================================================
// Multi-function evaluation tests
//=============================================================================

suite multi_function_eval_tests = [] {
    "multi-expr compiles two functions"_test = [] {
        auto result = compileMultiExpression("f = sin(x); g = cos(x)");
        expect(result.has_value());
        expect(result->functions.size() == 2_u);
    };

    "multi-expr first function evaluates correctly"_test = [] {
        float result = evalMultiExpr("f = sin(x); g = cos(x)", 0, 0.0f);
        expect(std::abs(result - 0.0f) < 0.001f);  // sin(0) = 0
    };

    "multi-expr second function evaluates correctly"_test = [] {
        float result = evalMultiExpr("f = sin(x); g = cos(x)", 1, 0.0f);
        expect(std::abs(result - 1.0f) < 0.001f);  // cos(0) = 1
    };

    "multi-expr three functions"_test = [] {
        auto result = compileMultiExpression("f = x; g = x*2; h = x*3");
        expect(result.has_value());
        expect(result->functions.size() == 3_u);

        CPUEvaluator eval;
        eval.x = 2.0f;

        float f_result = eval.evaluate(*result, 0);
        float g_result = eval.evaluate(*result, 1);
        float h_result = eval.evaluate(*result, 2);

        expect(std::abs(f_result - 2.0f) < 0.001f);
        expect(std::abs(g_result - 4.0f) < 0.001f);
        expect(std::abs(h_result - 6.0f) < 0.001f);
    };

    "multi-expr complex functions"_test = [] {
        auto result = compileMultiExpression("f = sin(x)*cos(x); g = x^2 - 1");
        expect(result.has_value());

        CPUEvaluator eval;
        eval.x = 1.0f;

        float f_result = eval.evaluate(*result, 0);
        float g_result = eval.evaluate(*result, 1);

        expect(std::abs(f_result - std::sin(1.0f)*std::cos(1.0f)) < 0.001f);
        expect(std::abs(g_result - 0.0f) < 0.001f);  // 1^2 - 1 = 0
    };
};

//=============================================================================
// Serialization round-trip tests
//=============================================================================

suite serialization_roundtrip_tests = [] {
    "roundtrip preserves evaluation result"_test = [] {
        auto original = compileExpression("sin(x) + cos(x) * 2");
        expect(original.has_value());

        auto buffer = original->serialize();
        auto restored = Program::deserialize(buffer);
        expect(restored.has_value());

        CPUEvaluator eval1, eval2;
        eval1.x = 1.5f;
        eval2.x = 1.5f;

        float orig_result = eval1.evaluate(*original, 0);
        float rest_result = eval2.evaluate(*restored, 0);

        expect(std::abs(orig_result - rest_result) < 0.0001f);
    };

    "roundtrip multi-function preserves all functions"_test = [] {
        auto original = compileMultiExpression("f = sin(x); g = cos(x); h = tan(x)");
        expect(original.has_value());
        expect(original->functions.size() == 3_u);

        auto buffer = original->serialize();
        auto restored = Program::deserialize(buffer);
        expect(restored.has_value());
        expect(restored->functions.size() == 3_u);

        for (uint32_t i = 0; i < 3; ++i) {
            CPUEvaluator eval1, eval2;
            eval1.x = 0.5f;
            eval2.x = 0.5f;

            float orig_result = eval1.evaluate(*original, i);
            float rest_result = eval2.evaluate(*restored, i);

            expect(std::abs(orig_result - rest_result) < 0.0001f);
        }
    };
};
